#ifndef  __MOTOR_H__
#define  __MOTOR_H__

#include "stm32f1xx_hal.h"
#include <stdlib.h>
#include "main.h"
#include "tim.h"
#include "encoder.h"

/****************左边电机引脚初始化**************/
/* 设置电机1速度（占空比） */
#define SET1_FWD_COMPAER(ChannelPulse)         __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,ChannelPulse)    // 设置比较寄存器的值   //AIN2
#define SET1_REV_COMPAER(ChannelPulse)     		 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,ChannelPulse)    // 设置比较寄存器的值   //AIN1 

/* 使能电机1输出 */
#define MOTOR1_FWD_ENABLE()      HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);  
#define MOTOR1_REV_ENABLE()      HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);  

/* 禁用电机1输出 */
#define MOTOR1_FWD_DISABLE()     HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
#define MOTOR1_REV_DISABLE()     HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);

/****************右边电机引脚初始化**************/
/* 设置电机2速度（占空比）2 */     
#define SET2_FWD_COMPAER(ChannelPulse)       __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,ChannelPulse)    // 设置比较寄存器的值   //BIN2
#define SET2_REV_COMPAER(ChannelPulse)       __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,ChannelPulse)    // 设置比较寄存器的值   //BIN1

/* 使能电机2输出2 */
#define MOTOR2_FWD_ENABLE()                  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);   
#define MOTOR2_REV_ENABLE()                  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);    

/* 禁用电机2输出2 */
#define MOTOR2_FWD_DISABLE()                 HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
#define MOTOR2_REV_DISABLE()                 HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2); 


/* 累计 TIM_Period个后产生一个更新或者中断*/		
/* 当定时器从0计数到PWM_PERIOD_COUNT，即为PWM_PERIOD_COUNT+1次，为一个定时周期 */
#define PWM1_PERIOD_COUNT     (1000)     //可以尝试把这个变大，这样PID控制可以更顺滑
#define PWM2_PERIOD_COUNT     (1000)

/* 最大比较值 */
#define PWM1_MAX_PERIOD_COUNT (PWM1_PERIOD_COUNT - 20)    //如果PWM弄成了满的，一些驱动板就会出现问题（硬件上的原因）
#define PWM2_MAX_PERIOD_COUNT (PWM2_PERIOD_COUNT - 20)

/* 电机方向控制枚举 */
typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV,
}motor_dir_t;

extern uint8_t Motor1_EN;//电机使能
extern uint8_t Motor2_EN;

void Motor_Output(int nMotor1Pwm,int nMotor2Pwm);//设置电机电压和方向
void Set_Motor1(motor_dir_t dir, uint16_t speed);
void Set_Motor2(motor_dir_t dir, uint16_t speed);
void Set_Motor1_Enable(void);
void Set_Motor1_Disable(void);
void Set_Motor2_Enable(void);
void Set_Motor2_Disable(void);

#endif


