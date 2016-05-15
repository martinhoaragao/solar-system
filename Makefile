CC=g++ -std=c++11 -I include

all: point rotation rotationstatic rotationanimation translation translationcatmull translationsimple file xmlparser group patchPoints generator engine

point: src/point.cpp
	$(CC) -c src/point.cpp -o src/point.o

rotation: src/rotation.cpp
	$(CC) -c src/rotation.cpp -o src/rotation.o

rotationanimation: src/rotationanimation.cpp
	$(CC) -c src/rotationanimation.cpp -o src/rotationanimation.o -Wno-deprecated

rotationstatic: src/rotationstatic.cpp
	$(CC) -c src/rotationstatic.cpp -o src/rotationstatic.o -Wno-deprecated

translation: src/translation.cpp
	$(CC) -c src/translation.cpp -o src/translation.o

translationcatmull: src/translationcatmull.cpp
	$(CC) -c src/translationcatmull.cpp -o src/translationcatmull.o -Wno-deprecated

translationsimple: src/translationsimple.cpp
	$(CC) -c src/translationsimple.cpp -o src/translationsimple.o -Wno-deprecated

file: src/file.cpp
	$(CC) -c src/file.cpp -o src/file.o

xmlparser: src/xmlparser.cpp
	$(CC) -c src/xmlparser.cpp -o src/xmlparser.o

group: src/group.cpp
	$(CC) -c src/group.cpp -o src/group.o

patchPoints: src/patchPoints.cpp src/patchPoints.o
	$(CC) -c src/patchPoints.cpp -o src/patchPoints.o

generator: src/generator.cpp src/point.o
	$(CC) src/generator.cpp src/patchPoints.o src/point.o -o generator.out

# Fix this...
engine: src/engine.cpp src/file.o
	$(CC) -framework GLUT -framework OpenGL -framework Cocoa src/point.o src/engine.cpp lib/tinyxml2.cpp src/file.o src/xmlparser.o src/group.o src/rotation.o src/rotationstatic.o src/rotationanimation.o src/translation.o src/translationcatmull.o src/translationsimple.o -o engine.out -Wno-deprecated

clean:
	rm -f *.out
