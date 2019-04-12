#include <iostream>
#include "random_graph.h"
#include "sud_graph.h"
#include <stack>
#include <vector>

using namespace std;

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
	cout<<arg<<" <Size of graph> <Number of edges> <type>\n";
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
	
	ret = generate_random_DAG( dgraph, n, edges );
	ret = topological_sort( dgraph, arr );
	ret = dgraph.print_graph_graphviz( "problem3.dot" );
	system( " dot -Tsvg -O problem3.dot " );
	int size = arr.size();
	for( int i=0; i< size; i++ )
		cout<<arr[i]<<" ";
	cout<<"\n";

	return 0;
}
