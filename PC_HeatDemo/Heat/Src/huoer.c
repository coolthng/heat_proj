#include "huoer.h"

void HUOER_Init(struct __HUOER_HandleTypeDef * phhuoer)
{
	phhuoer->HuoerSetTime = HUOER_SetTime;
	phhuoer->HuoerGetRpm = HUOER_GetRPM;
}
//获得霍尔转速
int HUOER_GetRPM()
{
	return 300;
	//char i;
	//int min;
	//min = HuoEr_TimeBuffer[0];
	//for (i = 0; i<4; i++)
	//{
	//	if (min>HuoEr_TimeBuffer[i])
	//		min = HuoEr_TimeBuffer[i];
	//}
	//return 60000 / min;//3000;//  //
}
void  HUOER_SetTime(int Time_ms)
{
	/*static char count;
	HuoEr_TimeBuffer[count++] = Time_ms;
	if (count == 4) count = 0;*/
}