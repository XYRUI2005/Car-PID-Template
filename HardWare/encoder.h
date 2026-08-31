#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f1xx_hal.h"
#include "tim.h"//包含tim头文件

extern long Motor1_PulseSigma;//电机1累计脉冲
extern long Motor2_PulseSigma;//电机2累计脉冲
extern short Motor1_Pulse;//电机1单位时间脉冲
extern short Motor2_Pulse;//电机2单位时间脉冲

void Get_Motor_Pulse(void);

#endif
