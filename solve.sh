#!/bin/bash

if [ -z "$1" ]; then
	echo "You must atleast specify an input file!"
else
	if [ -f $1 ]; then
		make
		ONE=$2
		TWO=$3	
		if [ -z $2 ]; then
			ONE=0
		fi
		if [ -z $3 ]; then
			TWO=5
		fi
		./a.out $1 $ONE
		python drawbox.py c_output.txt $TWO
	else
		echo "That was not a valid input file!"
	fi
fi

mkdir -p solution_files
cd solution_files

exit