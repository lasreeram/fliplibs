#include <errno.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <exception>

namespace error_lib{
        static bool use_syslog = false;
        static bool debug_off = false;
	static char* program_name;

        class Exception : public std::exception {
                public:
                        Exception(const char* err) {
                                _error = err;
                        }
                        //throw() here means that this function cannot not throw any exception
                        const char* what() const throw() { return _error.c_str(); }
                        //throw() here means that this function cannot not throw any exception
                        //this is required since the std::exception is compiled with noexcept (or throw nothing clause)
                        //In C++ 11 all functions are noexcept(true) by default. If you really want to throw exceptions
                        //you must use throw(exception,..) indicate which types you are throwing.
                        ~Exception() throw() {}

                private:
                        std::string _error;
        };

	void init(char* argv0){
               ( error_lib::program_name = strrchr( argv0, '/' ) )? error_lib::program_name++: (error_lib::program_name = argv0);
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
                        printf("%s: %s", program_name, buf );
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

