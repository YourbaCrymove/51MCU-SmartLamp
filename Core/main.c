/**
 * @file    main.c
 * @brief   智能台灯系统主程序
 * @details 实现台灯的手动/自动控制、亮度调节、人体检测等功能
 * @author  YourbaCrymove
 * @version 1.0
 * @projectIntroduction 参考...\README.md
 * @date    2025-10-24
 */
/* 头文件包含区域 */
#include "main.h"

/* 变量声明区域 */
static SystemData systemData;  // 系统数据实例
static SystemFlag systemFlag;  // 系统标志位实例
unsigned char keyNumber;       // 当前按键值


/**
 * @brief 系统初始化函数
 * @details 初始化定时器、GPIO、变量等系统资源
 */
void systemInit(void) {
    timer0Init();      // 初始化定时器0
    updateDisplay();   // 初始化指示灯显示
    
    // 初始化系统状态
    systemData.lightState = LAMP_MEDIUM;   // 默认中等亮度
    systemData.pwmCount = 0;               // PWM计数器归零
    
    // 初始化亮度等级数组
    systemData.lightGrade[0] = 0;    // 关闭
    systemData.lightGrade[1] = 25;   // 25%亮度
    systemData.lightGrade[2] = 50;   // 50%亮度
    systemData.lightGrade[3] = 75;   // 75%亮度
    systemData.lightGrade[4] = 100;  // 100%亮度
    
    systemData.systemState = MANUAL;       // 默认手动模式
    systemFlag.humanFlag = 1;              // 默认有人
    systemData.timeCount = 0;              // 计时器清零
    systemFlag.count = 0;                  // 预分频器清零
    systemFlag.countFlag = 0;              // 停止计时
    
    // 初始化显示数据
    systemData.lightIntensity = 0;
    systemData.nixieBuf[0] = 0;
    systemData.nixieBuf[1] = 0;
    systemData.nixieBuf[2] = 0;
    systemData.nixieBuf[3] = 0;
}

/**
 * @brief 状态机处理函数
 * @details 根据按键输入切换系统状态和调节亮度
 */
void stateMachine(void) {
    keyNumber = keyRead();
    
    switch(keyNumber) {
        case 1:  // 手动/自动模式切换
            systemData.systemState = (systemData.systemState == MANUAL) ? AUTO : MANUAL;
            
            if(systemData.systemState == MANUAL) {
                systemFlag.humanFlag = 1;  // 手动模式默认有人
            }
            
            if(systemData.systemState == AUTO) {
                systemData.lightState = LAMP_MEDIUM;  // 自动模式重置亮度
                systemData.timeCount = 0;             // 重置计时器
                systemFlag.countFlag = 1;             // 开始计时
            }
            
            updateDisplay();  // 更新指示灯
            break;
            
        case 2:  // 增加亮度挡位
            if(systemData.systemState == MANUAL) {
                systemData.lightState = (systemData.lightState + 1) % 5;
            }
            break;
            
        case 3:  // 降低亮度挡位
            if(systemData.systemState == MANUAL) {
                systemData.lightState = (systemData.lightState - 1 + 5) % 5;
            }
            break;
            
        case 4:  // 模拟人体检测(自动模式下)
            if(systemData.systemState == AUTO) {
                systemFlag.humanFlag = 1;      // 标记有人
                systemData.timeCount = 0;      // 重置计时
                systemFlag.countFlag = 1;      // 开始计时
            }
            break;
    }
}

/**
 * @brief 台灯自动控制函数
 * @details 根据环境光线强度自动调节台灯亮度
 * @note 仅在自动模式且检测到有人时生效
 */
void lampAutoControl(void) {
    // 检查模式条件
    if(systemData.systemState != AUTO || systemFlag.humanFlag == 0) {
        return;
    }
    
    // 读取光线传感器数据
    systemData.lightIntensity = xpt2046_read_adc_value(0xA4);
    
    // 根据光线强度设置亮度等级(光线越弱，亮度越高)
    if(systemData.lightIntensity > 0 && systemData.lightIntensity < LIGHT_LOWEST) {
        systemData.lightState = LAMP_HIGHEST;
    } else if(systemData.lightIntensity > LIGHT_LOWEST && systemData.lightIntensity < LIGHT_LOW) {
        systemData.lightState = LAMP_HIGH;
    } else if(systemData.lightIntensity > LIGHT_LOW && systemData.lightIntensity < LIGHT_MEDIUM) {
        systemData.lightState = LAMP_MEDIUM;
    } else if(systemData.lightIntensity > LIGHT_MEDIUM && systemData.lightIntensity < LIGHT_HIGH) {
        systemData.lightState = LAMP_LOW;
    } else if(systemData.lightIntensity > LIGHT_HIGH && systemData.lightIntensity < LIGHT_MAX) {
        systemData.lightState = LAMP_CLOSE;
    }
}

/**
 * @brief 指示灯更新函数
 * @details 更新模式指示灯状态和数码管显示数据
 */
void updateDisplay(void) {
    // 更新数码管显示缓冲区
    systemData.nixieBuf[0] = systemData.lightIntensity / 1000;
    systemData.nixieBuf[1] = systemData.lightIntensity % 1000 / 100;
    systemData.nixieBuf[2] = systemData.lightIntensity % 1000 % 100 / 10;
    systemData.nixieBuf[3] = systemData.lightIntensity % 1000 % 100 % 10;
    
    // 更新模式指示灯
    LED1 = !(systemData.systemState == AUTO);    // 自动模式指示灯
    LED2 = !(systemData.systemState == MANUAL);  // 手动模式指示灯
}

/**
 * @brief 数码管数据显示函数
 * @details 扫描刷新4位数码管显示光线强度值
 */
void dataDisplay(void) {
    nixieScan(5, systemData.nixieBuf[0], 0);
    nixieScan(6, systemData.nixieBuf[1], 0);
    nixieScan(7, systemData.nixieBuf[2], 0);
    nixieScan(8, systemData.nixieBuf[3], 0);
}

/**
 * @brief 主函数
 * @details 系统主循环，协调各功能模块运行
 */
void main() {
    systemInit();  // 系统初始化
    
    while(1) {
        stateMachine();     // 处理按键输入
        lampAutoControl();  // 自动亮度控制
        updateDisplay();    // 更新显示数据
        dataDisplay();      // 刷新数码管显示
    }
}

/**
 * @brief 定时器0中断服务函数
 * @details 每100us中断一次，处理PWM输出和人走计时
 * @note 中断号1对应定时器0
 */
void timer0_ISR(void) interrupt 1 {
    // 重装定时器初值(100us)
    TH0 = (65536 - 92) / 256;
    TL0 = (65536 - 92) % 256;
    
    // 人走计时处理
    if(systemFlag.countFlag) {
        systemFlag.count++;
        if(systemFlag.count == 10000) {  // 100us × 10000 = 1秒
            systemFlag.count = 0;        // 重置计数器
            systemData.timeCount++;      // 秒计数器递增
            
            // 检查是否达到关闭时间
            if(systemData.timeCount == LAMP_CLOSE_TIME) {
                systemFlag.countFlag = 0;   // 停止计时
                systemFlag.humanFlag = 0;   // 标记无人
            }
        }
    }
    
    // PWM输出处理
    systemData.pwmCount++;
    systemData.pwmCount %= 100;  // 保持0-99范围
    
    if(systemFlag.humanFlag == 1) {
        // 有人状态：根据亮度等级输出PWM
        if(systemData.pwmCount < systemData.lightGrade[systemData.lightState]) {
            LED_MAIN = 0;  // 点亮主灯
        } else {
            LED_MAIN = 1;  // 熄灭主灯
        }
    } else {
        // 无人状态：关闭主灯
        LED_MAIN = 1;
    }
}