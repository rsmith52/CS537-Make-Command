// Ray Smith - rsmith52 - rays - Nate Hoffman - nhoffman5 -nhoffman
#ifndef BUILDSPECGRAPH_H_INCLUDED
#define BUILDSPECGRAPH_H_INCLUDED
/* Include Guards ^^^ */
#include "main.h"
#include "build_spec_repr.h"
/* Prototypes for Variables */

typedef struct Spec_Graphs {
	Spec_Representation ** graph_nodes;
	int dimension;
	int ** adj_matrix;
} Spec_Graph;

/* Prototypes for Functions */

Spec_Graph * BuildSpecGraph(Spec_Representation ** nodes, int num_specs);

Spec_Representation ** TraverseGraph(Spec_Graph * graph, Spec_Representation * start_point);

void RecursiveTraversal(Spec_Graph * graph, Spec_Representation * start_point, Spec_Representation ** build_list);

void AddToList(Spec_Representation ** build_list, Spec_Representation * addition);

#endif
