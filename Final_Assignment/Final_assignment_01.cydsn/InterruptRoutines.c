/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "InterruptRoutines.h"

#define DATA_BYTES 6

#define empty_bit 0x20

char bufferUART[100];
#define UART_1_PutBuffer UART_1_PutString(bufferUART)
#include <stdio.h>
#include <string.h>

CY_ISR(Custom_Pin_ISR){
    
    

    
    PacketReadyFlag=1;
//    uint8_t data = LIS3DH_readByte(LIS3DH_INT1_SRC);
//    sprintf(bufferUART, "INT1_SRC 0x%02X\r\n", data);
//        UART_1_PutBuffer;
//    data = LIS3DH_readByte(LIS3DH_FIFO_SRC_REG);
//    sprintf(bufferUART, "FIFO_SRC 0x%02X\r\n", data);
//        UART_1_PutBuffer;
//    
//    LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,0x00);
//    LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,0x47);
    
//        if (data_read&0x1E){
//            PacketReadyFlag=1;  
//        }
//        else{
//            PacketReadyFlag=2;
//        }
        
//        else{
//            PacketReadyFlag = 2;
//        }
  
    //uint8_t clear=LIS3DH_readByte(LIS3DH_INT1_SRC); //clear interrupt register
//    LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
//    LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
//    LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
//    
    Pin_ISR_ClearInterrupt();
    
    //uint8_t value=LIS3DH_readByte(LIS3DH_FIFO_SRC_REG);
//    while (!(value&empty_bit)){
//    LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
//}
    
}

/* [] END OF FILE */
