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
    #include "25LC256.h"
    
    //uint8_t AccData[6]; // Acceleration data, 2 for each of the 3 direction (X, Y, Z)
    
    volatile uint8 PacketReadyFlag; //Flag to communicate when new data are available and read, to be sent then with UART
    volatile uint8 UARTVerboseFlag;
    //UARTVerboseFlag = 0;
    uint8 data_water;
    volatile uint8 FlagEnableDisable;
    volatile uint8 new_EEPROM;
    uint8 data_register;
    
    CY_ISR_PROTO(Custom_Pin_ISR); 
    CY_ISR_PROTO(Custom_Pin_Button);
    CY_ISR_PROTO(Custom_Timer_Button);
    CY_ISR_PROTO(Custom_LED_Blinking);
    CY_ISR_PROTO(Custom_Pin_Button_Positive);
    CY_ISR_PROTO(Custom_Pin_EnableDisable);
    
#endif
/* [] END OF FILE */
