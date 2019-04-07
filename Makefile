CPPFLAGS= -std=c++11

.PHONY:all testing
all: testing

testing: random_graph

sud_graph: sud_graph.o
	g++ -std=c++11 -g $^ -o $@

random_graph: random_graph.o sud_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

%.o: %.cpp
	g++ ${CPPFLAGS} -g $^ -c -o $@

clean:
	rm vertex.o random_graph.o sud_graph.o random_graph
