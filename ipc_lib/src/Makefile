CC=g++
CFLAGS=-c -Wall -g -I../include -I/home/hduser/pdir/include
LDFLAGS=-L/home/hduser/pdir/lib/ -lmyipc -lmydebug
INSTALLDIR=~/pdir/bin
EXECUTABLE=agent dumpshm recvagent sendagent createmb\


all: $(EXECUTABLE)

agent : agent.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

agent.o : agent.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

dumpshm : dumpshm.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

dumpshm.o : dumpshm.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

recvagent : recvagent.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

recvagent.o : recvagent.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

sendagent : sendagent.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

sendagent.o : sendagent.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

createmb : createmb.o
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< -o $(INSTALLDIR)/$@

createmb.o : createmb.cpp
	$(CC) $(CFLAGS) $< -o $(INSTALLDIR)/$@

clean:
	rm $(INSTALLDIR)/*.o

install:
