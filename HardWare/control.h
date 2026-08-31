#ifndef  __CONTROL_H__
#define  __CONTROL_H__

#include "main.h"
#include "tim.h"
#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include "line.h"
#include "protocol.h"
#include "led.h"
#include "mpu6050.h"
#include "dmp.h"

/* 小车物理特性 */
/* 编码器物理分辨率 */
#define ENCODER_RESOLUTION  13      //编码器线数

/* 经过倍频之后的总分辨率 */
#define ENCODER_TOTAL_RESOLUTION    (ENCODER_RESOLUTION * 4)  /* 4倍频后的总分辨率 */

/* 减速电机减速比 */
#define REDUCTION_RATIO  20

/*PID计算的周期，以毫秒为单位*/
#define SPEED_PID_PERIOD  20    //这个要看定时器6的中断周期

/* 小车速度限制 */
#define TARGET_SPEED_MAX  120  //单位rpm,如果实际使用位置环时两轮误差较大可以降低速度来改善，160rpm，1s能跑的最大距离53cm

/* 轮胎直径cm */
#define WHEEL_D  4.6     //单位cm,实际测量为准

/* 小车两轮距离 */
#define LUN_JU  13   //单位cm,实际测量为准

/* 小车轮胎到车头距离 */
#define HEAD_TO_WHEEL 15    //单位cm,实际测量为准

/* 外部变量声明 */
extern int Motor1_PWM;     //电机1装载pwm值
extern int Motor2_PWM;     //电机2装载pwm值

extern float Motor1_Journey_cm;      //电机1走过的路程
extern float Motor2_Journey_cm;      //电机2走过的路程

extern uint8_t Line_Flag;           //巡线标志位,0不巡线,1巡线
extern uint8_t Stop_Flag;           //停止标志位,0行驶,1停止
extern uint8_t Spin_Start_Flag;     //转向开始标志位
extern uint8_t Spin_Succeed_Flag;   //转向结束标志位
extern uint8_t Turn_Flag;           //转向标志位
extern uint8_t Angle_Flag;          //角度环调试标志位

/* 小车旋转选项 */
typedef enum
{
    left_90,
    right_90,
    back_180
}spin_dir;

/* 小车控制函数 */
void Car_Go(int32_t distance_cm);
void Car_Spin(spin_dir direction);

/* pid实现函数 */
void Location_Speed_Control(void);
float Speed1_PID_Control(void);
float Location1_PID_Control(void);
float Speed2_PID_Control(void);
float Location2_PID_Control(void);
float Turn_Angle_PID_Control(void);
void Turn_Angle_Speed_Control(void);
float Line_PID_Control(void);
void Line_Speed_Control(void);
void Angle_Speed_Control(void);
float Angle_PID_Control(void);
	
#endif
