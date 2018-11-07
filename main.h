// Ray Smith - rsmith52 - rays - Nate Hoffman - nhoffman5 -nhoffman
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
/* Include Guards ^^^ */
#include <stdio.h>
/* Prototypes for Variables */

const int LINE_BUFF_SIZE;
const int BASE_LIST_SIZE;

FILE * makefile;

typedef struct Parse_Inputs {
	int line_num;
	char * file_line;
} Parse_Input;

typedef struct Parse_Outputs {
	int line_num;
	char line_type;
	char ** file_line_array;
} Parse_Output;

typedef struct Spec_Representations {
	int id;
	char * target;
	int num_dependencies;
	char ** dependencies;
	int curr_commands_size;
	int num_commands;
	char *** commands;
} Spec_Representation;

#endif
