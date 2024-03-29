#include <stdlib.h>
#include <string.h>
#include "build_spec_repr.h"

Spec_Representation * CreateSpec(char ** file_line, int index) {
	// Allocate Memory for Spec_Representation
	Spec_Representation * new_spec_repr = malloc(sizeof(Spec_Representation));
	new_spec_repr->id = index;

	// Set Target of Spec_Representation
	new_spec_repr->target = malloc(sizeof(char) * LINE_BUFF_SIZE);
	new_spec_repr->target = file_line[0];
	
	// Allocate Memory for Dependencies
	new_spec_repr->dependencies = malloc(sizeof(char *) * LINE_BUFF_SIZE); 
	new_spec_repr->num_dependencies = 0;
	while (file_line[new_spec_repr->num_dependencies + 1][0] != '\0') {
		new_spec_repr->dependencies[new_spec_repr->num_dependencies] = file_line[new_spec_repr->num_dependencies + 1];
		new_spec_repr->num_dependencies++;
	}

	// Allocate Memory for Commands
	new_spec_repr->num_commands = 0;
	new_spec_repr->commands = malloc(sizeof(char **) * BASE_LIST_SIZE);
	new_spec_repr->curr_commands_size = BASE_LIST_SIZE;
	return new_spec_repr;
}

void AddCommand(Spec_Representation * spec_repr, char ** file_line) {
	// Allocate More Memory for List if Needed
        if (++(spec_repr->num_commands) > (spec_repr->curr_commands_size)) {
        	spec_repr->curr_commands_size = spec_repr->curr_commands_size * 2;
                char *** temp = realloc(spec_repr->commands, sizeof(char **) * spec_repr->curr_commands_size);
                if (temp == NULL) {
                	fprintf(stderr, "Memory Reallocation Failed.\n");
                } else {
                	//printf("Reallocated Memory Successfully.\n");
                        spec_repr->commands = temp;
                }
        }
	
	// Add Command
	spec_repr->commands[spec_repr->num_commands - 1] = file_line;

	return;
}

Spec_Representation * GetSpec(char * target, Spec_Representation ** nodes) {
	int index = 0;
	// Go through all nodes to see if matching target is found
	while (1) {
		if (nodes[index] == NULL) {
			// Node with matching target not found, return NULL
			break;
		} else if (strcmp(target, nodes[index]->target) == 0) {
			// Return node with matching target
			return nodes[index];
		}
		index++;
	}
	return NULL;
}

int GetSpecIndex(char * target, Spec_Representation ** nodes) {
	int index = 0;
        // Go through all nodes to see if matching target found
	while (1) {
                if (nodes[index] == NULL) {
                        // Node with matching target not found, return -1
			break;
                } else if (strcmp(target, nodes[index]->target) == 0) {
                        // Return node with matching target's index
			return index;
                }
                index++;
        }
        return -1;
}
