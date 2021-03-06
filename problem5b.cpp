#include <iostream>
#include <vector>
#include "random_graph.h"
#include "sud_graph.h"
#include <algorithm>
#include "input.h"
#include <string.h>

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
 * @class d_set
 * @brief Disjoint set
 */
class d_set {
	vector<int> rep;
	vector<int> rank;
public:
	int set_union( int first, int second );
	int find( int index );
	d_set( int n );
};

/**
 * @func union
 * @brief Merges 2 disjoint sets ( representatives )
 *
 * @param first First representative
 * @param second Second representative
 *
 * @return 1 if second becomes the child, 2 if first becomes the child, -1 on error
 */
int d_set::set_union( int first, int second )
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
		return 1;
	} else if( rank[first] < rank[second] ){
		rep[first] = second;
		return 2;
	} else{
		rep[second] = first;
		rank[first]++;
		return 1;
	}

	//return 0;
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

typedef pair<int,int> sides;
typedef pair<sides,int> edge;

bool edge_comp( edge& first, edge& second )
{
	return ( first.second > second.second );
}

/**
 * @func kruskal_mst
 * @brief Kruskal mst algorithm which finds the edges that are present in the MST
 *
 * @param graph
 * @param arr Array of edges which are present in the MST
 *
 * @return 0 on success
 */
int kruskal_mst( sud_graph& graph, vector<edge>& arr )
{
	int size = graph.get_graph_size();
	d_set set(size);
	vector< edge > elist; 
	arr.clear();
	int taken=0;
	arr.reserve( size-1 );
	arr.resize( size-1, edge(sides(-1,-1),0) );

	for( int i=0; i<size; i++ ){
		vertex v = graph.get_vertex( i );
		auto begin = v.cbegin(), end = v.cend();

		for(; begin != end; begin++ ){
			if( begin->first <= i ) continue;
			sides t1( i, begin->first );
			edge t2( t1, begin->second );
			elist.push_back( t2 );
		}
	}

	sort( elist.begin(), elist.end(), edge_comp );
	taken = 1;
	while( taken <= size - 1 ){
		//cout<<elist.size()<<endl;
		edge e = elist.back();
		sides s = e.first;
		int rep1,rep2;

		elist.pop_back();
		rep1 = set.find( s.first );
		rep2 = set.find( s.second );

		if( rep1 != rep2 ){
			int ret = set.set_union( rep1, rep2 );
		       	//cout<<ret<<endl;
			arr[ taken-1 ] = edge( sides( s.first, s.second ) , e.second );
			taken++;
		}
	}	
	
	return 0;
}	


int main( int argc, char *argv[] )
{
	int ret;
	sd_graph dgraph;
	sud_graph udgraph;
	int n,edges;
	int source;
	string type;
	vector<edge> arr;
	FILE *fp = fopen("mst.dot", "w" );
	if( !fp ){
		exit(-1);
	}
					
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
		ret = generate_random_connected_graph( udgraph, n, edges );	
	else if( mode == FILE_MODE ){
		ret = read_undirected_graph( filename, udgraph );
		if( ret != 0 ){
			fprintf(stderr,"Ensure the graph is undirected\n");
			usage(argv[0]);
			exit(-1);
		}
	}

	//ret = generate_random_connected_graph( udgraph, n, edges );	
	//ret = dijkstra_shortest_path( udgraph, source, arr );
	ret = kruskal_mst( udgraph, arr );
	ret = udgraph.print_graph_graphviz( "problem5b.dot" );
	system( " dot -Tsvg -O problem5b.dot " );
	
	
	int size = arr.size();
	fprintf(fp,"digraph{\n");
	for( int i=0; i< size; i++ ){
		if( arr[i].first.first != -1 )
			//cout<<i<<"->"<<arr[i].first<<" [ label= "<<arr[i].second<<" ]\n";
			fprintf(fp,"%d->%d [ label=\"%d\",dir=none ]\n",arr[i].first.first,arr[i].first.second,arr[i].second);
	}
	fprintf(fp,"}\n");
	fclose(fp);
	system( " dot -Tsvg -O mst.dot" );


	//cout<<"\n";
	
	return 0;
}

