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
#ifndef _INTERRUPT_ROUTINES_H 
    
    #define _INTERRUPT_ROUTINES_H 
    
    #include "SPI_Interface.h"
    #include "project.h"
    #include "LIS3DH.h"
    #include "LIS3DH_SPI.h"
    
    CY_ISR_PROTO(Custom_Pin_ISR); 
    
    uint8_t AccData[6]; // Acceleration data, 2 for each of the 3 direction (X, Y, Z)
    
    volatile uint8 PacketReadyFlag; //Flag to communicate when new data are available and read, to be sent then with UART
 
#endif
/* [] END OF FILE */
