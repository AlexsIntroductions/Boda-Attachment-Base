#include "finger.h"


/**
 * FUNCTION: initFinger
 * Initializes a Finger instance
 * -------------------------------------------
 * PARAMETERS:
 * f  : Pointer to finger
 * s1 : Pointer to servo located near the top of the finger
 * s2 : Pointer to servo located near the bottom of the finger
 * -------------------------------------------
 * RETURNS: 
 * Void
 */
void initFinger(Finger *f, Servo *s1, Servo *s2)
{
    f->top = s1;
    f->bot = s2;
}

/**
 * FUNCTION: extendWhole
 * Extends finger by calling Servo::open on both servos of the finger
 * -------------------------------------------
 * PARAMETERS:
 * f : Pointer to finger
 * -------------------------------------------
 * RETURNS: 
 * Void
 */
void extendWhole(Finger *f)
{
    open(f->top, f->top->speed);
    open(f->bot, f->bot->speed);
}

/**
 * FUNCTION: extendTop
 * Extends top of finger by calling Servo::open on servo labeled 'top'
 * -------------------------------------------
 * PARAMETERS:
 * f : Pointer to finger
 * -------------------------------------------
 * RETURNS: 
 * Void
 */
void extendTop(Finger *f)
{
    open(f->top, f->top->speed);
}

/**
 * FUNCTION: extendBottom
 * Extends bottom of finger by calling Servo::open on servo labeled 'bot'
 * -------------------------------------------
 * PARAMETERS:
 * f : Pointer to finger
 * -------------------------------------------
 * RETURNS: 
 * Void
 */
void extendBottom(Finger *f)
{
    open(f->bot, f->bot->speed);
}

/**
 * FUNCTION: retractWhole
 * Retracts finger by calling Servo::close on both servos of the finger
 * -------------------------------------------
 * PARAMETERS:
 * f : Pointer to finger
 * -------------------------------------------
 * RETURNS: 
 * Void
 */
void retractWhole(Finger *f)
{
    close(f->top, f->top->speed);
    close(f->bot, f->bot->speed);
}

/**
 * FUNCTION: retractTop
 * Retracts top of finger by calling Servo::close on servo labeled 'top'
 * -------------------------------------------
 * PARAMETERS:
 * f : Pointer to finger
 * -------------------------------------------
 * RETURNS: 
 * Void
 */
void retractTop(Finger *f)
{
    close(f->top, f->top->speed);
}

/**
 * FUNCTION: retractBottom
 * Retracts bottom of finger by calling Servo::close on servo labeled 'bot'
 * -------------------------------------------
 * PARAMETERS:
 * f : Pointer to finger
 * -------------------------------------------
 * RETURNS: 
 * Void
 */
void retractBottom(Finger *f)
{
    close(f->bot, f->bot->speed);
}