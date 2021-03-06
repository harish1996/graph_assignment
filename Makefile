CPPFLAGS= -std=c++11

.PHONY:all testing
all: problem1 problem2 problem3a problem3b problem4 problem5a problem5b

testing: random_graph in

in: graph_in.o sud_graph.o sd_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

problem1: problem1.o sud_graph.o sd_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

problem2: problem2.o sud_graph.o sd_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

problem3a: problem3a.o sud_graph.o sd_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

problem3b: problem3b.o sud_graph.o sd_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

problem4: problem4.o sud_graph.o sd_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

problem5a: problem5a.o sud_graph.o sd_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

problem5b: problem5b.o sud_graph.o sd_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

sud_graph: sud_graph.o sd_graph.o vertex.o
	g++ -std=c++11 -g $^ -o $@

sd_graph: sd_graph.o
	g++ ${CPPFLAGS} -g $^ -o $@

random_graph: random_graph.o sud_graph.o sd_graph.o vertex.o
	g++ ${CPPFLAGS} -g $^ -o $@

%.o: %.cpp
	g++ ${CPPFLAGS} -g $^ -c -o $@

clean:
	@rm -f *.o
	@rm -f problem1
	@rm -f problem2
	@rm -f problem3
	@rm -f problem5a
	@rm -f problem5b
	@rm -f random_graph
