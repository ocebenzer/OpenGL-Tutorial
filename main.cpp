/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./common/shader_utils.h"
#include "teapot.cpp"
#include "camera.cpp"

#define PI 3.1415926535f

int screen_width=800, screen_height=600;
GLuint vbo_teapot_vertices, ibo_teapot_elements;
GLuint program;
GLint attribute_coord3d;
GLint uniform_mvp, uniform_rgba;

int currentTime, dt;

glm::vec4 rgba(0.3, 0.3, 0.3, 1.0);

bool init_resources() {
    build_teapot();

    glGenBuffers(1, &vbo_teapot_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_teapot_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(teapot_vertices), teapot_vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo_teapot_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_teapot_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapot_elements), teapot_elements, GL_STATIC_DRAW);

    GLint link_ok = GL_FALSE;

    GLuint vs, fs;
    if ((vs = create_shader("obj.vs", GL_VERTEX_SHADER))   == 0) return 0;
    if ((fs = create_shader("obj.fs", GL_FRAGMENT_SHADER)) == 0) return 0;

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        fprintf(stderr, "glLinkProgram:");
        print_log(program);
        return false;
    }

    attribute_coord3d = get_attrib(program, "coord3d");
    uniform_mvp = get_uniform(program, "mvp");
    uniform_rgba = get_uniform(program, "rgba");

    return true;
}

void onIdle() { // logic goes here
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    dt = currentTime - currentTime;
    currentTime = currentTime;

    setCamera(dt);

    glm::mat4 model = glm::rotate(glm::mat4(1.0f), PI/2, glm::vec3(-1.0f, 0.0f, 0.0f));
    glm::mat4 view = glm::lookAt(camera, direction+camera, up);
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);

    glm::mat4 mvp = projection * view * model;

    glUseProgram(program);
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform4fv(uniform_rgba, 1, glm::value_ptr(rgba));
    glutPostRedisplay();
}

void onDisplay() { // calculate per frame
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Draw Teapot
    glUseProgram(program);
    glEnableVertexAttribArray(attribute_coord3d);
    // Describe our vertices array to OpenGL (it can't guess its format automatically)
    glBindBuffer(GL_ARRAY_BUFFER, vbo_teapot_vertices);
    glVertexAttribPointer(
        attribute_coord3d, // attribute
        3,                 // number of elements per vertex, here (x,y,z)
        GL_FLOAT,          // the type of each element
        GL_FALSE,          // take our values as-is
        0,                 // no extra data between each position
        0                  // offset of first element
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_teapot_elements);
    glDrawElements(GL_TRIANGLES, sizeof(teapot_elements)/sizeof(teapot_elements[0]), GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(attribute_coord3d);
    glutSwapBuffers();
}

void onReshape(int width, int height) {
    screen_width = width;
    screen_height = height;
    glViewport(0, 0, screen_width, screen_height);
}

void free_resources() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_teapot_vertices);
    glDeleteBuffers(1, &ibo_teapot_elements);
}


int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitContextVersion(2,0);
    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(screen_width, screen_height);
    glutCreateWindow("My unicolor teapot w/ movement");

    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return 1;
    }

    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
        return 1;
    }

    if (init_resources()) {
        glutDisplayFunc(onDisplay);
        glutReshapeFunc(onReshape);
        glutIdleFunc(onIdle);
        glutKeyboardFunc(onKeyboard);
        glutKeyboardUpFunc(onKeyboardUp);
        glutSpecialFunc(onSpecial);
        glutSpecialUpFunc(onSpecialUp);
        glFrustum(-1000, 1000, -1000, 1000, -1000, 1000);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glutMainLoop();
    }

    free_resources();
    return 0;
}
