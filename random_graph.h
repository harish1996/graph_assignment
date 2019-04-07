#ifndef _RANDOM_GRAPH_H
#define _RANDOM_GRAPH_H

#include "sud_graph.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

/**
 * @func generate_random_connected_graph
 * @brief Generate random connected graph with given number of vertices and the
 *       given sparsity ( no.of edges )
 *
 * @param graph Reference to an sud_graph object
 * @param vertices Number of vertices
 * @param sparseness No. of edges required
 * 
 * @return -1 if vertices < n-1 ( connected graph not possible ), 
 *         -2 if vertices > nC2 ( simple graph not possible ),
 *          0 if successful
 */
int generate_random_connected_graph( sud_graph& graph, int vertices, int sparseness );

#endif
