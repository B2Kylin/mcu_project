/******************************************
**��������:ds18b20���¶���ʾ
**�����ܣ�
**�������ߣ�������
**����ʱ�� ��2014.3.22
**����˵����
******************************************/
#include <reg52.h>
#include "delay.h"
#include "ds18b20.h"
#define uchar unsigned char
#define uint unsigned int
sbit DQ=P3^6;
uint tvalue;
bit tflag;

/******************************************
**��������:void ds18b20rst()
**����������ds18b20��ʼ��
**����:
**�����
**���ߣ�������
******************************************/
void ds18b20rst()
{
    DQ=1;
	  delayms(5);
	  DQ=0;
	  delayus(500);
	  DQ=1;
	  delayus(60);
}

/******************************************
**��������: uchar ds18b20read()
**������������ds18b20����
**����:
**�����dat
**���ߣ�������
******************************************/
uchar ds18b20read()
{
    uchar i=0;
	  uchar dat=0;//��dat�Ѷ����������ݴ�����
    for(i=0;i<8;i++)
	  {
        DQ=0;
			  dat=dat>>1;//���ƣ�������ʱ1us
			  DQ=1;
			  if(DQ)
					dat|=0x80;
				  delayus(15);
    }
		return(dat);	
}

/******************************************
**��������: uchar ds18b20write(uchar com)
**����������дds18b20����
**����:
**�����dat
**���ߣ�������
******************************************/
void ds18b20write(uchar com)
{
    uchar i=0;
	  for(i=0;i<8;i++)
	  {
        DQ=0;
			  DQ=com&0x01;
			  delayus(15);
			  DQ=1;
			  com=com>>1;  
    }
}
/******************************************
**��������: uint read ds18b20temp()
**������������ȡds18b20�¶�ֵ
**����:
**�����
**���ߣ�������
******************************************/
uint read_ds18b20temp()//ע�������ȡ16���ݲ�����uchar
{
    uchar a,b;
    ds18b20rst();
	  ds18b20write(0xcc);//������ROM
	  ds18b20write(0x44);//�����¶�ת��ָ��
	  ds18b20rst();
    ds18b20write(0xcc);//������ROM
    ds18b20write(0xbe);
    a=ds18b20read();//��18b20�Ͱ�λ
	  b=ds18b20read();//��18b20�߰�λ
	  tvalue=b;
	  tvalue=tvalue<<8;
	  tvalue|=a;
	  if(tvalue<0xffff)
			tflag=0;
		else
		{
      tvalue=~tvalue+1;
			tflag=1;
    }
	  tvalue=tvalue*0.625;//����10��
		return(tvalue);    	
}




