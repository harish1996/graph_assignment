#include <iostream>
#include "random_graph.h"
#include "sud_graph.h"
#include <stack>
#include <vector>
#include "input.h"
#include <string.h>

using namespace std;
/**
 * @func topological_sort_dfs
 * @brief The dfs helper for topological sort
 *
 * @param graph
 * @param start Start vertex of the dfs
 * @param s Stack containing finished vertices
 * @param visited Visited array
 *
 * @return 0 on success
 */
int topological_sort_dfs( sd_graph& graph, int start, stack<int>& s, vector<bool>& visited )
{
	vertex ver = graph.get_vertex( start );
	auto begin = ver.cbegin();
	auto end = ver.cend();

	visited[start] = true;
	for( ; begin!= end; begin++ ){
		int ret;
		if( visited[begin->first] == false )
			ret = topological_sort_dfs( graph, begin->first, s, visited );
	}

	s.push( start );
	return 0;
}

/**
 * @func topological_sort
 * @brief Topological sort function which fills the array of topological sort vertices
 *
 * @param graph
 * @param arr Array where vertices are added in topological sort order
 *
 * @return 0 on success
 */
int topological_sort( sd_graph& graph, vector<int>& arr )
{
	int size = graph.get_graph_size();
	stack<int> list;
	vector<bool> visited( size, false);
	arr.erase( arr.begin(), arr.end() );
	arr.reserve( size );
	
	for( int i=0; i<size; i++ ){
		int ret;
		if( visited[i] == false )
			ret = topological_sort_dfs( graph, i, list, visited );
	}
	int stack_size = list.size();
	for( int j=0; j<stack_size; j++ ){
		arr.push_back( list.top() );
		list.pop();
	}

	return 0;
}

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

int main( int argc, char *argv[] )
{
	int ret;
	sd_graph dgraph;
	int n,edges;
	string type;
	vector<int> arr;

				
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
		ret = generate_random_DAG( dgraph, n, edges );	
	else if( mode == FILE_MODE )
		ret = read_directed_graph( filename, dgraph );

	//ret = generate_random_DAG( dgraph, n, edges );
	ret = topological_sort( dgraph, arr );
	ret = dgraph.print_graph_graphviz( "problem3.dot" );
	system( " dot -Tsvg -O problem3.dot " );
	int size = arr.size();
	for( int i=0; i< size; i++ )
		cout<<arr[i]<<" ";
	cout<<"\n";

	return 0;
}
