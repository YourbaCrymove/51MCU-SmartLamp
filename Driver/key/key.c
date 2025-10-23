#include "key.h"
#include <regx52.h>
#include "config.h"

// 全局变量定义
static keyState currentKeyState = KEY_STATE_IDLE;    // 当前按键状态机状态，初始为空闲状态
static unsigned char keyDebounceTimer = 0;           // 消抖计时器，用于计算消抖时间
static unsigned char confirmedKey = 0;               // 保存确认的按键值，确保返回正确的按键编号

/**
 * @brief 扫描按键物理状态
 * @return 按键编号：1-4对应不同按键，0表示无按键按下
 * @note 此函数仅读取IO口状态，不进行任何消抖处理
 */
unsigned char keyScan(void) {
    if(P3_1 == 0) return 1;		// 按键1
    if(P3_0 == 0) return 2;		// 按键2
    if(P3_2 == 0) return 3;		// 按键3
    if(P3_3 == 0) return 4;		// 按键4
    return 0;		// 无按键
}

/**
 * @brief 读取经过消抖处理的按键值
 * @return 有效的按键编号：1-4对应不同按键，0表示无有效按键事件
 * @note 使用状态机实现按键消抖，确保每次按键只触发一次
 */
unsigned char keyRead(void) {
    unsigned char keyNumber = 0;      // 要返回的按键值，默认为0（无按键）
    unsigned char currentKey = 0;     // 当前扫描到的按键值
    
    // 扫描当前按键状态
    currentKey = keyScan();
    
    // 按键状态机处理
    switch(currentKeyState) {
        case KEY_STATE_IDLE:    // 空闲状态：按键未按下
            // 检测到有按键按下（非0表示有按键）
            if(currentKey != 0) {
                // 转移到按下消抖状态，开始消抖处理
                currentKeyState = KEY_STATE_DEBOUNCE_PRESS;
                confirmedKey = currentKey;  // 记录当前按下的按键，用于后续验证
            }            
            break;
            
        case KEY_STATE_DEBOUNCE_PRESS:     // 按下消抖状态：确认按键真的按下
            // 消抖计时器递增，模拟时间流逝
            keyDebounceTimer++;
            
            // 检查是否达到消抖时间要求
            if(keyDebounceTimer >= DEBOUNCE_TIME) {
                keyDebounceTimer = 0;  // 重置计时器
                
                // 重新扫描按键状态，确认消抖后的实际状态
                currentKey = keyScan();
                
                // 验证是否仍然是之前记录的按键被按下（确认不是抖动）
                if(confirmedKey == currentKey) {
                    // 确认按键有效按下，转移到按下确认状态
                    currentKeyState = KEY_STATE_PRESSED;
                    keyNumber = confirmedKey;  // 返回确认的按键值给调用者
                } else {
                    // 按键状态不一致，判断为机械抖动，返回空闲状态
                    currentKeyState = KEY_STATE_IDLE;
                }
            }
            break;
            
        case KEY_STATE_PRESSED:    // 按下确认状态：处理按键按下逻辑
            // 等待按键释放，这里不返回按键值（按键值已在消抖状态返回）
            // 检测按键是否释放（扫描值为0表示释放）
            if(keyScan() == 0) {
                // 按键释放，转移到释放消抖状态
                currentKeyState = KEY_STATE_DEBOUNCE_RELEASE;
            }
            break;
        
        case KEY_STATE_DEBOUNCE_RELEASE:    // 释放消抖状态：确认按键真的释放
            // 释放消抖计时器递增
            keyDebounceTimer++;
            
            // 检查是否达到消抖时间要求
            if(keyDebounceTimer >= DEBOUNCE_TIME) {
                keyDebounceTimer = 0;  // 重置计时器
                
                // 重新扫描按键状态，确认是否真的释放
                if(keyScan() == 0) {
                    // 确认按键完全释放，返回空闲状态，准备检测下一次按键
                    currentKeyState = KEY_STATE_IDLE;
                } else {
                    // 按键仍然被按下，可能是长按或抖动，保持释放消抖状态继续等待
                    // 注意：不要回到PRESSED状态，避免重复触发按键事件
                    currentKeyState = KEY_STATE_DEBOUNCE_RELEASE;
                }
            }
            break;
    }
    
    // 返回按键值（仅在按下消抖状态确认后会返回非0值）
    return keyNumber;
}