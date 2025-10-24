/**
 * @file    key.h
 * @brief   按键驱动头文件
 * @details 实现基于状态机的按键消抖处理
 * @author  智能台灯项目组
 * @version 1.0
 * @date    2025-10-24
 */
#ifndef KEY_H
#define KEY_H

// * @brief 按键状态枚举
 //* @details 描述按键状态机的完整流程
typedef enum {
    KEY_STATE_IDLE = 0,          // 空闲状态：按键未被按下，等待按键动作
    KEY_STATE_DEBOUNCE_PRESS,    // 按下消抖状态：检测到按键按下，正在进行消抖处理
    KEY_STATE_PRESSED,           // 按下确认状态：按键已确认按下，可以返回按键值
    KEY_STATE_DEBOUNCE_RELEASE   // 释放消抖状态：检测到按键释放，正在进行消抖处理
} keyState;

// 函数声明，用户只需调用keyRead函数
unsigned char keyScan(void);      // 扫描按键物理状态，返回当前按下的按键编号
unsigned char keyRead(void);      // 读取经过消抖处理的按键值，返回有效的按键编号

#endif