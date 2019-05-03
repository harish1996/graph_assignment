#include<iostream>
#include "sud_graph.h"
#include <string.h>
#include <cstdlib>
#include "random_graph.h"

/**
 * @func copy_file
 * @brief Copies entire contents of a file to the buffer
 *
 * @param filename
 * @param buf The target buffer
 *
 * @return size of the file on success, -1 on failure
 */
int copy_file( char* filename, char* &buf )
{
	FILE *fp;
	int ret;
	size_t size;

	if( !( fp = fopen( filename, "r" ) ) ){
		return -1;
	}
	ret = fseek( fp, 0, SEEK_END );
	size = ftell(fp);
	rewind(fp);

	buf = (char *)malloc( size + 1 );
	if( !buf )
		return -1;
	
	ret = fread( buf, size, 1, fp );
	buf[size] = 0;	
	fclose(fp);
	return size;
}

/**
 * @func readline_list
 * @brief Reads a line of the buffer as a list
 *
 * @param buf Buffer to read from
 * @param edges The edge
 * @param weight Weight of the corresponding edge
 *
 * @return length of the line on success, negative on failure
 */ 
int readline_list( char* buf, pair<int,int>& edges, int& weight )
{
	char read = 0;
	size_t len;
	size_t spaces;
	size_t num;
	char *line;
	char *ret;

	if( !buf )
		return -1;

	line = strsep( &buf, "\n\0" );
	len = strlen(line);

	ret = strtok( line, " \n\0" );
	if( !ret ) return -2;
	edges.first = atoi(ret);

	ret = strtok( NULL, " \n\0" );
	if( !ret ) return -2;
	weight = atoi(ret);

	ret = strtok( NULL, " \n\0" );
	if( !ret ) return -2;
	edges.second = atoi(ret);

	//num = sscanf( buf+spaces,"%d %d %d\n",&edges.first,&weight,&edges.second);	
	
	//printf("%d %d %d\n",edges.first,weight,edges.second);	
	return len+1;
}

int readline_matrix( char *buf, vector<int> &edges )
{
	char read = 0;
	size_t len;
	size_t spaces;
	size_t num;
	char *line;
	char *ret;

	if( !buf )
		return -1;

	line = strsep( &buf, "\n\0" );
	len = strlen(line);

	ret = strtok( line, " " );
	while( ret ){
		edges.push_back( atoi(ret) );
		ret = strtok( NULL, " " );
	}

	return len+1;
}

	template <class T>
int read_list( char *buf, T& graph )
{
	size_t pos = 0;
	int size = strlen(buf);
	int created = 0;
	int ret;

	while( pos < size ){
		pair<int,int> edge;
		int w;

		ret = readline_list( buf, edge, w );
		if( ret < 0 ){
			ret = -4;
			goto rl_wrong_format;
		}

		buf += ret;
		pos += ret;

		if( edge.first >= created ){
			graph.add_vertex( edge.first - created + 1 );
			created = edge.first + 1;
		}
		if( edge.second >= created ){
			graph.add_vertex( edge.second - created + 1 );
			created = edge.second + 1;
		}
		if( graph.check_edge( edge.first, edge.second ) != 0 ){
			fprintf( stderr," Ignoring %d %d %d\n",edge.first,w,edge.second);
			continue;
		}		

		if( w == 0 ){
			fprintf( stderr," 0 weight edges not allowed. Ignoring %d %d %d\n",edge.first,w,edge.second);
			continue;
		}
		//printf("%d\n",created);
		graph.add_edge( edge.first, edge.second, w );
	}

rl_wrong_format:
	return ret;
}

	template <class T>
int read_matrix( char *buf, T& graph )
{
	int dim;
	vector<int> row;
	int ret;

	ret = readline_matrix( buf, row ); 
	dim = row.size();
	graph.add_vertex( dim );
	for( int j=1; j<dim; j++ ){
		if( row[j] != 0 ){
			graph.add_edge( 0, j, row[j] );
		}
	}
	row.clear();
	buf += ret;

	for( int i=1; i<dim; i++ ){
		ret = readline_matrix( buf, row );
		buf += ret;
		if( row.size() != dim ){
			ret = -5;
			goto rm_wrong_format;
		}
		for( int j=0; j<dim; j++ ){
			if( j!=i && row[j] != 0 && graph.check_edge(i,j) == 0 ){
				graph.add_edge( i, j, row[j] );
			}
		}
		row.clear();
	}

	ret = 0;
rm_wrong_format:
	return ret;
}

int read_undirected_graph( char *filename, sud_graph& graph )
{
	char read;
	int ret;
	size_t size;
	char list;
	char *buf,*tmp;

	size = copy_file( filename, buf );
	if( size == -1 ){
		ret = -1;
		goto ru_no_file;
	}

	if( buf[0] != 'u' ){
		ret = -2;
		goto ru_wrong_format;
	}

	if( buf[1] == 'm' ) list = 0;
	else if( buf[1] == 'l' ) list = 1;
	else{
		ret = -3;
		goto ru_wrong_format;
	}

	tmp = buf;
	buf += 2;
	
	while( *buf != '\n' ) buf++;
	buf++;
	
	if( list )
		read_list( buf, graph );
	else
		read_matrix( buf, graph );
	
	ret = 0;
ru_wrong_format:
ru_no_file:
	return ret;
}


int read_directed_graph( char *filename, sd_graph& graph )
{
	char read;
	int ret;
	size_t size;
	char list;
	char *buf,*tmp;

	size = copy_file( filename, buf );
	if( size == -1 ){
		ret = -1;
		goto rd_no_file;
	}

	if( buf[0] != 'd' ){
		ret = -2;
		goto rd_wrong_format;
	}

	if( buf[1] == 'm' ) list = 0;
	else if( buf[1] == 'l' ) list = 1;
	else{
		ret = -3;
		goto rd_wrong_format;
	}

	tmp = buf;
	buf += 2;
	
	while( *buf != '\n' ) buf++;
	buf++;
	
	if( list )
		read_list( buf, graph );
	else
		read_matrix( buf, graph );
	ret = 0;
rd_wrong_format:
rd_no_file:
	return ret;
}

	template <class T>
int _write_list( T& graph, char *filename, char directed )
{
	FILE *fp = fopen( filename, "w" );
       	if( !fp ) return -1;

	if( directed )
		fprintf(fp,"dl\n");	
	else
		fprintf(fp,"ul\n");

	int size = graph.get_graph_size();

	for( int i=0; i<size; i++ ){
		vertex v = graph.get_vertex( i );
		auto begin = v.cbegin(), end = v.cend();

		for( ; begin != end; begin++ ){
			if( directed || begin->first > i ) fprintf(fp,"%d %d %d\n",i,begin->second,begin->first);
		}
	}
	
	fclose(fp);
	return 0;
}

template <class T>
int write_list( T& graph, char *filename );

template<> int write_list<sd_graph>( sd_graph& graph, char *filename )
{
	return _write_list( graph, filename, 1 );
}

template<> int write_list<sud_graph>( sud_graph& graph, char *filename )
{
	return _write_list( graph, filename, 0 );
}

	template <class T>
int _write_matrix( T& graph, char *filename, char directed )
{
	FILE *fp = fopen( filename, "w" );
       	if( !fp ) return -1;

	if( directed )
		fprintf(fp,"dm\n");	
	else
		fprintf(fp,"um\n");

	int size = graph.get_graph_size();

	for( int i=0; i<size; i++ ){
		//vertex v = graph.get_vertex( i );
		/*
		auto begin = v.cbegin(), end = v.cend();

		for( ; begin != end; begin++ ){
			if( directed || begin->first > i ) fprintf(fp,"%d %d %d\n",i,begin->second,begin->first);
		}
		*/
		for( int j=0; j<size; j++ ){
			if( j != i ){
				fprintf(fp,"%d ",graph.check_edge( i, j ));
			}
			else
				fprintf(fp,"0 ");
		}
		fprintf(fp,"\n");
	}
	
	fclose(fp);
	return 0;
}

template <class T>
int write_matrix( T& graph, char *filename );

template<> int write_matrix<sd_graph>( sd_graph& graph, char *filename )
{
	return _write_matrix( graph, filename, 1 );
}

template<> int write_matrix<sud_graph>( sud_graph& graph, char *filename )
{
	return _write_matrix( graph, filename, 0 );
}

/*
int main( int argc, char* argv[] )
{
	pair<int,int> a;
	int w;
	int ret;
	sd_graph dgraph;
	sud_graph udgraph;
	int vertices,edges;

	if( argc != 4 ){
		printf(" need atleast one argument filename\n");
		return 0;
	}


	ret = read_directed_graph( argv[1], dgraph );
       	dgraph.print_graph_graphviz( "graph.dot" );
	system( "dot -Tsvg -O graph.dot" );	

	vertices = atoi( argv[2] );
	edges = atoi( argv[3] );
	//ret = read_undirected_graph( argv[1], udgraph );
       	
	ret = generate_random_graph( dgraph, vertices, edges );
       	ret = write_matrix( dgraph, "rdgraph_m" );	
	dgraph.print_graph_graphviz( "graph_d.dot" );
	system( "dot -Tsvg -O graph_d.dot" );

	ret = generate_random_graph( udgraph, vertices, edges );
       	ret = write_matrix( udgraph, "rudgraph_m" );
	udgraph.print_graph_graphviz( "graph_ud.dot" );
	system( "dot -Tsvg -O graph_ud.dot" );


	
	char *str="   54 45 83\n4         7            5\n5 3 2\n5            5 2\n";
	int len = strlen(str);
	int read = 0;
	while( read < len ){
		read += readline_directed_list( str+read, a, w );
		cout<< str+read <<endl;
		cout<< a.first<< endl<<a.second<<endl<< w<<endl;
	}
	
}
*/


