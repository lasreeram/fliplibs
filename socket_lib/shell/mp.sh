MSGNO=1
while true
	do
		echo message $MSGNO
		sleep 1
		MSGNO=`expr $MSGNO + 1`
	done
