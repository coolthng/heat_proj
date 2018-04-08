/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* USER CODE BEGIN Includes */
#include "heat.h"

struct __HEAT_HandleTypeDef hheat;
USHORT   usRegInputBuf[20];
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim14;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;
DMA_HandleTypeDef hdma_tim3_ch4_up;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint16_t YouBengH=10000;//40;
uint16_t YouBengL=40000;//960;
uint16_t YouBengPried=1000;
uint16_t HuoSaiH=500;//10000  50Hz
uint16_t FengShanH=1600;// 3200

volatile uint8_t RS485_TxFlag=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM14_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM17_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  PWM Pulse finished callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
uint32_t HuoErInput[10];
uint8_t HuoErTail=0;
uint8_t HuoErOverCount=0;
uint8_t RxArr[100]="";
uint8_t RxArrTmp;
uint8_t RxArrFlag=0;


uint32_t uhCapture = 0;
__IO uint32_t uhCCR1_Val = 500;

/**
  * @brief  Output Compare callback in non blocking mode 
  * @param  htim: pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @retval None
  */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_OC_DelayElapsedCallback could be implemented in the user file
   */
	static uint8_t Flag=0;
	Flag=!Flag;

	 /* TIM1_CH1 toggling with frequency = 244.13 Hz */
  if(htim->Instance==TIM14 &&htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    uhCapture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    /* Set the Capture Compare Register value */
		if(Flag)
		{
		 __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, (uhCapture + uhCCR1_Val));
		}
		else{
		 __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, (uhCapture + uhCCR1_Val*19));
		}
   
  }
  
}

void HAL_SYSTICK_Callback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SYSTICK_Callback could be implemented in the user file
   */
	static uint16_t count_ms=0;
	count_ms++;
	if(count_ms==999)
	{
		hheat.hSysTime.SysTime.Time_s_up_flag=1;
	count_ms=0;
		
	}
	
	if((count_ms%500)==0)
	{
		hheat.hSysTime.SysTime.Time_100ms_up_flag=1;
		HuoErOverCount++;
		if(HuoErOverCount>1)
		{
			HuoErOverCount=0;
			HuoErInput[HuoErTail++]=0x7fffffff;
			if(HuoErTail>=HUO_ER_INPUT_LENGTH)
			{
				HuoErTail=0;
			}
		}
	}
	
}

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */



uint8_t TxArr[200]="Heat-1s V1.0\r\n";

uint8_t HelpCommandLine[][100]=
{
"吴婷婷\n",
"2333333\n",
	


};
// 油泵y 火花塞h 风扇f 霍尔e 壳体温度k 出口温度c 
uint8_t CommandLine[]="**\nhelp\n";
uint8_t CommandLine2222[]="******************************************\n\
帮助命令列表\n\
h 帮助命令获取帮助\n\
s或set 设置当前状态命令 \n\
g或get 获得当前状态命令 \n\
v 获得当前版本 \n\
i 获得设备ID \n\
*************************************************\n";


/**
  * @brief  Rx Half Transfer completed callback.
  * @param  huart: UART handle.
  * @retval None
  */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
 
}
 
 /**
  * @brief Rx Transfer completed callback.
  * @param huart: UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static uint8_t RxPtr=0;
	//if(RS485_TxFlag)
	if(1)
	{
		RxArr[RxPtr++]=RxArrTmp;
		if(RxPtr>2&&RxArr[RxPtr-2]==0x0d&&RxArr[RxPtr-1]==0x0a)
		{
			RxPtr=0;
			if(TxArr[1]!=RxArr[1])
			{
				RxArrFlag=1;
				RS485_TX;//使能发送
			}else if(TxArr[0]!=RxArr[0])
			{
				RxArrFlag=1;
				RS485_TX;//使能发送
			}
			
		}
	}
	HAL_UART_Receive_IT(&huart1,&RxArrTmp,1);
  //HAL_UART_Transmit_DMA(&huart1,TxArr,15);
	//HAL_UART_Receive_IT(&huart1,RxArr,5);
	//HAL_UART_Receive_DMA(&huart1,TxArr,5);
}
/**
  * @brief Tx Transfer completed callback.
  * @param huart: UART handle.
  * @retval None
  */
 void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	RS485_RX;
	//huart1.State=HAL_UART_STATE_BUSY_RX;
	//HAL_UART_Receive_IT(&huart1,RxArr,100);
	HAL_UART_Receive_IT(&huart1,&RxArrTmp,1);
	//HAL_HalfDuplex_EnableReceiver(&huart1);
	
	//HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_TxCpltCallback can be implemented in the user file.
   */
}

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM3)
	{
	
	
	}
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_TriggerCallback could be implemented in the user file
   */
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance==TIM3)
	{
	static uint16_t lastValue=0;
	static uint16_t currentValue=0;
		static uint8_t first=0;
		if(first)
		{
			currentValue=htim->Instance->CCR4;
			
			hheat.MyTest =HuoErInput[0];//HuoErOverCount;
			//HuoErInput[HuoErTail++]=HuoErOverCount*0xffff+(currentValue+0xffff-lastValue)%0xffff;
			//HuoErInput[HuoErTail++]=currentValue;//(currentValue+0xffff-lastValue)%0xffff;
			HuoErInput[HuoErTail++]=(currentValue+0xffff-lastValue)%0xffff;
			HuoErOverCount=0;
			if(HuoErTail>=HUO_ER_INPUT_LENGTH)
			{
			HuoErTail=0;
			}
		}else{
		first=1;
		}
		
		lastValue=currentValue;
	HuoErOverCount=0;
	}
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_IC_CaptureCallback could be implemented in the user file
   */
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
	if(htim->Instance==TIM3)
	{
//			HuoErOverCount++;
//		if(HuoErOverCount>1)
//		{
//			HuoErOverCount=0;
//		HuoErInput[HuoErTail++]=0x7fffffff;
//			if(HuoErTail>=HUO_ER_INPUT_LENGTH)
//			{
//				HuoErTail=0;
//			}
//		}
//		static uint16_t lastValue=0;
//	static uint16_t currentValue=0;
//		currentValue=htim->Instance->CCR4;
//		if(((currentValue+0xffff-lastValue)%0xffff)>0xdfff)
//		{
//		//hheat.MyTest++;
//			HuoErInput[HuoErTail++]=0xfffff;
//			HuoErOverCount=0;
//			if(HuoErTail>=10)
//			{
//			HuoErTail=0;
//			}
//		}
//	
//		lastValue=currentValue;
	//hheat.MyTest = htim->Instance->CNT+1;// htim->Instance->CCR4+2; //
//		HuoErOverCount++;
//		if(HuoErOverCount>=10)
//		{
//			HuoErInput[HuoErTail++]=HuoErOverCount*0xffff;
//			HuoErOverCount=0;
//			if(HuoErTail>=10)
//			{
//			HuoErTail=0;
//			}
//		}
	}
	

}

//定时器5中断服务程序	
#if 0
void TIM5_IRQHandler(void) 
{ 	     
u16 tsr; 
tsr=TIM5->SR; 
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{ 
		if(tsr&0X01)//溢出 
		{	     
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了 
			{ 
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了 
				{ 
					TIM5CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次 
					TIM5CH1_CAPTURE_VAL=0XFFFF; 
				}else TIM5CH1_CAPTURE_STA++; 
			}	  
		} 
		if(tsr&0x02)//捕获1发生捕获事件 
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)	//捕获到一个下降沿 	
			{	  	
				TIM5CH1_CAPTURE_STA|=0X80;	//标记成功捕获到一次上升沿 
					TIM5CH1_CAPTURE_VAL=TIM5->CCR1;	//获取当前的捕获值. 
				TIM5->CCER&=~(1<<1);	//CC1P=0 设置为上升沿捕获 
			}else  	//还未开始,第一次捕获上升沿 
			{ 
				TIM5CH1_CAPTURE_STA=0;	//清空 
				TIM5CH1_CAPTURE_VAL=0; 
					TIM5->CNT=0; 
				TIM5CH1_CAPTURE_STA|=0X40;	//标记捕获到了上升沿 
					TIM5->CCER|=1<<1; 	//CC1P=1 设置为下降沿捕获 
			}	     
		}	     	    	    
 	} 
TIM5->SR=0;//清除中断标志位 	     
} 
#endif

uint16_t Get_JTAG_ID()
                {
                        if( *( uint8_t *)( 0xE00FFFE8 ) & 0x08 )
                        {
                                return   ( ( *( uint8_t *)( 0xE00FFFD0 ) & 0x0F ) << 8 ) |
                                              ( ( *( uint8_t *)( 0xE00FFFE4 ) & 0xFF ) >> 3  )  | 
                                              ( ( *( uint8_t *)( 0xE00FFFE8 ) & 0x07 ) << 5 ) + 1 ;
                        }
                
                
                        return  0;
                }
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	
	while(0)
	{
	
	}
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM14_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
	static uint16_t test_ID=0;
	test_ID=Get_JTAG_ID();
	RS485_TX;

//__HAL_TIM_ENABLE(&htim14);
//HAL_TIM_OnePulse_Start(&htim14,TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
//HAL_TIM_IC_Start_DMA(&htim3, TIM_CHANNEL_4, HuoErInput, 10);
//HAL_TIM_Base_Start_IT(&htim3);
HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
//HAL_TIM_IC_Start_DMA(&htim3, TIM_CHANNEL_4, HuoErInput, 10);

//HAL_TIMEx_HallSensor_Start_DMA(&htim3,HuoErInput,10);
//HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
//HAL_TIM_OnePulse_Start(&htim16,TIM_CHANNEL_1);
//HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
HAL_TIM_OC_Start_IT(&htim14,TIM_CHANNEL_1);

//HAL_HalfDuplex_EnableTransmitter(&huart1);
HAL_UART_Receive_IT(&huart1,&RxArrTmp,1);
HAL_UART_Transmit_DMA_my(&huart1,TxArr,strlen(TxArr));
//UART_Receive_IT(&huart1);
//HAL_UART_Receive_IT(&huart1,&RxArrTmp,1);
//RS485_RX;

HEAT_Init(&hheat);

while(0)
{
	hheat.pCommPoll();
}

HAL_Delay(1000);
POWER_EN;
	//HEAT_Init(&hheat);
	//start  测试函数
	HEAT_Poll(&hheat);

	while (1)
	{

	}
	
	//end  测试函数

while(1)
{

}
//HAL_TIM_PWM_Start(&htim14,TIM_CHANNEL_1);
uint16_t Proid_test=1000;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//HAL_UART_Transmit(&huart1,"hello world99\n",15,100);
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
		
//		YouBengL+=10;
//		if(YouBengL>2000)
//		{
//		YouBengL=400;
//		}
		
		
		
//		Proid_test +=10;
//		if(Proid_test>5000)
//		{
//			Proid_test=1000;
//		}
//	  YouBengL=Proid_test-YouBengH;
//		HuoSaiH+=100;
//		if(HuoSaiH>9000)
//			HuoSaiH=1000;
//		htim16.Instance->CCR1=HuoSaiH;
//		
		FengShanH+=10;
		if(FengShanH>2000)
			FengShanH=500;
		
		htim3.Instance->CCR2=FengShanH;
		//	htim14.Init.Period = Proid_test;
		//htim14.Instance->ARR=Proid_test;
			//HAL_TIM_Base_Init(&htim14);
			//HAL_TIM_PWM_Init(&htim14);
		//__HAL_TIM_ENABLE(&htim14);
		//HAL_TIM_OnePulse_Start(&htim14,TIM_CHANNEL_1);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;
//TIM_HallSensor_InitTypeDef sHallSensorConfig;
	
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 479;//实际100k
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0xffff;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_IC_Init(&htim3);
	
//	sHallSensorConfig.IC1Prescaler=TIM_ICPSC_DIV1;
//  sHallSensorConfig.IC1Polarity=TIM_ICPOLARITY_FALLING;//TIM_ICPOLARITY_RISING;
//  sHallSensorConfig.IC1Filter=0;
//  sHallSensorConfig.Commutation_Delay=1;  
//  HAL_TIMEx_HallSensor_Init(&htim3,&sHallSensorConfig);
	

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC1;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 15;
  HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_4);

//HAL_TIMEx_HallSensor_Init();


}

/* TIM14 init function */
void MX_TIM14_Init(void)
{

  TIM_OC_InitTypeDef sConfigOC;

  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 479;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 0xFFFF;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim14);

  HAL_TIM_OC_Init(&htim14);

  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 400;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_OC_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1);

}

/* TIM16 init function */
void MX_TIM16_Init(void)
{

  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 95;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 10000;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim16);

  HAL_TIM_PWM_Init(&htim16);

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htim16, &sBreakDeadTimeConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  HAL_TIM_PWM_ConfigChannel(&htim16, &sConfigOC, TIM_CHANNEL_1);

}

/* TIM17 init function */
void MX_TIM17_Init(void)
{

  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 0;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 3199;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim17);

  HAL_TIM_PWM_Init(&htim17);

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htim17, &sBreakDeadTimeConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1);

}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;////115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED ;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart1);

}

/** 
  * Enable DMA controller clock
  */
void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
	__GPIOF_CLK_ENABLE();

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
