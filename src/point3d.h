/*
 * point3d.h
 *
 *  Created on: 27-Feb-2016
 *      Author: abhi
 *
 *  Class : Point3d
 *
 *  Desc  : Structure to define 3D point.
 */

#ifndef POINT3D_H
#define POINT3D_H

#include <iostream>

#if defined(_WIN64) || defined(_WIN32)
  #include <freeglut.h>
#elif __linux
  #include <GL/glut.h>
#endif

using namespace std;


struct Point3d
{
	// Ctor & Dtor
	Point3d()
	{
		x = y = z = 0.0f;
	}

	Point3d(const GLfloat &x, const GLfloat &y, const GLfloat &z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// member variables
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

// non-member function
bool operator==(const Point3d &lhs, const Point3d &rhs);
bool operator!=(const Point3d &lhs, const Point3d &rhs);

/*class Point3d
{
public:
	// Ctor & Dtor
	Point3d();
	Point3d(const GLfloat &x, const GLfloat &y, const GLfloat &z);
	Point3d(const Point3d &other);
	~Point3d();

	// operator overloading
	Point3d& operator=(const Point3d &other);

	// get/set methods
	GLfloat getX() const;
	void	setX(const GLfloat &x);
	GLfloat getY() const;
	void	setY(const GLfloat &y);
	GLfloat getZ() const;
	void   setZ(const GLfloat &z);

private:
	// member variables
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

// non-member function
bool operator==(const Point3d &lhs, const Point3d &rhs);
bool operator!=(const Point3d &lhs, const Point3d &rhs);*/

#endif /* POINT3D_H */
