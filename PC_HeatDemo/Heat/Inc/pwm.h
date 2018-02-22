#ifndef __PWM_H
#define __PWM_H
#include "heat.h"
void PWM_HuoSaiInit(struct __OUT_HandleTypeDef * phout);




void PWM_FengShanInit(struct __OUT_HandleTypeDef * phout);
void PWM_YouBengInit(struct __OUT_HandleTypeDef * phout);
void PWM_HuoSaiStart(struct __OUT_HandleTypeDef * phout);
void PWM_FengShanStart(struct __OUT_HandleTypeDef * phout);
void PWM_YouBengStart(struct __OUT_HandleTypeDef * phout);
void PWM_HuoSaiStop(struct __OUT_HandleTypeDef * phout);
void PWM_FengShanStop(struct __OUT_HandleTypeDef * phout);
void PWM_YouBengStop(struct __OUT_HandleTypeDef * phout);

void PWM_HuoSaiSetParm(struct __HEAT_HandleTypeDef * phheat);
void PWM_HuoSaiSetParm22(struct __OUT_HandleTypeDef * phout, uint16_t fre, uint16_t pre, uint16_t power_voltage);

void PWM_FengShanSetParm(struct __HEAT_HandleTypeDef * phheat);
void PWM_FengShanSetParm22(struct __OUT_HandleTypeDef * phout, uint16_t fre, uint16_t pre, uint16_t power_voltage);

void PWM_YouBengSetParm(struct __HEAT_HandleTypeDef * phheat);
void PWM_YouBengSetParm22(struct __OUT_HandleTypeDef * phout, uint16_t fre, uint16_t pre, uint16_t power_voltage);


#endif