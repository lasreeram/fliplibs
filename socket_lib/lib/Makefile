VPATH = .:../include:/usr/include
CC=g++
INSTALLDIR = ~/pdir/lib
CFLAGS=-c -Wall -g -fPIC -I../include -shared -o 
CLEANEXTS = o so
.PHONY:all
all : libmycppsock.so
libmycppsock.so : Socket.cpp
	$(CC) $(CFLAGS) $(INSTALLDIR)/$@ $<

clean:
