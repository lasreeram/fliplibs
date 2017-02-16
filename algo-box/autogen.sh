if [ -f "./configure.ac" ] 
then
	libtoolize
	autoreconf -i

else
	echo "configure.ac not found! Create or copy!"
fi
