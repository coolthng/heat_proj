#ifndef __MODBUS_H
#define __MODBUS_H

#include "heat.h"

#include "stdint.h"
#include "stdio.h"
//start ¶Á txtÎÄ¼þ
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024
//end  ¶ÁtxtÎÄ¼þ
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

void CommPoll();
#if(PLATE_FORM_SIM==PLATE_FORM_SIM_PC)
void CommInit(eMBMode eMode, UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity);
#endif


#endif
