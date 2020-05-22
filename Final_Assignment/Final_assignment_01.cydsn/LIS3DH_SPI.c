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
#include "LIS3DH_SPI.h"
#include "LIS3DH.h"


/** ====== User-level Functions ====== **/

uint8_t LIS3DH_readByte(uint8_t addr) {

	/* Prepare the TX data packet: instruction + address */

    uint8_t packet[1]={(SPI_LIS3DH_READ)|(addr & address_mask6)};
    
	
	
	/* Prepare the RX byte */
	uint8_t dataRX=0;
   
	
	/* Read 1 byte from addr */
    
	SPI_Interface_Multi_RW(packet, 1, &dataRX, 1);
	
	return dataRX;
    
}

void LIS3DH_writeByte(uint8_t addr, uint8_t dataByte) {
    
    /* Enable WRITE operations */
//    EEPROM_writeEnable();

	/* Prepare the TX packet */
    uint8_t dataTX[2] = {((SPI_LIS3DH_WRITE)|(addr & address_mask6)), dataByte};
	/* Nothing to RX... */
	uint8_t temp = 0;
	/* Write 1 byte to addr */
	SPI_Interface_Multi_RW(dataTX, 2, &temp, 0);
	    
}


void LIS3DH_readPage(uint8_t addr, uint8_t* dataRX, uint8_t nBytes) {
    
	/* Prepare the TX data packet: instruction + address */
    uint8_t dataTX[1]={(SPI_LIS3DH_READ)|(SPI_LIS3DH_MS)|(addr & address_mask6)};
    
	
	
	/* Read the nBytes */
	SPI_Interface_Multi_RW(dataTX, 1, dataRX, nBytes);
		
}

void LIS3DH_writePage(uint8_t addr, uint8_t* data, uint8_t nBytes) {
	    
    CyDelayUs(1);
    
	/* Prepare the TX packet of size nBytes+3 
       [ Write Instruction - Address MSB - Address LSB - +++data+++ ]
    */
	uint8_t dataTX[1+nBytes];
    dataTX[0] = (SPI_LIS3DH_WRITE)|(SPI_LIS3DH_MS)|(addr & address_mask6);

    /* Copy the input data in the memory */
	memcpy(&dataTX[1], data, nBytes);
	
	/* Nothing to RX: point to a dummy variable */
	uint8_t temp = 0;
	
	SPI_Interface_Multi_RW(dataTX, 1+nBytes, &temp, 0);
	
}
//
//void EEPROM_waitForWriteComplete() {
//    
//    while( EEPROM_readStatus() & SPI_EEPROM_WRITE_IN_PROGRESS );
//    
//}

/* [] END OF FILE */
