//#include "heat.h"
#include "alarm.h"

void ALARM_Init(struct __ALARM_HandleTypeDef * phalarm)
{
	phalarm->AlarmCheck = ALARM_Check;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Alarm Init \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}
void ALARM_Check(struct __ALARM_HandleTypeDef * phalarm)
{

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Alarm Check \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}

