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

#define UART_1_PutBuffer UART_1_PutString(bufferUART)
#define DATA_SIZE 6
#define DATA_BYTES 6
#define CONVERSION_FACTOR_DIGIT_MG 8
#define TRANSMIT_BUFFER_SIZE 8

// New value check
#define LIS3DH_STATUS_REG_NEW_VALUE 0x08 //00001000

char bufferUART[100];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /*          INITIALIZATION          */
    
    UART_1_Start();
    
    /* Start SPI Master */
    SPIM_1_Start();
    
    CyDelay(10);
    uint8_t data_read;
    
    /*********************WHO AM I REGISTER*******************/
    data_read = LIS3DH_readByte(LIS3DH_WHO_AM_I_REG_ADDR);
    sprintf(bufferUART, "** LIS3DH WHO I AM REGISTER = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 1*******************/
    /*          settings:                                     */
    /*          0b00100111                                    */
    /*          frequency=10 Hz                               */
    /*          x,y,z enable                                  */
    /*          normal/high resolution mode                   */
    
    LIS3DH_writeByte(LIS3DH_CTRL_REG1, LIS3DH_NORMAL_MODE_CTRL_REG1_10Hz);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG1);
    sprintf(bufferUART, "** LIS3DH CTRL REGISTER 1= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 4*******************/
    /*          settings:                                     */
    /*          CTRL4[0]=0 SPI with 4 wires (not 3)           */
    /*          CTRL4[4:5]= FULL SCALE RANGE. e.g. 01 +-4g    */
    /*          CTRL4[3]=RESOLUTION. normal 0                 */
    /*                                                        */
    LIS3DH_writeByte(LIS3DH_CTRL_REG4, LIS3DH_CTRL_REG4_4G_NORMAL);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG4);
    sprintf(bufferUART, "** LIS3DH CTRL REGISTER 4= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 5*******************/
    /*          settings:                                     */
    /*          0b01--0000                                    */
    /*          FIFO enable                                   */
 
    LIS3DH_writeByte(LIS3DH_CTRL_REG5, LIS3DH_CTRL_REG5_FIFO_EN);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG5);
    sprintf(bufferUART, "** LIS3DH CTRL REGISTER 5= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************FIFO_CTRL_REG*******************/
    /*          settings:                                     */
    /*          0b01000001                                    */
    /*          FIFO mode                                     */
    /*          watermark: 1 sample                           */

    LIS3DH_writeByte(LIS3DH_FIFO_CTRL_REG, LIS3DH_FIFO_CTRL_REG_FIFO_MODE_WTM_1);
    data_read = LIS3DH_readByte(LIS3DH_FIFO_CTRL_REG);
    sprintf(bufferUART, "** LIS3DH FIFO CTRL REGISTER = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /*********************CONTROL REGISTER 3*******************/
    /*          settings:                                     */
    /*          0b00000010                                    */
    /*          watermark interrupt set                       */

    LIS3DH_writeByte(LIS3DH_CTRL_REG3, LIS3DH_CTRL_REG3_WTM_INT);
    data_read = LIS3DH_readByte(LIS3DH_CTRL_REG3);
    sprintf(bufferUART, "** LIS3DH CTRL REGISTER 3= 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    
    /* Write */
//    uint8_t datapage[1];
//    datapage[0]= 0x37;
//    
//    LIS3DH_writePage(LIS3DH_CTRL_REG1, (uint8_t*) datapage, DATA_BYTES);


    //Variables declaration
//    int8_t AccData[DATA_SIZE];
//    int16_t OutAccX;
//    int16_t OutAccY;
//    int16_t OutAccZ;
//    uint8_t header = 0xA0;
//    uint8_t footer = 0xC0;
//    uint8_t OutArray[TRANSMIT_BUFFER_SIZE];
   
    //Header and footer set up
//    OutArray[0] = header;
//    OutArray[TRANSMIT_BUFFER_SIZE - 1] = footer;
    
    
    for(;;)
    {
//        uint8_t status_register = LIS3DH_readByte(LIS3DH_STATUS_REG);
//        if (((status_register) & (LIS3DH_STATUS_REG_NEW_VALUE))){
//            
//            LIS3DH_readPage(LIS3DH_OUT_X_L, (uint8_t*) AccData, DATA_BYTES);
//        
//            OutAccX = ((int16)((AccData[0]) | ((AccData[1])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
//            OutAccY = ((int16)((AccData[2]) | ((AccData[3])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
//            OutAccZ = ((int16)((AccData[4]) | ((AccData[5])<<8))>>6)*CONVERSION_FACTOR_DIGIT_MG;
//            
//            //data preparing for UART serial Communication
//            OutArray[1] = (uint8_t)(OutAccX & 0xFF); 
//            OutArray[2] = (uint8_t)(OutAccX >> 8);
//            OutArray[3] = (uint8_t)(OutAccY & 0xFF);
//            OutArray[4] = (uint8_t)(OutAccY >> 8);
//            OutArray[5] = (uint8_t)(OutAccZ & 0xFF);
//            OutArray[6] = (uint8_t)(OutAccZ >> 8);
//            
//            UART_1_PutArray(OutArray, TRANSMIT_BUFFER_SIZE);
//        }
    }
}

/* [] END OF FILE */
