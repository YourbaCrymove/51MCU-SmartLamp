#ifndef MAIN_H
#define MAIN_H

#include <regx52.h>

// 引脚重定义
sbit LED_MAIN = P2^0;	// 目前给主照明灯随便弄了引脚，到时候有引脚冲突了再改过


// 照明灯状态枚举
typedef enum {
	LAMP_CLOSE,		// 1档- 关闭
	LAMP_LOW,		// 2档- 弱
	LAMP_MEDIUM,	// 3档- 中
	LAMP_HIGHT,		// 4档- 高
	LAMP_HIGHTEST	// 5档- 最高
} lightStatus;

#endif