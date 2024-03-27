#include "console.h"
#include <ncurses.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>

#define BOARD_SIZE 10
#define MOVE_DELAY 15
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

using std::string;

int x = (BOARD_SIZE-2) / 2;
int y = (BOARD_SIZE-2) / 2;
string board[BOARD_SIZE-2][BOARD_SIZE-2] = {""};
int snake_body[(BOARD_SIZE-2)^2][2] = {0};

int score = 0;
int snake_length = 0;

// 사과 생성
void drawApple()
{
  int none_x[(BOARD_SIZE-2)^2] = {0}; 
  int none_y[(BOARD_SIZE-2)^2] = {0};

  int num = 0;
  for(int i = 1; i < BOARD_SIZE-1; i++)
  {
    for (int j = 1; i < BOARD_SIZE-1; j++)
    {
      if (board[i][j] == "")
      {
        none_x[num] = i;
        none_y[num] = j;
        num++;
      }
    }
  }

  int x_length, y_length;
  for(int i = 0; i < sizeof(none_x); i++)
  {
    if (none_x[i] == 0)
      x_length = i;
  }

  for (int i = 0; i < sizeof(none_y); i++)
  {
    if (none_y[i] == 0)
      y_length = i;
  }
  
  int x = std::rand() % (x_length);
  int y = std::rand() % (y_length);

  console::draw(none_x[x], none_y[y], APPLE_STRING);
  board[none_x[x]][none_y[y]] = "x";
}

// snake 생성 및 이동
void moving()
{
  if (console::key(console::K_LEFT)) 
  {
    x--;
    if (board[x][y] == "x")
    {
      score += 10;
      snake_length++;
      drawApple();
    }
  }
  if (console::key(console::K_RIGHT)) 
  {
    x++;
    if (board[x][y] == "x")
    {
      score += 10;
      snake_length++;
      drawApple();
    }
  }
  if (console::key(console::K_UP)) 
  {
    y--;
    if (board[x][y] == "x")
    {
      score += 10;
      snake_length++;
      drawApple();
    }
  }
  if (console::key(console::K_DOWN)) 
  {
    y++;
    if (board[x][y] == "x")
    {
      score += 10;
      snake_length++;
      drawApple();
    }
  }
}

// snake 생성 및 이동
void drawSnake()
{
  for(int i = snake_length; i > 0; i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }
  snake_body[0][0] = x;
  snake_body[0][1] = y;

  for(int i = 0; i < snake_length; i++)
  {
    if (i == 0)
      console::draw(x, y, SNAKE_STRING);
    else
      console::draw(snake_body[i][0], snake_body[i][1], SNAKE_BODY_STRING);
  }
}

// x, y 위치 제한
void restrictInScreen()
{
  if (x < 0)
    x = 0;
  if (x >= console::SCREEN_WIDTH)
    x = console::SCREEN_WIDTH - 1;
  if (y < 0)
    y = 0;
  if (y >= console::SCREEN_HEIGHT)
    y = console::SCREEN_HEIGHT - 1;
}

// 게임 벽 생성
void drawWall()
{
  console::draw(0, 0, WALL_LEFT_TOP_STRING);
  console::draw(0, BOARD_SIZE - 1, WALL_LEFT_BOTTOM_STRING);
  console::draw(BOARD_SIZE - 1, 0, WALL_RIGHT_TOP_STRING);
  console::draw(BOARD_SIZE - 1, BOARD_SIZE - 1, WALL_RIGHT_BOTTOM_STRING);

  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      if (i == 0 && 0 < j && j< BOARD_SIZE-1)
        console::draw(0, j, WALL_HORIZONTAL_STRING);
      else if (j == 0 && 0 < i && j < BOARD_SIZE-1)
        console::draw(i, j, WALL_VERTICAL_STRING);
      else if (i == BOARD_SIZE - 1 && 0 < j && j < BOARD_SIZE-1)
        console::draw(BOARD_SIZE - 1, j, WALL_HORIZONTAL_STRING);
      else if (j == BOARD_SIZE && 0 < i && i < BOARD_SIZE-1)
        console::draw(i, BOARD_SIZE-1, WALL_VERTICAL_STRING);
    }
  }
}



// Game over
bool gameOver()
{
  bool isFalse = true;
  
  return isFalse;
}

void printFalse()
{
  console::draw((BOARD_SIZE-2)/2, (BOARD_SIZE-2)/2, "You lose!");
}


// 게임시작
void gameStart()
{
  // 콘솔 라이브러리 초기화
  console::init();

  while(true)
  {
    console::clear();

    moving();
    restrictInScreen();
    drawWall();
    drawSnake();
    if(!gameOver())
    {
      printFalse();
      break;
    }

    // 화면 갱신 후 다음 프레임까지 대기
    console::wait();

  }
}

// 게임종료