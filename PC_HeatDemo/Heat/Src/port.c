#include "heat.h"


extern HEAT_HandleTypeDef hheat;

#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
#define DEBUG_PRINT(x)    printf(x)  //NULL
#else
#include "stm32f0xx_hal.h"
extern TIM_HandleTypeDef htim17;
#define DEBUG_PRINT(x)   NULL
#endif
int16_t PTgetJinKouTemp()
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取进口温度\n");

	return  100;
}
int16_t PTgetChuKouTemp()
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取出口温度\n");

	return 100;
}
int16_t PTgetKeTiTemp()
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取壳体温度\n");

	return 50;
}
int16_t PTgetPowerVal()
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取电源电压\n");

	return 2400;
}
uint16_t PTgetHsFbVal()
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取火花塞反馈\n");
	return 100;
}
uint16_t PTgetYbFbVal()
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取油泵反馈\n");
	return 100;
}
uint16_t PTgetFsFbVal()
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取风扇反馈\n");
	return 100;
}
uint16_t PTgetHallFbVal()
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取霍尔风扇转速\n");
#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	return hheat.TargetPrm;
#else
	
	extern uint32_t HuoErInput[10];
	static uint32_t CapValue=0;
	for(int i=0;i<HUO_ER_INPUT_LENGTH;i++)
	{
		CapValue+=HuoErInput[i];
		if(HuoErInput[i]>0xfffffff)
		return 0;
	}
	CapValue=CapValue>>2;
	//hheat.CurrentPrm=(100000.0 * 60 * HUO_ER_INPUT_LENGTH)/CapValue;
	//hheat.CurrentPrm=(100000.0 * 60)/CapValue;
	return (uint16_t)((100000.0 * 60)/CapValue);//(uint16_t)((100000 * 60)/HuoErInput[0]);/////(uint16_t)((100000.0 * 60*HUO_ER_INPUT_LENGTH)/CapValue);
	
#endif
	
}

int PTgetUserSetTemp()
{
	//添加进口获取进口温度
	DEBUG_PRINT("获取霍设定转速\n");
	return 100;
}

uint16_t PTsetPreFengShan(int16_t adjust)
{
	static int16_t current_pre = 0;
	if (adjust == 0)//如果为0 清空输出
	{
		current_pre += adjust;
		if (current_pre < 10)
			current_pre = 0;
	}
	else
	{
		current_pre += adjust;
		if (current_pre < 10)
			current_pre = 0;
	}
#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	hheat.CurrentPrm = current_pre;
	hheat.CurrentPrmTest = current_pre;
	printf("当前转速:%d,adjust:%d\n", hheat.CurrentPrm, adjust);
#else

	hheat.CurrentPrmTest = current_pre;
	htim17.Instance->CCR1 = current_pre;
#endif
}


uint16_t PTsetHsVolt(uint16_t voltageValue)
{
	hheat.CurrentHsPre = voltageValue;
#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)

	printf("设置火花塞电压：%d", voltageValue);
#else
#endif
}
uint16_t PTsetYbHz(uint16_t ybHz)
{
	hheat.CurrentYbHz = ybHz;
#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	printf("设置油泵频率：%d", ybHz);
#else
#endif
}

int16_t readParmFromFlash(HeatParm *pHeatParm)
{
#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	uint16_t DefaultParm[] = { 0,5,0,1,10,1000,50,60,1500,90,120,2000,0,0,0,0,0,0,4,4,8,500,8,12,1400,0,0,0,0,0,0,0,0,0,60,60,65,7,60,150,70,90,0,10,1000,60,70,2000,0,0,0,0,0,0,0,0,0,1,10,1000,50,60,1500,90,120,2000,0,0,0,0,0,0,50,55,1500,110,115,0,0,0,0,0,0,0,0,0,0,4,10,1500,0,0,0,0,0,0,0,0,0,0,0,0,60,65,0,10,2000,55,60,1500,0,0,0,10,15,1500,50,55,0,0,0,0,60,70,0,10,2000,0,0,0,0,0,0 };
	uint16_t deParmLen = sizeof(DefaultParm);
	for (int i = 0; i < deParmLen; i++)
	{
		pHeatParm->un_parm[i] = DefaultParm[i];
	}
	uint16_t crc_16 = 0;
	for (int i = 0; i < pHeatParm->st.PARM_LENGTH; i++)
	{
		crc_16 += pHeatParm->un_parm[i];
	}
	if (crc_16 == pHeatParm->st.PARM_CRC)
	{
		return 0;
	}
	else
	{
		return -1;
	}
	printf("获取配置参数");
#else

	return -1;
#endif
}
