#include <iostream>
#include <string>
#include <string.h>

using namespace std;

char *readlines( int lines ) 
{
	string full;
	string line;
	for( int i=0; i<lines; i++ ){
		getline(line);
		full += line;
	}

	return strdup( full.c_str() );
}

int main()
{
	int n;
	cin>>n;
	char *s;

	cout<<readlines(n);
}
