#ifndef _DEBUG_LIB_H_
#define _DEBUG_LIB_H_
#include <string>

namespace debug_lib{
	void init(char* argv0, bool isMultiThreaded = false);
	void init(pthread_t tid, const char* threadname);
	void logDebugInSyslog();
	void logDebugInStdOut();
	void throw_fatal_error( const char* format, ... );
	void throw_error( const char* format, ... );
	void log( const char* format, ... );
	void log_no_newline( const char* format, ... );
	void lograw( const unsigned char* buf, int len );
	class Exception : public std::exception {
		public:
			Exception(const char* err) {
				_error = err;
			}
			Exception(string err) {
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
}

#endif
