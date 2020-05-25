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
#include "RGBLedDriver.h"
#define DATA_BYTES 6

#define empty_bit 0x20

char bufferUART[100];
#define UART_1_PutBuffer UART_1_PutString(bufferUART)
#include <stdio.h>
#include <string.h>

#define OFF 0
#define ON 1
#define TIME_FACTOR 6

uint8_t system_status=OFF;
uint8_t configuration_status=OFF;
uint8 TimerFlag=0; 

uint8_t i=0;

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

CY_ISR(Custom_Pin_Button){
    
    /*      NEGATIVE EDGE OF THE DEBOUNCED BUTTON -->CLICKED
    STOP timer button. this timer is responsible of counting the time difference between two differentc clicks. 
    I don't want that timer keeps counting if i'm pressing (i don't consided that as time INTER clicks) 
    */
    
    TIMER_button_Stop();
    
    /*
    index i is increased with a timer (overflow 250 ms) that is used for counting the time between
    negative edge and positive edge --> duration of pressing. everytime a negative edge is detected this index is initialized.
    */
    i=0;
 
}

CY_ISR(Custom_Pin_Button_Positive){
    
    /*      POSITIVE EDGE OF THE DEBOUNCED BUTTON --> BUTTON RELEASED
    when the button is released the first thing to be checked is the time intercurred 
    between button pressed and button released.
    since overflow is 250 ms, index=TIME FACTOR=6 means 1500 ms
    if the time was higher than time factor : CONFIGURATION MODE
    if the time was lower that time factor: check for double click options (START/STOP ACQUISITION) 
    */
    
    if (i>=TIME_FACTOR){
        
        /*LONG PRESSURE: configuration mode. depending on the current configuration state
        enter or exit configuration mode
        */
        if (configuration_status==OFF){
            configuration_status=ON;
            RGBLed_WriteColor(OFF, OFF, OFF);
            SPIM_1_Stop();
            SPIM_2_Stop();
            UART_1_Stop();
        }
        else if (configuration_status==ON){

            configuration_status=OFF;
            
            /*
            if the system is exiting the configuration mode, RETURN to the previous
            system status.
            i.e. I can enter configuration mode both from system ON and system OFF. then, when i decide to 
            close CONFIGURATION mode the system is going back to ON or OFF. 
            */
            
            if (system_status==OFF){
                Pin_Led_Blue_Write(OFF);
            }
            else if (system_status==ON){
                Pin_Led_Blue_Write(ON);
                
                UART_1_Start();
                SPIM_1_Start();
                SPIM_2_Start();
                RGBLed_Start();
                
            }
        }
        /*flag for switch case for double click detection. if i'm in long pressure,
        initialize to 0, to be sure that long pressure is not detected as normal click*/
        TimerFlag=0;
    }
    
    /*
    the system CANNOT enter the start/stop CONDITIONS if it's in configuration mode.
    so it enters if:
    - time between push and release of the button is lower than time factor (not long pressure)
       --> CHECK INSIDE IF DOUBLE CLICK
    - the system is not in CONFIGURATION mode
    */
    
    else if (i<TIME_FACTOR && configuration_status==OFF) {
        switch (TimerFlag)
    	{
    		case 0: 
            //first time that the button has been pressed. normal pressure, SINGLE. Nothing to do.
            TimerFlag=1;
            /*
            if the button has been released, and it was not a long pressure, that is identified as click.
            TIMER button start, that is the one used to measure the time intercurring between two clicks, is re-started.
            it will be stopped in the NEGATIVE isr of the next debounced pressure
            */
            TIMER_button_Start();
    		break;
    		case 1:
            /*button has been pressed for (at least) two times.
            the time between first and second pressure is less than timer overflow, since
            if it overflows (at least one time, that is 500 ms) the TimerFlag would be set as 2.
            */
            
            /* CHECK THE PREVIOUS SYSTEM STATUS */
            
            if (system_status==OFF){
                /******************* SYSTEM START ACQUISITION ******************/
                          
                system_status=ON;
                Pin_Led_Blue_Write(ON);
                 
                UART_1_Start();
                SPIM_1_Start();
                SPIM_2_Start();
                RGBLed_Start();
                
                EEPROM_writeByte(0x0000, system_status);
            }
            else if (system_status==ON){
                /******************* SYSTEM STOP ACQUISITION ******************/
                system_status=OFF;
                Pin_Led_Blue_Write(OFF);
                EEPROM_writeByte(0x0000, system_status);
                RGBLed_WriteColor(OFF, OFF, OFF);
                SPIM_1_Stop();
                SPIM_2_Stop();
                UART_1_Stop();
                //RGBLed_Stop();
            }
            //flag cleared
            TimerFlag=0;  
    		break;
            
    		case 2: 
            /*
            time over 500ms--> TIME OVERFLOW OCCURED, FLAG=2, so it is not considered as a double click. 
            Anyway, it could be considered as the first click of a double click, 
            so the system will check the second pressing if it is before 500 ms.
            */
            TimerFlag=1;
            /*
            if the button has been released, and it was not a long pressure, that is identified as click.
            TIMER button start, that is the one used to measure the time intercurring between two clicks, is re-started.
            it will be stopped in the NEGATIVE isr of the next debounced pressure
            */
            TIMER_button_Start();
    		    
    		break;
            
        }

    }
}

CY_ISR(Custom_Timer_Button){
    /* TIMER for double click interval. this timer has been started from previous ISR only after positive edge
    it it reaches overflow before next button negative edge, the flag is set to 2
    */
    TimerFlag=2;
    TIMER_button_ReadStatusRegister();

}

CY_ISR(Custom_LED_Blinking){
    /* ISR on overflow 250 ms
    double function:
    1. toggle the led if configuration mode
    2. count the time between NEGATIVE (pressed) and POSITIVE (released)
    edge of the debouncer
    */
    Timer_Blinking_ReadStatusRegister();
    i=i+1;
    if (configuration_status==1){
        Pin_Led_Blue_Write(!Pin_Led_Blue_Read());
    }    
    
}
