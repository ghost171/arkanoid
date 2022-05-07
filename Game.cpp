#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <stdio.h>

using namespace std;

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, VK_RETURN)
//
//  schedule_quit_game() - quit game after act()


char labirint[23][32];

class Paddle {
 
  public:
    Paddle();
    ~Paddle();
    void move();
    void setDx(int);
 
    int dx;
    static const int INITIAL_X = 20 * 32;
    static const int INITIAL_Y = 7 * 32;
    int x = INITIAL_X;
    int y = INITIAL_Y;
};

Paddle::Paddle() {
  dx = 0;
}

Paddle::~Paddle() {}

void Paddle::setDx(int x) {
    dx = x;
}

void Paddle::move() {
    x = x + dx * 32;
    y = y + 32;
}

Paddle paddle;

class Brick {
 
  public:
    Brick(int, int);
    ~Brick();
 
  public:
    bool isDestroyed();
    void setDestroyed(bool);
 
  private:
    bool destroyed;
};

Brick::Brick(int x, int y) {
  destroyed = false;
}
 
Brick::~Brick() {}
 
void Brick::setDestroyed(bool destr) {
  destroyed = destr;
}

bool Brick::isDestroyed() {
    return destroyed;
}

class Ball {
 
  public:
    Ball();
    ~Ball();
 
  public:
    //void resetState();
    void autoMove();
    //void setXDir(int);
    //void setYDir(int);
    //int getXDir();
    //int getYDir();

    int xdir = 0.5;
    int ydir = -0.5;
    static const int INITIAL_X = 15 * 32;
    static const int INITIAL_Y = 10 * 32;    
    int x = INITIAL_X;
    int y = INITIAL_Y;
};

Ball::Ball() {}
Ball::~Ball() {}

void Ball::autoMove() {
 
    if (y - 32 <= 0) {
        xdir = 1;
    }
    
    if (y+ 32 >= SCREEN_WIDTH) {
        xdir = -1;
    }
    
    if (x - 32 == 0) {
        ydir = 1;
    }
    
    if (y - 32 == paddle.y) {
        xdir = 1;
    }

    if (y + 32 == paddle.y) {
        xdir = -1;
    }

    if (x - 32 == paddle.x) {
        ydir = 1;
    }

    x = x + 32 * xdir;
    y = y + 32 * ydir;
}



// initialize game data in this function
void initialize()
{

}


Ball ball;

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    if (is_key_pressed(VK_ESCAPE))
        schedule_quit_game();

    if (is_key_pressed(VK_RIGHT)){
        paddle.dx=0.1;
        paddle.move();
    }

    
    if (is_key_pressed(VK_LEFT)){
        paddle.dx=-0.1;
        paddle.move();
    }

    ball.autoMove();

    
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{

    for (int i = 0; i < 23; i++) {
        for (int j = 0; j < 32; j++) {
            if (i < 8) {
                labirint[i][j] = '#';
            } else if (i == 20 && (j == 15 || j == 14 || j == 16)) {
                labirint[i][j] = '=';
            } else if (i == 19 && j == 15) {
                labirint[i][j] = '@';
            }
            

        }
    }

  // clear backbuffer
    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

    for (int i = 0; i < 23; i++) {
        for (int j = 0; j < 32; j++) {
            if (labirint[i][j] == ' ') {
                char red = 0;
                char green = 0;
                char blue = 0;
                unsigned int current_pixel = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
                for(int y = 0; y <= 32; ++y)
                {
                    for(int x = 0; x <= 32; ++x)
                    {
                        if (i * 32 + y <= SCREEN_HEIGHT && j * 32 + x <= SCREEN_WIDTH) {
                            buffer[i * 32 + y][j * 32 + x] = current_pixel; 
                        }
                    }
                }
            } else if (labirint[i][j] == '#') {
                char red = 200;
                char green = 0;
                char blue = 200;
                unsigned int current_pixel = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
                for(int y = 0; y <= 28; ++y)
                {
                    for(int x = 0; x <= 28; ++x)
                    {   
                        if (i * 32 + y <= SCREEN_HEIGHT && j * 32 + x <= SCREEN_WIDTH) {
                            buffer[i * 32 + y][j * 32 + x] = current_pixel; 
                        }
                    }
                
                }
            }
        }
    }

    char red = 20;
    char green = 100;
    char blue = 100;
    unsigned int current_pixel = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
    for(int y = 0; y <= 32; ++y)
    {
        for(int x = 0; x <= 32; ++x)
        {
            if (paddle.x + x <= SCREEN_HEIGHT && paddle.y + y <= SCREEN_WIDTH) {
                buffer[paddle.x +  x][paddle.y + y] = current_pixel; 
                buffer[paddle.x + x][paddle.y + y - 32] = current_pixel; 
                buffer[paddle.x + x][paddle.y + y + 32] = current_pixel;

            } else {
                int x_now = (paddle.x + x) % SCREEN_HEIGHT;
                int y_now = (paddle.y + y) % SCREEN_WIDTH;

                buffer[paddle.x +  x][paddle.y + y] = current_pixel; 
                buffer[paddle.x + x][paddle.y + y - 32] = current_pixel; 
                buffer[paddle.x + x][paddle.y + y + 32] = current_pixel;
            }
    
        }
    }

    for(int y = 0; y <= 32; ++y)
    {
        for(int x = 0; x <= 32; ++x)
        {
            char red = 100;
            char green = 0;
            char blue = 0;
            unsigned int current_pixel = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
            for(int y = 0; y <= 32; ++y)
            {
                for(int x = 0; x <= 32; ++x)
                {
                    if (ball.x + x <= SCREEN_HEIGHT && ball.y + y <= SCREEN_WIDTH) {
                        buffer[ball.x +  x][ball.y + y] = current_pixel; 
                    }
                }
            }
        }
    }
}

// free game data in this function
void finalize()
{
}

