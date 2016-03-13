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

// enums

// game states
enum GAME_STATE
{
	GS_UNSTARTED 	= -1,
	GS_RUNNING		= 0,
	GS_PAUSED		= 1,
	GS_END			= 2,
	GS_WIN			= 3
};

// ball move status
enum BALL_MOVE
{
	BM_DOWN_LEFT		= 0,
	BM_DOWN_RIGHT		= 1,
	BM_UP_LEFT			= 2,
	BM_UP_RIGHT			= 3,
	BM_DOWN_STRAIGHT	= 4,
	BM_UP_STRAIGHT		= 5
};

void glOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
void setBlock(Block &block, const Point3d &lt, const Point3d &lb, const Point3d &rb, const Point3d &rt,
		const Color3f &c);
void resizeCallback_low(GLfloat &win_left, GLfloat &win_right,
							GLfloat &win_bottom, GLfloat &win_top,
							int &w, int &h);
void toggleGlutWindowMaximizeBox(char *szWindowTitle);
void initTopBlock(vector<Block> &blocks, const GLfloat &win_left, const GLfloat &win_right,
					const GLfloat &win_top, const GLfloat &block_width, const GLfloat &block_height);
bool isBallBottomBlockCollide(const Circle &ball, const Block &bottom_block, int &pos);
void moveBall(Circle &ball, int &ball_state, const GLfloat &step,
		const GLfloat &win_left, const GLfloat &win_right, const GLfloat &win_top, const GLfloat &win_bottom);
bool isGameOver(const Circle &ball, const Block &bottom_block, const GLfloat &win_bottom);
bool isGameWin(const vector<Block> &top_blocks);
void drawGameOverBlock(const GLfloat &center_x, const GLfloat &center_y);
void drawGameWinBlock(const GLfloat &center_x, const GLfloat &center_y);
void glutBitmapString(void* font, char *string);
void breakTopBlocks(vector<Block> &top_blocks, Circle &ball, int &ball_state);

#endif /* UTILITY_H */
