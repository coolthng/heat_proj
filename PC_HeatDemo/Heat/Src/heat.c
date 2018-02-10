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
	printf("��ǰת��:%d,adjust:%d\n", hheat.CurrentPrm, adjust);
#else
	hheat.CurrentPrmTest=current_pre;
	htim17.Instance->CCR1=current_pre;
#endif
}
#define DEBUG_PRINT(x)    printf(x)  //NULL
int16_t getJinKouTemp(HEAT_HandleTypeDef *phheat)
{
	//��ӽ��ڻ�ȡ�����¶�
	DEBUG_PRINT("��ȡ�����¶�\n");
	
	phheat->JinKouTemp = 100;
}
int16_t getChuKouTemp(HEAT_HandleTypeDef *phheat)
{
	//��ӽ��ڻ�ȡ�����¶�
	DEBUG_PRINT("��ȡ�����¶�\n");

	phheat->ChuKouTemp = 100;
}
int16_t getKeTiTemp(HEAT_HandleTypeDef *phheat)
{
	//��ӽ��ڻ�ȡ�����¶�
	DEBUG_PRINT("��ȡ�����¶�\n");

	phheat->KeTiTemp = 100;
}
int16_t getPowerVal(HEAT_HandleTypeDef *phheat)
{
	//��ӽ��ڻ�ȡ�����¶�
	DEBUG_PRINT("��ȡ��Դ��ѹ\n");

	phheat->PowerVal_M100 = 2400;
}
HEAT_StatusTypeDef HEAT_Init(HEAT_HandleTypeDef *phheat)
{
	
//������ʼ��  ���ò���  �汾�š�
//hheat->hParm.ParmInit = PARM_Init;
//hheat->hParm.ParmInit(hheat.hParm);
//hheat->hParm.ParmSet(hheat.hParm);
//�����ӿڳ�ʼ��  ��ȡ�¶ȡ���ѹ������   ���� ����
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

	//start ��ʼ��ϵͳ
	

	

	
	
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

	phheat->peMBPoll = eMBPoll;//485ͨ��
	phheat->pPrintCurState = PrintCurrentState;
	phheat->pStateMachineAdjest = StateMachineAdjust;
	phheat->pSetFenShanPre = SetPreFengShan;
	//hheat.hPower.SensorInit=ADC
	//end  ��ʼ��ϵͳ
	//hheat->test = 100;

	return HEAT_OK;

}




