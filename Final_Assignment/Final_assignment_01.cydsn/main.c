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

    
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
