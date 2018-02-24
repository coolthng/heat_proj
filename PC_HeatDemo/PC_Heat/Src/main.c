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
	CommInit(MB_RTU, 0x01, 1, 115200, MB_PAR_NONE);//通信初始化
	
	HEAT_Init(&hheat);
	//start  测试函数
	HEAT_Poll(&hheat);

	while (1)
	{

	}
	
	//end  测试函数
	
	


}
