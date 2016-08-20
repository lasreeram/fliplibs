ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include

#headers
include_HEADERS = include/libfun.h

#libraries
lib_LTLIBRARIES = libmyfunlib.la
libmyfunlib_la_SOURCES = lib/funlib.cpp 

#binaries
bin_PROGRAMS = hello
hello_SOURCES = src/hello.cpp
hello_LDADD = $(lib_LTLIBRARIES) 

