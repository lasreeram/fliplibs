CC=g++
CFLAGS=-c -Wall -g -I/home/hduser/sdir/socket_lib/include  -I/home/hduser/pdir/include
LDFLAGS=-L/home/hduser/pdir/lib/ -L/usr/lib/x86_64-linux-gnu/ -lmycppsock -lmydebug -lssl -lcrypto
INSTALLDIR=~/pdir/bin
EXECUTABLE=server_tcp client_tcp \
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



all: $(EXECUTABLE)

server_tcp : server_tcp.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  

server_tcp.o : server_tcp.cpp 
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

client_tcp : client_tcp.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

client_tcp.o : client_tcp.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
server_udp : server_udp.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

server_udp.o : server_udp.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

client_udp : client_udp.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

client_udp.o : client_udp.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

server_udptest : server_udptest.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

server_udptest.o : server_udptest.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

client_udptest : client_udptest.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

client_udptest.o : client_udptest.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

client_tcptest : client_tcptest.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

client_tcptest.o : client_tcptest.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

server_tcptest : server_tcptest.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

server_tcptest.o : server_tcptest.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

client_tcp_hb : client_tcp_hb.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

client_tcp_hb.o : client_tcp_hb.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

client_tcp_nl : client_tcp_nl.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

client_tcp_nl.o : client_tcp_nl.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

server_tcp_hb : server_tcp_hb.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

server_tcp_hb.o : server_tcp_hb.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

server_tcp_nl : server_tcp_nl.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

server_tcp_nl.o : server_tcp_nl.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

tcpecho : tcpecho.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

tcpecho.o : tcpecho.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

shutdownc : shutdownc.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

shutdownc.o : shutdownc.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

rlnumd : rlnumd.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

rlnumd.o : rlnumd.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

udpecho1 : udpecho1.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

udpecho1.o : udpecho1.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
udpclient : udpclient.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

udpclient.o : udpclient.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
udpecho2 : udpecho2.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

udpecho2.o : udpecho2.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
mytcpmux : mytcpmux.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

mytcpmux.o : mytcpmux.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
mytcpmux_client : mytcpmux_client.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

mytcpmux_client.o : mytcpmux_client.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
mykeep : mykeep.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

mykeep.o : mykeep.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
xout1 : xout1.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

xout1.o : xout1.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
mykeep2 : mykeep2.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

mykeep2.o : mykeep2.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
xout2 : xout2.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

xout2.o : xout2.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
xout3 : xout3.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

xout3.o : xout3.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
extsys : extsys.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

extsys.o : extsys.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
sslclient : sslclient.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

sslclient.o : sslclient.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
sslserver : sslserver.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

sslserver.o : sslserver.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
sslbioclient : sslbioclient.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

sslbioclient.o : sslbioclient.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
sslbioserver : sslbioserver.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

sslbioserver.o : sslbioserver.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

server_tcpnw : server_tcpnw.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  

server_tcpnw.o : server_tcpnw.cpp 
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

client_tcpnw : client_tcpnw.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

client_tcpnw.o : client_tcpnw.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
server_tcpstnw : server_tcpstnw.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  

server_tcpstnw.o : server_tcpstnw.cpp 
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

client_tcpstnw : client_tcpstnw.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

client_tcpstnw.o : client_tcpstnw.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
server_tcpvarnw : server_tcpvarnw.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  

server_tcpvarnw.o : server_tcpvarnw.cpp 
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

client_tcpvarnw : client_tcpvarnw.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

client_tcpvarnw.o : client_tcpvarnw.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@
	
	
clean:
	rm $(INSTALLDIR)/*.o

install:
	cp mp.sh $(INSTALLDIR)/
	chmod +x $(INSTALLDIR)/mp.sh
