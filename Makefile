all: generator engine

build: point generator

point: src/point.cpp
	g++ -c src/point.cpp -o src/point.o

generator: src/generator.cpp
	g++ src/generator.cpp src/point.o -o generator.out

engine: src/engine.cpp
	g++ src/engine.cpp src/tinyxml2.cpp -o engine.out

clean:
	rm *.out
