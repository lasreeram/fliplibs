CC=g++
CFLAGS=-v -c -Wall -g 
INCLD = -I/home/hduser/sdir/threads_lib/include  -I/home/hduser/pdir/include
LDFLAGS=-L/home/hduser/pdir/lib/ -L/usr/lib/x86_64-linux-gnu/ -lmydebug -lmypthread -pthread
INSTALLDIR=~/pdir/bin
EXECUTABLE= simpledemo twothreads cancelthread manythreads \



all: $(EXECUTABLE)

simpledemo : simpledemo.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  

simpledemo.o : simpledemo.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

twothreads : twothreads.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  

twothreads.o : twothreads.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

cancelthread : cancelthread.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  

cancelthread.o : cancelthread.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

manythreads : manythreads.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@  

manythreads.o : manythreads.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

clean:
	rm $(INSTALLDIR)/*.o

install:
