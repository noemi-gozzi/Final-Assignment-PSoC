/* ========================================
 *
 * @file InterruptRoutines.h
 * @brief Functions required to interface with the ISR events.
 * 
 * This header file contains macros and ISR function prototypes 
 *
 * ========================================
*/
#ifndef _INTERRUPT_ROUTINES_H 
    
    #define _INTERRUPT_ROUTINES_H 
    //ADC MACROS
    #define FULL_SCALE_RANGE_LOW 0
    #define FULL_SCALE_RANGE_HIGH 65535
    #define FULL_SCALE_RANGE_HALF 32767 
    
    #define OFF 0
    #define ON 1
    
    #include "SPI_Interface.h"
    #include "project.h"
    #include "LIS3DH.h"
    #include "LIS3DH_SPI.h"
    #include "25LC256.h"
     
    volatile uint8 PacketReadyFlag;       //Flag ISR LIS3DH on watermark and overthreshold event
    volatile uint8 UARTVerboseFlag;       //Flag dependent on potentiomter value in config mode. Used to send data to Bridge Control Panel
    volatile uint8 FlagEnableDisable;     //Set by pin 3.2, to identify ReadWrite or Read-Only modality in config mode
    volatile uint8 new_EEPROM;            //Flag to communicate a change in SYSTEM (start/stop) or CONFIGURATION status to be saved in EEPROM
    volatile uint8 new_EnableDisable;     //new change in Pin 3.2 Vcc-GND through ISR on pin --> change in FlagEnableDisable value
    volatile uint8_t Counter_overflow;    //Counter Overflows index 
    volatile uint8_t system_status;       //system status: ON 1, OFF 0
    volatile uint8_t configuration_status;//configuration status: ON 1, OFF 0
    volatile uint8_t FlagChangeParameters;//flag to communicate a change in potentiometer range --> UARTVerboseFlag is modified
    
    uint8 data_register;                  //data register: set in InterruptRoutines.c, then in main.c is saved in EEPROM but not changed.
                                          //system_status on bit [7], UARTVerboseFlag on bit [0]
    
             
    /* ISR PROTOTYPES */
    
    CY_ISR_PROTO(Custom_Pin_ISR); 
    CY_ISR_PROTO(Custom_Pin_Button);
    CY_ISR_PROTO(Custom_Timer_Button);
    CY_ISR_PROTO(Custom_LED_Blinking);
    CY_ISR_PROTO(Custom_Pin_Button_Positive);
    CY_ISR_PROTO(Custom_Pin_EnableDisable);
    CY_ISR_PROTO(Custom_TimeStamp);
    
#endif
/* [] END OF FILE */
