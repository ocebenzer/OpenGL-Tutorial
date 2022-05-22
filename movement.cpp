#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <GL/freeglut.h>
#include <iostream>

// will be defined later
const glm::vec3 INITIAL_CAMERA = glm::vec3(0.0f, 0.0, -6.0f);
const glm::vec3 INITIAL_DIRECTION = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 INITIAL_UP = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 camera(INITIAL_CAMERA);
glm::vec3 direction(INITIAL_DIRECTION);
glm::vec3 up(INITIAL_UP);

struct movement {
    float move = 0;
    float sidemove = 0;
    float fly = 0;
    float rotate = 0;
    float cam_up = 0;
    float cam_right = 0;
};

struct movement move;


void onKeyboard(unsigned char key, int x, int y) {
  switch(key) {
    case 'w':
        move.move++;
        return;
    case 'a':
		move.sidemove--;
		return;
    case 's':
		move.move--;
		return;
    case 'd':
		move.sidemove++;
		return;
    case 'q':
		move.rotate--;
		return;
    case 'e':
		move.rotate++;
		return;
    case ' ':
        move.fly++;
        return;
    case 'c':
        move.fly--;
        return;
    // special keys
    case 'x':
        glutLeaveMainLoop();
        return;
	case 'r':
		camera = INITIAL_CAMERA;
		direction = INITIAL_DIRECTION;
		up = INITIAL_UP;
		return;
    default:
        return;
  }
}

void onKeyboardUp(unsigned char key, int x, int y) {
  switch(key) {
    case 'w':
        move.move--;
        return;
    case 'a':
		move.sidemove++;
		return;
    case 's':
		move.move++;
		return;
    case 'd':
		move.sidemove--;
		return;
    case 'q':
		move.rotate++;
		return;
    case 'e':
      move.rotate--;
      return;
    case ' ':
        move.fly--;
        return;
    case 'c':
        move.fly++;
        return;
    default:
        return;
  }
}

void onSpecial(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            move.cam_up++;
            return;
        case GLUT_KEY_DOWN:
            move.cam_up--;
            return;
        case GLUT_KEY_LEFT:
            move.cam_right--;
            return;
        case GLUT_KEY_RIGHT:
            move.cam_right++;
            return;
        case GLUT_KEY_CTRL_L:
        case GLUT_KEY_SHIFT_L:
            move.fly--;
            return;
        default:
            return;
    }
}

void onSpecialUp(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            move.cam_up--;
            return;
        case GLUT_KEY_DOWN:
            move.cam_up++;
            return;
        case GLUT_KEY_LEFT:
            move.cam_right++;
            return;
        case GLUT_KEY_RIGHT:
            move.cam_right--;
            return;
        case GLUT_KEY_CTRL_L:
        case GLUT_KEY_SHIFT_L:
            move.fly++;
            return;
        default:
            return;
    }
}

void setCamera(int dt) {
	glm::mat4 transformCamera(1.0f);
	glm::mat4 transformDirection(1.0f);
	glm::mat4 transformUp(1.0f);

	glm::vec3 cameraNormal = glm::cross(direction, up);

  // forward-backward
  transformCamera = glm::translate(transformCamera, move.move*direction*0.5f);
  // right-left
  transformCamera = glm::translate(transformCamera, move.sidemove*cameraNormal*0.5f);
  // up-down
  transformCamera = glm::translate(transformCamera, move.fly*up*0.3f);

  // clk. ctr-clk . rotation
  transformUp = glm::rotate(transformUp, move.rotate*0.1f, direction);

  // camera movement - left&right
  transformDirection = glm::rotate(transformDirection, move.cam_right*-0.1f, up);

  // camera movement - up&down
  transformDirection = glm::rotate(transformDirection, move.cam_up*0.1f, cameraNormal);
  transformUp = glm::rotate(transformUp, move.cam_up*0.1f, cameraNormal);

	camera = transformCamera * glm::vec4(camera, 1.0f);
	direction = transformDirection * glm::vec4(direction, 1.0f);
	up = transformUp * glm::vec4(up, 1.0f);
}