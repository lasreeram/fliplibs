CC=g++
CFLAGS=-v -c -Wall -g 
INCLD = -I/home/hduser/pdir/include -I/home/hduser/mongo_cxx_driver_src/install/include 
LDFLAGS=-L/home/hduser/pdir/lib/ -L/usr/lib/x86_64-linux-gnu/lib/ 
LDLIBS=-lmydebug -pthread -lboost_thread -lboost_system -lboost_regex -lssl -lcrypto -lmongoclient
INSTALLDIR=~/pdir/bin
EXECUTABLE= mongoinsert mongoread mongoquery



all: $(EXECUTABLE)

mongoinsert : mongoinsert.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  $(LDLIBS)

mongoinsert.o : mongoinsert.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

mongoread : mongoread.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  $(LDLIBS)

mongoread.o : mongoread.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

mongoquery : mongoquery.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  $(LDLIBS)

mongoquery.o : mongoquery.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

clean:
	rm $(INSTALLDIR)/*.o

install:
