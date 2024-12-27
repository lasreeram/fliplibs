ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(srcdir)/include -I$(prefix)/include 
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/bcc_helper.h

#libraries
lib_LTLIBRARIES = libmybcc_helper.la
libmybcc_helper_la_SOURCES = libsrc/bcc_helper.cpp
libmybcc_helper_la_LIBADD = -lmydebug

#binaries
bin_PROGRAMS = argdist_test memleak_test usdt_test signal_11_test

argdist_test_SOURCES = demo/argdist_test.cpp
argdist_test_LDADD = -lmydebug  -lmybcc_helper

memleak_test_SOURCES = demo/memleak_test.cpp
memleak_test_LDADD = -lmydebug  -lmybcc_helper

usdt_test_SOURCES = demo/usdt_test.cpp
usdt_test_LDADD = -lmydebug  -lmybcc_helper

signal_11_test_SOURCES = demo/signal_11_test.cpp
signal_11_test_LDADD = -lmydebug  -lmybcc_helper
