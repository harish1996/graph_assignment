#include "random_graph.h"

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
	sd_graph graph2;
	ret = generate_random_connected_graph( graph, SIZE, EDGES ); 
	graph.print_graph();
	graph.print_graph_graphviz( "random.dot" );
	system("dot -Tsvg -O random.dot");
	ret = generate_random_connected_graph( graph, SIZE2, EDGES2 );
	graph.print_graph_graphviz( "second.dot" );
	system("dot -Tsvg -O second.dot");
	ret = generate_random_connected_graph( graph2, 15, 50 );
	graph2.print_graph_graphviz( "third.dot" );
	system("dot -Tsvg -O third.dot");
}



	

	
	
	
