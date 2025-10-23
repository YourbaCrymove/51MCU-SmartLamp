#ifndef _xpt2046_H
#define _xpt2046_H

#include <regx52.h>

//管脚定义
sbit DOUT = P3^7;	  //输出
sbit CLK  = P3^6;	  //时钟
sbit DIN  = P3^4;	  //输入
sbit CS   = P3^5;	  //片选


//函数声明
unsigned int xpt2046_read_adc_value(unsigned char cmd);

#endif