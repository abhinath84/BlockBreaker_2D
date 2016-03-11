/*
 * utility.cpp
 *
 *  Created on: 25-Feb-2016
 *      Author: abhi
 */

#include "block.h"

// Ctor & Dtor
Block::Block()
:color(),
 isBroken(false)
{
	corner[0] = Point3d();
	corner[1] = Point3d();
	corner[2] = Point3d();
	corner[3] = Point3d();
}

Block::Block(const Point3d &lt, const Point3d &lb, const Point3d &rb, const Point3d &rt, const Color3f &c)
:color(c),
 isBroken(false)
{
	setLeftTopCorner(lt);
	setLeftBottomCorner(lb);
	setRightBottomCorner(rb);
	setRightTopCorner(rt);
}

Block::Block(const Point3d &lt, const Point3d &lb, const Point3d &rb, const Point3d &rt,
		const GLfloat &r, const GLfloat &g, GLfloat &b)
:color(r, g, b),
 isBroken(false)
{
	setLeftTopCorner(lt);
	setLeftBottomCorner(lb);
	setRightBottomCorner(rb);
	setRightTopCorner(rt);
}

Block::Block(const Block &other)
:color(other.color),
 isBroken(other.isBroken)
{
  this->corner[0] = other.corner[0];
  this->corner[1] = other.corner[1];
  this->corner[2] = other.corner[2];
  this->corner[3] = other.corner[3];
}

Block::~Block()
{
}

// operator overloading
Block& Block::operator=(const Block &other)		// copy assignment
{
	if(this != &other)
	{
		this->color = other.color;
		this->corner[0] = other.corner[0];
		this->corner[1] = other.corner[1];
		this->corner[2] = other.corner[2];
		this->corner[3] = other.corner[3];
	}

	return(*this);
}

// equality operator
bool operator==(const Block &lhs, const Block &rhs)
{
	bool result = false;

	if(lhs.getLeftTopCorner() == rhs.getLeftTopCorner() &&
			lhs.getLeftBottomCorner() == rhs.getLeftBottomCorner() &&
			lhs.getRightBottomCorner() == rhs.getRightBottomCorner() &&
			lhs.getRightTopCorner() == rhs.getRightTopCorner())
		result = true;

	return(result);
}

// not equality operator
bool operator!=(const Block &lhs, const Block &rhs)
{
	return(!(lhs==rhs));
}

// get & set method;
Color3f Block::getColor() const
{
	return(color);
}

void Block::setColor(const Color3f &c)
{
	this->color = c;
}

Point3d	Block::getLeftTopCorner() const
{
	return(this->corner[CP_LEFT_TOP]);
}

Point3d	Block::getLeftBottomCorner() const
{
	return(this->corner[CP_LEFT_BOTTOM]);
}

Point3d	Block::getRightBottomCorner() const
{
	return(this->corner[CP_RIGHT_BOTTOM]);
}

Point3d	Block::getRightTopCorner() const
{
	return(this->corner[CP_RIGHT_TOP]);
}

void Block::setLeftTopCorner(const Point3d &lt)
{
	this->corner[CP_LEFT_TOP] = lt;
}

void Block::setLeftBottomCorner(const Point3d &lb)
{
	this->corner[CP_LEFT_BOTTOM] = lb;
}

void Block::setRightBottomCorner(const Point3d &rb)
{
	this->corner[CP_RIGHT_BOTTOM] = rb;
}

void Block::setRightTopCorner(const Point3d &rt)
{
	this->corner[CP_RIGHT_TOP] = rt;
}

void Block::setCorners(const Point3d &lt, const Point3d &lb, const Point3d &rb, const Point3d &rt)
{
	this->setLeftTopCorner(lt);
	this->setLeftBottomCorner(lb);
	this->setRightBottomCorner(rb);
	this->setRightTopCorner(rt);
}

bool Block::IsBroken() const
{
	return(this->isBroken);
}

void Block::Broken(bool b)
{
	this->isBroken = b;
}

void Block::draw()
{
	// check block is broken or not.
	if(!IsBroken())
	{
		/// set the pencil
		glColor3f(color.red, color.green, color.blue);	// green
		/// draw
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(this->getLeftTopCorner().x, this->getLeftTopCorner().y,
						this->getLeftTopCorner().z);
			glVertex3f(this->getLeftBottomCorner().x, this->getLeftBottomCorner().y,
					this->getLeftBottomCorner().z);
			glVertex3f(this->getRightBottomCorner().x, this->getRightBottomCorner().y,
					this->getRightBottomCorner().z);
			glVertex3f(this->getRightTopCorner().x, this->getRightTopCorner().y,
					this->getRightTopCorner().z);
		glEnd();
	}
}
