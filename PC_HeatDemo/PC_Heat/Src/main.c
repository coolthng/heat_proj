//#include "../../Heat/Src/heat.h"
#include "heat.h"//$(ProjectDir)\..\Heat\Inc



//USHORT   usRegInputBuf[REG_INPUT_NREGS];
USHORT   usRegInputBuf[20];


HEAT_HandleTypeDef hheat;

HEAT_HandleTypeDef *phheat;
//start  test

//end test

#include "fileapi.h"
int main(void)
{
	
	eMBInit(MB_RTU, 0x01, 1, 115200, MB_PAR_NONE);//modbus初始化
#if 0//测试模式
	while (1)
	{
		eMBPoll();
		Sleep(100);

	}
#endif
	

	HEAT_Init(&hheat);
	//start  测试函数
	
	
	//hheat.hHuoSai.OutStart(&hheat.hHuoSai);

	

	

	//end  测试函数
	
	hheat.StateMachine = STATE_MACHINE_IDEL;//置位为空状态
	hheat.StateMachineNext = STATE_MACHINE_IDEL;

	while (1)
	{
		switch (hheat.StateMachine)
		{

		case STATE_MACHINE_POWER_OFF:
			StateMachinePowerOff(&hheat);;
			break;
		case STATE_MACHINE_NORMAL:
			//StateMachineNormal(&hheat);
			break;
		case STATE_MACHINE_WIND:
			StateMachineWind(&hheat);
			break;
		case STATE_MACHINE_HEAT:
			StateMachineHeat(&hheat);
			break;
		case STATE_MACHINE_STOP:
			//StateMachineStop(&hheat);
			break;
		case STATE_MACHINE_HEAT2:
			//StateMachineHeat2(&hheat);
			break;
		case STATE_MACHINE_IDEL:
			StateMachineIdel(&hheat);
			
			break;
		case STATE_MACHINE_DEBUG:
			StateMachineDebug(&hheat);

			break;
		
		default:
			StateMachineIdel(&hheat);
			break;
		}
	}


}
