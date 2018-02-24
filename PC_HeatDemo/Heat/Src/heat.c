#include "heat.h"
#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
#else
#include "stm32f0xx_hal.h"
extern TIM_HandleTypeDef htim17;
#endif
extern HEAT_HandleTypeDef hheat;

HEAT_StatusTypeDef HEAT_Init(HEAT_HandleTypeDef *phheat)
{
	
//参数初始化  配置参数  版本号、
//hheat->hParm.ParmInit = PARM_Init;
//hheat->hParm.ParmInit(hheat.hParm);
//hheat->hParm.ParmSet(hheat.hParm);
//函数接口初始化  获取温度、电压、反馈   风扇 壳体
phheat->pStateMachineInit = StateMachineInit;
phheat->pStateMachineInit(phheat);
	phheat->HW_Version = 100;
	
	phheat->JinKouTemp = 0;
	phheat->getjinKouTemp = PTgetJinKouTemp;

	phheat->ChuKouTemp = 0;
	phheat->getchuKouTemp = PTgetChuKouTemp;

	phheat->KeTiTemp = 0;
	phheat->getkeTiTemp = PTgetKeTiTemp;

	phheat->UserSetTemp = 0;
	phheat->getuserSetTemp = PTgetUserSetTemp;

	phheat->getHallFbVal = PTgetHallFbVal;
	phheat->PowerVal_M100 = 2400;
	phheat->getPowerVal = PTgetPowerVal;

	phheat->getFsFbVal = PTgetFsFbVal;
	phheat->getHsFbVal = PTgetHsFbVal;
	phheat->getYbFbVal = PTgetYbFbVal;

	phheat->hHuoSai.OutInit = PWM_HuoSaiInit;
	phheat->hHuoSai.OutInit(&phheat->hHuoSai);

	phheat->hFengShan.OutInit = PWM_FengShanInit;
	phheat->hFengShan.OutInit(&phheat->hFengShan);

	phheat->hYouBeng.OutInit = PWM_YouBengInit;
	phheat->hYouBeng.OutInit(&phheat->hYouBeng);



	
	int aa = 1;

	//start 初始化系统
	
	phheat->hSysTime.TimeInit = TIME_Init;
	phheat->hSysTime.TimeInit(&phheat->hSysTime);

	phheat->hKey.KeyStateInit = KEY_StateInit;
	phheat->hKey.KeyStateInit(&phheat->hKey);

	phheat->hAlarm.AlarmInit = ALARM_Init;
	phheat->hAlarm.AlarmInit(&phheat->hAlarm);

	phheat->pCommPoll = CommPoll;//485通信
	phheat->pStateMachineAdjest = StateMachineAdjust;
	phheat->pSetFenShanPre = PTsetPreFengShan;
	phheat->pSetHsVolt = PTsetHsVolt;
	phheat->pSetYbHz = PTsetYbHz;

	phheat->hParm.ParmInit = PARM_Init;
	phheat->hParm.ParmSet = PARM_Set;
	phheat->hParm.ParmInit(&phheat->hParm);
	phheat->hParm.ParmSet(&phheat->hParm);
	
	//end  初始化系统
	
	return HEAT_OK;

}




