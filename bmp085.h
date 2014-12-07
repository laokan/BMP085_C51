#ifndef __BMP085_H
#define __BMP085_H

#define	BMP085_SlaveAddress   0xee	  //bmp085 i2c address
#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)

#include "typedef.h"


void delay(unsigned int k);                                                                        
//------------------------------------
void Init_BMP085();
//void BMP085_Start();
//void BMP085_Stop();
//void BMP085_SendACK(bit ack);
//bit  BMP085_RecvACK();
//void BMP085_SendByte(BYTE dat);
//BYTE BMP085_RecvByte();
//long bmp085ReadTemp(void);
//long bmp085ReadPressure(void);
long *bmp085ConvertAll();
//-----------------------------------
#endif