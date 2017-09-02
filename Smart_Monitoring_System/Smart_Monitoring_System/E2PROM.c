#include "I2C.h"
#include "E2PROM.h"

/*********************************************
**�������ܣ��������ַд������
**���룺unsigned char add д�����ݵĵ�ַ��0x00��0xFF �� 
**      unsigned char dat д����������ֵ
**�����void
*********************************************/
void Write_E2PROM_Add(unsigned char add,unsigned char dat)
{
	I2C_start();
	WriteByte(0xa0);//���ʹ��豸��ַ
	response();//�ȴ����豸��Ӧ
	WriteByte(add);//����оƬ�ڵ�ַ
	response();//�ȴ����豸��Ӧ
	WriteByte(dat);//��������
	response();//�ȴ����豸��Ӧ
	I2C_stop();
}

/***********************************************
**�������ܣ������õ�ַ�洢������ֵ
**���룺unsigned char add �����ݵĵ�ַ
**�����dat �õ�ַ�Ĵ洢������ֵ
***********************************************/
unsigned char Read_E2PROM_Add(unsigned char add)
{
	unsigned char dat;
	I2C_start();
	WriteByte(0xa0);//���ʹ��豸��ַ д����
	response();//�ȴ����豸��Ӧ
	WriteByte(add);//����оƬ�ڵ�ַ
	response();//�ȴ����豸��Ӧ
	I2C_start();
	WriteByte(0xa1);//���ʹ��豸��ַ ������
	response();//�ȴ����豸��Ӧ
	dat = ReadByte();//��ȡ����
	noresponse();
	I2C_stop();	
	return dat;
}