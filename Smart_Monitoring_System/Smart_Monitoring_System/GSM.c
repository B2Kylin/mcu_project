/**************************************************
**�ļ����ƣ�ͨ��TC35I����Ӣ�Ķ���
**��������:����Ӣ�Ķ��Ų��裺
**                          1.����ATָ��
**                          2.����AT+CPMS=\"MT\",\"MT\",\"MT\" �������в�������MT��ģ���նˣ��н���
**                          3.����AT+CNMI=2��1���õ����¶��ŵ���ʱ��ʾ
**                          4.����AT+CMGF=1,�����ı�ģʽ
**                          5.����AT+CMGD=1,ɾ����һ������
**                          6.�жϽ����ַ������Ƿ��С�+CMTI�������б�ʾ���µĶ���
**                          7.����AT+CMGR=1�����ö�ȡ��Ϣָ��
**                          8.�жϽ�����Ϣ�Ƿ�һ�£�������Ӧ����
**                          9.����AT+CMGD=1��ɾ����ȡ�����Ϣ
**���ߣ�������
**ʱ�䣺2014��11-16
**************************************************/
#include <reg52.h>
#include <string.h>
#include "ds18b20.h"
#include "delay.h"
#include "LCD12864.h"
#include "DHT11.h"
#include "GSM.h"
#include "beep.h"
#include "LCDMenu.h"
#include "ADC0832.h"
#define uchar unsigned char
#define uint unsigned int
#define buff_num 20
/******************���ն���ָ��*****************************/
uchar code AT[]="AT\r";
uchar code CPMS[]={"AT+CPMS=\"MT\",\"MT\",\"MT\"\r"};
uchar code CNMI[]={"AT+CNMI=2,1\r"};
uchar code CMGF[]={"AT+CMGF=1\r"};
uchar code CMGR[]={"AT+CMGR=1\r"};
uchar code CMGD[]={"AT+CMGD=1\r"};
/********************�������Ķ���ָ��***********************/
uchar code MSXZ[]={"AT+CMGF=0\r"};//����ģʽѡ��
uchar code MSCS[]={"AT+CSMP=17,167,0,8\r"} ;//����PDUģʽ������8��ʾUnicode����
uchar code CSCS[]={"AT+CSCS=GSM\r"};
//uchar code MBSJ[]={"AT+CMGS=54\r"};//����PDU����
uchar code MBSJ[]={"AT+CMGS=84\r"};//����PDU����
//uchar code DXNR[]={"0891683108904105F011000B818123999721F50008A7"};//18329979125F   8123999721F5  
uchar code DXNR[]={"089168"};
//uchar Message_zxh[] ={"3108904105F0"};
uchar code DXNR3[]={"11000B81"};
//uchar code DXNR1[]={"8123999721F5"};
//uchar code phone[13]={'8','1','2','3','9','9','9','7','2','1','F','5','\0'};//�޸�2
//uchar phone[13] = {0};//�޸�3
uchar code DXNR2[]={"0008A7"};
/*****************ע������PDU���볤��/2��16���Ʊ���  42(72)***********/
uchar code MESS[]={"465F53524D68C06D4B73AF58836E295EA64E3AFF1A"};//��ǰ��⻷���¶�Ϊ��25��  ��PDU��   20
uchar code MESS3[]={"002C6E7F5EA64E3AFF1A"};//ʪ��Ϊ��   10
uchar code MESS4[] = {"002C70DF96FE6D535EA64E3AFF1A"};//����Ũ��Ϊ��14
uchar code MESS5[] = {"002C963276D76B635E383002"};//,��������    12
uchar code MESS6[] = {"002C67094EBA95EF51653002"};//���˴���
/**********************����0-9�ģУģձ���****************************/
//uchar code *num[] = {"0030","0031","0032","0033","0034","0035","0036","0037","0038","0039"};
uchar code num0[] = "0030";
uchar code num1[] = "0031";
uchar code num2[] = "0032";
uchar code num3[] = "0033";
uchar code num4[] = "0034";
uchar code num5[] = "0035";
uchar code num6[] = "0036";
uchar code num7[] = "0037";
uchar code num8[] = "0038";
uchar code num9[] = "0039";
uchar code MESS1[]={"5EA6"};//�ȵģУģձ���  2
uchar Rec_Buf[buff_num];
uchar i=0;
bit GSM_SR501_flag = 0;
sbit led1=P0^0;
//sbit led2=P0^1;
unsigned char FNum = 10,TNum = 5,RNum = 4;
/******************************************
**��������:void uart_int(void)
**��������:���ڳ�ʼ��
**����:void
**���:
******************************************/
void serial_int(void)
{
    SCON=0x50;//���пڹ�����ʽ1��REN=1�������пڽ�������
    PCON=0x00;//SMOD=0�������ʲ��ӱ�
    TMOD|=0x20;//T1��ʽ2
    TH1=0xFD;//װ��ֵ
    TL1=0xFD;//װ��ֵ
    TR1=1;//������ʱ��1
	ES=1;//���������ж�
	EA=1;//����ȫ���ж�
}
/******************************************
**��������:void send(uchar *tab)
**��������:����ָ������
**����:uchar *tab
**���:
******************************************/
void send(uchar *tab)
{
	ES=0;
    while((*tab)!='\0')
    {
        SBUF=*tab;
        while(TI==0);
        TI=0;
        tab++;
    }
	ES=1;
}
/******************************************
**��������:void CLR_Buf()
**��������:�����������
**����:��
**���:
******************************************/
void CLR_Buf()
{
    unsigned char k;
    for(k=0;k<buff_num;k++)
    {
        Rec_Buf[k]=0;
    }
    i=0;
}

/******************************************
**��������:bit hand(unsigned char *a)
**��������:�жϻ������Ƿ���ָ�����ַ���
**����:unsigned char *a
**���:bit 1����������   0����������������������
******************************************/
bit hand(unsigned char *a)
{
    if(strstr(Rec_Buf,a)!=NULL)
		return 1;
	else
		return 0;
}
/******************************************
**��������:void Send_num()
**��������:����0-9PDU����
**����:
**���:
******************************************/
void Send_num(uchar NUM)
{
//	switch(NUM)
//	{	
//		case 0: send(num[0]); delayms(1000); break;
//		case 1: send(num[1]); delayms(1000); break;
//		case 2: send(num[2]); delayms(1000); break;
//		case 3: send(num[3]); delayms(1000); break;
//		case 4: send(num[4]); delayms(1000); break;
//		case 5: send(num[5]); delayms(1000); break;
//		case 6: send(num[6]); delayms(1000); break;
//		case 7: send(num[7]); delayms(1000); break;
//		case 8: send(num[8]); delayms(1000); break;
//		case 9: send(num[9]); delayms(1000); break;	
//		default:send(num[0]); delayms(1000); 
//	}
	switch(NUM)
	{	
		case 0: send(num0); delayms(1000); break;
		case 1: send(num1); delayms(1000); break;
		case 2: send(num2); delayms(1000); break;
		case 3: send(num3); delayms(1000); break;
		case 4: send(num4); delayms(1000); break;
		case 5: send(num5); delayms(1000); break;
		case 6: send(num6); delayms(1000); break;
		case 7: send(num7); delayms(1000); break;
		case 8: send(num8); delayms(1000); break;
		case 9: send(num9); delayms(1000); break;	
		default:send(num0); delayms(1000); 
	}
}
/******************************************
**��������:void Send_Value()
**��������:���Ͳɼ�����
**����:
**���:
******************************************/
void Send_Value()
{
    uchar bai,shi;
	uint T_Value;
	T_Value=read_ds18b20temp();
	bai=T_Value%1000/100;
	shi=T_Value%100/10;
	write_LCD_command(0x90);
	write_LCD_data('T');
	write_LCD_data(0x30+bai);
	write_LCD_data(0x30+shi);
	Send_num(bai);
	Send_num(shi);
//	Send_num(1);
//	Send_num(2);
}
/******************************************
**��������:void Send_Value()
**��������:���Ͳɼ�ʪ������
**����:
**���:
******************************************/
void Send_Wetness_Value()
{
	uchar bai,shi;
	bai=RH_data/10%10;
	shi=RH_data%10;
	write_LCD_command(0x90+2);
	write_LCD_data('R');
	write_LCD_data(0x30+bai);
	write_LCD_data(0x30+shi);
	Send_num(bai);
	Send_num(shi);
//	Send_num(3);
//	Send_num(4);
}
void Send_fogValue()
{
	uchar bai,shi;
	uint fog_val = 0;
	//fog_val = ADC_val;
    write_LCD_command(0x90+4);
	write_LCD_data('F');
	fog_val = getvalue0832(0)*100/255;
	bai = fog_val/10;
	shi = fog_val%10;
//	shi = 0;
//	ge = 6;
	write_LCD_data(0x30+bai);
	write_LCD_data(0x30+shi);
	//bai = 0;
	//shi = 0;
	//write_LCD_data('A');
	Send_num(bai);
	//write_LCD_data('b');
	Send_num(shi);
	//write_LCD_data('c');
	
	
}


//void EC_num(uchar *phone)//�޸�
//{
//	uchar i;
//	/*
//	phone[0] = '1';
//	phone[1] = '8';
//	phone[2] = '3';
//	phone[3] = '2';
//	phone[4] = '9';
//	phone[5] = '9';
//	phone[6] = '7';
//	phone[7] = '9';
//	phone[8] = '1';
//	phone[9] = '2';
//	phone[10] = '5';
//	phone[11] = 'F';
//	phone[12] = '\0';
//	*/
//	for(i = 0; i < 6; i++)
//	{
//		
//		phone[2*i] ^= phone[2*i+1];
//		phone[2*i+1] ^= phone[2*i];
//		phone[2*i] ^= phone[2*i+1];
//	}
//	
//}





void Receive_Message_init()
{
    serial_int();//���ڳ�ʼ��
	CLR_Buf();//���������
	while(!hand("OK"))
	{
        send(AT);       // ����ATָ��
        delayms(500);
	}
	LCD_display(0x90,"                ");
	LCD_display(0x90,"GSM ������...   ");
	CLR_Buf();
	//led1=0;
	//delayms(1000);
    send(CPMS); //��������ģʽ��MT��ģ���նˣ��н���      
    delayms(500);
	while(!hand("OK"));
	CLR_Buf();
	//led2=0;
	//delayms(1000);
    send(CNMI);  //���õ����µĶ�����ʱ��ʾ     
    delayms(500);
	while(!hand("OK"));
	CLR_Buf();
	//led1=1;
	//delayms(1000);
	send(CMGF);//����Ϊ�ı�ģʽ       
    delayms(500);
	while(!hand("OK"));
	CLR_Buf();
	//led2=1;
	//delayms(1000);
	send(CMGD); //ɾ����һ������      
    delayms(500);
	while(!hand("OK"));
	CLR_Buf();
	//led1=0;
	//delayms(500);
	LCD_display(0x90,"                ");
	LCD_display(0x90,"GSM ����OK      ");
	delayms(2000);
	LCD_display(0x90,"                ");
//	LCD_display(0x80,"--���ܼ��ϵͳ--");
//	delayms(2000);
//	di();
//	di();
//	di();
//	di();
	
}
/******************************************
**��������:void Send_Message()
**��������:�������Ķ���
**����:
**���:
******************************************/
void Send_Message()
{
	uchar i;
	LCD_display(0x90,"                ");
	LCD_display(0x88,"���Ͷ�����...   ");
	LCD_display(0x80,"                ");
	LCD_display(0x98,"                ");
	delayms(2000);
    send(AT);       // 1.����ATָ��
    delayms(500);
    send(MSXZ);     //���Ͷ���ģʽѡ��2.����AT+CMGF=0  ����PDUģʽ
    delayms(500);
	led1 =0;
    send(MSCS);     //3.����AT+CSMP=17��167��0��8����PDUģʽ����
    delayms(500);
    send(CSCS);     //4.����AT+CSCS=GSM ����GSM�ַ���
    delayms(500);
	//led2=0;
    send(MBSJ);// 5.����AT+CMGS=40 ����PDU����
    delayms(500);
    send(DXNR);//���Ͷ�������,6.���Ͷ������ݣ���������ֻ�ܷ�������
	send(Message_zxh);
	LCD_display(0x80,Message_zxh);
	delayms(500);
	send(DXNR3);
	//send(DXNR1);
	//EC_num(phoneMenu);//�޸�4

	if(phoneMenu[0] == '1')
	{
		for(i = 0; i < 6; i++)
		{
		
			phoneMenu[2*i] ^= phoneMenu[2*i+1];
			phoneMenu[2*i+1] ^= phoneMenu[2*i];
			phoneMenu[2*i] ^= phoneMenu[2*i+1];
		}
}
	LCD_display(0x80,phoneMenu);
	delayms(1000);
	send(phoneMenu);//�޸�
	LCD_display(0x80,"������Ϣ�������");
	send(DXNR2);
    send(MESS);
	Send_Value();
	led1=1;
	LCD_display(0x80,"�¶���Ϣ�������");
	delayms(500);
	send(MESS1);
	delayms(500);
	send(MESS3);
	Send_Wetness_Value();
	LCD_display(0x80,"ʪ����Ϣ�������");
	send(MESS4);//����
	Send_fogValue();
	LCD_display(0x80,"������Ϣ�������");
	delayms(500);
	if(GSM_SR501_flag == 0)
		send(MESS5);//��������
	else
	{
		send(MESS6);//���˴���
		GSM_SR501_flag = 0;
	}
	LCD_display(0x80,"������Ϣ�������");
    delayms(500);
	LCD_display(0x80,"                ");
	//led2=1;
	
//	delayms(500);
//    delayms(500);
    SBUF=0x1A;
    //while(TI==0);//���TI=0��δ�����ꡣѭ���ȴ�
    //TI=0;//�ѷ����ꡣTI����
	//di();
	//di();
	delayms(2000);//������ʱ�ȴ����Ͷ������
	delayms(2000);//������ʱ�ȴ����Ͷ������
	LCD_display(0x90,"�������        ");
	LCD_display(0x88,"                ");
	
	Receive_Message_init();
}

/******************************************
**��������:void Receive_Message()
**��������:������Ϣ���������ݴ���
**����:
**���:
******************************************/
void Receive_Message()
{
	uint j;
	if(strstr(Rec_Buf,"+CMTI")!=NULL)//�������ַ����к��� ��+CMTI�� �ͱ�ʾ���µĶ���
    {
		CLR_Buf();
		delayms(1000);
		send(CMGR);//��ȡ��Ϣ
        delayms(1000);
        if(strstr(Rec_Buf,"report")!=NULL)//�����Ϣ����Ϊopen
		{
			//LCD_display(0x90,"���Ͷ�����");
			for(j=0;j<8;j++)
			{
				led1=0;
				delayms(100);
				led1=1;
				delayms(100);
			}
			FNum = 10;
			TNum = 5;
			RNum = 4;
            Send_Message();
		}
        CLR_Buf();
        delayms(500);
        send(CMGD);
		while(!hand("OK")); 
	    CLR_Buf();
       // Receive_Message_init();				
    }					
	
}
/******************************************
**��������:void serial()
**��������:�����жϴ���
**����:
**���:
******************************************/
void serial()interrupt 4
{
	ES=0;
	if(TI)
	{
		TI=0;
    }
	if(RI)
	{
		RI=0;
		Rec_Buf[i]=SBUF;
		i++;
		if(i>buff_num)
			i=0;
		ES=1;
    }
}

