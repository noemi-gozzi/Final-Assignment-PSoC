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
#ifndef __LIS3DH_
    #define __LIS3DH_
        
    #define LIS3DH_DEVICE_ADDRESS 0x18

    /**
    *   \brief Address ofthe WHO AM I register
    */
    #define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

    /**
    *   \brief Address of the Status register
    */
    #define LIS3DH_STATUS_REG 0x27

    /**
    *   \brief Address of the Control register 1
    */
    #define LIS3DH_CTRL_REG1 0x20

    /**
    *   \brief Hex value to set normal mode to the accelerator
    */
    #define LIS3DH_NORMAL_MODE_CTRL_REG1_100Hz 0x57

    /**
    *   \brief  Address of the Temperature Sensor Configuration register
    */
    #define LIS3DH_TEMP_CFG_REG 0x1F

    #define LIS3DH_TEMP_CFG_REG_ACTIVE 0xC0

    /**
    *   \brief Address of the Control register 4
    */
    #define LIS3DH_CTRL_REG4 0x23

    #define LIS3DH_CTRL_REG4_BDU_ACTIVE 0x80
    
    #define LIS3DH_CTRL_REG4_4G_NORMAL 0x10
    /**
    *   \brief Address of the ADC output LSB register
    */
    #define LIS3DH_OUT_ADC_3L 0x0C

    /**
    *   \brief Address of the ADC output MSB register
    */
    #define LIS3DH_OUT_ADC_3H 0x0D

    #define LIS3DH_OUT_X_L 0x28 //register for acceleration in X (LSB)

    /**
    *   Hex value to set the accelerator in high resolution mode acceleration and 100 Hz,
        correspondent to 0101 1000, enabling XYZ 
    */
    #define LIS3DH_HIGH_RES_CTRL_REG1_XYZ_100 0x57

    /*
    ctrl register 4 inhigh resolutio HR=1, +- 4g (bit FS[1:0] set to 01 --> FSR +-4g
    */
    #define LIS3DH_CTRL_REG4_HIGH_RES 0x18
    
    #define LIS3DH_CTRL_REG1_10HZ 0x27
    
    #define LIS3DH_CTRL_REG4_2G_NORMAL 0x00
    
    #define LIS3DH_CTRL_REG5 0x24
    
    #define LIS3DH_CTRL_REG6 0x25
    
    #define LIS3DH_CTRL_REG5_FIFO_EN 0x40
    
    #define LIS3DH_FIFO_CTRL_REG 0x2E
    
    #define LIS3DH_FIFO_CTRL_REG_FIFO_MODE_WTM_1 0x41
    
    #define LIS3DH_FIFO_CTRL_REG_FIFO_MODE_WTM_7 0x47
    
    #define LIS3DH_CTRL_REG3 0x22
    
    #define LIS3DH_CTRL_REG3_WTM_INT 0x04
    
    #define LIS3DH_CTRL_REG3_IA1_WTM_INT 0x44
    
    #define LIS3DH_FIFO_SRC_REG 0x2F
    
    #define LIS3DH_INT1_SRC 0x31
    
    #define LIS3DH_INT1_THS 0x32
    
    #define LIS3DH_INT1_DURATION 0x33
    
    #define LIS3DH_INT1_CFG 0x30
    
    #define LIS3DH_INT1_CFG_OR_XYZIHE 0x2A
    
    
#endif
/* [] END OF FILE */
