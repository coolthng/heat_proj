#ifndef  __STATEMACHINE_
#define  __STATEMACHINE_
#include "heat.h"
//#include "heat.h"
void STATE_heat();/*¼ÓÈÈ×´Ì¬*/
//void StateIdel(HEAT_HandleTypeDef *phheat);




float FengShanAdjust_Pre(float target_pre, float current_pre, int remain_time);
int HuoSaiAdjust_Pre(int target_pre, int current_pre, int remain_time);
int YouBengAdjust_Pre(int target_pre, int current_pre, int remain_time);


#endif