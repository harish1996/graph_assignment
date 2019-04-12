#include "vertex.h"

using namespace std;

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

int vertex::delete_edge( int n )
{
	unordered_map<int,int>::iterator res = adjacency.find(n);
	int ret = -1;
	if( res != adjacency.end() ){
		adjacency.erase(n);
		ret = 0;
	}
	return ret;
}

unordered_map<int,int>::const_iterator vertex::cbegin()
{
	return adjacency.cbegin();
}

unordered_map<int,int>::const_iterator vertex::cend()
{
	return adjacency.cend();
}
