/*
 * utility.h
 *
 *  Created on: 26-Feb-2016
 *      Author: abhi
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

#if defined(_WIN64) || defined(_WIN32)
  #include <freeglut.h>
#elif __linux
  #include <GL/glut.h>
#endif

#include "block.h"
#include "circle.h"

using namespace std;

void glOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
void glutBitmapString(void* font, char *string);

#endif /* UTILITY_H */
