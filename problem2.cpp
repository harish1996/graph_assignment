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

int _find_highest_postvisit( sd_graph& graph, int start, vector<bool>& visited )
{
	int size;
	vertex v = graph.get_vertex( start );
	size = visited.size();
	visited[start] = true;

	auto begin = v.cbegin(),end = v.cend();
	for( ; begin != end; begin++ ){
		if( visited[begin->first] == false )
			_find_highest_postvisit( graph, begin->first, visited );
	}

	return 0;
}

int find_highest_postvisit( sd_graph& graph, vector<bool> visited )
{
	int largest;
	int size;

	size = visited.size();
	for( int i=0; i<size; i++ ){
		if( visited[i] == false ){
			largest = i;
			int ret = _find_highest_postvisit( graph, i, visited );
		}
	}

	return largest;
}

			
int reverse_graph( sd_graph& graph, sd_graph& reversed )
{
	int size = graph.get_graph_size();
	reversed.add_vertex( size );

	for( int i=0; i<size; i++ ){
		vertex v = graph.get_vertex( i );
		auto begin = v.cbegin(), end = v.cend();

		for( ; begin != end; begin++ ){
			reversed.add_edge( begin->first, i, begin->second );
		}
	}

	return 0;
}

int scc_dfs( sd_graph& graph, int start, vector<bool>& visited, vector<int>& arr )
{
	vertex v = graph.get_vertex( start );

	auto begin = v.cbegin(), end = v.cend();
	visited[start] = true;
	arr.push_back( start );
	for( ; begin != end; begin++ ){
		if( visited[begin->first] == false )
			scc_dfs( graph, begin->first, visited, arr );
	}
}

int find_strongly_connected_components( sd_graph& graph, vector<vector<int>>& arr )
{
	sd_graph reversed;
	int size = graph.get_graph_size();
	int detected = 0;
	vector<bool> visited( size, false );
	int ret;
	int i=0;
	vector<int> tmp;

	arr.clear();

	ret = reverse_graph( graph, reversed );

	while( detected != size ){
		arr.push_back( tmp );
		ret = find_highest_postvisit( reversed, visited );
		ret = scc_dfs( graph, ret, visited, arr[i] );
		detected += arr[i].size();
		i++;
	}

	return 0;
}

int main( int argc, char *argv[] )
{
	int ret;
	sd_graph dgraph;
	int n,edges;
	//string type;
	vector<vector<int>> arr;
		
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

	ret = generate_random_graph( dgraph, n, edges );	
	ret = find_strongly_connected_components( dgraph, arr );
	
	ret = dgraph.print_graph_graphviz( "problem2.dot" );
	system( " dot -Tsvg -O problem2.dot " );
	
	int size = arr.size();
	for( int i=0; i<size; i++ ){
		int scc_size = arr[i].size();
		cout<<"Strongly Connected Component "<<i<<" contains:\n";
		for( int j=0; j<scc_size; j++ ){
			cout<<arr[i][j]<<" ";
		}
		cout<<endl;
	}
			
	/*
	fprintf(fp,"digraph{\n");
	for( int i=0; i< size; i++ ){
		if( arr[i].first.first != -1 )
			//cout<<i<<"->"<<arr[i].first<<" [ label= "<<arr[i].second<<" ]\n";
			fprintf(fp,"%d->%d [ label=\"%d\",dir=none ]\n",arr[i].first.first,arr[i].first.second,arr[i].second);
	}
	fprintf(fp,"}\n");
	fclose(fp);
	system( " dot -Tsvg -O mst.dot" );
	*/

	//cout<<"\n";
	
	return 0;
}

