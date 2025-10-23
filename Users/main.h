#ifndef MAIN_H
#define MAIN_H

#include <regx52.h>

// 引脚重定义
sbit LED_MAIN = P2^0;	// 目前给主照明灯随便弄了引脚，到时候有引脚冲突了再改过
sbit LED1 = P2^6;	// 自动模式指示灯
sbit LED2 = P2^7;	// 手动模式指示灯

// 无人时台灯延时关闭时间
#define LAMP_CLOSE_TIME 5

// 照明灯状态枚举
typedef enum {
	LAMP_CLOSE,		// 1档- 关闭
	LAMP_LOW,		// 2档- 弱
	LAMP_MEDIUM,	// 3档- 中
	LAMP_HIGHT,		// 4档- 高
	LAMP_HIGHTEST	// 5档- 最高
} lightStatus;

// 系统模式枚举
typedef enum {
	MANUAL,		// 手动模式
	AUTO		// 自动模式
} systemStatus;

// 函数声明
// 系统初始化函数
void systemInit(void);
// 状态机处理函数
void stateMachine(void);
// 指示灯更新函数
void updateIndicators(void);

#endif