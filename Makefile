all: generator

generator: src/generator.cpp
	g++ src/generator.cpp -o generator

clean:
	rm generator
