#ifndef SD_GRAPH_H
#define SD_GRAPH_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include "vertex.h"

class sd_graph {
	/* Adjacency list */
	vector<vertex> list;

 public:
	/* Add new named vertex */
	int add_vertex( string name );
	
	/* Add new unnamed vertex */
	int add_vertex( );

	/* Add n new unnamed vertices */
	int add_vertex( int n );
	
	/* Add edge */
	void add_edge( int from, int to, int weight );

	/* Get nth vertex in the graph */
	vertex get_vertex( int n );

	/* Gets Graph size ( Number of vertices ) */
	int get_graph_size( );
	
	/* Prints graph */
	void print_graph( );

	/* Checks if there is an edge between from and to. */
	int check_edge( int from, int to );

	/* Prints graph in graphviz format in a given file */
	int print_graph_graphviz( string filename );
	
	/* Empty constructor */
	sd_graph() {}

	/* Initializing graph using adjacency matrix representation */
	sd_graph( vector<vector<int>> adjacency_matrix, int size );
};

#endif
