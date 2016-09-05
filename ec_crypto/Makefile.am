ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include -I/home/hduser/sdir/socket_lib/include  -I/home/hduser/pdir/include -I/home/hduser/googletest-master/googletest/include
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/elliptic_curve.h

#libraries
lib_LTLIBRARIES = libmyelliptic.la
libmyelliptic_la_SOURCES = lib/elliptic_curve.cpp
libmyelliptic_la_LIBADD = -lssl -lcrypto -lcrypt -lmydebug

#binaries
bin_PROGRAMS = pubfrompriv ec_sslclient ec_sslserver

pubfrompriv_SOURCES = demo/pubfrompriv.cpp
ec_sslclient_SOURCES =  demo/ec_sslclient.cpp
ec_sslserver_SOURCES = demo/ec_sslserver.cpp

#tests
check_PROGRAMS = test_pubfrompriv testf_pubfrompriv testf_der_pubfrompriv testf_vals_pubfrompriv

test_pubfrompriv_SOURCES = tests/test_pubfrompriv.cpp
testf_pubfrompriv_SOURCES =  tests/testf_pubfrompriv.cpp
testf_der_pubfrompriv_SOURCES =  tests/testf_der_pubfrompriv.cpp
testf_vals_pubfrompriv_SOURCES =  tests/testf_vals_pubfrompriv.cpp
LDADD = -lmydebug /home/hduser/pdir/lib/gtest_main.a /home/hduser/pdir/lib/gtest.a -lmyelliptic -lmydebug -lmycppsock

TESTS = test_pubfrompriv testf_pubfrompriv testf_der_pubfrompriv testf_vals_pubfrompriv
