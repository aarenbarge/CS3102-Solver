#include <stdio.h>
#include <stdlib.h>
#include <float.h>

//---------------------------------------------------------------//
// Structs //
//---------------------------------------------------------------//
typedef struct Point {
   int value;
   struct Point * next;
} Point;
typedef struct ArrayList {
	int id;
	int rotation;
	int * array;
	struct ArrayList * next;
} ArrayList;
typedef struct PieceNode {
	struct PieceNode * top;
	struct PieceNode * left;
	struct PieceNode * right;
	struct PieceNode * bottom;
	int value;
	int x_coord;
	int y_coord;
} PieceNode;
typedef struct SolutionNode {
	int * fields;
	struct PointerList * first;
}SolutionNode;
typedef struct PointerList {
	struct SolutionNode * solution;
	struct PointerList * next;
}PointerList;
typedef struct SolutionList {
	struct SolutionNode * node;
	struct SolutionList * next;
}SolutionList;
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
// Forward Declarations //
//---------------------------------------------------------------//
int * openFileIntoArray(char * file_name);
void print2DIntArray(int * array, int width, int height);
int * cloneIntArray(int * array);
ArrayList * findAllPieces(int * input_array, int rows, int row_length);
int foundAllPieces(int * array, int width, int height);
void examineNeighbors(int * array, int width, int height, PieceNode * n);
int * getInfo(PieceNode * n, int cur_top, int cur_bottom, int cur_left, int cur_right);
void makePieceArray(int * array, PieceNode * n, int left_offset, int top_offset, int width);
int * findAndRemoveLargestArray(ArrayList * list);
int numberOfTiles(int * n, int width, int height);
void debugPieces(int * biggest, ArrayList * pieces);
SolutionList * findSolutions(int * biggest, ArrayList * pieces);
int isAFit(int * biggest, int * smaller, int x, int y);
void negateValues(int * biggest, int * smaller, int x, int y);
void makeValuesPositive(int * biggest);
ArrayList * readSolutions(SolutionList * solutions, ArrayList * list, int num);
int getNumberOfPieces(ArrayList * pieces);
void printSolutionFromArrayList(ArrayList * soln, int num);
void debugSolutionList(ArrayList * temporary, int p);
ArrayList * parseSolutionsList(SolutionList * solutions, int p);
ArrayList * solveNoRotations(char * file_name);
SolutionList * findSolutionsRotations(int * biggest, RotationList * pieces);
int * rotateLeft(int * array);
int * flipHorizontal(int * array);
RotationList * createRotationList(ArrayList * list);
ArrayList * solveWithRotations(char * file_name, int rotation);
void removeIsometricRotations(RotationList * t);
void debugRotationsList(RotationList * r);
SolutionList * findPartialSolutionRotations(int * biggest, int * cur_array, RotationList * pieces, SolutionList * most_recent, int ident, int rotation);
void removeAllSolutionIsometries(ArrayList * list, int * biggest, int num, RotationList * pieces);
int * initializeIntArray(ArrayList * list, int * array, int width, int height, int num, RotationList * pieces);
int areIsomorphic(int * array, int * temp, int num);
int areSame(int * array, int * temp, int num);
void removeNullSolutions(ArrayList * list);
ArrayList * finalize(ArrayList * readout, RotationList * r, int * biggest, int num);
void write2DIntArrayToFile(FILE * f, int * array);

//Global Variable :(
RotationList * all_rotations;






//---------------------------------------------------------------//
// Functions //
//---------------------------------------------------------------//
int main(int argc, char*argv[]) {
	int rotate = 0;
	if(argc > 2) {
		int arg = atoi( argv[2] );
		if(arg == 1) {
			rotate = 1;
		}
	}
	printf("rotated: %d\n", rotate);
	ArrayList * solutions = solveWithRotations(argv[1], rotate);
}

ArrayList * solveWithRotations(char * file_name, int rotation) {
	int * input_array = openFileIntoArray(file_name);
	int rows = *(input_array + 0);
	int row_length = *(input_array + 1);
	int * array = (input_array + 2);
	ArrayList * pieces_array = findAllPieces(array, rows, row_length);
	int * biggest = findAndRemoveLargestArray(pieces_array);
	ArrayList * pieces = pieces_array->next;
	debugPieces(biggest, pieces);
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
	all_rotations = r;
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
			while(r->id != Id) {
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
		if(r->id != Id) {
			while(r->id != Id) {
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
	int * check = malloc(num + 1);
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
	return 1;
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
	int id = pieces->id;
	
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

RotationList * createRotationList(ArrayList * list) {
	RotationList * to_return = malloc(sizeof(RotationList));
	RotationList * a = to_return;
	while(list != NULL) {
		if(a->o != NULL) {
			a->next = malloc(sizeof(RotationList));
			a = a->next;
		}
		a->id = list->id;
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
	int id = pieces->id;
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

int * findAndRemoveLargestArray(ArrayList * list) {
	int * big_ptr = list->array;
	int highest_so_far = numberOfTiles(list->array + 2, *(list->array +1), *(list->array));
	int i = 1;
	while(list->next != NULL) {
		list->next->id = i;
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
		int * total_array = malloc(array_height * array_width * sizeof(int) + 2);
		int temp_i = 0;
		while(temp_i < array_height * array_width + 2) {
			*(total_array + temp_i) = 32;
			temp_i++;
		}
		*(total_array + 0) = array_height;
		*(total_array + 1) = array_width;
		makePieceArray(total_array + 2, &first, left, top, array_width);
		ArrayList * iterator = start;
		while(iterator->next != NULL) {
			iterator = iterator->next;
		}
		ArrayList * next_it = malloc(sizeof(ArrayList));
		iterator->next = next_it;
		next_it->array = total_array;
	}
	return start->next;
	free(temp_array);
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

void debugPieces(int * biggest, ArrayList * pieces) {
	printf("\n\nThe Biggest Piece:\n\n");                                  
	print2DIntArray(biggest + 2, *(biggest + 1), *biggest);                
	printf("\n\nThe Rest of the Pieces:\n\n");                             
	int test = 1;                                                          
	ArrayList * n = pieces;                                                
	while(test == 1) {                                                     
		print2DIntArray((n->array) + 2, *(n->array + 1), *(n->array + 0)); 
		printf("\n\n");                                                    
		if(n->next == NULL) {                                              
			test = 0;
		}
		else {
			n = n->next;
		}
	}
}
