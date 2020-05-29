/* ========================================
 *
 * FINAL PROJECT 
 * Francioli Lorenzo
 * Gozzi Noemi
 *
 * ========================================
*/
#include "project.h"
#include "LIS3DH.h"
#include "LIS3DH_SPI.h"
#include "SPI_Interface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "InterruptRoutines.h"
#include "RGBLedDriver.h"
#include "25LC256.h"
#include "SPI_Interface_EEPROM.h"

#define UART_1_PutBuffer UART_1_PutString(bufferUART)
#define DATA_BYTES 6
#define CONVERSION_FACTOR_DIGIT_MG 4
#define TRANSMIT_BUFFER_SIZE 8
#define CONVERSION_MG_RGB 255/2000
#define CONVERSION_COUNTER_TIMESTAMP_SEC 0.02 //each clock 0.02 sec
#define CONVERSION_COUNTER_ISR 1200 //clock overflow 60000*0.02=1200 sec. when an ISR occured 1200 sec have passed
#define FSR_COUNTER 60000
#define EEPROM_ADDRESS_TIMESTAMP 0x0002
#define EEPROM_ADDRESS_ENABLEDISABLE 0x0001
#define EEPROM_ADDRESS_STATUS_VERBOSE_FLAG 0x0000
#define EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD 0x0080

#define ACC_PACKET_DIMENSION 6
#define empty_bit 0x20
char bufferUART[100];


int main(void)
{
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /*          INITIALIZATION          */
    UART_1_Start();
    SPIM_1_Start();
    SPIM_2_Start();
    RGBLed_Start();
    ADC_DelSig_Start();
    Counter_TimeStamp_Start();

    CyDelay(10);
    
    sprintf(bufferUART, "^^^^^^^^ Final Project: Noemi Gozzi, Lorenzo Francioli ^^^^^^^^\r\n\n");
    UART_1_PutBuffer;
    
    /*********************CONFIGURATION and TEST SECTION*******************/
    sprintf(bufferUART, "**************** TEST EEPROM and LIS3DH CONFIGURATION******************* \r\n\n");
    UART_1_PutBuffer;
    
    /*********************EEPROM TEST*******************/
    int16_t data_test_eeprom[3] = {15, -32, 258};
    EEPROM_writePage(0x0020, (uint8_t*) data_test_eeprom, DATA_BYTES);
    EEPROM_waitForWriteComplete();
    int16_t data_EEPROM[3];
    EEPROM_readPage(0x0020, (uint8_t*) data_EEPROM, DATA_BYTES);
    sprintf(bufferUART, " --> EEPROM Test Read = %d %d %d, expected 15 -32 258\r\n\n", data_EEPROM[0], data_EEPROM[1], data_EEPROM[2]);
    UART_1_PutBuffer;
    
    uint8_t data_read;
    
    /*********************WHO AM I REGISTER*******************/
    data_read = LIS3DH_readByte(LIS3DH_WHO_AM_I_REG_ADDR);
    sprintf(bufferUART, " --> LIS3DH WHO I AM REGISTER = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 1*******************/
    /*          settings: 0x47                                */
    /*          0b01000111                                    */
    /*          CTRL1[4:7]=0100 frequency=50 Hz               */
    /*          CTRL1[0:2]=1 x,y,z enable                     */
    /*          CTRL1[3]=1 normal/high resolution mode        */
    
    LIS3DH_writeByte(LIS3DH_CTRL_REG1, 0x47);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG1);
    sprintf(bufferUART, " --> LIS3DH CTRL REGISTER 1= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 4*******************/
    /*          settings:  0x00                               */
    /*          CTRL4[0]=0 SPI with 4 wires (not 3)           */
    /*          CTRL4[4:5]= FULL SCALE RANGE. e.g. 00 +-2g    */
    /*          CTRL4[3]=RESOLUTION. normal 0                 */
    /*                                                        */
    LIS3DH_writeByte(LIS3DH_CTRL_REG4, LIS3DH_CTRL_REG4_2G_NORMAL);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG4);
    sprintf(bufferUART, " --> LIS3DH CTRL REGISTER 4= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 5*******************/
    /*          settings: 0x40                                */
    /*          0b01--0000                                    */
    /*          CTRL5[6]=1 FIFO enable                        */
 
    LIS3DH_writeByte(LIS3DH_CTRL_REG5, LIS3DH_CTRL_REG5_FIFO_EN);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG5);
    sprintf(bufferUART, " --> LIS3DH CTRL REGISTER 5= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************FIFO_CTRL_REG************************/
    /*          settings:  0x00                               */
    /*          0b00000000                                    */
    /*          FIFOCTRL[6:7]=00 BYPASS mode                   */
    LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,LIS3DH_FIFO_CTRL_REG_BYPASS_MODE);
    data_read = LIS3DH_readByte(LIS3DH_FIFO_CTRL_REG);
    sprintf(bufferUART, " --> LIS3DH FIFO CTRL REGISTER (BYPASS mode) = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*          settings:  0x47                               */
    /*          0b01000111                                    */
    /*          FIFOCTRL[6:7]=01 FIFO mode                    */
    /*          FIFOCTRL[5]=0 allows triggering signal on INT1*/
    /*          FIFOCTRL[0:4]=00111 watermark: 7 sample       */
    LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_FIFO_MODE_WTM_7);
    data_read = LIS3DH_readByte(LIS3DH_FIFO_CTRL_REG);
    sprintf(bufferUART, " --> LIS3DH FIFO CTRL REGISTER (FIFO mode) = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 3*******************/
    /*          settings:   0x44                              */
    /*          0b01000100                                    */
    /*          CTRL3[6]=1 IA1 interrupt on INT1              */
    /*          CTRL3[2]=1 watermark interrupt set            */

    LIS3DH_writeByte(LIS3DH_CTRL_REG3, LIS3DH_CTRL_REG3_IA1_WTM_INT);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG3);
    sprintf(bufferUART, " --> LIS3DH CTRL REGISTER 3= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************INT1 CFG REGISTER *******************/
    /*          settings:   0x2A                              */
    /*          0b00101010                                    */
    /*          INT1CFG[7]=0 OR combination of interrupts     */
    /*          INT1CFG[5]=1 enable interrupt on Z HIGH event */
    /*          INT1CFG[3]=1 enable interrupt on Y HIGH event */
    /*          INT1CFG[1]=1 enable interrupt on X HIGH event */

    LIS3DH_writeByte(LIS3DH_INT1_CFG, LIS3DH_INT1_CFG_OR_XYZIHE);
    data_read = LIS3DH_readByte(LIS3DH_INT1_CFG);
    sprintf(bufferUART, " --> LIS3DH INT 1 CFG REGISTER= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************INT1 THS REGISTER *******************/
    /*          settings:   0x52                              */
    /*          0b01011100                                    */
    /*          thresh:1312mg (82 x 16 mg)                    */
    
    LIS3DH_writeByte(LIS3DH_INT1_THS, 0x52);
    data_read = LIS3DH_readByte(LIS3DH_INT1_THS);
    sprintf(bufferUART, " --> LIS3DH INT1 THS REGISTER= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*******************INT1 DURATION REGISTER ****************/
    /*          settings:   0x04                              */
    /*          thresh:4/ODR=160ms (1/ODR = 40 ms)            */
 
    LIS3DH_writeByte(LIS3DH_INT1_DURATION, 0x04);
    data_read = LIS3DH_readByte(LIS3DH_INT1_DURATION);
    sprintf(bufferUART, " --> LIS3DH DURATION REGISTER= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*******************CONFIGURATION MODE ****************/    
    
    FlagEnableDisable=Pin_EnableDisable_Read();
    EEPROM_writeByte(EEPROM_ADDRESS_ENABLEDISABLE, FlagEnableDisable);
    EEPROM_waitForWriteComplete();
    
    if (FlagEnableDisable){
        sprintf(bufferUART, "\r\nREAD/WRITE CONFIGURATION MODALITY. Configuration mode: %d\r\nUART VERBOSE FLAG: %d\r\n", FlagEnableDisable, UARTVerboseFlag);
        UART_1_PutBuffer;
    }
    else {
        sprintf(bufferUART, "\r\nATTENTION! READ ONLY MODALITY. Configuration mode: %d\r\nUART VERBOSE FLAG: %d\r\n", FlagEnableDisable, UARTVerboseFlag);
        UART_1_PutBuffer;
    }  
    
    UART_1_PutString("\r\nREADY \r\n\r\n");
    CyDelay(10);
    
    /* Variables declaration */
    uint8_t AccData[48];
    uint8_t AccData_trash[ACC_PACKET_DIMENSION];
    uint8_t AccData_Threshold [48];
    uint32 timestamp;
    int16_t Acc_x;
    int16_t Acc_y;
    int16_t Acc_z;
    int16 OutAccX;
    int16 OutAccY;
    int16 OutAccZ;
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray[TRANSMIT_BUFFER_SIZE];
    uint8_t red_x, green_y, blue_z;
    //    uint8_t reading_eeprom;
    
    /* Header and footer set up */
    OutArray[0] = header;
    OutArray[TRANSMIT_BUFFER_SIZE - 1] = footer;

     /* ISRs StartEx */   
    isr_ACC_StartEx(Custom_Pin_ISR);
    isr_DEBOUNCER_StartEx(Custom_Pin_Button);
    isr_TIMER_StartEx(Custom_Timer_Button);
    isr_BLINKING_StartEx(Custom_LED_Blinking);
    isr_positive_StartEx(Custom_Pin_Button_Positive);
    isr_EnableDisable_StartEx(Custom_Pin_EnableDisable);
    isr_TimeStamp_StartEx(Custom_TimeStamp);
        
    ADC_DelSig_StartConvert();
    
    CyDelay(10);
    
    /* Variables initialization */
    UARTVerboseFlag=0;
    new_EEPROM=0;
    new_EnableDisable=0;
    Counter_overflow=0;
    PacketReadyFlag = 0;
    system_status=0;
    configuration_status=0;
    FlagChangeParameters=0;

    /* The Accelerometer SPI must be ON only in acquisition mode*/
    SPIM_1_Stop();

    for(;;)
    {
        /*
        TESTING FOR eeprom 0x0000 --> data register system_status - - - - - - verboseFlag      
        reading_eeprom = EEPROM_readByte(0x0000);
        sprintf(bufferUART, "** DATA REGISTER= 0x%02X\r\n", reading_eeprom);
        UART_1_PutBuffer;
        */
        
        /* Variables initialization */
        Acc_x = 0;
        Acc_y = 0;
        Acc_z = 0;
        
        /*
        If the system enters in a new state (from acquisition ON to OFF or viceversa, 
        from acquisition ON to CONFIGURATION MODE), the system state is stored at the 
        EEPROM address 0x0000, together with the value of the verbose flag, indicating 
        the enabled or disabled communication via Bridge Control Panel
        */
        if(new_EEPROM){
            EEPROM_writeByte(EEPROM_ADDRESS_STATUS_VERBOSE_FLAG, data_register);
            EEPROM_waitForWriteComplete();
            new_EEPROM=0;
        }
        
        /*
        */
        if(new_EnableDisable){
            EEPROM_writeByte(EEPROM_ADDRESS_ENABLEDISABLE, FlagEnableDisable);
            EEPROM_waitForWriteComplete();
            new_EnableDisable=0;

            if (FlagEnableDisable){
                sprintf(bufferUART, "READ/WRITE CONFIGURATION MODALITY. configuration mode: %d\r\nUART VERBOSE FLAG: %d\r\n", FlagEnableDisable, UARTVerboseFlag);
                UART_1_PutBuffer;
            }
            else {
                sprintf(bufferUART, "ATTENTION! READ ONLY MODALITY. configuration mode: %d\r\nUART VERBOSE FLAG: %d\r\n", FlagEnableDisable, UARTVerboseFlag);
                UART_1_PutBuffer;
            }    
        }
        /*
        */
        if (configuration_status && FlagChangeParameters){
                sprintf(bufferUART, "NEW UART VERBOSE FLAG: %d\r\n", UARTVerboseFlag);
                UART_1_PutBuffer;
                FlagChangeParameters=0;
        
        }

        if (PacketReadyFlag==1 && system_status==1 && configuration_status==0){
            uint8_t data = LIS3DH_readByte(LIS3DH_INT1_SRC);
            if(data&0x40){
                sprintf(bufferUART, "EVENT OVERTHRESHOLD\r\n");
                UART_1_PutBuffer;
                
                /*              SIGNAL WAVEFORM              */
                uint8 j=0;
                uint8 count_fifo=0;
                
                LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData_trash, ACC_PACKET_DIMENSION);
                while(j<=7){
                    
                    /*FIFO REGISTER: ONLY IF IT'S NOT EMPTY WE READ AND SAVE NEW VALUES*/
                    
                    if ((LIS3DH_readByte(LIS3DH_FIFO_SRC_REG))!=empty_bit){
                    LIS3DH_readPage(LIS3DH_OUT_X_L,  &AccData_Threshold[0+j], ACC_PACKET_DIMENSION);
                    count_fifo++;
                    }
                    j++;
                }
                
                /* SIGNAL WAVEFORM CASES
                - if the new values saved in the FIFO are less than the overthreshold event duration(cases 1, 2, 3, 4)
                  the data are retrieved from the previous FIFO saved in accData (watermark reading)
                  e.g. only 2 values in FIFO
                  OVERTHRESHOLD overall duration 5 samples (INT1_DURATION register counts also if the FIFO is cleared.)
                  so, it means that some overthreshold values were in the previous read FIFO.
                  --> read 2 values from current FIFO, and 3 last values from previous FIFO (most recent ones).
                
                - if the new values saved in the FIFO are more or equal than the overthreshold event duration (cases 5,6,7,8)
                  only the most recent 5 acceleration packets are identified as signalwaveform.
                
                */
                switch (count_fifo)
            	{
            		case 1:
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD),&AccData[24], ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD+ACC_PACKET_DIMENSION*1),&AccData[30], ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD+ACC_PACKET_DIMENSION*2),&AccData[36], ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD+ACC_PACKET_DIMENSION*3),&AccData[42], ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD+ACC_PACKET_DIMENSION*4),&AccData_Threshold[0], 1*ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
            		break;
                    case 2: 
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD),&AccData[30], 6);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD+ACC_PACKET_DIMENSION*1),&AccData[36], ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD+ACC_PACKET_DIMENSION*2),&AccData[42], ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD+ACC_PACKET_DIMENSION*3),&AccData_Threshold[0], 2*ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
            		break;
                    case 3:
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD),&AccData[36], ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD+ACC_PACKET_DIMENSION*1),&AccData[42], ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD+ACC_PACKET_DIMENSION*2),&AccData_Threshold[0], 3*ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
            		break;
                    case 4: 
            		EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD),&AccData[42], ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
                    EEPROM_writePage((EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD+ACC_PACKET_DIMENSION*1),&AccData_Threshold[0], 4*ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
                    break;
                    case 5:
                    EEPROM_writePage(EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD,&AccData_Threshold[0], 5*ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
            		break;
                    case 6:
                    EEPROM_writePage(EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD,&AccData_Threshold[6], 5*ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
            		break;
                    case 7:
                    EEPROM_writePage(EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD,&AccData_Threshold[12], 5*ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
            		break;
                    case 8:
                    EEPROM_writePage(EEPROM_ADDRESS_ACC_DATA_OVER_THRESHOLD,&AccData_Threshold[18], 5*ACC_PACKET_DIMENSION);
                    EEPROM_waitForWriteComplete();
            		break;
                }
                
                LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,LIS3DH_FIFO_CTRL_REG_BYPASS_MODE);
                LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,LIS3DH_FIFO_CTRL_REG_FIFO_MODE_WTM_7);
                
                /*              TIME STAMP              */
                
                timestamp=(FSR_COUNTER-Counter_TimeStamp_ReadCounter())*CONVERSION_COUNTER_TIMESTAMP_SEC;
                timestamp=timestamp+Counter_overflow*CONVERSION_COUNTER_ISR;
                
                uint8 timestamp_array[4];
                timestamp_array[0]=(timestamp>>24)&0xFF; //MSB
                timestamp_array[1]=(timestamp>>16)&0xFF;
                timestamp_array[2]=(timestamp>>8)&0xFF;
                timestamp_array[3]=(timestamp)&0xFF; //LSB
                
                EEPROM_writePage(EEPROM_ADDRESS_TIMESTAMP,(uint8_t*)timestamp_array, 4);
                EEPROM_waitForWriteComplete();
                             
            }
            else{
                LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData_trash, DATA_BYTES);
                for (int i=0; i<=7; i++){
                    
                    LIS3DH_readPage(LIS3DH_OUT_X_L, &AccData[0+i], DATA_BYTES);
                    Acc_x = Acc_x + ((int16)((AccData[0+i]) | ((AccData[1+i])<<8))>>6);
                    Acc_y = Acc_y + ((int16)((AccData[2+i]) | ((AccData[3+i])<<8))>>6);
                    Acc_z = Acc_z + ((int16)((AccData[4+i]) | ((AccData[5+i])<<8))>>6);

                }
                
                LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,LIS3DH_FIFO_CTRL_REG_BYPASS_MODE);
                LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,LIS3DH_FIFO_CTRL_REG_FIFO_MODE_WTM_7);
                
                Acc_x = Acc_x/8;
                Acc_y = Acc_y/8;
                Acc_z = Acc_z/8;
                
                OutAccX = Acc_x * CONVERSION_FACTOR_DIGIT_MG;
                OutAccY = Acc_y * CONVERSION_FACTOR_DIGIT_MG;
                OutAccZ = Acc_z * CONVERSION_FACTOR_DIGIT_MG;
                sprintf(bufferUART, "X: %d, Y: %d, Z: %d [mg]\r\n", OutAccX, OutAccY, OutAccZ);
                UART_1_PutBuffer;
                
                if (UARTVerboseFlag){
                    /*Data preparing for UART serial Communication with Bridge Control Panel*/
                    OutArray[1] = (uint8_t)(OutAccX & 0xFF); 
                    OutArray[2] = (uint8_t)(OutAccX >> 8);
                    OutArray[3] = (uint8_t)(OutAccY & 0xFF);
                    OutArray[4] = (uint8_t)(OutAccY >> 8);
                    OutArray[5] = (uint8_t)(OutAccZ & 0xFF);
                    OutArray[6] = (uint8_t)(OutAccZ >> 8);
                    
                    UART_1_PutArray(OutArray, TRANSMIT_BUFFER_SIZE);
                }
                
                red_x= (uint8_t)(abs(OutAccX*CONVERSION_MG_RGB));
                green_y=(uint8_t)(abs(OutAccY*CONVERSION_MG_RGB));
                blue_z=(uint8_t)(abs(OutAccZ*CONVERSION_MG_RGB));

                RGBLed_WriteColor(red_x, green_y, blue_z);
            }
          
        PacketReadyFlag=0;
        
        }    
    }
}

/* [] END OF FILE */
