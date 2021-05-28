/**
  ******************************************************************************
  * @file    lcd.h
  * @author  zhy
  * @version 1.0
  * @date    2021-05-13
  * @brief   lcd.c的对外接口
  ******************************************************************************
  */

#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>

/* 显示器颜色 */
typedef enum __LcdColor
{
    LCD_WHITE = 0xFFFF,
    LCD_BLACK = 0x0000,
    LCD_RED = 0xF800,
    LCD_YELLOW = 0xFFE0,
    LCD_BLUE = 0x001F,
    LCD_GREEN = 0x07E0,
    LCD_GRAY = 0x8430
} LcdColor;

/* 显示器的尺寸 */
typedef enum __LcdSize
{
    LCD_WIDTH = 480,
    LCD_HEIGHT = 600
} LcdSize;

/* 字体大小 */
typedef enum __FontSize
{
    FS1 = 8,
    FS2 = 16,
    FS3 = 24,
    FS4 = 32,
    FS5 = 40,
    FS6 = 48,
    FS7 = 56,
    FS8 = 64
} FontSize;

/* 坐标 */
typedef struct
{
    uint16_t x; //行数
    uint16_t y; //列数
} LcdCoordinate;

extern void LcdInit(void);
extern void LcdClear(LcdColor color);
extern uint8_t LcdSkip(uint8_t num);
extern void LcdShowChinese(const uint8_t *encoder);
extern void LcdShowString(const char *str);
extern void LcdShowNumber(uint32_t, uint8_t lengthShow);

extern void LcdSetFocus(LcdCoordinate coor);
extern LcdCoordinate LcdGetFocus(void);
extern void LcdSetFontSize(FontSize size);
extern FontSize LcdgetFontSize(void);
extern void LcdSetColor(LcdColor foreColor, LcdColor backColor);

#endif
