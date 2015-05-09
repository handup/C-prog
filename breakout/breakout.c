//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// list of colors

// prototype
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    //speed of the ball
    double xvelocity = 0.0;
    double yvelocity = drand48();
    GEvent event;
    waitForClick();
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        
        move(ball, xvelocity/20, yvelocity/20);
        
        if (getX(ball) + getWidth(ball) >= getWidth(window)||getX(ball)<0)
        {
            xvelocity = -xvelocity;
            
        }
        if(getY(ball)<0)
        {
           yvelocity = -yvelocity; 
        }
        
        if(getY(ball) + getHeight(ball) >= getHeight(window))
        {
            lives--;
            removeGWindow(window, ball);
            waitForClick();
            ball = initBall(window);
        }
        
        // check for mouse event
        event = getNextEvent(MOUSE_EVENT);
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure paddle follows top cursor
                double xcursor = getX(event) - getWidth(paddle) / 2;
                setLocation(paddle, xcursor, getY(paddle));
            }
        }
        if(detectCollision(window, ball)!=NULL)
        {GObject object = detectCollision(window, ball);
        if (strcmp(getType(object), "GRect") == 0)
        {   
            if(object==paddle)
            {
            xvelocity =  drand48()*2-1;
            yvelocity =  -drand48();
            }
            else
            {removeGWindow(window, object);
            bricks--;
            points++;
            xvelocity =  drand48()*2-1;
            yvelocity =  drand48();
            
            
            updateScoreboard(window, label , points);
            }
        }
        }
        
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    char str[] = "BLUE,RED,YELLOW,ORANGE,GREEN,VIOLETtokenize(str);";
    char* color = strtok(str, ",");
    GRect brick;
    int x=2;
    int z=10;
    int i;
    for(int j=1; j<=ROWS; j++)
       {x=2;
        for(i=1; i<=COLS; i++)
        {
        brick = newGRect(x, z, 35, 10);
        setColor(brick, color);
        setFilled(brick, true);
        add(window, brick);
        x+=40;
        }
        z+=12;
        color = strtok(NULL, ",");
        }
}  


/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
        GOval ball = newGOval(190,290,20,20);
        setColor(ball, "BLACK");
        setFilled(ball, true);
        add(window, ball);
        return ball;
    return NULL;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
        GRect paddle = newGRect(170,560,40,10);
        setColor(paddle, "BLACK");
        setFilled(paddle, true);
        add(window, paddle);
        return paddle;
    return NULL;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
      GLabel label = newGLabel("");
      setFont(label, "SansSerif-36");
      add(window, label);
      updateScoreboard(window, label, 0);
      return label;
    return NULL;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
