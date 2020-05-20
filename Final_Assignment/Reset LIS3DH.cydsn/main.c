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
#include "project.h"
#include "LIS3DH.h"
#include "LIS3DH_SPI.h"
#include "SPI_Interface.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "InterruptRoutines.h"
#include "RGBLedDriver.h"

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
    RGBLed_Start();
    CyDelay(10);
    
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
    
    LIS3DH_writeByte(LIS3DH_CTRL_REG1, 0x57);
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

    LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,0x00);
    data_read = LIS3DH_readByte(LIS3DH_FIFO_CTRL_REG);
    sprintf(bufferUART, "** LIS3DH FIFO CTRL REGISTER = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG,0x42);
    data_read = LIS3DH_readByte(LIS3DH_FIFO_CTRL_REG);
    sprintf(bufferUART, "** LIS3DH FIFO CTRL REGISTER = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 3*******************/
    /*          settings:   0x04                              */
    /*          0b000000100                                   */
    /*          watermark interrupt set                       */

    LIS3DH_writeByte(LIS3DH_CTRL_REG3, 0x00);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG3);
    sprintf(bufferUART, "** LIS3DH CTRL REGISTER 3= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************INT1 CFG REGISTER *******************/
    /*          settings:   0x2A                              */
    /*          OR combination                                */
    /*          HIGH event on z y x                           */

    LIS3DH_writeByte(LIS3DH_INT1_CFG, 0x00);
    data_read = LIS3DH_readByte(LIS3DH_INT1_CFG);
    sprintf(bufferUART, "** LIS3DH INT 1 CFG REGISTER= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************INT1 THS REGISTER *******************/
    /*          settings:   0x3E                              */
    /*          thresh:2000mg                                 */
 
    LIS3DH_writeByte(LIS3DH_INT1_THS, 0x00);
    data_read = LIS3DH_readByte(LIS3DH_INT1_THS);
    sprintf(bufferUART, "** LIS3DH INT1 THS REGISTER= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*******************INT1 DURATION REGISTER ****************/
    /*          settings:   0x05                              */
    /*          thresh:5/ODR=50ms                                 */
 
    LIS3DH_writeByte(LIS3DH_INT1_DURATION, 0x00);
    data_read = LIS3DH_readByte(LIS3DH_INT1_DURATION);
    sprintf(bufferUART, "** LIS3DH DURATION REGISTER= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
 
    /******TEST READ WRITE PAGE************/
//    uint8_t data[3];
//    LIS3DH_readPage(LIS3DH_CTRL_REG1, (uint8_t*)data, 3);
//    sprintf(bufferUART, "** LIS3DH CTRL REGISTER 6=  0x%02X 0x%02X 0x%02X \r\n", data[0], data[1], data[2]);
//    UART_1_PutBuffer;
    /* Write */
//    uint8_t datapage[1];
//    datapage[0]= 0x37;
//    
//    LIS3DH_writePage(LIS3DH_CTRL_REG1, (uint8_t*) datapage, DATA_BYTES);


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
    
    PacketReadyFlag = 0;
    isr_ACC_StartEx(Custom_Pin_ISR);
    
    for(;;)
    {
        //READ LIS3DH STATUS REGISTER: EXPECTED IT CHANGES
        //PROBLEM=IT IS ALWAYS 0X20 (EMPTY)
//        
        data_read = LIS3DH_readByte(LIS3DH_FIFO_SRC_REG);
        sprintf(bufferUART, "** LIS3DH FIFO SRC REG= 0x%02X\r\n", data_read);
        UART_1_PutBuffer;
        if (data_read&0x80){
            
            LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES); 
            LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
            LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
            sprintf(bufferUART, "LETTI");
            UART_1_PutBuffer;
            data_read = LIS3DH_readByte(LIS3DH_FIFO_SRC_REG);
            sprintf(bufferUART, "** LIS3DH FIFO SRC REG= 0x%02X\r\n", data_read);
            UART_1_PutBuffer;
        }
        
        
        if (PacketReadyFlag){
        PacketReadyFlag=0;
        sprintf(bufferUART, "ciao\r\n");
        UART_1_PutBuffer;
        RGBLed_WriteColor(255, 0, 0);
        
        }

//        uint8_t status_register = LIS3DH_readByte(LIS3DH_STATUS_REG);
//        if (((status_register) & (LIS3DH_STATUS_REG_NEW_VALUE))){
//            
//            LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
//        
            OutAccX = ((int16)((AccData[0]) | ((AccData[1])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
            OutAccY = ((int16)((AccData[2]) | ((AccData[3])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
            OutAccZ = ((int16)((AccData[4]) | ((AccData[5])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
            
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
        
              OutAccX=-1000;
              OutAccY=0;
              OutAccZ=-2000;
            
            /* RGB VALUE DEFINITION BASED ON ACCELERATION DATA*/
            
              uint8_t red_x, green_y, blue_z;
              //int abs (int x);
              red_x= (uint8_t)(abs(OutAccX*CONVERSION_MG_RGB));
              green_y=(uint8_t)(abs(OutAccY*CONVERSION_MG_RGB));
              blue_z=(uint8_t)(abs(OutAccZ*CONVERSION_MG_RGB));
              RGBLed_WriteColor(red_x, green_y, blue_z);


    }
}

/* [] END OF FILE */
