using namespace std;

void swap( vector<int>& arr, int from, int to )
{
	int temp;
	temp = arr[to];
	arr[to] = arr[from];
	arr[from] = temp;
}

/**
 * @func _generate_random_graph
 * @brief Helper function generating random graph with a set of vertices and a given
 *        sparseness
 *
 * @param graph Reference variable pointing to the graph
 * @param sparseness Number of edges to generate
 * @param vert_arr The array containing all the vertices to be used while generating edges
 *
 * @return 0 on success.
 */
template <class T>
int _generate_random_graph( T& graph, int& sparseness, vector<int>& vert_arr )
{
	// Array containing all possible edges
	vector<int> edge_arr;
	
	// Edge not present partition. i.e. in the edge_arr, 0 - enp is thought to
	// be not present edges. On finding an edge is present [0,enp], the particular
	// index is swapped with arr[enp] and enp is decreased by 1.
	int enp;

	int vertices = vert_arr.size();

	// Allocating space for edge array
	edge_arr.reserve(vertices*vertices);
	edge_arr.resize(vertices*vertices);

	// Initializing the edge array
	int sz = edge_arr.size();
	for( int i=0; i<sz; i++ ){
		edge_arr[i] = i;
	}

	// Initially all the edges are considered not present.
	enp = sz;
	
	while( sparseness != 0 && enp != 0 ){
		int num,from,to;
		
		// Randomly choose a non existant edge. This is like picking a matrix element
		// in random, but from a linearized matrix.
		num = rand()%enp;

		// Find the column and row of the corresponding number
		from = edge_arr[ num ] / vertices;
		to = edge_arr[ num ] % vertices;

		// If edge is already present, move it to existing edges and continue to next
		// iteration
		if( from == to || graph.check_edge( vert_arr[from], vert_arr[to] ) != 0 ){
			
			// Moving the corresponding edges to present array
			swap( edge_arr, num, --enp );
			continue;
		}
		else{
			// Add new edge between the from and to
			graph.add_edge( vert_arr[from], vert_arr[to], rand()%100 + 1 );
			swap( edge_arr, num, --enp);
			sparseness--;
		}
	}

	return 0;
}

/**
 * @func generate_random_graph
 * @brief Generates a random graph with a given number of additional vertices ( in case, a graph is
 *        already present ) and sparseness
 * 
 * @param graph Reference to the graph
 * @param vertices Number of vertices to be populated
 * @param sparseness Requested sparseness
 *
 * @return 0 on success, < 0 on error
 */
template <class T>
int generate_random_graph( T& graph, int vertices, int sparseness )
{
	if( sparseness > vertices * ( vertices - 1 ) ){
		return -1;
	}

	// The function works in a non empty graph. In case of a non
	// empty graph, the function adds a graph with `vertices` number of
	// vertices alongside the existing graph without any connections
	// to the existing graph
	int start,ret;

	// Initialising space for the array required by the helper
	// function
	vector<int> arr;
	arr.reserve(vertices);
	arr.resize(vertices);

	// Add n (more) vertices to the graph
	start = graph.add_vertex( vertices ) - vertices + 1;

	// Initialising the array
	for( int i=0; i<vertices; i++ ){
		arr[i] = start+i;
	}
	
	srand( time(NULL) );

	// Generate random graph with the array
	ret = _generate_random_graph( graph, sparseness, arr );

	return 0;
}
	
template <class T>
int generate_random_connected_graph( T& graph, int vertices, int sparseness )
{
	if( sparseness < vertices - 1 ){
		return -1;
	}
	if( sparseness > vertices * ( vertices - 1 ) ){
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

	int ret;
	
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

	// Add n (more) vertices to the graph
	start = graph.add_vertex( vertices ) - vertices + 1;

	// Allocating n number of elements in the array
	arr.reserve(vertices);
	arr.resize(vertices);

	// Initialising the array
	for( int i=0; i<vertices; i++ ){
		arr[i] = start+i;
	}

	// Randomly pick a vertex and move it the connected portion
	swap( arr, rand()%(nc+1), nc );
	
	// Until all vertices are connected
        while( nc != 0 ){
		int from,to;

		// Randomly picking a connected vertex
		from = ( rand()%( vertices - nc ) ) + nc;

		// Randomly picking a non connected vertex
		to = ( rand()%nc );

		// Put an edge between them with a random integer weight
		graph.add_edge( arr[from], arr[to], rand()%100 );

		// Moving the selected non connected vertex to connected set
		swap( arr, to, nc-1 );

		nc--;

		sparseness--;
	}

	ret = _generate_random_graph( graph, sparseness, arr );

	return ret;
}

/**
 * @func _remove_backedges
 * @brief Helper recursive function for removing backedges from a directed graph
 *
 * @param graph Reference to the graph
 * @param cur Current vertex
 * @param ancestry Vector keeping track of ancestry
 * @param visited Vector keeping track of visited nodes
 *
 * @return 0 on success
 */
int _remove_backedges( sd_graph& graph, int cur, vector<bool>& ancestry, vector<bool>& visited )
{
	vertex ver = graph.get_vertex( cur );
	auto begin = ver.cbegin();
	auto end = ver.cend();

	ancestry[cur] = true;
	visited[cur] = true;
	
	for( ; begin != end; begin++ ){

		if( ancestry[begin->first] == true ){
			graph.delete_edge( cur, begin->first );
		}
		else{
			_remove_backedges( graph, begin->first, ancestry, visited );
		}
	}
	
	ancestry[cur] = false;

	return 0;
}

/**
 * @func remove_backedges
 * @brief User facing function which removes back edges in a directed graph
 *
 * @param graph Graph reference
 * @param start Start of the DAG vertices
 * @param end End of the DAG vertices
 *
 * @return 0 on success
 */
int remove_backedges( sd_graph& graph, int start, int end )
{
	int size = graph.get_graph_size();
	
	vector<bool> ancestry( size, false);
	vector<bool> visited( size, false);

	/*
	// Array required for generating random permutation.
	// Expected apparent improvement in randomness.
	vector<int> v_arr( end-start+1, 0);
	
	srand( time(NULL) );

	// Generating a random permutation of vertices in the
	// Directed graph. Random permutation is necessary because
	// backedge removal will give different results for
	// different strategies of backedge removal. But is it necessary ?
	// This is Questionable.!
	for( int i=0; i< end-start+1; i++ ){
		int last = end-start+1-i
		int index = rand()%(last);
		
		swap( v_arr, index, last );

		if( !v_arr[index] ) v_arr[index] = start+last+1;
		if( !v_arr[last] ) v_arr[last] = start+index+1;
	}
	*/
	
	for( int i=start; i<end; i++ ){
		//int index = v_arr[i] - 1;
		if( visited[ i ] == false )
			_remove_backedges( graph, i, ancestry, visited );
	}

	return 0;
}

/**
 * @func generate_random_DAG
 * @brief Generates random Directed Acyclic Graph with a maximum `sparseness`
 *
 * Generates DAG by removing backedges from a directed graph which is generated
 * in random.
 *
 * @param graph The reference to the graph 
 * @param vertices Vertices to be added
 * @param max_sparseness Maximum sparseness of the DAG
 *
 * @return 0 on success
 */
int generate_random_DAG( sd_graph& graph, int vertices, int max_sparseness )
{
	int size = graph.get_graph_size();
	int ret;

	ret = generate_random_graph( graph, vertices, max_sparseness );
	if( ret )
		return -1;
	ret = remove_backedges( graph, size, size + vertices - 1 );

	return 0;
}
