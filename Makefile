CC=g++ -std=c++11

all: point rotation rotationstatic rotationanimation file xmlparser group generator engine

point: src/point.cpp
	$(CC) -c src/point.cpp -o src/point.o

rotation: src/rotation.cpp
	$(CC) -c src/rotation.cpp -o src/rotation.o

rotationanimation: src/rotationanimation.cpp src/rotation.o
	$(CC) -c src/rotationanimation.cpp src/rotation.o -o src/rotationanimation.o -Wno-deprecated

rotationstatic: src/rotationstatic.cpp src/rotation.o
	$(CC) -c src/rotationstatic.cpp src/rotation.o -o src/rotationstatic.o -Wno-deprecated

file: src/file.cpp
	$(CC) -c src/file.cpp -o src/file.o

xmlparser: src/xmlparser.cpp
	$(CC) -c src/xmlparser.cpp -o src/xmlparser.o

group: src/group.cpp
	$(CC) -c src/group.cpp -o src/group.o

generator: src/generator.cpp src/point.o
	$(CC) src/generator.cpp src/point.o -o generator.out

# Fix this...
engine: src/engine.cpp src/file.o
	$(CC) -framework GLUT -framework OpenGL -framework Cocoa src/point.o src/engine.cpp src/tinyxml2.cpp src/file.o src/xmlparser.o src/group.o src/rotation.o src/rotationstatic.o src/rotationanimation.o -o engine.out -Wno-deprecated

clean:
	rm -f *.out
