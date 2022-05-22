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
int screen_width=800, screen_height=600;
GLuint program;
GLuint vbo_cube_vertices;
GLuint ibo_cube_elements;
GLint attribute_coord3d, attribute_v_color, uniform_fade;  /* Global */
GLint uniform_m_transform;
GLint uniform_mvp;


struct attributes {
  	GLfloat coord3d[3];
	GLfloat v_color[3];
};

bool init_resources() {
	struct attributes cube_vertices[] = {
		// front
		{{-1.0, -1.0,  1.0}, {1.0, 0.0, 0.0}},
		{{1.0, -1.0,  1.0}, {0.0, 1.0, 0.0}},
		{{1.0,  1.0,  1.0}, {0.0, 0.0, 1.0}},
		{{-1.0,  1.0,  1.0}, {1.0, 1.0, 1.0}},
		// back
		{{-1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
		{{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
		{{1.0,  1.0, -1.0}, {0.0, 0.0, 1.0}},
		{{-1.0,  1.0, -1.0}, {1.0, 1.0, 1.0}}
	};
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	/* init_resources */
	GLushort cube_elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

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

	const char* uniform_name;
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(program, uniform_name);
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
		return 0;
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
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(program);
	glEnableVertexAttribArray(attribute_coord3d);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glVertexAttribPointer(
		attribute_coord3d,   // attribute
		3,                   // number of elements per vertex, here (x,y,z)
		GL_FLOAT,            // the type of each element
		GL_FALSE,            // take our values as-is
		sizeof(struct attributes), // next coord3d appears every 6 floats
		(GLvoid*) 0          // offset of the first element
	);
	glEnableVertexAttribArray(attribute_v_color);
	glEnableVertexAttribArray(attribute_coord3d);
	glVertexAttribPointer(
		attribute_v_color,      // attribute
		3,                      // number of elements per vertex, here (r,g,b)
		GL_FLOAT,               // the type of each element
		GL_FALSE,               // take our values as-is
		sizeof(struct attributes),    // next color appears every 6 floats
		(GLvoid*) offsetof(struct attributes, v_color)  // offset of first element
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	
	glDisableVertexAttribArray(attribute_v_color);
	glDisableVertexAttribArray(attribute_coord3d);

	SDL_GL_SwapWindow(window);
}

void onResize(int width, int height) {
  screen_width = width;
  screen_height = height;
  glViewport(0, 0, screen_width, screen_height);
}

void free_resources() {
	glDeleteProgram(program);
 	glDeleteBuffers(1, &vbo_cube_vertices);
 	glDeleteBuffers(1, &ibo_cube_elements);
}

float aspectaxis() {
	float outputzoom = 1.0f;
	float aspectorigin = 16.0f / 9.0f;
	int aspectconstraint = 1;
	switch (aspectconstraint) {
		case 1:
			if ((screen_width / screen_height) < aspectorigin) {
				outputzoom *= (((float)screen_width / screen_height) / aspectorigin);
			}
			else {
				outputzoom *= ((float)aspectorigin / aspectorigin);
			}
		break;
		case 2:
			outputzoom *= (((float)screen_width / screen_height) / aspectorigin);
		break;
		default:
			outputzoom *= ((float)aspectorigin / aspectorigin);
	}
	return outputzoom;
}

float recalculatefov() {
	return 2.0f * glm::atan(glm::tan(glm::radians(45.0f / 2.0f)) / aspectaxis());
}

void logic() {
	// mvp projection matrix
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(recalculatefov(), 1.0f * screen_width / screen_height, 0.1f, 10.0f);

	float angle = SDL_GetTicks() / 1000.0 * 45;  // 45° per second
	glm::vec3 axis_y(0, 1, 0);
	glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y);

	glm::mat4 mvp = projection * view * model * anim;
  	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	// alpha 0->1->0 every 2 seconds
	float cur_fade = sinf(SDL_GetTicks() / 1000.0 * (2*PI) / 2) / 2 + 0.5;
	glUseProgram(program);
	glUniform1f(uniform_fade, 1);
}

void mainLoop(SDL_Window* window) {
	while (true) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				onResize(ev.window.data1, ev.window.data2);
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
	SDL_Window* window = SDL_CreateWindow("My Textured Cube",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height,
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

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
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

