/* Using standard C++ output libraries */
#include <cstdlib>
#include <iostream>
using namespace std;

/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/* Using SDL2 for the base window and OpenGL context init */
#include <SDL2/SDL.h>

// include, utility functions etc.
#include "./common/shader_utils.h"

#define PI 3.141592653589f

// global variables
GLuint program;
GLuint vbo_triangle;
GLint attribute_coord3d, attribute_v_color, uniform_fade;  /* Global */
GLint uniform_m_transform;

struct attributes {
  	GLfloat coord3d[3];
	GLfloat v_color[3];
};

bool init_resources() {
	// vertices
	struct attributes triangle_attributes[] = {
		{{ 0.0,  0.8, 0.0}, {1.0, 1.0, 0.0}},
		{{-0.8, -0.8, 0.0}, {0.0, 0.0, 1.0}},
		{{ 0.8, -0.8, 0.0}, {1.0, 0.0, 0.0}}
	};
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

	// shaders
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
	
	const char* attribute_name = "coord3d";
	attribute_coord3d = glGetAttribLocation(program, attribute_name);
	if (attribute_coord3d == -1) {
		cerr << "Could not bind attribute " << attribute_name << endl;
		return false;
	}
	attribute_name = "v_color";
	attribute_v_color = glGetAttribLocation(program, attribute_name);
	if (attribute_v_color == -1) {
		cerr << "Could not bind attribute " << attribute_name << endl;
		return false;
	}

	const char* uniform_name = "m_transform";
	uniform_m_transform = glGetUniformLocation(program, uniform_name);
	if (uniform_m_transform == -1) {
		cerr << "Could not bind uniform " << uniform_name << endl;
		return false;
	}

	uniform_name = "fade";
	uniform_fade = glGetUniformLocation(program, uniform_name);
	if (uniform_fade == -1) {
		cerr << "Could not bind uniform " << uniform_name << endl;
		return false;
	}
	
	return true;
}

void render(SDL_Window* window) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(program);

	glEnableVertexAttribArray(attribute_coord3d);
	glEnableVertexAttribArray(attribute_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glVertexAttribPointer(
		attribute_coord3d,   // attribute
		3,                   // number of elements per vertex, here (x,y,z)
		GL_FLOAT,            // the type of each element
		GL_FALSE,            // take our values as-is
		sizeof(struct attributes), // next coord3d appears every 6 floats
		(GLvoid*) 0          // offset of the first element
	);
	glVertexAttribPointer(
		attribute_v_color,      // attribute
		3,                      // number of elements per vertex, here (r,g,b)
		GL_FLOAT,               // the type of each element
		GL_FALSE,               // take our values as-is
		sizeof(struct attributes),    // next color appears every 6 floats
		(GLvoid*) offsetof(struct attributes, v_color)  // offset of first element
	);
	
	/* Push each element in buffer_vertices to the vertex shader */
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_v_color);

	SDL_GL_SwapWindow(window);
}

void free_resources() {
	glDeleteProgram(program);
 	glDeleteBuffers(1, &vbo_triangle);
}

void logic() {
	float movex = sinf(SDL_GetTicks() / 1000.0 * (-2*PI) / 4); // -1<->+1 every 5 seconds
	float movey = cosf(SDL_GetTicks() / 1000.0 * (2*PI) / 4); // -1<->+1 every 5 seconds
	float angle = SDL_GetTicks() / 1000.0 * 90;  // 45° per second
	glm::vec3 axis_z(0, 0, 1);
	glm::mat4 m_transform = 
		glm::translate(glm::mat4(1.0f), glm::vec3(movex, movey, 0.0)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z) *
		glm::translate(glm::mat4(1.0f), glm::vec3(0, -1.8, 0));
	 glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));

	// alpha 0->1->0 every 2 seconds
	float cur_fade = sinf(SDL_GetTicks() / 1000.0 * (2*PI) / 2) / 2 + 0.5;
	glUseProgram(program);
	glUniform1f(uniform_fade, 1);
}

void mainLoop(SDL_Window* window) {
	while (true) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT)
				return;
		}
		logic();
		render(window);
	}
}

int main(int argc, char* argv[]) {
	/* SDL-related initialising functions */
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("My Second Triangle",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 480,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (window == NULL) {
		cerr << "Error: can't create window: " << SDL_GetError() << endl;
		return EXIT_FAILURE;
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);
	if (SDL_GL_CreateContext(window) == NULL) {
		cerr << "Error: SDL_GL_CreateContext: " << SDL_GetError() << endl;
		return EXIT_FAILURE;
	}

	/* Extension wrangler initialising */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << endl;
		return EXIT_FAILURE;
	}

	if (!GLEW_VERSION_2_0) {
		cerr << "Error: your graphic card does not support OpenGL 2.0" << endl;
		return EXIT_FAILURE;
	}

	/* When all init functions run without errors,
	   the program can initialise the resources */
	if (!init_resources())
		return EXIT_FAILURE;

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* We can display something if everything goes OK */
	mainLoop(window);

	/* If the program exits in the usual way,
	   free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}

