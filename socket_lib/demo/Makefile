CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=
OBJDIR=bin
EXECUTABLE=server_tcp client_tcp \
		server_udp client_udp \
		server_udptest client_udptest  \
		server_tcptest client_tcptest \
		server_tcp_hb client_tcp_hb \
		server_tcp_nl client_tcp_nl \
		tcpecho	shutdownc rlnumd udpecho1 \
		udpclient udpecho2 mytcpmux mytcpmux_client mykeep xout1 \
		mykeep2 xout2 xout3 extsys



all: $(EXECUTABLE)

server_tcp : server_tcp.o 
	$(CC) $(LDFLAGS) $< -o $@

server_tcp.o : server_tcp.cpp 
	$(CC) $(CFLAGS) $< -o $@

client_tcp : client_tcp.o
	$(CC) $(LDFLAGS) $< -o $@

client_tcp.o : client_tcp.cpp
	$(CC) $(CFLAGS) $< -o $@
	
server_udp : server_udp.o
	$(CC) $(LDFLAGS) $< -o $@

server_udp.o : server_udp.cpp
	$(CC) $(CFLAGS) $< -o $@

client_udp : client_udp.o
	$(CC) $(LDFLAGS) $< -o $@

client_udp.o : client_udp.cpp
	$(CC) $(CFLAGS) $< -o $@

server_udptest : server_udptest.o
	$(CC) $(LDFLAGS) $< -o $@

server_udptest.o : server_udptest.cpp
	$(CC) $(CFLAGS) $< -o $@

client_udptest : client_udptest.o
	$(CC) $(LDFLAGS) $< -o $@

client_udptest.o : client_udptest.cpp
	$(CC) $(CFLAGS) $< -o $@

client_tcptest : client_tcptest.o
	$(CC) $(LDFLAGS) $< -o $@

client_tcptest.o : client_tcptest.cpp
	$(CC) $(CFLAGS) $< -o $@

server_tcptest : server_tcptest.o
	$(CC) $(LDFLAGS) $< -o $@

server_tcptest.o : server_tcptest.cpp
	$(CC) $(CFLAGS) $< -o $@

client_tcp_hb : client_tcp_hb.o
	$(CC) $(LDFLAGS) $< -o $@

client_tcp_hb.o : client_tcp_hb.cpp
	$(CC) $(CFLAGS) $< -o $@

server_tcp_hb : server_tcp_hb.o
	$(CC) $(LDFLAGS) $< -o $@

server_tcp_hb.o : server_tcp_hb.cpp
	$(CC) $(CFLAGS) $< -o $@

server_tcp_nl : server_tcp_nl.o
	$(CC) $(LDFLAGS) $< -o $@

server_tcp_nl.o : server_tcp_nl.cpp
	$(CC) $(CFLAGS) $< -o $@

tcpecho : tcpecho.o
	$(CC) $(LDFLAGS) $< -o $@

tcpecho.o : tcpecho.cpp
	$(CC) $(CFLAGS) $< -o $@

shutdownc : shutdownc.o
	$(CC) $(LDFLAGS) $< -o $@

shutdown.o : shutdown.cpp
	$(CC) $(CFLAGS) $< -o $@

rlnumd : rlnumd.o
	$(CC) $(LDFLAGS) $< -o $@

rlumd.o : rlnumd.cpp
	$(CC) $(CFLAGS) $< -o $@

udpecho1 : udpecho1.o
	$(CC) $(LDFLAGS) $< -o $@

udpecho1.o : udpecho1.cpp
	$(CC) $(CFLAGS) $< -o $@
	
udpclient : udpclient.o
	$(CC) $(LDFLAGS) $< -o $@

udpclient.o : udpclient.cpp
	$(CC) $(CFLAGS) $< -o $@
	
udpecho2 : udpecho2.o
	$(CC) $(LDFLAGS) $< -o $@

udpecho2.o : udpecho2.cpp
	$(CC) $(CFLAGS) $< -o $@
	
mytcpmux : mytcpmux.o
	$(CC) $(LDFLAGS) $< -o $@

mytcpmux.o : mytcpmux.cpp
	$(CC) $(CFLAGS) $< -o $@
	
mytcpmux_client : mytcpmux_client.o
	$(CC) $(LDFLAGS) $< -o $@

mytcpmux_client.o : mytcpmux_client.cpp
	$(CC) $(CFLAGS) $< -o $@
	
mykeep : mykeep.o
	$(CC) $(LDFLAGS) $< -o $@

mykeep.o : mykeep.cpp
	$(CC) $(CFLAGS) $< -o $@
	
xout1 : xout1.o
	$(CC) $(LDFLAGS) $< -o $@

xout1.o : xout1.cpp
	$(CC) $(CFLAGS) $< -o $@
	
mykeep2 : mykeep2.o
	$(CC) $(LDFLAGS) $< -o $@

mykeep2.o : mykeep2.cpp
	$(CC) $(CFLAGS) $< -o $@
	
xout2 : xout2.o
	$(CC) $(LDFLAGS) $< -o $@

xout2.o : xout2.cpp
	$(CC) $(CFLAGS) $< -o $@
	
xout3 : xout3.o
	$(CC) $(LDFLAGS) $< -o $@

xout3.o : xout3.cpp
	$(CC) $(CFLAGS) $< -o $@
	
extsys : extsys.o
	$(CC) $(LDFLAGS) $< -o $@

extsys.o : extsys.cpp
	$(CC) $(CFLAGS) $< -o $@
	

clean:
	rm $(OBJDIR)/*.o

install:
	mkdir -p $(OBJDIR)
	mv $(EXECUTABLE) $(OBJDIR)/
	cp mp.sh $(OBJDIR)/
	chmod +x $(OBJDIR)/mp.sh
	mv *.o $(OBJDIR)/
