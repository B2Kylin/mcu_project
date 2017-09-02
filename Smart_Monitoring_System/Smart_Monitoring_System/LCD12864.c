/******************************************
**��������:LCD12864Һ������ʾ
**�����ܣ�
**�������ߣ�������
**����ʱ�� ��2014.4.4
**����˵����
******************************************/
#include <reg52.h>
#include "LCD12864.h"
#include "delay.h"
#define uchar unsigned char
#define uint  unsigned int	 
sbit RS=P0^5;
sbit RW=P0^6;
sbit EN=P0^7;	//Һ�����������ƶ� 


/******************************************
**��������:void read_busy()
**������������æ��־ 
**����:
**�����
**���ߣ�������
**˵��:��æ��־ ���λΪ1��busy�����ܽ��ж�д���� 
******************************************/
//void read_busy()
//{
//	RS=0;
//	RW=1;
//	EN=1;	//��ʱ�� 
//	while(P2 & 0x80); 
//	EN=0;
//}
/******************************************
**��������:void write_LCD_command(uchar value)
**����������д�����
**����:uchar value
**�����
**���ߣ�������
**˵��:
******************************************/
void write_LCD_command(uchar value)
{
	//read_busy();	//ÿ�ζ�д��Ҫ���ж�æ��־ 
	RS=0;
	RW=0;
	delayus(200);
	EN=1;		//EN��1--0�������� 
	P2=value;
	delayus(200);
	EN=0;
}
/******************************************
**��������:void write_LCD_data(uchar value)
**����������д���ݺ��� 
**����:uchar value
**�����
**���ߣ�������
**˵��:
******************************************/
void write_LCD_data(uchar value)
{
	//read_busy();
	RS=1;
	RW=0;
	delayus(200);
	EN=1;	
	P2=value;
	delayus(200);
	EN=0;
}

/******************************************
**��������:void init_LCD()
**����������8λ���ڷ�ʽLCD1864��ʼ������ 
**����:
**�����
**���ߣ�������
**˵��:
******************************************/
void init_LCD()	
{
	delayus(4000);			//�ȴ�ʱ��>40ms 
	write_LCD_command(0x30);//�����趨��8λ���ݡ�����ָ����� 
	delayus(100);				//�ȴ�ʱ��>100us 
	write_LCD_command(0x30);//�����趨��8λ���ݡ�����ָ����� 
	delayus(37);				//�ȴ�ʱ��>37us 
	write_LCD_command(0x0C);//��ʾ�趨��������ʾ���α�ء������� 
	delayus(100);				// �ȴ�ʱ��>100us 
	write_LCD_command(0x01);//����ָ�� 
	delayus(10000);			//�ȴ�ʱ��>10ms 
	write_LCD_command(0x06);//������趨����ַָ���1 
}
void LCD_display(unsigned char add,char *hz)
{
    write_LCD_command(add);
    delayus(5);
    while(*hz!='\0')
    {
        write_LCD_data(*hz);
        hz++;
        delayus(100);
    }
}
