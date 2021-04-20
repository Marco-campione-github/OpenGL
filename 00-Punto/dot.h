#ifndef DOT_H
#define DOT_H

#include <iostream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

void init(int argc, char* argv[]);
void dot();
void MyScene();
void startDot(int argc, char* argv[]);

#endif //DOT_H