/**
 * @file SPI_Interface_EEPROM.h
 * @brief Functions required to interface with the SPI MASTER (SPIM).
 * 
 * This header file contains macros and function prototypes to interface
 * with the SPI Master (SPIM) of the PSoC.
 *
 * CS_2 and SPIM_2 are referred to EEPROM 25LC256

*/


#pragma once

#include <cytypes.h>

#include "CS_2.h"
#include "SPIM_2.h"

#define SPI_DUMMY_BYTE  0x00

/* ======= PARAMETERS DEPENDING ON THE MASTER ======= */
#define SPI_TxBufferSize 8
#define SPI_RxBufferSize 8
/* ================================================= */

/*
* @brief Full-Duplex, Single-Operation 1-Byte Trade
*
* This Function writes 1 byte (TX) on the MOSI line while reading (RX)
* one byte from the slave on the MISO line (simultaneous TX/RX)
*
* @param[in]: uint8_t byte > 1-byte word to TX
*
* @return: uint8_t > received 1-byte word 
*/
uint8_t SPI_Interface_EEPROM_tradeByte(uint8_t byte);

/*
* @brief RX-only, Dual-Operation 1-Byte READ
*
* This Function requests 1 byte from the SPI Slave.
* First, it sends (TX) a 1-byte address/instruction to the Slave
* that replies on the next clock cycle.
* One byte from the slave is read on the MISO line 
* on the next clock cycle, while transmetting a 
* dummy byte (0x00)
*
* @param[in]: uint8_t byte > 1-byte address/instruction to TX
*
* @return: uint8_t > received 1-byte word 
*/
uint8_t SPI_Interface_EEPROM_ReadByte(uint8_t addr);

/*
* @brief Full-Duplex, Multi-Byte Trade (RX/TX)
*
* This Function writes *nBytes* bytes (TX) on the MOSI line while reading (RX)
* *nBytes* bytes from the slave on the MISO line (simultaneous TX/RX)
*
* @param[in]:  uint8_t* dataTX > Pointer to the input (TX) data array
* @param[out]: uint8_t* dataRX > Pointer to the output (RX) data array
* @param[in]:  uint8_t nBytes  > Number of bytes to TX/RX
*/
void SPI_Interface_EEPROM_Multi_Trade(uint8_t* dataTX, uint8_t* dataRX, uint8_t nBytes);

/*
* @brief RX-only, Dual-Operation Multi-Byte READ/WRITE
*
* This Function FIRST sends *bytesTX* bytes to the SPI Slave.
* Then, it reads *bytesRX* bytes from the slave while 
* transmitting dummy bytes (0x00).
* Read/write operations are not simultaneous: data may be requested
* to the slave with the TX operation and then read afterwards.
*
* @param[in]:  uint8_t* dataTX > Pointer to the input (TX) data array
* @param[in]:  uint8_t bytesTX > Number of bytes to transmit
* @param[out]: uint8_t* dataRX > Pointer to the output (RX) data array
* @param[in]:  uint8_t bytesRX  > Number of bytes to receive
*/
void SPI_Interface_EEPROM_Multi_RW(uint8_t* dataTX, uint8_t bytesTX, uint8_t* dataRX, uint8_t bytesRX);

/* [] END OF FILE */