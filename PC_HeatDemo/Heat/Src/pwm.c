
#include "pwm.h"



void PWM_HuoSaiInit(struct __OUT_HandleTypeDef * phout)
{
	phout->OutState = OUT_STATE_IDEL;
	phout->OutStart = PWM_HuoSaiStart;
	phout->OutStop = PWM_HuoSaiStop;
	//phout->OutSetParm = PWM_HuoSaiSetParm;

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Hua Sai Init \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}
void PWM_FengShanInit(struct __OUT_HandleTypeDef * phout)
{
	phout->OutState = OUT_STATE_IDEL;
	phout->OutStart = PWM_FengShanStart;
	phout->OutStop = PWM_FengShanStop;
//	phout->OutSetParm = PWM_FengShanSetParm;

	

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Feng Shan Init \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}
void PWM_YouBengInit(struct __OUT_HandleTypeDef * phout)
{
	phout->OutState = OUT_STATE_IDEL;
	phout->OutStart = PWM_YouBengStart;
	phout->OutStop = PWM_YouBengStop;
//	phout->OutSetParm = PWM_YouBengSetParm;

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("You Beng Init \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}
void PWM_HuoSaiStart(struct __OUT_HandleTypeDef * phout)
{
	phout->OutState = OUT_STATE_ENABLE;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Hua Sai Start \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}
void PWM_FengShanStart(struct __OUT_HandleTypeDef * phout)
{
	phout->OutState = OUT_STATE_ENABLE;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Feng Shan Start \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}
void PWM_YouBengStart(struct __OUT_HandleTypeDef * phout)
{
	phout->OutState= OUT_STATE_ENABLE;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("You Beng Start \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}
void PWM_HuoSaiStop(struct __OUT_HandleTypeDef * phout)
{
	phout->OutState = OUT_STATE_IDEL;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Hua Sai Stop \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}
void PWM_FengShanStop(struct __OUT_HandleTypeDef * phout)
{
	phout->OutState = OUT_STATE_IDEL;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Feng Shan Stop \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}
void PWM_YouBengStop(struct __OUT_HandleTypeDef * phout)
{
	phout->OutState = OUT_STATE_IDEL;
//	phout->curPre = 0;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("You Beng Stop \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}

void PWM_HuoSaiSetParm(struct __HEAT_HandleTypeDef * phheat)
{
	
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Huo Sai Set Parm \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}
void PWM_FengShanSetParm(struct __HEAT_HandleTypeDef * phheat)
{
	
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Feng Shan Set Parm \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}
void PWM_YouBengSetParm(struct __HEAT_HandleTypeDef * phheat)
{
	
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("You Beng Set Parm \n");
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}

