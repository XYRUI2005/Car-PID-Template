#include "menu.h"

extern uint8_t BMP1[];
extern uint8_t BMP2[];
extern uint8_t BMP3[];
extern uint8_t BMP4[];

//OLED菜单数据缓冲区
uint8_t str_buff1[64];
uint8_t str_buff2[64];
uint8_t str_buff3[64];
uint8_t str_buff4[64];   
uint8_t str_buff5[64];
uint8_t str_buff6[64];
uint8_t str_buff7[64];
uint8_t str_buff8[64];

uint8_t ITEM_Num;  //这个表示一开始就跳转哪个界面

void MENU_Display(uint8_t num)
{
	switch(num)//这里光放要显示的内容
	{
		case 0:
//	    OLED_ShowPicture(0, 0, 128, 64, BMP3,1);
			OLED_DrawLine(0,0,127,0,10);
			OLED_ShowString(10, 4, "Smart Car", 24 , 1);
			OLED_DrawLine(0,32,127,32,10);
			OLED_ShowString(20, 35, "<START>", 24 , 1);
		break;
		
		//选中一级界面 第一大选项
		case 1:
			OLED_ShowString(50, 0, "MENU", 16 , 1);
			OLED_DrawLine(0,16,127,16,10);
		  OLED_ShowString(20, 16+8, "MPU6050", 8 , 1);
		  OLED_ShowString(20, 32+8, "TASK", 8 , 1);
		  OLED_ShowString(20, 48+8, "TEST", 8 , 1);
		  OLED_ShowString(0, 16+8, ">>", 8 , 1);
		break;
		
		//第一大选项 二级界面
		case 10:
			OLED_ShowString(0, 0, "MPU6050", 16 , 1);
			OLED_ShowString(20, 16+8, "Gyroscope", 8 , 1);//陀螺仪
			OLED_ShowString(20, 32+8, "Accelerometer", 8 , 1);//加速度计
			OLED_ShowString(20, 48+8, "Euler angle", 8 , 1);//欧拉角
			OLED_ShowString(0, 16+8, ">>", 8 , 1);
		break;
		
	  case 11:
			OLED_ShowString(0, 0, "MPU6050", 16 , 1);
			OLED_ShowString(20, 16+8, "Gyroscope", 8 , 1);//陀螺仪
			OLED_ShowString(20, 32+8, "Accelerometer", 8 , 1);//加速度计
			OLED_ShowString(20, 48+8, "Euler angle", 8 , 1);//欧拉角
			OLED_ShowString(0, 32+8, ">>", 8 , 1);
	  break;
		 
	  case 12:
			OLED_ShowString(0, 0, "MPU6050", 16 , 1);
			OLED_ShowString(20, 16+8, "Gyroscope", 8 , 1);//陀螺仪
			OLED_ShowString(20, 32+8, "Accelerometer", 8 , 1);//加速度计
			OLED_ShowString(20, 48+8, "Euler angle", 8 , 1);//欧拉角
			OLED_ShowString(0, 48+8, ">>", 8 , 1);
	  break;
		 
		 //第一大选项三级界面
		case 110:
			OLED_ShowString(0, 0, "Gyroscope :", 16 , 1);
	
			sprintf((char *)str_buff1, "gyrox: %5.5f", gx_speed);
			sprintf((char *)str_buff2, "gyroy: %5.5f", gy_speed);
			sprintf((char *)str_buff3, "gyroz: %5.5f", gz_speed);
			
			OLED_ShowString(20, 16+4,str_buff1, 8 , 1);
			OLED_ShowString(20, 24+4,str_buff2, 8 , 1);
			OLED_ShowString(20, 32+4,str_buff3, 8 , 1);
		break;
				
		case 111 :
			OLED_ShowString(0, 0, "Accelerometer :", 16 , 1);
			
			sprintf((char *)str_buff1, "Accx: %6d", ax_speed);
			sprintf((char *)str_buff2, "Accy: %6d", ay_speed); 
			sprintf((char *)str_buff3, "Accz: %6d", az_speed);
			
			OLED_ShowString(20, 16+4,str_buff1, 8 , 1);
			OLED_ShowString(20, 24+4,str_buff2, 8 , 1);
			OLED_ShowString(20, 32+4,str_buff3, 8 , 1);		
		break;
			
		case 112:
			OLED_ShowString(0, 0, "Euler Angle :", 16 , 1);

			sprintf((char *)str_buff1, "pitch:%3.4f", pitch);
			sprintf((char *)str_buff2, "roll :%3.4f", roll);
			sprintf((char *)str_buff3, "yaw  :%3.4f", yaw);
			sprintf((char *)str_buff4, "temp :%3.4f", temperature_c);
			
			OLED_ShowString(20, 16+4,str_buff1, 8 , 1);
			OLED_ShowString(20, 24+4,str_buff2, 8 , 1);
			OLED_ShowString(20, 32+4,str_buff3, 8 , 1);
			OLED_ShowString(20, 44+4,str_buff4, 8 , 1);
		break;
		
		
		//第二大选项
		case 2:                                  //记得与前面主菜单界面保持一直
			OLED_ShowString(50, 0, "MENU", 16 , 1);
			OLED_DrawLine(0,16,127,16,10);
		  OLED_ShowString(20, 16+8, "MPU6050", 8 , 1);
		  OLED_ShowString(20, 32+8, "TASK", 8 , 1);
		  OLED_ShowString(20, 48+8, "TEST", 8 , 1);
		  OLED_ShowString(0, 32+8, ">>", 8 , 1);
		break;
		
		//第二大选项二级界面
		case 20:
			OLED_ShowString(0, 0, "TASK", 16 , 1);
			OLED_ShowString(20, 16+8, "TASK ONE", 8 , 1);
			OLED_ShowString(20, 32+8, "TASK TWO", 8 , 1);
			OLED_ShowString(20, 48+8, "TASK THREE", 8 , 1);
			OLED_ShowString(0, 16+8, ">>", 8 , 1);
		break;
		 
		case 21:
			OLED_ShowString(0, 0, "TASK", 16 , 1);
			OLED_ShowString(20, 16+8, "TASK ONE", 8 , 1);
			OLED_ShowString(20, 32+8, "TASK TWO", 8 , 1);
			OLED_ShowString(20, 48+8, "TASK THREE", 8 , 1);
			OLED_ShowString(0, 32+8, ">>", 8 , 1);
		break;
		 
		case 22:
			OLED_ShowString(0, 0, "TASK", 16 , 1);
			OLED_ShowString(20, 16+8, "TASK ONE", 8 , 1);
			OLED_ShowString(20, 32+8, "TASK TWO", 8 , 1);
			OLED_ShowString(20, 48+8, "TASK THREE", 8 , 1);
			OLED_ShowString(0, 48+8, ">>", 8 , 1);
		break;
		 
		case 23:
			OLED_ShowString(0, 0, "TASK", 16 , 1);
			OLED_ShowString(20, 16+8, "TASK TWO", 8 , 1);
			OLED_ShowString(20, 32+8, "TASK THREE", 8 , 1);
			OLED_ShowString(20, 48+8, "TASK ALL", 8 , 1);
			OLED_ShowString(0, 48+8, ">>", 8 , 1);
		break;
		 
		 //第二大选项三级界面    //任务界面的显示函数用一个函数封装起来， 以后调用不同任务用得上
		case 120:
			OLED_ShowString(0, 0, "TASK ONE :", 16 , 1);
			OLED_ShowString(20, 16+8, "", 16 , 1);              
		break;
				
		case 121 :
			OLED_ShowString(0, 0, "TASK TWO :", 16 , 1);
			OLED_ShowString(20, 16+8, "", 16 , 1);
		break;
			
		case 122:
			OLED_ShowString(0, 0, "TASK THREE :", 16 , 1);
			OLED_ShowString(20, 16+8, "", 16 , 1);
		break;
		
		case 123:
			OLED_ShowString(0, 0, "TASK ALL :", 16 , 1);
			OLED_ShowString(20, 16+8, "", 16 , 1);
		break;
		
		//第三大选项
		case 3:
			OLED_ShowString(50, 0, "MENU", 16 , 1);
			OLED_DrawLine(0,16,127,16,10);
		  OLED_ShowString(20, 16+8, "MPU6050", 8 , 1);
		  OLED_ShowString(20, 32+8, "TASK", 8 , 1);
		  OLED_ShowString(20, 48+8, "TEST", 8 , 1);
		  OLED_ShowString(0, 48+8, ">>", 8 , 1);
		break;
		
		//第三大选项二级界面
		case 30:
			OLED_ShowString(0, 0, "TEST", 16 , 1);
			OLED_ShowString(20, 16+8, "Motor", 8 , 1);
			OLED_ShowString(20, 32+8, "GreySensor", 8 , 1);
			OLED_ShowString(20, 48+8, "OpenMV", 8 , 1);
			OLED_ShowString(0, 16+8, ">>", 8 , 1);
		break;
		 
		case 31:
			OLED_ShowString(0, 0, "TEST", 16 , 1);
			OLED_ShowString(20, 16+8, "Motor", 8 , 1);
			OLED_ShowString(20, 32+8, "GreySensor", 8 , 1);
			OLED_ShowString(20, 48+8, "OpenMV", 8 , 1);
			OLED_ShowString(0, 32+8, ">>", 8 , 1);
		break;

		case 32:
			OLED_ShowString(0, 0, "TEST", 16 , 1);
			OLED_ShowString(20, 16+8, "Motor", 8 , 1);
			OLED_ShowString(20, 32+8, "GreySensor", 8 , 1);
			OLED_ShowString(20, 48+8, "OpenMV", 8 , 1);
			OLED_ShowString(0, 48+8, ">>", 8 , 1);
		break;

		case 33:
			OLED_ShowString(0, 0, "TEST", 16 , 1);
			OLED_ShowString(20, 16+8, "GreySensor", 8 , 1);
			OLED_ShowString(20, 32+8, "OpenMV", 8 , 1);
			OLED_ShowString(20, 48+8, "BlueTooth", 8 , 1);
			OLED_ShowString(0, 48+8, ">>", 8 , 1);
		break;
		 
		//第三大选项三级界面
		case 130:

		/*********测试编码器是否正常**************/

			OLED_ShowString(0, 0, "Motor:", 16,1);
			sprintf((char *)str_buff1, "M1_EncNum: %5d", Motor1_Pulse);
			sprintf((char *)str_buff2, "rpm:      %2.2f", ((float)Motor1_Pulse*1000.0*60.0)/(ENCODER_TOTAL_RESOLUTION*REDUCTION_RATIO*SPEED_PID_PERIOD));
			sprintf((char *)str_buff3, "M2_EncNum: %5d", Motor2_Pulse);
			sprintf((char *)str_buff4, "rpm:      %2.2f",((float)Motor2_Pulse*1000.0*60.0)/(ENCODER_TOTAL_RESOLUTION*REDUCTION_RATIO*SPEED_PID_PERIOD));

			OLED_ShowString(20, 16+4,str_buff1, 8 , 1);
			OLED_ShowString(20, 24+4,str_buff2, 8 , 1);
			OLED_ShowString(20, 32+4,str_buff3, 8 , 1);
			OLED_ShowString(20, 40+4,str_buff4, 8 , 1);

		break;
		 
		case 131:
			
			OLED_ShowString(0, 0, "GreySensor:", 16,1);

			sprintf((char *)str_buff1, "HW1:%d", HW1);
			sprintf((char *)str_buff2, "HW2:%d", HW2);
			sprintf((char *)str_buff3, "HW3:%d", HW3);
			sprintf((char *)str_buff4, "HW4:%d", HW4);
			sprintf((char *)str_buff5, "HW5:%d", HW5); 
			sprintf((char *)str_buff6, "HW6:%d", HW6);
			sprintf((char *)str_buff7, "HW7:%d", HW7); 
			sprintf((char *)str_buff8, "HW8:%d", HW8);

			OLED_ShowString(0, 16+8, str_buff1, 8,1);
			OLED_ShowString(0, 16+16,str_buff2, 8,1);
			OLED_ShowString(0, 16+24,str_buff3, 8,1);
			OLED_ShowString(0, 16+32,str_buff4, 8,1);

			OLED_ShowString(64, 16+8, str_buff5, 8,1);
			OLED_ShowString(64, 16+16,str_buff6, 8,1);
			OLED_ShowString(64, 16+24,str_buff7, 8,1);
			OLED_ShowString(64, 16+32,str_buff8, 8,1);

		break;

		case 132:
//			sprintf((char *)str_buff1, "TargetRoom: %c", TargetRoom);//TargetRoom
//			sprintf((char *)str_buff2, "OneTargetRoom: %c", 'X');//OneTargetRoom
//			sprintf((char *)str_buff3, "LoR: %d", LoR); //LoR
//			sprintf((char *)str_buff4, "Num: %d", Num);//Num
//			sprintf((char *)str_buff5, "FindTask: %d", FindTask);//FindTask
//			sprintf((char *)str_buff6, "Load_flag:%d", Load_flag);
//						 
			OLED_ShowString(0, 0, "OpenMV:", 16,1);
//			OLED_ShowString(0, 16+8 ,str_buff1, 8,1);
//			OLED_ShowString(0, 16+16,str_buff2, 8,1);
//			OLED_ShowString(0, 16+24,str_buff3, 8,1);
//			OLED_ShowString(64,16+24,str_buff4, 8,1);
//			OLED_ShowString(0, 16+32,str_buff5, 8,1);	
//			OLED_ShowString(0, 16+40,str_buff6, 8,1);
		break;

		case 133:
//			sprintf((char *)str_buff1, "%d", 0);//BlueRxBuffer[0]
//			sprintf((char *)str_buff2, "%d", 0); //BlueRxBuffer[1]
//			sprintf((char *)str_buff3, "%d", 0); //BlueRxBuffer[2]
//			sprintf((char *)str_buff4, "%d", 0);//BlueRxBuffer[3]
//			sprintf((char *)str_buff5, "%d", 0);//BlueRxBuffer[4]

			OLED_ShowString(0, 0, "BlueTooth:", 16,1);
//			OLED_ShowString(0, 16+8,str_buff1, 8,1);
//			OLED_ShowString(0, 16+16,str_buff2, 8,1);
//			OLED_ShowString(0, 16+24,str_buff3, 8,1);
//			OLED_ShowString(0, 16+32,str_buff4, 8,1);
//			OLED_ShowString(0, 16+40,str_buff5, 8,1);
		break;
	}
	OLED_Refresh();
}


void MENU_Item(void) //按键按下才给调参数，所以代码逻辑要放这里面
{
	switch(KEY_Num)
	{
		case KEY1_PRESSED:     //确定键
			OLED_Clear();
			switch(ITEM_Num)
			{
				case 1:
				case 2:
				case 3:
					ITEM_Num *= 10;   //这里以乘10操作来实现确认的效果
				break;
				
				//下面几个虽然可以都写到一起 ，但这样好像更明了，容易改
				case 10:      //第一大选项
				case 11:	
				case 12:
					ITEM_Num  += 100;   //这里以进百位操作来实现确认的效果
				break;
				
				case 20:      //第二大选项
				case 21:	
				case 22:
				case 23:
					ITEM_Num  += 100;   //这里以进百位操作来实现确认的效果
				break;
				
				case 30:      //第三大选项
				case 31:	
				case 32:
				case 33:
					ITEM_Num  += 100;   //这里以进百位操作来实现确认的效果
				break;
			}
		break;
		
		case KEY2_PRESSED: //选择键/调参键：加数，当到了最后一个选项后返回第一个选项，或者在最后一级界面去调参
			ITEM_Num++;
			OLED_Clear();
			//下面是控制ITEM_Num的switch
			switch(ITEM_Num)
			{
				case 4: 
					ITEM_Num  = 1; 
				break;
				
				//第一大选项一级界面
				case 13:
					ITEM_Num = 10;
				break;
				
				//第一大选项二级界面
				case 113:
					ITEM_Num = 110;
				break;
				
				//第二大选项一级界面
				case 24:
					ITEM_Num = 20;
				break;
				
				//第二大选项二级界面
				case 124:
					ITEM_Num = 120;
				break;
				
				//第三大选一级界面
				case 34:
					ITEM_Num = 30;
				break;
				
				//第三大选项二级界面
				case 134:
					ITEM_Num = 130;
				break;
			}
		break;
		
		//返回主菜单或者状态清零
		case KEY3_PRESSED:              //home键 
			OLED_Clear();
			switch(ITEM_Num)
			{
				case 1:
				case 2:
				case 3:
					ITEM_Num = 0;   //这里以乘10操作来实现确认的效果
				break;
				
				//下面几个虽然可以都写到一起 ，但这样好像更明了，容易改
				case 10:      //第一大选项
				case 11:	
				case 12:
					ITEM_Num  = 1;   //这里以进百位操作来实现确认的效果
				break;
				
				case 20:      //第二大选项
				case 21:	
				case 22:
				case 23:
					ITEM_Num  = 2;   //这里以进百位操作来实现确认的效果
				break;
				
				case 30:      //第三大选项
				case 31:	
				case 32:
				case 33:
					ITEM_Num  =3;   //这里以进百位操作来实现确认的效果
				break;
				
				//下面几个虽然可以都写到一起 ，但这样好像更明了，容易改
				case 110:      //第一大选项
				case 111:	
				case 112:
					ITEM_Num  = 10;   //这里以进百位操作来实现确认的效果
				break;
				
				case 120:      //第二大选项
				case 121:	
				case 122:
				case 123:
					ITEM_Num  = 20;   //这里以进百位操作来实现确认的效果
				break;
				
				case 130:      //第三大选项
				case 131:	
				case 132:
				case 133:
					ITEM_Num  = 30;   //这里以进百位操作来实现确认的效果
				break;
			}
		break;
	}
}
