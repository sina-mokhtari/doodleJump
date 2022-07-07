/*
 * LiquidCrystal.c - liquidCrystal Library for STM32 ARM microcontrollers
 *
 *  Created on: April 12, 2018
 *      Author: S. Saeed Hosseini (sayidhosseini@hotmail.com)
 *      Ported from: Arduino, Adafruit (https://github.com/arduino-libraries/LiquidCrystal)
 *      Published to: Github (https://github.com/SayidHosseini/STM32LiquidCrystal)
 */

#include "stm32f3xx_hal.h" // change this line accordingly
#include "LiquidCrystal.h"
#include <string.h>
#include <cmsis_os.h>

// change this to 0 if you want to use 8-bit mode
uint8_t fourbitMode = 1;

// change this to LCD_5x10DOTS for some 1 line displays that can select a 10 pixel high font
uint8_t dotsize = LCD_5x8DOTS;

// pin definitions and other LCD variables
uint16_t rsPin; // LOW: command.  HIGH: character.
uint16_t rwPin; // LOW: write to LCD.  HIGH: read from LCD.
uint16_t enablePin; // activated by a HIGH pulse.
uint16_t dataPins[8];
GPIO_TypeDef *port;

uint8_t displayfunction;
uint8_t displaycontrol;
uint8_t displaymode;

uint8_t initialized;

uint8_t numlines;
uint8_t rowOffsets[4];

void liquidCrystal(GPIO_TypeDef *gpioport, uint16_t rs, uint16_t rw, uint16_t enable,
                   uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3) {
    if (fourbitMode)
        init(1, gpioport, rs, rw, enable, d0, d1, d2, d3, 0, 0, 0, 0);
    else
        init(0, gpioport, rs, rw, enable, d0, d1, d2, d3, 0, 0, 0, 0);
}

void init(uint8_t fourbitmode, GPIO_TypeDef *gpioport, uint16_t rs, uint16_t rw, uint16_t enable,
          uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3,
          uint16_t d4, uint16_t d5, uint16_t d6, uint16_t d7) {
    rsPin = rs;
    rwPin = rw;
    enablePin = enable;
    port = gpioport;

    dataPins[0] = d0;
    dataPins[1] = d1;
    dataPins[2] = d2;
    dataPins[3] = d3;
    dataPins[4] = d4;
    dataPins[5] = d5;
    dataPins[6] = d6;
    dataPins[7] = d7;

    if (fourbitmode)
        displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    else
        displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

    begin(16, 2);
}

void begin(uint8_t cols, uint8_t lines) {
    if (lines > 1) {
        displayfunction |= LCD_2LINE;
    }
    numlines = lines;

    setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
        displayfunction |= LCD_5x10DOTS;
    }

    //Initializing GPIO Pins
    enableClock();

    GPIO_InitTypeDef gpioInit;
    gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;

    if (fourbitMode)
        gpioInit.Pin = rsPin | rwPin | enablePin | dataPins[0] | dataPins[1] | dataPins[2] | dataPins[3];
    else
        gpioInit.Pin = rsPin | rwPin | enablePin | dataPins[0] | dataPins[1] | dataPins[2] | dataPins[3] |
                       dataPins[4] | dataPins[5] | dataPins[6] | dataPins[7];

    HAL_GPIO_Init(port, &gpioInit);

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V,
    // so we'll wait 50 just to make sure
    osDelay(50);

    // Now we pull both RS and R/W low to begin commands
    HAL_GPIO_WritePin(port, rsPin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(port, enablePin, GPIO_PIN_RESET);

    if (rwPin != 255) {
        HAL_GPIO_WritePin(port, rwPin, GPIO_PIN_RESET);
    }

    //put the LCD into 4 bit or 8 bit mode
    if (!(displayfunction & LCD_8BITMODE)) {
        // this is according to the hitachi HD44780 datasheet
        // figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        write4Bits(0x03);
        osDelay(5); // wait min 4.1ms

        // second try
        write4Bits(0x03);
        osDelay(5); // wait min 4.1ms

        // third go!
        write4Bits(0x03);
        osDelay(1);

        // finally, set to 4-bit interface
        write4Bits(0x02);
    } else {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        command(LCD_FUNCTIONSET | displayfunction);
        osDelay(5);  // wait more than 4.1ms

        // second try
        command(LCD_FUNCTIONSET | displayfunction);
        osDelay(1);

        // third go
        command(LCD_FUNCTIONSET | displayfunction);
    }

    // finally, set # lines, font size, etc.
    command(LCD_FUNCTIONSET | displayfunction);

    // turn the display on with no cursor or blinking default
    displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;

    display();

    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | displaymode);

}

// enables GPIO RCC Clock
void enableClock(void) {
    if (port == GPIOA)
        __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port == GPIOC)
        __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (port == GPIOD)
        __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (port == GPIOE)
        __HAL_RCC_GPIOE_CLK_ENABLE();
    else if (port == GPIOF)
        __HAL_RCC_GPIOF_CLK_ENABLE();

    // if you have a port that is not listed add it below the other else ifs
}

void setRowOffsets(int row0, int row1, int row2, int row3) {
    rowOffsets[0] = row0;
    rowOffsets[1] = row1;
    rowOffsets[2] = row2;
    rowOffsets[3] = row3;
}

/********** high level commands, for the user! */
void clear(void) {
    command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    osDelay(2);  // this command takes a long time!
}

void home(void) {
    command(LCD_RETURNHOME);  // set cursor position to zero
    osDelay(2);  // this command takes a long time!
}

void setCursor(uint8_t col, uint8_t row) {
    const size_t maxLines = sizeof(rowOffsets) / sizeof(*rowOffsets);
    if (row >= maxLines) {
        row = maxLines - 1;    // we count rows starting w/0
    }
    if (row >= numlines) {
        row = numlines - 1;    // we count rows starting w/0
    }

    command(LCD_SETDDRAMADDR | (col + rowOffsets[row]));
}

// Turn the display on/off (quickly)
void noDisplay(void) {
    displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | displaycontrol);
}

void display(void) {
    displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | displaycontrol);
}

// Turns the underline cursor on/off
void noCursor(void) {
    displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | displaycontrol);
}

void cursor(void) {
    displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | displaycontrol);
}

// Turn on and off the blinking cursor
void noBlink(void) {
    displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | displaycontrol);
}

void blink(void) {
    displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void scrollDisplayRight(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight(void) {
    displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | displaymode);
}

// This is for text that flows Right to Left
void rightToLeft(void) {
    displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | displaymode);
}

// This will 'right justify' text from the cursor
void autoscroll(void) {
    displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | displaymode);
}

// This will 'left justify' text from the cursor
void noAutoscroll(void) {
    displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | displaymode);
}

// This will print character string to the LCD
size_t print(const char str[]) {
    if (str == NULL) return 0;

    const uint8_t *buffer = (const uint8_t *) str;
    size_t size = strlen(str);
    size_t n = 0;

    while (size--) {
        if (write(*buffer++)) n++;
        else break;
    }
    return n;
}

// Allows us to fill the first 8 CGRAM locations
// with custom charactersArr
void createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}

/*********** mid level commands, for sending data/cmds */

inline void command(uint8_t value) {
    send(value, GPIO_PIN_RESET);
}

inline size_t write(uint8_t value) {
    send(value, GPIO_PIN_SET);
    return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void send(uint8_t value, GPIO_PinState mode) {
    HAL_GPIO_WritePin(port, rsPin, mode);

    // if there is a RW pin indicated, set it low to Write
    if (rwPin != 255) {
        HAL_GPIO_WritePin(port, rwPin, GPIO_PIN_RESET);
    }

    if (displayfunction & LCD_8BITMODE) {
        write8Bits(value);
    } else {
        write4Bits(value >> 4);
        write4Bits(value);
    }
}

void pulseEnable(void) {
    HAL_GPIO_WritePin(port, enablePin, GPIO_PIN_RESET);
   // osDelay(1);
    HAL_GPIO_WritePin(port, enablePin, GPIO_PIN_SET);
   // osDelay(1);    // enable pulse must be >450ns
    HAL_GPIO_WritePin(port, enablePin, GPIO_PIN_RESET);
    osDelay(1);   // commands need > 37us to settle
}

void write4Bits(uint8_t value) {
    for (int i = 0; i < 4; i++) {
        HAL_GPIO_WritePin(port, dataPins[i], ((value >> i) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }

    pulseEnable();
}

void write8Bits(uint8_t value) {
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(port, dataPins[i], ((value >> i) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }

    pulseEnable();
}
