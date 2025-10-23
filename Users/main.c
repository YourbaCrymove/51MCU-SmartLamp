/* 头文件包含区域 */
#include "main.h"

/* 变量声明区域 */
lightStatus lightState;	// 台灯状态变量
unsigned char lightGrade[5] = {0, 25, 50, 75, 100};	// 台灯等级
unsigned char pwmCount;		// PWM周期计数（0~99，累计100×10μs=1ms）

systemStatus systemState;	// 用于存储系统所处模式
unsigned char keyNumber;	// 存储按键的值

bit humanFlag;	// 0-表示没人，1-表示有人
unsigned char timeCount;	// 人走计时
unsigned long count;	// 模拟预分频器
bit countFlag;	// 计时标志位

unsigned int lightIntensity;	// 存储光线强度值
unsigned char nixieBuf[4];	// 显示光线强度

/* 函数声明区域 */
// 系统初始化函数
void systemInit(void) {
	timer0Init();	// 初始化定时器0
	updateDisplay();	// 初始化指示灯
	
	lightState = LAMP_MEDIUM;	// 上电时，台灯默认中等亮度
	pwmCount = 0;	// PWM周期计数初始为 0
	
	systemState = MANUAL;	// 上电时，系统处于手动模式
	humanFlag = 1;	// 上电时默认有人
	timeCount = 0;	// 初始化倒计时时间
	count = 0;	// 初始化预分频器
	countFlag = 0;	// 默认不计时
	
	lightIntensity = 0;
	nixieBuf[0] = 0;
	nixieBuf[1] = 0;
	nixieBuf[2] = 0;
	nixieBuf[3] = 0;
}

// 状态机处理函数
// 处理按键输入，控制系统状态切换和台灯挡位修改
void stateMachine(void) {
	keyNumber = keyRead();
	switch(keyNumber) {
		case 1:		// 系统手动/自动模式切换
			systemState =  (systemState==MANUAL)?AUTO:MANUAL;
			// 手动模式表示有人来
			if(systemState==MANUAL) {
				humanFlag = 1;
			}
			// 如果是自动模式，设置中等亮度
			if(systemState == AUTO) {
				lightState = LAMP_MEDIUM;
				// 重置计时器并开始计时
				timeCount = 0;
				countFlag = 1;
			}
			// 更新指示灯
            updateDisplay();
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
		case 4:		// 用按键4模拟人来
			// 只在自动模式下有用
			if(systemState == AUTO) {
				humanFlag = 1;	// 模拟人来了，标志位置1			
				// 人来了之后就重置并开始计时
				timeCount = 0;
				countFlag = 1;
			}			
			break;
	}
}

// 台灯自动控制
// 根据光线自动调节照明灯亮度
void lampAutoControl(void) {
	// 系统模式处于非自动模式并且没检测到人
	if(systemState!=AUTO || humanFlag==0) {
		return;	// 如果不处于自动模式就结束函数
	}
	lightIntensity = xpt2046_read_adc_value(0xA4);	// 存储光敏电阻的值
	
	// 光敏电阻的范围是0-4095，将其分为五个档次，分别对应照明灯状态
	// 即：光线越弱，照明越强
	if(lightIntensity>0 && lightIntensity<LIGHT_LOWEST) {
		lightState = LAMP_HIGHTEST;
	} else if(lightIntensity>LIGHT_LOWEST && lightIntensity<LIGHT_LOW) {
		lightState = LAMP_HIGHT;
	} else if(lightIntensity>LIGHT_LOW && lightIntensity<LIGHT_MEDIUM) {
		lightState = LAMP_MEDIUM;
	} else if(lightIntensity>LIGHT_MEDIUM && lightIntensity<LIGHT_HIGH) {
		lightState = LAMP_LOW;
	} else if(lightIntensity>LIGHT_HIGH && lightIntensity<LIGHT_MAX) {
		lightState = LAMP_CLOSE;
	}
	
}

// 指示灯更新函数
void updateDisplay(void) {
	nixieBuf[0] = lightIntensity/1000;	// 4095/1000=4
	nixieBuf[1] = lightIntensity%1000/100;	// 4095%1000=095,095/100=0
	nixieBuf[1] = lightIntensity%1000%100/10;
	nixieBuf[1] = lightIntensity%1000%100%10;
	
    LED1 = !(systemState == AUTO);   // 自动模式指示灯
    LED2 = !(systemState == MANUAL); // 手动模式指示灯
}

// 数据显示函数：扫描刷新4位数码管（动态显示）
void dataDisplay(void) {
	nixieScan(5, nixieBuf[0], 0);
	nixieScan(6, nixieBuf[1], 0);
	nixieScan(7, nixieBuf[2], 0);
	nixieScan(8, nixieBuf[3], 0);
}

/* Main */
void main() {	
	systemInit();	// 系统初始化
	
	while(1) {
		stateMachine();	// 处理按键输入
		lampAutoControl();	// 台灯自动控制
		updateDisplay();	// 更新数据
		dataDisplay();	// 显示数据
		
	}
}

// 定时器0中断服务函数，每100us中断一次
void timer0_ISR(void) interrupt 1 {
    // 重装载初值，保证每次定时一致
    TH0 = (65536 - 92) / 256;
    TL0 = (65536 - 92) % 256;
    
	if(countFlag) {	// 计时标志位=1，则开始计时
		count++;
		if(count == 10000) {	// 1ms * 1 000 = 1s
			count = 0;
			timeCount++;
			// 如果等于预设时间就停止计时，并设置人离开
			// 实际测量时间时7s多暂时不清楚原因，我先完成功能，后面再来优化
			if(timeCount == LAMP_CLOSE_TIME) {
				countFlag = 0;
				humanFlag = 0;	// 置0表示人走了
			}
		}
	}	
	
	pwmCount++;
    pwmCount %= 100;
	// 简化的PWM输出逻辑
    if(humanFlag == 1) {
        // 有人，正常PWM调光
        if(pwmCount < lightGrade[lightState]) {
            LED_MAIN = 0;    // 照明灯引脚给 0 亮
        } else {
            LED_MAIN = 1;    // 照明灯引脚给 1 灭
        }
    } else {
        // 没人，灯熄灭
        LED_MAIN = 1;    // 照明灯引脚给 1 灭
    }
}