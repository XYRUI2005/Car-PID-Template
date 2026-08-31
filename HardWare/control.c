#include "control.h"

/* 定义全局变量 */
float Motor1_Journey_cm = 0;    //电机1走过的路程
float Motor2_Journey_cm = 0;    //电机2走过的路程

float g_fTargetJourney = 0;  		//存放小车左右轮走的路程,单位cm，需要在下一阶段任务中设置(Car_Go),用于判断车子到达目标位置

uint8_t Stop_Count = 0;   //停止计时，用于判断是否停车
uint8_t Spin_Count = 0;   //旋转计时，用于判断是否完成旋转

uint8_t Location_Control_Count = 0;  //执行频率不需要那么高的用这个事件计数，用在中断中，控制串级pid内外环执行频率
uint8_t Turn_Angle_Control_Count = 0;//执行频率不需要那么高的用这个事件计数，用在中断中，控制串级pid内外环执行频率
uint8_t Line_Speed_Control_Count = 0;//执行频率不需要那么高的用这个事件计数，用在中断中，控制串级pid内外环执行频率

float Speed1_Outval = 0;      //电机1速度环输出值
float Location1_Outval = 0;   //电机1位置环输出值
float Speed2_Outval = 0;      //电机2速度环输出值
float Location2_Outval = 0;   //电机2位置环输出值
float Turn_Outval = 0;        //电机差速输出值，用于角度环
float Speed3_Outval = 0;      //电机1转向速度环输出值
float Speed4_Outval = 0;      //电机2转向速度环输出值
float Line_Outval = 0;        //巡线差值量

int Motor1_PWM = 0;     //电机1装载pwm值
int Motor2_PWM = 0;     //电机2装载pwm值

uint8_t Line_Flag = 0;            //巡线标志位,0不巡线,1巡线
uint8_t Stop_Flag = 0;            //停止标志位,0行驶,1停止
uint8_t Spin_Start_Flag = 0;      //转向开始标志位
uint8_t Spin_Succeed_Flag = 0;    //转向结束标志位
uint8_t Turn_Flag = 0;            //转向标志位,好像没用到
uint8_t Angle_Flag = 0;           //角度环调试标志位

extern uint8_t Run_Mode;//运行哪个模式


/****************** 定时器回调函数 ******************/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim6)//PID控制在这里进行,TIM6定时周期20ms
	{
		/* 读取脉冲 更新累计脉冲 */
		Get_Motor_Pulse();
		
		//有了巡线环之后，小车直线距离可能需要补偿，根据寻直线的效果来定。  3.14即可补偿？巡线效果不好（不直）的话可以继续减小这个pi的值
		Motor1_Journey_cm = (Motor1_PulseSigma / (REDUCTION_RATIO*ENCODER_TOTAL_RESOLUTION)) * (WHEEL_D*3.1416);
		Motor2_Journey_cm = (Motor2_PulseSigma / (REDUCTION_RATIO*ENCODER_TOTAL_RESOLUTION)) * (WHEEL_D*3.1416);
	
//		/******上位机调试速度环PID时用，实际使用时注释掉******/
//		if(Motor1_EN == 1 && Motor2_EN == 1)
//		{
//				/* pid控制 */
//				Motor1_PWM = Speed1_PID_Control();  
//				Motor2_PWM = Speed2_PID_Control();
//				
//				/* 装载 */
//				Motor_Output(Motor1_PWM,Motor2_PWM);
//		}
		
//		/******上位机调试位置速度串级PID时用，实际使用时注释掉******/
//		if(Motor1_EN == 1 && Motor2_EN == 1)
//		{
//				/* pid控制 */
//				Location_Speed_Control();
//				Motor1_PWM = Speed1_Outval;  
//				Motor2_PWM = Speed2_Outval;
//				
//				/* 装载 */
//				Motor_Output(Motor1_PWM,Motor2_PWM);
//		}
		
//			/****** 上位机调试转向角度速度串级PID时用，实际使用时注释掉 ******/
//			if(Motor1_EN == 1 && Motor2_EN == 1)
//			{
//					/* pid控制 */
//					Turn_Angle_Speed_Control();
//					Motor1_PWM = Speed3_Outval;  
//					Motor2_PWM = Speed4_Outval;
//					
//					/* 装载 */
//					Motor_Output(Motor1_PWM,Motor2_PWM);
//			}
			
//			/****** 上位机调试巡线速度串级PID时用，实际使用时注释掉 ******/
//			if(Motor1_EN == 1 && Motor2_EN == 1)
//			{
//					/* pid控制 */
//					Line_Speed_Control();
//					
//					Motor1_PWM = Speed3_Outval;  
//					Motor2_PWM = Speed4_Outval;
//					
//					/* 装载 */
//					Motor_Output(Motor1_PWM,Motor2_PWM);
//			}
			
//			/****** 上位机调试角度速度串级PID时用，实际使用时注释掉 ******/
//			if(Motor1_EN == 1 && Motor2_EN == 1 && Turn_Flag==1)
//			{
//					/* pid控制 */
//					Angle_Speed_Control();
//					
//					Motor1_PWM = Speed3_Outval;  
//					Motor2_PWM = Speed4_Outval;
//					
//					/* 装载 */
//					Motor_Output(Motor1_PWM,Motor2_PWM);
//			}
		
	}
}

/******************小车控制函数******************/

/**
 * @brief       小车直行一定距离函数
 * @param       要走的路程
 * @retval      无
 */
/****走完后自动将 Stop_Flag置1****/
/*这里加将位置环的输入直接编程距离(cm)的函数 */   //所有普通直行都用这个
void Car_Go(int32_t distance_cm)   //直走函数     //连续两次的直行好像会让巡线补偿失效
{
	float target_pluse;//目标距离对应的脉冲值
	
	/* 设置对应标志位 */
	Line_Flag = 1;   
	Stop_Flag = 0;
	
	Spin_Start_Flag = 0;
	Spin_Succeed_Flag = 0;  
	
	/* 清除上次走的路程记录 */
	Motor1_Journey_cm = 0;    //之前没清除所以用不了
	Motor2_Journey_cm = 0;
	Motor1_PulseSigma = 0;
	Motor2_PulseSigma = 0; 
	
	g_fTargetJourney = distance_cm;   //防止长时间PID控制用
	
	/* 将目标距离转换为对应的脉冲值，设为位置环期望 */
	//如果巡线效果不好就将3.142加大
	target_pluse = (distance_cm / (WHEEL_D * 3.1416) ) * (REDUCTION_RATIO*ENCODER_TOTAL_RESOLUTION);  //将distance_cm 转换为对应的脉冲数脉冲数 
	//小车位置直接用一个电机的脉冲数累积就好，转向时不计数，开始一个位置前直接清零。   
	//打滑导致一个轮比另一个轮转位置不一样咋办,用“巡线环“弥补就好，转向就用“转向环”
	/* 设为位置环期望 */
	set_pid_target(&pid_location1,target_pluse);   
	set_pid_target(&pid_location2,target_pluse);
	
	/* 使能电机 */
	Set_Motor1_Enable();   //使能电机1控制PWM输出    
	Set_Motor2_Enable();   //使能电机2控制PWM输出
}


/**
 * @brief       小车旋转控制函数
 * @param       left_90左转90度,right_90右转90度,back_180掉头
 * @retval      无
 */
/******转向完成时自动将  Spin_Start_Flag == 0 、 Spin_Succeed_Flag  ********/
/*转角有左转90，右转90，和转180三种情况。*/
void Car_Spin(spin_dir direction) //原地转向可以直接 调用这个  //其实转向环并不需要很精准，转弯后直接用直走时的巡线函数回正车身就好
{
	float spin90_cm;       //自旋90度的距离
	float car_turn_pluse;  //小车旋转对应的脉冲数
	
	/* 设置对应标志位 */
	Line_Flag = 0;  //不进行巡线的补偿了
	Stop_Flag = 0;   //执行转弯时，将直走完成的标志位清零. 即如果上一次是直行，这次是转弯，则不用在业务代码里手动置位
	
	Spin_Start_Flag = 1;   
	Spin_Succeed_Flag = 0; 
	
	/* 清除上次路程 */
	Motor1_Journey_cm = 0;
	Motor2_Journey_cm = 0;
	Motor1_PulseSigma = 0;   
	Motor2_PulseSigma = 0;  //之前没清除所以用不了
	
	/* 自旋90度的距离 */
	spin90_cm = 0.25*3.1416*LUN_JU; //以车中心为圆心，小车两轮距离为直径，小车旋转90度即为圆的1/4
	
	/****转弯后不循线可以调转向系数**********/
	if(direction == left_90)  //需要往左边走
	{
			car_turn_pluse = (spin90_cm / (WHEEL_D*3.142)) * (REDUCTION_RATIO*ENCODER_TOTAL_RESOLUTION); 
			car_turn_pluse = 0.97 * car_turn_pluse;   //90*0.94 = 84.6   //惯性影响，导致转弯比理论设定的多。直接设90度接下来的巡线就寻不回来了
	}
	else if(direction == right_90)  //需要往右边走
	{
			car_turn_pluse = -(spin90_cm / (WHEEL_D*3.142)) * (REDUCTION_RATIO*ENCODER_TOTAL_RESOLUTION); 
			car_turn_pluse = 0.97 * car_turn_pluse;  //90*0.96 = 86.4    //惯性影响，导致转弯比理论设定的多。接下来的巡线可能就寻不回来了
	}
	else if(direction == back_180)
	{
			car_turn_pluse = -(spin90_cm / (WHEEL_D*3.142)) * (REDUCTION_RATIO*ENCODER_TOTAL_RESOLUTION);
			car_turn_pluse = 0.92 * car_turn_pluse;   //同理
			car_turn_pluse = 2 * car_turn_pluse;      //0.96*180 = 175.5
	}
	
	set_pid_target(&pid_location1, -car_turn_pluse);   
	set_pid_target(&pid_location2, car_turn_pluse);
	
	/* 使能电机 */
	Set_Motor1_Enable();   //使能电机1控制PWM输出    
	Set_Motor2_Enable();   //使能电机2控制PWM输出
}




/******************各PID******************/

/**
 * @brief        速度环位置环串级PID控制
 * @param       无
 * @retval      无
 */
void Location_Speed_Control(void)          
{
	if (Motor1_EN == 1 || Motor2_EN == 1)     // 电机在使能状态下才进行控制处理
	{
			/* 位置环控制 */
			Location_Control_Count++;
			if(Location_Control_Count >= 2)  //内环执行频率大于外环
			{ 
					Location_Control_Count = 0; 
					Location1_Outval = Location1_PID_Control();
					Location2_Outval = Location2_PID_Control();
			}

			/* 位置环输出作为速度环输入 */
			set_pid_target(&pid_speed1,Location1_Outval); //每次都必须有位置环的值     
			set_pid_target(&pid_speed2,Location2_Outval); //每次都必须有位置环的值       
			
			/* 速度环控制 */
			Speed1_Outval = Speed1_PID_Control();    //要是电机转向不符合预期，就在这两句里取反数值
			Speed2_Outval = Speed2_PID_Control();  
	}
}


/**
 * @brief       电机1位置环控制
 * @param       无
 * @retval      位置环的输出值
 */
float Location1_PID_Control(void)  
{
	float cont_val = 0.0; 
	int32_t actual_location;
	
	actual_location = Motor1_PulseSigma;   //1圈 = 1768个脉冲 = 34*13*4  //这里位置用圈数代替。

	cont_val = location_pid_realize(&pid_location1,actual_location);   
	
	//还没加串级PID之前，位置环的cont_val对应PWM。 改成串级PID后，位置换的cont_val对应目标速度
		 
	/* 目标速度上限处理 */
	if (cont_val > TARGET_SPEED_MAX)
	{
			cont_val = TARGET_SPEED_MAX;
	}
	else if (cont_val < -TARGET_SPEED_MAX)
	{
			cont_val = -TARGET_SPEED_MAX;
	}
	
	/* 上位机调试代码 */
//  #if defined(PID_ASSISTANT_EN)
//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1);    // 给通道 1 发送实际值
//  #endif

	return cont_val;
}

/**
 * @brief       电机1速度环控制
 * @param       无
 * @retval      速度环输出值
 */
float Speed1_PID_Control(void)  
{
	float cont_val = 0.0;           //当前控制值
	int32_t actual_speed;           //车轮实际转速度,单位rpm(转/min)
	
	/* 计算公式 */
	/* 
	编码器倍频后的总分辨率 = 编码器线数*倍频数------倍频数由编码器计数方式决定
	单位时间内脉冲/(编码器倍频后的总分辨率*电机减速比)-----一个控制周期转的圈数
	单位时间内脉冲/(编码器倍频后的总分辨率*电机减速比*PID控制周期)-----每ms转的圈数
	单位时间内脉冲/(编码器倍频后的总分辨率*电机减速比*PID控制周期)*1000-----每s转的圈数
	单位时间内脉冲/(编码器倍频后的总分辨率*电机减速比*PID控制周期)*1000*60-----每min转的圈数
	*/
	/* 转速=单位时间内脉冲/(编码器倍频后的总分辨率*电机减速比*PID控制周期)*1000*60 */
	actual_speed = ((float)Motor1_Pulse*1000.0*60.0)/(ENCODER_TOTAL_RESOLUTION*REDUCTION_RATIO*SPEED_PID_PERIOD);

	cont_val = speed_pid_realize(&pid_speed1,actual_speed);    // 进行 PID 计算

	/* 上位机调试代码 */
//  #if defined(PID_ASSISTANT_EN)
//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1);                // 给通道 1 发送实际值
//  #endif
	
	return cont_val;
}

/**
 * @brief       电机2位置环控制
 * @param       无
 * @retval      位置环的输出值
 */
float Location2_PID_Control(void)  
{
	float cont_val = 0.0; 
	int32_t actual_location;
	
	actual_location = Motor2_PulseSigma;   //1圈 = 1768个脉冲 = 34*13*4  //这里位置用圈数代替。

	cont_val = location_pid_realize(&pid_location2,actual_location);   
	
	//还没加串级PID之前，位置环的cont_val对应PWM。 改成串级PID后，位置换的cont_val对应目标速度
		 
	/* 目标速度上限处理 */
	if (cont_val > TARGET_SPEED_MAX)
	{
			cont_val = TARGET_SPEED_MAX;
	}
	else if (cont_val < -TARGET_SPEED_MAX)
	{
			cont_val = -TARGET_SPEED_MAX;
	}
	
	/* 上位机调试代码 */
//    #if defined(PID_ASSISTANT_EN)
//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1);                // 给通道 1 发送实际值
//  #endif

	return cont_val;
}

/**
 * @brief       电机2速度环控制
 * @param       无
 * @retval      速度环输出值
 */
float Speed2_PID_Control(void)  
{
	float cont_val = 0.0;           //当前控制值
	int32_t actual_speed;           //车轮实际转速度,单位rpm(转/min)
	
	/* 计算公式 */
	/* 
	编码器倍频后的总分辨率 = 编码器线数*倍频数------倍频数由编码器计数方式决定
	单位时间内脉冲/(编码器倍频后的总分辨率*电机减速比)-----一个控制周期转的圈数
	单位时间内脉冲/(编码器倍频后的总分辨率*电机减速比*PID控制周期)-----每ms转的圈数
	单位时间内脉冲/(编码器倍频后的总分辨率*电机减速比*PID控制周期)*1000-----每s转的圈数
	单位时间内脉冲/(编码器倍频后的总分辨率*电机减速比*PID控制周期)*1000*60-----每min转的圈数
	*/
	/* 转速=单位时间内脉冲/(编码器倍频后的总分辨率*电机减速比*PID控制周期)*1000*60 */
	actual_speed = ((float)Motor2_Pulse*1000.0*60.0)/(ENCODER_TOTAL_RESOLUTION*REDUCTION_RATIO*SPEED_PID_PERIOD);

	cont_val = speed_pid_realize(&pid_speed2,actual_speed);    // 进行 PID 计算

	/* 上位机调试代码 */
//  #if defined(PID_ASSISTANT_EN)
//    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1);   // 给通道 2 发送实际值
//  #endif
	
	return cont_val;
}

/**
 * @brief       转向角度速度串级PID控制，角度外环的输出给到速度内环
 * @param       无
 * @retval      无
 */
void Turn_Angle_Speed_Control(void)
{
    if (Motor1_EN == 1 || Motor2_EN == 1)     // 电机在使能状态下才进行控制处理
    {
        Turn_Angle_Control_Count++;
        if(Turn_Angle_Control_Count >= 2)     //内环执行频率大于外环
        {
            Turn_Angle_Control_Count = 0; 
            /* 角度外环输出 输出一个值就可以给速度内环一正一负就可以实现转向 */
            Turn_Outval = Turn_Angle_PID_Control();
        }
        /* 角度外环输出给到速度内环输入 */  
        set_pid_target(&pid_speed1,-Turn_Outval); //如果极性错误将左右轮正负号取反
        set_pid_target(&pid_speed2,Turn_Outval); 
        Speed3_Outval = Speed1_PID_Control();   
        Speed4_Outval = Speed2_PID_Control();  
    }

}

/**
 * @brief       转向角度环控制
 * @param       无
 * @retval      角度环的输出值
 */
float Turn_Angle_PID_Control(void)
{
    float cont_val = 0.0; 
    int32_t actual_angle;
    
    actual_angle = yaw;  

    cont_val = turn_angle_pid_realize(&pid_turn_angle,actual_angle);   
    

    /* 目标速度上限处理 */
    if (cont_val > TARGET_SPEED_MAX)
    {
        cont_val = TARGET_SPEED_MAX;
    }
    else if (cont_val < -TARGET_SPEED_MAX)
    {
        cont_val = -TARGET_SPEED_MAX;
    }

//    #if defined(PID_ASSISTANT_EN)
//        set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_angle, 1);    // 给通道 1 发送实际值
//    #endif
    return cont_val;
}

/**
 * @brief       巡线速度串级PID实现，通过巡线实现差速转弯
 * @param       无
 * @retval      无
 */
void Line_Speed_Control(void)
{
    if (Motor1_EN == 1 || Motor2_EN == 1)     // 电机在使能状态下才进行控制处理
    {
        Line_Speed_Control_Count++;         //这个是巡线串级pid外环
        if(Line_Speed_Control_Count >= 2)
        {
            Line_Speed_Control_Count = 0; 
            Line_Outval = Line_PID_Control();
        }
        
        
        //这个的100是基础的速度加减g_turn_outval实现差速
        set_pid_target(&pid_speed1, TARGET_SPEED_MAX-Line_Outval); 
        set_pid_target(&pid_speed2, TARGET_SPEED_MAX+Line_Outval); 
        Speed3_Outval = Speed1_PID_Control();   
        Speed4_Outval = Speed2_PID_Control();  
    }
}

/**
 * @brief       巡线环控制
 * @param       无
 * @retval      巡线环的输出值，实际上是两个轮子的差速值
 */
float Line_PID_Control(void)  
{
   
    float cont_val = 0.0;                       // 当前控制值
    int32_t actual_speed;    
    
    //这里的实际值我给的是角度偏差的值，如果是灰度传感器巡线可以把传感器的值送进去
		if(TARGET_SPEED_MAX<=200)
			actual_speed = Line_Num;
		else
			actual_speed = Line_Num*2;

    cont_val = line_pid_realize(&pid_line, actual_speed);    // 进行 PID 计算
	
//    #if defined(PID_ASSISTANT_EN)
//        set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_angle, 1);    // 给通道 1 发送实际值
//    #endif  
	
    return cont_val;
}

/**
 * @brief       角度速度串级PID实现，通过巡线实现差速转弯
 * @param       无
 * @retval      无
 */
void Angle_Speed_Control(void)
{
    if (Motor1_EN == 1 || Motor2_EN == 1)     // 电机在使能状态下才进行控制处理
    {
        Line_Speed_Control_Count++;         //这个是巡线串级pid外环
        if(Line_Speed_Control_Count >= 2)
        {
            Line_Speed_Control_Count = 0; 
            Line_Outval = Angle_PID_Control();
        }
        
        
        //这个的100是基础的速度加减g_turn_outval实现差速
        set_pid_target(&pid_speed1, TARGET_SPEED_MAX-Line_Outval); 
        set_pid_target(&pid_speed2, TARGET_SPEED_MAX+Line_Outval); 
        Speed3_Outval = Speed1_PID_Control();   
        Speed4_Outval = Speed2_PID_Control();  
    }
}

/**
 * @brief       角度环控制
 * @param       无
 * @retval      角度环的输出值，实际上是两个轮子的差速值
 */
float Angle_PID_Control(void)  
{
   
    float cont_val = 0.0;                       // 当前控制值
    int32_t actual_speed;    
    
		actual_speed = yaw;
    //这里的实际值我给的是角度偏差的值，如果是灰度传感器巡线可以把传感器的值送进去
    cont_val = line_pid_realize(&pid_angle, actual_speed);    // 进行 PID 计算
	
//    #if defined(PID_ASSISTANT_EN)
//        set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_angle, 1);    // 给通道 1 发送实际值
//    #endif  
	
    return cont_val;
}

