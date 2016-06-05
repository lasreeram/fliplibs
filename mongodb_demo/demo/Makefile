CC=g++
CFLAGS=-v -c -Wall -g 
INCLD = -I/home/hduser/pdir/include -I/home/hduser/mongo_cxx_driver_src/install/include 
LDFLAGS=-L/home/hduser/pdir/lib/ -L/usr/lib/x86_64-linux-gnu/lib/ 
LDLIBS=-lmydebug -pthread -lboost_thread -lboost_system -lboost_regex -lssl -lcrypto -lmongoclient
INSTALLDIR=~/pdir/bin
EXECUTABLE= mongotest



all: $(EXECUTABLE)

mongotest : mongotest.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  $(LDLIBS)

mongotest.o : mongotest.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

clean:
	rm $(INSTALLDIR)/*.o

install:
