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
#ifndef _RGB_LED_DRIVER_H_
    #define _RGB_LED_DRIVER_H_
    
    #include "project.h"
    
   /**
    *   \brief Start RGB Led Driver.
    */
    void RGBLed_Start(void);
    
    /**
    *   \brief Stop RGB Led Driver.
    */
    void RGBLed_Stop(void);
    
    /**
    *   \brief Write new color value.
    */
    void RGBLed_WriteColor(uint8_t red, uint8_t green, uint8_t blue);
    
    
#endif

/* [] END OF FILE */
