#include <iostream>
#include <vector>
#include <math.h>
#include <ctime>

// opengl related includes
#if defined(_WIN64) || defined(_WIN32)
  #include <freeglut.h>
#elif __linux
  #include <GL/glut.h>
#endif

#include "point3d.h"
#include "block.h"
#include "circle.h"
#include "utility.h"

using namespace std;

// Global variableS

// Window related
const char *win_title = "Block Breaker Game !!!";

static int win_width = 800;
static int win_height = 600;

static int win_pos_x = 100;
static int win_pos_y = 50;

// Set the world coordinates.
static GLfloat win_left 	= -100.0;
static GLfloat win_right 	= 100.0;
static GLfloat win_bottom 	= -100.0;
static GLfloat win_top 		= 100.0;


// Block related

// top block dimension
#define TOP_BLOCK_WIDTH		25
#define TOP_BLOCK_HEIGHT	10

// bottom block dimension
#define BOTTOM_BLOCK_WIDTH		40
#define BOTTOM_BLOCK_HEIGHT		10

static GLfloat center_x 	= 0.0;
//static GLfloat center_y 	= 0.0;

// Define game objects and related stuff
vector<Block> 	top_blocks;
Block			bottom_block;
Circle			ball;

static GLfloat ballStepSize = 2.0f;
static int ball_move 		= BM_UP_RIGHT;
static int game_status		= GS_UNSTARTED;
static int timer			= 30;				// 50 milliseconds


/// Methods

void initElements()
{
	// initialize top blocks.
	initTopBlock(top_blocks, win_left, win_right, win_top, TOP_BLOCK_WIDTH, TOP_BLOCK_HEIGHT);

	// initialize bottom block.
	GLfloat b_base_y	= win_bottom + 10.0f;
	Point3d b_lt		= Point3d((center_x - (BOTTOM_BLOCK_WIDTH/2)), (b_base_y + BOTTOM_BLOCK_HEIGHT), 0.0f);
	Point3d b_lb 		= Point3d((center_x - ((BOTTOM_BLOCK_WIDTH/2))), (b_base_y), 0.0f);
	Point3d b_rb 		= Point3d((center_x + ((BOTTOM_BLOCK_WIDTH/2))), (b_base_y), 0.0f);
	Point3d b_rt 		= Point3d((center_x + ((BOTTOM_BLOCK_WIDTH/2))), (b_base_y + BOTTOM_BLOCK_HEIGHT), 0.0f);
	Color3f b_c 		= Color3f(0.0f, 1.0f, 0.0f);

	setBlock(bottom_block, b_lt, b_lb, b_rb, b_rt, b_c);

	// initialize ball.
	GLfloat ball_x = (b_lt.x + b_rt.x)/2;
	ball.setCenter(Point3d(ball_x, (b_lt.y + 5.0f), 0.0f));
	ball.setRadius(5.0f);
	ball.setColor(Color3f(0.5f, 0.3f, 0.3f));
}

/// Helper functions

/// Do all the setup staff.
void setupRC()
{
	initElements();

	glOrtho2D(win_left, win_right, win_bottom, win_top);
}

/// Rendering callback.
void renderCallback()
{
	//  Set the frame buffer clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// black background

	//  Clear the window or more specifically the frame buffer...
	//  This happens by replacing all the contents of the frame
	//  buffer by the clear color
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//  Set shading model
	glShadeModel(GL_FLAT);

	/// draw top blocks.
	for(int i = 0; i < (int)top_blocks.size(); ++i)
		top_blocks[i].draw();

	/// draw ball
	ball.draw();

	/// draw Bottom block
	bottom_block.draw();

	if(game_status == GS_END)
		drawGameOverBlock(0.0f, 0.0f);

	if(game_status == GS_WIN)
			drawGameWinBlock(0.0f, 0.0f);

	/// swap background buffer.
	glutSwapBuffers();

	/*if(game_status == GS_RUNNING)
	{
		moveBall(ball, ball_move, ballStepSize, win_left, win_right, win_top, win_bottom);

		// check collision with ball.
		// ball will collide with bottom block if
		// 1. ball.center.y + radius <= bottom block.lt.y
		// 2. bottom block.lt.x < ball.center.x < bottom block.rt.x
		if(isBallBottomBlockCollide(ball, bottom_block))
		{
			// set ball position and ball move state according to the collision coordinate.
			ball_move = ((ball_move == BM_DOWN_LEFT) ? BM_UP_LEFT : BM_UP_RIGHT);
		}

		glutPostRedisplay(); // Redraw
	}*/
}

static void timerCallback(int value)
{
	if(game_status == GS_RUNNING)
	{
		moveBall(ball, ball_move, ballStepSize, win_left, win_right, win_top, win_bottom);

		// check collision with ball.
		// ball will collide with bottom block if
		// 1. ball.center.y + radius <= bottom block.lt.y
		// 2. bottom block.lt.x < ball.center.x < bottom block.rt.x
		if(isBallBottomBlockCollide(ball, bottom_block))
		{
			// set ball position and ball move state according to the collision coordinate.
			ball_move = ((ball_move == BM_DOWN_LEFT) ? BM_UP_LEFT : BM_UP_RIGHT);
		}

		// game over
		if(isGameOver(ball, bottom_block, win_bottom))
			game_status = GS_END;

		// win the game
		if(isGameWin(top_blocks))
			game_status = GS_WIN;

		/// break top block(s) if it's collide with ball
		/// and also change the ball's movement.
		breakTopBlocks(top_blocks, ball, ball_move);

		glutPostRedisplay();
	}

    // 50 milliseconds
    glutTimerFunc(timer, timerCallback, 0);
}

void resizeCallback(int w, int h)
{
	win_width = w;
	win_height = h;

	resizeCallback_low(win_left, win_right, win_bottom, win_top, w, h);
	initTopBlock(top_blocks, win_left, win_right, win_top, TOP_BLOCK_WIDTH, TOP_BLOCK_HEIGHT);
	//glutReshapeWindow(800, 600);
}

void specialKeysCallback(int key, int x, int y)
{
	bool isProperKey = false;
	GLfloat stepSize = 5.0f;

	Point3d lt = bottom_block.getLeftTopCorner();
	Point3d lb = bottom_block.getLeftBottomCorner();
	Point3d rb = bottom_block.getRightBottomCorner();
	Point3d rt = bottom_block.getRightTopCorner();
	Color3f c = bottom_block.getColor();

	if(key == GLUT_KEY_LEFT)
	{
		lt.x = lt.x - stepSize;
		lb.x = lb.x - stepSize;
		rb.x = rb.x - stepSize;
		rt.x = rt.x - stepSize;

		// Collision detection
		if(win_left >= lt.x)
		{
			lt.x = win_left;
			lb.x = win_left;
			rb.x = win_left + BOTTOM_BLOCK_WIDTH;
			rt.x = win_left + BOTTOM_BLOCK_WIDTH;
		}

		isProperKey = true;
	}

	if(key == GLUT_KEY_RIGHT)
	{
		lt.x = lt.x + stepSize;
		lb.x = lb.x + stepSize;
		rb.x = rb.x + stepSize;
		rt.x = rt.x + stepSize;

		// Collision detection
		if(rt.x >= win_right)
		{
			lt.x = win_right - BOTTOM_BLOCK_WIDTH;
			lb.x = win_right - BOTTOM_BLOCK_WIDTH;
			rb.x = win_right;
			rt.x = win_right;
		}

		isProperKey = true;
	}

	if(isProperKey)
	{
		// reset bottom_block with new values (if any)
		setBlock(bottom_block, lt, lb, rb, rt, c);

		// move ball accordingly if game is not started.
		if(game_status == GS_UNSTARTED)
		{
			ball.setCenter(Point3d(((lt.x + rt.x)/2), ball.getCenter().y, ball.getCenter().z));
		}

		if(game_status == GS_RUNNING)
		{
			// how to decide ball is going up or down.
			// set a flag which control this and calculate ball position according to that.
			// check ball is collide with boundary wall or not.

			moveBall(ball, ball_move, ballStepSize, win_left, win_right, win_top, win_bottom);

			// check collision with ball.
			// ball will collide with bottom block if
			// 1. ball.center.y + radius <= bottom block.lt.y
			// 2. bottom block.lt.x < ball.center.x < bottom block.rt.x
			if(isBallBottomBlockCollide(ball, bottom_block))
			{
				// set ball position and ball move state according to the collision coordinate.
				ball_move = ((ball_move == BM_DOWN_LEFT) ? BM_UP_LEFT : BM_UP_RIGHT);
			}
		}
	}

	glutPostRedisplay();
}

void keyboardCallback(unsigned char key, int x, int y)
{
	// enter
	if(key == 13)
		game_status = GS_RUNNING;

	// space bar
	if(key == 32)
		game_status = GS_PAUSED;

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	/// Initialize glut.
	glutInit(&argc, argv);

	/// Create window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowPosition(win_pos_x, win_pos_y);
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow(win_title);

	// Set OpenGL program initial state.
	setupRC();

	glutDisplayFunc(renderCallback);
	glutReshapeFunc(resizeCallback);
	glutSpecialFunc(specialKeysCallback);
	glutKeyboardFunc(keyboardCallback);

	timerCallback(0);

	/// start gl mainloop
	glutMainLoop();

	return(0);
}
