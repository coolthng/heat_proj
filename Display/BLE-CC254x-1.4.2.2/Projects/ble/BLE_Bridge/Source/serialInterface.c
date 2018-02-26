#include "hal_uart.h"
#include "serialInterface.h"
#include "BLE_Bridge.h"
#include "bcomdef.h"

//local function
static void SerialInterface_ProcessOSALMsg( osal_event_hdr_t *pMsg );

uint8 serialInterface_TaskID;   // Task ID for internal task/event processing

#define SERIAL_MSG_START_ID        0xAB //indicate the start of serial message
#define SERIAL_ACK                 0xA5 //inidicate an ACK
#define SERIAL_DATA                0xAD //indiciate data from central device
#define SEND_TIME_RS485_EVENT   0x01
#define SEND_TIME_RS485MAX  1

#define SBP_DISPLAY_EVT                                   0x0010
#define SBP_DISPLAY_OUT_EVT                               0x0020

#define SBP_DISPLAY_EVT_PERIOD                      10
#define SBP_DISPLAY_OUT_EVT_PERIOD                  500

uint8 serialBuffer[RX_BUFF_SIZE];

uint16 serialBufferOffset = 0;
static serial_state_t  serialRxState = SERIAL_STATE_LEN;
uint8 packet_length = 0;

//static uint8 temp_buf[48];

void SerialInterface_Init( uint8 task_id )
{
  serialInterface_TaskID = task_id;
    
  
  
  NPI_InitTransport(cSerialPacketParser);
  
    osal_start_timerEx( serialInterface_TaskID, SBP_DISPLAY_EVT, SBP_DISPLAY_EVT_PERIOD );
    osal_start_timerEx( serialInterface_TaskID, SBP_DISPLAY_OUT_EVT, SBP_DISPLAY_OUT_EVT_PERIOD );
}

uint16 SerialInterface_ProcessEvent( uint8 task_id, uint16 events )
{
  
  VOID task_id; // OSAL required parameter that isn't used in this function
  
//  if(event &HAL_UART_TX_EMPTY){
//     event &=(~HAL_UART_TX_EMPTY);
//  //    P0_1 = 0;
//    osal_start_timerEx(serialInterface_TaskID,SEND_TIME_RS485_EVENT,SEND_TIME_RS485MAX); 
//  }
  
  
    if ( events & SEND_TIME_RS485_EVENT )
  {
    P1_4 = 0;
    return (events ^ SEND_TIME_RS485_EVENT);
  } 
  
    #if 1
   static unsigned char test[4]={1,1,1};
    static unsigned char count=0;
    static unsigned char count_num=0;
    static int count_int=0;
     // Restart timer
    if ( events & SBP_DISPLAY_OUT_EVT )
    {
      test[0]=count;//+1;
      test[1]=count;//+2;
      test[2]=count;//+3;
      count++;
      if(count>16)
      {
      count=0;
      }
      count_num++;
      count_int++;
      osal_start_timerEx( serialInterface_TaskID, SBP_DISPLAY_OUT_EVT, SBP_DISPLAY_OUT_EVT_PERIOD );
    }
    if ( events & SBP_DISPLAY_EVT )
    {
      static unsigned char num=0;
      num++;
      if(num>2)
      {
      num=0;
      }
      test[4]=num;
      //smg_display_num(test,num);
//smg_display_str(&count_num);
smg_display_str_nor(count_int);
      osal_start_timerEx( serialInterface_TaskID, SBP_DISPLAY_EVT, SBP_DISPLAY_EVT_PERIOD );
    }
#endif

  
  if ( events & SYS_EVENT_MSG )
  {
    uint8 *pMsg;
    
    if ( (pMsg = osal_msg_receive( serialInterface_TaskID )) != NULL )
    {
      SerialInterface_ProcessOSALMsg( (osal_event_hdr_t *)pMsg );
      
      // Release the OSAL message
      VOID osal_msg_deallocate( pMsg );
    }
    
    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  
  // Discard unknown events
  return 0;
}

static void SerialInterface_ProcessOSALMsg( osal_event_hdr_t *pMsg )
{
  switch ( pMsg->event )
  {
  default:
    // do nothing
    break;
  }
}
#include "hal_led.h"
void cSerialPacketParser( uint8 port, uint8 events )
{  
  //unused input parameters
  (void)port;
  (void)events;
#define NPI_RX_LENGTH 40
  static uint8 NPI_Rx[NPI_RX_LENGTH];
  static uint8 NpiRxHeader=0;
  static uint8 NpiRxTail=0;
  static uint8 NpiPointer=0;
   uint8 done = FALSE;
   uint8 numBytes; 
      
	if(events &HAL_UART_TX_EMPTY){
     events &=(~HAL_UART_TX_EMPTY);
    P1_4=0;
    osal_start_timerEx(serialInterface_TaskID,SEND_TIME_RS485_EVENT,SEND_TIME_RS485MAX); 
  }
   // get the number of available bytes to process
   numBytes = NPI_RxBufLen();
   // check if there's any serial port data to process
#if 1 //透传ok
  // alloc temporary buffer
   uint8 *temp_buf;
   temp_buf = osal_mem_alloc( numBytes );
         
   //store dma buffer into temp buffer
   (void)NPI_ReadTransport(temp_buf, numBytes);

   // copy data from temp buffer into rx circular buffer
   for (uint8 i = 0; i < numBytes; i++)
   {
     //copy one byte to data buffer
     serialBuffer[serialBufferOffset] = temp_buf[i];                    
     //update offset
     serialBufferOffset = circular_add(serialBufferOffset,1);
   }
   //free temp buffer
   osal_mem_free(temp_buf);
         
#endif
#if 0 //my 0d0a结束标志
while(numBytes)
{
  NPI_ReadTransport((uint8 *)&NPI_Rx[NpiPointer], 1);
  if((NpiPointer>1)&&(NPI_Rx[NpiPointer-2]==0x0d)&&(NPI_Rx[NpiPointer-1]==0x0a))
  {
    for(int i=0;i<NpiPointer;i++)
    {
     //copy one byte to data buffer
     serialBuffer[serialBufferOffset] = NPI_Rx[i];                    
     //update offset
     serialBufferOffset = circular_add(serialBufferOffset,1);
    }
    osal_memset(NPI_Rx,0,NpiPointer);
    NpiPointer=0;
  }
  numBytes--;
  NpiPointer++;
}
   
#endif
   
#if 0//my 0d0a结束标志  有问题
  //读数据到缓冲区
   if(NpiRxHeader+numBytes>=NPI_RX_LENGTH)
  {
    NPI_ReadTransport((uint8 *)&NPI_Rx[NpiRxHeader], numBytes);
    NpiRxHeader+=numBytes;
  }else
  {
    NPI_ReadTransport((uint8 *)&NPI_Rx[NpiRxHeader], NPI_RX_LENGTH-NpiRxHeader);
    NPI_ReadTransport((uint8 *)&NPI_Rx[0], numBytes+NpiRxHeader-NPI_RX_LENGTH);
  }

  if(NpiRxTail<NpiRxHeader)//正常顺序
  {
    NpiPointer=NpiRxTail+2;
    while(NpiPointer<NpiRxHeader)
    {
      if(NPI_Rx[NpiPointer-2]==0x0d&&NPI_Rx[NpiPointer-1]==0x0a)//接收到有效数据
      {
        //temp_buf = osal_mem_alloc( NpiPointer-NpiRxTail);//有效数据长度
        while(NpiRxTail<NpiPointer)
        {
          //copy one byte to data buffer
          serialBuffer[serialBufferOffset] = NPI_Rx[NpiRxTail];                    
          //update offset
          serialBufferOffset = circular_add(serialBufferOffset,1);
          NPI_Rx[NpiRxTail++]=0;//清除数据
        }
       }
      NpiPointer++;
     }
  }//非正常顺序
  else{
    NpiPointer=NpiRxTail+2;
    while(NpiPointer<NPI_RX_LENGTH)//先判断到结尾
    {
      if(NPI_Rx[NpiPointer-2]==0x0d&&NPI_Rx[NpiPointer-1]==0x0a)//接收到有效数据
      {
        //temp_buf = osal_mem_alloc( NpiPointer-NpiRxTail);//有效数据长度
        while(NpiRxTail<NpiPointer)
        {
          //copy one byte to data buffer
          serialBuffer[serialBufferOffset] = NPI_Rx[NpiRxTail];                    
          //update offset
          serialBufferOffset = circular_add(serialBufferOffset,1);
          NPI_Rx[NpiRxTail++]=0;//清除数据
        }
       }
      NpiPointer++;
    }
    if(NPI_Rx[NPI_RX_LENGTH-1]==0x0d&&NPI_Rx[0]==0x0a)//接收到有效数据
    {
        while(NpiRxTail<NPI_RX_LENGTH)
        {
          //copy one byte to data buffer
          serialBuffer[serialBufferOffset] = NPI_Rx[NpiRxTail];                    
          //update offset
          serialBufferOffset = circular_add(serialBufferOffset,1);
          NPI_Rx[NpiRxTail++]=0;//清除数据
        }
        NpiRxTail=0;
          serialBuffer[serialBufferOffset] = NPI_Rx[NpiRxTail];                    
          //update offset
          serialBufferOffset = circular_add(serialBufferOffset,1);
          NPI_Rx[NpiRxTail++]=0;//清除数据
     }
    NpiPointer=2;
    while(NpiPointer<NpiRxHeader)
    {
      if(NPI_Rx[NpiPointer-2]==0x0d&&NPI_Rx[NpiPointer-1]==0x0a)//接收到有效数据
      {
        //temp_buf = osal_mem_alloc( NpiPointer-NpiRxTail);//有效数据长度
        if(NpiRxTail>NpiPointer)
        {
          while(NpiRxTail<NPI_RX_LENGTH)
          {
            //copy one byte to data buffer
            serialBuffer[serialBufferOffset] = NPI_Rx[NpiRxTail];                    
            //update offset
            serialBufferOffset = circular_add(serialBufferOffset,1);
            NPI_Rx[NpiRxTail++]=0;//清除数据
          }
        }
        NpiRxTail=NpiRxTail%NPI_RX_LENGTH;
        while(NpiRxTail<NpiPointer)
        {
          //copy one byte to data buffer
          serialBuffer[serialBufferOffset] = NPI_Rx[NpiRxTail];                    
          //update offset
          serialBufferOffset = circular_add(serialBufferOffset,1);
          NPI_Rx[NpiRxTail++]=0;//清除数据
        }
       }
      NpiPointer++;
     }
    
  
  }
   if(NpiRxHeader==NPI_RX_LENGTH)
   {
   NpiRxHeader=0;
   }
  if(NpiRxTail==NPI_RX_LENGTH)
  {
    NpiRxTail=0;
  }
#endif
#if 0
   while ( (numBytes > 0) && (!done) )
   {
     HalLedSet( HAL_LED_1, HAL_LED_MODE_TOGGLE );
     switch (serialRxState)
     {
     case SERIAL_STATE_LEN:
       //read the length 
       (void)NPI_ReadTransport((uint8 *)&packet_length, 1);
       // decrement the number of available bytes
       numBytes -= 1;
       
       // next state
       serialRxState = SERIAL_STATE_DATA;
       
       //DROP THROUGH
       
     case SERIAL_STATE_DATA:
       //check if we've dma'd the entire packet
       if (numBytes >= packet_length)
       {
         // alloc temporary buffer
         uint8 *temp_buf;
         temp_buf = osal_mem_alloc( packet_length );
         
         //store dma buffer into temp buffer
         (void)NPI_ReadTransport(temp_buf, packet_length);

         // copy data from temp buffer into rx circular buffer
         for (uint8 i = 0; i < packet_length; i++)
         {
           //copy one byte to data buffer
           serialBuffer[serialBufferOffset] = temp_buf[i];                    
           //update offset
           serialBufferOffset = circular_add(serialBufferOffset,1);
         }
         //free temp buffer
         osal_mem_free(temp_buf);
         
         //decrement number of available bytes
         numBytes -= packet_length;
         
         //reset state machine
         serialRxState = SERIAL_STATE_LEN;
          HalLedSet( HAL_LED_2, HAL_LED_MODE_TOGGLE );

       }
       else
       {
         //not enough data to progress, so leave it in driver buffer
         done = TRUE;
       }
       break;
     }
   }
#endif

}

uint8 sendAckMessage(uint8 bytes_sent)
{
  return SUCCESS;//屏蔽掉确认  lgh
  uint8 data[3] = {0};
  
  data[0]= SERIAL_MSG_START_ID;
  data[1]= SERIAL_ACK;
  data[2]= bytes_sent;
  uint8 success_len = HalUARTWrite(NPI_UART_PORT, (uint8*)data, 3);
  if (success_len == 3)
  {
    return SUCCESS;
  }
  else
  {
    return 1;   //ack wasn't sent over UAR
  }
}

uint8 sendDataToHost(uint8* data, uint8 len)
{
  uint8* buf = osal_mem_alloc((2+len)*sizeof(uint8));
  if (buf)  //if allocated
  {
    buf[0] = SERIAL_MSG_START_ID;
    buf[1]= SERIAL_DATA;
    osal_memcpy(&buf[0], data, len);
     P1_4=1;
    uint8 bytes_sent = HalUARTWrite(NPI_UART_PORT, (uint8*)buf, len);//len+2);
    osal_mem_free(buf);
     //P1_4=0;
    if (bytes_sent == len)
    {
      return SUCCESS;
    }
    else
    {
      return 1;  //data not sent over UART
    }    
  }
  else
  {
    return bleMemAllocError;
  }
}

uint16 circular_diff(uint16 offset, uint16 tail)
{
  if (offset > tail)
  {
    return (offset - tail);
  }
  else
  {
    return (RX_BUFF_SIZE - tail) + offset;
  }    
}

uint16 circular_add(uint16 x, uint16 y)
{
  uint16 sum = x + y;
  if (sum != RX_BUFF_SIZE)
  {
    sum = sum % RX_BUFF_SIZE;
  }
  else
  {
    sum = 0;
  }
  return sum;
}