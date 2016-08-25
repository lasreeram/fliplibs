ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/Threads.h  

#libraries
lib_LTLIBRARIES = libmypthread.la
libmypthread_la_SOURCES = lib/Threads.cpp
libmypthread_la_LIBADD = -lmydebug

#binaries
bin_PROGRAMS = simpledemo twothreads cancelthread manythreads

simpledemo_SOURCES = demo/simpledemo.cpp
twothreads_SOURCES =  demo/twothreads.cpp
cancelthread_SOURCES = demo/cancelthread.cpp
manythreads_SOURCES = demo/manythreads.cpp

LDADD = -lmypthread -lmydebug
