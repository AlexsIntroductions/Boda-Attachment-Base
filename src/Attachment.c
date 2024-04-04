#include "Attachment.h"

uint16_t att_input;
uint16_t att_output = 0x01;
uint16_t WHO_AM_I = 'A1';
uint8_t buttons[8] = {ATT_X, ATT_CIRCLE, ATT_TRIANGLE, ATT_SQUARE, ATT_R1, ATT_R2, ATT_L1, ATT_L2};
uint8_t data[128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t indices[8] = {0, 1, 2, 3, 4, 5, 6, 7};
uint8_t currentIndex = 8;
uint8_t indexMax = 127;
uint8_t indexMin = 8;

/**
 * FUNCTION:
 *
 * -------------------------------------------
 * PARAMETERS:
 *
 * -------------------------------------------
 * RETURNS:
 *
 */

/**
 * FUNCTION: initAttachment
 * Initializes instance of attachment struct
 * -------------------------------------------
 * PARAMETERS:
 * a : Pointer to attachment
 * -------------------------------------------
 * RETURNS:
 * Void
 */
void initAttachment(Attachment *a)
{
    a->buttonStatus = &att_input;

    a->buttonFunctions[0] = buttonOneFunction;
    a->buttonFunctions[1] = buttonTwoFunction;
    a->buttonFunctions[2] = buttonThreeFunction;
    a->buttonFunctions[3] = buttonFourFunction;
    a->buttonFunctions[4] = buttonFiveFunction;
    a->buttonFunctions[5] = buttonSixFunction;
    a->buttonFunctions[6] = buttonSevenFunction;
    a->buttonFunctions[7] = buttonEightFunction;

    // initialize SPI
    spi_init(spi_default, 2 * 1000 * 1000);
    spi_set_format(spi_default, 16, 1, 0, true);
    spi_set_slave(spi_default, true);

    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);

    // TEST CODE ----------------------------------
    for (int i = 0; i < 128; i++)
    {
        data[i] = i + 1;
    }
    // TEST CODE ----------------------------------

    // initialize spi irq for control data
    spi_irq_init();
}

/**
 * FUNCTION: pollButtonFunctions
 * When called, polls each bit in attachment's buttonStatus,
 * if the bit is set, execute the corresponding buttonFunction
 * -------------------------------------------
 * PARAMETERS:
 * a : Attachment to Poll
 * -------------------------------------------
 * RETURNS:
 * Void
 */
void pollButtonFunctions(Attachment *a)
{
    for (int i = 0; i < 8; i++)
    {
        if (*(a->buttonStatus) & (1 << i))
        {
            // if the buttons isnt meant to be held:

            (a->buttonFunctions[i])();

            uint16_t temp = 0b1111111111111111;
            temp ^= (1 << i);
            *(a->buttonStatus) &= temp;
        }
    }
}

/**
 * FUNCTION: spi_irq
 * Triggers on incoming SPI transaction, reads incoming
 * byte and sends one byte
 * !!NOT meant to be called by user, but can be edited by user if necessary
 * -------------------------------------------
 * PARAMETERS:
 * N/A
 * -------------------------------------------
 * RETURNS:
 * Void
 */
void spi_irq()
{
    spi_write16_read16_blocking(spi_default, (uint16_t *)&att_output, (uint16_t *)&att_input, 1);
    att_output = 0x01;
    uint8_t cmd = (uint8_t)((att_input >> 8) & 0xFF);
    uint8_t recData = (uint8_t)(att_input & 0xFF);
    if (cmd != 0)
    {
        // if command is greater than 127 return the index of the array
        if (cmd > 127)
        {
            att_output = (uint16_t)data[cmd - 128];
        }
        else
        {
            switch (cmd)
            {
            case 0x01:
                att_output = (uint16_t)buttons[0];
                att_output |= ((uint16_t)buttons[1] << 8);
                break;
            case 0x02:
                att_output = (uint16_t)buttons[2];
                att_output |= ((uint16_t)buttons[3] << 8);
                break;
            case 0x03:
                att_output = (uint16_t)buttons[4];
                att_output |= ((uint16_t)buttons[5] << 8);
                break;
            case 0x04:
                att_output = (uint16_t)buttons[6];
                att_output |= ((uint16_t)buttons[7] << 8);
                break;
                break;
            case 0x0C:
                att_output = 0x01;
                break;
            case 0x0D:
                break;
            case 0x0E:
                att_output = 0xAA;
                break;
            // Set Index 1, 2, 3, 4, 5, 6, 7, 8
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
                indices[(cmd & 0x0F)] = recData;
                break;
            // Send Data to Index 1, 2, 3, 4, 5, 6, 7, 8
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
            case 0x1C:
            case 0x1D:
            case 0x1E:
            case 0x1F:
                data[indices[(cmd & 0x0F) - 8]] = recData;
                break;
            // Genearal Data Access and Index Management
            // Receive Data to Index then Increment Index
            case 0x20:
                if ((uint16_t)(currentIndex + 1) > indexMax)
                {
                    data[currentIndex] = recData;
                    break;
                }
                data[currentIndex] = recData;
                currentIndex += 1;
                break;
            // Receive Data to index
            case 0x21:
                data[currentIndex] = recData;
                break;
            // Receive Data to Index then Decrement Index
            case 0x22:
                if ((int16_t)(currentIndex - 1) < indexMin)
                {
                    data[currentIndex] = recData;
                    break;
                }
                data[currentIndex--] = recData;
                break;
            // Set Index
            case 0x23:
                if (recData > indexMax)
                {
                    currentIndex = indexMax;
                }
                else if (recData < indexMin)
                {
                    currentIndex = indexMin;
                }
                else
                {
                    currentIndex = recData;
                }
                break;
            // Set Range Max
            case 0x24:
                // if range is greater than max elements in data
                if (recData > 127)
                {
                    indexMax = 127;
                }
                // or if range is less than the current set minimum
                else if (recData <= indexMin)
                {
                    indexMax = indexMin;
                }
                else
                {
                    indexMax = recData;
                }

                // check if the current index is within the new bound
                if (currentIndex > indexMax)
                {
                    currentIndex = indexMax;
                }
                break;
            // Set Range Min
            case 0x25:
                // If range goes into reserved space
                if (recData < 8)
                {
                    indexMin = 8;
                }
                // OR if the range is greater than the current Max
                else if (recData >= indexMax)
                {
                    indexMin = indexMax;
                }
                else
                {
                    indexMin = recData;
                }
                // Check if the currentIndex is within the new range
                if (currentIndex < indexMin)
                {
                    currentIndex = indexMin;
                }

                break;
            // Get Data at Current Index
            case 0x26:
                att_output = data[currentIndex];
                break;
            // Get Data at received Index
            case 0x27:
                if (recData > indexMax)
                {
                    att_output = data[indexMax];
                }
                else if (recData < indexMin)
                {
                    att_output = data[indexMin];
                }
                else
                {
                    att_output = data[recData];
                }
                break;
            // Send data and Increment Index
            case 0x28:
                if (currentIndex + 1 > indexMax)
                {
                    att_output = data[currentIndex];
                    break;
                }
                att_output = data[currentIndex++];
                break;
            // Send data and Decrement Index
            case 0x29:
                if ((int16_t)(currentIndex - 1) < indexMin)
                {
                    att_output = data[currentIndex];
                    break;
                }
                att_output = data[currentIndex--];
                break;
            // Send WHO_AM_I value
            case 0x64:
                att_output = WHO_AM_I;
                break;
            default:
                break;
            }
        }
        uint16_t temp = 0b1111111111111111;
        temp ^= 0xF0;
        att_input &= temp;
    }
}

/**
 * FUNCTION: spi_irq_init
 * Initialization for SPI IRQ
 * !!To be called after an attachment has been initialized
 * -------------------------------------------
 * PARAMETERS:
 * N/A
 * -------------------------------------------
 * RETURNS:
 * Void
 */
void spi_irq_init()
{
    spi0_hw->imsc = 1 << 2;
    irq_set_exclusive_handler(SPI0_IRQ, spi_irq);
    irq_set_enabled(SPI0_IRQ, true);
}