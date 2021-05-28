/**
  ******************************************************************************
  * @file    lcd.c
  * @author  zhy
  * @version 1.0
  * @date    2021-05-13
  * @brief   lcd驱动程序
  ******************************************************************************
  * @version 1.1
  * @date    2021-05-26
  * @brief   lcd驱动程序
  *             将内部变量与程序更新为内部连接
  ******************************************************************************
  * @version 1.2
  * @date    2021-05-27
  * @brief   lcd驱动程序
  *             增加了空格的支持
  ******************************************************************************
  */
#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "delay.h"
#include "font.h"

//{
/*-----------------------------------公有域:开始--------------------------------------*/

/*-----------------------------------公有域:结束--------------------------------------*/
//}

//{
/*-----------------------------------私有域:开始--------------------------------------*/

typedef enum
{
    EN,
    CH
} CharacterType;

/* 字符特性 */
typedef struct
{
    LcdCoordinate st;   //起始坐标
    FontSize size;      //字符大小
    LcdColor foreColor; //前景色
    LcdColor backColor; //背景色
} LcdCharacter;

/* LCD参数 */
typedef struct
{
    volatile uint16_t command; //DCX(A18)为低电平——指令
    volatile uint16_t data;    //DCX(A18)为高电平——数据
} LCDTypedef;

#define LCD_BASE (0x60000000UL | 0x0007FFFEUL) //此处的地址对应A18，由于是16位总线，所以A18实际上对应地址19位。
#define LCD ((LCDTypedef *)LCD_BASE)           //将类型强制转换为结构体地址类型

#define __LcdWriteReg(__reg, __data) \
    do                               \
    {                                \
        LCD->command = __reg;        \
        LCD->data = __data;          \
    } while (0)

static LcdCharacter character = {0,          //字符起始行
                                 0,          //字符起始列
                                 FS2,        //字体大小
                                 LCD_BLACK,  //前景色
                                 LCD_WHITE}; //背景色

static void ShowWord(const uint8_t *pc, CharacterType type);
static void SetRegion(uint16_t colS, uint16_t colE, uint16_t rowS, uint16_t rowE);

/*-----------------------------------私有域:结束--------------------------------------*/
//}

/** 
 * @brief 跳过了几个空字符
 * @note 无
 * @param {uint8_t} num 增加空格的个数
 * @retval 1：表示正确增加  0：表示超过边界
 */
uint8_t LcdSkip(uint8_t num)
{
    while (num--)
    {
        character.st.y += character.size >> 1;                  //空格占半个字符
        if (character.st.y + (character.size >> 1) > LCD_WIDTH) //若列数超过边界
        {
            character.st.x += character.size;                 //增加行数
            character.st.y = 0;                               //列数归零
            if (character.st.x + character.size > LCD_HEIGHT) //若行数超过边界
            {
                return 0;
            }
        }
    }
    return 1;
}

/** 
 * @brief 设置焦点
 * @note  无
 * @param {LcdCoordinate} coor 焦点
 * @retval 无
 */
void LcdSetFocus(LcdCoordinate coor)
{
    character.st.x = coor.x;
    character.st.y = coor.y;
}

/** 
 * @brief 获得屏幕焦点
 * @note 无
 * @param {*}无
 * @retval 无
 */
LcdCoordinate LcdGetFocus()
{
    return character.st;
}

/** 
 * @brief 设置字体大小
 * @note 无
 * @param {FontSize} size 无
 * @retval 无
 */
void LcdSetFontSize(FontSize size)
{
    character.size = size;
}

/** 
 * @brief 获得字体大小
 * @note 无
 * @param {*} 无
 * @retval 无
 */
FontSize LcdgetFontSize()
{
    return character.size;
}

/** 
 * @brief 设置屏幕的颜色
 * @note 无
 * @param {LcdColor} foreColor 前景色
 * @param {LcdColor} backColor 背景色
 * @retval 无
 */
void LcdSetColor(LcdColor foreColor, LcdColor backColor)
{
    character.foreColor = foreColor;
    character.backColor = backColor;
}

/** 
 * @brief 显示数字
 * @note 无
 * @param {uint32_t} num 显示的数字
 * @param {uint8_t} length 若为0，按照数字的长度显示，若大于0，则强制为数字的长度
 * @retval 无
 */
void LcdShowNumber(uint32_t num, uint8_t lengthShow)
{
    uint8_t length = 0;
    uint8_t digit[10] = {0};
    uint32_t tempNum = num;

    while (tempNum)
    {
        digit[length] = tempNum % 10;
        length++;
        tempNum /= 10;
    }

    for (int i = lengthShow; i > length; i--)
    {
        ShowWord(tableFS2E[0], EN); //显示空格
    }

    for (int i = length - 1; i >= 0; i--)
    {
        ShowWord(tableFS2E[digit[i] - ' ' + '0'], EN);
    }
}

/** 
 * @brief 将字符串输出到屏幕上
 * @note 无
 * @param {const char} *str 带输出的字符
 * @retval 无
 */
void LcdShowString(const char *str)
{
    uint8_t sizeX = character.size;
    for (const char *ch = str; *ch; ch++)
    {
        if (*ch == 0) //字符串结束
        {
            return;
        }

        if (*ch == '\n')
        {
            character.st.x += sizeX;
            character.st.y = 0;
            continue;
        }
        else if (*ch < ' ' || *ch > '~') //忽略不可打印字符
        {
            continue;
        }

        ShowWord(tableFS2E[*ch - ' '], EN);
    }
}

/** 
 * @brief 将汉字输出到屏幕上
 * @note 无
 * @param {const uint8_t} *encodes 汉字编码
 * @retval 无
 */
void LcdShowChinese(const uint8_t *encodes)
{
    uint16_t size = character.size;
    uint16_t index = 0;

    for (int i = 0; encodes[i] != 253; i++)
    {
        index = encodes[i];

        if (index < 250)
        {
            ShowWord(tableFS2C[index], CH);
        }
        else if (index == 255) //作为换行符
        {
            character.st.x += size;
            character.st.y = 0;
        }
        else if (index == 254) //作为空格
        {
            ShowWord(tableFS2E[0], EN);
        }
    }
}

/** 
 * @brief 清除屏幕为同一种颜色
 * @note 无
 * @param {LcdColor} color 设置的颜色
 * @retval 无
 */
void LcdClear(LcdColor color)
{
    SetRegion(0, 479, 0, 799); //设置写入范围
    LCD->command = 0x2c00;     //从左上角开始写入
    for (uint32_t i = 0; i < 800 * 480; i++)
    {
        LCD->data = color;
    }
}

/** 
 * @brief lcd的初始化程序
 * @note    1.该程序仿造官方手册，但是参数有的改变了。没有找打改变的依据
 *          2.LCD使用FMC驱动，在使用该LCD之前，需要首先配置FMC相关数据。背光使能为PA5
 * @param {*}无
 * @retval 无
 */
void LcdInit(void)
{
    //page1 enable
    __LcdWriteReg(0xF000, 0x55);
    __LcdWriteReg(0xF001, 0xAA);
    __LcdWriteReg(0xF002, 0x52);
    __LcdWriteReg(0xF003, 0x08);
    __LcdWriteReg(0xF004, 0x01);
    //AVDD Set AVDD 5.2V
    __LcdWriteReg(0xB000, 0x0D);
    __LcdWriteReg(0xB001, 0x0D);
    __LcdWriteReg(0xB002, 0x0D);
    //AVDD ratio
    __LcdWriteReg(0xB600, 0x34);
    __LcdWriteReg(0xB601, 0x34);
    __LcdWriteReg(0xB602, 0x34);
    //AVEE -5.2V
    __LcdWriteReg(0xB100, 0x0D);
    __LcdWriteReg(0xB101, 0x0D);
    __LcdWriteReg(0xB102, 0x0D);
    //AVEE ratio
    __LcdWriteReg(0xB700, 0x34);
    __LcdWriteReg(0xB701, 0x34);
    __LcdWriteReg(0xB702, 0x34);
    //VCL -2.5V
    __LcdWriteReg(0xB200, 0x00);
    __LcdWriteReg(0xB201, 0x00);
    __LcdWriteReg(0xB202, 0x00);
    //VCL ratio
    __LcdWriteReg(0xB800, 0x24);
    __LcdWriteReg(0xB801, 0x24);
    __LcdWriteReg(0xB802, 0x24);
    //VGH 15V (Free pump)
    __LcdWriteReg(0xBF00, 0x01);
    __LcdWriteReg(0xB300, 0x0F);
    __LcdWriteReg(0xB301, 0x0F);
    __LcdWriteReg(0xB302, 0x0F);
    //VGH ratio
    __LcdWriteReg(0xB900, 0x34);
    __LcdWriteReg(0xB901, 0x34);
    __LcdWriteReg(0xB902, 0x34);
    //VGL_REG -10V
    __LcdWriteReg(0xB500, 0x08);
    __LcdWriteReg(0xB501, 0x08);
    __LcdWriteReg(0xB502, 0x08);
    __LcdWriteReg(0xC200, 0x03);
    //VGLX ratio
    __LcdWriteReg(0xBA00, 0x24);
    __LcdWriteReg(0xBA01, 0x24);
    __LcdWriteReg(0xBA02, 0x24);
    //VGMP/VGSP 4.5V/0V
    __LcdWriteReg(0xBC00, 0x00);
    __LcdWriteReg(0xBC01, 0x78);
    __LcdWriteReg(0xBC02, 0x00);
    //VGMN/VGSN -4.5V/0V
    __LcdWriteReg(0xBD00, 0x00);
    __LcdWriteReg(0xBD01, 0x78);
    __LcdWriteReg(0xBD02, 0x00);
    //VCOM
    __LcdWriteReg(0xBE00, 0x00);
    __LcdWriteReg(0xBE01, 0x64);
    //Gamma Setting
    __LcdWriteReg(0xD100, 0x00);
    __LcdWriteReg(0xD101, 0x33);
    __LcdWriteReg(0xD102, 0x00);
    __LcdWriteReg(0xD103, 0x34);
    __LcdWriteReg(0xD104, 0x00);
    __LcdWriteReg(0xD105, 0x3A);
    __LcdWriteReg(0xD106, 0x00);
    __LcdWriteReg(0xD107, 0x4A);
    __LcdWriteReg(0xD108, 0x00);
    __LcdWriteReg(0xD109, 0x5C);
    __LcdWriteReg(0xD10A, 0x00);
    __LcdWriteReg(0xD10B, 0x81);
    __LcdWriteReg(0xD10C, 0x00);
    __LcdWriteReg(0xD10D, 0xA6);
    __LcdWriteReg(0xD10E, 0x00);
    __LcdWriteReg(0xD10F, 0xE5);
    __LcdWriteReg(0xD110, 0x01);
    __LcdWriteReg(0xD111, 0x13);
    __LcdWriteReg(0xD112, 0x01);
    __LcdWriteReg(0xD113, 0x54);
    __LcdWriteReg(0xD114, 0x01);
    __LcdWriteReg(0xD115, 0x82);
    __LcdWriteReg(0xD116, 0x01);
    __LcdWriteReg(0xD117, 0xCA);
    __LcdWriteReg(0xD118, 0x02);
    __LcdWriteReg(0xD119, 0x00);
    __LcdWriteReg(0xD11A, 0x02);
    __LcdWriteReg(0xD11B, 0x01);
    __LcdWriteReg(0xD11C, 0x02);
    __LcdWriteReg(0xD11D, 0x34);
    __LcdWriteReg(0xD11E, 0x02);
    __LcdWriteReg(0xD11F, 0x67);
    __LcdWriteReg(0xD120, 0x02);
    __LcdWriteReg(0xD121, 0x84);
    __LcdWriteReg(0xD122, 0x02);
    __LcdWriteReg(0xD123, 0xA4);
    __LcdWriteReg(0xD124, 0x02);
    __LcdWriteReg(0xD125, 0xB7);
    __LcdWriteReg(0xD126, 0x02);
    __LcdWriteReg(0xD127, 0xCF);
    __LcdWriteReg(0xD128, 0x02);
    __LcdWriteReg(0xD129, 0xDE);
    __LcdWriteReg(0xD12A, 0x02);
    __LcdWriteReg(0xD12B, 0xF2);
    __LcdWriteReg(0xD12C, 0x02);
    __LcdWriteReg(0xD12D, 0xFE);
    __LcdWriteReg(0xD12E, 0x03);
    __LcdWriteReg(0xD12F, 0x10);
    __LcdWriteReg(0xD130, 0x03);
    __LcdWriteReg(0xD131, 0x33);
    __LcdWriteReg(0xD132, 0x03);
    __LcdWriteReg(0xD133, 0x6D);
    __LcdWriteReg(0xD200, 0x00);
    __LcdWriteReg(0xD201, 0x33);
    __LcdWriteReg(0xD202, 0x00);
    __LcdWriteReg(0xD203, 0x34);
    __LcdWriteReg(0xD204, 0x00);
    __LcdWriteReg(0xD205, 0x3A);
    __LcdWriteReg(0xD206, 0x00);
    __LcdWriteReg(0xD207, 0x4A);
    __LcdWriteReg(0xD208, 0x00);
    __LcdWriteReg(0xD209, 0x5C);
    __LcdWriteReg(0xD20A, 0x00);

    __LcdWriteReg(0xD20B, 0x81);
    __LcdWriteReg(0xD20C, 0x00);
    __LcdWriteReg(0xD20D, 0xA6);
    __LcdWriteReg(0xD20E, 0x00);
    __LcdWriteReg(0xD20F, 0xE5);
    __LcdWriteReg(0xD210, 0x01);
    __LcdWriteReg(0xD211, 0x13);
    __LcdWriteReg(0xD212, 0x01);
    __LcdWriteReg(0xD213, 0x54);
    __LcdWriteReg(0xD214, 0x01);
    __LcdWriteReg(0xD215, 0x82);
    __LcdWriteReg(0xD216, 0x01);
    __LcdWriteReg(0xD217, 0xCA);
    __LcdWriteReg(0xD218, 0x02);
    __LcdWriteReg(0xD219, 0x00);
    __LcdWriteReg(0xD21A, 0x02);
    __LcdWriteReg(0xD21B, 0x01);
    __LcdWriteReg(0xD21C, 0x02);
    __LcdWriteReg(0xD21D, 0x34);
    __LcdWriteReg(0xD21E, 0x02);
    __LcdWriteReg(0xD21F, 0x67);
    __LcdWriteReg(0xD220, 0x02);
    __LcdWriteReg(0xD221, 0x84);
    __LcdWriteReg(0xD222, 0x02);
    __LcdWriteReg(0xD223, 0xA4);
    __LcdWriteReg(0xD224, 0x02);
    __LcdWriteReg(0xD225, 0xB7);
    __LcdWriteReg(0xD226, 0x02);
    __LcdWriteReg(0xD227, 0xCF);
    __LcdWriteReg(0xD228, 0x02);
    __LcdWriteReg(0xD229, 0xDE);
    __LcdWriteReg(0xD22A, 0x02);
    __LcdWriteReg(0xD22B, 0xF2);
    __LcdWriteReg(0xD22C, 0x02);
    __LcdWriteReg(0xD22D, 0xFE);
    __LcdWriteReg(0xD22E, 0x03);
    __LcdWriteReg(0xD22F, 0x10);
    __LcdWriteReg(0xD230, 0x03);
    __LcdWriteReg(0xD231, 0x33);
    __LcdWriteReg(0xD232, 0x03);
    __LcdWriteReg(0xD233, 0x6D);
    __LcdWriteReg(0xD300, 0x00);
    __LcdWriteReg(0xD301, 0x33);
    __LcdWriteReg(0xD302, 0x00);
    __LcdWriteReg(0xD303, 0x34);
    __LcdWriteReg(0xD304, 0x00);
    __LcdWriteReg(0xD305, 0x3A);
    __LcdWriteReg(0xD306, 0x00);
    __LcdWriteReg(0xD307, 0x4A);
    __LcdWriteReg(0xD308, 0x00);
    __LcdWriteReg(0xD309, 0x5C);
    __LcdWriteReg(0xD30A, 0x00);

    __LcdWriteReg(0xD30B, 0x81);
    __LcdWriteReg(0xD30C, 0x00);
    __LcdWriteReg(0xD30D, 0xA6);
    __LcdWriteReg(0xD30E, 0x00);
    __LcdWriteReg(0xD30F, 0xE5);
    __LcdWriteReg(0xD310, 0x01);
    __LcdWriteReg(0xD311, 0x13);
    __LcdWriteReg(0xD312, 0x01);
    __LcdWriteReg(0xD313, 0x54);
    __LcdWriteReg(0xD314, 0x01);
    __LcdWriteReg(0xD315, 0x82);
    __LcdWriteReg(0xD316, 0x01);
    __LcdWriteReg(0xD317, 0xCA);
    __LcdWriteReg(0xD318, 0x02);
    __LcdWriteReg(0xD319, 0x00);
    __LcdWriteReg(0xD31A, 0x02);
    __LcdWriteReg(0xD31B, 0x01);
    __LcdWriteReg(0xD31C, 0x02);
    __LcdWriteReg(0xD31D, 0x34);
    __LcdWriteReg(0xD31E, 0x02);
    __LcdWriteReg(0xD31F, 0x67);
    __LcdWriteReg(0xD320, 0x02);
    __LcdWriteReg(0xD321, 0x84);
    __LcdWriteReg(0xD322, 0x02);
    __LcdWriteReg(0xD323, 0xA4);
    __LcdWriteReg(0xD324, 0x02);
    __LcdWriteReg(0xD325, 0xB7);
    __LcdWriteReg(0xD326, 0x02);
    __LcdWriteReg(0xD327, 0xCF);
    __LcdWriteReg(0xD328, 0x02);
    __LcdWriteReg(0xD329, 0xDE);
    __LcdWriteReg(0xD32A, 0x02);
    __LcdWriteReg(0xD32B, 0xF2);
    __LcdWriteReg(0xD32C, 0x02);
    __LcdWriteReg(0xD32D, 0xFE);
    __LcdWriteReg(0xD32E, 0x03);
    __LcdWriteReg(0xD32F, 0x10);
    __LcdWriteReg(0xD330, 0x03);
    __LcdWriteReg(0xD331, 0x33);
    __LcdWriteReg(0xD332, 0x03);
    __LcdWriteReg(0xD333, 0x6D);
    __LcdWriteReg(0xD400, 0x00);
    __LcdWriteReg(0xD401, 0x33);
    __LcdWriteReg(0xD402, 0x00);
    __LcdWriteReg(0xD403, 0x34);
    __LcdWriteReg(0xD404, 0x00);
    __LcdWriteReg(0xD405, 0x3A);
    __LcdWriteReg(0xD406, 0x00);
    __LcdWriteReg(0xD407, 0x4A);
    __LcdWriteReg(0xD408, 0x00);
    __LcdWriteReg(0xD409, 0x5C);
    __LcdWriteReg(0xD40A, 0x00);
    __LcdWriteReg(0xD40B, 0x81);

    __LcdWriteReg(0xD40C, 0x00);
    __LcdWriteReg(0xD40D, 0xA6);
    __LcdWriteReg(0xD40E, 0x00);
    __LcdWriteReg(0xD40F, 0xE5);
    __LcdWriteReg(0xD410, 0x01);
    __LcdWriteReg(0xD411, 0x13);
    __LcdWriteReg(0xD412, 0x01);
    __LcdWriteReg(0xD413, 0x54);
    __LcdWriteReg(0xD414, 0x01);
    __LcdWriteReg(0xD415, 0x82);
    __LcdWriteReg(0xD416, 0x01);
    __LcdWriteReg(0xD417, 0xCA);
    __LcdWriteReg(0xD418, 0x02);
    __LcdWriteReg(0xD419, 0x00);
    __LcdWriteReg(0xD41A, 0x02);
    __LcdWriteReg(0xD41B, 0x01);
    __LcdWriteReg(0xD41C, 0x02);
    __LcdWriteReg(0xD41D, 0x34);
    __LcdWriteReg(0xD41E, 0x02);
    __LcdWriteReg(0xD41F, 0x67);
    __LcdWriteReg(0xD420, 0x02);
    __LcdWriteReg(0xD421, 0x84);
    __LcdWriteReg(0xD422, 0x02);
    __LcdWriteReg(0xD423, 0xA4);
    __LcdWriteReg(0xD424, 0x02);
    __LcdWriteReg(0xD425, 0xB7);
    __LcdWriteReg(0xD426, 0x02);
    __LcdWriteReg(0xD427, 0xCF);
    __LcdWriteReg(0xD428, 0x02);
    __LcdWriteReg(0xD429, 0xDE);
    __LcdWriteReg(0xD42A, 0x02);
    __LcdWriteReg(0xD42B, 0xF2);
    __LcdWriteReg(0xD42C, 0x02);
    __LcdWriteReg(0xD42D, 0xFE);
    __LcdWriteReg(0xD42E, 0x03);
    __LcdWriteReg(0xD42F, 0x10);
    __LcdWriteReg(0xD430, 0x03);
    __LcdWriteReg(0xD431, 0x33);
    __LcdWriteReg(0xD432, 0x03);
    __LcdWriteReg(0xD433, 0x6D);
    __LcdWriteReg(0xD500, 0x00);
    __LcdWriteReg(0xD501, 0x33);
    __LcdWriteReg(0xD502, 0x00);
    __LcdWriteReg(0xD503, 0x34);
    __LcdWriteReg(0xD504, 0x00);
    __LcdWriteReg(0xD505, 0x3A);
    __LcdWriteReg(0xD506, 0x00);
    __LcdWriteReg(0xD507, 0x4A);
    __LcdWriteReg(0xD508, 0x00);
    __LcdWriteReg(0xD509, 0x5C);
    __LcdWriteReg(0xD50A, 0x00);
    __LcdWriteReg(0xD50B, 0x81);

    __LcdWriteReg(0xD50C, 0x00);
    __LcdWriteReg(0xD50D, 0xA6);
    __LcdWriteReg(0xD50E, 0x00);
    __LcdWriteReg(0xD50F, 0xE5);
    __LcdWriteReg(0xD510, 0x01);
    __LcdWriteReg(0xD511, 0x13);
    __LcdWriteReg(0xD512, 0x01);
    __LcdWriteReg(0xD513, 0x54);
    __LcdWriteReg(0xD514, 0x01);
    __LcdWriteReg(0xD515, 0x82);
    __LcdWriteReg(0xD516, 0x01);
    __LcdWriteReg(0xD517, 0xCA);
    __LcdWriteReg(0xD518, 0x02);
    __LcdWriteReg(0xD519, 0x00);
    __LcdWriteReg(0xD51A, 0x02);
    __LcdWriteReg(0xD51B, 0x01);
    __LcdWriteReg(0xD51C, 0x02);
    __LcdWriteReg(0xD51D, 0x34);
    __LcdWriteReg(0xD51E, 0x02);
    __LcdWriteReg(0xD51F, 0x67);
    __LcdWriteReg(0xD520, 0x02);
    __LcdWriteReg(0xD521, 0x84);
    __LcdWriteReg(0xD522, 0x02);
    __LcdWriteReg(0xD523, 0xA4);
    __LcdWriteReg(0xD524, 0x02);
    __LcdWriteReg(0xD525, 0xB7);
    __LcdWriteReg(0xD526, 0x02);
    __LcdWriteReg(0xD527, 0xCF);
    __LcdWriteReg(0xD528, 0x02);
    __LcdWriteReg(0xD529, 0xDE);
    __LcdWriteReg(0xD52A, 0x02);
    __LcdWriteReg(0xD52B, 0xF2);
    __LcdWriteReg(0xD52C, 0x02);
    __LcdWriteReg(0xD52D, 0xFE);
    __LcdWriteReg(0xD52E, 0x03);
    __LcdWriteReg(0xD52F, 0x10);
    __LcdWriteReg(0xD530, 0x03);
    __LcdWriteReg(0xD531, 0x33);
    __LcdWriteReg(0xD532, 0x03);
    __LcdWriteReg(0xD533, 0x6D);
    __LcdWriteReg(0xD600, 0x00);
    __LcdWriteReg(0xD601, 0x33);
    __LcdWriteReg(0xD602, 0x00);
    __LcdWriteReg(0xD603, 0x34);
    __LcdWriteReg(0xD604, 0x00);
    __LcdWriteReg(0xD605, 0x3A);
    __LcdWriteReg(0xD606, 0x00);
    __LcdWriteReg(0xD607, 0x4A);
    __LcdWriteReg(0xD608, 0x00);
    __LcdWriteReg(0xD609, 0x5C);
    __LcdWriteReg(0xD60A, 0x00);
    __LcdWriteReg(0xD60B, 0x81);

    __LcdWriteReg(0xD60C, 0x00);
    __LcdWriteReg(0xD60D, 0xA6);
    __LcdWriteReg(0xD60E, 0x00);
    __LcdWriteReg(0xD60F, 0xE5);
    __LcdWriteReg(0xD610, 0x01);
    __LcdWriteReg(0xD611, 0x13);
    __LcdWriteReg(0xD612, 0x01);
    __LcdWriteReg(0xD613, 0x54);
    __LcdWriteReg(0xD614, 0x01);
    __LcdWriteReg(0xD615, 0x82);
    __LcdWriteReg(0xD616, 0x01);
    __LcdWriteReg(0xD617, 0xCA);
    __LcdWriteReg(0xD618, 0x02);
    __LcdWriteReg(0xD619, 0x00);
    __LcdWriteReg(0xD61A, 0x02);
    __LcdWriteReg(0xD61B, 0x01);
    __LcdWriteReg(0xD61C, 0x02);
    __LcdWriteReg(0xD61D, 0x34);
    __LcdWriteReg(0xD61E, 0x02);
    __LcdWriteReg(0xD61F, 0x67);
    __LcdWriteReg(0xD620, 0x02);
    __LcdWriteReg(0xD621, 0x84);
    __LcdWriteReg(0xD622, 0x02);
    __LcdWriteReg(0xD623, 0xA4);
    __LcdWriteReg(0xD624, 0x02);
    __LcdWriteReg(0xD625, 0xB7);
    __LcdWriteReg(0xD626, 0x02);
    __LcdWriteReg(0xD627, 0xCF);
    __LcdWriteReg(0xD628, 0x02);
    __LcdWriteReg(0xD629, 0xDE);
    __LcdWriteReg(0xD62A, 0x02);
    __LcdWriteReg(0xD62B, 0xF2);
    __LcdWriteReg(0xD62C, 0x02);
    __LcdWriteReg(0xD62D, 0xFE);
    __LcdWriteReg(0xD62E, 0x03);
    __LcdWriteReg(0xD62F, 0x10);
    __LcdWriteReg(0xD630, 0x03);
    __LcdWriteReg(0xD631, 0x33);
    __LcdWriteReg(0xD632, 0x03);
    __LcdWriteReg(0xD633, 0x6D);
    //LV2 Page 0 enable
    __LcdWriteReg(0xF000, 0x55);
    __LcdWriteReg(0xF001, 0xAA);
    __LcdWriteReg(0xF002, 0x52);
    __LcdWriteReg(0xF003, 0x08);
    __LcdWriteReg(0xF004, 0x00);
    //Display control
    __LcdWriteReg(0xB100, 0xCC);
    __LcdWriteReg(0xB101, 0x00);
    //Source hold time
    __LcdWriteReg(0xB600, 0x05);
    //Gate EQ control
    __LcdWriteReg(0xB700, 0x70);
    __LcdWriteReg(0xB701, 0x70);
    //Source EQ control (Mode 2)
    __LcdWriteReg(0xB800, 0x01);
    __LcdWriteReg(0xB801, 0x03);
    __LcdWriteReg(0xB802, 0x03);
    __LcdWriteReg(0xB803, 0x03);
    //Inversion mode (2-dot)
    __LcdWriteReg(0xBC00, 0x02);
    __LcdWriteReg(0xBC01, 0x00);
    __LcdWriteReg(0xBC02, 0x00);
    //Timing control 4H w/ 4-delay
    __LcdWriteReg(0xC900, 0xD0);
    __LcdWriteReg(0xC901, 0x02);
    __LcdWriteReg(0xC902, 0x50);
    __LcdWriteReg(0xC903, 0x50);
    __LcdWriteReg(0xC904, 0x50);

    __LcdWriteReg(0x3500, 0x00); //Tearing Effect Line ON
    __LcdWriteReg(0x3A00, 0x55); //16-bit/pixel

    LCD->command = 0x1100; //Sleep out & booster on
    delay_ms(120);
    LCD->command = 0x2900; //Display on

    LcdClear(LCD_WHITE);
}

/*-----------------------------------内部函数--------------------------------------*/

/** 
 * @brief 设置写入区域范围
 * @note 从0开始计数
 * @param {uint16_t} colS 开始的列数(包含)
 * @param {uint16_t} colE 结束的列数(包含)
 * @param {uint16_t} rowS 开始的行数(包含)
 * @param {uint16_t} rowE 结束的行数(包含)
 * @retval 无
 */
static void SetRegion(uint16_t colS, uint16_t colE, uint16_t rowS, uint16_t rowE)
{
    /* 1.设置列范围 */
    __LcdWriteReg(0x2a00, colS >> 8);
    __LcdWriteReg(0x2a01, colS & 0xFF);
    __LcdWriteReg(0x2a02, colE >> 8);
    __LcdWriteReg(0x2a03, colE & 0xFF);

    /* 2.设置行范围 */
    __LcdWriteReg(0x2b00, rowS >> 8);
    __LcdWriteReg(0x2b01, rowS & 0xFF);
    __LcdWriteReg(0x2b02, rowE >> 8);
    __LcdWriteReg(0x2b03, rowE & 0xFF);
}

/** 
 * @brief 在给定位置显示给定颜色的汉字
 * @note 无
 * @param {uint8_t} pc 汉字编码数组
 * @param {CharacterType} type 字符类型：英文或者中文
 * @retval 无
 */
static void ShowWord(const uint8_t *pc, CharacterType type)
{
    int length = 0;
    uint16_t *col = &character.st.y;
    uint16_t *row = &character.st.x;
    uint8_t sizeX = character.size;
    uint8_t sizeY = type == EN ? character.size >> 1 : character.size;

    /* 1.判断字符是否越界 */
    if (character.st.y + sizeY > LCD_WIDTH)
    {
        character.st.x += sizeX;
        character.st.y = 0;
    }
    if (character.st.x + sizeX > LCD_HEIGHT)
    {
        return;
    }

    /* 2.设置字符区域 */
    SetRegion(*col, *col + sizeY - 1, *row, *row + sizeX - 1);
    length = sizeX * sizeY >> 3;

    /* 3.写入字符 */
    LCD->command = 0x2c00;
    for (int i = 0; i < length; i++)
    {
        uint8_t temp = pc[i];
        for (int j = 0; j < 8; j++)
        {
            LCD->data = temp & 0x01 ? character.foreColor : character.backColor;
            temp >>= 1;
        }
    }

    /* 4.光标右移一个位置 */
    if (type == EN)
    {
        LcdSkip(1);
    }
    else
    {
        LcdSkip(2);
    }
}
