/**
 * @file    delay.h
 * @brief   精确延时函数头文件
 * @author  智能台灯项目组  
 * @version 1.0
 * @date    2025-10-24
 */
#ifndef DELAY_H
#define DELAY_H


// @defgroup delay_functions 延时函数组

// @brief 微秒级延时函数
// @param us 延时微秒数，范围: 0-65535
void delayUs(unsigned int us);

// @brief 毫秒级延时函数  
// @param ms 延时毫秒数，范围: 0-65535
void delayMs(unsigned int ms);

// @brief 秒级延时函数
// @param s 延时秒数
void delayS(unsigned int s);

#endif
