#include "sud_graph.h"
#include "random_graph.h"
#include <stdlib.h>
#include <errno.h>
#include <vector>
#include <stdio.h>
#include <string>

using namespace std;

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
	cout<<arg<<" <Size of graph> <Number of edges>\n";
}

template<class T>
int _dfs( T& graph, vector<bool>& list, int cur, FILE *fp )
{
	vertex ver = graph.get_vertex( cur );
	//vertex ver;
	auto start = ver.cbegin(), end = ver.cend();

	list[cur] = true;
	fprintf(fp,"%d [ label = %s ]\n",cur,ver.get_name().c_str());
	for( ; start != end; start ++ ){
		
		// If the vertex is not already visited
		if( list[start->first] == false ){
			cout<<cur<<" --"<<start->second<<"--> "<<start->first<<"\n";
			fprintf(fp,"%d->%d [ label = %d,color=\"green\" ]\n",cur,start->first,start->second);
			_dfs( graph, list, start->first, fp );
		}
		else{
			fprintf(fp,"%d->%d [ label = %d,color=\"red\" ]\n",cur,start->first,start->second);
		}
			
	}
	return 0;
}

template<class T>
int dfs( T& graph, string filename )
{
	int size = graph.get_graph_size();
	vector<bool> list;
	int ret;
	FILE *fp= fopen( filename.c_str(), "w" );
	
	list.reserve( size );
	list.resize( size );

	for( int i=0; i<size; i++ )
		list[i] = false;
	fprintf(fp,"digraph{\n");
	ret = _dfs( graph, list, 0, fp );
	fprintf(fp,"\n}");
	fclose(fp);
	return ret;
}
	
int main( int argc, char *argv[])
{
	int ret;
	sud_graph graph;
	int n,edges;
	char *endptr;
	
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
	
	ret = generate_random_connected_graph( graph, n, edges );
	ret = dfs( graph,"problem1.dot" );
	//graph.print_graph_graphviz( "problem1.dot" );
	system(" dot -Tsvg -O problem1.dot" );
}
	
