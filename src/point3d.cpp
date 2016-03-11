/*
 * point3d.cpp
 *
 *  Created on: 27-Feb-2016
 *      Author: abhi
 */

#include "point3d.h"

// Point3d
bool operator==(const Point3d &lhs, const Point3d &rhs)
{
	bool result = false;

	if((lhs.x == rhs.x) &&
		(lhs.y == rhs.y) &&
		(lhs.z == rhs.z))
		result = true;

	return(result);
}

bool operator!=(const Point3d &lhs, const Point3d &rhs)
{
	return(!(lhs == rhs));
}

/*Point3d::Point3d()
:x(0.0f),
 y(0.0f),
 z(0.0f)
{
}

Point3d::Point3d(const GLfloat &x, const GLfloat &y, const GLfloat &z)
:x(x),
 y(y),
 z(z)
{
}

Point3d::Point3d(const Point3d &other)
:x(other.x),
 y(other.y),
 z(other.z)
{
}

Point3d::~Point3d()
{
}

// operator overloading
Point3d& Point3d::operator=(const Point3d &other)
{
	if(this != &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	return(*this);
}

// non-member operator overloading
bool operator==(const Point3d &lhs, const Point3d &rhs)
{
	bool result = false;

	if((lhs.getX() == rhs.getX()) &&
		(lhs.getY() == rhs.getY()) &&
		(lhs.getZ() == rhs.getZ()))
		result = true;

	return(result);
}

bool operator!=(const Point3d &lhs, const Point3d &rhs)
{
	return(!(lhs == rhs));
}

// get/set methods
GLfloat Point3d::getX() const
{
	return(this->x);
}

void Point3d::setX(const GLfloat &x)
{
	this->x = x;
}

GLfloat Point3d::getY() const
{
	return(this->y);
}

void Point3d::setY(const GLfloat &y)
{
	this->y = y;
}

GLfloat Point3d::getZ() const
{
	return(this->z);
}

void Point3d::setZ(const GLfloat &z)
{
	this->z = z;
}*/
