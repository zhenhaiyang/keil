/*
 * @file    key.h
 * @author  zhy
 * @version 1.0
 * @date    2020-11-12
 * @brief   按键输入的接口
 */

#ifndef __KEY__H__
#define __KEY__H__

//按键编号
typedef enum 
{
    zero,
    one,
    two,
    wkup,
    nothing
}KeyNum;


//函数声明
extern void KEY_init(void);
extern KeyNum scanKey(void);



#endif
