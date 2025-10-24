/**
 * @file    xpt2046.h
 * @brief   XPT2046触摸芯片驱动头文件
 * @details 12位ADC转换，支持触摸和光敏电阻数据读取
 * @author  智能台灯项目组
 * @version 1.0
 * @date    2025-10-24
 */
#ifndef XPT2046_H
#define XPT2046_H

#include <regx52.h>
#include "config.h"

// @brief 读取XPT2046 ADC转换值
unsigned int xpt2046_read_adc_value(unsigned char cmd);

#endif