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



CY_ISR(Custom_Pin_ISR){
    
    Pin_ISR_ClearInterrupt();
    PacketReadyFlag = 1;
    
    uint8_t clear=LIS3DH_readByte(LIS3DH_INT1_SRC); //clear interrupt register
    
    uint8_t value=LIS3DH_readByte(LIS3DH_FIFO_SRC_REG);
    while (!(value&empty_bit)){
    LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
}
    
}

/* [] END OF FILE */
