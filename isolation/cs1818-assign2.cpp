#include <iostream>
#include "sud_graph.h"

#define DIRECTED 1
#define UNDIRECTED 2

#define WEIGHTED 1
#define UNWEIGHTED 2

#define DFS 1
#define SCC 2
#define AP  3
#define BRG 4
#define TPS 5
#define DSP 6
#define MST 7

#define ADJ_MAT 1
#define ADJ_LST 2
#define EDG_LST 3

int main()
{
	int problem;
	int directed=0;
	int format;
	int vertices;
	int weighted;

	sud_graph ud_graph;
	sd_graph d_graph;

	string problems[7] = { "DFS", "Strongly Connected Components", "Articulation points", "Bridges", "Topological Sort", "Dijkstra's shortest path algorithm", "Kruskal's minimum spanning tree algorithm" };

	for( int i=0; i<7; i++ )
		cout<<" Enter "<<i<<" for "<<problems[i]<<endl;

	cout<<" Enter option:\n";
	cin>>problem;

	switch( problem ){
	case SCC:
	case TPS:
		directed = DIRECTED;
		weighted = UNWEIGHTED;
		cout<<" Directed and unweighted graph implied\n";
		break;
	case AP:
	case BRG:
		directed = UNDIRECTED;
		weighted = UNWEIGHTED;
		cout<<" Undirected and unweighted graph implied\n";
		break;
	case MST:
		directed = UNDIRECTED;
		weighted = WEIGHTED;
		cout<<" Undirected and weighted graph implied\n";
		break;
	case DFS:
	case DSP:
		cout<<" Enter 1 for directed\n Enter 2 for undirected\n";
		cout<<" Enter option:\n";
		cin>>directed;
		
		if( directed != DIRECTED && directed != UNDIRECTED) {
			cout<<" Invalid option.. Exiting\n";
			exit(-1);
		}

		if( problem == DFS ){
			weighted = UNWEIGHTED;
			cout<<" Unweighted graph implied"<<endl;
		}
		else{
			weighted = WEIGHTED;
			cout<<"Weighted graph implied"<<endl;
		}
	default:
		cout<<" Invalid option.. Exiting\n";
		exit(-1);
	}

	cout<<" Enter the graph input format\n1.Adjacency matrix\n2.Adjacency list\n3.Edge list\n";
	cin>>format;
	if( format < ADJ_MAT || format > EDG_LST ){
		cout<<" Invalid option.. Exiting\n"
		exit(-1);
	}

	if( directed == DIRECTED )
		ret = get_directed_input_graph( weighted, format, d_graph );
	else
		ret = get_undirected_input_graph( weighted, format, ud_graph );


	// Validate the return value first

	switch(problem){
		case DFS:
			if( directed == DIRECTED )
				ret = dfs( d_graph ); // Remove the second argument in the implementation.
			else
				ret = dfs( ud_graph );
			break;
		case SCC:
			// Implement the function which prints the output of `find_strongly_connected_components`
			ret = print_find_strongly_connected_components( d_graph );
			break;
		case AP:
			// Implement the function which prints the necessary output
			ret = print_find_articulation_points( ud_graph );
			break;
		case BRG:
			ret = print_find_bridge_edges( ud_graph );
			break;
		case TPS:
			ret = print_topological_sort( d_graph );
			break;
		case DSP:
			if( directed == DIRECTED )
				ret = read_print_dijkstra_shortest_path( d_graph );
			else
				ret = read_print_dijkstra_shortest_path( ud_graph );
			break;
		case MST:
			ret = print_kruskal_mst( ud_graph );
			break;

	}
}

	
