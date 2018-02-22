#include "heat.h"

HEAT_StatusTypeDef HEAT_Init(HEAT_HandleTypeDef *phheat)
{
	HEAT_HandleTypeDef *p2hheat;
//参数初始化  配置参数  版本号、
//hheat->hParm.ParmInit = PARM_Init;
//hheat->hParm.ParmInit(hheat.hParm);
//hheat->hParm.ParmSet(hheat.hParm);
//函数接口初始化  获取温度、电压、反馈   风扇 壳体

	phheat->HW_Version = 100;
	
	phheat->



	//start 初始化系统
	//hheat.pStateMachineInit = StateMachineInit;
	//hheat.pStateMachineInit(&hheat);

	//hheat.hHuoSai.OutInit = PWM_HuoSaiInit;
	//hheat.hHuoSai.OutInit(&hheat.hHuoSai);

	//hheat.hFengShan.OutInit = PWM_FengShanInit;
	//hheat.hFengShan.OutInit(&hheat.hFengShan);

	//hheat.hYouBeng.OutInit = PWM_YouBengInit;
	//hheat.hYouBeng.OutInit(&hheat.hYouBeng);

	//hheat.hSensor.SenInit = ADC_Init;
	//hheat.hSensor.SenInit(&hheat.hSensor);

	//hheat.hSysTime.TimeInit = TIME_Init;
	//hheat.hSysTime.TimeInit(&hheat.hSysTime);

	//hheat.hKey.KeyStateInit = KEY_StateInit;
	//hheat.hKey.KeyStateInit(&hheat.hKey);

	//hheat.hAlarm.AlarmInit = ALARM_Init;
	//hheat.hAlarm.AlarmInit(&hheat.hAlarm);

	

	//hheat.hHuoer.HuoerInit = HUOER_Init;
	//hheat.hHuoer.HuoerInit(&hheat.hHuoer);

	//hheat.peMBPoll = eMBPoll;//485通信
	//hheat.pPrintCurState = PrintCurrentState;
	//hheat.hPower.SensorInit=ADC
	//end  初始化系统
	//hheat->test = 100;

	return HEAT_OK;

}




