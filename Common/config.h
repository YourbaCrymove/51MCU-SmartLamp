#ifndef KEYCONFIG_H
#define KEYCONFIG_H


// 引脚重定义
sbit LED_MAIN = P2^0;	// 目前给主照明灯随便弄了引脚，到时候有引脚冲突了再改过
sbit LED1 = P2^6;	// 自动模式指示灯
sbit LED2 = P2^7;	// 手动模式指示灯

// 按键消抖时间（单位：ms），STC89C52适合10-20ms
// 这个时间需要根据实际按键的机械特性调整，一般为10-20ms
#define DEBOUNCE_TIME 20




#endif