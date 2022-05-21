/* Using standard C++ output libraries */
#include <cstdlib>
#include <iostream>
using namespace std;

/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using SDL2 for the base window and OpenGL context init */
#include <SDL2/SDL.h>

// include, utility functions etc.
#include "./common/shader_utils.h"

// global variables
GLuint program;
GLint attribute_coord2d;
GLuint vbo_triangle;


bool init_resources() {
	GLfloat triangle_vertices[] = {
	    0.0,  0.8,
	   -0.8, -0.8,
	    0.8, -0.8,
	};
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	
	GLint link_ok = GL_FALSE;
	
	GLuint vs, fs;
	if ((vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER))   == 0) return false;
	if ((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return false;
	
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		cerr << "glLinkProgram:";
		print_log(program);
		return false;
	}
	
	const char* attribute_name = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord2d == -1) {
		cerr << "Could not bind attribute " << attribute_name << endl;
		return false;
	}
	
	return true;
}

void render(SDL_Window* window) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(program);
	glEnableVertexAttribArray(attribute_coord2d);
	/* Describe our vertices array to OpenGL (it can't guess its format automatically) */
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glVertexAttribPointer(
		attribute_coord2d, // attribute
		2,                 // number of elements per vertex, here (x,y)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);
	
	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableVertexAttribArray(attribute_coord2d);
	SDL_GL_SwapWindow(window);
}

void free_resources() {
	glDeleteProgram(program);
}

void mainLoop(SDL_Window* window) {
	while (true) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT)
				return;
		}
		render(window);
	}
}

int main(int argc, char* argv[]) {
	/* SDL-related initialising functions */
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("My First Triangle",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 480,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_GL_CreateContext(window);

	/* Extension wrangler initialising */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << endl;
		return EXIT_FAILURE;
	}

	/* When all init functions run without errors,
	   the program can initialise the resources */
	if (!init_resources())
		return EXIT_FAILURE;

	/* We can display something if everything goes OK */
	mainLoop(window);

	/* If the program exits in the usual way,
	   free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}