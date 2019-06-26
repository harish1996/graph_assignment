#include <iostream>
#include <vector>
#include "random_graph.h"
#include "sud_graph.h"
#include "string.h"
#include "input.h"

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
	cout<<arg<<" -f <filename> /-r<Size of graph> <Number of edges>\n";
	cout<<" filename - Input filename with the graph\n";
	cout<<" Size of the graph - Integer\n";
	cout<<" Number of edges - Integer\n";
}

/**
 * @func art_dfs 
 * @brief Depth first search for finding articulation point
 *
 * @param graph The graph instance
 * @param start Starting point of the dfs
 * @param visited The vector storing the visited status of each node
 * @param ids The vector storing all ids allocated to each node
 * @param low The vector storing the low values of each node
 * @param art A boolean vector storing whether some vertex is a articulation
 * 		point or not
 *
 * @return 0 on success
 */
int art_dfs( sud_graph& graph, int start, vector<bool>& visited, vector<int>& ids, vector<int>& low, vector<bool>& art )
{
	static int id = 1;
	vertex ver = graph.get_vertex( start );
	auto begin = ver.cbegin();
	auto end = ver.cend();

	visited[start] = true;
	ids[start] = id;
	id++;
	low[start] = ids[start];
	art[start] = false;
	for( ; begin!= end; begin++ ){
		int ret;
		// Tree edge
		if( visited[begin->first] == false ){
			ret = art_dfs( graph, begin->first, visited, ids, low, art );
			// Updating low values of the current vertex if there is a chance of that
			// becoming better
			if( low[start] >= low[begin->first] )
				low[start] = low[begin->first];
			// Current vertex is a articulation point if any children has a low value
			// less than current vertex id. i.e. if current vertex is removed, the children
			// cannot be connected to anywhere above in the DFS tree ( low value is greater )
			// => graph is disconnected
			if( low[begin->first] >= ids[start] )
				art[start] = true;
		}
		// Back edge
		else{
			if( low[start] >= ids[begin->first] )
				low[start] = ids[begin->first];
		}
	}
	//cout<<start<<" "<<ids[start]<<" "<<low[start]<<endl;

	return 0;
}

/**
 * @func find_articulation_points
 * @brief Finds articulation points in a graph
 *
 * @param graph The graph instance
 * @param arr The array which where the articulation point indices will be stored
 *
 * @return 1 if connected, 0 if not connected
 */
int find_articulation_points( sud_graph& graph, vector<int>& arr )
{
	int size = graph.get_graph_size();
	
	// Arrays to store ids and low values during dfs
	vector<int> ids(size,0),low(size,0);
	// Arrays to store visited values and articulation points
	vector<bool> visited(size,false), art(size,false);
	
	int ret;
	vertex ver = graph.get_vertex( 0 );
	auto begin = ver.cbegin();
	auto end = ver.cend();

	// Start from 0th vertex as a default.
	ids[0] = 0;
	low[0] = 0;
	visited[0] = true;
	art[0] = false;

	// Manually perfom one dfs. If 0 is not an articulation point, this one
	// dfs should be enough to cover all vertices
	if( begin != end ){
		ret = art_dfs( graph, begin->first, visited, ids, low, art );
		begin++;
	}
	else{
		// If there are no edges from 0, return graph is disconnected
		return 0;
	}

	for( ; begin != end; begin++ ){
		int ret;
		// If some vertices are not covered in the first dfs, then 0th vertex 
		// is an articulation point as there is no connection between the first tree
		// and the other tree. So removing 0 will make the graph disconnected
		if( visited[begin->first] == false ){
			art[0] = true;
			ret = art_dfs( graph, begin->first, visited, ids, low, art );
		}
	}

	for( int i=0; i<size; i++ ){
		if( visited[i] == false )
			return 0;
		if( art[i] == true )
			arr.push_back( i );
	}

	return 1;
}


int main( int argc, char *argv[] )
{
	int ret;
	sud_graph udgraph;
	int n,edges;
	//string type;
	vector<int> arr;
	int connected;
			
#define FILE_MODE 1
#define RANDOM_MODE 2
	char mode;
	char *filename;
	
	if( argc < 2 ){
		usage(argv[0]);
		exit(-1);
	}

	if( !strcmp( argv[1], "-f" ) ){
		if( argc != 3 ){
			usage(argv[0]);
			exit(-1);
		}
		else{
			mode = FILE_MODE;
			filename = argv[2];
		}
	}
	else if( !strcmp( argv[1], "-r" ) ){
		if( argc != 4 ){
			usage(argv[0]);
			exit(-1);
		}
		else{
			mode = RANDOM_MODE;
			if( custom_atoi( argv[2], n ) ){
				usage( argv[0] );
				exit(-1);
			}
			if( custom_atoi( argv[3], edges ) ){
				usage( argv[0] );
				exit(-1);
			}
			//type = argv[4];
		}
	}
	
	if( mode == RANDOM_MODE )	
		ret = generate_random_graph( udgraph, n, edges );	
	else if( mode == FILE_MODE )
		ret = read_undirected_graph( filename, udgraph );


	//ret = generate_random_graph( udgraph, n, edges );	
	//ret = find_strongly_connected_components( dgraph, arr );
	connected = find_articulation_points( udgraph, arr );
	ret = udgraph.print_graph_graphviz( "problem3a.dot" );
	system( " dot -Tsvg -O problem3a.dot " );
	
	if( connected ){
		int size = arr.size();
		if( size ){
			cout<<"Articulation points are:\n";
				for( int i=0; i<size; i++ ){
					cout<<arr[i]<<endl;
				}
		}
		else{
			cout<<"No articulation points\n";
		}
	}
	else{
		cout<<"Graph is already disconnected\n";
	}

	return 0;
}

