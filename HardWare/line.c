#include "line.h"

uint8_t Line_Data;
int32_t Line_Num;

/**
 * @brief       灰度巡线补偿值获取
 * @param       无
 * @retval      巡线偏移的值
 */
void LINE_Scan(void)
{
    /* 读取并记录电平状态，这里根据实际01代表的情况修改，可能相反，传感器越多效果越好 */
		Line_Data = (HW8<<7)|(HW7<<6)|(HW6<<5)|(HW5<<4)|(HW4<<3)|(HW3<<2)|(HW2<<1)|HW1;

    /* 巡线值 */
		if (Line_Data == 0x01) {Line_Num=24;}          //0000 0001偏左，需要往右转 8421
		else if (Line_Data == 0x03) {Line_Num=20;}     //0000 0011	
		else if (Line_Data == 0x02) {Line_Num=16;}     //0000 0010
		else if (Line_Data == 0x06) {Line_Num=12;}     //0000 0110
		else if (Line_Data == 0x04) {Line_Num=8;}     //0000 0100
		else if (Line_Data == 0x0C) {Line_Num=4;}      //0000 1100
		else if (Line_Data == 0x08) {Line_Num=2;}      //0000 1000
				
		else if (Line_Data == 0x18) {Line_Num=0;}      //0001 1000正中间，直行

		else if (Line_Data == 0x10) {Line_Num=-2;}     //0001 0000偏右，需要往左转
		else if (Line_Data == 0x30) {Line_Num=-4;}     //0011 0000
		else if (Line_Data == 0x20) {Line_Num=-8;}    //0010 0000
		else if (Line_Data == 0x60) {Line_Num=-12;}    //0110 0000
		else if (Line_Data == 0x40) {Line_Num=-16;}    //0100 0000
		else if (Line_Data == 0xC0) {Line_Num=-20;}    //1100 0000
		else if (Line_Data == 0x80) {Line_Num=-24;}    //1000 0000
		
		else {Line_Num=0;}
}


