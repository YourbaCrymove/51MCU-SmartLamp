#include "delay.h"
#include <regx52.h>
#include <intrins.h>

/**
 * 微秒级延时函数
 * 说明：基于11.0592MHz晶振，每个机器周期约为1.085us
 * @param us 延时微秒数，最大约为65535us
 */
void delayUs(unsigned int us)
{
    unsigned int i;
    // 根据晶振频率计算循环次数，这里做了近似处理
    for(i = 0; i < us; i++)
    {
        // 大约1us的延时（根据实际情况微调）
        _nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();_nop_();_nop_();
        _nop_();_nop_();
    }
}

/**
 * 毫秒级延时函数
 * @param ms 延时毫秒数，最大约为65535ms
 */
void delayMs(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
    {
        // 大约1ms的延时（11.0592MHz晶振）
        for(j = 0; j < 112; j++);
    }
}

/**
 * 秒级延时函数
 * @param s 延时秒数
 */
void delayS(unsigned int s)
{
    unsigned int i;
    // 1秒 = 1000毫秒，通过循环调用毫秒延时函数实现
    for(i = 0; i < s; i++)
    {
        delayMs(1000);  // 延时1000毫秒即1秒
    }
}
