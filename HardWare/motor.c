#include "motor.h"

uint8_t Motor1_EN = 0;//电机使能
uint8_t Motor2_EN = 0;

static motor_dir_t direction1  = MOTOR_FWD;     // 记录电机1方向
static uint16_t    dutyfactor1 = 0;             // 记录电机1占空比
static motor_dir_t direction2  = MOTOR_FWD;     // 记录电机2方向
static uint16_t    dutyfactor2 = 0;             // 记录电机2占空比

/**********************电机的控制函数*************************/
void Motor_Output(int nMotor1Pwm, int nMotor2Pwm)
{
    // 电机1处理
    motor_dir_t dir1 = (nMotor1Pwm >= 0) ? MOTOR_FWD : MOTOR_REV;
    uint16_t speed1 = (uint16_t)abs(nMotor1Pwm);
    speed1 = (speed1 > PWM1_MAX_PERIOD_COUNT) ? PWM1_MAX_PERIOD_COUNT : speed1;
    Set_Motor1(dir1, speed1);  // 统一设置方向和速度

    // 电机2处理
    motor_dir_t dir2 = (nMotor2Pwm >= 0) ? MOTOR_FWD : MOTOR_REV;
    uint16_t speed2 = (uint16_t)abs(nMotor2Pwm);
    speed2 = (speed2 > PWM2_MAX_PERIOD_COUNT) ? PWM2_MAX_PERIOD_COUNT : speed2;
    Set_Motor2(dir2, speed2);  // 统一设置方向和速度
}

/**********************电机1驱动函数（优化后）*************************/
/**
  * @brief  设置电机1方向和速度
  * @param  dir: 方向 (MOTOR_FWD 或 MOTOR_REV)
  * @param  speed: 速度（占空比）
  * @retval 无
  */
void Set_Motor1(motor_dir_t dir, uint16_t speed)
{
    direction1 = dir;
    dutyfactor1 = speed;
    
    if (direction1 == MOTOR_FWD) {
        SET1_REV_COMPAER(0);         // 关闭反转通道
        SET1_FWD_COMPAER(dutyfactor1); // 设置正转PWM
    } else {
        SET1_FWD_COMPAER(0);         // 关闭正转通道
        SET1_REV_COMPAER(dutyfactor1); // 设置反转PWM
    }
}

/**********************电机2驱动函数*************************/
/**
  * @brief  设置电机2方向和速度
  * @param  dir: 方向 (MOTOR_FWD 或 MOTOR_REV)
  * @param  speed: 速度（占空比）
  * @retval 无
  */
void Set_Motor2(motor_dir_t dir, uint16_t speed)
{
    direction2 = dir;
    dutyfactor2 = speed;
    
    if (direction2 == MOTOR_FWD) {
        SET2_REV_COMPAER(0);         // 关闭反转通道
        SET2_FWD_COMPAER(dutyfactor2); // 设置正转PWM
    } else {
        SET2_FWD_COMPAER(0);         // 关闭正转通道
        SET2_REV_COMPAER(dutyfactor2); // 设置反转PWM
    }
}

/**********************电机使能函数*************************/
/**
  * @brief  使能电机1
  * @param  无
  * @retval 无
  */
void Set_Motor1_Enable(void)
{
		Motor1_EN  = 1;
		MOTOR1_FWD_ENABLE();
		MOTOR1_REV_ENABLE();
}

/**
  * @brief  禁用电机1
  * @param  无
  * @retval 无
  */
void Set_Motor1_Disable(void)
{
		Motor1_EN  = 0;
		MOTOR1_FWD_DISABLE();
		MOTOR1_REV_DISABLE();
}

/**
  * @brief  使能电机2
  * @param  无
  * @retval 无
  */
void Set_Motor2_Enable(void)
{
		Motor2_EN  = 1;
		MOTOR2_FWD_ENABLE();
		MOTOR2_REV_ENABLE();
}

/**
  * @brief  禁用电机2
  * @param  无
  * @retval 无
  */
void Set_Motor2_Disable(void)
{
		Motor2_EN  = 0;
		MOTOR2_FWD_DISABLE();
		MOTOR2_REV_DISABLE();
}

