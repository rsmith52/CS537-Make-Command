#include <stdlib.h>
#include <string.h>
#include "build_spec_graph.h"

Spec_Graph * BuildSpecGraph(Spec_Representation ** nodes, int num_nodes) {
	
	// Initialize Graph Struct
	Spec_Graph * graph = malloc(sizeof(Spec_Graph));
	graph->graph_nodes = nodes;
	graph->adj_matrix = malloc(sizeof(int *) * num_nodes);
	for (int i = 0; i < num_nodes; i++) {
		graph->adj_matrix[i] = malloc(sizeof(int) * num_nodes);
	}	
	
	// Build Adjacency Matrix
	// [i][j] = 1 -> i depends on j
	// [i][j] = 0 -> i does not depend on j
	Spec_Representation * temp_spec;
	Spec_Representation * temp_spec_2;
	char * dependency = malloc (sizeof(char) * LINE_BUFF_SIZE);
	int found = 0;
	for (int i = 0; i < num_nodes; i++) {
		// Set row
		temp_spec = nodes[i];
		for (int j = 0; j < num_nodes; j++) {
			// Set column, assume not dependent of i
			temp_spec_2 = nodes[j];
			found = 0;
			for (int k = 0; k < temp_spec->num_dependencies; k++) {
				// Check all of the things i depends on with what j is
				dependency = nodes[i]->dependencies[k];
				if (strcmp(dependency, temp_spec_2->target) == 0) {
					found = 1;
					break;
				}				
			}
			// Mark if dependent or not
			if (found) {
				graph->adj_matrix[i][j] = 1;
			} else {
				graph->adj_matrix[i][j] = 0;
			}
		}
	}
	
	return graph;
}

