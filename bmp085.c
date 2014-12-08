#include  "stc15f204ea.h"
#include	"typedef.h"
#include	"bmp085.h"
#include	"delay.h"

sbit	  	SCL=P3^5;      //IIC SCL PIN
sbit 	  	SDA=P3^6;      //IIC SDA PIN

short ac1;
short ac2;
short ac3;
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1;
short b2;
short mb;
short mc;
short md;

/**************************************
起始信号
**************************************/
void BMP085_Start()
{
	SDA = 1;                    //拉高数据线
	SCL = 1;                    //拉高时钟线
	Delay5us();                 //延时
	SDA = 0;                    //产生下降沿
	Delay5us();                 //延时
	SCL = 0;                    //拉低时钟线
}

/**************************************
停止信号
**************************************/
void BMP085_Stop()
{
	SDA = 0;                    //拉低数据线
	SCL = 1;                    //拉高时钟线
	Delay5us();                 //延时
	SDA = 1;                    //产生上升沿
	Delay5us();                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void BMP085_SendACK(bit ack)
{
	SDA = ack;                  //写应答信号
	SCL = 1;                    //拉高时钟线
	Delay5us();                 //延时
	SCL = 0;                    //拉低时钟线
	Delay5us();                 //延时
}

/**************************************
接收应答信号
**************************************/
bit BMP085_RecvACK()
{
	SCL = 1;                    //拉高时钟线
	Delay5us();                 //延时
	CY = SDA;                   //读应答信号
	SCL = 0;                    //拉低时钟线
	Delay5us();                 //延时

	return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void BMP085_SendByte(BYTE dat)
{
	BYTE i;
	for (i=0; i<8; i++)         //8位计数器
	{
		dat <<= 1;              //移出数据的最高位
		SDA = CY;               //送数据口
		SCL = 1;                //拉高时钟线
		Delay5us();             //延时
		SCL = 0;                //拉低时钟线
		Delay5us();             //延时
	}
	BMP085_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
BYTE BMP085_RecvByte()
{
	BYTE i;
	BYTE dat = 0;

	SDA = 1;                    //使能内部上拉,准备读取数据,
	for (i=0; i<8; i++)         //8位计数器
	{
		dat <<= 1;
		SCL = 1;                //拉高时钟线
		Delay5us();             //延时
		dat |= SDA;             //读数据
		SCL = 0;                //拉低时钟线
		Delay5us();             //延时
	}
	return dat;
}

//*********************************************************
//读出BMP085内部数据,连续两个
//*********************************************************
short Multiple_read(uchar ST_Address)
{
	uchar msb, lsb;
	short _data;
	BMP085_Start();                          //起始信号
	BMP085_SendByte(BMP085_SlaveAddress);    //发送设备地址+写信号
	BMP085_SendByte(ST_Address);             //发送存储单元地址
	BMP085_Start();                          //起始信号
	BMP085_SendByte(BMP085_SlaveAddress+1);         //发送设备地址+读信号

	msb = BMP085_RecvByte();                 //BUF[0]存储
	BMP085_SendACK(0);                       //回应ACK
	lsb = BMP085_RecvByte();
	BMP085_SendACK(1);                       //最后一个数据需要回NOACK

	BMP085_Stop();                           //停止信号
	Delay5ms();
	_data = msb << 8;
	_data |= lsb;
	return _data;
}

//**************************************************************

//初始化BMP085，根据需要请参考pdf进行修改**************
void Init_BMP085()
{
	ac1 = Multiple_read(0xAA);
	ac2 = Multiple_read(0xAC);
	ac3 = Multiple_read(0xAE);
	ac4 = Multiple_read(0xB0);
	ac5 = Multiple_read(0xB2);
	ac6 = Multiple_read(0xB4);
	b1 =  Multiple_read(0xB6);
	b2 =  Multiple_read(0xB8);
	mb =  Multiple_read(0xBA);
	mc =  Multiple_read(0xBC);
	md =  Multiple_read(0xBE);
}
//********************************************************************
long bmp085ReadTemp(void)
{
	BMP085_Start();                  //起始信号
	BMP085_SendByte(BMP085_SlaveAddress);   //发送设备地址+写信号
	BMP085_SendByte(0xF4);	          // write register address
	BMP085_SendByte(0x2E);       	// write register data for temp
	BMP085_Stop();                   //发送停止信号
	delay(10);	// max time is 4.5ms
	
	return (long) Multiple_read(0xF6);
}
//*************************************************************
long bmp085ReadPressure(void)
{
	long pressure = 0;

	BMP085_Start();                       //起始信号
	BMP085_SendByte(BMP085_SlaveAddress); //发送设备地址+写信号
	BMP085_SendByte(0xF4);	              // write register address
	BMP085_SendByte(0x34);       	        // write register data for pressure
	BMP085_Stop();                        //发送停止信号
	delay(10);    	                      // max time is 4.5ms
	
	pressure = Multiple_read(0xF6);
	pressure &= 0x0000FFFF;
	
	return pressure;
}

long *bmp085ConvertAll()
{
	long ut,up;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;
	long ret[2];
	ut = bmp085ReadTemp();
	ut = bmp085ReadTemp();	   // 读取温度
	up = bmp085ReadPressure();
	up = bmp085ReadPressure();
	
	x1 = ((long)ut - ac6) * ac5 >> 15;
	x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	ret[0] = (b5 + 8) >> 4;

	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = (((long)ac1 * 4 + x3) + 2)/4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) up - b3) * (50000 >> OSS);
	if( b7 < 0x80000000)
		p = (b7 * 2) / b4 ;
	else
		p = (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	ret[1] = p + ((x1 + x2 + 3791) >> 4);
	return ret;
}