/* Simple directed graph implementation */
#include "sd_graph.h"

using namespace std;

/**
 * @func add_vertex
 * @brief Adds a named vertex
 * 
 * @param name Name of the vertex
 * 
 * @return Index of the added vertex
 */
int sd_graph::add_vertex( string name )
{
	vertex temp(name);
	list.push_back( temp );
	return list.size() - 1;
}

/**
 * @func add_vertex
 * @brief Add an unnamed vertex ( procedurally named )
 *
 * @return Index of the added vertex
 */
int sd_graph::add_vertex( )
{
	vertex temp;
	list.push_back( temp );
	return list.size() - 1;
}

/**
 * @func add_vertex
 * @brief Adds `n` unnamed vertices in the graph
 * 
 * @param n Number of vertices to be added
 * 
 * @return Index of the last added vertex
 */
int sd_graph::add_vertex( int n )
{
	for( int i=0; i<n; i++ )
		add_vertex();
	return list.size() - 1;
}


/**
 * @func add_edge
 * @brief Adds an edge between `from` and `to` with weight `weight`
 *
 * @param from Starting vertex of the edge
 * @param to Terminating vertex of the edge
 * @param weight Weight of the edge
 */
void sd_graph::add_edge( int from, int to, int weight )
{
	list[from].add_edge( to, weight );
	//list[to].add_edge( from,weight );
}	

/**
 * @func get_vertex
 * @brief Returns nth vertex
 * 
 * @param n Index of the vertex
 *
 * @return vertex instance corresponding to the nth index, if invalid, a vertex object
 *         with name "invalid" is returned
 */
vertex sd_graph::get_vertex( int n )
{
	vertex ret( "invalid" );
	if( n <= list.size() ){
		return list[n];
	}
	else{
		return ret;
	}
}

/**
 * @func sd_graph
 * @brief Constructor for simple directed graph
 *
 * @param adjacency_matrix An adjacency matrix depicting the directed graph
 * @param size Number of vertices.
 */
sd_graph::sd_graph( vector<vector<int>> adjacency_matrix, int size )
{
	/* Skip creating graph, if size doesnt match */
	if( adjacency_matrix.size() == size ){
		
		/* Create n vertices */
		add_vertex( size );

		/* Add all edges */
		for( int i=0; i<size; i++ ){

			/* Skip adding edges, if size doesnt match */
			if( adjacency_matrix[i].size() == size ){

				/* Consider the entire matrix */
				for( int j=0; j<size; j++ ){
					
					if( adjacency_matrix[i][j] && i != j){
						add_edge(i, j, adjacency_matrix[i][j] );
					}
				}
			}
		}
	}
}

/**
 * @func get_graph_size
 * @brief returns the graph size
 *
 * @return Number of vertices of the graph
 */
int sd_graph::get_graph_size()
{
	return list.size();
}

/**
 * @func print_graph
 * @brief Prints all the edges of the graph to stdout
 */
void sd_graph::print_graph( )
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
 * @func check_edge 
 * @brief Checks whether there is an edge between from and to and returns
 *        the weight of the edge if it exists
 * @param from One side of the edge
 * @param to Other side of the edge
 * 
 * @return edge weight if edge exists, 0 in case of no edge
 */
int sd_graph::check_edge( int from, int to )
{
	if( from < 0 || to < 0 )
		return 0;
	if( from >= list.size() || to >= list.size() )
		return 0;

	pair<int,int> ret;
	ret = list[from].get_edge( to );
	if( ret.first != -1 )
		return ret.second;
	else
		return 0;
	
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
int sd_graph::print_graph_graphviz( string filename )
{
	FILE *fp = fopen(filename.c_str(),"w");
	if( !fp ){
		return -1;
	}
	fprintf(fp,"digraph{\n");
	vertex ret;
	auto size =  list.size();
	for( int i=0; i< size; i++ ){
		auto it = list[i].cbegin();
		auto end = list[i].cend();
		fprintf(fp,"%d [ label = %s ]\n",i,list[i].get_name().c_str());
		for( ; it != end; it++ ){
			//cout<<i<<" --"<<it->second<<"--> "<<it->first<<"\n";
			//if( it->first > i )
			fprintf(fp,"%d->%d [ label = %d ]\n",i,it->first,it->second);
		}
	}
	fprintf(fp,"}\n");
	fclose(fp);
	return 1;
}
