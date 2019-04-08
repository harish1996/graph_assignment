CPPFLAGS= -std=c++11

.PHONY:all testing
all: testing

testing: random_graph

sud_graph: sud_graph.o sd_graph.o vertex.o
	g++ -std=c++11 -g $^ -o $@

sd_graph: sd_graph.o
	g++ ${CPPFLAGS} -g $^ -o $@

random_graph: random_graph.o sud_graph.o sd_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

%.o: %.cpp
	g++ ${CPPFLAGS} -g $^ -c -o $@

clean:
	rm vertex.o random_graph.o sud_graph.o random_graph
