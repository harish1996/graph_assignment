#include <iostream>
#include <vector>
#include "random_graph.h"
#include "sud_graph.h"

using namespace std;

/**
 * @func custom_atoi
 * @brief Converts string to integer
 *
 * @param arg Command line argument/ String to be converted
 * @param num Placeholder for the number
 *
 * @return -1 on error, 0 on success
 */
int custom_atoi( char *arg, int& num  )
{
	char *endptr;
	num = strtoq( arg, &endptr, 10 );
	if( errno )
		return -1;
	if( *endptr != '\0' )
		return -1;
	return 0;
}

void usage( char* arg )
{
	cout<<arg<<" <Size of graph> <Number of edges> <type> <source>\n";
	cout<<" Size of the graph - Integer\n";
	cout<<" Number of edges - Integer\n";
}

/**
 * @func bridge_dfs 
 * @brief Depth first search for finding a bridge edge
 *
 * @param graph The graph instance
 * @param start Starting point of the dfs
 * @param ids The vector storing all ids allocated to each node
 * @param low The vector storing the low values of each node
 * @param bridge A vector of bridge edges
 *
 * @return 0 on success
 */
int bridge_dfs( sud_graph& graph, int start, vector<int>& ids, vector<int>& low, vector<pair<int,int>>& bridge, int parent )
{
	static int id = 2;
	vertex ver = graph.get_vertex( start );
	auto begin = ver.cbegin();
	auto end = ver.cend();

	ids[start] = id;
	id++;
	low[start] = ids[start];

	for( ; begin!= end; begin++ ){
		int ret;
		// Tree edge
		if( ids[begin->first] == 0 ){
			ret = bridge_dfs( graph, begin->first, ids, low, bridge, start );
			// Updating low values of the current vertex if there is a chance of that
			// becoming better
			if( low[start] >= low[begin->first] )
				low[start] = low[begin->first];
			// Current edge is a bridge if the low value of the next edge is not less
			// than current edge id i.e there is no path from below the tree to above the
			// tree. => removing the current edge disconnects the graph
			if( low[begin->first] > ids[start] )
				bridge.push_back( pair<int,int>(start,begin->first) );
		}
		// Back edge
		else{
			if( low[start] >= ids[begin->first] && begin->first != parent )
				low[start] = ids[begin->first];
		}
	}
	cout<<start<<" "<<ids[start]<<" "<<low[start]<<endl;

	return 0;
}

/**
 * @func find_bridge_edges
 * @brief Finds bridges in a graph
 *
 * @param graph The graph instance
 * @param bridges Array of bridge edges
 *
 * @return 1 if connected, 0 if not connected
 */
int find_bridge_edges( sud_graph& graph, vector<pair<int,int>>& arr )
{
	int size = graph.get_graph_size();
	
	// Arrays to store ids and low values during dfs
	vector<int> ids(size,0),low(size,0);
	
	int ret;
	vertex ver = graph.get_vertex( 0 );
	auto begin = ver.cbegin();
	auto end = ver.cend();

	// Start from 0th vertex as a default.
	ids[0] = 1;
	low[0] = 0;

	
	if( begin == end ){
		// If there are no edges from 0, return graph is disconnected
		return 0;
	}

	for( ; begin != end; begin++ ){
		int ret;
		// After every DFS, check whether there is another path from the this vertex via the
		// tree edge to the same vertex using atleast one back edge. If there are no such path,
		// removing this corresponding edge, will disconnect the graph, as there is no path
		// from the new vertex to the root vertex.
		if( ids[begin->first] == 0 ){
			ret = bridge_dfs( graph, begin->first, ids, low, arr, 0 );
			if( low[begin->first] != 1 )
				arr.push_back( pair<int,int>(0,begin->first) );
		}
	}

	// Return disconnected, if some vertices are not visited.
	for( int i=0; i<size; i++ ){
		if( ids[i] == 0 )
			return 0;
	}

	return 1;
}


int main( int argc, char *argv[] )
{
	int ret;
	sud_graph udgraph;
	int n,edges;
	vector<pair<int,int>> arr;
	int connected;
		
	if( argc != 3 ){
		usage(argv[0]);
		exit(-1);
	}

	if( custom_atoi( argv[1], n ) ){
		usage(argv[0]);
		exit(-1);
	}
	if( custom_atoi( argv[2], edges ) ){
		usage(argv[0]);
		exit(-1);
	}

	ret = generate_random_graph( udgraph, n, edges );	
	//ret = find_strongly_connected_components( dgraph, arr );
	connected = find_bridge_edges( udgraph, arr );
	ret = udgraph.print_graph_graphviz( "problem3b.dot" );
	system( " dot -Tsvg -O problem3b.dot " );
	
	if( connected ){
		int size = arr.size();
		if( size ){
			cout<<"Bridge edges are:\n";
				for( int i=0; i<size; i++ ){
					cout<<arr[i].first<<"->"<<arr[i].second<<endl;
				}
		}
		else{
			cout<<"No bridges\n";
		}
	}
	else{
		cout<<"Graph is already disconnected\n";
	}

	return 0;
}

