#include "key.h"

uint8_t KEY_Count,KEY_Num,KEY_Flag;//按键计数值、按键号、按键状态标志位

void KEY_Scan(void)
{
	uint8_t KEY_Single_Num = KEY_Read();
	if(KEY_Single_Num != KEY_NONE_PRESSED)//如果有按键按下
	{
		KEY_Count++;          //systick那1ms进行一次扫描，则这里1ms加一次，乘上10就是20ms
		if(KEY_Count >= 20)   //如果相应更新中断的定时器的更新周期为1ms，那么这里就是得保持至少20ms的按键按下的电平
		{
			KEY_Count = 0;    //按键计数器清零
			if(KEY_Flag == NONE_PRESSED)   //判断有没有重按，1为有，0为没有
			{
				KEY_Flag = PRESSED;     //按键按下标志位置1
				KEY_Num = KEY_Single_Num;
			}
		}
		else KEY_Num = KEY_NONE_PRESSED;
	}
	else
	{
		KEY_Count = 0;      //按键计数值清零
		KEY_Flag = NONE_PRESSED;       //按键按下标志位置0
		KEY_Num = KEY_NONE_PRESSED;        //如果没有稳定按下10ms，则表示没有按下按键
	}
}

uint8_t KEY_Read(void)   //检测有无按键按下 
{
	if(KEY1==0)       return KEY1_PRESSED;
  else if(KEY2==0)  return KEY2_PRESSED;
	else if(KEY3==0)  return KEY3_PRESSED;
	
	return KEY_NONE_PRESSED;   //无按键按下
}




