#ifndef  __PLATE_FORM_CONFIG_
#define  __PLATE_FORM_CONFIG_

#define DEBUG_TEST_PC  
#define DEBUG_PRINTF_TEST  

#define PLATE_FORM_SIM_PC		1
#define PLATE_FORM_SIM_STM32	2
#define PLATE_FORM_SIM_STM8		3

//#define STM32_PLATEFORM  


#ifdef STM32_PLATEFORM
#define PLATE_FORM_SIM   2//PLATE_FORM_SIM_32
#else
#define PLATE_FORM_SIM 1
#endif 

//#define PLATE_FORM_SIM 2 //PLATE_FORM_SIM_32


#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM32)
#elif(PLATE_FORM_SIM==PLATE_FORM_SIM_STM8)
#else
#error  请先确认编译平台
#endif

#endif

#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)

#else

typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
//#define RS485_TX HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
//#define RS485_RX HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)


//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET
//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)
#define RS485_TX //__HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE); \
RS485_TxFlag=0
#define RS485_RX //__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE); \
RS485_TxFlag=1


#define POWER_EN      HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET)
#define POWER_DIS     HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET)



#endif

