#include "adc.h"


static SENSOR_HandleTypeDef *phSensor;
void ADC_Init(struct __SENSOR_HandleTypeDef * phsensor)
{
	
	phsensor->SenGetPowerVal = ADC_GetPowerVal;
	phsensor->SenGetKeTiVal = ADC_GetKeTiVal;
	phsensor->SenGetJinKouVal = ADC_GetJinKouVal;
	phsensor->SenGetChuKouVal = ADC_GetChuKouVal;
	phsensor->SenGetYouBengFbVal = ADC_GetYouBengFbVal;
	phsensor->SenGetHuoSaiFbVal = ADC_GetHuoSaiFbVal;
	phsensor->SenGetStateRunVal = ADC_GetDianWeiQiVal;
	phsensor->SenTestPower = 240;
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Sensor Init \n");
	phSensor = phsensor;
	

#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif
}

int16_t ADC_GetPowerVal()
{
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Get Power Value \n");
	return phSensor->SenTestPower;
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}
int16_t ADC_GetKeTiVal()
{
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Get Power Value \n");
	return phSensor->SenTestKeTi;
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}
int16_t ADC_GetJinKouVal()
{
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Get Power Value \n");
	return phSensor->SenTestJinKou;
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}
int16_t ADC_GetChuKouVal()
{
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Get Power Value \n");
	return phSensor->SenTestChuKou;
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}
int16_t ADC_GetHuoSaiFbVal()
{
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Get Power Value \n");
	return phSensor->SenTestHuoSaiFb;
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}
int16_t ADC_GetYouBengFbVal()
{
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Get Power Value \n");
	return phSensor->SenTestYouBengFb;
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}

int16_t ADC_GetDianWeiQiVal()
{
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("Get Power Value \n");
	return phSensor->SenTestDianWeiQi;
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

}
