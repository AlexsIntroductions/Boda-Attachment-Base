#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "src/servo.h"
#include "src/finger.h"
#include "src/Attachment.h"

uint8_t numServos = 0;

// Array containint list of all initialized servos
// NOTE: Only 8 servos can be initialized at a time
Servo servos[8];
uint16_t servoPWMS[8];

Finger leftFinger;
Finger rightFinger;

Attachment a;

/**
 * FUNCTION:
 * -------------------------------------------
 * PARAMETERS:
 *
 * -------------------------------------------
 * RETURNS:
 *
 */

/**
 * TODO:
 *  - Verify get angle works
 *  - add a delete servo function to free gpio pins
 *  - Create an SPI input for controllers
 *      - have it trigger an interrupt to update the value, this can be easiy checked with UART
 *  - Actually connect the servos
 *  - Research amps needed to control the servos while in motion
 *  - put it on tiny breadboard
 *  - have preprogrammed motions
 *      - Open, Close, Grab, Let Go
 *      - have an options to switch to joystick controls.
 *
 *  - Pitch idea of mode switching where a button/combo is pressed to route all controls to the attachment
 *  - ADD 360 DEGREE SERVO SUPPORT
 */


/**
 * FUNCTION: pwm_irq_handler
 * This function is set to be called anytime ANY pwm interrupt goes off.
 * Determines which pwm slice was triggered from the list of initialized servos
 * and calls pwm_irq to update its pwm level
 * -------------------------------------------
 * PARAMETERS: N/A
 *
 * -------------------------------------------
 * RETURNS: Void
 */
void pwm_irq_handler()
{
    uint8_t status = pwm_get_irq_status_mask();
    for (uint8_t i = 0; i < numServos; i++)
    {
        // if the status bit for the pwm slice has been set (an interrupt was triggered for the pwm slice)
        if (status & (1 << servos[i].sliceNum))
        {
            // set the pwm level of the servo whose interrupt was triggered
            updatePWM(&servos[i]);
            // clear the status bit
            pwm_clear_irq(pwm_gpio_to_slice_num(i));
        }
    }
}

/**
 * FUNCTION: pwm_irq_init
 * Called at the start of main to enable interrupts and
 * sets pwm to be able to trigger interrupts
 * -------------------------------------------
 * PARAMETERS: N/A
 * -------------------------------------------
 * RETURNS: Void
 */
void pwm_irq_init()
{
    irq_set_mask_enabled(PWM_IRQ_WRAP, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_irq_handler);
    irq_set_enabled(PWM_IRQ_WRAP, true);
}

int main()
{

    stdio_init_all();

    initAttachment(&a);

    // intialize pwm irq for servos
    pwm_irq_init();

    // // Initialize two servos for finger 
    initServo(&servos[0], 0, &servoPWMS[0], 16.f, 8193, MD, 0);
    // initServoMapping(&servos[0], 4095, 0);
    setServoLimits(&servos[0], MAX16 / 4, 8192);

    initServo(&servos[1], 1, &servoPWMS[1], 16.f, MAX16 - 8193, MD, 1);
    // initServoMapping(&servos[1], 4095, 0);
    setServoLimits(&servos[1], MAX16 / 4, 8192);

    initFinger(&leftFinger, &servos[0], &servos[1]);

    initServo(&servos[2], 2, &servoPWMS[2], 16.f, MAX16 - 8191, MD, 1);
    // initServoMapping(&servos[2], 4095, 0);
    setServoLimits(&servos[2], MAX16 / 4, 8192);

    initServo(&servos[3], 3, &servoPWMS[3], 16.f, 8193, MD, 0);
    // initServoMapping(&servos[3], 4095, 0);
    setServoLimits(&servos[3], MAX16 / 4, 8192);
    numServos = 4;
    initFinger(&rightFinger, &servos[2], &servos[3]);

    while (1)
    {
        pollButtonFunctions(&a);
    }
}

//----------Button Function Definitions For Each Button's Actions----------//

// Button One Function
void buttonOneFunction()
{
    // X Pressed
    retractWhole(&leftFinger);
    retractWhole(&rightFinger);
    return;
}

// Button Two Function
void buttonTwoFunction()
{
    // Circle Pressed
    extendWhole(&leftFinger);
    extendWhole(&rightFinger);
    return;
}

// Button Three Function
void buttonThreeFunction()
{
    // // Square Pressed
    // for (uint8_t i = 0; i < numServos; i++)
    // {
    //     modeDown(&servos[i]);
    // }
    // return;
    return;
}

// Button Four Function
void buttonFourFunction()
{
    // // Triangle Pressed
    // for (uint8_t i = 0; i < numServos; i++)
    // {
    //     modeUp(&servos[i]);
    // }
    // return;
    return;
}

// Button Five Function
void buttonFiveFunction()
{
    // L1 Pressed
    close(leftFinger.top, leftFinger.top->speed);
    close(rightFinger.top, rightFinger.top->speed);
    return;
}

// Button Six Function
void buttonSixFunction()
{
    // L2 Pressed
    open(leftFinger.top, leftFinger.top->speed);
    open(rightFinger.top, rightFinger.top->speed);
    return;
}

// Button Seven Function
void buttonSevenFunction()
{
    // R1 Pressed
    close(leftFinger.bot, leftFinger.bot->speed);
    close(rightFinger.bot, rightFinger.bot->speed);
    return;
}

// Button Eight Function
void buttonEightFunction()
{
    // R2 Pressed
    open(leftFinger.bot, leftFinger.bot->speed);
    open(rightFinger.bot, rightFinger.bot->speed);
    return;
}