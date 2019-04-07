#include "random_graph.h"

using namespace std;

void swap( vector<int>& arr, int from, int to )
{
	int temp;
	temp = arr[to];
	arr[to] = arr[from];
	arr[from] = temp;
}


int generate_random_connected_graph( sud_graph& graph, int vertices, int sparseness )
{
	if( sparseness < vertices - 1 ){
		return -1;
	}
	if( sparseness > vertices * ( vertices - 1 ) / 2 ){
		return -2;
	}

	// Array for generating random edges
	vector<int> arr;
	vector<int> edge_arr;
	
	// Non connected vertices count
	int nc;
	// Non present edge count
	int enp;
	
	// As this function is meant to create a random connected on top of an already
	// exisiting graph, if there are vertices already, this function should completely
	// ignore those. So marking the first vertex created by the function.
	int start;

	// Allocating n number of elements in the array
	arr.reserve(vertices);
	arr.resize(vertices);

	// Initialising the array
	for( int i=0; i<vertices; i++ ){
		arr[i] = i;
	}

	// Allocating space for edge array
	edge_arr.reserve(vertices*vertices);
	edge_arr.resize(vertices*vertices);

	// Initializing the edge array
	int sz = edge_arr.size();
	for( int i=0; i<sz; i++ ){
		edge_arr[i] = i;
	}

	srand( time(NULL) );

	// The array of integers will be seperated into two regions
	// namely non connected and connected. The first 'nc' many elements
	// in the array are non connected and the remaining are connected.
	nc = vertices-1;

	// The array of integers will be seperated into two regions
	// namely edge not present and edge present. The first 'enp' many elements
	// in the array are non existent edges in the graph and the remaining are
	// present.
	enp = sz;

	// Randomly pick a vertex and move it the connected portion
	swap( arr, rand()%(nc+1), nc );

	// Add n (more) vertices to the graph
	start = graph.add_vertex( vertices ) - vertices + 1;

	// Until all vertices are connected
        while( nc != 0 ){
		int from,to;

		// Randomly picking a connected vertex
		from = ( rand()%( vertices - nc ) ) + nc;

		// Randomly picking a non connected vertex
		to = ( rand()%nc );

		// Put an edge between them with a random integer weight
		graph.add_edge( start+arr[from], start+arr[to], rand()%100 );

		// Moving the selected non connected vertex to connected set
		swap( arr, to, nc-1 );

		nc--;

		sparseness--;
	}

	while( sparseness != 0 ){
		int num,from,to;

		// Randomly choose a non existant edge. This is like picking a matrix element
		// in random, but from a linearized matrix.
		num = rand()%enp;

		// Find the column and row of the corresponding number
		from = edge_arr[ num ] / vertices;
		to = edge_arr[ num ] % vertices;

		// If edge is already present, move it to existing edges and continue to next
		// iteration
		if( from == to || graph.check_edge( start+from, start+to ) != 0 ){
			
			// Moving the corresponding edges to present array
			swap( edge_arr, num, --enp );
			continue;
		}
		else{
			// Add new edge between the from and to
			graph.add_edge( start+from, start+to, rand()%100 + 1 );
			swap( edge_arr, num, --enp);
			sparseness--;
		}
	}

	return 0;
}

/*
#define SIZE 15
#define EDGES 105
#define SIZE2 10
#define EDGES2 45

int main()
{
	//vector< vector<int> > matrix( SIZE,vector<int>(SIZE) );
	//int a[][SIZE]={ {1,5,0,0,2,0},{0,1,6,4,7,3},{0,0,1,8,3,1},{0,0,0,1,0,2},{0,0,0,0,1,0},{0,0,0,0,0,1}};

	//for( int i=0; i<SIZE; i++ ){
	//	for( int j=0; j<SIZE; j++ ){
	//		matrix[i][j] = a[i][j];
	//	}
	//}
	int ret;
	sud_graph graph;
	ret = generate_random_connected_graph( graph, SIZE, EDGES ); 
	graph.print_graph();
	graph.print_graph_graphviz( "random.dot" );
	system("dot -Tsvg -O random.dot");
	ret = generate_random_connected_graph( graph, SIZE2, EDGES2 );
	graph.print_graph_graphviz( "second.dot" );
	system("dot -Tsvg -O second.dot");
}

*/

	

	
	
	
