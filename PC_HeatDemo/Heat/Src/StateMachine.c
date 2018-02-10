
#include "StateMachine.h"
#include "heat.h"
static OUT_HandleTypeDef  *pshHuoSai;
static OUT_HandleTypeDef  *pshFengShan;
static OUT_HandleTypeDef  *pshYouBeng;
static TIME_HandleTypeDef  *pshSysTime;
static ALARM_HandleTypeDef *pshAlarm;
static SENSOR_HandleTypeDef *pshSen;
static PARM_HandleTypeDef *pshParm;


static float f_target_adjust_pre;
static int i_target_adjust_pre;
extern USHORT   usRegInputBuf[20];

void StateMachineInit(struct __HEAT_HandleTypeDef *phheat)
{
	pshAlarm = &phheat->hAlarm;
	pshHuoSai = &phheat->hHuoSai;
	pshFengShan = &phheat->hFengShan;
	pshYouBeng = &phheat->hYouBeng;
	pshSen = &phheat->hSensor;
	pshSysTime = &phheat->hSysTime;
	pshParm = &phheat->hParm;
}


void StateMachineIdel(struct __HEAT_HandleTypeDef * phheat)
{											
	//����ո����״̬   ֻ�ж����

//	pshHuoSai->OutSetParm(phheat);//(phheat);
	pshHuoSai->OutStop(pshHuoSai);
//	pshFengShan->OutSetParm(phheat);//(pshFengShan,1500,0, pshSen->SenGetPowerVal());
	pshFengShan->OutStop(pshFengShan);
	pshYouBeng->OutStop(pshYouBeng);

	pshSysTime->StateMachineRun_s = 0;//�������ʱ�����
	phheat->TargetPrm = 0;
	
	while (1)//
	{
		if (pshSysTime->SysTime.Time_s_up_flag)//ϵͳʱ��������±�־  ִ�����м�����
		{
			pshSysTime->SysTime.Time_s_up_flag = 0;
			pshSysTime->StateMachineRun_s++;//ϵͳ����ʱ�����

			if (pshSysTime->StateMachineRun_s > 36000)//0xffff,�������18Сʱ��
			{
				pshSysTime->StateMachineRun_s = 3600;
				pshSysTime->TimeReset(pshSysTime);
				
			}
			phheat->pPrintCurState(phheat);//��ӡ��ǰ״̬
			phheat->pStateMachineAdjest(phheat);
		}
		phheat->peMBPoll();//����ͨ��
		
		if (pshSysTime->StateMachineRun_s > 2)//��������ж�
		{

		}
		//start ��������
		switch (phheat->hKey.KeyStateGetClear())
		{
		case KEY_STATE_HEAT:
			/*phheat->hKey.KeyStateSet(KEY_STATE_SW_2_HEAT_F_IDEL);
			phheat->StateMachineNext = STATE_MACHINE_HEAT;
		break;*/
			phheat->StateMachine = STATE_MACHINE_DEBUG;
			return;
			break;
		case KEY_STATE_STOP:
			phheat->StateMachineNext = STATE_MACHINE_IDEL;
			break;
		case KEY_STATE_WIND:
			phheat->StateMachineNext = STATE_MACHINE_WIND;
			return;
			break;
		case KEY_STATE_DEBUG:
			phheat->StateMachine = STATE_MACHINE_DEBUG;
			return;
			break;
		case KEY_STATE_SW_2_HEAT_F_IDEL:
			if (pshSen->SenGetKeTiVal() > pshAlarm->ALARM_NS_KeTiLowTemp)//�����¶ȹ��߽�ͨ��ģʽ
			{
				phheat->hKey.KeyStateSet(KEY_STATE_SW_2_HEAT_F_WIND);
				phheat->StateMachine = STATE_MACHINE_WIND;
				phheat->StateMachineNext = STATE_MACHINE_HEAT;
				return;
			}
			else {
				phheat->StateMachine = STATE_MACHINE_HEAT;
				phheat->StateMachineNext = STATE_MACHINE_HEAT;

				return;
			}
			break;
		default:
			break;

		}
		//end  ��������

	}


}


void StateMachineAdjust(struct __HEAT_HandleTypeDef *phheat)
{
	//start ��ʼ���ڷ���ת��
	static uint8_t HighPrmFlag=0;
	if(phheat->TargetPrm>800)
	{
	 HighPrmFlag=1;
	}else
	{
	HighPrmFlag=0;
	}
	if (phheat->TargetPrm > phheat->CurrentPrm)
	{
		if (HighPrmFlag&&phheat->TargetPrm - phheat->CurrentPrm > 200)
		{
			phheat->pSetFenShanPre(200);
		}else if (HighPrmFlag&&phheat->TargetPrm - phheat->CurrentPrm > 100)
		{
			phheat->pSetFenShanPre(100);
		}
		else if (phheat->TargetPrm - phheat->CurrentPrm > 20)
		{
			phheat->pSetFenShanPre(20);
		}
		else if(phheat->TargetPrm - phheat->CurrentPrm > 2)
		{
			phheat->pSetFenShanPre(2);
		}
		else
		{

		}
	}
	else if (phheat->TargetPrm < phheat->CurrentPrm)
	{
		if (HighPrmFlag&&phheat->CurrentPrm - phheat->TargetPrm > 200)
		{
			phheat->pSetFenShanPre(-200);
		}
		else if (HighPrmFlag&&phheat->CurrentPrm - phheat->TargetPrm > 100)
		{
			phheat->pSetFenShanPre(-100);
		}else if (phheat->CurrentPrm - phheat->TargetPrm > 20)
		{
			phheat->pSetFenShanPre(-20);
		}
		else if (phheat->CurrentPrm - phheat->TargetPrm > 2)
		{
			phheat->pSetFenShanPre(-2);
		}
		else
		{

		}
	}


	//end ���ڷ���ת��

}
void StateMachineDebug(struct __HEAT_HandleTypeDef *phheat)
{
	//����ո����״̬   ֻ�ж����
	//	pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, 0, pshSen->SenGetPowerVal());
	//pshHuoSai->OutStop(pshHuoSai);
	//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, 5, pshSen->SenGetPowerVal());
	//pshFengShan->OutStart(pshFengShan);
	//pshYouBeng->OutStop(pshYouBeng);
	pshSysTime->StateMachineRun_s = 0;//�������ʱ�����
	phheat->TargetPrm = 1000;
	while (1)//
	{
		if (pshSysTime->SysTime.Time_s_up_flag)//ϵͳʱ��������±�־  ִ�����м�����
		{
			pshSysTime->SysTime.Time_s_up_flag = 0;

			pshSysTime->StateMachineRun_s++;//ϵͳ����ʱ�����

			phheat->pPrintCurState(phheat);//��ӡ��ǰ״̬

			
			phheat->pStateMachineAdjest(phheat);
		}
		if (pshSysTime->SysTime.Time_100ms_up_flag)
		{
			pshSysTime->SysTime.Time_100ms_up_flag = 0;
		//phheat->pStateMachineAdjest(phheat);
		}	
		
		phheat->peMBPoll();//����ͨ��
		
		//start ��������
		switch (phheat->hKey.KeyStateGetClear())
		{
		case KEY_STATE_STOP:
			phheat->StateMachine = STATE_MACHINE_IDEL;
			return;
			break;
		default:
			break;

		}
		//end  ��������
		pshAlarm->AlarmCheck(pshAlarm);//���ϼ��

	}

}

void StateMachineWind(struct __HEAT_HandleTypeDef *phheat)
{
	//����ո����״̬   ֻ�ж����
//	pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, 0, pshSen->SenGetPowerVal());
	pshHuoSai->OutStop(pshHuoSai);
	//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, 5, pshSen->SenGetPowerVal());
	pshFengShan->OutStart(pshFengShan);
	pshYouBeng->OutStop(pshYouBeng);
	pshSysTime->StateMachineRun_s = 0;//�������ʱ�����

	while (1)//
	{
		if (pshSysTime->SysTime.Time_s_up_flag)//ϵͳʱ��������±�־  ִ�����м�����
		{
			
			pshSysTime->StateMachineRun_s++;//ϵͳ����ʱ�����
			//start ת���Զ�����
			if (pshSysTime->StateMachineRun_s<pshParm->WIND_FS_D1.Stop_s)//60s
			{
				//f_target_adjust_pre=FengShanAdjust_Pre(pshParm->WIND_FS_D1.Pre, pshFengShan->curPre, pshParm->WIND_FS_D1.Stop_s - pshSysTime->StateMachineRun_s);
				//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			//end ת���Զ�����
			if (pshSysTime->StateMachineRun_s > 36000)//0xffff,�������18Сʱ��
			{
				pshSysTime->StateMachineRun_s = 3600;
				pshSysTime->TimeReset(pshSysTime);

			}
			phheat->pPrintCurState(phheat);//��ӡ��ǰ״̬
		}
		phheat->peMBPoll();//����ͨ��

		if (pshSysTime->StateMachineRun_s > 2)//��������ж�
		{

		}
		//start ��������
		switch (phheat->hKey.KeyStateGetClear())
		{
		case KEY_STATE_HEAT:
			phheat->hKey.KeyStateSet(KEY_STATE_SW_2_HEAT_F_WIND);
			phheat->StateMachineNext = STATE_MACHINE_HEAT;
			break;
		case KEY_STATE_STOP:
			phheat->StateMachineNext = STATE_MACHINE_IDEL;
			break;
		case KEY_STATE_SW_2_HEAT_F_WIND:
			if (pshSen->SenGetKeTiVal() > pshAlarm->ALARM_NS_KeTiLowTemp)//�����¶ȹ��߽�ͨ��ģʽ
			{
				phheat->hKey.KeyStateSet(KEY_STATE_SW_2_HEAT_F_WIND);
				
			}
			else {
				phheat->StateMachine = STATE_MACHINE_HEAT;
				phheat->StateMachineNext = STATE_MACHINE_HEAT;

				return;
			}
			break;
		default:
			break;

		}
		//end  ��������
		pshAlarm->AlarmCheck(pshAlarm);//���ϼ��

	}

}
void StateMachineHeat(struct __HEAT_HandleTypeDef *phheat)
{
	//����ո����״̬   ֻ�ж����
	//pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, 0, pshSen->SenGetPowerVal());
	pshHuoSai->OutStop(pshHuoSai);
	//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, 5, pshSen->SenGetPowerVal());
	pshFengShan->OutStart(pshFengShan);
	pshYouBeng->OutStop(pshYouBeng);
	pshSysTime->StateMachineRun_s = 0;//�������ʱ�����

	while (1)//
	{
		if (pshSysTime->SysTime.Time_s_up_flag)//ϵͳʱ��������±�־  ִ�����м�����
		{
			pshSysTime->StateMachineRun_s++;//ϵͳ����ʱ�����
			//start ���ȿ���
			if (pshSysTime->StateMachineRun_s>pshParm->HEAT_FS_D1.Start_s &&pshSysTime->StateMachineRun_s<pshParm->HEAT_FS_D1.Stop_s)//60s
			{
				f_target_adjust_pre = FengShanAdjust_Pre(pshParm->HEAT_FS_D1.Pre , pshFengShan->curPre, pshParm->HEAT_FS_D1.Stop_s - pshSysTime->StateMachineRun_s);
				//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
			}else if (pshSysTime->StateMachineRun_s>pshParm->HEAT_FS_D2.Start_s &&pshSysTime->StateMachineRun_s<pshParm->HEAT_FS_D2.Stop_s)//60s
			{
				f_target_adjust_pre = FengShanAdjust_Pre(pshParm->HEAT_FS_D2.Pre, pshFengShan->curPre, pshParm->HEAT_FS_D2.Stop_s - pshSysTime->StateMachineRun_s);
				//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			else if (pshSysTime->StateMachineRun_s>pshParm->HEAT_FS_D3.Start_s &&pshSysTime->StateMachineRun_s<pshParm->HEAT_FS_D3.Stop_s)//60s
			{
				f_target_adjust_pre = FengShanAdjust_Pre(pshParm->HEAT_FS_D3.Pre, pshFengShan->curPre, pshParm->HEAT_FS_D3.Stop_s - pshSysTime->StateMachineRun_s);
				//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			//end ���ȿ���
			//start ��������
			if (pshSysTime->StateMachineRun_s == pshParm->HEAT_HS_EN_Time)
			{
				//pshHuoSai->OutSetParm(phheat);//(pshHuoSai,50,pshParm->HS_StartPrmPre, pshSen->SenGetPowerVal());
			}
			else if (pshSysTime->StateMachineRun_s >pshParm->HEAT_HS_D1.Start_s && pshSysTime->StateMachineRun_s <pshParm->HEAT_HS_D1.Stop_s)
			{
				i_target_adjust_pre= HuoSaiAdjust_Pre(pshParm->HEAT_HS_D1.Pre,pshHuoSai->curPre, pshParm->HEAT_HS_D1.Stop_s -pshSysTime->StateMachineRun_s);
				//pshHuoSai->OutSetParm(phheat);//(pshHuoSai,50,i_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			else if (pshSysTime->StateMachineRun_s >pshParm->HEAT_HS_D2.Start_s && pshSysTime->StateMachineRun_s <pshParm->HEAT_HS_D2.Stop_s)
			{
				i_target_adjust_pre = HuoSaiAdjust_Pre(pshParm->HEAT_HS_D2.Pre, pshHuoSai->curPre, pshParm->HEAT_HS_D2.Stop_s - pshSysTime->StateMachineRun_s);
				//pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, i_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			
			//end ��������
			//start �ͱÿ���
			if (pshSysTime->StateMachineRun_s == pshParm->HEAT_YB_EN_Time)
			{
				switch (pshParm->ModeXkw)
				{
				case MODE_2KW:
					//pshYouBeng->OutSetParm(phheat);//(pshYouBeng,100,10,240);
					break;
				case MODE_3KW:
					//pshYouBeng->OutSetParm(phheat);//(pshYouBeng, 130, 10, 240);
					break;
				case MODE_5KW:
					////pshYouBeng->OutSetParm(phheat);//(pshYouBeng, 150, 10, 240);
					break;
				default:
					//pshYouBeng->OutSetParm(phheat);//(pshYouBeng, 100, 10, 240);
					break;
				}

			}
			if (pshSysTime->StateMachineRun_s > pshParm->HEAT_YB_ADJ_Time)
			{
				//pshYouBeng->OutSetParm(phheat);//(pshYouBeng,(int)((pshFengShan->curPre*100)/(pshParm->FS_PreToYB_Hz)),10,240);
			}
			//end  �ͱÿ���

			//start �жϵ��ɹ�
			if (pshSysTime->StateMachineRun_s == pshParm->HEAT_KT_StartTime)
			{
				phheat->StartKeTiTemp = pshSen->SenGetKeTiVal();
			}
			if (pshSysTime->StateMachineRun_s > pshParm->HEAT_KT_StartTime&&phheat->StartKeTiTemp > pshSen->SenGetKeTiVal())
			{
				phheat->StartKeTiTemp = pshSen->SenGetKeTiVal();
			}
			if (pshSysTime->StateMachineRun_s > pshParm->HEAT_KT_JudgeTime)
			{
				if (pshSen->SenGetKeTiVal() > (pshParm->HEAT_STATE_KETI_RISE_TEMP + phheat->StartKeTiTemp))
				{
					phheat->StateMachine = STATE_MACHINE_NORMAL;
					return;
				}
				else {
					phheat->StateMachine = STATE_MACHINE_HEAT2;
					return;
				}
			}
			//end �жϵ��ɹ�
			if (pshSysTime->StateMachineRun_s > 36000)//0xffff,�������18Сʱ��
			{
				pshSysTime->StateMachineRun_s = 3600;
				pshSysTime->TimeReset(pshSysTime);

			}
			phheat->pPrintCurState(phheat);//��ӡ��ǰ״̬
		}
		phheat->peMBPoll();//����ͨ��

		if (pshSysTime->StateMachineRun_s > 2)//��������ж�
		{

		}
		//start ��������
		switch (phheat->hKey.KeyStateGetClear())
		{
		case KEY_STATE_HEAT:
			break;
		case KEY_STATE_STOP:
			phheat->StateMachineNext = STATE_MACHINE_IDEL;
			if (pshSysTime->StateMachineRun_s < pshParm->HEAT_YB_EN_Time)
			{
				phheat->StateMachine = STATE_MACHINE_POWER_OFF;
				return;
			}
			else {
				phheat->StateMachine = STATE_MACHINE_STOP;
				return;
			}
			break;
		case KEY_STATE_AUTO_OIL:
			phheat->StateMachine = STATE_MACHINE_AUTO_OIL;
			return;
			
			break;
		case KEY_STATE_TEST:
			phheat->StateMachine = STATE_MACHINE_TEST;
			return;
			break;
		default:
			break;

		}
		//end  ��������
		pshAlarm->AlarmCheck(pshAlarm);//���ϼ��

	}

}

void StateMachineHeat2(struct __HEAT_HandleTypeDef *phheat)
{
	//����ո����״̬   ֻ�ж����
	//pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, 0, pshSen->SenGetPowerVal());
	pshHuoSai->OutStop(pshHuoSai);
	//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, 5, pshSen->SenGetPowerVal());
	pshFengShan->OutStart(pshFengShan);
	pshYouBeng->OutStop(pshYouBeng);
	pshSysTime->StateMachineRun_s = 0;//�������ʱ�����
	
	phheat->HEAT2_KT_START_HEAT_Flag = 0;

	while (1)//
	{
		if (pshSysTime->SysTime.Time_s_up_flag)//ϵͳʱ��������±�־  ִ�����м�����
		{
			pshSysTime->StateMachineRun_s++;//ϵͳ����ʱ�����
											//start ���ȿ���
			if (pshSysTime->StateMachineRun_s>pshParm->HEAT2_FS_D1.Start_s &&pshSysTime->StateMachineRun_s<pshParm->HEAT2_FS_D1.Stop_s)//60s
			{
				f_target_adjust_pre = FengShanAdjust_Pre(pshParm->HEAT2_FS_D1.Pre, pshFengShan->curPre, pshParm->HEAT2_FS_D1.Stop_s - pshSysTime->StateMachineRun_s);
				//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			else if (pshSysTime->StateMachineRun_s>pshParm->HEAT2_FS_D2.Start_s &&pshSysTime->StateMachineRun_s<pshParm->HEAT2_FS_D2.Stop_s)//60s
			{
				f_target_adjust_pre = FengShanAdjust_Pre(pshParm->HEAT2_FS_D2.Pre, pshFengShan->curPre, pshParm->HEAT2_FS_D2.Stop_s - pshSysTime->StateMachineRun_s);
				//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			else if (pshSysTime->StateMachineRun_s>pshParm->HEAT2_FS_D3.Start_s &&pshSysTime->StateMachineRun_s<pshParm->HEAT2_FS_D3.Stop_s)//60s
			{
				f_target_adjust_pre = FengShanAdjust_Pre(pshParm->HEAT2_FS_D3.Pre, pshFengShan->curPre, pshParm->HEAT2_FS_D3.Stop_s - pshSysTime->StateMachineRun_s);
				//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			//end ���ȿ���
			if (phheat->HEAT2_KT_START_HEAT_Flag)
			{
				//start ��������

				if (pshSysTime->StateMachineRun_s == pshParm->HEAT2_HS_EN_OFST_Time)
				{
					//pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, pshParm->HS_StartPrmPre, pshSen->SenGetPowerVal());
				}
				else if (pshSysTime->StateMachineRun_s > (pshParm->HEAT2_HS_OFST_D1.Start_s+ phheat->HEAT2_KT_START_HEAT_Time) && pshSysTime->StateMachineRun_s < (pshParm->HEAT2_HS_OFST_D1.Stop_s + phheat->HEAT2_KT_START_HEAT_Time))
				{
					i_target_adjust_pre = HuoSaiAdjust_Pre(pshParm->HEAT2_HS_OFST_D1.Pre, pshHuoSai->curPre, pshParm->HEAT2_HS_OFST_D1.Stop_s + phheat->HEAT2_KT_START_HEAT_Time- pshSysTime->StateMachineRun_s);
					//pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, i_target_adjust_pre, pshSen->SenGetPowerVal());
				}
				

				//end ��������
				//start �ͱÿ���
				if (pshSysTime->StateMachineRun_s == (pshParm->HEAT2_YB_EN_OFST_Time +phheat->HEAT2_KT_START_HEAT_Time))
				{
					switch (pshParm->ModeXkw)
					{
					case MODE_2KW:
						//pshYouBeng->OutSetParm(phheat);//(pshYouBeng, 100, 10, 240);
						break;
					case MODE_3KW:
						//pshYouBeng->OutSetParm(phheat);//(pshYouBeng, 130, 10, 240);
						break;
					case MODE_5KW:
						//pshYouBeng->OutSetParm(phheat);//(pshYouBeng, 150, 10, 240);
						break;
					default:
						//pshYouBeng->OutSetParm(phheat);//(pshYouBeng, 100, 10, 240);
						break;
					}

				}
				if (pshSysTime->StateMachineRun_s > (pshParm->HEAT2_YB_ADJ_OFST_Time + phheat->HEAT2_KT_START_HEAT_Time))
				{
					//pshYouBeng->OutSetParm(phheat);//(pshYouBeng, (int)((pshFengShan->curPre * 100) / (pshParm->FS_PreToYB_Hz)), 10, 240);
				}
				//end  �ͱÿ���
			}
			//start �жϵ��ɹ�
			if ((!(phheat->HEAT2_KT_START_HEAT_Flag)) && pshSen->SenGetKeTiVal() < pshParm->HEAT2_KT_DROPDOWN_START_HEAT&&pshSysTime->StateMachineRun_s > pshParm->HEAT2_START_HEAT_Time)
			{
				phheat->StartKeTiTemp= pshSen->SenGetKeTiVal();
				phheat->HEAT2_KT_START_HEAT_Time = pshSysTime->StateMachineRun_s;
				phheat->HEAT2_KT_START_HEAT_Flag = 1;

			}
			if (phheat->HEAT2_KT_START_HEAT_Flag&&pshSysTime->StateMachineRun_s>(phheat->HEAT2_KT_START_HEAT_Time+pshParm->HEAT2_KT_HeatOffJudgeTime))
			{
				if (phheat->StartKeTiTemp > pshSen->SenGetKeTiVal())
				{
					phheat->StartKeTiTemp = pshSen->SenGetKeTiVal();
				}
				if(pshSysTime->StateMachineRun_s>(phheat->HEAT2_KT_START_HEAT_Time + pshParm->HEAT2_KT_HeatOffJudgeTime)&&pshSen->SenGetKeTiVal()>(phheat->StartKeTiTemp+pshParm->HEAT_STATE_KETI_RISE_TEMP))
				{
					phheat->StateMachine = STATE_MACHINE_NORMAL;
					return;
				}
				else {
					phheat->StateMachine = STATE_MACHINE_STOP;
					return;
				}
			}

			if (pshSysTime->StateMachineRun_s == 100)
			{
				phheat->StartKeTiTemp = pshSen->SenGetKeTiVal();
			}
			if (pshSysTime->StateMachineRun_s > 100&&phheat->StartKeTiTemp > pshSen->SenGetKeTiVal())
			{
				phheat->StartKeTiTemp = pshSen->SenGetKeTiVal();
			}
			if (pshSysTime->StateMachineRun_s > 210)
			{
				if (pshSen->SenGetKeTiVal() > (pshParm->HEAT_STATE_KETI_RISE_TEMP + phheat->StartKeTiTemp))
				{
					phheat->StateMachine = STATE_MACHINE_NORMAL;
					return;
				}
				else {
					phheat->StateMachine = STATE_MACHINE_STOP;
					return;
				}
			}
			//end �жϵ��ɹ�
			if (pshSysTime->StateMachineRun_s > 36000)//0xffff,�������18Сʱ��
			{
				pshSysTime->StateMachineRun_s = 3600;
				pshSysTime->TimeReset(pshSysTime);

			}
			phheat->pPrintCurState(phheat);//��ӡ��ǰ״̬
		}
		phheat->peMBPoll();//����ͨ��

		if (pshSysTime->StateMachineRun_s > 2)//��������ж�
		{

		}
		//start ��������
		switch (phheat->hKey.KeyStateGetClear())
		{
		case KEY_STATE_HEAT:
			break;
		case KEY_STATE_STOP:
			phheat->StateMachineNext = STATE_MACHINE_IDEL;
			phheat->StateMachine = STATE_MACHINE_STOP;
			return;
			break;
		default:
			break;

		}
		//end  ��������
		pshAlarm->AlarmCheck(pshAlarm);//���ϼ��

	}

}

void StateMachineNormal(struct __HEAT_HandleTypeDef *phheat)
{
#define  NORMAL_STATE_CHUKOU_TEMP_DELAY_30S  6//30��*6��=3min�Ƚϣ�״̬ʱ���������¶���ʱ�������ж������  

	//����ո����״̬   ֻ�ж����
	int16_t  l_ChuKou_Temp_Old[NORMAL_STATE_CHUKOU_TEMP_DELAY_30S];//���徲̬���飬�����ж��Ƿ����
	int16_t  l_KeTi_Temp_Old[NORMAL_STATE_CHUKOU_TEMP_DELAY_30S];//���徲̬���飬����ж����ӿ����¶�
	int16_t  l_Count_30s = 0;//��¼���뵱ǰ״̬��ʱ�䣬��֤30s��¼һ������

	for (uint8_t j = 0; j<NORMAL_STATE_CHUKOU_TEMP_DELAY_30S; j++)//��ʼ�������¶�����
	{
		l_ChuKou_Temp_Old[j] = 0;
		l_KeTi_Temp_Old[j] = 0;
	}
	pshSysTime->StateMachineRun_s = 0;//�������ʱ�����
	phheat->StateRunLevel = STATE_RUN_LEVEL_3;//Ĭ������3��
	int16_t l_level_count_s = 0;//ͨ��ģʽ����ʱ��
	int16_t l_jinkou_over_dianweiqi_continue_count_s = 0;//�����������ڵ�λ���¶ȼ���
	int16_t l_dianweiqi_over_jinkou_continue_count_s = 0;//��λ���������ڽ����¶ȼ���
	uint8_t l_level_switch_lock_flag = 0;//״̬�л�����־λ�������ڵ����ڼ䣬ת�ٲ���
	int16_t  l_adjust_count_s = 0;

	while (1)//
	{
		if (pshSysTime->SysTime.Time_s_up_flag)//ϵͳʱ��������±�־  ִ�����м�����
		{
			pshSysTime->StateMachineRun_s++;//ϵͳ����ʱ�����
			//start ���ȿ���
			if (pshSysTime->StateMachineRun_s>pshParm->NORMAL_FS_D1.Start_s &&pshSysTime->StateMachineRun_s<pshParm->NORMAL_FS_D1.Stop_s)//60s
			{
				f_target_adjust_pre = FengShanAdjust_Pre(pshParm->NORMAL_FS_D1.Pre, pshFengShan->curPre, pshParm->NORMAL_FS_D1.Stop_s - pshSysTime->StateMachineRun_s);
				//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			else if (pshSysTime->StateMachineRun_s>pshParm->NORMAL_FS_D2.Start_s &&pshSysTime->StateMachineRun_s<pshParm->NORMAL_FS_D2.Stop_s)//60s
			{
				f_target_adjust_pre = FengShanAdjust_Pre(pshParm->NORMAL_FS_D2.Pre, pshFengShan->curPre, pshParm->NORMAL_FS_D2.Stop_s - pshSysTime->StateMachineRun_s);
				//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			
			//end ���ȿ���
			//start ��������
			if (pshSysTime->StateMachineRun_s == pshParm->NORMAL_HS_DIS_Time)
			{
				//pshHuoSai->OutSetParm(phheat);//(pshHuoSai,15000,0,pshSen->SenGetPowerVal());
				pshHuoSai->OutStop(pshHuoSai);
			}
		
			//end ��������
			//start �ͱÿ���
			
			if (pshSysTime->StateMachineRun_s > pshParm->HEAT_YB_ADJ_Time)
			{
				//pshYouBeng->OutSetParm(phheat);//(pshYouBeng, (int)((pshFengShan->curPre * 100) / (pshParm->FS_PreToYB_Hz)), 10, 240);
			}
			//end  �ͱÿ���
			//start �Զ���������
			if (pshSysTime->StateMachineRun_s > pshParm->NORMAL_START_SWLEVEL_Time)
			{
				l_level_count_s++;
				if (l_level_count_s > pshParm->NORMAL_STATE_LEVEL_HOLD_Second)
				{
					switch (phheat->StateRunLevel)
					{
					case STATE_RUN_LEVEL_1:
						//start �����ж�
						if (pshSen->SenGetJinKouVal()>( pshParm->NORMAL_DWQ_JK_TEMP_Cpmpare+ pshSen->SenGetStateRunVal()))//����3�ȿ�ʼ����
						{
							l_jinkou_over_dianweiqi_continue_count_s++;
							l_dianweiqi_over_jinkou_continue_count_s = 0;
#if(DEBUG_MODE==1)
							sprintf(StringPrint, "O level1_dropdown_time:%d", l_jinkou_over_dianweiqi_continue_count_s);
#endif
							if (l_jinkou_over_dianweiqi_continue_count_s>pshParm->NORMAL_STATE_SWITCH_LOW_LEVEL_DELAY_Second)//ʱ��������������ʼ����
							{
								if (!l_level_switch_lock_flag)//����ȷ�ϵ�ǰ�������ڹ���û�б���ס
								{
									printf( "O Current Level is lowst cant adjust run_time:%d\n", l_level_count_s);
									/*
									l_adjust_count_s=0;
									l_level_switch_lock_flag=1;//������ס�������ڹ���
									l_next_level_flag=1;//�������ڣ���һ��level1.
									*/
								}
							}
						}
						else {
							l_jinkou_over_dianweiqi_continue_count_s = 0;
						}
						//end  �����ж�
						//start �����ж�
						if (pshSen->SenGetStateRunVal()>(pshParm->NORMAL_DWQ_JK_TEMP_Cpmpare  + pshSen->SenGetJinKouVal()))//���������ж�
						{
							l_dianweiqi_over_jinkou_continue_count_s++;
							l_jinkou_over_dianweiqi_continue_count_s = 0;
#if(DEBUG_MODE==1)
							sprintf(StringPrint, "O level1_rise_time:%d", l_dianweiqi_over_jinkou_continue_count_s);
#endif
							if (l_dianweiqi_over_jinkou_continue_count_s>pshParm->NORMAL_STATE_SWITCH_HIGH_LEVEL_DELAY_Second)
							{
								if (!l_level_switch_lock_flag)//����ȷ�ϵ�ǰ�������ڹ���û�б���ס
								{
									l_adjust_count_s = 0;
									l_level_switch_lock_flag = 1;//������ס�������ڹ���
									phheat->StateRunLevel = STATE_RUN_LEVEL_2;//l_next_level_flag = 3;//�������ڣ���һ��level2.
								}
							}
						}
						else {
							l_dianweiqi_over_jinkou_continue_count_s = 0;
						}
						//end �����ж�
						break;
					case STATE_RUN_LEVEL_2:
						//start �����ж�
						if (pshSen->SenGetJinKouVal()>(pshParm->NORMAL_DWQ_JK_TEMP_Cpmpare + pshSen->SenGetStateRunVal()))//����3�ȿ�ʼ����
						{
							l_jinkou_over_dianweiqi_continue_count_s++;
							l_dianweiqi_over_jinkou_continue_count_s = 0;
#if(DEBUG_MODE==1)
							sprintf(StringPrint, "O level1_dropdown_time:%d", l_jinkou_over_dianweiqi_continue_count_s);
#endif
							if (l_jinkou_over_dianweiqi_continue_count_s>pshParm->NORMAL_STATE_SWITCH_LOW_LEVEL_DELAY_Second)//ʱ��������������ʼ����
							{
								if (!l_level_switch_lock_flag)//����ȷ�ϵ�ǰ�������ڹ���û�б���ס
								{
									printf("O Current Level is lowst cant adjust run_time:%d\n", l_level_count_s);
									
									l_adjust_count_s=0;
									l_level_switch_lock_flag=1;//������ס�������ڹ���
									//l_next_level_flag=1;//�������ڣ���һ��level1.
									phheat->StateRunLevel = STATE_RUN_LEVEL_1;//l_next_level_flag = 3;//�������ڣ���һ��level2.
								}
							}
						}
						else {
							l_jinkou_over_dianweiqi_continue_count_s = 0;
						}
						//end  �����ж�
						//start �����ж�
						if (pshSen->SenGetStateRunVal()>(pshParm->NORMAL_DWQ_JK_TEMP_Cpmpare + pshSen->SenGetJinKouVal()))//���������ж�
						{
							l_dianweiqi_over_jinkou_continue_count_s++;
							l_jinkou_over_dianweiqi_continue_count_s = 0;
#if(DEBUG_MODE==1)
							sprintf(StringPrint, "O level1_rise_time:%d", l_dianweiqi_over_jinkou_continue_count_s);
#endif
							if (l_dianweiqi_over_jinkou_continue_count_s>pshParm->NORMAL_STATE_SWITCH_HIGH_LEVEL_DELAY_Second)
							{
								if (!l_level_switch_lock_flag)//����ȷ�ϵ�ǰ�������ڹ���û�б���ס
								{
									l_adjust_count_s = 0;
									l_level_switch_lock_flag = 1;//������ס�������ڹ���
									phheat->StateRunLevel = STATE_RUN_LEVEL_3;//l_next_level_flag = 3;//�������ڣ���һ��level2.
								}
							}
						}
						else {
							l_dianweiqi_over_jinkou_continue_count_s = 0;
						}
						//end �����ж�
						break;
					case STATE_RUN_LEVEL_3:
						//start �����ж�
						if (pshSen->SenGetJinKouVal()>(pshParm->NORMAL_DWQ_JK_TEMP_Cpmpare + pshSen->SenGetStateRunVal()))//����3�ȿ�ʼ����
						{
							l_jinkou_over_dianweiqi_continue_count_s++;
							l_dianweiqi_over_jinkou_continue_count_s = 0;
#if(DEBUG_MODE==1)
							sprintf(StringPrint, "O level1_dropdown_time:%d", l_jinkou_over_dianweiqi_continue_count_s);
#endif
							if (l_jinkou_over_dianweiqi_continue_count_s>pshParm->NORMAL_STATE_SWITCH_LOW_LEVEL_DELAY_Second)//ʱ��������������ʼ����
							{
								if (!l_level_switch_lock_flag)//����ȷ�ϵ�ǰ�������ڹ���û�б���ס
								{
									printf("O Current Level is lowst cant adjust run_time:%d\n", l_level_count_s);
									
									l_adjust_count_s=0;
									l_level_switch_lock_flag=1;//������ס�������ڹ���
									//l_next_level_flag=1;//�������ڣ���һ��level1.
									phheat->StateRunLevel = STATE_RUN_LEVEL_2;//l_next_level_flag = 3;//�������ڣ���һ��level2.
								}
							}
						}
						else {
							l_jinkou_over_dianweiqi_continue_count_s = 0;
						}
						//end  �����ж�
						//start �����ж�
						if (pshSen->SenGetStateRunVal()>(pshParm->NORMAL_DWQ_JK_TEMP_Cpmpare + pshSen->SenGetJinKouVal()))//���������ж�
						{
							l_dianweiqi_over_jinkou_continue_count_s++;
							l_jinkou_over_dianweiqi_continue_count_s = 0;
#if(DEBUG_MODE==1)
							sprintf(StringPrint, "O level1_rise_time:%d", l_dianweiqi_over_jinkou_continue_count_s);
#endif
							if (l_dianweiqi_over_jinkou_continue_count_s>pshParm->NORMAL_STATE_SWITCH_HIGH_LEVEL_DELAY_Second)
							{
								if (!l_level_switch_lock_flag)//����ȷ�ϵ�ǰ�������ڹ���û�б���ס
								{
									//l_adjust_count_s = 0;
									//l_level_switch_lock_flag = 1;//������ס�������ڹ���
									//phheat->StateRunLevel = STATE_RUN_LEVEL_2;//l_next_level_flag = 3;//�������ڣ���һ��level2.
								}
							}
						}
						else {
							l_dianweiqi_over_jinkou_continue_count_s = 0;
						}
						//end �����ж�
						break;
					default:
						break;
					}
				}
				
				//start ִ�е���
				//start ����
				if (l_level_switch_lock_flag)//����ס��������
				{
					l_adjust_count_s++;
#if(DEBUG_MODE==1)
					sprintf(StringPrint, "O level_switch_time:%d", l_adjust_count_s);
#endif
					if (l_adjust_count_s>pshParm->NORMAL_STATE_SWITCH_LEVEL_Second)
					{
						l_level_switch_lock_flag = 0;//����
						l_level_count_s = 0;
						//l_level_flag = l_next_level_flag;
						//l_next_level_flag == 0;
						l_jinkou_over_dianweiqi_continue_count_s = 0;
						l_dianweiqi_over_jinkou_continue_count_s = 0;
					}
					switch (phheat->StateRunLevel)
					{
					case STATE_RUN_LEVEL_1:
						if (l_adjust_count_s<pshParm->NORMAL_STATE_SWITCH_LEVEL_Second)
						{
							f_target_adjust_pre = FengShanAdjust_Pre(pshParm->FS3000PrmPre, pshFengShan->curPre, pshParm->NORMAL_STATE_SWITCH_LEVEL_Second - l_adjust_count_s);
							//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
						}
						break;
					case STATE_RUN_LEVEL_2:
						if (l_adjust_count_s<pshParm->NORMAL_STATE_SWITCH_LEVEL_Second)
						{
							f_target_adjust_pre = FengShanAdjust_Pre(pshParm->FS4000PrmPre, pshFengShan->curPre, pshParm->NORMAL_STATE_SWITCH_LEVEL_Second - l_adjust_count_s);
							//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
						}
						break;
					case STATE_RUN_LEVEL_3:
						if (l_adjust_count_s<pshParm->NORMAL_STATE_SWITCH_LEVEL_Second)
						{
							f_target_adjust_pre = FengShanAdjust_Pre(pshParm->FS4500PrmPre, pshFengShan->curPre, pshParm->NORMAL_STATE_SWITCH_LEVEL_Second - l_adjust_count_s);
							//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
						}
						break;
					default:
						break;
					}
					
					
				}
				//end  ����
				//end ִ�е���

			}
			//end  �Զ���������
			
			//start ����ж�
		
			if ((pshSysTime->StateMachineRun_s == 0) || (pshSysTime->StateMachineRun_s == 30))//ÿ30sִ��1��
			{
				l_KeTi_Temp_Old[l_Count_30s] = pshSen->SenGetKeTiVal();// s_KeTiTemp;
				l_ChuKou_Temp_Old[l_Count_30s++] = pshSen->SenGetChuKouVal();//s_ChuKouTemp;//�����ڱ����洢��������
				if (l_Count_30s == NORMAL_STATE_CHUKOU_TEMP_DELAY_30S)
				{
					l_Count_30s = 0;
				}
			}
			if (pshSysTime->StateMachineRun_s >pshParm->NORMAL_STATE_HEAT_OUT_JUDGE_Time && pshSen->SenGetKeTiVal() <pshParm-> NORMAL_STATE_HEAT_OUT_JUDGE_KeTiTemp)//ʱ��>5min������С��40�ȣ��ж�Ϊ���
			{
				phheat->StateMachine = STATE_MACHINE_HEAT2;//�л������ε��ģʽ
				return;
			}
			static char i;
			for (i = 0; i<NORMAL_STATE_CHUKOU_TEMP_DELAY_30S; i++)//ʵʱ�Ƚϵ�ǰ
			{
			
				if (l_ChuKou_Temp_Old[i]>(pshSen->SenGetChuKouVal() +pshParm->NORMAL_STATE_CHUKOU_DROPDOWN_Temp) && l_KeTi_Temp_Old[i]>(pshSen->SenGetKeTiVal() +pshParm->NORMAL_STATE_KETI_DROPDOWN_Temp))
				{
					phheat->StateMachine = STATE_MACHINE_HEAT2;//�л������ε��ģʽ
					return;
				}
			}
			//end �ж����
			if (pshSysTime->StateMachineRun_s > 36000)//0xffff,�������18Сʱ��
			{
				pshSysTime->StateMachineRun_s = 3600;
				pshSysTime->TimeReset(pshSysTime);

			}
			phheat->pPrintCurState(phheat);//��ӡ��ǰ״̬
		}//end  if (pshSysTime->SysTime.Time_s_up_flag)//ϵͳʱ��������±�־  ִ�����м�����
		phheat->peMBPoll();//����ͨ��

		if (pshSysTime->StateMachineRun_s > 2)//��������ж�
		{

		}
		//start ��������
		switch (phheat->hKey.KeyStateGetClear())
		{
		case KEY_STATE_HEAT:
			break;
		case KEY_STATE_STOP:
			phheat->StateMachineNext = STATE_MACHINE_IDEL;
			phheat->StateMachine = STATE_MACHINE_STOP;
			return;
			break;
		
		default:
			break;

		}
		//end  ��������
		pshAlarm->AlarmCheck(pshAlarm);//���ϼ��

	}

}


void StateMachineStop(struct __HEAT_HandleTypeDef *phheat)
{
	//����ո����״̬   ֻ�ж����
	//pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, 0, pshSen->SenGetPowerVal());
	pshHuoSai->OutStop(pshHuoSai);
	//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, 5, pshSen->SenGetPowerVal());
	pshFengShan->OutStart(pshFengShan);
	pshYouBeng->OutStop(pshYouBeng);
	pshSysTime->StateMachineRun_s = 0;//�������ʱ�����

	while (1)//
	{
		if (pshSysTime->SysTime.Time_s_up_flag)//ϵͳʱ��������±�־  ִ�����м�����
		{
			pshSysTime->StateMachineRun_s++;//ϵͳ����ʱ�����
											//start ���ȿ���
			if (pshSysTime->StateMachineRun_s>pshParm->STOP_FS_D1.Start_s &&pshSysTime->StateMachineRun_s<pshParm->STOP_FS_D1.Stop_s)//60s
			{
				f_target_adjust_pre = FengShanAdjust_Pre(pshParm->STOP_FS_D1.Pre, pshFengShan->curPre, pshParm->STOP_FS_D1.Stop_s - pshSysTime->StateMachineRun_s);
				//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, f_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			
			//end ���ȿ���
			//start ��������
			if (pshSysTime->StateMachineRun_s == pshParm->STOP_HS_D1.Start_s)
			{
				//pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, pshParm->HS_StartPrmPre, pshSen->SenGetPowerVal());
			}
			else if (pshSysTime->StateMachineRun_s >pshParm->STOP_HS_D1.Start_s && pshSysTime->StateMachineRun_s <pshParm->STOP_HS_D1.Stop_s)
			{
				i_target_adjust_pre = HuoSaiAdjust_Pre(pshParm->STOP_HS_D1.Pre, pshHuoSai->curPre, pshParm->STOP_HS_D1.Stop_s - pshSysTime->StateMachineRun_s);
				//pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, i_target_adjust_pre, pshSen->SenGetPowerVal());
			}
			if (pshParm->STOP_HS_DIS_Time == pshSysTime->StateMachineRun_s)
			{
				//pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, i_target_adjust_pre,240);
				pshHuoSai->OutStop(pshHuoSai);
			}
			//end ��������

			//start �����¶��жϹػ�
			
			if (( pshSen->SenGetKeTiVal()< 50) && (pshSysTime->StateMachineRun_s>pshParm->STOP_HS_DIS_Time))
			{
				phheat->StateMachine = STATE_MACHINE_POWER_OFF;
				return;
			}
			
			//end  �����¶��жϹػ�
			if (pshSysTime->StateMachineRun_s > 36000)//0xffff,�������18Сʱ��
			{
				pshSysTime->StateMachineRun_s = 3600;
				pshSysTime->TimeReset(pshSysTime);

			}
			phheat->pPrintCurState(phheat);//��ӡ��ǰ״̬
		}
		phheat->peMBPoll();//����ͨ��

		if (pshSysTime->StateMachineRun_s > 2)//��������ж�
		{

		}
		//start ��������
		switch (phheat->hKey.KeyStateGetClear())
		{
		case KEY_STATE_HEAT:
			phheat->hKey.KeyStateSet(KEY_STATE_SW_2_HEAT_F_WIND);
			phheat->StateMachineNext = STATE_MACHINE_HEAT;
			break;
		case KEY_STATE_STOP:
			phheat->StateMachineNext = STATE_MACHINE_IDEL;
			break;
		case KEY_STATE_SW_2_HEAT_F_WIND:
			if (pshSen->SenGetKeTiVal() > pshAlarm->ALARM_NS_KeTiLowTemp)//�����¶ȹ��߽�ͨ��ģʽ
			{
				phheat->hKey.KeyStateSet(KEY_STATE_SW_2_HEAT_F_WIND);

			}
			else {
				phheat->StateMachine = STATE_MACHINE_HEAT;
				phheat->StateMachineNext = STATE_MACHINE_HEAT;

				return;
			}
			break;
		default:
			break;

		}
		//end  ��������
		pshAlarm->AlarmCheck(pshAlarm);//���ϼ��

	}

}
void StateMachinePowerOff(struct __HEAT_HandleTypeDef *phheat)
{
	//����ո����״̬   ֻ�ж����
	//pshHuoSai->OutSetParm(phheat);//(pshHuoSai, 50, 0, pshSen->SenGetPowerVal());
	pshHuoSai->OutStop(pshHuoSai);
	pshYouBeng->OutStop(pshYouBeng);
	pshSysTime->StateMachineRun_s = 0;//�������ʱ�����

	while (1)//
	{
		if (pshSysTime->SysTime.Time_s_up_flag)//ϵͳʱ��������±�־  ִ�����м�����
		{
			pshSysTime->StateMachineRun_s++;//ϵͳ����ʱ�����
			//start ת���Զ�����
			//pshFengShan->OutSetParm(phheat);//(pshFengShan, 1500, pshFengShan->curPre*0.8, pshSen->SenGetPowerVal());
			if (pshFengShan->curPre < 10)
			{
				//MainBoardPowerDisable;//�����ػ����������
				phheat->StateMachine = STATE_MACHINE_IDEL;
				return;
			}
			//end ת���Զ�����
			if (pshSysTime->StateMachineRun_s > 36000)//0xffff,�������18Сʱ��
			{
				pshSysTime->StateMachineRun_s = 3600;
				pshSysTime->TimeReset(pshSysTime);

			}
			phheat->pPrintCurState(phheat);//��ӡ��ǰ״̬
		}
		phheat->peMBPoll();//����ͨ��

		if (pshSysTime->StateMachineRun_s > 2)//��������ж�
		{

		}
		//start ��������
		
		//end  ��������
		pshAlarm->AlarmCheck(pshAlarm);//���ϼ��

	}

}

//20150719��Coolthing.Liang ����ת�ٵ���
//��õ��ڷ���ռ�ձ�
float FengShanAdjust_Pre(float target_pre, float current_pre, int remain_time)
{
	if (remain_time<1)
	{
		return current_pre;
	}
	else {
		return (current_pre + (target_pre - current_pre) / remain_time);
	}
}
//��õ��ڷ���ռ�ձ�
int YouBengAdjust_Pre(int target_pre, int current_pre, int remain_time)
{
	if (remain_time<1)
	{
		return current_pre;
	}
	else {
		return (current_pre + (target_pre - current_pre) / remain_time);
	}
}
//20150821��Coolthing.Liang ���ӻ�������
//��û�������ռ�ձ�
int HuoSaiAdjust_Pre(int target_pre, int current_pre, int remain_time)
{
	if (remain_time<1)
	{
		return current_pre;
	}
	else {
		return (current_pre + (target_pre - current_pre) / remain_time);
	}
}

//��ӡ��ǰ״̬��
void PrintCurrentState(struct __HEAT_HandleTypeDef *phheat)
{

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
#else
	//extern uint32_t HuoErInput[100];
	//static uint32_t CapValue = 0;
	//#if 0
	//if (HuoErInput[0]<HuoErInput[1])
	//{
	//	CapValue = HuoErInput[1] - HuoErInput[0];
	//}
	//else {
	//	CapValue = 0xffff + HuoErInput[1] - HuoErInput[0];

	//}
	//phheat->CurrentPrm=(100000.0 * 60)/CapValue;
	//#endif
	//#if true
	//CapValue=0;
	//static uint8_t overflag=0;
	//for(int i=0;i<HUO_ER_INPUT_LENGTH;i++)
	//{
	//	if(HuoErInput[i]>0xffff)
	//	{
	//		overflag=1;
	//	break;
	//	}
	//CapValue+=HuoErInput[i];
	//	
	//}
	//if(overflag)
	//{
	//phheat->CurrentPrm=0;
	//	overflag=0;
	//}else
	//{
	////CapValue=CapValue/HUO_ER_INPUT_LENGTH;
	//phheat->CurrentPrm=(100000.0 * 60*HUO_ER_INPUT_LENGTH)/CapValue;
	//}

	//#endif
#endif
	//��õ�ǰ״̬
	//static  STATE_TypeDef  l_state_machine;
	/*uint8_t temp1 = 0;
	uint8_t temp2 = 0;
	uint8_t temp3 = 0;
	uint8_t temp4 = 0;
	uint8_t temp = 0;

	int16_t temp_uint16_t;*/
	////״̬�л�  ����
	////l_state_machine = STATE_GetNextState();//STATE_GetState();//����20161022
	//temp1 = phheat->StateMachine;//phheat->StateMachineNext;//0;// l_state_machine;
	//temp2 = phheat->hAlarm.AlarmState;// phheat->hAlarm.//ALARM_State_Get();
	//temp3 = (temp1 << 4) | temp2;
	////����״̬  ��λ +����
	//temp1 = phheat->StateRunLevel;//0;//g_Level_Flag;//Get_DianWeiQi_Tempreture();
	//temp2 = phheat->StateMachineNext; //0;
	//temp4 = (temp1 << 4) | temp2;
	//usRegInputBuf[0] = (temp3 << 8) | temp4;
	////��ѹ
	//temp_uint16_t = phheat->PowerVal_M100;//phheat->hSensor.SenGetPowerVal();//(uint16_t)Get_Power_VolotageMUL10();
	//usRegInputBuf[1] = temp_uint16_t;
	////ת��
	//temp_uint16_t = phheat->hHuoer.HuoerGetRpm();// phheat->hSensor.SenGetJinKouVal();//Get_JinKou_Tempreture();//HUOER_GetRPM();
	//usRegInputBuf[2] = temp_uint16_t;
	////�����¶�
	//temp_uint16_t = phheat->hSensor.SenGetKeTiVal()+200;//Get_KeTi_Tempreture() + 200;
	//usRegInputBuf[3] = temp_uint16_t;
	////��������ռ�ձ�
	//temp_uint16_t = phheat->hHuoSai.curPre;// 0;// PWM_HuoSai_Get_Adj_Pre();
	//usRegInputBuf[4] = temp_uint16_t;
	////���ȵ���ռ�ձ�
	//temp_uint16_t = phheat->hFengShan.curPre;//0;// PWM_FengShan_Get_Adj_Pre();
	//usRegInputBuf[5] = temp_uint16_t;
	////����ʱ��
	//temp_uint16_t = phheat->hSysTime.StateMachineRun_s;//(uint16_t)run_time;
	//usRegInputBuf[6] = temp_uint16_t;
	////����ռ�ձ�
	//temp_uint16_t = phheat->hHuoSai.curPre;//0;// PWM_HuoSaiGet_Percentage();
	//usRegInputBuf[7] = temp_uint16_t;
	////����ռ�ձ�
	//temp_uint16_t = phheat->hFengShan.curPre;//0;// PWM_FengShanGet_Percentage();
	//usRegInputBuf[8] = temp_uint16_t;
	////�ͱ�Ƶ��
	//temp3 = phheat->hYouBeng.curPre/10;//0;// PWM_YouBengGet_MUL100_Hz() / 10;

	//temp4 = phheat->hParm.ModeXkw;//0;// g_StateMode;//����ģʽ
	//usRegInputBuf[9] = (temp3 << 8) | temp4;
	//usRegInputBuf[10] = phheat->hSensor.SenGetHuoSaiFbVal();//Get_Huosai_feedback_Vtge();//ADC_Conversion_Value[0][1]; //
	//usRegInputBuf[11] = phheat->hSensor.SenGetChuKouVal();// phheat->hSensor.SenGetYouBengFbVal();//Get_YouBeng_feedback_Vtge();
	////usRegInputBuf[12] = phheat->hSensor.SenGetYouBengFbVal();

}



