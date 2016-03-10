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

namespace debug_lib{
        static bool use_syslog = false;
        static bool debug_off = false;
	static char* program_name;

	void init(char* argv0){
               ( debug_lib::program_name = strrchr( argv0, '/' ) )? debug_lib::program_name++: (debug_lib::program_name = argv0);
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
                if ( use_syslog ){
                        syslog(LOG_USER|LOG_CRIT, "%s", buf );
                }else{
                        if ( errno )
                                printf("%s %s: error type = %d, errno = %d(%s), error = %s\n",
                                                program_name, getCurrentTime(), LOG_CRIT, errno, strerror(errno), buf );
                        else
                                printf("%s %s: error type = %d, error = %s\n", program_name, getCurrentTime(), LOG_CRIT, buf );
                }
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
                if ( use_syslog ){
                        syslog(LOG_USER|LOG_ERR, "%s", buf );
                }else{
                        if ( errno )
                                printf("%s %s: error type = %d, errno = %d(%s), error = %s\n",
                                                        program_name, getCurrentTime(), LOG_ERR, errno, strerror(errno), buf );
                        else
                                printf("%s %s: error type = %d, error = %s\n", program_name, getCurrentTime(), LOG_ERR, buf );
                }
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
                if ( use_syslog )
                        syslog(LOG_USER|LOG_INFO, "%s", buf );
                else
                        printf("%s", buf );
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
                if ( use_syslog )
                        syslog(LOG_USER|LOG_INFO, "%s", buf );
                else
                        printf("%s %s: %s\n", program_name, getCurrentTime(), buf );
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
                	if ( use_syslog )
                        	syslog(LOG_USER|LOG_INFO, "%-54s | %-16s |\n", raw_hex_data, ascii_data);
                	else
				printf( "%-54s | %-16s |\n", raw_hex_data, ascii_data );
		}
		printf( "\n" );
	}
	
}

