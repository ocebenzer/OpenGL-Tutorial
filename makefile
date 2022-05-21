LDLIBS=-lglut -lGLEW -lGL -lSDL2

all: triangle

triangle: ./common/shader_utils.o

clean:
	rm -f *.o triangle
.PHONY: all clean
