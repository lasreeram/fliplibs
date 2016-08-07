CC=g++
CFLAGS=-v -c -Wall -g
INCLD = -I/home/hduser/sdir/socket_lib/include  -I/home/hduser/pdir/include -I/home/hduser/googletest-master/googletest/include
LDFLAGS=-L/home/hduser/pdir/lib/ -L/usr/lib/x86_64-linux-gnu/ -lmycppsock -lmydebug -lssl -lcrypto -lmyelliptic
INSTALLDIR=~/pdir/tests
EXECUTABLE= test_pubfrompriv testf_pubfrompriv testf_der_pubfrompriv testf_vals_pubfrompriv \



all: $(EXECUTABLE)

test_pubfrompriv : test_pubfrompriv.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< ~/pdir/lib/gtest_main.a -lpthread -o $(INSTALLDIR)/$@ 

test_pubfrompriv.o : test_pubfrompriv.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

testf_pubfrompriv : testf_pubfrompriv.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< ~/pdir/lib/gtest.a -lpthread -o $(INSTALLDIR)/$@ 

testf_pubfrompriv.o : testf_pubfrompriv.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

testf_der_pubfrompriv : testf_der_pubfrompriv.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< ~/pdir/lib/gtest.a -lpthread -o $(INSTALLDIR)/$@ 

testf_der_pubfrompriv.o : testf_der_pubfrompriv.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

testf_vals_pubfrompriv : testf_vals_pubfrompriv.o 
	$(CC) $(LDFLAGS) $(INSTALLDIR)/$< ~/pdir/lib/gtest.a -lpthread -o $(INSTALLDIR)/$@ 

testf_vals_pubfrompriv.o : testf_vals_pubfrompriv.cpp 
	$(CC) $(CFLAGS) $(INCLD) $< -o $(INSTALLDIR)/$@

clean:
	rm $(INSTALLDIR)/*.o

install:
