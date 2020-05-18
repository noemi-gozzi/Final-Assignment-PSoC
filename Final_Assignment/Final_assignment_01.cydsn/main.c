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
    UART_1_Start();
    
    /* Start SPI Master */
    SPIM_1_Start();
    
    CyDelay(10);
    LIS3DH_writeByte(0x20, 0x37);
     // String to print out messages on the UART

    
    uint8_t data_read;
    data_read = LIS3DH_readByte(0x20);
    sprintf(bufferUART, "** LIS3DH CTRL REG 1 = 0x%02X\r\n", data_read);
    UART_1_PutBuffer;
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    /*          initialize CTRL REG 1 10 Hz, normale mode 0x27  
                LIS3DH_NORMAL_MODE_CTRL_REG1_10Hz 0x27                   */
    
    /* set CTRL REG 4 */
    
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
