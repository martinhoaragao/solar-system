all: clean vertex generator

vertex: src/Vertex.cpp
	g++ -c src/Vertex.cpp -o src/Vertex.o

generator: src/generator.cpp
	g++ src/generator.cpp src/Vertex.o -o generator

clean:
	rm generator
