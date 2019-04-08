#ifndef SUD_GRAPH_H
#define SUD_GRAPH_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include "vertex.h"
#include "sd_graph.h"

class sud_graph : public sd_graph {
 public:
	//
	// @func add_edge
	// @brief Adds an undirected edge
	//
	// @param from One side of the edge
	// @param to Other side of the edge
	// @param weight Weight of the edge
	//
	void add_edge( int from, int to, int weight );

	//
	// @func print_graph
	// @brief Prints the graph in the stdio
	//
	void print_graph();

	//
	// @func print_graph_graphviz
	// @brief Prints the graph to a file pointed by `filename` in graphviz
	//        format
	//
	// @param filename Filename of the graph
	//
	// @return 0 on success, -1 on error
	//
	int print_graph_graphviz( string filename );


	//
	// @func sud_graph
	// @brief Empty constructor
	//
	sud_graph():sd_graph() {}

	//
	// @func sud_graph
	// @brief Initializing sud graph with an adjacency matrix
	//
	// @param adjacency_matrix The vector of vector of ints corresponding to the
	//        adjacency_matrix.
	// @param size Dimension of the adjacency_matrix
	//
	sud_graph( vector<vector<int>> adjacency_matrix, int size );
	
};

/*
class sud_graph {
	// Adjacency list 
	vector<vertex> list;

 public:
	// Add new named vertex 
	int add_vertex( string name );
	
	// Add new unnamed vertex 
	int add_vertex( );

	// Add n new unnamed vertices
	int add_vertex( int n );
	
	// Add edge 
	void add_edge( int from, int to, int weight );

	// Get nth vertex in the graph 
	vertex get_vertex( int n );

	// Gets Graph size ( Number of vertices )
	int get_graph_size( );
	
	// Prints graph 
	void print_graph( );

	// Checks if there is an edge between from and to.
	int check_edge( int from, int to );

	// Prints graph in graphviz format in a given file 
	int print_graph_graphviz( string filename );
	
	// Empty constructor
	sud_graph() {}

	// Initializing graph using adjacency matrix representation
	sud_graph( vector<vector<int>> adjacency_matrix, int size );
};

*/


#endif
