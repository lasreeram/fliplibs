#!/usr/bin/env python

fd = open ("test.txt", 'w' )

i = 1
while ( i <= 100000 ):
	fd.write( "Line " + str(i) + " - Test file for opendsa\n"  )
	i += 1

fd.close()

