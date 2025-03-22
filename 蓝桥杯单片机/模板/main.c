/*
	����������ܺͰ���(ʹ�ö�ʱ��2)
	�޳���������ģ�顣
	
	Finish:2024/5/25  21:40
*/

#include <STC15F2K60S2.H>
#include "configure.h"
/************������*****************/
u8 key_down_t = 0;	//ɨ��һ�ΰ�������ʱ��
u8 key_old,key_val,key_down,key_up;
u8 mode=1;//����


/************������*****************/
u8 key_read()	//������Ӧ����
{				//���󰴼�
	u8 temp = 0;
	ET0 = 0;
	P35=0;P34=1;
	if(P32==0)	temp=13;
	if(P33==0)	temp=12;
	if(P32==0&&P33==0)	temp=23;	//����˫������s12+s13ͬʱ����
	P35=1;P34=0;
	if(P32==0)	temp=17;
	if(P33==0)	temp=16;
	ET0 = 1;
	return temp;
}
/****************************/
void key_work()
{
	if(key_down_t)	return;
	key_down_t=1;
	key_val = key_read();	//���а����Ĵ���
	key_down = key_val&(key_val^key_old);
	key_up 	= ~key_val&(key_val^key_old);
	key_old = key_val;
	
	if(key_down==12)	//S12������Ч�������л�1��2
	{
		mode++;
		if(mode==3)	mode=1;
	}
	if(key_up==12)		//S12�ɿ���Ч
	{
		
	}
	//......
}
void smg_work()
{
	if(mode==1)//����1
	{	//��ʾΪ��123  1.23
		smg_data[0] = SMG_NoDot[1];
		smg_data[1] = SMG_NoDot[2];
		smg_data[2] = SMG_NoDot[3];
		smg_data[3] = 0xff;	smg_data[4] = 0xff;//0xff��ʾ�հ�
		smg_data[5] = SMG_Dot[1];
		smg_data[6] = SMG_NoDot[2];
		smg_data[7] = SMG_NoDot[3];
	}
	if(mode==2)//����2
	{
		
	}
	//......
}
/*
	ʹ�ö�ʱ��2��
	��isp���ɣ������������оͿɣ�
	16λ�Զ�װ�أ�12T���Ͷ�ʱ��0��1һ��ʹ��
*/
void Timer2Init(void)		//1����@12.000MHz
{
	AUXR &= 0xFB;
	T2L = 0x18;
	T2H = 0xFC;
	AUXR |= 0x10;
	IE2 |=0x04;		
	EA = 1;
}
void system_init()//��ʼ������
{
	HC138_Init(4,0xff);
	HC138_Init(5,0x00);
	Timer2Init();
}

void main()
{
	system_init();
	while(1)
	{
		smg_work();
		key_work();
	}
}
void Timer2_Routine() interrupt 12
{
	if(++key_down_t==10)	key_down_t = 0;
	
	
	smg_bit();
}
