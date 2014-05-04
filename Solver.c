//---------------------------------------------------------------//
//																 //
//         Theory of Computation (CS 3102) Term Project			 //
// 					  Aaren Barge - 5/3/2014					 //
//																 //
// 				   Solver.c source code - Takes  				 //
//																 //							
//---------------------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

//---------------------------------------------------------------//
//																 //
//                     Struct Declarations 						 //
//																 //
//---------------------------------------------------------------//

/*
Point is used to store the input from a file, contains an integer value and a pointer to the next Point
*/
typedef struct Point {
   int value;
   struct Point * next;
} Point;

/*
ArrayList allows the creating of a list of arrays, each node has a pointer to an int array, and a pointer
to the next ArrayList node
*/
typedef struct ArrayList {
	int * id;
	int * array;
	struct ArrayList * next;
} ArrayList;

/*
Piece Node is used to create the 2D array which will represent the pieces
*/
typedef struct PieceNode {
	struct PieceNode * top;
	struct PieceNode * left;
	struct PieceNode * right;
	struct PieceNode * bottom;
	int value;
	int x_coord;
	int y_coord;
} PieceNode;

/*
SolutionNode contains information about where a piece will fit in the graph and pointers to all subsolutions
that are possible after that choice has been made
*/
typedef struct SolutionNode {
	int * fields;
	struct PointerList * first;
}SolutionNode;

/*
PointerList is just used to store a list of pointers
*/
typedef struct PointerList {
	struct SolutionNode * solution;
	struct PointerList * next;
}PointerList;

/*
Solution List is used to represent solutions to a subproblem, there is a pointer to the first Solution 
Node which describes the position of one of the pieces in the solution, and inside that solution node struct
there is information about the locations of all the other piece locations.  The pointer points to the next
subsolution
*/
typedef struct SolutionList {
	struct SolutionNode * node;
	struct SolutionList * next;
}SolutionList;

/*
Rotation List stores pointers to all the non isomorphic rotations and reflections of a piece
*/
typedef struct RotationList{
	int id;
	int * o;
	int * ro;
	int * rro;
	int * rrro;
	int * ho;
	int * hro;
	int * hrro;
	int * hrrro;
	struct RotationList * next;
}RotationList;

//---------------------------------------------------------------//
//																 //
// 					 Forward Function Declarations 			     //
//																 //
//---------------------------------------------------------------//

/*
Function Name: int * openFileIntoArray
Description: This function opens a file and reads it into a 2D array of integers such that
the value at any location in the array is the ASCII encoding of the character in that location
in the input file

Inputs: 		char * file_name		= The name of the file to be opened and read
Outputs:		int * 					= An allocated int array with the information from file_name
*/
int * openFileIntoArray(char * file_name);

/*
Function Name: void print2DIntArray
Description: This function is used for debugging a 2D integer array, it will display to standard
output the formatted content of an int array

Inputs:			int * array				= The pointer to the array to be printed
				int width				= The value of the width of this array
				int height				= The value of the height of this array
Outputs:		void
*/
void print2DIntArray(int * array, int width, int height);

/*
Function Name: int * cloneIntArray
Description: This function allocates memory for a duplicate array and returns a pointer to this new 
array

Inputs:			int * array				= Pointer to the array to be copied
Outputs:		int * 					= The memory location of the new array (note this is offset
										by 2 from the location of the actual array, space is left at 
										the beginning of the array to store height and width ints)
*/
int * cloneIntArray(int * array);

/*
Function Name: ArrayList * findAllPieces
Description: This function searches through a properly formatted 2D int array and finds all of the
pieces contained in the array.  A list of ArrayLists is created such that their array fields point to
a 2D int array for each of the pieces found.  A pointer to the first ArrayList is returned, the last
ArrayList in the list has ArrayList * next = NULL

Inputs:			int * input_array		= Pointer to the array to be searched through
				int rows				= The number of rows in input_array ( = height)
				int row_length			= The number of columns per row ( = height)
Outputs:		ArrayList *				= The start of the ArrayList containing arrays of all the 
										pieces discovered in input_array
*/
ArrayList * findAllPieces(int * input_array, int rows, int row_length);

/*
Function Name: int foundAllPieces
Description: Searched through a 2D int array to determine whether all pieces have been found or not.
In the array, negative values denote values that have already been found by the findAllPieces
algorithm.  positive values (not equal to 32) are values that have not been found yet (not yet associated
with a piece).  This algorithm loops through all values in the array to determine if there are any positive
values left.  Returns 0 if all pieces have been found, 1 otherwise.

Inputs:			int * array				= Pointer to the array in question
				int width				= Value of the width of the array
				int height				= Value of the height of the array
Outputs:		int						= 0 if all pieces have been found, 1 otherwise
*/
int foundAllPieces(int * array, int width, int height);

/*
Function Name: void examineNeighbors
Description: This builds a list of PieceNodes that represent non-negative values in the array that are 
next to each other.  This function searches an array to find the neighbors at a location specified by 
the PieceNode pointer n.  The function alters the fields of this node to reflect found neighbors, and 
returs

Inputs:			int * array				= Pointer to the array loaded from file
				int width				= Value of the width of the array
				int height				= Value of the height of the array
				PieceNode * n			= Pointer to the piece node struct whose neighbors we want to find
Outputs:		void
*/
void examineNeighbors(int * array, int width, int height, PieceNode * n);

/*
Function Name: int * getInfo
Description: This function recursively traverses the PieceNode tree rooted at *n to find the 
width and height of the piece

Inputs:			PieceNode * n			= Pointer to the root piece node
				int cur_top				= value of the index of the top most value seen so far
				int cur_bottom			= Value of the index of the bottom most value seen so far
				int cur_left			= Value of the index of the left most value seen so far
				int cur_right			= Value of the index of the right most value seen so far
Outputs:		int * 					= A formatted array which contains the information found at the root node n
*/
int * getInfo(PieceNode * n, int cur_top, int cur_bottom, int cur_left, int cur_right);

/*
Function Name: void makePieceArray
Description: This function recursively fills in the values of the int array pointed to by * array,
by determining the value of the PieceNode n and where its location in the new array will be

Inputs:			int * array				= A pointer to an int array that will store our piece
				PieceNode * n			= A pointer to the pieceNode that we'll write into the array
				int left_offset			= Value of the x coordinate to write in the array
				int width				= Value of the y coordinate to write in the array
Outputs:		void
*/
void makePieceArray(int * array, PieceNode * n, int left_offset, int top_offset, int width);

/*
Function Name: int * findAndRemoveLargestArray
Description: This function removes the largest piece from a list of ArrayList structs and "removes" it from the list.
When the function returns a pointer to the int array storing the largest piece is returned, and the list of ArrayLists
rooted at list->next is the root for the new list of ArrayList structs containing every piece except the biggest

Inputs:			ArrayList * list		= A Pointer to the array list we'll be examining
Outputs:		int *					= A pointer to the largest piece in list
*/
int * findAndRemoveLargestArray(ArrayList * list);

/*
Function Name: int numberOfTiles
Description: This functino determines the number of tiles in the representation of a piece as an int array.
The way I represent the pieces, non-negative values and values != 32 are valid tiles...eg

97 97 32     a a
32 97 97  =    a a
32 97 32       a

has 5 tiles

Inputs:			int * n					= A pointer to the array of integers which represents the piece to be evaluated
				int width				= Value of the width of the array
				int height				= Value of the height of the array
Outputs:		int						= The number of tiles that the piece contains
*/
int numberOfTiles(int * n, int width, int height);

/*
Function Name: void debugPieces
Description: This function is used purely to debug the pieces that are generated by the input routine
It takes a pointer to the biggest pieces (assumed to be the target configuration) and a pointer
to the first ArrayList in the list of pieces and prints these pieces to standard output using
the above declared print2DIntArray function

Inputs:			int * biggest			= A Pointer to the int array representing the target configuration
				ArrayList * pieces		= A pointer to the first ArrayList struct in the list of pieces
Outputs:		void
*/
void debugPieces(int * biggest, ArrayList * pieces);

/*
Function Name: SolutionList * findSolutions
Description: This is an outdated solution finding routine that works exactly the same as the newer version
but doesnt allow for rotation of pieces.  This is a little buggy because I havent debugged it since the addition
of the new routine.  No reason to use this code artifact

Inputs:			int * biggest			= A pointer to the int array that represents the current state
										of the target configuration
				ArrayList * pieces		= A pointer to the first ArrayList struct which holds the array representing
										the piece that is currently being considered
Outputs:		SolutionList *			= A pointer to the solution list struct which holds the solutions generated
										for a given sub problem
*/
SolutionList * findSolutions(int * biggest, ArrayList * pieces);

/*
Function Name: int isAFit
Description: This function examines two int arrays and determines whether the smallest array "fits"
inside the biggest array at the given x and y values (that is the smallest array's top corner
starts at (x,y) )

Inputs:			int * biggest			= A pointer to the array representing the target configuration
				int * smaller			= A pointer to the array repersenting the small piece
				int x					= the x location of the top left corner of the small piece inside biggest
				int y					= the y locatino of the top left corner of the small piece inside biggest
Outputs:		int						= 1 if it is a fit, 0 otherwise
*/
int isAFit(int * biggest, int * smaller, int x, int y);

/*
Function Name: void negateValues
Description: This function acts on the target configuration array by negating the values in the piece array
given by the pointer smaller at position (x,y) this is used so that subproblems will not consider the location
that the piece is currently inhabiting as a potential location to be placed

Inputs:			int * biggest			= A pointer to the target configuration
				int * smaller			= A pointer to the piece
				int x					= the x location of the top left corner of the small piece inside biggest
				int y 					= the y location of the top left corner of the small piece inside biggest
Outputs:		void
*/
void negateValues(int * biggest, int * smaller, int x, int y);

/*
Function Name: void makeValuesPositive
Description: This function makes every value in the target configuration positive, this is used to
re initialize the target configuration after a full solution has been found

Inputs:			int * biggest			= A pointer to the target configuration
Outputs:		void
*/
void makeValuesPositive(int * biggest);

/*
Function Name: ArrayList * readSolutions
Description: This function takes the solution list struct generated by the solution finding routine,
which is basically a tree of node configurations that do not interfere with each other and are 
assumed to span the target configuraiton.  This adds one solution to the list of arrays

Inputs:			SolutionList * solutions= A pointer to the solutions list generated by solution
										finding routine
				ArrayList * list 		= A pointer to the list of solutions that is being generated
				int num					= Value of the number of pieces in a solution
Outputs:		ArrayList *				= A pointer to the first ArrayList struct
*/
ArrayList * readSolutions(SolutionList * solutions, ArrayList * list, int num);

/*
Function Name: int getNumberOfPieces
Description: This function returns the number of pieces in a list of ArrayList structs

Inputs:			ArrayList * pieces		= A pointer to the first ArrayList struct in the list
Outputs:		int						= The number of pieces in the list
*/
int getNumberOfPieces(ArrayList * pieces);

/*
Function Name: printSolutionFromArrayList
Description: This is used for debugging, it prints the first solution from an ArrayList struct

Inputs:			ArrayList * soln		= A pointer to the array list from which to print
				int num					= The number of pieces in the puzzle
Outputs:		void
*/
void printSolutionFromArrayList(ArrayList * soln, int num);

/*
Function Name: debugSolutionList
Description: This debugs a solution list by printing out all of the solutions in a solution list
tree.  this function is quite buggy

Inputs:			ArrayList * temporary	= A pointer to the array list to be printed from
				int p					= The number of pieces that make up the solution
Outputs:		void
*/
void debugSolutionList(ArrayList * temporary, int p);

/*
Function Name: ArrayList * ParseSolutionsList
Description: This function searches the entire solution list and writes each individual solution to
an array and stores that array in an ArrayList node which is connected to a list of ArrayList nodes.
This function makes use of readSolutions which will return one solution, and then merges all of these
solutions together into a usable list

Inputs:			SolutionList * solutions= A pointer to the solutionlist returned 
				int p					= the number of pieces that make up a solution
Outputs:		ArrayList *				= The arrays represent a full solution, they are linked together
										by a list of ArrayList structs.  A pointer to the first in this list
										is returned by this function
*/
ArrayList * parseSolutionsList(SolutionList * solutions, int p);

/*
Function Name: ArrayList * findSolutionsRotations
Description: This is the main solution routine, it examines each piece rotation for the first piece in the 
RotationList given through pieces, and calls findPartialSolutionRotations with the information it has on this piece.
Once it has found solutions lists for all of the possible rotations of a piece, it returns the merged 
SolutionList struct

Inputs:			int * biggest			= A pointer to the target configuration
				RotationList * pieces	= A pointer to the rotation list which contains the pieces allowed to be
										examined by this recursion
Outputs:		SolutionList *			= A pointer to the solution list corresponding to the problem it has just solved
*/
SolutionList * findSolutionsRotations(int * biggest, RotationList * pieces);

/*
Function Name: int * rotateLeft
Description: This function takes an int array representation of a piece and rotates it 90 degrees to the left.
It allocates memory on the heap to do this.

Inputs:			int * array				= A pointer to the array to be rotated	
Outputs:		int *					= A poitner to the rotated array
*/
int * rotateLeft(int * array);

/*
Function Name: int * flipHorizontal
Description: This function takes an int array representation of a piece and flips it across its y-axis.
It allocates memory on the heap to accomodate this

Inputs:			int * array				= A pointer to the array to be reflected
Outputs:		int *					= A pointer to the reflected array
*/
int * flipHorizontal(int * array);

/*
Function Name: RotationList * createRotationList
Description: This function takes the piece arrays stored in an ArrayList and creates RotationList
nodes for each of them so that every possible rotation and reflection of each piece is 
represented as an int array and accessable in a data structure.  The RotationList is the data
structure I use to do this and contains many pointers to int arrays

Inputs:			ArrayList * list		= A pointer to the first node in a list of ArrayList structs
										each of which contain an int array representaiton of one of 
										the puzzle pieces
Outputs:		RotationList *			= A pointer to the first RotationList node in the list which
										stores the information about the first puzzle piece
*/
RotationList * createRotationList(ArrayList * list);

/*
Function Name: ArrayList * solveWithRotations
Description: This is a wrapper function that is called by the main method.  It automates the entire file parsing, 
solution finding, isometry testing, and file writing routines given the name of the target file and
the user flag of whether rotations/reflections are allowed or not

Inputs:			char * file_name		= A pointer to the string naming the input file
				int rotation			= 1 to allow rotation/reflection, 0 otherwise
Outputs:		ArrayList * 			= A pointer to the first node in a list of ArrayList structures
										each of the arrays stores the solutions generated by parseSolutionsList
										This return value is useless after I modified my code to allow solutions
										To be written to a file
*/
ArrayList * solveWithRotations(char * file_name, int rotation);

/*
Function Name: void removeIsometricRotations
Description: This function examines each node in a list of RotaionList structs and for each, removes
(sets the pointers to NULL and frees the memory) the rotations that are isomorphic to other rotations 
within the node.  This eliminates calculations when finding solutions

Inputs:			RotationList * t		= A pointer to the first node in a list of RotationList nodes
										that are being considered by the function
Outputs:		void					= The function modifies the data by dereferencing the pointer		
*/
void removeIsometricRotations(RotationList * t);

/*
Function Name: void debugRotationsList
Description: Used purely for debugging, in theory this function prints all of the non isomorphic rotations
in the RotationList r.  It doesnt really work, not sure why I included this

Inputs:			RotationList * r		= A pointer to the rotation list being considered
Outputs:		void
*/
void debugRotationsList(RotationList * r);

/*
Function Name: SolutionList * findPartialSolutionRotations
Description: This function works with findSolutionRotations, and finds all the ways that a piece _cur_array_ 
can be placed inside of _biggest_ and for each of those it calls findSolutionsRotations on an altered
biggest array and pieces rotation list.  Once the subproblems have finished being solved, this function
merges the solutions together into a SolutionList which it returns

Inputs:			int * biggest			= A pointer to the target configuration
				int * cur_array			= A pointer to the piece array
				RotationList * pieces	= A pointer to the rotation list of all pieces available
				SolutionList * most_recent = this is the solution list that we'll update when the solutions
										   to subproblems are finally returned
				int ident				= Value of the ID field of the piece array we are currently considering
				int rotation			= The value of the rotation of the piece we are considering
Outputs:		Solution List *			= A pointer to the solution list we've created
*/
SolutionList * findPartialSolutionRotations(int * biggest, int * cur_array, RotationList * pieces, SolutionList * most_recent, int ident, int rotation);

/*
Function Name: void removeAllSolutionIsometries
Description: This is a wrapper function that removes all of the isometric solutions from a list
of ArrayList nodes each of which contains a well formed solution to the problem.  This makes
use of removeAllSolutionIsometries, areIsometric, areSame, and initializeIntArray

Inputs:			ArrayList * list		= A pointer to the ArrayList being considered
				int * biggest			= A pointer to the target configuration
				int num					= The number of puzzle pieces in the solution
				RotationList * pieces	= A pointer to the RotationList which stores the pieces
Outputs:		
*/
void removeAllSolutionIsometries(ArrayList * list, int * biggest, int num, RotationList * pieces);

/*
Function Name: int * initializeIntArray
Description: This function is used when testing for isometry.  Once we have a solution in array form
it tells us what piece (ID) is rotated how (1,-1,2,-2,3,-3,4,-4 for different rotations) and in what
x, y point it is located.  This function creates an int array which initializes the values of the
array to the ID of the piece that occupies that space in the array.  This allows the isometry
checking routine to make sure that even duplicate pieces are detected
	
Inputs:			ArrayList * list		= A pointer to the list of solutions
				int * array				= A pointer to the array that we want to initialize with the values
										of the most recent solution
				int width				= Value of the width of the target configuration
				int height				= Value of the height of the target configuration
				int num					= Number of pieces that are used to construct the solution
				RotationList * pieces	= A pointer to the beginning of the RotationList which stores
										all non isomorphic rotations of the available pieces
Outputs:		int *					= A pointer to the array that we've initialized
*/
int * initializeIntArray(ArrayList * list, int * array, int width, int height, int num, RotationList * pieces);

/*
Function Name: int areIsomorphic		
Description: This function returns 1 if the two int arrays are isomorphic to each other, 0 otherwise
This works by rotating the first array every possible way and comparing it to the other array
using the areSame method which returns whether the two array parameters are identical.  If no rotaion
of the first array results in the areSame method returning true, then the solutions are not isometric

Inputs:			int * array				= A pointer to the first array to consider
				int * temp				= A pointer to the second array to consider
				int num					= Value of the number of pieces in the puzzle
Outputs:		int						= 1 if the arrays are isomorphic, 0 otherwise
*/
int areIsomorphic(int * array, int * temp, int num);

/*
Function Name: int areSame
Description: This compares the values in two int arrays to determine if they are the same array 
or not.  This creates a mapping from one value in array to a value in temp, and if any of the values in 
array do not correspond to their correct value in temp, then the arrays are not the same

Inputs:			int * array				= A pointer to the first array to consider
				int * temp				= A pointer to the second array to consider
				int num					= The number of pieces in the puzzle
Outputs:		int						= 1 if the arrays are the same, 0 otherwise
*/
int areSame(int * array, int * temp, int num);

/*
Function Name: void removeNullSolutions
Description: This function goes through the arrays in list and determines which of the solutions
were eliminated by the isometry checking routine and eliminates them from the list

Inputs:			ArrayList * list		= A pointer to the list of ArrayList structs to consider
Outputs:		void					= the function modifies the data passed by pointer
*/
void removeNullSolutions(ArrayList * list);

/*
Function Name: ArrayList * finalize
Description: This function takes difficult to understand reprsentaiton of a solution
and creates an easy to understand representation for the python script.

Inputs:			ArrayList * readout 	= A pointer to the array list that contains the solutions
				RotationList * r		= A pointer to the rotation list with the pieces
				int * biggest			= A pointer to the target configuration
				int num					= The number of puzzle pieces
Outputs:		
*/
ArrayList * finalize(ArrayList * readout, RotationList * r, int * biggest, int num);

/*
Function Name: void write2DIntArrayToFile
Description: This function writes the contents of a 2D int array into a file

Inputs:			FILE * f				= A file pointer which specifies the file to be written into
				int * array				= A pointer to the int array to write
Outputs:		void
*/
void write2DIntArrayToFile(FILE * f, int * array);

/*
Function Name: int getPiecesSize
Description: This function is a duplicate that I didnt realize I made, it just determines the size of the int array

Inputs:			int * array				= A pointer to the array in question
Outputs:		int						= The size of the array
*/
int getPieceSize(int * array);

/*
Function Name: void writeNullSolFile
Description: This writes 0 to a file which it opens itself and checks that it is valid.  This is just to reduce
code reuse because writing 0 to a file is very common when things are not provided as expected

Inputs:			
Outputs:		
*/
void writeNullSolFile();


//---------------------------------------------------------------//
//																 //
//					   	  Main Function: 					   	 //
//																 //
//		- Opens the file in append mode to check that it 		 //
//        isnt empty, if so then return NULL immediately		 //
//		- Determine whether rotation/reflection is desired		 //
//		  or prohibeted											 //
//		- Call the solve routine with the proper arguments		 //
// 																 //
//---------------------------------------------------------------//
int main(int argc, char*argv[]) {
	
	FILE * test = fopen(argv[1], "a+");
	size_t size = ftell(test);
	if(size == 0) {
		writeNullSolFile();
		return -1;
	}
	
	int rotate = 0;
	if(argc > 2) {
		int arg = atoi( argv[2] );
		if(arg == 1) {
			rotate = 1;
		}
	}
	ArrayList * solutions = solveWithRotations(argv[1], rotate);
}


//---------------------------------------------------------------//
//																 //
// 					 File Input / Piece Finding 			     //
//																 //
//---------------------------------------------------------------//

int * openFileIntoArray(char * file_name) {
	FILE *input;
	char *mode = "r";
	input = fopen(file_name, mode);
	if(input == NULL) { 
		printf("Please provide a valid input file!\n");
		exit(1);
	}
	
	else {
		int ch = getc(input);
		Point first;
		Point * n = &first;
		while(ch != EOF) {
			n->value = ch;
			n->next = malloc(sizeof(Point));
			n = n->next;
			ch = getc(input);
		}
		n = &first;
		int row_length = 0;
		int rows = 1;
		int counter = 0;
		while(n->next != NULL) {
			if(n->value != 10) {
				counter++;
			}
			if(n->value == 10) {
				if(counter > row_length) {
					row_length = counter;
				}
				counter = 0;
				rows++;
			}
			n = n->next;
		}
		if(counter > row_length) {
			row_length = counter;
		}
		row_length++;
		int * to_return = malloc(row_length * rows * sizeof(int) + 2*sizeof(int));
		*(to_return) = rows;
		*(to_return + 1) = row_length;
		n = &first;
		int i = 0;
		while(i < rows) {
			int counters = 0;
			while(counters < row_length) {
				if(n->next == NULL) {
					*(to_return + i*row_length + counters + 2) = 32;
				}
				else if(n->value != 10) {
					*(to_return + i*row_length + counters + 2) = n->value;
					if(counters + 1 != row_length) {
						n = n->next; 
					}
				}
				else {
					*(to_return + i*row_length + counters + 2) = 32;
				}
				counters++;
			}
			if(n->next != NULL) {
				n = n->next;
			}
			i++;
		}		
		return to_return;
	}
}

int * findAndRemoveLargestArray(ArrayList * list) {
	int * big_ptr = list->array;
	int highest_so_far = numberOfTiles(list->array + 2, *(list->array +1), *(list->array));
	int i = 1;
	while(list->next != NULL) {
		list->next->id = malloc(sizeof(int *));
		*(list->next->id) = i;
		i++;
		int count = numberOfTiles(list->next->array + 2, *(list->next->array +1), *(list->next->array));
		if(count > highest_so_far) {
			highest_so_far = count;
			int * place_holder = list->next->array;
			list->next->array = big_ptr;
			big_ptr = place_holder;
		}		
		list = list->next;
	}
	return big_ptr;
}

int numberOfTiles(int * n, int width, int height) {
	int i = 0;
	int count = 0;
	while(i < width * height) {
		if(*(n+i) != 32) {
			count++;
		}
		i++;
	}
	return count;
}

ArrayList * findAllPieces(int * input_array, int rows, int row_length) {
	int * temp_array = cloneIntArray(input_array);
	
	ArrayList * start = malloc(sizeof(ArrayList));
	
	while(foundAllPieces(temp_array, row_length, rows) == 1) {
		int i = 0;
		int found = 0;
		while(found == 0 && i < rows * row_length) {
			if(*(temp_array + i) != 32) {
				found = 1;
			}
			else {
				i++;
			}
		}
		
		
		int y = i / row_length;
		int x = i - y*row_length;
		PieceNode first;
		first.value = *(temp_array + i);
		*(temp_array + i) = 32;
		first.x_coord = x;
		first.y_coord = y;
		examineNeighbors(temp_array, row_length, rows, &first);
		int * info = getInfo(&first, first.y_coord, first.y_coord, first.x_coord, first.x_coord);
		int top = *(info + 0);
		int bottom = *(info + 1);
		int left = *(info + 2);
		int right = *(info + 3);
		int array_height = bottom - top + 1;
		int array_width = right - left + 1;
		
		ArrayList * iterator = start;
		while(iterator->next != NULL) {
			iterator = iterator->next;
		}
		iterator->next = malloc(sizeof(ArrayList));
		iterator = iterator->next;
		
		
		iterator->array = malloc((array_height * array_width + 2) * sizeof(int));
		int temp_i = 0;
		while(temp_i < array_height * array_width + 2) {
			*(iterator->array + temp_i) = 32;
			temp_i++;
		}
		*(iterator->array + 0) = array_height;
		*(iterator->array + 1) = array_width;
		makePieceArray(iterator->array + 2, &first, left, top, array_width);
	}
	free(temp_array);
	return start->next;
}

void makePieceArray(int * array, PieceNode * n, int left_offset, int top_offset, int width) {
	int x_c = n->x_coord;
	int y_c = n->y_coord;
	int x = x_c - left_offset;
	int y = y_c - top_offset;
	*(array + y*width + x) = n->value;
	if(n->top != NULL) {
		makePieceArray(array, n->top, left_offset, top_offset, width);
	}
	if(n->bottom != NULL) {
		makePieceArray(array, n->bottom, left_offset, top_offset, width);
	}
	if(n->left != NULL) {
		makePieceArray(array, n->left, left_offset, top_offset, width);
	}
	if(n->right != NULL) {
		makePieceArray(array, n->right, left_offset, top_offset, width);
	}
}

int * getInfo(PieceNode * n, int cur_top, int cur_bottom, int cur_left, int cur_right) {
	if(n->x_coord < cur_left) {
		cur_left = n->x_coord;
	}
	if(n->x_coord > cur_right) {
		cur_right = n->x_coord;
	}
	if(n->y_coord < cur_top) {
		cur_top = n->y_coord;
	}
	if(n->y_coord > cur_bottom) {
		cur_bottom = n->y_coord;
	}
	if(n->top != NULL) {
		int * top_info = getInfo(n->top, cur_top, cur_bottom, cur_left, cur_right);
		if(*(top_info + 0) < cur_top) {
			cur_top = *(top_info + 0);
		}
		if(*(top_info + 1) > cur_bottom) {
			cur_bottom = *(top_info + 1);
		}
		if(*(top_info + 2) < cur_left) {
			cur_left = *(top_info + 2);
		}
		if(*(top_info + 3) > cur_right) {
			cur_right = *(top_info + 3);
		}
		free(top_info);
	}
	if(n->bottom != NULL) {
		int * bottom_info = getInfo(n->bottom, cur_top, cur_bottom, cur_left, cur_right);
		if(*(bottom_info + 0) < cur_top) {
			cur_top = *(bottom_info + 0);
		}
		if(*(bottom_info + 1) > cur_bottom) {
			cur_bottom = *(bottom_info + 1);
		}
		if(*(bottom_info + 2) < cur_left) {
			cur_left = *(bottom_info + 2);
		}
		if(*(bottom_info + 3) > cur_right) {
			cur_right = *(bottom_info + 3);
		}
		free(bottom_info);
	}
	if(n->left != NULL) {
		int * left_info = getInfo(n->left, cur_top, cur_bottom, cur_left, cur_right);
		if(*(left_info + 0) < cur_top) {
			cur_top = *(left_info + 0);
		}
		if(*(left_info + 1) > cur_bottom) {
			cur_bottom = *(left_info + 1);
		}
		if(*(left_info + 2) < cur_left) {
			cur_left = *(left_info + 2);
		}
		if(*(left_info + 3) > cur_right) {
			cur_right = *(left_info + 3);
		}
		free(left_info);
	}
	if(n->right != NULL) {
		int * right_info = getInfo(n->right, cur_top, cur_bottom, cur_left, cur_right);
		if(*(right_info + 0) < cur_top) {
			cur_top = *(right_info + 0);
		}
		if(*(right_info + 1) > cur_bottom) {
			cur_bottom = *(right_info + 1);
		}
		if(*(right_info + 2) < cur_left) {
			cur_left = *(right_info + 2);
		}
		if(*(right_info + 3) > cur_right) {
			cur_right = *(right_info + 3);
		}
		free(right_info);
	}
	int * to_return = malloc(4 * sizeof(int));
	*(to_return + 0) = cur_top;
	*(to_return + 1) = cur_bottom;
	*(to_return + 2) = cur_left;
	*(to_return + 3) = cur_right;
	return to_return;
}

void examineNeighbors(int * array, int width, int height, PieceNode * n) {
	int x = n->x_coord;
	int y = n->y_coord;
	
	//Check Top
	if(y != 0 && *(array + (y-1)*width + x) != 32 ) {
		PieceNode * new_node = malloc(sizeof(PieceNode));
		new_node->value = *(array + (y-1)*width + x);
		*(array + (y-1)*width + x) = 32;
		new_node->x_coord = x;
		new_node->y_coord = y-1;
		n->top = new_node;
		examineNeighbors(array, width, height, new_node);
	}
	else {
		n->top = NULL;
	}
	//Check Bottom
	if(y != (height-1) && *(array + (y+1)*width + x) != 32 ) {
		PieceNode * new_node1 = malloc(sizeof(PieceNode));
		new_node1->value = *(array + (y+1)*width + x);
		*(array + (y+1)*width + x) = 32;
		new_node1->x_coord = x;
		new_node1->y_coord = y+1;
		n->bottom = new_node1;
		examineNeighbors(array, width, height, new_node1);
	}
	else {
		n->bottom = NULL;
	}
	//Check Left
	if(x != 0 && *(array + y*width + x - 1) != 32 ) {
		PieceNode * new_node2 = malloc(sizeof(PieceNode));
		new_node2->value = *(array + y*width + x - 1);
		*(array + y*width + x - 1) = 32;
		new_node2->x_coord = x - 1;
		new_node2->y_coord = y;
		n->left = new_node2;
		examineNeighbors(array, width, height, new_node2);
	}
	else {
		n->left = NULL;
	}
	//Check Right
	if(x != (width-1) && *(array + y*width + x + 1) != 32 ) {
		PieceNode * new_node3 = malloc(sizeof(PieceNode));
		new_node3->value = *(array + y*width + x + 1);
		*(array + y*width + x + 1) = 32;
		new_node3->x_coord = x + 1;
		new_node3->y_coord = y;
		n->right = new_node3;
		examineNeighbors(array, width, height, new_node3);
	}
	else {
		n->right = NULL;
	}
}

//---------------------------------------------------------------//
//																 //
// 					 Finding Solutions 			     //
//																 //
//---------------------------------------------------------------//

ArrayList * solveWithRotations(char * file_name, int rotation) {
	
	int * input_array = openFileIntoArray(file_name); 
	int rows = *(input_array + 0);
	int row_length = *(input_array + 1);
	int * array = (input_array + 2);
	ArrayList * pieces_array = findAllPieces(array, rows, row_length); 
	int * biggest = findAndRemoveLargestArray(pieces_array); 
	ArrayList * pieces = pieces_array->next;
	
	int total_num = getPieceSize(biggest);
	ArrayList * to_get = pieces;
	int other = 0;
	while(to_get != NULL) {
		other += getPieceSize(to_get->array);
		to_get = to_get->next;
	}

	if(other != total_num) {
		FILE * file = fopen("c_output.txt", "w");
		if (file == NULL)
		{
		    printf("Error opening file!\n");
		    exit(1);
		}
		fprintf(file, "0\n");
		fclose(file);
		return NULL;
	}
	
	RotationList * r = createRotationList(pieces);
	RotationList * t = r;
	if(rotation == 1) {
		while(t!=NULL) {
			removeIsometricRotations(t);
			t = t->next;
		}
	}
	else {
		while(t!=NULL) {
			t->ho = NULL;
			t->ro = NULL;
			t->hro = NULL;
			t->rro = NULL;
			t->hrro = NULL;
			t->rrro = NULL;
			t->hrrro = NULL;
			t = t->next;
		}
	}
		
	SolutionList * solutions = findSolutionsRotations(biggest, r);
	if(solutions == NULL) {
		FILE * file = fopen("c_output.txt", "w");
		if (file == NULL)
		{
		    printf("Error opening file!\n");
		    exit(1);
		}
		fprintf(file, "0\n");
		fclose(file);
		return NULL;
	}
	int num_pieces = getNumberOfPieces(pieces);

	ArrayList * readout = parseSolutionsList(solutions, num_pieces);
	removeAllSolutionIsometries(readout, biggest, num_pieces, r);
	removeNullSolutions(readout);
	removeAllSolutionIsometries(readout, biggest, num_pieces, r);
	removeNullSolutions(readout);
	ArrayList * returnable = finalize(readout, r, biggest, num_pieces);
	
	FILE * file = fopen("c_output.txt", "w");
	if (file == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
	
	int num_solutions = 0;
	ArrayList * templist = returnable;
	while(templist != NULL) {
		num_solutions++;
		templist = templist->next;
	}
	
	fprintf(file, "%d\n", num_solutions);
	fprintf(file, "%d %d\n", *(biggest + 1), *(biggest));
	while(returnable != NULL) {
		write2DIntArrayToFile(file, returnable->array);
		fprintf(file, "\n");
		returnable = returnable->next;
	}
	
	fclose(file);
	
	return readout;
}

SolutionList * findPartialSolutionRotations(int * biggest, int * cur_array, RotationList * pieces, SolutionList * most_recent, int ident, int rot) {
	int width = *(biggest + 1);
	int height = *(biggest + 0);
	
	int i = 0;
	int piece_width = *(cur_array + 1);
	int piece_height = *(cur_array + 0);
	int id = ident;
	while (i <= (height - piece_height)) {
		int j = 0;
		while(j <= (width - piece_width)) {
			if(isAFit(biggest, cur_array, j, i)==0) {
				negateValues(biggest, cur_array, j, i);
				if(pieces->next != NULL) {
					if(most_recent->node != NULL) {
						most_recent->next = malloc(sizeof(SolutionList));
						most_recent = most_recent->next;
						most_recent->node = NULL;
						most_recent->next = NULL;
					}
					if(most_recent->node == NULL) {
						most_recent->node = malloc(sizeof(SolutionNode));
						most_recent->node->fields = NULL;
						most_recent->node->first = NULL;
						SolutionList * partial_solution = findSolutionsRotations(biggest, pieces->next);
						if(partial_solution == NULL) {
							free(most_recent->node);
							most_recent->node = NULL;
						}
						else {
							most_recent->node->fields = malloc(4 * sizeof(int));
							*(most_recent->node->fields) = id;
							*(most_recent->node->fields + 1) = j;
							*(most_recent->node->fields + 2) = i;
							*(most_recent->node->fields + 3) = rot;
							most_recent->node->first = malloc(sizeof(PointerList));
							most_recent->node->first->next = NULL;
							most_recent->node->first->solution = NULL;
							PointerList * cur_ptr = most_recent->node->first;
							int done = 0;
							while(done == 0) {
								if(partial_solution->next == NULL) {
									done = 1;
								}
								if(cur_ptr->solution == NULL && partial_solution->node != NULL){
									cur_ptr->solution = partial_solution->node;
									cur_ptr->next = NULL;
								}
								else if(partial_solution->node != NULL) {
									cur_ptr->next = malloc(sizeof(PointerList));
									cur_ptr = cur_ptr->next;
									cur_ptr->solution = partial_solution->node;
									cur_ptr->next = NULL;
								}
								partial_solution = partial_solution->next;
							}
						}
					}
					else {
						printf("most_recent->node: %p\n", most_recent->node);
					}
					negateValues(biggest, cur_array, j, i);
				}
				else {
					if(most_recent->node != NULL) {
						most_recent->next = malloc(sizeof(SolutionList));
						most_recent = most_recent->next;
						most_recent->next = NULL;
					}
					most_recent->node = malloc(sizeof(SolutionNode));
					most_recent->node->fields = malloc(4 * sizeof(int));
					*(most_recent->node->fields) = id;
					*(most_recent->node->fields + 1) = j;
					*(most_recent->node->fields + 2) = i;
					*(most_recent->node->fields + 3) = rot;
					most_recent->node->first = NULL;
					negateValues(biggest, cur_array, j, i);
				}
			}
			j++;
		}
		i++;
	}
	return most_recent;
}

SolutionList * findSolutionsRotations(int * biggest, RotationList * pieces) {
	int width = *(biggest + 1);
	int height = *(biggest + 0);

	SolutionList * first = malloc(sizeof(SolutionList));
	first->node = NULL;
	first->next = NULL;
	SolutionList * most_recent = first;
	int id = (pieces->id);
	
	int * o = pieces->o;
	int * ho = pieces->ho;
	int * rro = pieces->rro;
	int * hrro = pieces->hrro;
	if(o != NULL) {
		most_recent = findPartialSolutionRotations(biggest, o, pieces, most_recent, id, 1);
	}
	if(ho != NULL) {
		most_recent = findPartialSolutionRotations(biggest, ho, pieces, most_recent, id, -1);
	}
	if(rro != NULL) {
		most_recent = findPartialSolutionRotations(biggest, rro, pieces, most_recent, id, 3);
	}
	if(hrro != NULL) {
		most_recent = findPartialSolutionRotations(biggest, hrro, pieces, most_recent, id, -3);
	}
	
	int * ro = pieces->ro;
	int * hro = pieces->hro;
	int * rrro = pieces->rrro;
	int * hrrro = pieces->hrrro;
	
	if(ro != NULL) {
		most_recent = findPartialSolutionRotations(biggest, ro, pieces, most_recent, id, 2);
	}
	if(hro != NULL) {
		most_recent = findPartialSolutionRotations(biggest, hro, pieces, most_recent, id, -2);
	}
	if(rrro != NULL) {
		most_recent = findPartialSolutionRotations(biggest, rrro, pieces, most_recent, id, 4);
	}
	if(hrrro != NULL) {
		most_recent = findPartialSolutionRotations(biggest, hrrro, pieces, most_recent, id, -4);
	}

	if(first->node == NULL) {
		free(first);
		return NULL;
	}
	else {
		return first;
	}
}

RotationList * createRotationList(ArrayList * list) {
	RotationList * to_return = malloc(sizeof(RotationList));
	RotationList * a = to_return;
	while(list != NULL) {
		if(a->o != NULL) {
			a->next = malloc(sizeof(RotationList));
			a = a->next;
		}
		a->id = *(list->id);
		a->o = list->array;
		a->ro = rotateLeft(a->o);
		a->rro = rotateLeft(a->ro);
		a->rrro = rotateLeft(a->rro);
		a->ho = flipHorizontal(a->o);
		a->hro = flipHorizontal(a->ro);
		a->hrro = flipHorizontal(a->rro);
		a->hrrro = flipHorizontal(a->rrro);
		list = list->next;
	}
	return to_return;
}

int * flipHorizontal(int * array) {
	int height = *(array);
	int width = *(array + 1);
	int * to_return = malloc(width * height * sizeof(int)+2);
	int i = 0;
	while(i < width) {
		int j = 0;
		while(j < height) {
			*(to_return + (j)*width + (width-1-i) + 2) = *(array + j*width + i + 2);
			j++;
		}
		i++;
	}
	*to_return = height;
	*(to_return + 1) = width;
	return to_return;
}

int * rotateLeft(int * array) {
	int height = *array;
	int width = *(array + 1); 
	int * to_return = malloc(width * height * sizeof(int) + 2);
	int i = 0;
	while(i < width) {
		int j = 0;
		while(j < height) {
			*(to_return + (width - 1 - i)*height + (j) + 2) = *(array+ j*width + i + 2);
			j++;
		}
		i++;
	}
	to_return[0] = *(array + 1);
	*(to_return + 1) = height + 0;
	return to_return;
}

ArrayList * parseSolutionsList(SolutionList * solutions, int p) {
	ArrayList * readout = malloc(sizeof(ArrayList));
	readout->array = NULL;
	readout->next = NULL;
	if(solutions == NULL) {
		return NULL;
	}
	else {
		ArrayList * t = readSolutions(solutions, readout, p);
		while(solutions != NULL && solutions->node != NULL) {
			if(solutions->node->first == NULL) {
				solutions = solutions->next;
			}
			else if(solutions->node != NULL){
				t = readSolutions(solutions, t, p);
			}
		}
	}
	return readout;
}

ArrayList * readSolutions(SolutionList * solutions, ArrayList * list, int num) {
	ArrayList * m = list;
	if(list->array != NULL) {
		if(list->next == NULL) {
			list->next = malloc(sizeof(ArrayList));
			list = list->next;
			list->next = NULL;
		}
		else {
			list = list->next;
		}
	}
	list->array = malloc(4 * num * sizeof(int));
	int * temp = list->array;
	SolutionNode * node = solutions->node;
	PointerList ** f;
	while(node != NULL && node->first != NULL) {
		*(temp) = *(node->fields);
		*(temp+1) = *(node->fields + 1);
		*(temp+2) = *(node->fields + 2);
		*(temp+3) = *(node->fields + 3);
		temp = temp + 4;
		f = &(node->first);
		
		while((*f)->next != NULL) {
			f = &((*f)->next);
		}
		node = (*f)->solution;
	}
	if(node == NULL) {}
	else if(*(node->fields) == num) {
		*(temp) = *(node->fields);
		*(temp + 1) = *(node->fields + 1);
		*(temp + 2) = *(node->fields + 2);
		*(temp + 3) = *(node->fields + 3);
	}
	else {
		free(list->array);
		list->array = NULL;
	}
	*f = NULL;
	
	return list;
}

int getNumberOfPieces(ArrayList * pieces) {
	int i = 1;
	while(pieces->next != NULL) {
		i++;
		pieces = pieces->next;
	}
	return i;
}

SolutionList * findSolutions(int * biggest, ArrayList * pieces) {
	int width = *(biggest + 1);
	int height = *(biggest + 0);
	int i = 0;
	int * cur_array = pieces->array;
	int piece_width = *(cur_array + 1);
	int piece_height = *(cur_array + 0);
	int id = *(pieces->id);
	SolutionList * first = malloc(sizeof(SolutionList));
	first->node = NULL;
	first->next = NULL;
	SolutionList * most_recent = first;
	while (i <= (height - piece_height)) {
		int j = 0;
		while(j <= (width - piece_width)) {
			int * a = cloneIntArray(biggest);
			if(isAFit(biggest, cur_array, j, i)==0) {
				negateValues(biggest, cur_array, j, i);
				if(pieces->next != NULL) {
					if(most_recent->node != NULL) {
						most_recent->next = malloc(sizeof(SolutionList));
						most_recent = most_recent->next;
						most_recent->node = NULL;
						most_recent->next = NULL;
					}
					if(most_recent->node == NULL) {
						most_recent->node = malloc(sizeof(SolutionNode));
						most_recent->node->fields = NULL;
						most_recent->node->first = NULL;
						ArrayList * pmt = pieces;
						SolutionList * partial_solution = findSolutions(biggest, pieces->next);
						if(partial_solution == NULL) {
							free(most_recent->node);
							most_recent->node = NULL;
						}
						else {
							most_recent->node->fields = malloc(4 * sizeof(int));
							*(most_recent->node->fields) = id;
							*(most_recent->node->fields + 1) = j;
							*(most_recent->node->fields + 2) = i;
							*(most_recent->node->fields + 3) = 1;
							most_recent->node->first = malloc(sizeof(PointerList));
							most_recent->node->first->next = NULL;
							most_recent->node->first->solution = NULL;
							PointerList * cur_ptr = most_recent->node->first;
							int done = 0;
							while(done == 0) {
								if(partial_solution->next == NULL) {
									done = 1;
								}
								if(cur_ptr->solution == NULL){
									cur_ptr->solution = partial_solution->node;
									cur_ptr->next = NULL;
								}
								else {
									cur_ptr->next = malloc(sizeof(PointerList));
									cur_ptr = cur_ptr->next;
									cur_ptr->solution = partial_solution->node;
									cur_ptr->next = NULL;
								}
								partial_solution = partial_solution->next;
							}
						}
					}
					else {
						printf("most_recent->node: %p\n", most_recent->node);
					}
					negateValues(biggest, cur_array, j, i);
				}
				else {
					if(most_recent->node != NULL) {
						most_recent->next = malloc(sizeof(SolutionList));
						most_recent = most_recent->next;
						most_recent->next = NULL;
					}
					most_recent->node = malloc(sizeof(SolutionNode));
					most_recent->node->fields = malloc(4 * sizeof(int));
					*(most_recent->node->fields) = id;
					*(most_recent->node->fields + 1) = j;
					*(most_recent->node->fields + 2) = i;
					*(most_recent->node->fields + 3) = 1;
					most_recent->node->first = NULL;
					negateValues(biggest, cur_array, j, i);
				}
			}
			j++;
		}
		i++;
	}
	if(first->node == NULL) {
		free(first);
		return NULL;
	}
	else {
		return first;
	}
}

void negateValues(int * biggest, int * smaller, int x, int y) {
	int i = 0;
	int small_width = *(smaller + 1);
	int small_height = *(smaller + 0);
	int big_width = *(biggest + 1);
	int big_height = *(biggest + 0);
	smaller = smaller + 2;
	biggest = biggest + 2;
	while(i < small_width) {
		int j = 0;
		while( j < small_height) {
			int small_int = *(smaller + (j)*small_width + (i));
			if(small_int == 32) { 
				
			}
			else {
				*(biggest + (y+j)*big_width + (x+i)) = -1 * *(biggest + (y+j)*big_width + (x+i));
			}
			j++;
		}
		i++;
	}
}

int isAFit(int * biggest, int * smaller, int x, int y) {
	int i = 0;
	int small_width = *(smaller + 1);
	int small_height = *(smaller + 0);
	int big_width = *(biggest + 1);
	int big_height = *(biggest + 0);
	smaller = smaller + 2;
	biggest = biggest + 2;
	int ruined = 0;
	
	if(small_height > big_height || small_width > big_width) {
		return 1;
	}
	
	while(i < small_width && ruined==0) {
		int j = 0;
		while( j < small_height && ruined==0) {
			int small_int = *(smaller + (j)*small_width + (i));
			int big_int = *(biggest + (y+j)*big_width + (x+i));
			if(small_int == 32) { 
				
			}
			else if(small_int != big_int) {
				ruined = 1;
			}
			j++;
		}
		i++;
	}
	return ruined;
}

int getPieceSize(int * array) {
	int width = *(array + 1);
	int height = *(array);
	array = array + 2;
	int i = 0;
	int count = 0;
	while(i < width) {
		int j = 0;
		while(j < height) {
			if (*(array + j*width + i) != 32) {
				count++;
			}
			j++;
		}
		i++;
	}
	return count;
}

int foundAllPieces(int * array, int width, int height) {
	int done = 0;
	int i = 0;
	while(i < height) {
		if(done == 1) {
			break;
		}
		int j = 0;
		while(j < width) {
			if(*(array + i*width + j) != 32) {
				done = 1;
				break;
			}
			j++;
		}
		i++;
	}
	return done;
}

//---------------------------------------------------------------//
//																 //
// 					 Isometry Testing 			     //
//																 //
//---------------------------------------------------------------//

void removeNullSolutions(ArrayList * list) {
	ArrayList * last = list;
	ArrayList * t = list->next;
	while(t != NULL) {
		if(t->array != NULL) {
			last->next = t;
			last = t;
		}
		t = t->next;
	}
	last->next = NULL;
}

void removeAllSolutionIsometries(ArrayList * list, int * biggest, int num, RotationList * pieces) {
	int i = 0;
	int width = *(biggest + 1);
	int height = *(biggest);
	int * anchor = malloc(width * height * sizeof(int) + 2);
	int * temp = malloc(width * height * sizeof(int) + 2);
	while (list != NULL) {
		if(list->array == NULL) {}
		else {
			anchor = initializeIntArray(list, anchor, width, height, num, pieces);
			ArrayList * t = list->next;
			while(t != NULL) {
				if(t->array != NULL) {
					temp = initializeIntArray(t, temp, width, height, num, pieces);
					if(areIsomorphic(anchor, temp, num) == 1) {
						t->array = NULL;
					}
				}
				t = t->next;
			}
		}
		list = list->next;
	}
}

int * initializeIntArray(ArrayList * list, int * array, int width, int height, int num, RotationList * pieces) {
	int * to_return = array;
	*(array) = height;
	*(array + 1) = width;
	array = array + 2;
	int i = 0;
	while(i < width * height) {
		*(array + i) = 0;
		i++;
	}
	i = 0;
	int * a = list->array;
	while(i < num) {
		RotationList * r = pieces;
		int Id = *(a+0);
		int x = *(a+1);
		int y = *(a+2);
		int rot = *(a+3);
		if((r->id) != Id) {
			while((r->id) != Id) {
				r = r->next;
			}
		}
		int * the_piece;
		if(rot == 1) {
			the_piece = r->o;
		}
		if(rot == -1) {
			the_piece = r->ho;
		}
		if(rot == 2) {
			the_piece = r->ro;
		}
		if(rot == -2) {
			the_piece = r->hro;
		}
		if(rot == 3) {
			the_piece = r->rro;
		}
		if(rot == -3) {
			the_piece = r->hrro;
		}
		if(rot == 4) {
			the_piece = r->rrro;
		}
		if(rot == -4) {
			the_piece = r->hrrro;
		}
		int piece_width = *(the_piece+1);
		int piece_height = *(the_piece);
		the_piece = the_piece + 2;
		int m = 0;
		while(m < piece_height) {
			int n = 0;
			while(n < piece_width) {
				if(*(the_piece + m*piece_width + n)!=32){
					*(array + (y + m)*width + (x + n)) = Id;
				}
				n++;
			}
			m++;
		}
		a = a + 4;
		i++;
	}
	return to_return;
}

int areIsomorphic(int * array, int * temp, int num) {
	int to_return = 0;
	int * ho = flipHorizontal(array);
	int * ro = rotateLeft(array);
	int * hro = flipHorizontal(ro);
	int * rro = rotateLeft(ro);
	int * hrro = flipHorizontal(rro);
	int * rrro = rotateLeft(rro);
	int * hrrro = flipHorizontal(rrro);
	if(areSame(array, temp, num) == 1) {
		to_return = 1;
	}
	else if(areSame(ho, temp, num) == 1) {
		to_return = 1;
	}
	else if(areSame(ro, temp, num) == 1) {
		to_return = 1;
	}
	else if(areSame(hro, temp, num) == 1) {
		to_return = 1;
	}
	else if(areSame(rro, temp, num) == 1) {
		to_return = 1;
	}
	else if(areSame(hrro, temp, num) == 1) {
		to_return = 1;
	}
	else if(areSame(rrro, temp, num) == 1) {
		to_return = 1;
	}
	else if(areSame(hrrro, temp, num) == 1) {
		to_return = 1;
	}
		ho = NULL;
		ro = NULL;
		hro = NULL;
		rro = NULL;
		hrro = NULL;
		rrro = NULL;
		hrrro = NULL;
		return to_return;;
}

int areSame(int * array, int * temp, int num) {
	if(array == NULL) {
	}
	int * check = malloc((num + 1)*sizeof(int));
	int alt = 0;
	while (alt < num + 1) {
		*(check + alt) = 0;
		alt++;
	}
	int a_width = *(array + 1);
	int t_width = *(temp + 1);
	if(a_width != t_width) {
		return 0;
	}
	int a_height = *(array);
	int t_height = *(temp);
	if(a_height != t_height) {
		return 0;
	}
	array += 2;
	temp += 2;
	int i = 0;
	while(i < a_height) {
		int j = 0;
		while(j < a_width) {
			int id = *(array + i*a_width + j);
			if(*(check + id) == 0) {
				*(check + id) = *(temp + i*a_width + j);
			}
			else if(*(check + id) != *(temp + i*a_width + j)) {
				return 0;
			}
			j++;
		}
		i++;
	}
	free(check);
	check=NULL;
	return 1;
}

void removeIsometricRotations(RotationList * t) {
	if(isAFit(t->hrrro,t->rrro,0,0)==0 || isAFit(t->hrrro,t->hro,0,0)==0 || isAFit(t->hrrro,t->ro,0,0)==0 ||
		isAFit(t->hrrro,t->rro,0,0)==0 || isAFit(t->hrrro,t->ho,0,0)==0 || isAFit(t->hrrro,t->hrro,0,0)==0 ||
			isAFit(t->hrrro,t->o,0,0)==0) {
				t->hrrro = NULL;
			}
	if(isAFit(t->rrro,t->hro,0,0)==0 || isAFit(t->rrro,t->ro,0,0)==0 ||
		isAFit(t->rrro,t->rro,0,0)==0 || isAFit(t->rrro,t->ho,0,0)==0 || isAFit(t->rrro,t->hrro,0,0)==0 ||
			isAFit(t->rrro,t->o,0,0)==0) {
				t->rrro = NULL;
			}
	if(isAFit(t->hrro,t->hro,0,0)==0 || isAFit(t->hrro,t->ro,0,0)==0 ||
		isAFit(t->hrro,t->rro,0,0)==0 || isAFit(t->hrro,t->ho,0,0)==0 ||
			isAFit(t->hrro,t->o,0,0)==0) {
				t->hrro = NULL;
			}	
	if(isAFit(t->rro,t->hro,0,0)==0 || isAFit(t->rro,t->ro,0,0)==0 ||
		isAFit(t->rro,t->ho,0,0)==0 ||
			isAFit(t->rro,t->o,0,0)==0) {
				t->rro = NULL;
			}		
	if(isAFit(t->hro,t->ro,0,0)==0 ||
		isAFit(t->hro,t->ho,0,0)==0 ||
			isAFit(t->hro,t->o,0,0)==0) {
				t->hro = NULL;
			}	
	if(isAFit(t->ro,t->ho,0,0)==0 ||
			isAFit(t->ro,t->o,0,0)==0) {
				t->ro = NULL;
			}	
	if(isAFit(t->ho, t->o,0,0)==0) {
		t->ho = NULL;
	}
}

//---------------------------------------------------------------//
//																 //
// 					 Writing To File 			     //
//																 //
//---------------------------------------------------------------//

ArrayList * finalize(ArrayList * readout, RotationList * pieces, int * biggest, int num) {
	int width = *(biggest + 1);
	int height = *(biggest);
	ArrayList * to_return = malloc(sizeof(ArrayList));
	ArrayList * start = to_return;
	to_return->next = NULL;
	to_return->array = NULL;
	while(readout != NULL) {
		if(to_return->array != NULL) {
			to_return->next = malloc(sizeof(ArrayList));
			to_return = to_return->next;
			to_return->next = NULL;
			to_return->array = NULL;
		}
		int * a = readout->array;
		to_return->array = malloc( ( width * height + 2 ) * sizeof(int));
		int i = 0;
		while ( i < width * height + 2) {
			*(to_return->array + i) = 0;
			i++;
		}
		*(to_return->array)=height;
		*(to_return->array + 1)=width;
		int * ar = to_return->array + 2;
		
		i=0;
		while(i<num) {
			RotationList * r = pieces;
			int Id = *(a+0);
			int x = *(a+1);
			int y = *(a+2);
			int rot = *(a+3);
			while((r->id) != Id) {
				r = r->next;
			}
			int * the_piece;
			if(rot == 1) {
				the_piece = r->o;
			}
			if(rot == -1) {
				the_piece = r->ho;
			}
			if(rot == 2) {
				the_piece = r->ro;
			}
			if(rot == -2) {
				the_piece = r->hro;
			}
			if(rot == 3) {
				the_piece = r->rro;
			}
			if(rot == -3) {
				the_piece = r->hrro;
			}
			if(rot == 4) {
				the_piece = r->rrro;
			}
			if(rot == -4) {
				the_piece = r->hrrro;
			}
			int piece_width = *(the_piece+1);
			int piece_height = *(the_piece);
			the_piece = the_piece + 2;
			int m = 0;
			while(m < piece_height) {
				int n = 0;
				while(n < piece_width) {
					if(*(the_piece + m*piece_width + n)!=32){
						*(ar + (y + m)*width + (x + n)) = Id;
					}
					n++;
				}
				m++;
			}
			a = a + 4;
			i++;
		}
		readout = readout->next;
	}
	return start;
}

void writeNullSolFile() {
	FILE * file = fopen("c_output.txt", "w");
	if (file == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
	fprintf(file, "0\n");
	fclose(file);
}

void write2DIntArrayToFile(FILE * f, int * array) {
	int width = *(array + 1);
	int height = *array;
	array = array+2;
	int i = 0;
	while(i < height) {
		int j = 0;
		while(j < width) {
			fprintf(f, "%d ", *(array + i*width + j));
			j++;
		} 
		fprintf(f, "\n");
		i++;
	}
}

//---------------------------------------------------------------//
//																 //
// 					 Random / Debugging 			     //
//																 //
//---------------------------------------------------------------//

void debugSolutionList(ArrayList * temporary, int p) {
	while(temporary != NULL && temporary->array != NULL) {
		printSolutionFromArrayList(temporary, p);
		temporary = temporary->next;
	}
}

void printSolutionFromArrayList(ArrayList * soln, int num) {
	printf("\n");
	int i = 0;
	int * temp = soln->array;
	while(i < num) {
		printf("%d %d %d %d\n", *(temp),*(temp+1),*(temp+2),*(temp + 3));
		temp = temp + 4;
		i++;
	}
}

int * cloneIntArray(int * array) {
	int height = *array;
	int width =  *(array + 1);
	int i = 0;
	int * to_return = malloc(width * height * sizeof(int) + 2);
	while ( i < height * width + 2) {
		*(to_return + i) = *(array + i);
		i++;
	}

	return to_return;
} 

void print2DIntArray(int * array, int width, int height) {
	int i = 0;
	while(i < height) {
		int j = 0;
		while(j < width) {
			if(*(array + i*width + j) == 32) {
				printf("   ");
			}
			else {
				printf("%d ", *(array + i*width + j));
			}
			j++;
		}
		printf("\n");
		i++;
	}
} 