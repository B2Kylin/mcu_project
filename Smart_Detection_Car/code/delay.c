/******************************************
**��������:void delayms(uchar x)
**������������ʱ���� ��ʱx ms
**����:uchar x
**�����
**���ߣ�������
******************************************/
#include "delay.h"
void delayms(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
/******************************************
**��������:void delayus(unsigned int x)
**������������ʱ���� ��ʱx ms
**����:uchar x
**�����
**���ߣ�������
******************************************/
void delayus(unsigned int i)
{
    while(i--);
}

