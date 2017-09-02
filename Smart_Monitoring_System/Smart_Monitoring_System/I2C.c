#include <reg52.h>
#include "I2C.h"
#include "delay.h"

sbit SCL = P0^2;
sbit SDA = P0^3;
/**************************************
**�������ܣ����߳�ʼ��
**���룺void
**�����void
**************************************/
void I2C_init()
{
	SCL = 1;
	delayus(5);
	SDA = 1;
	delayus(5);
}

/*************************************
**�������ܣ���ʼ�ź�
**���룺void
**�����void
*************************************/
void I2C_start()
{
	SDA = 1;
	SCL = 1;
	delayus(5);//��ʱ����4.7us
	SDA = 0;
	delayus(5);//��ʱ����4us
	SCL = 0;
}

/*************************************
**�������ܣ�Ӧ���ź�
**���룺void
**�����void
**************************************/
void response()
{
	unsigned int i;
	SDA = 0;
	SCL = 1;
	delayus(5);//��ʱ����4us
	while((SDA==1)&&(i<255))i++;//��û���յ��ӻ�Ӧ��Ĭ�ϴӻ��յ����ݲ��ٵȴ�Ӧ���ź�	
	SCL = 0;
	delayus(5);
}
/*************************************
**�������ܣ���Ӧ���ź�
**���룺void
**�����void
**************************************/
void noresponse()
{
	SDA = 1;
	SCL = 1;
	delayus(5);
	SCL = 0;
	SDA = 1;
}
/*************************************
**�������ܣ�д1byte����
**���룺unsigned char date
**�����void
**************************************/
void WriteByte(unsigned char dat)
{
	unsigned char i,temp;
	temp = dat;
	for(i = 0; i < 8; i++)
	{
		temp = temp<<1;
		SCL = 0;
		delayus(5);
		SDA = CY;
		delayus(5);
		SCL = 1;
		delayus(5); 
	}
	SCL = 0;//����SCL��Ϊ�´����ݴ�������׼��
	delayus(5);
	SDA = 1;//�ͷ�SDA����
	delayus(5);
}
/*************************************
**�������ܣ���1byte����
**���룺void
**�����unsigned char temp
**************************************/
unsigned char ReadByte()
{
	unsigned char i,temp;
	SCL = 0;
	delayus(5);
	SDA = 1;
	for(i = 0; i < 8; i++)
	{
		SCL = 1;
		delayus(5);
		temp = (temp<<1) | SDA;
		SCL = 0;
		delayus(5);
	}
	delayus(5);
	return temp;
}

/*************************************
**�������ܣ���ֹ�ź�
**���룺void
**�����void
**************************************/
void I2C_stop()
{
	SDA = 0;
	SCL = 1;
	delayus(5);//��ʱ����4us
	SDA = 1;
	delayus(5);//��ʱ����4.7us
	SCL = 0;
}



