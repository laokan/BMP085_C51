//***************************************
// BMP085 IIC测试程序
// 使用单片机STC89C51 
// 晶振：22.1184M
// 编译环境 Keil uVision4
// 时间：2011年7月1日
// QQ： 41484807
//****************************************
#include  "stc15f204ea.h"
#include	"typedef.h"
#include	"uart.h"
#include	"bmp085.h"
//#include	"dht11.h"

unsigned char ge,shi,bai,qian,wan,shiwan;

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
	//long temperature,pressure;
	long *bmp085result;
	
	bmp085result = bmp085ConvertAll();
	conversion(bmp085result[0]);
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
	

	conversion(bmp085result[1]);
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
	//int *temp_value;
	delay(50);	                  //上电延时		
	uartInit();                   //液晶初始化
	delay(20);
	uartSendString("uart INIT");
	Init_BMP085();              //初始化BMP085 
	while(1)                      //循环
	{ 
		bmp085Convert();
		
		/*
		temp_value = getTempHumi();
		uartSendString("T2:");
		uartSendData(temp_value[0]);
		uartSendString(" \r\n");
		uartSendString("H:");	
		uartSendData(temp_value[1]);
		uartSendString(" \r\n");				
		*/
		delay(1000);
	}
} 