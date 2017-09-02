/*-----------------------------------------------
  ���ƣ�ң�������������Һ����ʾ
  ���ݣ�������ң����������Һ����ʾ4����ֵ���ֱ����û��� �û��� ������ ���ݷ���
        ��ʾ���£�
		 BOLE-HORSE
		 Code:1E-1E-00-FF
------------------------------------------------*/
#include <reg52.h>  
#include "LCD12864.h"
#include "IR.h"
#include "beep.h"
#include "GSM.h"
sbit IR=P3^3;  //����ӿڱ�־
sbit IN1=P3^4;//����
sbit IN2=P3^5;
sbit IN3=P3^6;//�ҵ��
sbit IN4=P3^7;

//char code Tab[16]="0123456789ABCDEF";
unsigned char  irtime;//������ȫ�ֱ���
bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char irdata[33];
//unsigned char TempData[16];

/****************��ʱ��0�жϴ���*******************/

void tim0_isr (void) interrupt 1 using 1
{
  irtime++;  //���ڼ���2���½���֮���ʱ��
}

/******************�ⲿ�ж�1�жϴ���****************/
void EX1_ISR (void) interrupt 2 //�ⲿ�ж�1������
{
    static unsigned char  i;             //���պ����źŴ���
    static bit startflag;                //�Ƿ�ʼ�����־λ
    if(startflag)                         
    {
        if(irtime<63&&irtime>=33)//������ TC9012��ͷ�룬9ms+4.5ms
            i=0;
    		irdata[i]=irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬�����Ժ��ж���0����1
    		irtime=0;
    		i++;
   			if(i==33)
      	{
	  			 irok=1;
				   i=0;
	  		}
    }
  	else
		{
		    irtime=0;
		    startflag=1;
		}

}

/************��ʱ��0��ʼ��****************/
void TIM0init(void)//��ʱ��0��ʼ��
{
    TMOD|=0x02;//��ʱ��0������ʽ2��TH0����װֵ��TL0�ǳ�ֵ
    TH0=0x00; //����ֵ
    TL0=0x00; //��ʼ��ֵ
    ET0=1;    //���ж�
    TR0=1;    
}
/*******************�ⲿ�ж�1��ʼ*******************/
void EX1init(void)
{
    IT1 = 1;   //ָ���ⲿ�ж�1�½��ش�����INT1 (P3.3)
    EX1 = 1;   //ʹ���ⲿ�ж�
    EA = 1;    //�����ж�
}
/*********************��ֵ����**********************/

void Ir_work(void)
{
    /* TempData[0] = Tab[IRcord[0]/16];   //����ͻ���
	   TempData[1] = Tab[IRcord[0]%16];
     TempData[2] = '-';
	   TempData[3] = Tab[IRcord[1]/16];   //����ͻ���
	   TempData[4] = Tab[IRcord[1]%16];
	   TempData[5] = '-';
	   TempData[6] = Tab[IRcord[2]/16];   //����������
	   TempData[7] = Tab[IRcord[2]%16];
	   TempData[8] = '-';
	   TempData[9] = Tab[IRcord[3]/16];   //�������ݷ���
	   TempData[10] = Tab[IRcord[3]%16];*/
     if((IRcord[2]/16==4)&&(IRcord[2]%16==6))
		 {
			 
        IN1=1;
	      IN2=0;
	      IN3=1;
	      IN4=0;
			  //LCD_display(0x90,"С��״̬��ǰ��");
     }
	   if((IRcord[2]/16==1)&&(IRcord[2]%16==5))
		 {
			  
        IN1=0;
	      IN2=1;
	      IN3=0;
	      IN4=1;
			 //LCD_display(0x90,"С��״̬������");
     }
		 if((IRcord[2]/16==4)&&(IRcord[2]%16==4))
		 {
			 
        IN1=1;
	      IN2=1;
	      IN3=1;
	      IN4=0;
			  //LCD_display(0x90,"С��״̬����ת");
     }
		 if((IRcord[2]/16==4)&&(IRcord[2]%16==3))
		 {
			  
        IN1=1;
	      IN2=0;
	      IN3=1;
	      IN4=1;
			  //LCD_display(0x90,"С��״̬����ת");
     }
		 if((IRcord[2]/16==4)&&(IRcord[2]%16==0))
		 {
			  
        IN1=1;
	      IN2=1;
	      IN3=1;
	      IN4=1;
			  //LCD_display(0x90,"С��״̬��ͣ��");
     }
		 if((IRcord[2]/16==4)&&(IRcord[2]%16==5))//������
		 {
          LED_3W_OPEN();			  
     }
		 if((IRcord[2]/16==4)&&(IRcord[2]%16==7))//�س���
		 {
			    LED_3W_CLOSE();	
     }
		 if((IRcord[2]/16==0)&&(IRcord[2]%16==7))//����̽��״��
		 {
			    Send_Message();  
     }
	   irpro_ok=0;//������ɱ�־

  }
/*------------------------------------------------
                ������ֵ����
------------------------------------------------*/
void Ircordpro(void)//������ֵ������
{ 
    unsigned char i, j, k;
    unsigned char cord,value;
    k=1;
    for(i=0;i<4;i++)      //����4���ֽ�
    {
        for(j=1;j<=8;j++) //����1���ֽ�8λ
        {
            cord=irdata[k];
            if(cord>7)//����ĳֵΪ1������;����о��Թ�ϵ������ʹ��12M���㣬��ֵ������һ�����
                value|=0x80;
            if(j<8)
		        {
			          value>>=1;
			      }
                k++;
         }
     IRcord[i]=value;
     value=0;     
     } 
	   irpro_ok=1;//������ϱ�־λ��1
}

  