VPATH = .:../include:/home/hduser/pdir/include
CC=g++
INSTALLDIR = ~/pdir/lib
LDFLAGS= -pthread
CFLAGS=-v -L/usr/lib/x86_64-linux-gnu/ -c -Wall -g -fPIC -I/home/hduser/pdir/include -I/home/hduser/sdir/socket_lib/include -shared -o
CLEANEXTS = o so
.PHONY:all
all : libmypthread.so
libmypthread.so : Threads.cpp
	$(CC) $(CFLAGS) $(INSTALLDIR)/$@ $<  $(LDFLAGS)

clean:
