if [ -f "./configure.ac" ] 
then
	libtoolize
	autoreconf -i
	touch NEWS README AUTHORS ChangeLog

else
	echo "configure.ac not found! Create or copy!"
fi
