#include "heat.h"

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
#else
#include "stm32f0xx_hal.h"
#endif

#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)

#else
static uint8_t TxArrTmp[200];
int  HAL_UART_Transmit_DMA_my(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	//Size=10;
	//TxArrTmp[0]=Size;
//memcpy(&TxArrTmp[1],pData,Size);
HAL_UART_Transmit_DMA(huart,pData,Size);
}


#endif
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
#include <stdio.h>
HANDLE hCom;
void CommInit(eMBMode eMode, UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity)
{

	hCom = CreateFile("COM1",//COM1口
		GENERIC_READ | GENERIC_WRITE, //允许读和写
		0, //独占方式
		NULL,
		OPEN_EXISTING, //打开而不是创建
		0, //同步方式
		NULL);
	if (hCom == (HANDLE)-1)
	{
		printf("open com faile!\n");
		return FALSE;
	}
	else
	{
		printf("open COM success \n");
	}
	SetupComm(hCom, 1024, 1024); //输入缓冲区和输出缓冲区的大小都是1024
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout = 10;
	TimeOuts.ReadTotalTimeoutMultiplier = 50;
	TimeOuts.ReadTotalTimeoutConstant = 100;
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 500;
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hCom, &TimeOuts); //设置超时
	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = ulBaudRate; //波特率为9600
	dcb.ByteSize = 8; //每个字节有8位
	dcb.Parity = NOPARITY; //无奇偶校验位
	dcb.StopBits = ONESTOPBIT;// ONE5STOPBITS; //两个停止位
	SetCommState(hCom, &dcb);
	
}
#else
#endif

extern USHORT   usRegInputBuf[20];
uint16_t HeatParmArr[300];
void CommPoll()
{
	static uint16_t res;
	static uint8_t tmp = 0;
	extern HEAT_HandleTypeDef hheat;
	static uint8_t MB_RxBuf[80];
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	static DWORD wCount;//读取的字节数
	static DWORD rCount;
	static BOOL bWriteStat;
	static uint8_t TxLength = 0;
	static BOOL bReadStat;
	static uint8_t RxArr[100];
	static uint8_t TxArr[500];
	static uint8_t RxArrTmp = 0;
	static uint8_t RxArrPtr = 0;
	static uint8_t MB_TxBuf[80];
	static uint8_t RxArrFlag;
	//PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR); //清空缓冲区
		//printf("%s\n", str);
	do {
		bReadStat = ReadFile(hCom, &RxArrTmp, 1, &rCount, NULL);
		
		
		if (rCount)
		{
			RxArr[RxArrPtr++] = RxArrTmp;
			if (RxArrPtr > 2 && RxArr[RxArrPtr - 2] == 0x0d && RxArr[RxArrPtr - 1] == 0x0a)
			{
				RxArrPtr = 0;
				RxArrFlag = 1;
				break;
			}
		}
		else
		{
			break;
		}
	} while (bReadStat);
	
	hheat.hSysTime.SysTime.Time_s_up_flag = 1;
#else
	extern uint8_t RxArrFlag;
	extern uint8_t RxArr[100];
	extern uint8_t TxArr[500];
	extern UART_HandleTypeDef huart1;
	hheat.RunTime=HAL_GetTick()/1000;
	
#endif

		
		if(RxArrFlag)
		{
			RxArrFlag=0;
			MB_RxBuf[0]=0;
			switch (RxArr[0])
			{
			case 'a':////扫描更新显示状态
				static HEAT_DIS heatDis;
				heatDis.st.comnd = 'a';
				heatDis.st.HsVoltage = hheat.TargetHsVolt / 10;
				heatDis.st.AlarmState = hheat.AlarmState;
				heatDis.st.StateMachine = hheat.StateMachine;
				heatDis.st.StateRunLevel = hheat.StateRunLevel;
				heatDis.st.StateMachineRun_s= hheat.hSysTime.StateMachineRun_s;
				heatDis.st.JinKouTemp= hheat.JinKouTemp;
				heatDis.st.CurrentPrm= hheat.CurrentPrm;// hheat.Prm;
				heatDis.st.KeTiTemp= hheat.KeTiTemp;
				heatDis.st.PowerVolatge = hheat.PowerVal_M100;
				heatDis.st.YouBengHz = hheat.CurrentYbHz;
				heatDis.st.resav3 =hheat.CurrentPrmTest;//hheat.MyTest;
				heatDis.st.resav4 = hheat.TargetPrm;
				heatDis.st.tail1 = 0x0d;
				heatDis.st.tail2 = 0x0a;

				for (int i = 0; i < 20; i++)
				{
					TxArr[i] = heatDis.un_uint8_t[i];
					
				}

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
				//bWriteStat = WriteFile(hCom, MB_TxBuf, TxLength + 5, &wCount, NULL);//发出
				static uint8_t TxArr22[500];
				for (int i = 0; i < 20; i++)
				{
					TxArr22[i + 1] = TxArr[i];
				}
				TxArr22[0] = 20;
				//Sleep(100);
				bWriteStat = WriteFile(hCom, TxArr, 20, &wCount, NULL);//发出
				//bWriteStat = WriteFile(hCom, TxArr22, 21, &wCount, NULL);//发出
#else
					RS485_TX;
					HAL_Delay(5);
				HAL_UART_Transmit_DMA_my(&huart1,TxArr,20);
				
#endif
				extern HEAT_HandleTypeDef hheat;
				//hheat.hSysTime.SysTime.Time_s_up_flag = 1;
				break;
			case 'b':
				hheat.hKey.KeyStateSet(KEY_STATE_DEBUG);
				TxArr[0]='s';
				TxArr[1]=0x0d;
				TxArr[2]=0x0a;
			TxArr[3]=NULL;
			
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
				//bWriteStat = WriteFile(hCom, MB_TxBuf, TxLength + 5, &wCount, NULL);//发出
				bWriteStat = WriteFile(hCom, TxArr, 3, &wCount, NULL);//发出
#else
					RS485_TX;
			HAL_Delay(5);
				HAL_UART_Transmit_DMA_my(&huart1,TxArr,3);
				
#endif
				
				break;
			case 'c':
				hheat.hKey.KeyStateSet(KEY_STATE_STOP);
				TxArr[0]='s';
				TxArr[1]=0x0d;
				TxArr[2]=0x0a;
			
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
				//bWriteStat = WriteFile(hCom, MB_TxBuf, TxLength + 5, &wCount, NULL);//发出
				bWriteStat = WriteFile(hCom, TxArr, 3, &wCount, NULL);//发出
#else
					RS485_TX;
			HAL_Delay(5);
				HAL_UART_Transmit_DMA_my(&huart1,TxArr,3);
				
#endif
				
				break;
			case 's':
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
				//ReadFile(hCom, MB_RxBuf, 20, &rCount, NULL);//再读20
#endif
				/*hheat.hSensor.SenTestPower = *(int16_t*)(RxArr + 2);
				hheat.hSensor.SenTestDianWeiQi = *(int16_t*)(RxArr+2 + 2);
				hheat.hSensor.SenTestKeTi = *(int16_t*)(RxArr+2 + 4);
				hheat.hSensor.SenTestJinKou = *(int16_t*)(RxArr+2 +6);
				hheat.hSensor.SenTestChuKou = *(int16_t*)(RxArr+2 + 8);
				hheat.hSensor.SenTestHuoSaiFb = *(int16_t*)(RxArr+2 + 10);
				hheat.hSensor.SenTestYouBengFb = *(int16_t*)(RxArr + 2 + 12);*/
				hheat.TargetPrm = *(uint16_t *)(RxArr + 2 + 14);//测试
				TxArr[0] = 'o';
				TxArr[1] = 'k';
				TxArr[2] = NULL;

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
				//bWriteStat = WriteFile(hCom, MB_TxBuf, TxLength + 5, &wCount, NULL);//发出
				bWriteStat = WriteFile(hCom, TxArr, strlen(TxArr), &wCount, NULL);//发出
#else
				RS485_TX;
				HAL_Delay(5);
				HAL_UART_Transmit_DMA_my(&huart1, TxArr, 2);

#endif
				break;
			case 'p':
				//RxArr[]//接收到的数组
				uint16_t parmPostion = 0;
				uint16_t parmLength = 0;
				parmLength = RxArr[1];
				parmPostion = RxArr[3] << 4 | RxArr[2] >> 4;
				for (int i = 0; i < parmLength; i++)
				{
					HeatParmArr[parmPostion + i] = RxArr[4 + 2 * i] | RxArr[4 + 1 + 2 * i]<<8;
				}
				TxArr[0] = 'p';
				TxArr[1] = RxArr[1];
				TxArr[2] = RxArr[2];
				TxArr[3] = RxArr[3];
				TxArr[4] = 0x0d;
				TxArr[5] = 0x0a;
				TxArr[6] = NULL;

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
				//bWriteStat = WriteFile(hCom, MB_TxBuf, TxLength + 5, &wCount, NULL);//发出
				bWriteStat = WriteFile(hCom, TxArr, strlen(TxArr), &wCount, NULL);//发出
#else
				RS485_TX;
				HAL_Delay(5);
				HAL_UART_Transmit_DMA_my(&huart1, TxArr, 2);

#endif
				break;
			case 'k':
				//hheat.hSensor.SenTestDianWeiQi= RxArr[1];
				hheat.hKey.KeyStateSet(RxArr[2]);
			TxArr[0]='o';
			TxArr[1]='k';
			TxArr[2]=NULL;
			
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
				//bWriteStat = WriteFile(hCom, MB_TxBuf, TxLength + 5, &wCount, NULL);//发出
				bWriteStat = WriteFile(hCom, TxArr, strlen(TxArr), &wCount, NULL);//发出
#else
					RS485_TX;
			HAL_Delay(5);
				HAL_UART_Transmit_DMA_my(&huart1,TxArr,2);
				
#endif
				break;
			default:
				//sprintf(TxArr,"Parm is not ok\n");
			
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
				//bWriteStat = WriteFile(hCom, MB_TxBuf, TxLength + 5, &wCount, NULL);//发出
				bWriteStat = WriteFile(hCom, TxArr, strlen(TxArr), &wCount, NULL);//发出
#else
					RS485_TX;
			HAL_Delay(5);
				HAL_UART_Transmit_DMA_my(&huart1,TxArr,6);
				
#endif

				break;

			}
			
		}
//HAL_Delay(300);
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
				if (!bReadStat)
		{
			printf("读串口失败!");
			return FALSE;
		}
		else
		{
			printf("readlength:%d\n", rCount);
			printf("writelength:%d\n", wCount);
		}
		Sleep(100);
#endif
		

}


