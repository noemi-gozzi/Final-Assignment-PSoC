/* ========================================
 *
FINAL PROJECT 
Francioli Lorenzo
Gozzi Noemi
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
#define DATA_SIZE 6
#define DATA_BYTES 6
#define CONVERSION_FACTOR_DIGIT_MG 8
#define TRANSMIT_BUFFER_SIZE 8

// New value check
#define LIS3DH_STATUS_REG_NEW_VALUE 0x08 //00001000

#define CONVERSION_MG_RGB 255/4000
char bufferUART[100];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /*          INITIALIZATION          */
    
    UART_1_Start();
    
    /* Start SPI Master */
    SPIM_1_Start();
    SPIM_2_Start();
    RGBLed_Start();
    CyDelay(10);
    
    UART_1_PutString("*********    EEPROM TEST    *********\r\n");
    
    /* Definition of the extern from 25LC256.c */
    //uint8_t eeprom_Status = 0;
    
    /* Data to Write */
    int16_t data[3] = {15, -32, 258};
    
    /**** PROJECT 3 *****/
    
    /* Write */
    EEPROM_writePage(0x0001, (uint8_t*) data, DATA_BYTES);
    
    
    EEPROM_waitForWriteComplete();

    /**** PROJECT 4 ****/
    /* Read */
    int16_t data_EEPROM[3];
    EEPROM_readPage(0x0001, (uint8_t*) data_EEPROM, DATA_BYTES);
    
    
    
    sprintf(bufferUART, "** EEPROM Read = %d %d %d \r\n", data_EEPROM[0], data_EEPROM[1], data_EEPROM[2]);
    UART_1_PutBuffer;
    
    UART_1_PutString("*************************************\r\n");
    
    uint8_t data_read;
    
    /*********************WHO AM I REGISTER*******************/
    data_read = LIS3DH_readByte(LIS3DH_WHO_AM_I_REG_ADDR);
    sprintf(bufferUART, "** LIS3DH WHO I AM REGISTER = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 1*******************/
    /*          settings: 0x57                                */
    /*          0b00100111                                    */
    /*          frequency=10 Hz                               */
    /*          x,y,z enable                                  */
    /*          normal/high resolution mode                   */
    
    LIS3DH_writeByte(LIS3DH_CTRL_REG1, 0x27);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG1);
    sprintf(bufferUART, "** LIS3DH CTRL REGISTER 1= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 4*******************/
    /*          settings:  0x10                               */
    /*          CTRL4[0]=0 SPI with 4 wires (not 3)           */
    /*          CTRL4[4:5]= FULL SCALE RANGE. e.g. 01 +-4g    */
    /*          CTRL4[3]=RESOLUTION. normal 0                 */
    /*                                                        */
    LIS3DH_writeByte(LIS3DH_CTRL_REG4, 0x00);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG4);
    sprintf(bufferUART, "** LIS3DH CTRL REGISTER 4= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 5*******************/
    /*          settings: 0x40                                */
    /*          0b01--0000                                    */
    /*          FIFO enable                                   */
 
    LIS3DH_writeByte(LIS3DH_CTRL_REG5, 0x40);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG5);
    sprintf(bufferUART, "** LIS3DH CTRL REGISTER 5= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************FIFO_CTRL_REG************************/
    /*          settings:  0x41                               */
    /*          0b01000001                                    */
    /*          FIFO mode                                     */
    /*          watermark: 1 sample                           */
    
    // BYPASS MODE
    LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,0x00);
    data_read = LIS3DH_readByte(LIS3DH_FIFO_CTRL_REG);
    sprintf(bufferUART, "** LIS3DH FIFO CTRL REGISTER = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    //FIFO MODE
    LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,0x47);
    data_read = LIS3DH_readByte(LIS3DH_FIFO_CTRL_REG);
    sprintf(bufferUART, "** LIS3DH FIFO CTRL REGISTER = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 3*******************/
    /*          settings:   0x04                              */
    /*          0b000000100                                   */
    /*          watermark interrupt set                       */

    LIS3DH_writeByte(LIS3DH_CTRL_REG3, 0x44);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG3);
    sprintf(bufferUART, "** LIS3DH CTRL REGISTER 3= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************INT1 CFG REGISTER *******************/
    /*          settings:   0x2A                              */
    /*          OR combination                                */
    /*          HIGH event on z y x                           */

    LIS3DH_writeByte(LIS3DH_INT1_CFG, 0x2A);
    data_read = LIS3DH_readByte(LIS3DH_INT1_CFG);
    sprintf(bufferUART, "** LIS3DH INT 1 CFG REGISTER= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************INT1 THS REGISTER *******************/
    /*          settings:   0x3E                              */
    /*          thresh:2000mg                                 */
 
    LIS3DH_writeByte(LIS3DH_INT1_THS, 0x5C);
    data_read = LIS3DH_readByte(LIS3DH_INT1_THS);
    sprintf(bufferUART, "** LIS3DH INT1 THS REGISTER= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*******************INT1 DURATION REGISTER ****************/
    /*          settings:   0x05                              */
    /*          thresh:5/ODR=50ms                                 */
 
    LIS3DH_writeByte(LIS3DH_INT1_DURATION, 0x03);
    data_read = LIS3DH_readByte(LIS3DH_INT1_DURATION);
    sprintf(bufferUART, "** LIS3DH DURATION REGISTER= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    SPIM_1_Stop();
    SPIM_2_Stop();
    UART_1_Stop();
    

    //Variables declaration
//    int8_t AccData[DATA_SIZE];
    int16_t OutAccX;
    int16_t OutAccY;
    int16_t OutAccZ;
//    uint8_t header = 0xA0;
//    uint8_t footer = 0xC0;
//    uint8_t OutArray[TRANSMIT_BUFFER_SIZE];
   
    //Header and footer set up
//    OutArray[0] = header;
//    OutArray[TRANSMIT_BUFFER_SIZE - 1] = footer;
    uint8_t red_x, green_y, blue_z;
    int16_t Acc_x;
    int16_t Acc_y;
    int16_t Acc_z;
    
    Acc_x = 0;
    Acc_y = 0;
    Acc_z = 0;
    
    PacketReadyFlag = 0;
    isr_ACC_StartEx(Custom_Pin_ISR);
    
    isr_DEBOUNCER_StartEx(Custom_Pin_Button);
    isr_TIMER_StartEx(Custom_Timer_Button);
    isr_BLINKING_StartEx(Custom_LED_Blinking);
    isr_positive_StartEx(Custom_Pin_Button_Positive);
    
    Timer_Blinking_Start();
    
    for(;;)
    {

        
//        data_read = LIS3DH_readByte(LIS3DH_FIFO_SRC_REG);
//        sprintf(bufferUART, "** LIS3DH FIFO SRC REG= 0x%02X\r\n", data_read);
//        UART_1_PutBuffer;


        if (PacketReadyFlag==1){
            uint8_t data = LIS3DH_readByte(LIS3DH_INT1_SRC);
            if(data&0x40){
                sprintf(bufferUART, "THRESH 0x%02X\r\n", data);
                UART_1_PutBuffer;
            }
            else{
                sprintf(bufferUART, "watermark \r\n");
                UART_1_PutBuffer;
                
                for (int i=0; i<7; i++){
                    LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
                    Acc_x = Acc_x + (((AccData[0]) | ((AccData[1])<<8))>>6);
                    Acc_y = Acc_y + (((AccData[2]) | ((AccData[3])<<8))>>6);
                    Acc_z = Acc_z + (((AccData[4]) | ((AccData[5])<<8))>>6);
                    
                }
                Acc_x = Acc_x/7;
                Acc_y = Acc_y/7;
                Acc_z = Acc_z/7;
                
                OutAccX = Acc_x * CONVERSION_FACTOR_DIGIT_MG;
                OutAccY = Acc_y * CONVERSION_FACTOR_DIGIT_MG;
                OutAccZ = Acc_z * CONVERSION_FACTOR_DIGIT_MG;
//                OutAccX = ((int16)((AccData[0]) | ((AccData[1])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
//                OutAccY = ((int16)((AccData[2]) | ((AccData[3])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
//                OutAccZ = ((int16)((AccData[4]) | ((AccData[5])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
                
                
            
                //data preparing for UART serial Communication
//                OutArray[1] = (uint8_t)(OutAccX & 0xFF); 
//                OutArray[2] = (uint8_t)(OutAccX >> 8);
//                OutArray[3] = (uint8_t)(OutAccY & 0xFF);
//                OutArray[4] = (uint8_t)(OutAccY >> 8);
//                OutArray[5] = (uint8_t)(OutAccZ & 0xFF);
//                OutArray[6] = (uint8_t)(OutAccZ >> 8);
//                
//                UART_1_PutArray(OutArray, TRANSMIT_BUFFER_SIZE);

                
                red_x= (uint8_t)(abs(OutAccX*CONVERSION_MG_RGB));
                green_y=(uint8_t)(abs(OutAccY*CONVERSION_MG_RGB));
                blue_z=(uint8_t)(abs(OutAccZ*CONVERSION_MG_RGB));
                sprintf(bufferUART, "RED value: %d, GREEN value: %d, BLUE value: %d \r\n", red_x, green_y, blue_z);
                UART_1_PutBuffer;
                RGBLed_WriteColor(red_x, green_y, blue_z);
            }
        
            
        PacketReadyFlag=0;
        LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,0x00);
        LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,0x47);
        
        }
    
//    CyDelay(10);


//        uint8_t status_register = LIS3DH_readByte(LIS3DH_STATUS_REG);
//        if (((status_register) & (LIS3DH_STATUS_REG_NEW_VALUE))){
//            
//            LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
//        
//            OutAccX = ((int16)((AccData[0]) | ((AccData[1])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
//            OutAccY = ((int16)((AccData[2]) | ((AccData[3])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
//            OutAccZ = ((int16)((AccData[4]) | ((AccData[5])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
            
            //data preparing for UART serial Communication
//            OutArray[1] = (uint8_t)(OutAccX & 0xFF); 
//            OutArray[2] = (uint8_t)(OutAccX >> 8);
//            OutArray[3] = (uint8_t)(OutAccY & 0xFF);
//            OutArray[4] = (uint8_t)(OutAccY >> 8);
//            OutArray[5] = (uint8_t)(OutAccZ & 0xFF);
//            OutArray[6] = (uint8_t)(OutAccZ >> 8);
//            
//            UART_1_PutArray(OutArray, TRANSMIT_BUFFER_SIZE);
//        }
        
//              OutAccX=-1000;
//              OutAccY=0;
//              OutAccZ=-2000;
//            
            /* RGB VALUE DEFINITION BASED ON ACCELERATION DATA*/
            
//              uint8_t red_x, green_y, blue_z;
//              //int abs (int x);
//              red_x= (uint8_t)(abs(OutAccX*CONVERSION_MG_RGB));
//              green_y=(uint8_t)(abs(OutAccY*CONVERSION_MG_RGB));
//              blue_z=(uint8_t)(abs(OutAccZ*CONVERSION_MG_RGB));
//              RGBLed_WriteColor(red_x, green_y, blue_z);


    }
}

/* [] END OF FILE */
