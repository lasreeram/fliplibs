ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include
AM_LDFLAGS = -pthread

#headers
include_HEADERS = include/skel.h  include/Socket.h

#libraries
lib_LTLIBRARIES = libmycppsock.la
libmycppsock_la_SOURCES = lib/Socket.cpp
libmycppsock_la_LIBADD = -lpthread -lcrypto -lcrypt -lssl -lmydebug

#binaries
bin_PROGRAMS = server_tcp client_tcp \
		server_udp client_udp \
		server_udptest client_udptest  \
		server_tcptest client_tcptest \
		server_tcp_hb client_tcp_hb \
		server_tcp_nl client_tcp_nl \
		tcpecho	shutdownc rlnumd udpecho1 \
		udpclient udpecho2 mytcpmux mytcpmux_client mykeep xout1 \
		mykeep2 xout2 xout3 extsys sslclient sslserver sslbioclient sslbioserver \
		server_tcpnw client_tcpnw \
		server_tcpstnw client_tcpstnw \
		server_tcpvarnw client_tcpvarnw

server_tcp_SOURCES = demo/server_tcp.cpp
client_tcp_SOURCES = demo/client_tcp.cpp
server_udp_SOURCES = demo/server_udp.cpp
client_udp_SOURCES = demo/client_udp.cpp
server_udptest_SOURCES = demo/server_udptest.cpp
client_udptest_SOURCES = demo/client_udptest.cpp
server_tcptest_SOURCES = demo/server_tcptest.cpp
client_tcptest_SOURCES = demo/client_tcptest.cpp
server_tcp_hb_SOURCES = demo/server_tcp_hb.cpp
client_tcp_hb_SOURCES = demo/client_tcp_hb.cpp
server_tcp_nl_SOURCES = demo/server_tcp_nl.cpp
client_tcp_nl_SOURCES = demo/client_tcp_nl.cpp
tcpecho_SOURCES = demo/tcpecho.cpp
shutdownc_SOURCES = demo/shutdownc.cpp
rlnumd_SOURCES = demo/rlnumd.cpp
udpecho1_SOURCES = demo/udpecho1.cpp
udpclient_SOURCES = demo/udpclient.cpp
udpecho2_SOURCES = demo/udpecho2.cpp
mytcpmux_SOURCES = demo/mytcpmux.cpp
mytcpmux_client_SOURCES = demo/mytcpmux_client.cpp
mykeep_SOURCES = demo/mykeep.cpp
xout1_SOURCES = demo/xout1.cpp
mykeep2_SOURCES = demo/mykeep2.cpp
xout2_SOURCES = demo/xout2.cpp
xout3_SOURCES = demo/xout3.cpp
extsys_SOURCES = demo/extsys.cpp
sslclient_SOURCES = demo/sslclient.cpp
sslserver_SOURCES = demo/sslserver.cpp
sslbioclient_SOURCES = demo/sslbioclient.cpp
sslbioserver_SOURCES = demo/sslbioserver.cpp
server_tcpnw_SOURCES = demo/server_tcpnw.cpp
client_tcpnw_SOURCES = demo/client_tcpnw.cpp
server_tcpstnw_SOURCES = demo/server_tcpstnw.cpp
client_tcpstnw_SOURCES = demo/client_tcpstnw.cpp
server_tcpvarnw_SOURCES = demo/server_tcpvarnw.cpp
client_tcpvarnw_SOURCES = demo/client_tcpvarnw.cpp

LDADD = -lssl -lcrypt -lmycppsock -lmydebug
