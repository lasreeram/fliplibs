ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/IPC.h  

#libraries
lib_LTLIBRARIES = libmyipc.la
libmyipc_la_SOURCES = lib/IPC.cpp
libmyipc_la_LIBADD = -lmydebug

#binaries
bin_PROGRAMS = agent dumpshm recvagent sendagent createmb sendbig_agent sysv_createmb sysv_recvagent sysv_sendagent

agent_SOURCES = src/agent.cpp
dumpshm_SOURCES =  src/dumpshm.cpp
recvagent_SOURCES = src/recvagent.cpp
sendagent_SOURCES = src/sendagent.cpp
createmb_SOURCES = src/createmb.cpp
sendbig_agent_SOURCES = src/sendbig_agent.cpp
sysv_createmb_SOURCES = src/sysv_createmb.cpp
sysv_recvagent_SOURCES = src/sysv_recvagent.cpp
sysv_sendagent_SOURCES = src/sysv_sendagent.cpp

LDADD = -lmyipc -lmydebug
