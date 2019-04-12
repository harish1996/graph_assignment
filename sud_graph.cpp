/* Simple undirected graph implementation */
#include "sud_graph.h"

using namespace std;

/**
 * @func add_edge
 * @brief Adds an edge between `from` and `to` with weight `weight`
 *
 * @param from Starting vertex of the edge
 * @param to Terminating vertex of the edge
 * @param weight Weight of the edge
 */
void sud_graph::add_edge( int from, int to, int weight )
{
	list[from].add_edge( to, weight );
	list[to].add_edge( from,weight );
}	

/**
 * @func delete_edge
 * @brief Deletes an edge between `from` and `to`.
 *
 * @param from Starting vertex of the edge
 * @param to Terminating vertex of the edge
 */
void sud_graph::delete_edge( int from, int to )
{
	list[from].delete_edge( to );
	list[to].delete_edge( from );
}	

/**
 * @func sud_graph
 * @brief Constructor for simple directed graph
 *
 * @param adjacency_matrix An adjacency matrix depicting the directed graph
 * @param size Number of vertices.
 */
sud_graph::sud_graph( vector<vector<int>> adjacency_matrix, int size ) : sd_graph()
{
	/* Skip creating graph, if size doesnt match */
	if( adjacency_matrix.size() == size ){
		
		/* Create n vertices */
		add_vertex( size );

		/* Add all edges */
		for( int i=0; i<size; i++ ){

			/* Skip adding edges, if size doesnt match */
			if( adjacency_matrix[i].size() == size ){

				/* Consider only upper triangle */
				for( int j=i; j<size; j++ ){
					
					if( adjacency_matrix[i][j] && i != j){
						add_edge(i, j, adjacency_matrix[i][j] );
					}
				}
			}
		}
	}
}

/**
 * @func print_graph
 * @brief Prints all the edges of the graph to stdout
 */
void sud_graph::print_graph( )
{
	vertex ret;
	auto size =  list.size();
	for( int i=0; i< size; i++ ){
		auto it = list[i].cbegin();
		auto end = list[i].cend();
		for( ; it != end; it++ ){
			cout<<i<<" --"<<it->second<<"--> "<<it->first<<"\n";
		}
	}
}

/**
 * @func print_graph_graphviz
 * @brief Prints the graph in graphviz format to the file mentioned by
 *        `filename`
 * 
 * @param filename File name of the output graphviz file
 *
 * @return 1 on success, -1 on failure
 */
int sud_graph::print_graph_graphviz( string filename )
{
	FILE *fp = fopen(filename.c_str(),"w");
	if( !fp ){
		return -1;
	}
	fprintf(fp,"digraph{\n");
	auto size =  list.size();
	for( int i=0; i< size; i++ ){
		auto it = list[i].cbegin();
		auto end = list[i].cend();
		fprintf(fp,"%d [ label = %s ]\n",i,list[i].get_name().c_str());
		for( ; it != end; it++ ){
			//cout<<i<<" --"<<it->second<<"--> "<<it->first<<"\n";
			if( it->first > i )
				fprintf(fp,"%d->%d [ label = %d,dir=none ]\n",i,it->first,it->second);
		}
	}
	fprintf(fp,"}\n");
	fclose(fp);
	return 1;
}
	
/*
#define SIZE 6

int main()
{
	vector< vector<int> > matrix( SIZE,vector<int>(SIZE) );
	int a[][SIZE]={ {1,5,0,0,2,0},{0,1,6,4,7,3},{0,0,1,8,3,1},{0,0,0,1,0,2},{0,0,0,0,1,0},{0,0,0,0,0,1}};
	for( int i=0; i<SIZE; i++ ){
		for( int j=0; j<SIZE; j++ ){
			matrix[i][j] = a[i][j];
		}
	}
	
	sud_graph graph( matrix, SIZE );
	graph.print_graph();
	graph.print_graph_graphviz( "trial.dot" );
	system("dot -Tsvg -O trial.dot");
	
}						
*/
