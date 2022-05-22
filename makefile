CXXFLAGS=-ggdb
LDLIBS=-lglut -lGLEW -lGL -lm -lstdc++
all: main
clean:
	rm -f *.o main
main: ./common/shader_utils.o main.o
.PHONY: all clean
