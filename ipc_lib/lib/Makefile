VPATH = .:../include:/usr/include
CC=g++
INSTALLDIR = ~/pdir/lib
CFLAGS=-c -Wall -g -fPIC -I../include -shared -o 
CLEANEXTS = o so
.PHONY:all
all : libmyipc.so
libmyipc.so : IPC.cpp
	$(CC) $(CFLAGS) $(INSTALLDIR)/$@ $<

clean:
