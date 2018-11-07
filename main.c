#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/sysmacros.h>
#include "main.h"
#include "text_parsing.h"
#include "build_spec_repr.h"
#include "build_spec_graph.h"
#include "proc_creation_prog_exe.h"

const int LINE_BUFF_SIZE = 1024;
const int BASE_LIST_SIZE = 10;

int main (int argc, char * argv[]) {
	// Determine if File Exists
	if ((access("makefile", F_OK) != -1) || (access("Makefile", F_OK) != -1)) {
		// File Exists
		if (access("makefile", R_OK) != -1) {
			// makefile Can Be Read
			makefile = fopen("makefile", "r");
		} else if (access("Makefile", R_OK) != -1) {
			// Makefile Can Be Read
			//makefile = fopen("Makefile", "r");
			makefile = fopen("make2", "r");
		} else {
			// File Cannot Be Read
			fprintf(stderr, "Makefile Cannot Be Read\n");
		}
	} else {
		// File Does Not Exist
		fprintf(stderr, "Makefile Not Found\n");
	}

	// Feed Lines of Makefile to Text Parser + Build Spec_Representations
	Spec_Representation ** nodes = malloc(sizeof(Spec_Representation) * BASE_LIST_SIZE);
	int line_number = 1;
	int spec_rep_index = -1;
	int curr_spec_repr_size = BASE_LIST_SIZE;
	Spec_Representation ** temp;
	while (1) {
		// Get Next Line from File
		char * detect_too_long = malloc(sizeof(char) * (LINE_BUFF_SIZE + 1));
		char * curr_line = malloc(sizeof(char) * LINE_BUFF_SIZE);
		if (fgets(detect_too_long, LINE_BUFF_SIZE + 1, makefile) == NULL) {
			// End of File Reached
			break;
		}
		if ((int)strlen(detect_too_long) > LINE_BUFF_SIZE - 1) {
			fprintf(stderr, "%d: Line exceeds buffer of: %d\n", line_number, LINE_BUFF_SIZE);
			exit(1);
		} else {
			for (int i = 0; i < LINE_BUFF_SIZE; i++) {
				curr_line[i] = detect_too_long[i];
			}
		}
		
		// Create Parse_Input Object
		Parse_Input * parse_input = malloc(sizeof(Parse_Input));
		parse_input->line_num = line_number++;
		parse_input->file_line = curr_line;
		
		// Create Parse_Output Object
		Parse_Output * parse_output;
		parse_output = ParseText(parse_input);
	

		// Throw Error if ParseText Found One
		if (parse_output == NULL) {
			// Error in Line
			fprintf(stderr, "%d: Invalid line: %s", parse_input->line_num, parse_input->file_line);		
			exit(1);
		}
	
		// Build Spec_Representation
		// Create new Spec_Representation when Target Line 't'
		else if (parse_output->line_type == 't') {
			// Allocate More Memory for List if Needed
                	if (++spec_rep_index > (curr_spec_repr_size - 1)) {
                        	curr_spec_repr_size = curr_spec_repr_size * 2;
                        	temp = realloc(nodes, sizeof(Spec_Representation) * curr_spec_repr_size);
                        	if (temp == NULL) {
                                	fprintf(stderr, "Memory Reallocation Failed.\n");
                        	} else {
                                	printf("Reallocated Memory Successfully.\n");
                                	nodes = temp;
                        	}
                	}
			nodes[spec_rep_index] = CreateSpec(parse_output->file_line_array, spec_rep_index);
		
		// Add Command to Spec_Representation when Command Line 'c'
		} else if (parse_output->line_type == 'c') {
			AddCommand(nodes[spec_rep_index], parse_output->file_line_array);
		
		// Ignore Line When it is empty
		} else if (parse_output->line_type == 'e') {
		
		// If Anything Else Happened Throw Error
		} else {
			fprintf(stderr, "%d: Invalid line: %s", parse_input->line_num, parse_input->file_line);
			exit(1);
		}
	}
	
	// Build Graph of Spec_Representations
	Spec_Graph * spec_graph = BuildSpecGraph(nodes, spec_rep_index + 1);

	// Get Starting Point
	Spec_Representation * start_spec;
	if (argc == 1) {
		// No command line arguments - Start with 1st Spec_Representation
		start_spec = nodes[0];
	} else if (argc == 2) {
		// Get Spec_Representation - Start there
		start_spec = GetSpec(argv[1], nodes);
		if (start_spec == NULL) {
			fprintf(stderr, "No Build Specification Found: %s\n", argv[1]);
			exit(1);
		}	
	} else {
		// Invalid number of arguments given - Print Error
		fprintf(stderr, "Incorrect Number of Arguments Given\n");
		exit(1);
	}

	// Traverse Graph to Get Build Order
	Spec_Representation ** build_order_list = TraverseGraph(spec_graph, start_spec);

	// Create and Execute Processes if Necessary
	int should_build = 0;
	struct stat file_stats;
	struct stat dep_file_stats;
	struct timespec target_time;
	struct timespec dependency_time;
	for (int i = 0; i < spec_graph->dimension; i++) {
		should_build = 0;
		if (build_order_list[i] == NULL) {
			// Done with List Early (Not Full Build)
			break;
		}
		// Check if Needs Build
		if (access(build_order_list[i]->target, F_OK) != -1) {
			// File Exists, Check if Out of Date
			// Get Target Time
			if (lstat(build_order_list[i]->target, &file_stats) == -1) {
				fprintf(stderr, "Reading from Stat Failed.\n");
				exit(1);
			}
			target_time = file_stats.st_mtim;
			// Get Each Dependency Time
			for (int j = 0; j < build_order_list[i]->num_dependencies; j++) {
				if (lstat(build_order_list[i]->dependencies[j], &dep_file_stats) == -1) {
					fprintf(stderr, "Reading from Stat Failed.\n");
					exit(1);
				}
				dependency_time = dep_file_stats.st_mtim;
				if (dependency_time.tv_sec < target_time.tv_sec) {
					// Dependency Newer, Needs Update
					should_build = 1;
					break;
				}
			}
		} else {
			// File Does Not Exist, Definitely Build
			should_build = 1;
		}
		if (should_build) {
			// Call Build Command
			printf("Build Command Call: %s\n", build_order_list[i]->target);
			ExecuteBuild(build_order_list[i]);
		} else {
			printf("Don't Build: %s\n", build_order_list[i]->target);
		}
	}
	
	return 0;


}
