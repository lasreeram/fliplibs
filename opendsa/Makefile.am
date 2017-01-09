ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include -std=c++11
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/revafile.h  

#binaries
bin_PROGRAMS = revafile fifty fortytwo uniq nofirst uniqsorted printalldup evenandodd randlist matchstr revastack

revafile_SOURCES = src/revafile.cpp
fifty_SOURCES = src/fifty.cpp
fortytwo_SOURCES = src/fortytwo.cpp
uniq_SOURCES = src/uniq.cpp
nofirst_SOURCES = src/nofirst.cpp
uniqsorted_SOURCES = src/uniqsorted.cpp
printalldup_SOURCES = src/printalldup.cpp
evenandodd_SOURCES = src/evenandodd.cpp
randlist_SOURCES = src/randlist.cpp
#randlistq_SOURCES = src/randlistq.cpp
matchstr_SOURCES = src/matchstr.cpp
revastack_SOURCES = src/revastack.cpp

LDADD = -lmydebug
