#ifndef KEYCONFIG_H
#define KEYCONFIG_H

// 按键消抖时间（单位：ms），STC89C52适合10-20ms
// 这个时间需要根据实际按键的机械特性调整，一般为10-20ms
#define DEBOUNCE_TIME 20

// 按键状态枚举
// 按下按键的完整流程：IDLE → DEBOUNCE_PRESS → PRESSED → DEBOUNCE_RELEASE → IDLE
typedef enum {
    KEY_STATE_IDLE = 0,          // 空闲状态：按键未被按下，等待按键动作
    KEY_STATE_DEBOUNCE_PRESS,    // 按下消抖状态：检测到按键按下，正在进行消抖处理
    KEY_STATE_PRESSED,           // 按下确认状态：按键已确认按下，可以返回按键值
    KEY_STATE_DEBOUNCE_RELEASE   // 释放消抖状态：检测到按键释放，正在进行消抖处理
} keyState;


#endif