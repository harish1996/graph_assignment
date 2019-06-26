#include <iostream>
#include <vector>
#include <algorithm>
#include "random_graph.h"
#include "sud_graph.h"
#include <queue>
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
	cout<<arg<<" [ -f <filename> /-r <Size of graph> <Number of edges> <type> ] [ SOURCE ]\n";
	cout<<" filename - Input filename with the graph\n";
	cout<<" Size of the graph - Integer\n";
	cout<<" Number of edges - Integer\n";
}

/**
 * @class ComparePairs
 * @brief To provide a function object for priority queue
 */
class comparePairs{
	public:
	int operator()( pair<int,pair<int,int>>& a, pair<int,pair<int,int>>& b )
	{
		return ( a.second.second > b.second.second );
	}
};

/**
 * @func dijkstra_shortest_path 
 * @brief Finds the shortest path to all nodes in the graph from a
 * 	a given source node
 *
 * @param graph Graph instance
 * @param source Source vertex to run shortest path algo from
 * @param arr Vector where the shortest path is to be filled in
 *
 * @return 0 on success, negative on failure
 */ 
	template <class T> 
int dijkstra_shortest_path( T& graph, int source, vector<int>& arr, vector<int>& from )
{
	//vector<pair<int,int>> heap;
	int size = graph.get_graph_size();

	// Vector to store whether shortest path for the index has been found or not
	vector<bool> optimised(size, false);

	// How many vertices are done
	int done=0;

	// Priority queue of vertices and distances from source
	// pair<int,int> stores ( vertex id, distance )
	// compare_pairs compares 2 nodes
	typedef	priority_queue<pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>,comparePairs> d_heap;
	d_heap heap;
	
	if( source >= size || source < 0 )
		return -1;

	arr.reserve( size );
	arr.resize( size );
	from.reserve( size );
	from.resize( size );
	//arr[source] = 0;
	
	// Push source node with 0 distance
	heap.push( pair<int,pair<int,int>>(-1,pair<int,int>(source,0)) );

	while( done < size ){
		
		if( heap.size() > 0 ){
			// Extract top element from priority queue, shortest path vertex
			pair<int,pair<int,int>> tp = heap.top();
			
			if( optimised[tp.second.first] == true ){
				heap.pop();
				continue;
			}
			// Get the new vertex from the graph
			vertex tv = graph.get_vertex( tp.second.first );

			// Declare shortest path for this vertex
			arr[tp.second.first] = tp.second.second;
			optimised[tp.second.first] = true;
			from[tp.second.first] = tp.first;

			//cout<<tp.first<<endl;
			// Iterate through all the edges and all its edges into the
			// priority_queue.
			auto begin = tv.cbegin(), end = tv.cend();
			for(; begin != end; begin++ ){
				if( optimised[begin->first] == false ){
					//cout<<begin->first<<" "<<tp.second+begin->second<<endl;
					heap.push( pair<int,pair<int,int>>(tp.second.first,pair<int,int>(begin->first,tp.second.second+begin->second) ));
				}
				//cout<<endl;
			}
			
			// Remove top element
			heap.pop();

			done++;	
		} else{
			// Mark other elements as unreachable
			for( int i=0; i<size; i++ ){
				if( optimised[i] != true ){
					arr[i] = -1;
					from[i] = -1;
					done++;
					optimised[i] = true;
				}
			}
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
	//string type;
	char *type;
	vector<int> arr,parent;
	char directed;	
#define FILE_MODE 1
#define RANDOM_MODE 2
	char mode;
	char *filename;
	
	if( argc < 3 ){
		usage(argv[0]);
		exit(-1);
	}

	if( !strcmp( argv[1], "-f" ) ){
		if( argc != 4 ){
			usage(argv[0]);
			exit(-1);
		}
		else{
			mode = FILE_MODE;
			filename = argv[2];
		}
		if( custom_atoi( argv[3], source ) ){
				usage( argv[0] );
				exit(-1);
		}
	}
	else if( !strcmp( argv[1], "-r" ) ){
		if( argc != 6 ){
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
			type = argv[4];	
			if( custom_atoi( argv[5], source ) ){
				usage( argv[0] );
				exit(-1);
			}

			//type = argv[4];
		}
	}
	
	if( mode == RANDOM_MODE ){
		if( strcmp( type, "u" ) == 0 ){
			directed = 0;
			ret = generate_random_connected_graph( udgraph, n, edges );
		}
		else if(strcmp( type, "d" ) == 0 ){
			directed = 1;
			ret = generate_random_connected_graph( dgraph, n, edges );	
		}
		else{
			usage( argv[0] );
			exit(-1);
		}
	}
	else if( mode == FILE_MODE ){
#define SUCCESS 0
#define WRONG_FORMAT -2
		ret = read_undirected_graph( filename, udgraph );
		if( ret == SUCCESS ){
			directed = 0;
			n = udgraph.get_graph_size();
		}
		else if( ret == WRONG_FORMAT ){
			directed = 1;
			ret = read_directed_graph( filename, dgraph );
			n = dgraph.get_graph_size();
		}
		else{
			usage( argv[0] );
			exit(-1);
		}

	}

	//ret = generate_random_DAG( dgraph, n, edges );
	//ret = generate_random_connected_graph( dgraph, n, edges );
       	//ret = generate_random_connected_graph( udgraph, n, edges );	
	if( source >= n || source < 0 ){
		usage(argv[0]);
		exit(-1);
	}
	//ret = topological_sort( dgraph, arr );
	//ret = dgraph.print_graph_graphviz( "problem3.dot" );
	//system( " dot -Tsvg -O problem3.dot " );
	if( directed ){
		ret = dijkstra_shortest_path( dgraph, source, arr, parent );
		ret = dgraph.print_graph_graphviz( "problem5ad.dot" );
		system( " dot -Tsvg -O problem5ad.dot " );

		int size = arr.size();
		for( int i=0; i< size; i++ )
			cout<<source<<"->"<<parent[i]<<"->"<<i<<" "<<arr[i]<<endl;
		cout<<"\n";
	}
	else{
		ret = dijkstra_shortest_path( udgraph, source, arr, parent );
		ret = udgraph.print_graph_graphviz( "problem5aud.dot" );
		system( " dot -Tsvg -O problem5aud.dot " );

		int size = arr.size();
		for( int i=0; i< size; i++ )
			cout<<source<<"->"<<parent[i]<<"->"<<i<<" "<<arr[i]<<endl;
		cout<<"\n";
	}
	return 0;
}

