#ifndef  __STATEMACHINE_
#define  __STATEMACHINE_
#include "heat.h"

#include "stdint.h"
void STATE_heat();/*¼ÓÈÈ×´Ì¬*/
//void StateIdel(HEAT_HandleTypeDef *phheat);




float FengShanAdjust_Pre(float target_pre, float current_pre, int remain_time);
int HuoSaiAdjust_Pre(int target_pre, int current_pre, int remain_time);
int YouBengAdjust_Pre(int target_pre, int current_pre, int remain_time);
uint16_t FengShanAdjust_Prm(uint16_t target_prm, uint16_t current_prm, uint16_t remain_time);

#endif