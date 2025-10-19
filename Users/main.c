/* 头文件包含区域 */
#include "main.h"
#include "keyInput.h"

/* 变量声明区域 */
lightStatus lightState;	// 台灯状态变量
unsigned char lightGrade[5] = {0, 25, 50, 75, 100};	// 台灯等级
unsigned char pwmCount;		// PWM周期计数（0~99，累计100×10μs=1ms）

/* 函数声明区域 */
// 系统初始化函数
void systemInit(void) {
	timer0Init();	// 初始化定时器0
	
	lightState = LAMP_CLOSE;	// 上电时默认关闭
	pwmCount = 0;	// PWM周期计数初始为 0
}

//// 台灯调节函数
//void lampControl(void) {
//	LED_MAIN = 0;	// 台灯引脚给 0 亮
//	
//}

/* Main */
void main() {		
	// 以下为台灯调节测试代码
	unsigned char keyNumber;
	
	systemInit();

	
	while(1) {
		keyNumber = keyRead();
		if(keyNumber == 1) {
			lightState = LAMP_LOW;	// 弱亮
		} else if(keyNumber == 2) {
			lightState = LAMP_MEDIUM;	// 中亮
		} else if(keyNumber == 3) {
			lightState = LAMP_HIGHT;	// 高亮
		} else if(keyNumber == 4) {
			lightState = LAMP_HIGHTEST;	// 最亮
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