#ifndef __GSM_H__
#define __GSM_H__

extern void serial_int(void);//���ڳ�ʼ��
extern void send(unsigned char *tab);//����ָ������
extern void CLR_Buf();//�����������
extern bit hand(unsigned char *a);//�жϻ������Ƿ���ָ�����ַ���
extern void Send_Value();//���Ͳɼ�����
extern void Send_Message();//�������Ķ���
extern void Receive_Message();//������Ϣ���������ݴ���
extern void serial();//�����жϴ���
extern void Receive_Message_init();
extern void Send_Wetness_Value();
#endif