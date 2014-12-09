//***************************************
// BMP085 IIC测试程序
// 使用单片机STC89C51 
// 晶振：22.1184M
// 编译环境 Keil uVision4
// 时间：2011年7月1日
// QQ： 41484807
//****************************************
#include	"stc15f204ea.h"
#include	"typedef.h"
#include	"uart.h"
#include	"bmp085.h"

unsigned char ge,shi,bai,qian,wan,shiwan;
extern long temperature,pressure;

//*********************************************************
void conversion(long temp_data)
{    
	shiwan=temp_data/100000+0x30 ;
	temp_data=temp_data%100000;  //取余运算
	wan=temp_data/10000+0x30 ;
	temp_data=temp_data%10000;   //取余运算
	qian=temp_data/1000+0x30 ;
	temp_data=temp_data%1000;    //取余运算
	bai=temp_data/100+0x30;
	temp_data=temp_data%100;     //取余运算
	shi=temp_data/10+0x30;
	temp_data=temp_data%10;      //取余运算
	ge=temp_data+0x30;
}

//***********************************************************************
void bmp085Convert()
{
	bmp085Compute();
	conversion(temperature);
	uartSendString("T");
	uartSendString(":");
	uartSendData(bai);
	uartSendData(shi);
	uartSendString(".");
	uartSendData(ge);
	uartSendString(" ");
	//uartSendString(0xDF);
	uartSendString("C");
	uartSendString("\r\n");	

	conversion(pressure);
	uartSendString("P");
	uartSendString(":");
	uartSendData(shiwan);
	uartSendData(wan);
	uartSendData(qian);
	uartSendString(".");
	uartSendData(bai);
	uartSendData(shi);
	uartSendString(" Kpa\r\n");
}

//*********************************************************
//******主程序********
//*********************************************************
void main()
{
	delay(50);	                  //上电延时		
	uartInit();                   //uart初始化
	delay(20);
	uartSendString("uart INIT");
	Init_BMP085();              //初始化BMP085 
	while(1)                      //循环
	{ 
		bmp085Convert();
		delay(1000);
	}
} 