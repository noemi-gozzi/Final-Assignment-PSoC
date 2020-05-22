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

#pragma once

#include "SPI_Interface.h"

#define SLAVE_CS_Write CS_1_Write
#define SPIM           SPIM_1

 /*** ========= MACROS ========= ***/   

/** Instruction Set **/
//#define READ_BIT 1
//#define WRITE_BIT 0
#define SPI_LIS3DH_READ 0b10000000
#define SPI_LIS3DH_WRITE  0b00000000

#define address_mask6 0b00111111

#define SPI_LIS3DH_MS 0b01000000 //if 1 autoincrement 
#define CS_LIS3DH 1
uint8_t LIS3DH_readByte(uint8_t addr);

/*
* @brief Write a single byte with the value dataByte @ addr.
*
* @param[in]: 16-bit memory address to write to.
* @param[out]: 8-bit (unsigned byte) value to write in the memory location.
*/
void LIS3DH_writeByte(uint8_t addr, uint8_t dataByte);

/*
* @brief Read 1+ bytes from memory.
*
* @param[in]: 16-bit memory address to from read.
* @param[out]: uint8_t* pointer to the output data (cast operation required for different data type).
* @param[in]: Number of bytes to read.
*
*/
void LIS3DH_readPage(uint8_t addr, uint8_t* dataRX, uint8_t nBytes);
 
/*
* @brief Write 1+ bytes from memory.
*
* @param[in]: 16-bit memory address to write to.
* @param[in]: uint8_t* pointer to the input data (max 64 bytes).
*/
void LIS3DH_writePage(uint8_t addr, uint8_t* data, uint8_t nBytes);

/*
* @brief Wait until the Write Process is Complete
* 
* @note This is a blocking function!
*/
void LIS3DH_waitForWriteComplete(void);
/* [] END OF FILE */
