ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(prefix)/include -I$(srcdir)/include -I/usr/local/include/bsoncxx/v_noabi/  -I/usr/local/include/mongocxx/v_noabi/
AM_LDFLAGS = -pthread 
AM_CXXFLAGS = -std=gnu++11


#binaries
#bin_PROGRAMS = mongoinsert mongoread mongoquery
bin_PROGRAMS = hellomongo

hellomongo_SOURCES = demo/hellomongo.cpp
hellomongo_LDADD = -lbsoncxx -lmongocxx
#mongoinsert_SOURCES = demo/mongoinsert.cpp
#mongoinsert_LDADD = /home/hduser/pdir/lib/libmongoclient.a  /usr/lib/x86_64-linux-gnu/libboost_filesystem.a /usr/lib/x86_64-linux-gnu/libboost_program_options.a /usr/lib/x86_64-linux-gnu/libboost_thread.a /usr/lib/x86_64-linux-gnu/libboost_system.a /usr/lib/x86_64-linux-gnu/libboost_regex.a -lmydebug -lssl -lcrypt -lcrypto
#mongoread_SOURCES =  demo/mongoread.cpp
#mongoquery_SOURCES = demo/mongoquery.cpp
