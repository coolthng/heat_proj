#include "time.h"

void TIME_Init(struct __TIME_HandleTypeDef * phtime)
{
	phtime->TimeUpdata = TIME_Updata;
	phtime->TimeReset = TIME_Reset;

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Time Init \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}

void TIME_Updata(struct __TIME_HandleTypeDef * phtime)
{

}

void TIME_Reset(struct __TIME_HandleTypeDef * phtime)
{

}