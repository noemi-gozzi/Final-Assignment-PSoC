/* ========================================
 *
 * @file InterruptRoutines.c
 * @brief ISR handlers
 * 1. Custom_Pin_ISR: ISR LIS3DH full watermark or overthreshold events
 * 2. Custom_Pin_Button: ISR on negative edge debounced button (button pressed)
 * 3. Custom_Pin_Button_Positive: ISR on positive edge debounced button (button released)
 * 4. Custom_Timer_Button: ISR on overflow 500 ms. it measures time between two clicks (Positive edge of the old one, negative of the new one)
 * 5. Custom_LED_Blinking: ISR on overflow 250 ms. Blinking, time between pressure and release of the same click for long pressure and ADC sampling of potentiometer
 * 6. Custom_Pin_EnableDisable: ISR on both edges of Pin 3.2 (1-->0 or 0-->1)
 * 7. Custom_TimeStamp: overflow on counter 60000 (1200s); it counts overflows, in order to count more than 1200s. 
 *
 * ========================================
*/
#include "InterruptRoutines.h"
#include "RGBLedDriver.h"

#define TIME_FACTOR 6

uint8 TimerFlag=0;              //variable used in the switch case for double click detection.
int32 value_POT;
uint8_t index_long_pressure=0;  //index to count how many 250ms elapse between button's pressure and release
uint8_t UARTVerboseFlagOLD=OFF; //Flag used to identify a change in UARTVerboseFlag


CY_ISR(Custom_Pin_ISR){
    //LIS3DH isr. When overthreshold or watermark the flag is set HIGH for further elaboration in main.c

    PacketReadyFlag=1;
    Pin_ISR_ClearInterrupt();

}

CY_ISR(Custom_Pin_Button){
    
    /*      NEGATIVE EDGE OF THE DEBOUNCED BUTTON --> PRESSED
    STOP timer button. This timer is responsible for counting time difference between two different clicks. 
    the time during pressure is not considered as INTER clicks. 
    START TIME: positive edge of the previous click (released, next ISR)
    STOP TIME: negative edge of the next click (pressed)
    INTERVAL TIME: to measure dobule click 
    */
    
    TIMER_button_Stop();
    Timer_Blinking_Start();
    
    /*
    index_long_pressure is increased in Custom_LED_Blinking (overflow 250 ms); it is used to count the time between
    negative edge and positive edge of the same pressure--> duration of pressing. everytime a negative edge is detected this index is initialized.
    */
    
    index_long_pressure=0;
 
}

CY_ISR(Custom_Pin_Button_Positive){
    
    /*      POSITIVE EDGE OF THE DEBOUNCED BUTTON --> BUTTON RELEASED
    when the button is released the first thing to be checked is the time intercurred 
    between button pressed and button released.
    since overflow is 250 ms, index is compared to TIME FACTOR=6 that means 1500 ms
    if the time is higher than time factor : CONFIGURATION MODE
    if the time is lower that time factor: check for double click options (START/STOP ACQUISITION) 
    */
    
    if (index_long_pressure>=TIME_FACTOR){
        
        /*LONG PRESSURE: configuration mode. depending on the current configuration state
        enter or exit configuration mode
        */
        if (configuration_status==OFF){
            
            configuration_status=ON;
                        
            /* DATA REGISTER (0x0000 EEPROM): system_status - - - - - - UARTVerboseFlag
            i.e. if the system is ON (acquisition) and the flag is high 
            (transmission via UART to the Bridge Control Panel) data 
            register is: 10000001 (0x81)*/
            data_register = (0<<7) | (UARTVerboseFlag);
            new_EEPROM=1;
            
            RGBLed_WriteColor(OFF, OFF, OFF);
            SPIM_1_Stop();
            Timer_Blinking_Start();
            
        }
        else if (configuration_status==ON){

            configuration_status=OFF;
            
            /*
            if the system is exiting the configuration mode, RETURN to the previous
            system status.
            i.e. it's possible to enter configuration mode both from system ON and system OFF. then, 
            CONFIGURATION mode is turned OFF, the system returns back to ON or OFF. 
            */

            if (system_status==OFF){
                Pin_Led_Blue_Write(OFF);
            }
            else if (system_status==ON){
                Pin_Led_Blue_Write(ON);
                SPIM_1_Enable();
                RGBLed_Start();
                Timer_Blinking_Stop();   
            }
            
            data_register = (system_status<<7) | (UARTVerboseFlag);
            new_EEPROM=1;
        }
        
        /*
        flag for switch case for double click detection. if in long pressure,
        initialize to 0 --> long pressure not detected as normal click
        */
        TimerFlag=0;
    }
    
    /*
    the system CANNOT enter the start/stop CONDITIONS if it's in configuration mode.
    so it enters if:
    - time between push and release of the button is lower than time factor (not long pressure)
       --> CHECK INSIDE IF DOUBLE CLICK
    - the system is not in CONFIGURATION mode
    */
    
    else if (index_long_pressure<TIME_FACTOR && configuration_status==OFF) {
        switch (TimerFlag)
    	{
    		case 0: 
            /*
            first time that the button has been pressed. normal pressure, SINGLE. Nothing to do.
            if the button has been released, and it was not a long pressure, that is identified as click.
            TIMER button start, that is the one used to measure the time intercurring between two clicks, is re-started.
            it will be stopped in the NEGATIVE isr of the next debounced pressure
            */
            TimerFlag=1;
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
                SPIM_1_Enable();
                RGBLed_Start();
                Timer_Blinking_Stop();
                system_status=ON;
                Pin_Led_Blue_Write(ON);
                
                data_register = (system_status<<7) | (UARTVerboseFlag);
                new_EEPROM=1;
            }
            
            else if (system_status==ON){
                /******************* SYSTEM STOP ACQUISITION ******************/
                system_status=OFF;
                Pin_Led_Blue_Write(OFF);
                data_register = (system_status<<7) | (UARTVerboseFlag);
                new_EEPROM=1;

                RGBLed_WriteColor(OFF, OFF, OFF);
                SPIM_1_Stop();
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
            TIMER_button_Start(); 
    		break;
            
        }

    }
}

CY_ISR(Custom_Timer_Button){
    /* 
    TIMER for double click interval. this timer has been started in Custom_Pin_Button_Positive (released button)
    if it reaches overflow before next button negative edge, the flag is set to 2
    */
    TimerFlag=2;
    TIMER_button_ReadStatusRegister();

}

CY_ISR(Custom_LED_Blinking){
    /* 
    ISR on overflow 250 ms
    1. count the time between NEGATIVE (pressed) and POSITIVE (released)
    edge of the debouncer
    2. toggle the led if configuration mode
    3. ADC sampling of potentiometer when in configuration mode
    */
    Timer_Blinking_ReadStatusRegister();
    index_long_pressure++;
    
    if (configuration_status==1){
        
        Pin_Led_Blue_Write(!Pin_Led_Blue_Read());
        value_POT = ADC_DelSig_Read32();        
        if (value_POT < FULL_SCALE_RANGE_LOW) value_POT = FULL_SCALE_RANGE_LOW;
        if (value_POT > FULL_SCALE_RANGE_HIGH) value_POT = FULL_SCALE_RANGE_HIGH;
        
        if(FlagEnableDisable){
            if (value_POT < FULL_SCALE_RANGE_HALF){
                UARTVerboseFlag = OFF;
                Pin_RED_UARTVerboseFlag_Write(OFF);
            }
            else{
                UARTVerboseFlag = ON;
                Pin_RED_UARTVerboseFlag_Write(ON);
            }
            //check if verbose flag is changed to communicate in main.c to UART
            if (UARTVerboseFlagOLD!=UARTVerboseFlag){
                FlagChangeParameters=1;
            }
            UARTVerboseFlagOLD=UARTVerboseFlag;
        }
    }     
}

CY_ISR(Custom_Pin_EnableDisable){
    /*
    Pin 3.2 changes: set new value of enabledisable and communicate this change.
    */
    Pin_EnableDisable_ClearInterrupt();
    FlagEnableDisable=Pin_EnableDisable_Read();
    new_EnableDisable=1;
}

CY_ISR(Custom_TimeStamp){
    Counter_TimeStamp_ReadStatusRegister();
    Counter_overflow=Counter_overflow+1;
}