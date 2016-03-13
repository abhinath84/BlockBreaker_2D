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

void setBlock(Block &block, const Point3d &lt, const Point3d &lb, const Point3d &rb, const Point3d &rt,
		const Color3f &c)
{
	block.setCorners(lt, lb, rb, rt);
	block.setColor(c);
}

void resizeCallback_low(GLfloat &win_left, GLfloat &win_right,
						GLfloat &win_bottom, GLfloat &win_top,
						int &w, int &h)
{
	GLfloat aspectRatio;
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
	{
		win_left = -100.0;
		win_right = 100.0;
		win_bottom = -100.0f / aspectRatio;
		win_top = 100.0f / aspectRatio;
	}
	else
	{
		win_left = -100.0f * aspectRatio;
		win_right = 100.0f * aspectRatio;
		win_bottom = -100.0;
		win_top = 100.0;
	}

	//glOrtho (win_left, win_right, win_bottom, win_top, 1.0, -1.0);
	glOrtho2D(win_left, win_right, win_bottom, win_top);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initTopBlock(vector<Block> &blocks, const GLfloat &win_left, const GLfloat &win_right,
					const GLfloat &win_top, const GLfloat &block_width, const GLfloat &block_height)
{
	int row_num	= 4;

	/// reset blocks
	blocks.clear();

	/// start with top left corner.
	for(int i = 0; i < row_num; ++i)
	{
		int block_num = (int)((win_right - win_left) / block_width);
		for(int j = 0; j <= block_num; ++j)
		{
			Color3f c;

			/// set the pencil
			if(((i%2 == 0) && ((j == 0) || (j%4 == 0))) || ((i%2 != 0) && ((j == 3) || (j%4 == 3))))
			{
				c.red = 1.0f; c.green = 0.0f; c.blue = 0.0f;	// red
			}

			if(((i%2 == 0) && ((j == 1) || (j%4 == 1))) || ((i%2 != 0) && ((j == 2) || (j%4 == 2))))
			{
				c.red = 0.0f; c.green = 1.0f; c.blue = 0.0f;	// green
			}

			if(((i%2 == 0) && ((j == 2) || (j%4 == 2))) || ((i%2 != 0) && ((j == 1) || (j%4 == 1))))
			{
				c.red = 0.0f; c.green = 0.0f; c.blue = 1.0f;	// blue
			}

			if(((i%2 == 0) && ((j == 3) || (j%4 == 3))) || ((i%2 != 0) && ((j == 0) || (j%4 == 0))))
			{
				c.red = 1.0f; c.green = 1.0f; c.blue = 1.0f;	// white
			}

			/// draw
			Point3d lt = Point3d((win_left + (block_width*j)), (win_top - (block_height*i)), 0.0f);
			Point3d lb = Point3d((win_left + (block_width*j)), (win_top - (block_height*(i+1))), 0.0f);
			Point3d rb = Point3d((win_left + (block_width*(j+1))), (win_top - (block_height*(i+1))), 0.0f);
			Point3d rt = Point3d((win_left + (block_width*(j+1))), (win_top - (block_height*i)), 0.0f);

			blocks.push_back(Block(lt, lb, rb, rt, c));
		}
	}
}

// pos 	= 1 -> left
//		= 2 -> center
//		= 3 -> right
bool isBallBottomBlockCollide(const Circle &ball, const Block &bottom_block, int &pos)
{
	bool status = false;

	// check collision of bottom block with ball.
	// ball will collide with bottom block if
	// 1. ball.center.y + radius <= bottom block.lt.y
	// 2. bottom block.lt.x < ball.center.x < bottom block.rt.x

	if((ball.getCenter().y - ball.getRadius()) <= bottom_block.getLeftTopCorner().y)
	{
		if((bottom_block.getLeftTopCorner().x < ball.getCenter().x) &&
				(ball.getCenter().x <= bottom_block.getRightTopCorner().x))
		{
			status = true;

			// set the collision pos.
			GLfloat middle = ((bottom_block.getLeftTopCorner().x + bottom_block.getRightTopCorner().x)/2);

			// left
			if(ball.getCenter().x < (middle - 4))
				pos = 1;
			// right
			else if(ball.getCenter().x > (middle + 4))
				pos = 3;
			// middle
			else
				pos = 2;
		}
	}

	return(status);
}

bool isGameOver(const Circle &ball, const Block &bottom_block, const GLfloat &win_bottom)
{
	bool status = false;

	if((ball.getCenter().y + ball.getRadius()) <= win_bottom)
	{
		int collision_pos = 0;
		if(!isBallBottomBlockCollide(ball, bottom_block, collision_pos))
			status = true;
	}

	return(status);
}

bool isGameWin(const vector<Block> &top_blocks)
{
	bool status = false;
	int size = top_blocks.size();

	// check all blocks are broken or not
	for(int i = 0; i < size; ++i)
	{
		if(top_blocks[i].IsBroken())
			status = true;
		else
		{
			status = false;
			break;
		}
	}

	return(status);
}

void drawGameOverBlock(const GLfloat &center_x, const GLfloat &center_y)
{
	// draw a red colored rectangle based on the center of the game area.
	/// set the pencil
	glColor3f(0.75f, 0.0f, 0.0f);	// green
	/// draw
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f((center_x - 45.0f), (center_y + 25.0f),0.0f);
		glVertex3f((center_x - 45.0f), (center_y - 25.0f),0.0f);
		glVertex3f((center_x + 45.0f), (center_y - 25.0f),0.0f);
		glVertex3f((center_x + 45.0f), (center_y + 25.0f),0.0f);
	glEnd();

	// write text.
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos3f((center_x - 30.0f), (center_y + 10.0f), 0.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (char *)("Game Over !!!"));
}

void drawGameWinBlock(const GLfloat &center_x, const GLfloat &center_y)
{
	// draw a red colored rectangle based on the center of the game area.
	/// set the pencil
	glColor3f(0.0f, 0.75f, 0.0f);	// green
	/// draw
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f((center_x - 45.0f), (center_y + 25.0f),0.0f);
		glVertex3f((center_x - 45.0f), (center_y - 25.0f),0.0f);
		glVertex3f((center_x + 45.0f), (center_y - 25.0f),0.0f);
		glVertex3f((center_x + 45.0f), (center_y + 25.0f),0.0f);
	glEnd();

	// write text.
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos3f((center_x - 30.0f), (center_y + 10.0f), 0.0f);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (char *)("Game Win !!!"));
}

void moveBall(Circle &ball, int &ball_state, const GLfloat &step,
		const GLfloat &win_left, const GLfloat &win_right, const GLfloat &win_top, const GLfloat &win_bottom)
{
	Point3d old_center = ball.getCenter();

	GLfloat x = old_center.x;
	GLfloat y = old_center.y;
	GLfloat z = 0.0f;

	if(ball_state == BM_DOWN_LEFT)
	{
		// check collide with left wall
		if((old_center.x - ball.getRadius() <= win_left))
		{
			// if collide then move ball to BM_DOWN_RIGHT
			ball_state = BM_DOWN_RIGHT;
		}
		else
		{
			x = old_center.x - step;
			y = old_center.y - step;
		}
	}
	else if(ball_state == BM_DOWN_RIGHT)
	{
		// check collide with right wall
		if((old_center.x + ball.getRadius() >= win_right))
		{
			// if collide then move ball to BM_DOWN_LEFT
			ball_state = BM_DOWN_LEFT;
		}
		else
		{
			x = old_center.x + step;
			y = old_center.y - step;
		}
	}
	else if(ball_state == BM_UP_LEFT)
	{
		// check collide with left wall
		// if collide then move ball to BM_UP_RIGHT
		if((old_center.x - ball.getRadius() <= win_left))
		{
			ball_state = BM_UP_RIGHT;
		}
		// check collide with top wall
		// if collide then move ball to BM_DOWN_LEFT
		else if((old_center.y + ball.getRadius() >= win_top))
		{
			ball_state = BM_DOWN_LEFT;
		}
		else
		{
			x = old_center.x - step;
			y = old_center.y + step;
		}
	}
	else if(ball_state == BM_UP_RIGHT)
	{
		// check collide with right wall
		if((old_center.x + ball.getRadius() >= win_right))
		{
			// if collide then move ball to BM_UP_LEFT
			ball_state = BM_UP_LEFT;
		}
		// check collide with top wall
		else if((old_center.y + ball.getRadius() >= win_top))
		{
			// if collide then move ball to BM_DOWN_RIGHT
			ball_state = BM_DOWN_RIGHT;
		}
		else
		{
			x = old_center.x + step;
			y = old_center.y + step;
		}
	}
	else if(ball_state == BM_DOWN_STRAIGHT)
	{
		y = old_center.y - step;
	}
	else if(ball_state == BM_UP_STRAIGHT)
	{
		y = old_center.y + step;
	}

	ball.setCenter(Point3d(x, y, z));
}

void breakTopBlocks(vector<Block> &top_blocks, Circle &ball, int &ball_state)
{
	int size = top_blocks.size();

	// check ball collide or not.
	// loop over all the top blocks and check:
	for(int i = (size - 1); i >=0; --i)
	{
		// 1. block is already broken or not
		if(top_blocks[i].IsBroken() == false)
		{
			// 2. check ball collide the top blocks element or not.
			// collide on bottom face of the block
			Block each = top_blocks[i];

			if(((ball.getCenter().y + ball.getRadius()) > each.getLeftBottomCorner().y) &&
				((each.getLeftBottomCorner().x <= ball.getCenter().x) &&
				(ball.getCenter().x <= each.getRightBottomCorner().x)))
			{
				top_blocks[i].Broken(true);
				ball_state = (ball_state == BM_UP_STRAIGHT) ?  BM_DOWN_STRAIGHT :
								(ball_state == BM_UP_LEFT) ? BM_DOWN_LEFT : BM_DOWN_RIGHT;
				break;
			}
			else
			{
				GLfloat diff = each.getLeftBottomCorner().x - (ball.getCenter().x + ball.getRadius());

				// collide on left face of the block
				if((diff >= -3 && diff <= -1) &&
					  ((each.getLeftTopCorner().y >= ball.getCenter().y) &&
					  (each.getLeftBottomCorner().y <= ball.getCenter().y)))
				{
				  top_blocks[i].Broken(true);
				  ball_state = (ball_state == BM_DOWN_RIGHT) ? BM_DOWN_LEFT : BM_UP_LEFT;
				  break;
				}

				// collide on right face of the block
				else if((diff >= 1 && diff <= 3) &&
					  ((each.getRightTopCorner().y >= ball.getCenter().y) &&
					  (each.getRightBottomCorner().y <= ball.getCenter().y)))
				{
				  top_blocks[i].Broken(true);
				  ball_state = (ball_state == BM_DOWN_LEFT) ? BM_DOWN_RIGHT : BM_UP_RIGHT;
				  break;
				}
			}
		}
	}
}

void glutBitmapString(void* font, char *string)
{
	int size = strlen(string);

	//glRasterPos3f(x, y, 0.0f);
	for(int i = 0; i < size; ++i)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
}
