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
	cout<<" Source node - Integer\n";
}

/**
 * @class d_set
 * @brief Disjoint set
 */
class d_set {
	vector<int> rep;
	vector<int> rank;
public:
	int union( int first, int second );
	int find( int index );
	d_set( int n );
}

/**
 * @func union
 * @brief Merges 2 disjoint sets ( representatives )
 *
 * @param first First representative
 * @param second Second representative
 *
 * @return 0 on success, -1 on error
 */
int d_set::union( int first, int second )
{
	if( first < 0 || first >= rep.size() )
		return -1;
	if( second < 0 || second >= rep.size() )
		return -1;
	if( first == second )
		return -2;
	if( rep[first] != first || rep[second] != second )
		return -3;

	if( rank[first] > rank[second] ){
		rep[second] = first;
	} else if( rank[first] < rank[second] ){
		rep[first] = second;
	} else{
		rep[first] = second;
		rank[first]++;
	}

	return 0;
}

/**
 * @func find
 * @brief Finds the representative element of the particular index
 *
 * @param index Index of the particular element
 *
 * @return index of the representative element
 */
int d_set::find( int index )
{
	vector<int> indices;
	int size;

	// Tracing the absolute ancestor
	while( rep[index] != index ){
		indices.push_back( index );
		index = rep[index];
	}
	size = indices.size();
	
	// Path Compression
	for( int i=0; i<size; i++ ){
		int tmp;
		tmp = indices.back();
		indices.pop_back();
		rep[tmp] = index;
	}

	return index;
}

/**
 * @func d_set
 * @brief Constructor for disjoint set
 *
 * @param n Number of individual elements in the set
 */
d_set::d_set( int n )
{
	rep.reserve( n );
	rank.reserve( n );
	rep.resize( n );
	// Rank of all elements is 0 initially
	rank.resize( n, 0 );

	// All elements are their own representatives initially.
	for( int i=0; i<n; i++ )
		rep[i] = i;
}


int kruskal_mst( sud_graph& graph, vector<int>& arr )
{
	int size = graph.get_graph_size();
	d_set set(size);


int main( int argc, char *argv[] )
{
	int ret;
	sd_graph dgraph;
	sud_graph udgraph;
	int n,edges;
	int source;
	string type;
	vector<int> arr;

	if( argc != 4 ){
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
	
	//ret = dijkstra_shortest_path( udgraph, source, arr );
	ret = kruskal_mst( udgraph, arr );
	ret = udgraph.print_graph_graphviz( "problem5aud.dot" );
	system( " dot -Tsvg -O problem5aud.dot " );
	
	int size = arr.size();
	for( int i=0; i< size; i++ )
		if( arr[i] )
			cout<<i<<"->"<<arr[i]<<endl;
	cout<<"\n";

	return 0;
}

