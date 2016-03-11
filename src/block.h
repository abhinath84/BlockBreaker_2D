/*
 * block.h
 *
 *  Created on: 25-Feb-2016
 *      Author: abhi
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>

#if defined(_WIN64) || defined(_WIN32)
  #include <freeglut.h>
#elif __linux
  #include <GL/glut.h>
#endif

#include "point3d.h"

struct Color3f
{
	// Ctor & Dtor
	Color3f()
	{
		red = green = blue = 0.0f;
	}

	Color3f(const GLfloat &r, const GLfloat &g, const GLfloat &b)
	{
		this->red = r;
		this->green = g;
		this->blue = b;
	}

	// member variables
	GLfloat red;
	GLfloat green;
	GLfloat blue;
};

class Block
{
public:
	// Ctor & Dtor
	Block();
	Block(const Point3d &lt, const Point3d &lb, const Point3d &rb, const Point3d &rt, const Color3f &c);
	Block(const Point3d &lt, const Point3d &lb, const Point3d &rb, const Point3d &rt,
			const GLfloat &r, const GLfloat &g, GLfloat &b);
	Block(const Block &other);
	~Block();

	// operators
	Block& operator=(const Block &other);		// copy assignment

	// get & set method;
	Color3f getColor() const;
	void 	setColor(const Color3f &c);

	Point3d	getLeftTopCorner() const;
	Point3d	getLeftBottomCorner() const;
	Point3d	getRightBottomCorner() const;
	Point3d	getRightTopCorner() const;
	void 	setLeftTopCorner(const Point3d &lt);
	void 	setLeftBottomCorner(const Point3d &lb);
	void 	setRightBottomCorner(const Point3d &rb);
	void 	setRightTopCorner(const Point3d &rt);

	void	setCorners(const Point3d &lt, const Point3d &lb, const Point3d &rb, const Point3d &rt);

	bool IsBroken() const;
	void Broken(bool b);

	void draw();

private:
	// helper function

private:
	Point3d corner[4];		// [lt, lb, rb, rt]
	Color3f	color;
	bool	isBroken;

	enum CORNER_POINT
	{
		CP_LEFT_TOP = 0,
		CP_LEFT_BOTTOM = 1,
		CP_RIGHT_BOTTOM = 2,
		CP_RIGHT_TOP = 3
	};
};

// non-member operator overloading
bool operator==(const Block &lhs, const Block &rhs);
bool operator!=(const Block &lhs, const Block &rhs);

#endif /* BLOCK_H */
