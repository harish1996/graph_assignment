CPPFLAGS= -std=c++11

.PHONY:all testing
all: testing

testing: sud_graph

sud_graph: sud_graph.o
	g++ -std=c++11 $^ -o $@

%.o: %.cpp
	g++ ${CPPFLAGS} $^ -c -o $@

clean:
	rm sud_graph.o sud_graph
