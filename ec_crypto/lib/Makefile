VPATH = .:../include:/home/hduser/pdir/include
CC=g++
INSTALLDIR = ~/pdir/lib
LDFLAGS=-lssl -lcrypto -lcrypt -lmydebug
CFLAGS=-v -L/usr/lib/x86_64-linux-gnu/ -c -Wall -g -fPIC -I/home/hduser/pdir/include -I/home/hduser/sdir/socket_lib/include -shared -o
CLEANEXTS = o so
.PHONY:all
all : libmyelliptic.so
libmyelliptic.so : elliptic_curve.cpp
	$(CC) $(CFLAGS) $(INSTALLDIR)/$@ $<  $(LDFLAGS)

clean:
