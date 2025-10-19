/* 头文件包含区域 */
#include "main.h"
#include "keyInput.h"

/* 变量声明区域 */
lightStatus lightState;	// 台灯状态变量
unsigned char lightGrade[5] = {0, 25, 50, 75, 100};	// 台灯等级
unsigned char pwmCount;		// PWM周期计数（0~99，累计100×10μs=1ms）

systemStatus systemState;	// 用于存储系统所处模式
unsigned char keyNumber;	// 存储按键的值

/* 函数声明区域 */
// 系统初始化函数
void systemInit(void) {
	timer0Init();	// 初始化定时器0
	
	lightState = LAMP_CLOSE;	// 上电时，台灯默认关闭
	pwmCount = 0;	// PWM周期计数初始为 0
	
	systemState = MANUAL;	// 上电时，系统处于手动模式
}

// 状态机处理函数
// 处理按键输入，控制系统状态切换和台灯挡位修改
void stateMachine(void) {
	keyNumber = keyRead();
	switch(keyNumber) {
		case 1:		// 系统手动/自动模式切换
			systemState =  (systemState==MANUAL)?AUTO:MANUAL;
			break;
		case 2:    // 增加亮度挡位（循环）
			if(systemState==MANUAL) {	// 按键2仅在手动模式下有效
				lightState = (lightState + 1) % 5;	// 枚举变量可以使用自增/自减操作
			}
			break;
		case 3:    // 降低亮度挡位（循环）
			if(systemState==MANUAL) {	// 按键3仅在手动模式下有效
				lightState = (lightState - 1 + 5) % 5;
			}
			break;
	}
}

/* Main */
void main() {	
	systemInit();
	
	while(1) {
		stateMachine();
		
		if(systemState == MANUAL) {
			LED1 = 1;	// 关闭自动模式指示灯
			LED2 = 0;	// 开启手动模式指示灯
		} else {
			LED1 = 0;	// 开启自动模式指示灯
			LED2 = 1;	// 关闭手动模式指示灯
		}
	}
}

// 定时器0中断服务函数，每10us中断一次
void timer0_ISR(void) interrupt 1 {
    // 重装载初值，保证每次定时一致
    TH0 = (65536 - 9) / 256;  
    TL0 = (65536 - 9) % 256;  
    
	pwmCount++;
    pwmCount %= 100;
	if(pwmCount < lightGrade[lightState]) {
		LED_MAIN = 0;	// 台灯引脚给 0 亮
	} else {
		LED_MAIN = 1;	// 台灯引脚给 1 灭
	}
}