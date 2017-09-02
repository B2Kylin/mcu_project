#include <reg52.h>
#include "LCD12864.h"
#include "delay.h"
#include "IR.h"
#include "GSM.h"
#include "DHT11.h"
#include "ds18b20.h"
#include "beep.h"
void main(void)
{
    EX1init();            //��ʼ���ⲿ�ж�1
    TIM0init();           //��ʼ����ʱ��
    init_LCD();           //��ʼ��Һ��
    delayms(20);          //��ʱ�������ȶ�
	  LCD_display(0x80,"  ����̽��С��");
	  LCD_display(0x98,"�¶�:");
	  LCD_display(0x98+4,"ʪ��:");
	  Receive_Message_init();
	Send_Message(); //����
    while(1)//��ѭ��
    {
			  write_LCD_command(0x98+3);
			 //write_LCD_data('T');
	     write_LCD_data(0x30+read_ds18b20temp()%1000/100);
	     write_LCD_data(0x30+read_ds18b20temp()%100/10);
			 init_DHT11();
       if(irok)                        //������պ��˽��к��⴦��
	     {   
	         Ircordpro();
 	         irok=0;
	     }
       if(irpro_ok)                   //�������ú���й��������簴��Ӧ�İ�������ʾ��Ӧ�����ֵ�
	     {
	        Ir_work();
  	   }
			 if((read_ds18b20temp()%1000/100>=2)&&(read_ds18b20temp()%100/10>7))
			 {
				  di_di();
          Send_Message();  
       }
			 if((RH_data/10%10>=6)&&(RH_data%10>=0))
			 {
				  di_di();
          Send_Message();  
       }
			 Receive_Message();
			 
   }
}