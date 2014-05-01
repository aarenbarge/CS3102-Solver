#!/bin/bash

if [ -z "$1" ]; then
	echo "You must atleast specify an input file!"
else
	if [ -f $1 ]; then
		make
		./a.out $1
		./a.out $1
		./a.out $1
		./a.out $1
		python drawbox.py
		echo "Ran 4 times!"
	else
		echo "That was not a valid input file!"
	fi
fi

mkdir -p solution_files
cd solution_files

exit