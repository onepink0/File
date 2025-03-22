/*
	仅包含数码管和按键(使用定时器2)
	无超声波其他模块。
	
	Finish:2024/5/25  21:40
*/

#include <STC15F2K60S2.H>
#include "configure.h"
/************参数区*****************/
u8 key_down_t = 0;	//扫描一次按键所需时间
u8 key_old,key_val,key_down,key_up;
u8 mode=1;//界面


/************函数区*****************/
u8 key_read()	//按键响应函数
{				//矩阵按键
	u8 temp = 0;
	ET0 = 0;
	P35=0;P34=1;
	if(P32==0)	temp=13;
	if(P33==0)	temp=12;
	if(P32==0&&P33==0)	temp=23;	//处理双按键、s12+s13同时按下
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
	key_val = key_read();	//三行按键的处理
	key_down = key_val&(key_val^key_old);
	key_up 	= ~key_val&(key_val^key_old);
	key_old = key_val;
	
	if(key_down==12)	//S12按下有效，界面切换1、2
	{
		mode++;
		if(mode==3)	mode=1;
	}
	if(key_up==12)		//S12松开有效
	{
		
	}
	//......
}
void smg_work()
{
	if(mode==1)//界面1
	{	//显示为：123  1.23
		smg_data[0] = SMG_NoDot[1];
		smg_data[1] = SMG_NoDot[2];
		smg_data[2] = SMG_NoDot[3];
		smg_data[3] = 0xff;	smg_data[4] = 0xff;//0xff显示空白
		smg_data[5] = SMG_Dot[1];
		smg_data[6] = SMG_NoDot[2];
		smg_data[7] = SMG_NoDot[3];
	}
	if(mode==2)//界面2
	{
		
	}
	//......
}
/*
	使用定时器2：
	先isp生成，再添加最后两行就可；
	16位自动装载，12T，和定时器0和1一样使用
*/
void Timer2Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;
	T2L = 0x18;
	T2H = 0xFC;
	AUXR |= 0x10;
	IE2 |=0x04;		
	EA = 1;
}
void system_init()//初始化函数
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
