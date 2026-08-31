#ifndef __DMP_H__
#define __DMP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define q30  1073741824.0f

unsigned short inv_row_2_scale(const signed char *row);
unsigned char run_self_test(void);
unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx);
unsigned short inv_row_2_scale(const signed char *row);
unsigned char mpu_dmp_init(void);
unsigned char mpu_dmp_get_data(float *pitch,float *roll,float *yaw);

extern float pitch,roll,yaw; 

	#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
