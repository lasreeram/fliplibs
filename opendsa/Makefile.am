ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/revafile.h  

#binaries
bin_PROGRAMS = revafile

revafile_SOURCES = src/revafile.cpp

LDADD = -lmydebug
