#ifndef __HEAT_H
#define __HEAT_H
#include "StateMachine.h"
#include "PlateFormConfig.h"
#include "modbus.h"
#include "pwm.h"
#include "adc.h"
#include "time.h"
#include "key.h"
#include "alarm.h"
#include "parm.h"
#include "huoer.h"
#include "port.h"


#include "stdint.h"
#include "stdio.h"

#define HUO_ER_INPUT_LENGTH 4
typedef struct sensor_s {
	int value;


}sensor_t;
typedef struct out_s {
	char state;

}out_t;

typedef enum
{
	HEAT_OK = 0x00,
	HEAT_ERROR = 0x01,
	HEAT_BUSY = 0x02,
	HEAT_TIMEOUT = 0x03
} HEAT_StatusTypeDef;
typedef enum
{
	STATE_MACHINE_HEAT = (uint8_t)0x01,/*����״̬1*/
	STATE_MACHINE_WIND,/*ͨ��״̬2*/
	STATE_MACHINE_NORMAL,/*����״̬3*/
	STATE_MACHINE_HEAT2,/*���ε��4*/
	STATE_MACHINE_STOP,/*�ػ�״̬5*/
	STATE_MACHINE_POWER_OFF,/*�����ػ�״̬6*/
	STATE_MACHINE_IDEL,/*��״̬7*/
	STATE_MACHINE_TEST,/*����״̬*/
	STATE_MACHINE_AUTO_OIL,/*�Զ�����״̬*/
	STATE_MACHINE_DEBUG,/*����״̬*/

}STATE_MACHINE_TypeDef;

typedef enum __KEY_STATE_TypeDef
{
	KEY_STATE_NONE = 0x00,  /*�Ƿ�����*/
	KEY_STATE_HEAT,  /*���Ȱ���*/
	KEY_STATE_WIND,  /*�������*/
	KEY_STATE_STOP,  /*ͣ������*/
	KEY_STATE_ADD,  /*��������*/
	KEY_STATE_SUB,  /*��������*/
	KEY_STATE_AUTO_OIL,  /*�Զ����ͼ�*/
	KEY_STATE_TEST,  /*����ģʽ��*/
	KEY_STATE_DEBUG,  /*����ģʽ��*/
	KEY_STATE_SW_2_HEAT_F_STOP,
	KEY_STATE_SW_2_HEAT_F_IDEL,
	KEY_STATE_SW_2_HEAT_F_WIND,

}KEY_STATE_TypeDef;
typedef enum
{
	STATE_RUN_LEVEL_1 = (uint8_t)0x01,/*���е�λ1��*/
	STATE_RUN_LEVEL_2,
	STATE_RUN_LEVEL_3,
	STATE_RUN_LEVEL_4,
	STATE_RUN_LEVEL_5,
	STATE_RUN_LEVEL_6,
	
}STATE_RUN_LEVEL_TypeDef;
typedef enum
{
	OUT_STATE_IDEL = (uint8_t)0x01,/**/
	OUT_STATE_ENABLE ,/**/
	OUT_STATE_OPEN,/**/
	OUT_STATE_SHORT,/**/
	OUT_STATE_ERROR,/**/

}OUT_STATE_TypeDef;
typedef struct __OUT_HandleTypeDef {
	OUT_STATE_TypeDef OutState;//���״̬
	uint16_t setPre;
	uint16_t curPre;
	//uint16_t OutFre;
	void(*OutInit)(struct __OUT_HandleTypeDef * phout); /* function pointer on Rx ISR */
	void(*OutStart)(struct __OUT_HandleTypeDef * phout); /* function pointer on Rx ISR */
	void(*OutStop)(struct __OUT_HandleTypeDef * phout); /* function pointer on Rx ISR */
	
	//void(*OutSetParm)(struct __HEAT_HandleTypeDef * pheat ); /* function pointer on Rx ISR */
	//void(*STATE_Machine)();
}OUT_HandleTypeDef;

typedef enum
{
	SENSOR_STATE_IDEL = (uint8_t)0x01,/**/
	SENSOR_STATE_ENABLE,/**/
	SENSOR_STATE_OPEN,/**/
	SENSOR_STATE_SHORT,/**/
	SENSOR_STATE_ERROR,/**/

}SENSOR_STATE_TypeDef;
typedef struct __SENSOR_HandleTypeDef {
	SENSOR_STATE_TypeDef SensorState;//���״̬
	uint16_t SenTestPower;
	uint16_t SenTestDianWeiQi;
	uint16_t SenTestKeTi; 
	uint16_t SenTestJinKou;
	uint16_t SenTestChuKou;
	uint16_t SenTestHuoSaiFb;
	uint16_t SenTestYouBengFb;

	void(*SenInit)(struct __SENSOR_HandleTypeDef * phsensor); /* function pointer on Rx ISR */
	//int16_t (*SensorGetValue)(); /* function pointer on Rx ISR */
	int16_t (*SenGetPowerVal)(); /* function pointer on Rx ISR */
	int16_t (*SenGetKeTiVal)(); /* function pointer on Rx ISR */
	int16_t (*SenGetJinKouVal)(); /* function pointer on Rx ISR */
	int16_t (*SenGetChuKouVal)(); /* function pointer on Rx ISR */
	int16_t (*SenGetYouBengFbVal)(); /* function pointer on Rx ISR */
	int16_t (*SenGetHuoSaiFbVal)(); /* function pointer on Rx ISR */
	int16_t(*SenGetStateRunVal)(); /* function pointer on Rx ISR */

	//void(*OutStop)(struct __SENSOR_HandleTypeDef * phsensor); /* function pointer on Rx ISR */
																											
}SENSOR_HandleTypeDef;
typedef struct
{
	uint8_t		Time_s;
	uint8_t		Time_min;
	uint8_t     Time_s_up_flag;
	uint8_t     Time_100ms_up_flag;
	uint16_t	Time_ms;
}TIME_TypeDef;
typedef struct __TIME_HandleTypeDef {
	TIME_TypeDef SysTime;//���״̬
	void(*TimeInit)(struct __TIME_HandleTypeDef * phtime);
	void(*TimeUpdata)(struct __TIME_HandleTypeDef * phtime); /* function pointer on Rx ISR */
	void(*TimeReset)(struct __TIME_HandleTypeDef * phtime); /* function pointer on Rx ISR */

	uint16_t StateMachineRun_s;
}TIME_HandleTypeDef;



typedef struct __KEY_HandleTypeDef {
	KEY_STATE_TypeDef keyState;
	KEY_STATE_TypeDef keyStateEver;

	void(*KeyStateInit)(struct __KEY_HandleTypeDef * phkey);
	void(*KeyStateSet)(KEY_STATE_TypeDef keyState); /* function pointer on Rx ISR */
	KEY_STATE_TypeDef(*KeyStateGet)(); /* function pointer on Rx ISR */
	KEY_STATE_TypeDef(*KeyStateGetClear)(); /* function pointer on Rx ISR */

}KEY_HandleTypeDef;
typedef enum
{
	ALARM_ToPowerOff = 0x01,//����ͣ��1
	ALARM_CommError,//ͨ�Ź���2
	ALARM_WindStop,//�������3
	ALARM_HuoSaiError,//��������4
	ALARM_YouBengError,//�ͱù���5
	ALARM_TempOver,//�¶ȹ��ȣ����ڳ���150��6
	ALARM_PowerError,//��ѹ����7
	ALARM_OutHeat2,//���ε��ʧ�� 8
	ALARM_Heat2,//���ε�� 9

	ALARM_NormalHeat,//��������ָʾ 10
	ALARM_NormalNone,//�ر�ָʾ 11
	ALARM_ToStop,//ͣ��12
	ALARM_NoChange,//�޹����л�13
	ALARM_Idel,//��״ָ̬ʾ14

}ALARM_STATE_TypeDef;

typedef struct __ALARM_HandleTypeDef {
	ALARM_STATE_TypeDef  AlarmState;
	void(*AlarmInit)(struct __ALARM_HandleTypeDef * phalarm);
	void (*AlarmCheck)(struct __ALARM_HandleTypeDef * phalarm);
	uint16_t ALARM_NS_KeTiLowTemp;

}ALARM_HandleTypeDef;
typedef enum
{
	MODE_2KW = 0x02,//ģʽ
	MODE_3KW = 0x03,//ģʽ
	MODE_5KW = 0x05,//ģʽ
	MODE_TEST = 0x80,//ģʽ


}MODE_XKW_TypeDef;
typedef struct __PARM_TypeDef {
	uint16_t Start_s;
	uint16_t Stop_s;
	uint16_t Pre;

}PARM_TypeDef;

typedef struct __PARM_HandleTypeDef {
	void(*ParmInit)(struct __PARM_HandleTypeDef * phparm);
	void(*ParmSet)(struct __PARM_HandleTypeDef * phparm);
	uint8_t FS_StartPrmPre;
	uint8_t FS1000PrmPre;
	uint8_t FS1500PrmPre;
	uint8_t FS2000PrmPre;
	uint8_t FS2500PrmPre;
	uint8_t FS3000PrmPre;
	uint8_t FS3500PrmPre;
	uint8_t FS4000PrmPre;
	uint8_t FS4500PrmPre;
	uint8_t FS5000PrmPre;

	uint8_t HS_StartPrmPre;
	uint8_t HS14V0PrmPre;
	uint8_t HS14V5PrmPre;
	uint8_t HS15V0PrmPre;
	uint8_t HS15V5PrmPre;
	uint8_t HS16V0PrmPre;
	uint8_t HS16V5PrmPre;

	uint16_t FS_PreToYB_Hz;
	MODE_XKW_TypeDef ModeXkw;


	//start �������������ñ�
	
	uint16_t HEAT_FS_EN_Time;
	PARM_TypeDef HEAT_FS_D1;
	PARM_TypeDef HEAT_FS_D2;
	PARM_TypeDef HEAT_FS_D3;
	PARM_TypeDef HEAT_FS_D4;
	PARM_TypeDef HEAT_FS_D5;
	uint16_t HEAT_FS_DIS_Time;

	uint16_t HEAT_HS_EN_Time;
	PARM_TypeDef HEAT_HS_D1;
	PARM_TypeDef HEAT_HS_D2;
	PARM_TypeDef HEAT_HS_D3;
	PARM_TypeDef HEAT_HS_D4;
	PARM_TypeDef HEAT_HS_D5;
	uint16_t HEAT_HS_DIS_Time;

	uint16_t HEAT_YB_EN_Time;
	uint16_t HEAT_YB_ADJ_Time;

	uint16_t HEAT_KT_StartTime;
	uint16_t HEAT_KT_JudgeTime;


	uint16_t HEAT2_FS_EN_Time;
	PARM_TypeDef HEAT2_FS_D1;
	PARM_TypeDef HEAT2_FS_D2;
	PARM_TypeDef HEAT2_FS_D3;
	PARM_TypeDef HEAT2_FS_D4;
	PARM_TypeDef HEAT2_FS_D5;

	PARM_TypeDef HEAT2_FS_OFST_D1;
	PARM_TypeDef HEAT2_FS_OFST_D2;
	PARM_TypeDef HEAT2_FS_OFST_D3;
	PARM_TypeDef HEAT2_FS_OFST_D4;
	PARM_TypeDef HEAT2_FS_OFST_D5;

	uint16_t HEAT2_FS_DIS_Time;

	uint16_t HEAT2_HS_EN_OFST_Time;
	PARM_TypeDef HEAT2_HS_OFST_D1;
	PARM_TypeDef HEAT2_HS_OFST_D2;
	PARM_TypeDef HEAT2_HS_OFST_D3;
	PARM_TypeDef HEAT2_HS_OFST_D4;
	PARM_TypeDef HEAT2_HS_OFST_D5;
	uint16_t HEAT2_HS_DIS_Time;

	uint16_t HEAT2_YB_EN_OFST_Time;
	uint16_t HEAT2_YB_ADJ_OFST_Time;

	uint16_t HEAT2_KT_HeatOffStartTime;
	uint16_t HEAT2_KT_HeatOffJudgeTime;

	uint16_t HEAT2_KT_DROPDOWN_START_HEAT;
	uint16_t HEAT2_START_HEAT_Time;
	
	PARM_TypeDef WIND_FS_D1;
	PARM_TypeDef WIND_FS_D2;

	PARM_TypeDef NORMAL_FS_D1;
	PARM_TypeDef NORMAL_FS_D2;

	uint16_t NORMAL_HS_DIS_Time;
	uint16_t NORMAL_START_SWLEVEL_Time;

	uint16_t NORMAL_STATE_LEVEL_HOLD_Second;
	uint16_t NORMAL_DWQ_JK_TEMP_Cpmpare;
	uint16_t NORMAL_STATE_SWITCH_LOW_LEVEL_DELAY_Second;
	uint16_t NORMAL_STATE_SWITCH_HIGH_LEVEL_DELAY_Second;
	uint16_t NORMAL_STATE_SWITCH_LEVEL_Second;

	uint16_t NORMAL_STATE_HEAT_OUT_JUDGE_Time;
	uint16_t NORMAL_STATE_HEAT_OUT_JUDGE_KeTiTemp;
	uint16_t NORMAL_STATE_CHUKOU_DROPDOWN_Temp;
	uint16_t NORMAL_STATE_KETI_DROPDOWN_Temp;


	//stop  ģʽ
	PARM_TypeDef STOP_FS_D1;
	PARM_TypeDef STOP_FS_D2;
	
	PARM_TypeDef STOP_HS_D1;
	PARM_TypeDef STOP_HS_D2;
	uint16_t STOP_HS_DIS_Time;

	//end �������������ñ�
	uint16_t HEAT_STATE_KETI_RISE_TEMP;

}PARM_HandleTypeDef;
typedef struct __HUOER_HandleTypeDef {
	void(*HuoerInit)(struct __HUOER_HandleTypeDef * phhuoer);
	void(*HuoerSetTime)(int time_ms);
	int(*HuoerGetRpm)();
	
}HUOER_HandleTypeDef;
typedef struct __HEAT_HandleTypeDef {
	STATE_MACHINE_TypeDef StateMachine;
	STATE_MACHINE_TypeDef StateMachineNext;
	STATE_RUN_LEVEL_TypeDef  StateRunLevel;
	int16_t StartKeTiTemp;
	uint8_t HEAT2_KT_START_HEAT_Flag;
	uint16_t HEAT2_KT_START_HEAT_Time;
	void(*pPrintCurState)(struct __HEAT_HandleTypeDef * phheat);
	void(*pStateMachineInit)(struct __HEAT_HandleTypeDef * phheat);
	void(*peMBPoll)();
	void (*pStateMachineAdjest)(struct __HEAT_HandleTypeDef * phheat);
	OUT_HandleTypeDef  hHuoSai;
	//uint16_t huoSai_SetPre;//����ռ�ձ�
	//uint16_t huoSai_CurPre;//������
	//uint16_t (*setHuoSaiParm)(struct __HEAT_HandleTypeDef * phheat);
	//OUT_HandleTypeDef  *phHuoSai;
	OUT_HandleTypeDef  hFengShan;
	//uint16_t fengShan_SetPre;//����ռ�ձ�
	//uint16_t fengShan_CurPre;//������
	//uint16_t fengShan_SetPrm;//����ռ�ձ�
	//uint16_t fengShan_CurPrm;//������
	//uint16_t (*setfengSHanParm)(struct __HEAT_HandleTypeDef * phheat);


	//OUT_HandleTypeDef  *phFengShan;
	OUT_HandleTypeDef  hYouBeng;
	
	//OUT_HandleTypeDef  *phYouBeng;
	SENSOR_HandleTypeDef hSensor;
	//SENSOR_HandleTypeDef hPower;
	//SENSOR_HandleTypeDef *phPower;
	//SENSOR_HandleTypeDef hKeTi;
	//SENSOR_HandleTypeDef *phKeTi;
	//SENSOR_HandleTypeDef hJinKou;
	//SENSOR_HandleTypeDef *phJinKou;
	//SENSOR_HandleTypeDef hChuKou;
	//SENSOR_HandleTypeDef *phChuKou;
	//SENSOR_HandleTypeDef hHuoSaiFb;
	//SENSOR_HandleTypeDef *phHuoSaiFb;
	//SENSOR_HandleTypeDef hYouBengFb;
	//SENSOR_HandleTypeDef *phYouBengFb;
	TIME_HandleTypeDef  hSysTime;
	//TIME_HandleTypeDef  *phSysTime;
	KEY_HandleTypeDef   hKey;
	//KEY_HandleTypeDef   *phKey;
	ALARM_HandleTypeDef  hAlarm;
	ALARM_STATE_TypeDef  AlarmState;
	//ALARM_HandleTypeDef  *phAlarm;
	PARM_HandleTypeDef hParm;
	HUOER_HandleTypeDef hHuoer;
	//void(*STATE_Machine)();

	uint16_t HW_Version;

	int16_t JinKouTemp;
	int16_t(*getJinKouTemp)(struct __HEAT_HandleTypeDef * phheat);
	
	int16_t KeTiTemp;
	int16_t(*getkeTiTemp)(struct __HEAT_HandleTypeDef * phheat);

	int16_t ChuKouTemp;
	int16_t(*getchuKouTemp)(struct __HEAT_HandleTypeDef * phheat);

	uint16_t PowerVal_M100;
	uint16_t(*getPowerVal)(struct __HEAT_HandleTypeDef * phheat);

	uint16_t Prm;
	uint16_t YouBengHz;


	uint16_t CurrentPrm;
	uint16_t CurrentPrmTest;
	
	uint16_t MyTest;

	uint16_t TargetPrm;
	uint16_t(*pSetFenShanPre)(int16_t adjust);
uint8_t test;
uint32_t RunTime;
	


}HEAT_HandleTypeDef;


void StateMachineIdel(struct __HEAT_HandleTypeDef *phheat);

void StateMachineInit(struct __HEAT_HandleTypeDef *phheat);

//
//void StateMachineIdel();

void StateMachineWind(struct __HEAT_HandleTypeDef *phheat);
void StateMachineHeat(struct __HEAT_HandleTypeDef *phheat);
void StateMachineHeat2(struct __HEAT_HandleTypeDef *phheat);
void StateMachineNormal(struct __HEAT_HandleTypeDef *phheat);
void StateMachineStop(struct __HEAT_HandleTypeDef *phheat);

void PrintCurrentState(struct __HEAT_HandleTypeDef *phheat);
void StateMachineAdjust(struct __HEAT_HandleTypeDef *phheat);


uint16_t SetPreFengShan(int16_t adjust);
#endif