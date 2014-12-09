#include "delay.h"
#include "intrins.h"

void Delay1ms()		//@22.1184MHz
{
	unsigned char i, j;

	i = 22;
	j = 128;
	do
	{
		while (--j);
	} while (--i);
}

/*******************************/
void delay(unsigned int k)
{
	unsigned int i;
	for(i=0;i<k;i++)
	{
		Delay1ms();
	}
}



/**************************************
延时5微秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数，注意时钟过快时需要修改
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay5us()		//@22.1184MHz
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}

/**************************************
延时5毫秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay5ms()		//@22.1184MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}