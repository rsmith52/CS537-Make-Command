#include <stdlib.h>
#include "build_spec_graph.h"

Spec_Graph * BuildSpecGraph(Spec_Representation ** nodes, int num_nodes) {
	
	// Initialize Graph Struct
	Spec_Graph * graph = malloc(sizeof(Spec_Graph));
	graph->graph_nodes = nodes;
	graph->adj_matrix = malloc((sizeof(int) * num_nodes) * num_nodes);
	
	// Build Adjacency Matrix
	// [i][j] = 1 -> i depends on j
	// [i][j] = 0 -> i does not depend on j
	Spec_Representation * temp_spec;
	Spec_Representation * temp_spec_2;
	char * dependency = malloc (sizeof(char) * LINE_BUFF_SIZE);
	for (int i = 0; i < num_nodes; i++) {
		temp_spec = nodes[i];
		for (int d = 0; d < temp_spec->num_dependencies; d++) {
			dependency = nodes[i]->dependencies[d];
			for (int j = 0; j < num_nodes; j++) {
				
			}
		}
	}
	
	return graph;
}
