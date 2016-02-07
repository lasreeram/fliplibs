#include <errno.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <exception>
#include <debug.h>

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

        void throw_fatal_error( const char* format, ... ){
                char buf[2048] = {0};
                va_list ap;
                va_start(ap, format);
                vsprintf(buf, format, ap);
                if ( use_syslog ){
                        syslog(LOG_USER|LOG_CRIT, "%s", buf );
                }else{
                        if ( errno )
                                printf("%s: error type = %d, errno = %d(%s), error = %s\n",
                                                program_name, LOG_CRIT, errno, strerror(errno), buf );
                        else
                                printf("%s: error type = %d, error = %s\n", program_name, LOG_CRIT, buf );
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
                                printf("%s: error type = %d, errno = %d(%s), error = %s\n",
                                                        program_name, LOG_ERR, errno, strerror(errno), buf );
                        else
                                printf("%s: error type = %d, error = %s\n", program_name, LOG_ERR, buf );
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
                        printf("%s: %s\n", program_name, buf );
                //vsyslog(LOG_USER|err, format, ap );
                va_end(ap);
        }
}

