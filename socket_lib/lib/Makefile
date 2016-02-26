VPATH = .:../include:/home/hduser/pdir/include
CC=g++
INSTALLDIR = ~/pdir/lib
#LDFLAGS=-L /usr/lib/x86_64-linux-gnu/ -lssl -lcrypt
LDFLAGS=-lssl -lcrypto -lcrypt
CFLAGS=-L/usr/lib/x86_64-linux-gnu/ -c -Wall -g -fPIC -I/home/hduser/pdir/include -I/home/hduser/sdir/socket_lib/include -shared -o
CLEANEXTS = o so
.PHONY:all
all : libmycppsock.so
libmycppsock.so : Socket.cpp
	$(CC) $(CFLAGS) $(INSTALLDIR)/$@ $<  $(LDFLAGS)

clean:
