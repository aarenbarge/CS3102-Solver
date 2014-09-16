README.txt
Aaren Barge - jab9rm@virginia.edu

//—————————————————————————————————————————//
Overview and Folder Contents
//—————————————————————————————————————————//

This folder contains all of the files associated with my CS3102 Theory of Computation course at UVA.  This program takes in a well-formed input describing a puzzle (in terms of a target configuration and available pieces) and displays images of the non isomorphic solutions (up to a maximum number).  

A detailed description of exactly how this program works, the data structures, and algorithms I wrote to accomplish this task, and directions on how to run this program are located in “CS3102_Documentation.pdf”.  

Solver.c is the source code for the input parsing, solving, and isometry detecting routines, and is compilable with the Makefile.

drawbox.py is a python script which draws the solutions so that they can be viewed visually.

solve.sh is a shell script that automates the process of running the ./a.out executable generated from the Solver.c source, and piping that output into drawbox.py.  

c_output.txt is a file that is used by the program and is overwritten every time Solver.c is executed, don’t store information here.

test_files contains sample input files that I used for debugging.  Use these or others of the same form to try the algorithm out

Finally, Sample Results contains test input files, output files, and screenshots of solutions for multiple puzzles I tested my project on.  

//—————————————————————————————————————————//
Running the Program
//—————————————————————————————————————————//

To run the whole program, navigate to the proper directory and call:

	./solve.sh __input_file__

This runs the default version of the program which does not allow for rotation of the pieces, and will display a maximum of 5 solutions.  To allow rotations and reflections call:

	./solve.sh __input_file__  1

To adjust the maximum number of solutions to display (up to 20), you must also specify whether rotations are allowed or not:

	./solve.sh __input_file__  1    15

Will allow rotations and reflections and display a maximum of 15 solutions,

	./solve.sh __input_file  0    100 

Will prohibit rotations and display a maximum of 20 solutions.

You can also compile and run the solving routine with:

	./a.out __input_file__  —rotate?—

Or You can display the solutions (as long as the file is well formed) using the python routine:

	python drawbox.py __input_file__  —max—

