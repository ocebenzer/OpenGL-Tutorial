all:
	gcc -c ./common/shader_utils.cpp -o ./common/shader_utils.o
	gcc -c main.cpp -o main.o
	cc   main.o ./common/shader_utils.o  -lglut -lGLEW -lGL -lm -lstdc++ -o main
