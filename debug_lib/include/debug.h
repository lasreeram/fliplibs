#ifndef _DEBUG_LIB_H_
#define _DEBUG_LIB_H_

namespace debug_lib{
	void init(const char* argv0);
	void logDebugInSyslog();
	void logDebugInStdOut();
	void throw_fatal_error( const char* format, ... );
	void throw_error( const char* format, ... );
	void log( const char* format, ... );
}

#endif
