all: clean point generator

build: point generator


point: src/point.cpp
	g++ -c src/point.cpp -o src/point.o

generator: src/generator.cpp
	g++ src/generator.cpp src/point.o -o generator

clean:
	rm generator
