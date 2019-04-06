#ifndef _VERTEX_H
#define _VERTEX_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdio.h>

class vertex {
 private:
	/* edge weight and index of the destination vertex */
	typedef pair<int, int> edge;
	
	/* Name of the vertex (Optional). Doesn't serve any purpose */
	string name;

	/* adjacency list of edges */
	unordered_map<int, int> adjacency;

	static int unnamed_instances;
	
 public:
	/* Constructor for vector with names */
	vertex( string s) : name(s) {}

	/* Constructor for vertices without names */
	vertex();

	/* Get the name of the vertex */
	string get_name();

	/* Get edge count from the vertex */
	int get_edge_count();

	/* Get the nth edge from the vertex */
	edge get_edge( int n );

	/* Add new edge from the vertex */
	int add_edge( int n, int weight );

	/* Add new edge from the vertex */
	int add_edge( pair<int,int>& x );

	/* Returns constant iterator to the start of the vertex set */
	unordered_map<int,int>::const_iterator cbegin();

	/* Returns constant iterator to the end of the vertex set */
	unordered_map<int,int>::const_iterator cend();
};

#endif
