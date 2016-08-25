ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include

#headers
include_HEADERS = include/debug.h

#libraries
lib_LTLIBRARIES = libmydebug.la
libmydebug_la_SOURCES = libsrc/debug.cpp 
libmydebug_la_LIBADD = -lpthread 
