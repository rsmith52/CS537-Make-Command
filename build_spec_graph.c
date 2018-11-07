#include <stdlib.h>
#include <string.h>
#include "build_spec_graph.h"

Spec_Graph * BuildSpecGraph(Spec_Representation ** nodes, int num_nodes) {
	
	// Initialize Graph Struct
	Spec_Graph * graph = malloc(sizeof(Spec_Graph));
	graph->graph_nodes = nodes;
	graph->dimension = num_nodes;
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

int ContainsCycles(Spec_Graph * graph) {
	// Check Simple Case, Nodes Depend on Each Other
	for (int i = 0; i < graph->dimension; i++) {
		for (int j = 0; j < graph->dimension; j++) {
			if (graph->adj_matrix[i][j] == 1 && graph->adj_matrix[j][i] == 1) {
				// 2 Nodes Depend on Each Other
				return 1;
			}
		}
	}
	// Advanced Case
	// Create Topological Ordering List
	Spec_Representation ** end_list = malloc(sizeof(Spec_Representation *) * graph->dimension);
	// Unmark All Nodes
	for (int i = 0; i < graph->dimension; i++) {
		graph->graph_nodes[i]->temp_mark = 0;
		graph->graph_nodes[i]->perm_mark = 0;
	}
	int unmarked_node_exists = 1;
	while (unmarked_node_exists) {
		Spec_Representation * next = NULL;
		for (int i = 0; i < graph->dimension; i++) {
			if (graph->graph_nodes[i]->temp_mark == 0 && graph->graph_nodes[i]->perm_mark == 0) {
				next = graph->graph_nodes[i];
				break;
			}
		}
		if (next == NULL) {
			unmarked_node_exists = 0;
		} else {
			Visit(graph, next, end_list);
		}
	}

	return 0;
}

void Visit(Spec_Graph * graph, Spec_Representation * curr_point, Spec_Representation ** end_list) {
	if (curr_point->perm_mark) {
		return;
	} else if (curr_point->temp_mark) {
		fprintf(stderr, "Cycle Detected, Make Failed\n");
                exit(1);
	}
	curr_point->temp_mark = 1;
	for (int i = 0; i < curr_point->num_dependencies; i++) {
		Spec_Representation * dependency = GetSpec(curr_point->dependencies[i], graph->graph_nodes);
		if (dependency == NULL) {
			// No Spec for that Dependency
		} else {
			Visit(graph, dependency, end_list);
		}
	}
	curr_point->perm_mark = 1;
	// Add to list
}

Spec_Representation ** TraverseGraph(Spec_Graph * graph, Spec_Representation * start_point) {
	Spec_Representation ** build_order_list = malloc(sizeof(Spec_Representation *) * graph->dimension);
	RecursiveTraversal(graph, start_point, build_order_list);
	return build_order_list;
}

void RecursiveTraversal(Spec_Graph * graph, Spec_Representation * start_point, Spec_Representation ** build_list) {
	// Base Case: No Dependents
	if (start_point->num_dependencies == 0) {
		// Add Spec_Representation to Build List
		AddToList(build_list, start_point, graph);
	// Recursive Case: Has Dependents
	} else {
		for (int i = 0; i < start_point->num_dependencies; i++) {
			Spec_Representation * dependency = GetSpec(start_point->dependencies[i], graph->graph_nodes);
			if (dependency == NULL) {
				// Nothing to build for that dependency (No Spec)
			} else {
				RecursiveTraversal(graph, dependency, build_list);
			}
		}
		AddToList(build_list, start_point, graph);
	}

	return;
}

void AddToList(Spec_Representation ** build_list, Spec_Representation * addition, Spec_Graph * graph) {
	
	Spec_Representation * curr;
	int index = 0;
	while (1) {
		if (index == graph->dimension) {
			fprintf(stderr, "Cycle Detected, Make Failed\n");
			exit(1);
		}
		curr = build_list[index];
		if (curr == NULL) {
			build_list[index] = addition;
			break;
		}
		index++;
	}
	return;
}
