#!/bin/bash

if [ -z "$1" ]; then          	#The user has not provided an input file, alert and exit
	echo "You must atleast specify an input file!"
	exit
else
	if [ -f $1 ]; then        	#This checks that the input file was valid, if not alert and exit
		make					#Compile the solving routine with makefile
		ONE=$2					#Initialize a variable (rotation flag)
		TWO=$3					#Initialize a variable (Max Solutions)
		if [ -z $2 ]; then			#If user has not specified rotation flag, set default to no rotate
			ONE=0
		fi
		if [ -z $3 ]; then			#If user has not specified max solutions flag, set default to 5
			TWO=5
		fi
		time ./a.out $1 $ONE					#Call C executable with proper command line arguments
		python drawbox.py c_output.txt $TWO		#Call python script with proper arguments			
	else
		echo "That was not a valid input file!"
		exit
	fi
fi

exit							#Exit the script