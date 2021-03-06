#ifndef __HEAT_H
#define __HEAT_H
#include "StateMachine.h"
#include "PlateFormConfig.h"
#include "comm.h"
#include "pwm.h"
#include "adc.h"
#include "time.h"
#include "key.h"
#include "alarm.h"
#include "parm.h"
#include "huoer.h"
#include "port.h"

#include "queue.h"

#include "stdint.h"
#include "stdio.h"

#define HUO_ER_INPUT_LENGTH 4
typedef struct sensor_s {
	int value;


}sensor_t;
typedef struct out_s {
	char state;

}out_t;

typedef union __HEAT_DIS
{
	struct st
	{
		uint8_t comnd;
		uint8_t HsVoltage;
		uint8_t AlarmState : 4;
		uint8_t StateMachine : 4;
		uint8_t noused : 4;
		uint8_t StateRunLevel : 4;
		uint8_t StateMachineRun_s;
		uint8_t JinKouTemp;
		uint16_t CurrentPrm;
		int16_t KeTiTemp;//10
		uint16_t PowerVolatge;//12
		uint16_t YouBengHz;//14
		int16_t resav3;//16
		int16_t resav4;//18
		uint8_t tail1;
		uint8_t tail2;

	}st;


	uint8_t un_uint8_t[20];

}HEAT_DIS;

typedef enum
{
	HEAT_OK = 0x00,
	HEAT_ERROR = 0x01,
	HEAT_BUSY = 0x02,
	HEAT_TIMEOUT = 0x03
} HEAT_StatusTypeDef;
typedef enum
{
	STATE_MACHINE_HEAT = (uint8_t)0x01,/*加热状态1*/
	STATE_MACHINE_WIND,/*通风状态2*/
	STATE_MACHINE_NORMAL,/*正常状态3*/
	STATE_MACHINE_HEAT2,/*二次点火4*/
	STATE_MACHINE_STOP,/*关机状态5*/
	STATE_MACHINE_POWER_OFF,/*立即关机状态6*/
	STATE_MACHINE_IDEL,/*空状态7*/
	STATE_MACHINE_TEST,/*测试状态*/
	STATE_MACHINE_AUTO_OIL,/*自动泵油状态*/
	STATE_MACHINE_DEBUG,/*测试状态*/

}STATE_MACHINE_TypeDef;

typedef enum __KEY_STATE_TypeDef
{
	KEY_STATE_NONE = 0x00,  /*非法按键*/
	KEY_STATE_HEAT,  /*加热按键*/
	KEY_STATE_WIND,  /*风机按键*/
	KEY_STATE_STOP,  /*停机按键*/
	KEY_STATE_ADD,  /*升档按键*/
	KEY_STATE_SUB,  /*将挡按键*/
	KEY_STATE_AUTO_OIL,  /*自动上油键*/
	KEY_STATE_TEST,  /*测试模式键*/
	KEY_STATE_DEBUG,  /*调试模式键*/
	KEY_STATE_SW_2_HEAT_F_STOP,
	KEY_STATE_SW_2_HEAT_F_IDEL,
	KEY_STATE_SW_2_HEAT_F_WIND,

}KEY_STATE_TypeDef;
typedef enum
{
	STATE_RUN_LEVEL_1 = (uint8_t)0x01,/*运行档位1档*/
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
	OUT_STATE_TypeDef OutState;//输出状态
	uint16_t setPre;
	uint16_t curPre;
	//uint16_t OutFre;
	void(*OutInit)(struct __OUT_HandleTypeDef * phout); /* function pointer on Rx ISR */
	void(*OutStart)(struct __OUT_HandleTypeDef * phout); /* function pointer on Rx ISR */
	void(*OutStop)(struct __OUT_HandleTypeDef * phout); /* function pointer on Rx ISR */
	void(*OutSetParm)(struct __HEAT_HandleTypeDef * pheat ); /* function pointer on Rx ISR */
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

typedef struct
{
	uint8_t		Time_s;
	uint8_t		Time_min;
	uint8_t     Time_s_up_flag;
	uint8_t     Time_100ms_up_flag;
	uint16_t	Time_ms;
}TIME_TypeDef;
typedef struct __TIME_HandleTypeDef {
	TIME_TypeDef SysTime;//输出状态
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
	ALARM_ToPowerOff = 0x01,//掉电停机1
	ALARM_CommError,//通信故障2
	ALARM_WindStop,//电机故障3
	ALARM_HuoSaiError,//火花塞故障4
	ALARM_YouBengError,//油泵故障5
	ALARM_TempOver,//温度过热，出口超过150度6
	ALARM_PowerError,//电压故障7
	ALARM_OutHeat2,//二次点火失败 8
	ALARM_Heat2,//二次点火 9

	ALARM_NormalHeat,//正常加热指示 10
	ALARM_NormalNone,//关闭指示 11
	ALARM_ToStop,//停机12
	ALARM_NoChange,//无故障切换13
	ALARM_Idel,//空状态指示14

}ALARM_STATE_TypeDef;

typedef struct __ALARM_HandleTypeDef {
	ALARM_STATE_TypeDef  AlarmState;
	void(*AlarmInit)(struct __ALARM_HandleTypeDef * phalarm);
	void (*AlarmCheck)(struct __ALARM_HandleTypeDef * phalarm);
	uint16_t ALARM_NS_KeTiLowTemp;

}ALARM_HandleTypeDef;
typedef enum
{
	MODE_2KW = 0x02,//模式
	MODE_3KW = 0x03,//模式
	MODE_5KW = 0x05,//模式
	MODE_TEST = 0x80,//模式


}MODE_XKW_TypeDef;
typedef struct __PARM_TypeDef {
	uint16_t Start_s;
	uint16_t Stop_s;
	uint16_t parm;

}PARM_TypeDef;
typedef union __HeatParm {
	struct {
		//点火参数

		uint16_t HEAT_KT_START_TEMP;
		uint16_t HEAT_KETI_RISE_TEMP;
		uint16_t HEAT_FS_EN_Time;
		PARM_TypeDef HEAT_FS_D1;


		PARM_TypeDef HEAT_FS_D2;


		PARM_TypeDef HEAT_FS_D3;


		PARM_TypeDef HEAT_FS_D4;


		PARM_TypeDef HEAT_FS_D5;
		uint16_t HEAT_HS_EN_Time;


		PARM_TypeDef HEAT_HS_D1;


		PARM_TypeDef HEAT_HS_D2;


		PARM_TypeDef HEAT_HS_D3;


		PARM_TypeDef HEAT_HS_D4;


		PARM_TypeDef HEAT_HS_D5;
		uint16_t HEAT_YB_EN_Time;
		uint16_t HEAT_YB_StaticHz;
		uint16_t HEAT_YB_ADJ_Time;
		uint16_t HEAT_YB_DynamicParm;
		uint16_t HEAT_KT_StartTime;
		uint16_t HEAT_KT_JudgeTime;
		uint16_t HEAT2_KT_DROPDOWN_START_HEAT;
		uint16_t HEAT2_START_HEAT_Time;


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


		PARM_TypeDef HEAT2_HS_D1;


		PARM_TypeDef HEAT2_HS_D2;


		PARM_TypeDef HEAT2_HS_D3;


		PARM_TypeDef HEAT2_HS_D4;


		PARM_TypeDef HEAT2_HS_D5;


		PARM_TypeDef HEAT2_HS_OFST_D1;


		PARM_TypeDef HEAT2_HS_OFST_D2;


		PARM_TypeDef HEAT2_HS_OFST_D3;


		PARM_TypeDef HEAT2_HS_OFST_D4;


		PARM_TypeDef HEAT2_HS_OFST_D5;
		uint16_t HEAT2_YB_EN_OFST_Time;
		uint16_t HEAT2_YB_ADJ_OFST_Time;


		PARM_TypeDef STOP_FS_D1;


		PARM_TypeDef STOP_FS_D2;


		PARM_TypeDef STOP_FS_D3;


		PARM_TypeDef STOP_HS_D1;


		PARM_TypeDef STOP_HS_D2;


		PARM_TypeDef STOP_HS_D3;
		uint16_t STOP_SW2OFF_Time;
		uint16_t STOP_SW2OFF_KetiTemp;


		PARM_TypeDef WIND_FS_D1;


		PARM_TypeDef WIND_FS_D2;


		PARM_TypeDef WIND_FS_D3;

		uint16_t PARM_LENGTH;
		uint16_t PARM_CRC;


	}st;
	uint16_t un_parm[135];

}HeatParm;

typedef struct __PARM_HandleTypeDef {
	void(*ParmInit)(struct __PARM_HandleTypeDef * phparm);
	void(*ParmDefSet)(struct __PARM_HandleTypeDef * phparm);
	void(*ParmSet)(struct __PARM_HandleTypeDef * phparm,union __HeatParm *pHeatParm);

	uint16_t HEAT_KT_START_TEMP;//开始点火壳体温度，大于某一温度，需要吹凉后再点火
	
	uint16_t HEAT_YB_StaticHz;//点火时油泵频率
	uint16_t HEAT_YB_DynamicParm;//点火时油泵



	
	uint16_t FS_PreToYB_Hz;
	MODE_XKW_TypeDef ModeXkw;


	//start 启动参数化配置表
	
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

	//start 二次点火参数
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

	uint16_t HEAT2_EN_OFST_Time;
	uint16_t HEAT2_EN_OFST_KT_Temp;

	PARM_TypeDef HEAT2_HS_D1;
	PARM_TypeDef HEAT2_HS_D2;
	PARM_TypeDef HEAT2_HS_D3;
	PARM_TypeDef HEAT2_HS_D4;
	PARM_TypeDef HEAT2_HS_D5;

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
	PARM_TypeDef WIND_FS_D3;

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


	//stop  模式
	PARM_TypeDef STOP_FS_D1;
	PARM_TypeDef STOP_FS_D2;
	PARM_TypeDef STOP_FS_D3;
	
	PARM_TypeDef STOP_HS_D1;
	PARM_TypeDef STOP_HS_D2;
	PARM_TypeDef STOP_HS_D3;

	uint16_t STOP_HS_DIS_Time;
	uint16_t STOP_SW2OFF_Time;
	uint16_t STOP_SW2OFF_KetiTemp;
	//end 启动参数化配置表
	uint16_t HEAT_KT_RISE_TEMP;

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
	
	void(*pStateMachineInit)(struct __HEAT_HandleTypeDef * phheat);
	void(*pCommPoll)();
	void (*pStateMachineAdjest)(struct __HEAT_HandleTypeDef * phheat);
	void (*pStateMachineUpdate)(struct __HEAT_HandleTypeDef * phheat);
	OUT_HandleTypeDef  hHuoSai;
	//uint16_t huoSai_SetPre;//设置占空比
	//uint16_t huoSai_CurPre;//调试用
	//uint16_t (*setHuoSaiParm)(struct __HEAT_HandleTypeDef * phheat);
	//OUT_HandleTypeDef  *phHuoSai;
	OUT_HandleTypeDef  hFengShan;
	//uint16_t fengShan_SetPre;//设置占空比
	//uint16_t fengShan_CurPre;//调试用
	//uint16_t fengShan_SetPrm;//设置占空比
	//uint16_t fengShan_CurPrm;//调试用
	//uint16_t (*setfengSHanParm)(struct __HEAT_HandleTypeDef * phheat);


	//OUT_HandleTypeDef  *phFengShan;
	OUT_HandleTypeDef  hYouBeng;
	
	//OUT_HandleTypeDef  *phYouBeng;
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
	int16_t(*getjinKouTemp)();
	
	int16_t KeTiTemp;
	int16_t(*getkeTiTemp)();

	int16_t ChuKouTemp;
	int16_t(*getchuKouTemp)();

	int16_t UserSetTemp;
	int16_t(*getuserSetTemp)();

	uint16_t PowerVal_M100;
	uint16_t(*getPowerVal)();

	uint16_t HsFbVal;
	uint16_t(*getHsFbVal)();

	uint16_t FsFbVal;
	uint16_t(*getFsFbVal)();

	uint16_t YbFbVal;
	uint16_t(*getYbFbVal)();
	
	uint16_t YouBengHz;

	
	uint16_t CurrentPrm;//当前转速
	uint16_t(*getHallFbVal)();
	uint16_t CurrentPrmTest;
	uint16_t CurrentYbHz;//当前油泵频率

	uint16_t MyTest;

	uint16_t TargetPrm;
	uint16_t TargetHsVolt;//火花塞电压
	uint16_t CurrentHsPre;//火花塞电压

	uint16_t(*pSetFenShanPre)(int16_t adjust);
	uint16_t(*pSetHsVolt)(uint16_t );
	uint16_t(*pSetYbHz)(uint16_t );
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

#endif