#include "encoder.h"

long Motor1_PulseSigma;//电机1累计脉冲
long Motor2_PulseSigma;//电机2累计脉冲
short Motor1_Pulse;//电机1单位时间脉冲
short Motor2_Pulse;//电机2单位时间脉冲

/*******************实际运行时读取编码器数值************************/
void Get_Motor_Pulse(void)//读取电机脉冲
{
	Motor1_Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim4));//获取计数器值
	Motor1_Pulse = Motor1_Pulse;   //测试下看看符不符合先
	
	__HAL_TIM_SET_COUNTER(&htim4,0);//TIM4计数器清零
	
	Motor2_Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim5));//获取计数器值
	Motor2_Pulse = -Motor2_Pulse;  //测试下看看符不符合先 

	//为什么只有在两个都取反之后，才能让两个串级PID有用？不然就会让位置环失效
	//暂时不要浪费时间去找原因,直接在内环（速度）的输出值那 数值取反 就好。
	
	__HAL_TIM_SET_COUNTER(&htim5,0);//TIM2计数器清零
	
	Motor1_PulseSigma += Motor1_Pulse;//位置外环使用的脉冲累积      //完成一次位置控制之后才清除。
	Motor2_PulseSigma += Motor2_Pulse;//位置外环使用的脉冲累积   		//记录了之后也要及时清零呀！！！
}
