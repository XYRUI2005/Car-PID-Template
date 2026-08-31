#ifndef __MENU_H
#define __MENU_H

#include "sys.h"
#include <stdio.h>
#include "key.h"
#include "oled.h"
#include "line.h"
#include "encoder.h"
#include "control.h"
#include "mpu6050.h"
#include "dmp.h"

extern uint8_t ITEM_Num;

void MENU_Display(uint8_t num);
void MENU_Item(void);

#endif

