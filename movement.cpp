#include <glm/glm.hpp>
#include <GL/freeglut.h>

// will be defined later
const glm::vec3 INITIAL_CAMERA = glm::vec3(0.0f, 0.0, -6.0f);
const glm::vec3 INITIAL_DIRECTION = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 INITIAL_UP = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 camera(INITIAL_CAMERA);
glm::vec3 direction(INITIAL_DIRECTION);
glm::vec3 up(INITIAL_UP);

struct movement {
    short move = 0;
    short sidemove = 0;
    short fly = 0;
    short rotate = 0;
    short cam_up = 0;
    short cam_right = 0;
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
        default:
            return;
    }
}

void setCamera(int dt) {
	glm::mat4 transformPos(1.0f);
	glm::mat4 transformTarget(1.0f);
	glm::mat4 transformUp(1.0f);

	glm::vec4 cameraPos4(camera, 1.0f);
	glm::vec4 cameraTarget4(direction, 1.0f);
	glm::vec4 cameraUp4(up, 1.0f);

	glm::vec3 cameraNormal = glm::cross(direction, up);
}