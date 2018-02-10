
#ifndef __ADC_H
#define __ADC_H
#include "heat.h"

void ADC_Init(struct __SENSOR_HandleTypeDef * phsensor);
int16_t ADC_GetPowerVal();
int16_t ADC_GetKeTiVal();
int16_t ADC_GetJinKouVal();
int16_t ADC_GetChuKouVal();
int16_t ADC_GetYouBengFbVal();
int16_t ADC_GetHuoSaiFbVal();
int16_t ADC_GetDianWeiQiVal();
#endif