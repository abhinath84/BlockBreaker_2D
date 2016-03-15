#include "game.h"


Game::Game()
  :top_blocks(),
  bottom_block(),
  ball(),
  ball_state(BM_UP_RIGHT),
  game_status(GS_UNSTARTED),
  window(),
  config()
{
}

Game::Game(const Game &other)
  :bottom_block(other.bottom_block),
  ball(other.ball),
  ball_state(other.ball_state),
  game_status(other.game_status),
  window(other.window),
  config(other.config)
{
  for(int i = 0; i < (int)other.top_blocks.size(); ++i)
    top_blocks[i] = other.top_blocks[i];
}

Game& Game::operator=(const Game &other)
{
  if(this != &other)
  {
    for(int i = 0; i < (int)other.top_blocks.size(); ++i)
      top_blocks[i] = other.top_blocks[i];

    bottom_block = other.bottom_block;
    ball = other.ball;
    ball_state = other.ball_state;
    game_status = other.game_status;
    window = other.window;
    config = other.config;
  }

  return(*this);
}

Game::~Game()
{
}

void Game::initGameCoreComponents()
{
  // initialize top blocks.
  initTopBlock();

  // initialize bottom block.
  initBottomBlock();

  // initialize ball.
  initBall();
}

void Game::initTopBlock()
{
  int       row_num	= 4;
  GLfloat   width   = config.top_block_width;
  GLfloat   height  = config.top_block_height;

  /// reset this->top_blocks
  this->top_blocks.clear();

  /// start with top left corner.
  for(int i = 0; i < row_num; ++i)
  {
    int block_num = (int)((window.right - window.left) / width);
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
      Point3d lt = Point3d((window.left + (width*j)), (window.top - (height*i)), 0.0f);
      Point3d lb = Point3d((window.left + (width*j)), (window.top - (height*(i+1))), 0.0f);
      Point3d rb = Point3d((window.left + (width*(j+1))), (window.top - (height*(i+1))), 0.0f);
      Point3d rt = Point3d((window.left + (width*(j+1))), (window.top - (height*i)), 0.0f);

      this->top_blocks.push_back(Block(lt, lb, rb, rt, c));
    }
  }
}

void Game::initBottomBlock()
{
  GLfloat center_x  = (window.right + window.left)/2;
  GLfloat width     = config.bottom_block_width;
  GLfloat height    = config.bottom_block_height;

  GLfloat b_base_y	= window.bottom + 10.0f;
  Point3d b_lt		= Point3d((center_x - (width/2)), (b_base_y + height), 0.0f);
  Point3d b_lb 		= Point3d((center_x - ((width/2))), (b_base_y), 0.0f);
  Point3d b_rb 		= Point3d((center_x + ((width/2))), (b_base_y), 0.0f);
  Point3d b_rt 		= Point3d((center_x + ((width/2))), (b_base_y + height), 0.0f);
  Color3f b_c 		= Color3f(0.0f, 1.0f, 0.0f);

  this->bottom_block.setCorners(b_lt, b_lb, b_rb, b_rt);
  this->bottom_block.setColor(b_c);
}

void Game::initBall()
{
  Point3d b_lt = bottom_block.getLeftTopCorner();
  Point3d b_rt = bottom_block.getRightTopCorner();
  GLfloat ball_x = (b_lt.x + b_rt.x)/2;
  GLfloat ball_r = config.ball_radius;

  ball.setCenter(Point3d(ball_x, (b_lt.y + ball_r), 0.0f));
  ball.setRadius(ball_r);
  ball.setColor(Color3f(0.5f, 0.3f, 0.3f));
}

void Game::setBlock(Block &block, const Point3d &lt, const Point3d &lb, const Point3d &rb, const Point3d &rt,
		const Color3f &c)
{
	block.setCorners(lt, lb, rb, rt);
	block.setColor(c);
}

void Game::moveBall()
{
  Point3d old_center = ball.getCenter();

  GLfloat x = old_center.x;
  GLfloat y = old_center.y;
  GLfloat z = 0.0f;

  if(ball_state == BM_DOWN_LEFT)
  {
    // check collide with left wall
    if((old_center.x - ball.getRadius() <= window.left))
    {
      // if collide then move ball to BM_DOWN_RIGHT
      ball_state = BM_DOWN_RIGHT;
    }
    else
    {
      x = old_center.x - config.ball_step_size;
      y = old_center.y - config.ball_step_size;
    }
  }
  else if(ball_state == BM_DOWN_RIGHT)
  {
    // check collide with right wall
    if((old_center.x + ball.getRadius() >= window.right))
    {
      // if collide then move ball to BM_DOWN_LEFT
      ball_state = BM_DOWN_LEFT;
    }
    else
    {
      x = old_center.x + config.ball_step_size;
      y = old_center.y - config.ball_step_size;
    }
  }
  else if(ball_state == BM_UP_LEFT)
  {
    // check collide with left wall
    // if collide then move ball to BM_UP_RIGHT
    if((old_center.x - ball.getRadius() <= window.left))
    {
      ball_state = BM_UP_RIGHT;
    }
    // check collide with top wall
    // if collide then move ball to BM_DOWN_LEFT
    else if((old_center.y + ball.getRadius() >= window.top))
    {
      ball_state = BM_DOWN_LEFT;
    }
    else
    {
      x = old_center.x - config.ball_step_size;
      y = old_center.y + config.ball_step_size;
    }
  }
  else if(ball_state == BM_UP_RIGHT)
  {
    // check collide with right wall
    if((old_center.x + ball.getRadius() >= window.right))
    {
      // if collide then move ball to BM_UP_LEFT
      ball_state = BM_UP_LEFT;
    }
    // check collide with top wall
    else if((old_center.y + ball.getRadius() >= window.top))
    {
      // if collide then move ball to BM_DOWN_RIGHT
      ball_state = BM_DOWN_RIGHT;
    }
    else
    {
      x = old_center.x + config.ball_step_size;
      y = old_center.y + config.ball_step_size;
    }
  }
  else if(ball_state == BM_DOWN_STRAIGHT)
  {
    y = old_center.y - config.ball_step_size;
  }
  else if(ball_state == BM_UP_STRAIGHT)
  {
    y = old_center.y + config.ball_step_size;
  }

  ball.setCenter(Point3d(x, y, z));
}

void Game::breakTopBlocks()
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

// pos  = 1 -> left
//    = 2 -> center
//    = 3 -> right
bool Game::isBallBottomBlockCollide(int &pos)
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

bool Game::isGameOver()
{
  bool status = false;

  if((ball.getCenter().y + ball.getRadius()) <= window.bottom)
  {
    int collision_pos = 0;
    if(!isBallBottomBlockCollide(collision_pos))
      status = true;
  }

  return(status);
}

bool Game::isGameWin()
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

void Game::drawGameOverBlock()
{
  GLfloat center_x = (window.right + window.left)/2;
  GLfloat center_y = (window.top + window.bottom)/2;

  // draw a red colored rectangle based on the center of the game area.
  /// set the pencil
  glColor3f(0.75f, 0.0f, 0.0f); // green
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

void Game::drawGameWinBlock()
{
  GLfloat center_x = (window.right + window.left)/2;
  GLfloat center_y = (window.top + window.bottom)/2;

  // draw a red colored rectangle based on the center of the game area.
  /// set the pencil
  glColor3f(0.0f, 0.75f, 0.0f); // green
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

Window Game::getWindow()  const
{
  return(this->window);
}

void Game::setWindow(const Window &w)
{
  this->window = w;
}

void Game::setWindow(const string &title_, int width_, int height_, int x, int y,
                      const GLfloat &left_, const GLfloat &right_, const GLfloat &bottom_, const GLfloat &top_)
{
  this->window = Window(title_, width_, height_, x, y, left_, right_, bottom_, top_);
}

void Game::initWindow(const string &title_, int width_, int height_, int x, int y,
                      const GLfloat &left_, const GLfloat &right_, const GLfloat &bottom_, const GLfloat &top_)
{
  setWindow(title_, width_, height_, x, y, left_, right_, bottom_, top_);
}

void Game::createWindow()
{
  /// Create window
  glutInitWindowPosition(window.pos_x, window.pos_y);
  glutInitWindowSize(window.width, window.height);
  glutCreateWindow(window.title.c_str());
}

GameConfig Game::getGameConfig()  const
{
  return(this->config);
}

void Game::setGameConfig(const GameConfig &gc)
{
  this->config = gc;
}

void Game::setGameConfig(const GLfloat &tb_w, const GLfloat &tb_h, const GLfloat &bb_w, const GLfloat &bb_h,
                              const GLfloat &b_stepSize, const GLfloat &b_rad, int t)
{
  this->config = GameConfig(tb_w, tb_h, bb_w, bb_h, b_stepSize, b_rad, t);
}

void Game::render()
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
		drawGameOverBlock();

	if(game_status == GS_WIN)
			drawGameWinBlock();

	/// swap background buffer.
	glutSwapBuffers();
}

void Game::resize(int w, int h)
{
  window.width = w;
  window.height = h;

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
    window.left = -100.0;
    window.right = 100.0;
    window.bottom = -100.0f / aspectRatio;
    window.top = 100.0f / aspectRatio;
  }
  else
  {
    window.left = -100.0f * aspectRatio;
    window.right = 100.0f * aspectRatio;
    window.bottom = -100.0;
    window.top = 100.0;
  }

  //glOrtho (window.left, window.right, window.bottom, window.top, 1.0, -1.0);
  glOrtho2D(window.left, window.right, window.bottom, window.top);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  initTopBlock();
}

void Game::specialKeys(int key, int x, int y)
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

		// Collision detection for bottom block with left wall
		if(window.left >= lt.x)
		{
			lt.x = window.left;
			lb.x = window.left;
			rb.x = window.left + config.bottom_block_width;
			rt.x = window.left + config.bottom_block_width;
		}

		isProperKey = true;
	}

	if(key == GLUT_KEY_RIGHT)
	{
		lt.x = lt.x + stepSize;
		lb.x = lb.x + stepSize;
		rb.x = rb.x + stepSize;
		rt.x = rt.x + stepSize;

		// Collision detection for bottom block with right wall
		if(rt.x >= window.right)
		{
			lt.x = window.right - config.bottom_block_width;
			lb.x = window.right - config.bottom_block_width;
			rb.x = window.right;
			rt.x = window.right;
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

			// check collision with ball.
			// ball will collide with bottom block if
			// 1. ball.center.y + radius <= bottom block.lt.y
			// 2. bottom block.lt.x < ball.center.x < bottom block.rt.x
			int collision_pos = 0;
			if(isBallBottomBlockCollide(collision_pos))
			{
				// set ball position and ball move state according to the collision coordinate.
				ball_state = ((collision_pos == 1) ? BM_UP_LEFT :
								(collision_pos == 2) ? BM_UP_STRAIGHT : BM_UP_RIGHT);
			}
		}
	}

	glutPostRedisplay();
}

void Game::keyboard(unsigned char key, int x, int y)
{
  // enter
  if(key == 13)
    game_status = GS_RUNNING;

  // space bar
  if(key == 32)
    game_status = GS_PAUSED;

  glutPostRedisplay();
}

void Game::timer(int value)
{
  if(game_status == GS_RUNNING)
  {
    moveBall();

    // check collision with ball.
    // ball will collide with bottom block if
    // 1. ball.center.y + radius <= bottom block.lt.y
    // 2. bottom block.lt.x < ball.center.x < bottom block.rt.x

    int collision_pos = 0;
    if(isBallBottomBlockCollide(collision_pos))
    {
      // set ball position and ball move state according to the collision coordinate.
      ball_state = ((collision_pos == 1) ? BM_UP_LEFT :
              (collision_pos == 2) ? BM_UP_STRAIGHT : BM_UP_RIGHT);
    }

    // game over
    if(isGameOver())
      game_status = GS_END;

    // win the game
    if(isGameWin())
      game_status = GS_WIN;

    /// break top block(s) if it's collide with ball
    /// and also change the ball's movement.
    breakTopBlocks();

    glutPostRedisplay();
  }
}

void Game::setupRC()
{
  initGameCoreComponents();

  glOrtho2D(window.left, window.right, window.bottom, window.top);
}
