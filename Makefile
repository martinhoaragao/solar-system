all: clean point file generator engine

build: point generator engine

point: src/point.cpp
	g++ -c src/point.cpp -o src/point.o

file: src/file.cpp
	g++ -c src/file.cpp -o src/file.o

generator: src/generator.cpp
	g++ src/generator.cpp src/point.o -o generator.out

engine: src/engine.cpp
	g++ -framework GLUT -framework OpenGL -framework Cocoa src/engine.cpp src/tinyxml2.cpp src/file.o -o engine.out -Wno-deprecated

clean:
	rm -f *.out
