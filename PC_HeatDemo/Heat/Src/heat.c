#include "heat.h"
#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
#else
#include "stm32f0xx_hal.h"
extern TIM_HandleTypeDef htim17;
#endif
extern HEAT_HandleTypeDef hheat;
uint16_t SetPreFengShan(int16_t adjust)
{
	static int16_t current_pre = 0;
	current_pre += adjust;
	if (current_pre < 0)
		current_pre = 0;
#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	hheat.CurrentPrm = current_pre;
	hheat.CurrentPrmTest = current_pre;
	printf("当前转速:%d,adjust:%d\n", hheat.CurrentPrm, adjust);
#else
	hheat.CurrentPrmTest=current_pre;
	htim17.Instance->CCR1=current_pre;
#endif
}
#define DEBUG_PRINT(x)    printf(x)  //NULL
int16_t getJinKouTemp(HEAT_HandleTypeDef *phheat)
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取进口温度\n");
	
	phheat->JinKouTemp = 100;
}
int16_t getChuKouTemp(HEAT_HandleTypeDef *phheat)
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取出口温度\n");

	phheat->ChuKouTemp = 100;
}
int16_t getKeTiTemp(HEAT_HandleTypeDef *phheat)
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取壳体温度\n");

	phheat->KeTiTemp = 100;
}
int16_t getPowerVal(HEAT_HandleTypeDef *phheat)
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取电源电压\n");

	phheat->PowerVal_M100 = 2400;
}
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
	phheat->getJinKouTemp = getJinKouTemp;
	//phheat->getJinKouTemp(phheat);
	phheat->ChuKouTemp = 0;
	
	phheat->KeTiTemp = 0;
	phheat->getkeTiTemp = getKeTiTemp;

	phheat->PowerVal_M100 = 2400;
	phheat->getPowerVal = getPowerVal;

	phheat->hHuoSai.OutInit = PWM_HuoSaiInit;
	phheat->hHuoSai.OutInit(&phheat->hHuoSai);

	phheat->hFengShan.OutInit = PWM_FengShanInit;
	phheat->hFengShan.OutInit(&phheat->hFengShan);

	phheat->hYouBeng.OutInit = PWM_YouBengInit;
	phheat->hYouBeng.OutInit(&phheat->hYouBeng);



	
	int aa = 1;

	//start 初始化系统
	

	

	
	
	phheat->hSensor.SenInit = ADC_Init;
	phheat->hSensor.SenInit(&phheat->hSensor);

	phheat->hSysTime.TimeInit = TIME_Init;
	phheat->hSysTime.TimeInit(&phheat->hSysTime);

	phheat->hKey.KeyStateInit = KEY_StateInit;
	phheat->hKey.KeyStateInit(&phheat->hKey);

	phheat->hAlarm.AlarmInit = ALARM_Init;
	phheat->hAlarm.AlarmInit(&phheat->hAlarm);

	

	phheat->hHuoer.HuoerInit = HUOER_Init;
	phheat->hHuoer.HuoerInit(&phheat->hHuoer);

	phheat->peMBPoll = eMBPoll;//485通信
	phheat->pPrintCurState = PrintCurrentState;
	phheat->pStateMachineAdjest = StateMachineAdjust;
	phheat->pSetFenShanPre = SetPreFengShan;

	phheat->hParm.ParmInit = PARM_Init;
	phheat->hParm.ParmSet = PARM_Set;
	phheat->hParm.ParmInit(&phheat->hParm);
	phheat->hParm.ParmSet(&phheat->hParm);
	//hheat.hPower.SensorInit=ADC
	//end  初始化系统
	//hheat->test = 100;

	return HEAT_OK;

}




