#ifndef __HUOER_H
#define __HUOER_H
#include "heat.h"

void HUOER_Init(struct __HUOER_HandleTypeDef * phhuoer);
int HUOER_GetRPM();
void  HUOER_SetTime(int Time_ms);


#endif
