#!/bin/bash

if [ -z "$1" ]; then
	echo "You must atleast specify an input file!"
else
	if [ -f $1 ]; then
		make	
		if [ -z $2 ]; then
			z=0
		fi
		./a.out $1 $2
		python drawbox.py c_output.txt 5
	else
		echo "That was not a valid input file!"
	fi
fi

mkdir -p solution_files
cd solution_files

exit