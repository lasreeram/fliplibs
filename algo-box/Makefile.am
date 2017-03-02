ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include -O0
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/ArrayList.h

#binaries
bin_PROGRAMS = array arraystack stack fastarraystack queue dualqueue rootisharrstk

array_SOURCES = demo/array.cpp 
arraystack_SOURCES = demo/arraystack.cpp
stack_SOURCES = demo/stack.cpp 
fastarraystack_SOURCES = demo/fastarraystack.cpp
queue_SOURCES = demo/queue.cpp
dualqueue_SOURCES = demo/dualqueue.cpp
rootisharrstk_SOURCES = demo/rootisharrstk.cpp

LDADD = -lmydebug
