#include "servo.h"

bool servoDebug = 0;


/**
 * FUNCTION:   initServo
 * Initializes a Servo instance to map pin to a pwm irq to mov a servo
 * -------------------------------------------
 * PARAMETERS:
 * s        : pointer to instance of servo struct
 * pin      : number of the GPIO pin the servo's pwm input will connect to
 * *inPTR   : Pointer to external variable that contains input data
 * clkDiv   : value of the clock divider for pwm
 * pwmLevel : Initial value to set servo pwm to
 * dir      : Direction of servo motion:  0 - clockwise : 1 - counter clockwise
 * -------------------------------------------
 * RETURNS: void
 */
void initServo(Servo *s, uint8_t pin, uint16_t *inPtr, float clkDiv, uint16_t pwmLevel, uint8_t mode, bool dir)
{
    s->pinNum = pin;
    s->pwm = pwmLevel;
    s->input = inPtr;
    s->dir = dir;
    s->mode = mode;
    s->speed = mode;
    s->limit_max = MAX16;
    s->limit_min = MIN16;
    gpio_set_function(pin, GPIO_FUNC_PWM);
    s->sliceNum = pwm_gpio_to_slice_num(pin);
    pwm_clear_irq(s->sliceNum);
    pwm_set_irq_enabled(s->sliceNum, true);
    pwm_set_irq_mask_enabled(s->sliceNum, true);

    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, clkDiv);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(s->sliceNum, &config, true);
    updatePWM(s);
}

/**
 * FUNCTION: initServoMapping
 * Initializes values for the mapping of the servo's input to the
 * pwm's output for servo motion
 * -------------------------------------------
 * PARAMETERS:
 * s   : Pointer to servo
 * max : Max value of the input for mapping
 * min : Min value of the input for mapping
 * -------------------------------------------
 * RETURNS: Void
 */
void initServoMapping(Servo *s, uint16_t max, uint16_t min)
{
    if (max < min)
    {
        return;
    }
    s->max = max;
    s->min = min;
    s->slope = (MAX16 - MIN16) / (max - min);
}

/**
 * FUNCTION: setServoLimits
 * Self set a max and min limit for servo's position according to the PWM output signal
 * The automatic MAX value is MAX16 (maximum value of 16 bit integer : 65535)
 * The automatic MIN value is MIN16 (0)
 * -------------------------------------------
 * PARAMETERS:
 * s   : Pointer to servo
 * max : Max limit
 * min : Min limit
 * -------------------------------------------
 * RETURNS: Void
 */
void setServoLimits(Servo *s, uint16_t max, uint16_t min)
{
    s->limit_max = max;
    s->limit_min = min;
}

/**
 * FUNCTION: calcAngle
 * Returns the angle calculated from the current PWM
 * state of the servo
 * -------------------------------------------
 * PARAMETERS:
 * s : Pointer to servo to move
 * -------------------------------------------
 * RETURNS: uint8_t containing an integer between 0 and 180
 */
uint8_t calcAngle(Servo *s)
{
    // use current pwm value, MAX16, MIN16 to map the current pwm value to a value between 0 and 180
    return 0;
}

/**
 * FUNCTION: incrementPWM
 * Called to increase the PWM value of a servo by a set amount
 * and performs limit checking so servos dont go over limits
 * -------------------------------------------
 * PARAMETERS:
 * s   : Pointer to Servo struct instance
 * val : value to increment servo pwm by
 * -------------------------------------------
 * RETURNS: Void
 */
void incrementPWM(Servo *s, uint16_t val)
{
    if ((uint32_t)(s->pwm + val) >= (uint32_t)s->limit_max)
    {
        s->pwm = s->limit_max;
        // printf("NEW PWM: %d\n", s->pwm);
        return;
    }
    s->pwm += val;
}

/**
 * FUNCTION: decrementPWM
 * Called to increase the PWM value of a servo by a set amount
 * and performs limit checking so servos dont go over limits
 * -------------------------------------------
 * PARAMETERS:
 * s   : Pointer to Servo struct instance
 * val : value to increment servo pwm by
 * -------------------------------------------
 * RETURNS: Void
 */
void decrementPWM(Servo *s, uint16_t val)
{
    if ((uint32_t)(s->pwm - val) <= (uint32_t)s->limit_min)
    {
        s->pwm = s->limit_min;
        // printf("NEW PWM: %d\n", s->pwm);
        return;
    }
    s->pwm -= val;
}

/**
 * FUNCTION: open
 * When called, will modify the servo's pwm value by
 * the specified amount based on its dir.
 * NOTE: a servo being called to "open" is relative
 *       to the position the servo's head is facing and its
 *       available area to move.
 * -------------------------------------------
 * PARAMETERS:
 * s   : Pointer to servo
 * val : value to move servo by
 * -------------------------------------------
 * RETURNS: Void
 */
void open(Servo *s, uint16_t val)
{
    if (servoDebug)
    {
        printf("OPENING SERVO\n");
    }

    if (s->dir)
    {
        incrementPWM(s, val);
        return;
    }
    decrementPWM(s, val);
}

/**
 * FUNCTION: close
 * When called, will modify the servo's pwm value by
 * the specified amount based on its dir.
 * NOTE: a servo being called to "close" is relative
 *       to the position the servo's head is facing and its
 *       available area to move.
 * -------------------------------------------
 * PARAMETERS:
 * s   : Pointer to servo
 * val : value to move servo by
 * -------------------------------------------
 * RETURNS: Void
 */
void close(Servo *s, uint16_t val)
{
    if (servoDebug)
    {
        printf("CLOSING SERVO\n");
    }

    if (s->dir)
    {
        decrementPWM(s, val);
        return;
    }
    incrementPWM(s, val);
}

/**
 * FUNCTION: inputPWM
 * Sets the GPIO PWM level to be the value pointed to
 * by the servo's input pointer.
 * -------------------------------------------
 * PARAMETERS:
 * s : Pointer to a servo
 * -------------------------------------------
 * RETURNS: void
 */
void inputPWM(Servo *s)
{
    // sets the pwm level
    pwm_set_gpio_level(s->pinNum, *(s->input));
}

/**
 * FUNCTION: updatePWM
 * Sets the GPIO PWM level to be the servo struct's pwm value
 * -------------------------------------------
 * PARAMETERS:
 * s : Pointer to a servo
 * -------------------------------------------
 * RETURNS: void
 */
void updatePWM(Servo *s)
{
    // sets the pwm level
    pwm_set_gpio_level(s->pinNum, s->pwm);
}

/**
 * FUNCTION: setMode
 * Sets the mode of the Servo (OFF, LO, MD, HI) AND
 * Sets the speed of the servo to its corresponding mode value 
 * -------------------------------------------
 * PARAMETERS:
 * s    : Pointer to a servo
 * mode : OFF, LD, MD, or HI
 * -------------------------------------------
 * RETURNS:
 * Void
 */
void setMode(Servo *s, uint8_t mode)
{
    s->mode = mode;
    s->speed = mode;
}

/**
 * FUNCTION: modeUp
 * Sets the mode to shift up to the next level
 * OFF -> LO : LO -> MD : MD -> HI : HI -> HI 
 * -------------------------------------------
 * PARAMETERS:
 * s : Pointer to a servo
 * -------------------------------------------
 * RETURNS:
 * Void
 */
void modeUp(Servo *s)
{
    switch(s->mode){
        case OFF:
            setMode(s, LO);
            break;
        case LO:
            setMode(s, MD);
            break;
        default: 
            setMode(s, HI);
            break;
    }
}

/**
 * FUNCTION: modeDown
 * Sets the mode to shift down to the previous level
 * HI -> MD : MD -> LO : LO -> OFF : OFF -> OFF
 * -------------------------------------------
 * PARAMETERS:
 * s : Pointer to a servo
 * -------------------------------------------
 * RETURNS:
 * Void
 */
void modeDown(Servo *s)
{
    switch(s->mode){
        case HI:
            setMode(s, MD);
            break;
        case MD:
            setMode(s, LO);
            break;
        default: 
            setMode(s, OFF);
            break;
    }
}

/**
 * FUNCTION: setSpeed
 * Sets the mode of the Servo (OFF, LO, MD, HI) AND
 * Sets the speed of the servo to its corresponding mode value 
 * -------------------------------------------
 * PARAMETERS:
 * s     : Pointer to a servo
 * speed : value to increment/decrement servo pwm by
 * -------------------------------------------
 * RETURNS:
 * Void
 */
void setSpeed(Servo *s, uint8_t speed)
{
    s->speed = speed;
}