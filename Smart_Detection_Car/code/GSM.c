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
#define uchar unsigned char
#define uint unsigned int
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
uchar code MBSJ[]={"AT+CMGS=54\r"};//����PDU����
uchar code DXNR[]={"0891683108904105F011000B818123999721F50008A7"};//18329979125F   8123999721F5              
uchar code MESS[]={"285F53524D68C06D4B73AF58836E295EA64E3AFF1A"};//��ǰ��⻷���¶�Ϊ��25��  ��PDU���
uchar code MESS3[]={"002C6E7F5EA64E3AFF1A"};
/**********************����0-9�ģУģձ���****************************/
uchar code num0[]={"0030"};
uchar code num1[]={"0031"};
uchar code num2[]={"0032"};
uchar code num3[]={"0033"};
uchar code num4[]={"0034"};
uchar code num5[]={"0035"};
uchar code num6[]={"0036"};
uchar code num7[]={"0037"};
uchar code num8[]={"0038"};
uchar code num9[]={"0039"};
uchar code MESS1[]={"5EA6"};//�ȵģУģձ���
uchar Rec_Buf[40];
uchar i=0;
uint T_Value;
sbit led1=P0^0;
sbit led2=P0^1;

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
    for(k=0;k<40;k++)
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
**��������:void Send_Value()
**��������:���Ͳɼ�����
**����:
**���:
******************************************/
void Send_Value()
{
    uchar bai,shi;
	  T_Value=read_ds18b20temp();
	  bai=T_Value%1000/100;
	  shi=T_Value%100/10;
	  write_LCD_command(0x90);
	  write_LCD_data('T');
	  write_LCD_data(0x30+bai);
	  write_LCD_data(0x30+shi);
	  if(bai==0)
		{
			send(num0);
			delayms(1000);
		}
		  
		if(bai==1)
		{
			send(num1);
			delayms(1000);
		}
		if(bai==2)
			{
			send(num2);
			delayms(1000);
		}
		if(bai==3)
			{
			send(num3);
			delayms(1000);
		}
		if(bai==4)
			{
			send(num4);
			delayms(1000);
		}
		if(bai==5)
			{
			send(num5);
			delayms(1000);
		}
		if(bai==6)
			{
			send(num6);
			delayms(1000);
		}
		if(bai==7)
			{
			send(num7);
			delayms(1000);
		}
		if(bai==8)
			{
			send(num8);
			delayms(1000);
		}
		if(bai==9)
			{
			send(num9);
			delayms(1000);
		}
		
		if(shi==0)
			{
			send(num0);
			delayms(1000);
		}
		if(shi==1)
			{
			send(num1);
			delayms(1000);
		}
		if(shi==2)
			{
			send(num2);
			delayms(1000);
		}
		if(shi==3)
			{
			send(num3);
			delayms(1000);
		}
		if(shi==4)
			{
			send(num4);
			delayms(1000);
		}
		if(shi==5)
			{
			send(num5);
			delayms(1000);
		}
		if(shi==6)
			{
			send(num6);
			delayms(1000);
		}
		if(shi==7)
			{
			send(num7);
			delayms(1000);
		}
		if(shi==8)
			{
			send(num8);
			delayms(1000);
		}
		if(shi==9)
			{
			send(num9);
			delayms(1000);
		}
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
	  //bai=5;
	  //shi=7;
	  write_LCD_command(0x90+4);
	  write_LCD_data('R');
	  write_LCD_data(0x30+bai);
	  write_LCD_data(0x30+shi);
	  if(bai==0)
		{
			send(num0);
			delayms(1000);
		}
		  
		if(bai==1)
		{
			send(num1);
			delayms(1000);
		}
		if(bai==2)
			{
			send(num2);
			delayms(1000);
		}
		if(bai==3)
			{
			send(num3);
			delayms(1000);
		}
		if(bai==4)
			{
			send(num4);
			delayms(1000);
		}
		if(bai==5)
			{
			send(num5);
			delayms(1000);
		}
		if(bai==6)
			{
			send(num6);
			delayms(1000);
		}
		if(bai==7)
			{
			send(num7);
			delayms(1000);
		}
		if(bai==8)
			{
			send(num8);
			delayms(1000);
		}
		if(bai==9)
			{
			send(num9);
			delayms(1000);
		}
		
		if(shi==0)
			{
			send(num0);
			delayms(1000);
		}
		if(shi==1)
			{
			send(num1);
			delayms(1000);
		}
		if(shi==2)
			{
			send(num2);
			delayms(1000);
		}
		if(shi==3)
			{
			send(num3);
			delayms(1000);
		}
		if(shi==4)
			{
			send(num4);
			delayms(1000);
		}
		if(shi==5)
			{
			send(num5);
			delayms(1000);
		}
		if(shi==6)
			{
			send(num6);
			delayms(1000);
		}
		if(shi==7)
			{
			send(num7);
			delayms(1000);
		}
		if(shi==8)
			{
			send(num8);
			delayms(1000);
		}
		if(shi==9)
			{
			send(num9);
			delayms(1000);
		}
}
void Receive_Message_init()
{
    serial_int();//���ڳ�ʼ��
	  CLR_Buf();//���������
	  while(!hand("OK"))
		{
        send(AT);       // ����ATָ��
        delayms(500);
		}
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
		LCD_display(0x90,"���ն���Waiting");
		di();
		di();
		di();
		di();
}
/******************************************
**��������:void Send_Message()
**��������:�������Ķ���
**����:
**���:
******************************************/
void Send_Message()
{
	  LCD_display(0x90,"                ");
	  LCD_display(0x88,"���Ͷ�����...");
    send(AT);       // 1.����ATָ��
    delayms(500);
    send(MSXZ);     //���Ͷ���ģʽѡ��2.����AT+CMGF=0  ����PDUģʽ
    delayms(500);
	  led1 =0;
    send(MSCS);     //3.����AT+CSMP=17��167��0��8����PDUģʽ����
    delayms(500);
    send(CSCS);     //4.����AT+CSCS=GSM ����GSM�ַ���
    delayms(500);
	  led2=0;
    send(MBSJ);// 5.����AT+CMGS=40 ����PDU����
    delayms(500);
    send(DXNR);//���Ͷ�������,6.���Ͷ������ݣ���������ֻ�ܷ�������
    send(MESS);
	  Send_Value();
	  led1=1;
	  send(MESS1);
	  delayms(500);
	  send(MESS3);
	  Send_Wetness_Value();
	  led2=1;
    delayms(500);
    SBUF=0x1A;
    //while(TI==0);//���TI=0��δ�����ꡣѭ���ȴ�
    //TI=0;//�ѷ����ꡣTI����
		di();
		di();
		LCD_display(0x90,"                ");
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
   // n=read_ds18b20temp();
	  //write_LCD_command(0x88);
	  //write_LCD_data('T');
	  //write_LCD_data(0x30+n%1000/100);
	  //write_LCD_data(0x30+n%100/10);
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
					      delayms(500);
				        led1=1;
				        delayms(500);
					  }
            Send_Message();
				}
				if(strstr(Rec_Buf,"openled")!=NULL)//�����Ϣ����Ϊopen
				{
            LED_3W_OPEN();
				}
				if(strstr(Rec_Buf,"closeled")!=NULL)//�����Ϣ����Ϊopen
				{
            LED_3W_CLOSE();
				}
        CLR_Buf();
        delayms(500);
        send(CMGD);
			  while(!hand("OK")); 
	      CLR_Buf();
        Receive_Message_init();				
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
			  if(i>40)
			      i=0;
			  ES=1;
    }
}

