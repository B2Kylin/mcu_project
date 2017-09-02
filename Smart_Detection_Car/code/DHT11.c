

/****************************************
DHT11��ʪ�ȴ�����
DHT=P3^7����������IO��
ʱ�� 2013.4.13
****************************************/

#include <reg52.h>
#include "DHT11.h"
#include "LCD12864.h"
#define uchar unsigned char
#define uint unsigned int

sbit DHT=P1^1;
uchar flag,temp;
uchar RH,RL,TH,TL,CK;
uchar RH_data,RL_data,TH_data,TL_data,CK_data;

/*void delay_us(unsigned int i)
{
    while(i--);
}*/
/************************
Լ8-10us�ӳٺ���
***********************/
void delay_us()
{
	uchar i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}
/*****************************
1ms�ӳٺ���
*****************************/
void delay_ms(uint t)
{
	 uchar i;
	 while(t--)
	 for(i=0;i<125;i++);
}

/************************************
���DHT11������������
*************************************/
uchar read_byte()
{
	uchar i,dat;
	for(i=0;i<8;i++)
	{
		    flag=2;
			while((DHT==0)&&flag++);
			delay_us();
			delay_us();
			delay_us();		          //���30us
			if(DHT==1)				  
			{
				temp=1;
				flag=2;
				while((DHT==1)&&flag++);
			}
			else
				temp=0;
				dat<<=1;
				dat|=temp;		
	}
	return dat;
}
/************************************
��λ����Ӧ����
*************************************/
void init_DHT11()
{
	DHT=0;
	delay_ms(20);	 //Լ20ms
	DHT=1;			 //����30us
	delay_us();
	delay_us();
	delay_us();
	DHT=1;
	if(DHT==0)
	{
		flag=2;
		while((DHT==0)&&flag++);
		flag=2;
		while((DHT==1)&&flag++);
		RH=read_byte();		   //ʪ����������
		RL=read_byte();		   //ʪ��С������
		TH=read_byte();		   //�¶���������
		TL=read_byte();		   //�¶�С������
		CK=read_byte();		   //У����
		DHT=1;				   //��������������
		CK_data=(RH+RL+TH+TL); //���У�����Ƿ���ȷ
		if(CK==CK_data)		   //�����ȷ ������һ������
		{
			RH_data=RH;
			RL_data=RL;
			TH_data=TH;
			TL_data=TL;
			CK_data=CK;
		}
	}
	  write_LCD_command(0x98+7);
	  //write_LCD_data('R');
	  write_LCD_data(0x30+RH_data/10%10);
	  write_LCD_data(0x30+RH_data%10);
}
