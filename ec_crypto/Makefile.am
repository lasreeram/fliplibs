ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/elliptic_curve.h

#libraries
lib_LTLIBRARIES = libmyelliptic.la
libmyelliptic_la_SOURCES = lib/elliptic_curve.cpp
libmyelliptic_la_LIBADD = -lssl -lcrypto -lcrypt -lmydebug

#binaries
bin_PROGRAMS = pubfrompriv ec_sslclient ec_sslserver

pubfrompriv_SOURCES = demo/simpledemo.cpp
ec_sslclient_SOURCES =  demo/twothreads.cpp
ec_sslserver_SOURCES = demo/cancelthread.cpp

#tests
tests_PROGRAMS = 

LDADD = -lmyelliptic -lmydebug
