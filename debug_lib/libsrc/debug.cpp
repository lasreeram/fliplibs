#include <errno.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <exception>
#include <debug.h>
#include <sys/time.h>
#include <map>
#include <pthread.h>

namespace debug_lib{
        static bool use_syslog = false;
        static bool debug_off = false;
	static bool multiThreaded = false;
	static bool _initialized = false;
	static char* program_name;
	static pthread_mutex_t gMutex;
	struct compareThreads{
			bool operator()( const pthread_t& lhs, const pthread_t& rhs ) const{
				//int ret = pthread_equal( lhs, rhs );
				//bool flag = ret;
				//printf( "****inside compare function**** return value %s, ret = %d\n", flag? "true":"false", ret );
				bool flag = memcmp( &lhs, &rhs, sizeof(pthread_t) );
				return flag;
			}
	};
	static std::map<pthread_t, std::string> gThreadNames;

	static int lockMutex(){
		if ( !multiThreaded )
			return 0;
		int ret = pthread_mutex_lock(&gMutex);
		if ( ret != 0 ){
                        syslog(LOG_USER|LOG_CRIT, "error %d locking mutex", ret );
			exit(-1);
		}
		return 0;
	}

	static int unlockMutex(){
		if ( !multiThreaded )
			return 0;
		int ret = pthread_mutex_unlock(&gMutex);
		if ( ret != 0 ){
                        syslog(LOG_USER|LOG_CRIT, "error %d un-locking mutex", ret );
			exit(-1);
		}
		return 0;
	}

	void init(pthread_t id, const char* threadname ){
		multiThreaded = true;
		lockMutex();
		if ( !_initialized ){
			int ret = pthread_mutex_init( &gMutex, NULL);
			if ( ret != 0 )
				syslog(LOG_USER|LOG_CRIT, "error %d initializing mutex", ret );
			_initialized = true;
		}
		std::string thread_name = threadname;
		//printf( "setting name in map %ld, %s\n", id, thread_name.c_str() );
		gThreadNames.insert(make_pair(id, thread_name));
		unlockMutex();
	}

	void init(char* argv0, bool isMultiThreaded){
		multiThreaded = isMultiThreaded;
		_initialized = false;
		if( multiThreaded ){
			init( pthread_self(), argv0 );
		}else{
               		( debug_lib::program_name = strrchr( argv0, '/' ) )? debug_lib::program_name++: (debug_lib::program_name = argv0);
		}
        }

	static const char* getThreadName(){
		if( !multiThreaded )
			return program_name;

		static char thread_name[64] = {0};
		pthread_t tid = pthread_self();
		std::map<pthread_t, std::string>::iterator it = gThreadNames.find( tid );
		//printf( "getting name in map %ld\n", tid);
		if ( it != gThreadNames.end() ){
			memset( thread_name, 0x00, sizeof(thread_name) );
			unsigned int len = strlen( it->second.c_str() );
			if ( len >= sizeof(thread_name) )
				len = sizeof(thread_name) - 1;
			strncpy( thread_name, it->second.c_str(), len );
			printf( "got name %s from map for thread %ld\n", it->second.c_str(), tid);
		}
		return thread_name;
	}

        void logDebugInSyslog(){
                use_syslog = true;
        }

        void logDebugInStdOut(){
                use_syslog = false;
        }

	char* getCurrentTime(){
		static char time_buf[256] = {0};
		struct tm* timeinfo;
		//use micro second	
		struct timeval curTime;
		gettimeofday( &curTime, NULL );
		timeinfo = localtime( &curTime.tv_sec );
		sprintf( time_buf, "%04d:%02d:%02d %02d:%02d:%02d.%06ld", 
				1900+timeinfo->tm_year, 1+timeinfo->tm_mon, timeinfo->tm_mday,
				timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,
				curTime.tv_usec );
		return time_buf;
	}

        void throw_fatal_error( const char* format, ... ){
                char buf[2048] = {0};
                va_list ap;
                va_start(ap, format);
                vsprintf(buf, format, ap);
		lockMutex();
                if ( use_syslog ){
                        syslog(LOG_USER|LOG_CRIT, "%s", buf );
                }else{
                        if ( errno )
                                printf("%s %s: error type = %d, errno = %d(%s), error = %s\n",
                                                getThreadName(), getCurrentTime(), LOG_CRIT, errno, strerror(errno), buf );
                        else
                                printf("%s %s: error type = %d, error = %s\n", getThreadName(), getCurrentTime(), LOG_CRIT, buf );
                }
		unlockMutex();
                //vsyslog(LOG_USER|err, format, ap );
                va_end(ap);
                Exception ex(buf);
                throw ex;
        }

        void throw_error( const char* format, ... ){
                char buf[2048] = {0};
                va_list ap;
                va_start(ap, format);
                vsprintf(buf, format, ap);
		lockMutex();
                if ( use_syslog ){
                        syslog(LOG_USER|LOG_ERR, "%s", buf );
                }else{
                        if ( errno )
                                printf("%s %s: error type = %d, errno = %d(%s), error = %s\n",
                                                        getThreadName(), getCurrentTime(), LOG_ERR, errno, strerror(errno), buf );
                        else
                                printf("%s %s: error type = %d, error = %s\n", getThreadName(), getCurrentTime(), LOG_ERR, buf );
                }
		unlockMutex();
                //vsyslog(LOG_USER|err, format, ap );
                va_end(ap);
                Exception ex(buf);
                throw ex;
        }

        void log_no_newline( const char* format, ... ){
                if ( debug_off )
                        return;
                char buf[2048] = {0};
                va_list ap;
                va_start(ap, format);
                vsprintf(buf, format, ap);
		lockMutex();
                if ( use_syslog )
                        syslog(LOG_USER|LOG_INFO, "%s", buf );
                else
                        printf("%s", buf );
		unlockMutex();
                //vsyslog(LOG_USER|err, format, ap );
                va_end(ap);
        }

        void log( const char* format, ... ){
                if ( debug_off )
                        return;
                char buf[2048] = {0};
                va_list ap;
                va_start(ap, format);
                vsprintf(buf, format, ap);
		lockMutex();
                if ( use_syslog )
                        syslog(LOG_USER|LOG_INFO, "%s", buf );
                else
                        printf("%s %s: %s\n", getThreadName(), getCurrentTime(), buf );
		unlockMutex();
                //vsyslog(LOG_USER|err, format, ap );
                va_end(ap);
        }

	void lograw( const unsigned char* raw, int rawlen ){
		int blocks = rawlen/16;
		if ( (rawlen % 16) != 0 )
			blocks += 1;
		static char raw_hex_data[1024] = {0};
		static char ascii_data[1024] = {0};
		char* ptr1;
		char* ptr2;
		int remain_len;
		int print_len;

		for(int i = 0; i < blocks; i++ ){
			memset ( raw_hex_data, 0x00, sizeof(raw_hex_data) );
			memset ( ascii_data, 0x00, sizeof(ascii_data) );
			ptr1 = raw_hex_data;
			ptr2 = ascii_data;
			remain_len = (rawlen - (16 * i));
			if( remain_len > 16 )
				print_len = 16;
			else
				print_len = remain_len;
			int start = i*16;
			for( int j = start; j < (start + print_len); j++ ){
				if ( j == start )
					ptr1 += sprintf( ptr1, "%04d: ", j );
				
				ptr1 += sprintf( ptr1, "%02x ", raw[j] );
				if ( isprint( raw[j] ) )
					ptr2 += sprintf( ptr2, "%c", raw[j] );
				else
					ptr2 += sprintf( ptr2, "." );
			}
			lockMutex();
                	if ( use_syslog )
                        	syslog(LOG_USER|LOG_INFO, "%-54s | %-16s |\n", raw_hex_data, ascii_data);
                	else
				printf( "%-54s | %-16s |\n", raw_hex_data, ascii_data );
			unlockMutex();
		}
		printf( "\n" );
	}
	
}

