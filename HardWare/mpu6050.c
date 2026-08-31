#include "mpu6050.h"

unsigned char IIC_buf[14];

signed short ax,ax_speed;
signed short ay,ay_speed;
signed short az,az_speed;

signed short gx;
signed short gy;
signed short gz;


float gx_speed=0;
float gy_speed=0;
float gz_speed=0;
	
signed short ax_cl = 0;
signed short ay_cl = 0;
signed short az_cl = 0;

signed short gx_cl = 0;
signed short gy_cl = 0;
signed short gz_cl = 0;

float temperature_c;

void IIC_Write(unsigned char dev_addr, unsigned char mem_addr, 
								unsigned char data)
{
	HAL_I2C_Mem_Write(&hi2c1, dev_addr, mem_addr,
						I2C_MEMADD_SIZE_8BIT, &data, 1, 2);
}

void IIC_Read(unsigned char dev_addr, unsigned char mem_addr, 
	unsigned char *buf, unsigned char len)
{
	HAL_I2C_Mem_Read(&hi2c1, dev_addr, mem_addr, 
					I2C_MEMADD_SIZE_8BIT, buf, len, 2);
}

/*写入初始化MPU6050*/
void MPU6050_Init(void)
{
	unsigned char temp = 0;
	reset_MPU6050:
	IIC_Write(MPU6050_ADDR,PWR_MGMT_1,0x80);	//复位MPU6050
	HAL_Delay(50);
	IIC_Write(MPU6050_ADDR,PWR_MGMT_1,0x01);	//唤醒MPU6050，使用x轴PLL为时钟
	IIC_Read(MPU6050_ADDR, MPU6050_WHO_AM_I, &temp, 1);	//读ID
	temp &= 0x7e;
	printf("%d\r\n",temp);
	if(temp != 0x68)
		goto reset_MPU6050;
		
	IIC_Write(MPU6050_ADDR,SMPLRT_DIV,0x00);	//采样率分频，不分频
	IIC_Write(MPU6050_ADDR,CONFIG,0x00);	//低通滤波器，带宽256~260Hz
	
	IIC_Write(MPU6050_ADDR,GYRO_CONFIG,(0x11<<3));	//陀螺量程,+-2000°
	IIC_Write(MPU6050_ADDR,ACCEL_CONFIG,(0x00<<3));	//加速度计量程,+-2g
/*
	IIC_Write(MPU6050_ADDR,0x6B,0x00);
	IIC_Write(MPU6050_ADDR,0x6A,0x00);
	IIC_Write(MPU6050_ADDR,0x37,0x02);
*/
}


void MPU6050_Get(void)
{
	long temperature_q16;//用于存储q16格式的温度值
	
	IIC_Read(MPU6050_ADDR,ACCEL_XOUT_H,IIC_buf,14);

	ax = (IIC_buf[0]<<8) + IIC_buf[1] - ax_cl;//X轴加速度
	ay = (IIC_buf[2]<<8) + IIC_buf[3] - ay_cl;//Y轴加速度
	az = (IIC_buf[4]<<8) + IIC_buf[5] - az_cl;//Z轴加速度
	
	gx = (IIC_buf[8]<<8) + IIC_buf[9] - gx_cl;//X轴角速度	
	gy = (IIC_buf[10]<<8) + IIC_buf[11] - gy_cl;//Y轴角速度	
	gz = (IIC_buf[12]<<8) + IIC_buf[13] - gz_cl;//Z轴角速度	
	
	gx_speed=(float)gx*Gyro_Gain;
	gy_speed=(float)gy*Gyro_Gain;
	gz_speed=(float)gz*Gyro_Gain;
	
	ax_speed = (float)(ax / ACCEL_SCALE)* GRAVITY;
	ay_speed = (float)(ay / ACCEL_SCALE)* GRAVITY;
	az_speed = (float)(az / ACCEL_SCALE)* GRAVITY;

	if (mpu_get_temperature(&temperature_q16, NULL) == 0) // 转换q16格式为浮点温度值（摄氏度）
	{
			temperature_c = (float)temperature_q16 / 65536.0f;
	}
}


/*
MPU6050校准函数
将IMU水平放置，z轴向上时，启动校准
思路是计算N个周期的平均值，得到校准参数
*/
#define	CL_cnt	128
void MPU6050_calibrate()
{
	unsigned short i;	
	signed int temp[6] = {0};
	for(i=0; i<CL_cnt; i++)
	{
		delay_ms(10);
		MPU6050_Get();
		temp[0] += ax;
		temp[1] += ay;
		temp[2] += az;
		temp[3] += gx;
		temp[4] += gy;
		temp[5] += gz;
	}	
	ax_cl = temp[0]/CL_cnt;
	ay_cl = temp[1]/CL_cnt;
	az_cl = temp[2]/CL_cnt - (0xffff>>2); //平放时z轴有重力加速度g，减去g值
	gx_cl = temp[3]/CL_cnt;
	gy_cl = temp[4]/CL_cnt;
	gz_cl = temp[5]/CL_cnt;
}

