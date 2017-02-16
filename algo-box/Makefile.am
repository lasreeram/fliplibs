ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include -O0
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/Array.h

#binaries
bin_PROGRAMS = array arraystack stack fastarraystack queue

array_SOURCES = demo/array.cpp 
arraystack_SOURCES = demo/arraystack.cpp
stack_SOURCES = demo/stack.cpp 
fastarraystack_SOURCES = demo/fastarraystack.cpp
queue_SOURCES = demo/queue.cpp

LDADD = -lmydebug
