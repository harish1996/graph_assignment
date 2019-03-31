/* Simple undirected graph implementation */
#include "sud_graph.h"

using namespace std;

class vertex {
 private:
	/* edge weight and index of the destination vertex */
	typedef pair<int, int> edge;
	
	/* Name of the vertex (Optional). Doesn't serve any purpose */
	string name;

	/* adjacency list of edges */
	unordered_map<int, int> adjacency;

	static int unnamed_instances;
	
 public:
	/* Constructor for vector with names */
	vertex( string s) : name(s) {}

	/* Constructor for vertices without names */
	vertex();

	/* Get the name of the vertex */
	string get_name();

	/* Get edge count from the vertex */
	int get_edge_count();

	/* Get the nth edge from the vertex */
	edge get_edge( int n );

	/* Add new edge from the vertex */
	int add_edge( int n, int weight );

	/* Add new edge from the vertex */
	int add_edge( pair<int,int>& x );

	/* Returns constant iterator to the start of the vertex set */
	unordered_map<int,int>::const_iterator cbegin();

	/* Returns constant iterator to the end of the vertex set */
	unordered_map<int,int>::const_iterator cend();
};

int vertex::unnamed_instances = 0;

vertex::vertex()
{
	name = "unnamed_"+ to_string(vertex::unnamed_instances);
	vertex::unnamed_instances++;
}

string vertex::get_name()
{
	return name;
}

int vertex::get_edge_count()
{
	return adjacency.size();	
}

pair<int,int> vertex::get_edge( int n )
{
	unordered_map<int,int>::iterator res = adjacency.find(n);
	pair<int,int> ret(-1,-1);
	if( res != adjacency.end() ){
		ret.first = res->first;
		ret.second = res->second;
	}
	return ret;
}

int vertex::add_edge( int n, int weight )
{
	auto ret = adjacency.insert( (pair<int,int>) make_pair(n,weight) );
	if( ret.second )
		return get_edge_count();
	else
		return -1;
}

int vertex::add_edge( pair<int,int>& x )
{
	auto ret = adjacency.insert( x );
	if( ret.second )
		return get_edge_count();
	else
		return -1;
}

unordered_map<int,int>::const_iterator vertex::cbegin()
{
	return adjacency.cbegin();
}

unordered_map<int,int>::const_iterator vertex::cend()
{
	return adjacency.cend();
}


class sud_graph {
	/* Adjacency list */
	vector<vertex> list;

 public:
	/* Add new named vertex */
	int add_vertex( string name );
	
	/* Add new unnamed vertex */
	int add_vertex( );

	/* Add n new unnamed vertices */
	int add_vertex( int n );
	
	/* Add edge */
	void add_edge( int from, int to, int weight );

	/* Get nth vertex in the graph */
	vertex get_vertex( int n );

	/* Gets Graph size ( Number of vertices ) */
	int get_graph_size( );
	
	/* Prints graph */
	void print_graph( );

	/* Prints graph in graphviz format in a given file */
	int print_graph_graphviz( string filename );
	
	/* Empty constructor */
	sud_graph() {}

	/* Initializing graph using adjacency matrix representation */
	sud_graph( vector<vector<int>> adjacency_matrix, int size );
};

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
						
