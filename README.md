### OpenGL

Followed tutorial at: https://en.wikibooks.org/wiki/OpenGL_Programming#The_basics_arc

Tutorial repository at: https://gitlab.com/wikibooks-opengl/modern-tutorials

### Installation (Ubuntu/Debian)

* Install required packages
```
sudo apt-get install build-essential libgl1-mesa-dev
sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev libglm-dev libfreetype6-dev
apt-get install libglm-dev
```
* Check your OpenGL Installation
```
glxinfo | grep OpenGL
```
* Select a branch
* make & run
```
make
./opengl
```