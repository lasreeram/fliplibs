CC=g++
CFLAGS=-v -c -Wall -g
INCLD = -I/home/hduser/sdir/socket_lib/include  -I/home/hduser/pdir/include
LDFLAGS=-L/home/hduser/pdir/lib/ -L/usr/lib/x86_64-linux-gnu/ -lmycppsock -lmydebug -lssl -lcrypto -lmyelliptic
INSTALLDIR=~/pdir/bin
EXECUTABLE= \
		pubfrompriv



all: $(EXECUTABLE)

pubfrompriv : pubfrompriv.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  

pubfrompriv.o : pubfrompriv.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

clean:
	rm $(INSTALLDIR)/*.o

install:
