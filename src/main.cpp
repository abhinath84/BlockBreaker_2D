#include <iostream>
#include <ctime>

// opengl related includes
#if defined(_WIN64) || defined(_WIN32)
  #include <freeglut.h>
#elif __linux
  #include <GL/glut.h>
#endif

#include "utility.h"
#include "game.h"


// top block dimension
#define TOP_BLOCK_WIDTH		25
#define TOP_BLOCK_HEIGHT	10

// bottom block dimension
#define BOTTOM_BLOCK_WIDTH		40
#define BOTTOM_BLOCK_HEIGHT		10

// forward declaration
void renderCallback();
void timerCallback(int value);
void resizeCallback(int w, int h);
void specialKeysCallback(int key, int x, int y);
void keyboardCallback(unsigned char key, int x, int y);


// main game object.
Game  game;


int main(int argc, char **argv)
{
  char win_title[]      = "Block Breaker Game !!!";
  int win_width         = 800;
  int win_height        = 600;

  int win_pos_x         = 100;
  int win_pos_y         = 50;

  // Set the world coordinates.
  GLfloat win_left      = -100.0;
  GLfloat win_right     = 100.0;
  GLfloat win_bottom    = -100.0;
  GLfloat win_top       = 100.0;

  GLfloat ballStepSize  = 2.0f;
  int timer             = 30;       // 50 milliseconds

	/// Initialize glut.
	glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

  /// set game config
  game.setGameConfig(TOP_BLOCK_WIDTH, TOP_BLOCK_HEIGHT, BOTTOM_BLOCK_WIDTH, BOTTOM_BLOCK_HEIGHT,
                        ballStepSize, 5.0f, timer);
	/// Create game window
  game.setWindow(win_title, win_width, win_height, win_pos_x, win_pos_y,
                  win_left, win_right, win_bottom, win_top);
  game.createWindow();

	// Set OpenGL program initial state.
	game.setupRC();

	// set callback functions
	glutDisplayFunc(renderCallback);
	glutReshapeFunc(resizeCallback);
	glutSpecialFunc(specialKeysCallback);
	glutKeyboardFunc(keyboardCallback);

	timerCallback(0);

	/// start glut mainloop
	glutMainLoop();

	return(0);
}


/// Callback functions

void renderCallback()
{
  game.render();
}

void timerCallback(int value)
{
  game.timer(value);

  // 50 milliseconds
  glutTimerFunc(game.getGameConfig().sleep, timerCallback, 0);
}

void resizeCallback(int w, int h)
{
  game.resize(w, h);
}

void specialKeysCallback(int key, int x, int y)
{
  game.specialKeys(key, x, y);
}

void keyboardCallback(unsigned char key, int x, int y)
{
  game.keyboard(key, x, y);
}
