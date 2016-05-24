CC=g++ -std=c++11 -Wno-deprecated -I include
CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

OUT = lib/alib.a
ODIR = build
SDIR = src
INC = -Iinc

all : generator engine

_OBJSG = patchpoints.o point.o generator.o
_OBJS += $(_OBJSG)

_OBJSE = file.o group.o light.o material.o point.o rotation.o \
				rotationanimation.o rotationstatic.o translation.o \
				translationcatmull.o translationsimple.o xmlparser.o \
				engine.o
_OBJS += $(_OBJSE)

OBJSG = $(patsubst %,$(ODIR)/%,$(_OBJSG))
OBJSE = $(patsubst %,$(ODIR)/%,$(_OBJSE))
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $< $(CFLAGS)

build/tinyxml2.o: lib/tinyxml2.cpp
	$(CC) -c lib/tinyxml2.cpp -o build/tinyxml2.o

generator: $(OBJSG)
	$(CC) $(OBJSG) -o generator

engine: $(OBJSE) build/tinyxml2.o
	$(CC) -framework GLUT -framework OpenGL -framework Cocoa $(CFLAGS) $(LIBS) $(OBJSE) build/tinyxml2.o -o engine

clean:
	rm -f build/*
	rm -f engine generator
