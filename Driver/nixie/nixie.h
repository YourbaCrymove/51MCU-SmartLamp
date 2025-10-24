/**
 * @file    nixie.h
 * @brief   数码管显示驱动头文件
 * @details 支持8位数码管动态扫描显示
 * @author  智能台灯项目组
 * @version 1.0
 * @date    2025-10-24
 */
#ifndef NIXIE_H
#define NIXIE_H

// 数码管显示函数
void nixieScan(unsigned char location, unsigned char number, unsigned char point);


#endif