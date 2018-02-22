#include "heat.h"

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
#else
#include "stm32f0xx_hal.h"
#endif

uint8_t aucCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40
};
uint8_t aucCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
	0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
	0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
	0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
	0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
	0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
	0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
	0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
	0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
	0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
	0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
	0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
	0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
	0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
	0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
	0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
	0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
	0x41, 0x81, 0x80, 0x40
};
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
uint16_t Crc16(uint8_t  *pucFrame, int usLen)
{
#if  (PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
	int i = 0;
	byte ucCRCHi = 0xFF;
	byte ucCRCLo = 0xFF;
	uint16_t iIndex = 0x0000;

	while (usLen-- > 0)
	{
		iIndex = (uint16_t)(ucCRCLo ^ pucFrame[i++]);
		ucCRCLo = (byte)(ucCRCHi ^ aucCRCHi[iIndex]);
		ucCRCHi = aucCRCLo[iIndex];
	}

	return (ucCRCHi << 8 | ucCRCLo);

#else
	int i = 0;
	uint8_t ucCRCHi = 0xFF;
	uint8_t ucCRCLo = 0xFF;
	uint16_t iIndex = 0x0000;

	while (usLen-- > 0)
	{
		iIndex = (uint16_t)(ucCRCLo ^ pucFrame[i++]);
		ucCRCLo = (uint8_t)(ucCRCHi ^ aucCRCHi[iIndex]);
		ucCRCHi = aucCRCLo[iIndex];
	}

	return (ucCRCHi << 8 | ucCRCLo);
#endif
	
}
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
#include <stdio.h>
HANDLE hCom;
void eMBInit(eMBMode eMode, UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity)
{

	
	//char arr[16][6];
	//char *parr;
	//parr = &arr[0][0];
	//char tmp[6];
	//for (int i = 1, j=0; i < 16; i++)
	//{
	//	sprintf(tmp, "com%d", i);
	//	hCom = CreateFile(tmp,//COM1口
	//		GENERIC_READ | GENERIC_WRITE, //允许读和写
	//		0, //独占方式
	//		NULL,
	//		OPEN_EXISTING, //打开而不是创建
	//		0, //同步方式
	//		NULL);
	//	if (hCom == (HANDLE)-1)
	//	{
	//	}
	//	else
	//	{
	//		strcpy(arr +j++, tmp);
	//		
	//	}
	//}
	//TEXT("COM1")
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
//void eMBInit(eMBMode eMode, UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity)
//{

//}
#endif

extern USHORT   usRegInputBuf[20];

void eMBPoll()
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
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR); //清空缓冲区
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
	//hheat.RunTime++;
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
				TxArr[0] = 0x61;
				TxArr[1] = 0;//保留为数据长度长度
				TxArr[2] = hheat.StateMachine << 4 | (hheat.AlarmState);
				TxArr[3] = hheat.StateRunLevel << 4;
				
				TxArr[4] = hheat.hSysTime.StateMachineRun_s;//hheat.RunTime;
				TxArr[5] = hheat.JinKouTemp;

				*((uint16_t *)&TxArr[6]) = hheat.CurrentPrm;// hheat.Prm;
				*((uint16_t *)&TxArr[8]) = hheat.KeTiTemp;
				*((uint16_t *)&TxArr[10]) = hheat.hHuoSai.curPre;
				*((uint16_t *)&TxArr[12]) = hheat.hFengShan.curPre;
				*((uint16_t *)&TxArr[14]) = hheat.RunTime;
				*((uint16_t *)&TxArr[16]) =hheat.MyTest;//hheat.PowerVal_M100;
				//*((uint16_t *)&TxArr[18]) = hheat.RunTime;
				//TxArr[20] = hheat.StateMachineNext;
				//TxArr[21] = hheat.YouBengHz;

				//*((uint16_t *)&TxArr[22]) = 1000;
				//*((uint16_t *)&TxArr[24]) = 1000;
				TxArr[18] = 0x0d;
				TxArr[19] = 0x0a;
				TxArr[20] = NULL;
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
				hheat.hSensor.SenTestPower = *(int16_t*)(RxArr + 2);
				hheat.hSensor.SenTestDianWeiQi = *(int16_t*)(RxArr+2 + 2);
				hheat.hSensor.SenTestKeTi = *(int16_t*)(RxArr+2 + 4);
				hheat.hSensor.SenTestJinKou = *(int16_t*)(RxArr+2 +6);
				hheat.hSensor.SenTestChuKou = *(int16_t*)(RxArr+2 + 8);
				hheat.hSensor.SenTestHuoSaiFb = *(int16_t*)(RxArr+2 + 10);
				hheat.hSensor.SenTestYouBengFb = *(int16_t*)(RxArr + 2 + 12);
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
			case 'k':
				hheat.hSensor.SenTestDianWeiQi= RxArr[1];
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


