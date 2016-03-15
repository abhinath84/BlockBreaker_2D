/*
 * utility.cpp
 *
 *  Created on: 26-Feb-2016
 *      Author: abhi
 */

#include "utility.h"

void glOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
	//  Set the world coordinates.
	//  gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
	//  The default world coordinates are between -1 and 1.
	//  So it is as if you're calling
	//  gluOrtho2D(-1.0, 1.0, -1.0, 1.0)
	gluOrtho2D(left, right, bottom, top);
}

void glutBitmapString(void* font, char *string)
{
	int size = strlen(string);

	//glRasterPos3f(x, y, 0.0f);
	for(int i = 0; i < size; ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
}
