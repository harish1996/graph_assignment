/* Simple undirected graph implementation */
#include "sud_graph.h"

using namespace std;

int sud_graph::add_vertex( string name )
{
	vertex temp(name);
	list.push_back( temp );
	return list.size() - 1;
}


int sud_graph::add_vertex( )
{
	vertex temp;
	list.push_back( temp );
	return list.size() - 1;
}

int sud_graph::add_vertex( int n )
{
	for( int i=0; i<n; i++ )
		add_vertex();
	return list.size() - 1;
}

void sud_graph::add_edge( int from, int to, int weight )
{
	list[from].add_edge( to, weight );
	list[to].add_edge( from,weight );
}	

vertex sud_graph::get_vertex( int n )
{
	vertex ret( "invalid" );
	if( n <= list.size() ){
		return list[n];
	}
	else{
		return ret;
	}
}

sud_graph::sud_graph( vector<vector<int>> adjacency_matrix, int size )
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

int sud_graph::get_graph_size()
{
	return list.size();
}

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
 * @func check_edge 
 * @brief Checks whether there is an edge between from and to and returns
 *        the weight of the edge if it exists
 * @param from One side of the edge
 * @param to Other side of the edge
 * 
 * @return edge weight if edge exists, 0 in case of no edge
 */
int sud_graph::check_edge( int from, int to )
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

int sud_graph::print_graph_graphviz( string filename )
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
						
