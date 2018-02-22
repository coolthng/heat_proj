#ifndef __TIME_H
#define __TIME_H
#include "heat.h"

void TIME_Init(struct __TIME_HandleTypeDef * phtime);
void TIME_Updata(struct __TIME_HandleTypeDef * phtime);
void TIME_Reset(struct __TIME_HandleTypeDef * phtime);

#endif
