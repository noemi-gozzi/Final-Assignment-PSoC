/**
 * @file LIS3DH_SPI.h
 * @brief Functions required to interface with the Accelerometer LIS3DH SPI.
 * 
 * This header file contains macros and function prototypes to interface
 * with the Accelerometer LIS3DH.
 *
*/

#pragma once

#include "SPI_Interface.h"

 /*** ========= MACROS ========= ***/   

/** Instruction Set **/
#define SPI_LIS3DH_READ 0b10000000 //MSB = 1 means READ
#define SPI_LIS3DH_WRITE  0b00000000 //MSB = 0 means WRITE

#define address_mask6 0b00111111 //The adress taken by the SPI is a 6-bit address

#define SPI_LIS3DH_MS 0b01000000 //if 1 autoincrement 

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
