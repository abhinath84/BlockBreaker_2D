/*
 * game.h
 *
 *  Created on: 15-Mar-2016
 *      Author: abhi
 */

#ifndef GAME_H
#define GAME_H

// c/cpp includes
#include <iostream>
#include <vector>
#include <math.h>
#include <ctime>

// opengl includes
#if defined(WIN32) || defined(WIN64)
  #include <freeglut.h>
#elif __LINUX
  #include <GL/glut.h>
#endif

// project specific includes
#include "point3d.h"
#include "block.h"
#include "circle.h"
#include "utility.h"

using namespace std;

/// preprocessors

// enums

// game states
enum GAME_STATE
{
  GS_UNSTARTED  = -1,
  GS_RUNNING    = 0,
  GS_PAUSED   = 1,
  GS_END      = 2,
  GS_WIN      = 3
};

// ball move status
enum BALL_MOVE
{
  BM_DOWN_LEFT    = 0,
  BM_DOWN_RIGHT   = 1,
  BM_UP_LEFT      = 2,
  BM_UP_RIGHT     = 3,
  BM_DOWN_STRAIGHT  = 4,
  BM_UP_STRAIGHT    = 5
};

/// windows structure
struct Window
{
  // Ctor & Dtor
  Window()
  {
    title   = "Default Title";
    width   = 800;
    height  = 600;
    pos_x   = 100;
    pos_y   = 50;
    left    = -100.0f;
    right   = 100.0f;
    bottom  = -100.0f;
    top     = 100.0f;
  }

  Window(const string &title_, int width_, int height_, int x, int y,
          const GLfloat &left_, const GLfloat &right_, const GLfloat &bottom_, const GLfloat &top_)
  {
    title.assign(title_);
    width   = width_;
    height  = height_;
    pos_x   = x;
    pos_y   = y;

    left    = left_;
    right   = right_;
    bottom  = bottom_;
    top     = top_;
  }

  /// member variables
  string  title;
  int     width;
  int     height;
  int     pos_x;
  int     pos_y;

  // Set the world coordinates.
  GLfloat left;
  GLfloat right;
  GLfloat bottom;
  GLfloat top;
};

struct GameConfig
{
  /// Ctor & Dtor
  GameConfig()
  {
    top_block_width   = 25.0f;
    top_block_height  = 10.0f;

    // bottom block
    bottom_block_width  = 40.0f;
    bottom_block_height = 10.f;

    // game specific
    ball_step_size      = 2.0f;
    ball_radius         = 5.0f;
    sleep               = 50;
  }

  GameConfig(const GLfloat &tb_w, const GLfloat &tb_h, const GLfloat &bb_w, const GLfloat &bb_h,
              const GLfloat &b_stepSize, const GLfloat &b_rad, int t)
  {
    top_block_width   = tb_w;
    top_block_height  = tb_h;

    // bottom block
    bottom_block_width  = bb_w;
    bottom_block_height = bb_h;

    // game specific
    ball_step_size      = b_stepSize;
    ball_radius         = b_rad;
    sleep               = t;
  }

  /// member variables
  // top block
  GLfloat   top_block_width;
  GLfloat   top_block_height;

  // bottom block
  GLfloat   bottom_block_width;
  GLfloat   bottom_block_height;

  // game specific
  GLfloat   ball_step_size;
  GLfloat   ball_radius;
  int       sleep;
};

class Game
{
public:
  Game();
  ~Game();

  /// Methode
  /// Game Core components
  void initGameCoreComponents();

  /// callback method.
  void render();
  void timer(int value);
  void resize(int w, int h);
  void specialKeys(int key, int x, int y);
  void keyboard(unsigned char key, int x, int y);
  void setupRC();

  /// game window
  Window  getWindow()  const;
  void    setWindow(const Window &w);
  void    setWindow(const string &title_, int width_, int height_, int x, int y,
                      const GLfloat &left_, const GLfloat &right_, const GLfloat &bottom_, const GLfloat &top_);
  void    initWindow(const string &title_, int width_, int height_, int x, int y,
                      const GLfloat &left_, const GLfloat &right_, const GLfloat &bottom_, const GLfloat &top_);
  void    createWindow();

  // game config
  GameConfig  getGameConfig()  const;
  void        setGameConfig(const GameConfig &gc);
  void        setGameConfig(const GLfloat &tb_w, const GLfloat &tb_h, const GLfloat &bb_w, const GLfloat &bb_h,
                              const GLfloat &b_stepSize, const GLfloat &b_rad, int t);

private:
  void initTopBlock();
  void initBottomBlock();
  void initBall();
  void setBlock(Block &block, const Point3d &lt, const Point3d &lb, const Point3d &rb, const Point3d &rt,
		  	  	  const Color3f &c);
  void moveBall();
  void breakTopBlocks();
  bool isBallBottomBlockCollide(int &pos);
  bool isGameOver();
  bool isGameWin();
  void drawGameOverBlock();
  void drawGameWinBlock();

private:
  // To stop copy of this class object.
  Game(const Game &other);
  Game& operator=(const Game &other);

  /// member variables
  /// core game components
  vector<Block>   top_blocks;
  Block           bottom_block;
  Circle          ball;
  int             ball_state;
  int             game_status;

  /// game window
  Window          window;

  // game config
  GameConfig      config;
};

#endif  // end of GAME_H
