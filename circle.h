/*
 * circle.h
 *
 *  Created on: 27-Feb-2016
 *      Author: abhi
 */

#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>
#include <math.h>

#if defined(_WIN64) || defined(_WIN32)
  #include <freeglut.h>
#elif __linux
  #include <GL/glut.h>
#endif

#include "point3d.h"
#include "block.h"


class Circle
{
public:
	Circle();
	Circle(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &radius, const Color3f &c);
	Circle(const Point3d &center, const GLfloat &radius, const Color3f &c);
	Circle(const Circle &other);
	~Circle();

	// operator overloading
	Circle& operator=(const Circle &other);

	// get/set method
	Point3d getCenter() const;
	void	setCenter(const Point3d &c);

	GLfloat getRadius() const;
	void	setRadius(const GLfloat &r);

	Color3f getColor() const;
	void	setColor(const Color3f &c);

	// other method
	void draw();

private:
	Point3d	center;
	GLfloat	radius;
	Color3f	color;
};

// non-member operator overloading
bool operator==(const Circle &lhs, const Circle &rhs);
bool operator!=(const Circle &lhs, const Circle &rhs);

#endif /* CIRCLE_H_ */
