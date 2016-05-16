VPATH = .:../include:/usr/include
CC=g++ -v
LD_FLAG = -lpthread -lglib
INSTALLDIR = ~/pdir/lib
CFLAGS=-c -Wall -g -fPIC -I../include -shared -o 
CLEANEXTS = o so
.PHONY:all
all : libmydebug.so
libmydebug.so : debug.cpp
	$(CC) $(CFLAGS) $(INSTALLDIR)/$@ $< $(LD_FLAG)

clean:
