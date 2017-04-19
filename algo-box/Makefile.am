ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include -O0
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/ArrayList.h include/LinkedList.h

#binaries
bin_PROGRAMS = array arraystack stack fastarraystack queue dualqueue rootisharrstk \
		stackll queuell

array_SOURCES = demo/array.cpp 
arraystack_SOURCES = demo/arraystack.cpp
stack_SOURCES = demo/stack.cpp 
fastarraystack_SOURCES = demo/fastarraystack.cpp
queue_SOURCES = demo/queue.cpp
dualqueue_SOURCES = demo/dualqueue.cpp
rootisharrstk_SOURCES = demo/rootisharrstk.cpp
stackll_SOURCES = demo/stackll.cpp 
queuell_SOURCES = demo/queuell.cpp


LDADD = -lmydebug
