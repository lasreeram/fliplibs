ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/bcc_helper.h

#libraries
lib_LTLIBRARIES = libmybcc_helper.la
libmybcc_helper_la_SOURCES = libsrc/bcc_helper.cpp
libmybcc_helper_la_LIBADD = -lmydebug

#binaries
bin_PROGRAMS = argdist_test

argdist_test_SOURCES = demo/argdist_test.cpp
argdist_test_LDADD = -lmydebug  -lmybcc_helper
