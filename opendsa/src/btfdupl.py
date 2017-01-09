#!/usr/bin/env python

fd = open ("dupl.txt", 'w' )

i = 1
while ( i <= 100000 ):
	fd.write( "Line " + str(i%10) + " - Test file for opendsa\n"  )
	i += 1

fd.close()

