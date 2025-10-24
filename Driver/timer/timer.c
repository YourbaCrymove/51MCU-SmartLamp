/**
 * @file    timer.c
 * @brief   定时器驱动实现文件
 * @details 基于11.0592MHz晶振，配置定时器0每100us中断一次
 * @author  智能台灯项目组
 * @version 1.0
 * @date    2025-10-24
 */
#include <regx52.h>
#include "timer.h"

/**
 * @brief 定时器0初始化函数
 * @details 配置为模式1(16位定时器)，基于11.0592MHz晶振
 * @note 定时器初值计算：65536 - 11059200/12/10000 = 65536-92
 */
void timer0Init(void) {
	TMOD &= 0xF0;  // 清零T0的控制位，不影响T1的配置
	TMOD |= 0x01;   // 设置T0为模式1 (16位定时器)
	
	TH0 = (65536 - 92) / 256;
    TL0 = (65536 - 92) % 256;
	
	EA = 1;  // 开启总中断
	ET0 = 1; // 开启定时器0中断
	TR0 = 1; // 启动定时器0
}

