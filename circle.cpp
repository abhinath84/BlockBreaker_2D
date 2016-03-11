/*
 * circle.cpp
 *
 *  Created on: 27-Feb-2016
 *      Author: abhi
 */

#include "circle.h"

Circle::Circle()
:center(),
 radius(0.0f),
 color()
{
}

Circle::Circle(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &radius, const Color3f &c)
:center(x, y, z),
 radius(radius),
 color(c)
{
}

Circle::Circle(const Point3d &center, const GLfloat &radius, const Color3f &c)
:center(center),
 radius(radius),
 color(c)
{
}

Circle::Circle(const Circle &other)
:center(other.center),
 radius(other.radius),
 color(other.color)
{
}

Circle::~Circle()
{
}

// operator overloading
Circle& Circle::operator=(const Circle &other)
{
	if(this != &other)
	{
		this->center = other.center;
		this->radius = other.radius;
		this->color = other.color;
	}

	return(*this);
}

bool operator==(const Circle &lhs, const Circle &rhs)
{
	bool status = false;

	if(lhs.getCenter() == rhs.getCenter())
		status = true;

	return(status);
}
bool operator!=(const Circle &lhs, const Circle &rhs)
{
	return(!(lhs == rhs));
}

Point3d Circle::getCenter() const
{
	return(this->center);
}

void Circle::setCenter(const Point3d &c)
{
	this->center = c;
}

GLfloat Circle::getRadius() const
{
	return(this->radius);
}

void Circle::setRadius(const GLfloat &r)
{
	this->radius = r;
}

Color3f Circle::getColor() const
{
	return(this->color);
}

void Circle::setColor(const Color3f &c)
{
	this->color = c;
}

/*
 * Function that handles the drawing of a circle using the triangle fan
 * method. This will create a filled circle.
 *
 * Params:
 *	x (GLFloat) - the x position of the center point of the circle
 *	y (GLFloat) - the y position of the center point of the circle
 *	radius (GLFloat) - the radius that the painted circle will have
 */
void Circle::draw()
{
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
  GLfloat twicePi = (GLfloat)(2.0f * 3.14159);

	// set pencil
	glColor3f(color.red, color.green, color.blue);

	// draw
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(center.x, center.y, center.z); // center of circle

		for(i = 0; i <= triangleAmount;i++)
		{
			glVertex3f(center.x + (radius * cos(i *  twicePi / triangleAmount)),
						center.y + (radius * sin(i * twicePi / triangleAmount)), 0.0f);
		}
	glEnd();
}
