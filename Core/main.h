/**
 * @file    main.h
 * @brief   系统主头文件 - 包含类型定义和函数声明
 * @author  智能台灯项目组
 * @version 1.0
 * @date    2025-10-24
 */
#ifndef MAIN_H
#define MAIN_H

#include "timer.h"
#include "xpt2046.h"
#include "key.h"
#include "nixie.h"
#include "config.h"


// @brief 照明灯亮度等级枚举
typedef enum {
	LAMP_CLOSE,		// 1档- 关闭
	LAMP_LOW,		// 2档- 弱
	LAMP_MEDIUM,	// 3档- 中
	LAMP_HIGH,		// 4档- 高
	LAMP_HIGHEST	// 5档- 最高
} LightStatus;

// @brief 系统工作模式枚举
typedef enum {
	MANUAL,		// 手动模式
	AUTO		// 自动模式
} SystemStatus;


// * @brief 系统核心数据结构体
// * @details 包含所有与系统状态相关的数据变量
typedef struct {
    LightStatus lightState;        // 当前台灯亮度状态
    unsigned char lightGrade[5];   // 亮度等级对应PWM占空比数组
    SystemStatus systemState;      // 系统工作模式
    unsigned int lightIntensity;   // 光线强度传感器读数
    unsigned char nixieBuf[4];     // 数码管显示缓冲区
    unsigned char pwmCount;        // PWM周期计数器(0-99)
    unsigned char timeCount;       // 人走计时器(秒)
} SystemData;

// * @brief 系统标志位结构体
 //* @details 包含所有系统状态标志位
typedef struct {
    unsigned char humanFlag;   // 人体检测标志: 0-无人, 1-有人
    unsigned char countFlag;   // 计时使能标志: 0-停止, 1-开始
    unsigned long count;       // 定时器预分频计数器
} SystemFlag;


// 函数声明
// 系统初始化函数
void systemInit(void);
// 状态机处理函数
void stateMachine(void);
// 台灯自动控制
void lampAutoControl(void);
// 显示数据更新函数
void updateDisplay(void);

#endif