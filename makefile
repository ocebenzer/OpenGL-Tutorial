all:
	gcc -c main.cpp -o main.o
	cc   main.o common/shader_utils.o  -lglut -lGLEW -lGL -lm -lstdc++ -o main