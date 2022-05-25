## cmpe460 project3 OmerBenzer

First of all, I'd like to point out that this project is done by referencing the tutorials at: https://en.wikibooks.org/wiki/OpenGL_Programming#The_basics_arc

* The common/shader_utils ([source](https://gitlab.com/wikibooks-opengl/modern-tutorials/-/tree/master/common)) stores the common glm functions to be used later
* Similarly, I used the teapot data given in the tutorial above
* You can use the keys described below to move around the world. Note that pressing too many buttons might cause buttons to stick, restart might be required.
* The color of the teapot is a single variable "rgba" defined in main.cpp

### Build (Ubuntu/Debian)

* Install required packages
```
sudo apt-get install build-essential libgl1-mesa-dev
sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev
sudo apt-get install libglm-dev
sudo apt-get install freeglut3 freeglut3-dev
```
* Check your OpenGL Installation
```
glxinfo | grep OpenGL
```
* make & run
```
make
./main
```
### Movement control:
* Use w-a-s-d for forward-left-backward-right movement
* Use space to ascend
* Use c/Ctrl/Shift to descend
* Use arrow keys to look around
* Use q-e to rotate the camera
* Additionally, You can reset camera using r or exit using x
For a graphic documentation of movement, see [movement documentation](documentation/README.md)
