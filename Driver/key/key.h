#ifndef KEYINPUT_H
#define KEYINPUT_H

// 按键状态枚举
// 按下按键的完整流程：IDLE → DEBOUNCE_PRESS → PRESSED → DEBOUNCE_RELEASE → IDLE
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