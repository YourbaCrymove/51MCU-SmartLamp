#include "nixieSTC89C52RC.h"
#include <regx52.h>
#include "delay.h"

// 共阳极数码管段码表（0-9）
// 段码顺序：dp g f e d c b a (P0.7-P0.0)
unsigned char code nixieTable[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66,		// 显示 0-4
    0x6d, 0x7d, 0x07, 0x7f, 0x6f, 		// 显示 5-9
	0x00
};  

// 位选数组：存储每个位置对应的CBA电平组合
// 数组索引0-7对应位置1-8，每个元素的低3位分别对应C(P2_4)、B(P2_3)、A(P2_2)
unsigned char code locationTable[] = {
    0x07, 0x06, 0x05, 0x04,		// 位置 1-4
    0x03, 0x02, 0x01, 0x00		// 位置 5-8
};

/**
 * @brief 数码管显示函数
 * @param location 数码管位置（1-8）
 * @param number 要显示的数字（0-9）
 * @param point 小数点（0表示不显示，1表示显示）
 */
void nixieScan(unsigned char location, unsigned char number, unsigned char point) {   
	unsigned char pos;		// 数组索引变量
	
    // 校验位置范围
    if(location < 1 || location > 8) return;
    
    // 根据位置设置位选信号（通过74HC138译码器）
    pos = location - 1;  // 转换为数组索引
    P2_4 = (locationTable[pos] >> 2) & 0x01;  // 提取C位
    P2_3 = (locationTable[pos] >> 1) & 0x01;  // 提取B位
    P2_2 = locationTable[pos] & 0x01;         // 提取A位
    
    // 设置段选信号（通过P0口输出段码）
	if(point) {	// 判断是不是数码管的偶数位
		P0 = nixieTable[number] | 0x80;		// 偶数位加上小数点
	} else {
		P0 = nixieTable[number];		// 奇数位正常显示
	}
    
    
    // 静态显示数码管去掉下面两行代码
    // 短暂延时，维持显示
    delayMs(1);  // 约1ms延时，保证亮度
    
    // 消隐处理：关闭所有段，防止切换到下一个数码管时产生鬼影
    P0 = 0x00;  // 所有段置高电平（共阳极数码管熄灭）
}