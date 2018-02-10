#ifndef __MODBUS_H
#define __MODBUS_H

#include "heat.h"

#include "stdint.h"
#include "stdio.h"
//start 读 txt文件
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024
//end  读txt文件
//#include "thread.h"
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
#include<Windows.h>;
#endif

typedef enum
{
	MB_RTU,                     /*!< RTU transmission mode. */
	MB_ASCII,                   /*!< ASCII transmission mode. */
	MB_TCP                      /*!< TCP mode. */
} eMBMode;

typedef enum
{
	MB_PAR_NONE,                /*!< No parity. */
	MB_PAR_ODD,                 /*!< Odd parity. */
	MB_PAR_EVEN                 /*!< Even parity. */
} eMBParity;

void eMBPoll();
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
void eMBInit(eMBMode eMode, UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity);
#endif


#endif
