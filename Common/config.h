/**
 * @file    config.h
 * @brief   系统硬件配置和参数定义
 * @author  智能台灯项目组
 * @version 1.0
 * @date    2025-10-24
 */
#ifndef CONFIG_H
#define CONFIG_H

// @defgroup xpt2046_pins XPT2046触摸芯片引脚定义
sbit DOUT = P3^7;  // 数据输出引脚
sbit CLK  = P3^6;  // 时钟信号引脚  
sbit DIN  = P3^4;  // 数据输入引脚
sbit CS   = P3^5;  // 片选信号引脚


// @defgroup led_pins LED灯引脚定义
sbit LED_MAIN = P2^0;  // 主照明灯控制引脚
sbit LED1 = P2^6;      // 自动模式指示灯
sbit LED2 = P2^7;      // 手动模式指示灯

// @defgroup system_params 系统参数配置
#define DEBOUNCE_TIME   20    // 按键消抖时间(ms)
#define LAMP_CLOSE_TIME 30    // 无人时台灯延时关闭时间(s)

// * @defgroup light_thresholds 光线强度阈值定义
// * @brief 光敏电阻ADC值范围0-4095，分为5个等级
#define LIGHT_LOWEST    819   // 最弱光线阈值
#define LIGHT_LOW       1683  // 弱光线阈值
#define LIGHT_MEDIUM    2457  // 中等光线阈值  
#define LIGHT_HIGH      3276  // 强光线阈值
#define LIGHT_MAX       4095  // 最强光线阈值

#endif