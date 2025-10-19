#ifndef KEYINPUT_H
#define KEYINPUT_H

// 函数声明，用户只需调用keyRead函数
unsigned char keyScan(void);      // 扫描按键物理状态，返回当前按下的按键编号
unsigned char keyRead(void);      // 读取经过消抖处理的按键值，返回有效的按键编号

#endif