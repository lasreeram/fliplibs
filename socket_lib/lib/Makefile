VPATH = .:../include:/home/hduser/pdir/include
CC=g++
INSTALLDIR = ~/pdir/lib
CFLAGS=-c -Wall -g -fPIC -I/home/hduser/pdir/include -I/home/hduser/sdir/socket_lib/include -shared -o
CLEANEXTS = o so
.PHONY:all
all : libmycppsock.so
libmycppsock.so : Socket.cpp
	$(CC) $(CFLAGS) $(INSTALLDIR)/$@ $<

clean:
