#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "i2c.h"
#include "usart.h"
#include "delay.h"
#include "main.h"
#include "stdio.h"
#include "inv_mpu.h"

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}INT16_XYZ;

//三轴浮点型
typedef struct
{
	float X;
	float Y;
	float Z;
}FLOAT_XYZ;

void MPU6050_Init(void);
void MPU6050_Get(void);

void MPU6050_calibrate(void);

extern unsigned char IIC_buf[14];
extern float gx_speed;
extern float gy_speed;
extern float gz_speed;
	
extern signed short ax,ax_speed;
extern signed short ay,ay_speed;
extern signed short az,az_speed;

extern signed short gx;
extern signed short gy;
extern signed short gz;

extern signed short ax_cl;
extern signed short ay_cl;
extern signed short az_cl;

extern signed short gx_cl;
extern signed short gy_cl;
extern signed short gz_cl;

extern float temperature_c;

#define Gyro_Gain 	0.0609756f			  //角速度变成度 (初始化陀螺仪满量程+-2000 LSBg = 2*250/65535.0)
#define Gyro_Gr	  0.0010641f		  //角速度变成弧度(3.1415/180 * LSBg)  

#define ACCEL_SCALE 16384.0f  // ±2g量程时的灵敏度（LSB/g）
#define GRAVITY 9.80665f  // 标准重力加速度 (m/s2)

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)

#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)

#define	GYRO_CONFIG		0x1B	
//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
/*
0bxx<<3:
00 ~ +-250°
01 ~ +-500°
10 ~ +-1000°
11 ~ +-2000°
*/


#define	ACCEL_CONFIG	0x1C	
//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
/*
0bxx<<3:
00 ~ +-2g
01 ~ +-4g
10 ~ +-18g
11 ~ +-16g
*/

#define	ACCEL_XOUT_H	0x3B//加速度计测量值寄存器地址
#define	ACCEL_XOUT_L	0x3C 

#define	ACCEL_YOUT_H	0x3D //2 3
#define	ACCEL_YOUT_L	0x3E

#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
/********************/
#define	TEMP_OUT_H		0x41//温度测量值寄存器地址
#define	TEMP_OUT_L		0x42
/*********************/
#define	GYRO_XOUT_H		0x43// 8 9 陀螺仪测量值寄存器地址
#define	GYRO_XOUT_L		0x44
	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46

#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define MPU6050_ADDR  	0xD0

#define MPU6050_WHO_AM_I         0x75

#endif
