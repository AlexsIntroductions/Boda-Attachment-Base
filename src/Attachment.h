#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#ifndef ATTACHMENT_H
#define ATTACHMENT_H

extern uint16_t att_input;

extern uint16_t att_output;

extern uint16_t WHO_AM_I;

extern uint8_t buttons[8];

extern uint8_t data[128];

extern uint8_t indices[8];

extern uint8_t currentIndex;

extern uint8_t indexMax;

extern uint8_t indexMin;

#define ATT_X 0xBF
#define ATT_CIRCLE 0xDF
#define ATT_TRIANGLE 0xEF
#define ATT_SQUARE 0x7F

#define ATT_R1 0xF7
#define ATT_R2 0xFD
#define ATT_L1 0xFB
#define ATT_L2 0xFE

#define ATT_DRIGHT 0xDF
#define ATT_DUP 0xEF
#define ATT_DDOWN 0xBF
#define ATT_DLEFT 0x7F

#define ATT_START 0xF7
#define ATT_SELECT 0xFE
#define ATT_L3 0xFD
#define ATT_R3 0xFB

typedef void (*buttonFunction)(void);

typedef struct
{
    uint16_t *buttonStatus;
    buttonFunction buttonFunctions[8];

} Attachment;

void initAttachment(Attachment *a);
void pollButtonFunctions(Attachment *a);
void buttonTimeout(Attachment *a);
void spi_irq_init();
void spi_irq();

//----------Button Function Declarations for User to Define----------//
void buttonOneFunction();
void buttonTwoFunction();
void buttonThreeFunction();
void buttonFourFunction();
void buttonFiveFunction();
void buttonSixFunction();
void buttonSevenFunction();
void buttonEightFunction();

#endif