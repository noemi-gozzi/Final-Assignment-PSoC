/**
*   \brief Source code for RGB Led Driver.
*/

#include "RGBLedDriver.h"

static void RGBLed_WriteRed(uint8_t red);       // Write new red value
static void RGBLed_WriteGreen(uint8_t green);   // Write new green value
static void RGBLed_WriteBlu(uint8_t blu);       // Write new blu value

void RGBLed_Start(void)
{
    // Start PWM Components
    PWM_RG_Start(); // Start PWM connected to red and green channels
    PWM_B_Start();  // Start PWM connected to blu channel
}

void RGBLed_Stop(void)
{

    // Stop PWM Components
    PWM_RG_Stop(); // Stop PWM connected to red and green channels
    PWM_B_Stop();  // Stop PWM connected to blu channel
   
}

void RGBLed_WriteColor(uint8_t red, uint8_t green, uint8_t blue)
{
    RGBLed_WriteRed(red);
    RGBLed_WriteGreen(green);
    RGBLed_WriteBlu(blue);
}

static void RGBLed_WriteRed(uint8_t red)
{
    PWM_RG_WriteCompare1(red);
}

static void RGBLed_WriteGreen(uint8_t green)
{
    PWM_RG_WriteCompare2(green);
}

static void RGBLed_WriteBlu(uint8_t blu)
{
    PWM_B_WriteCompare(blu);
}
/* [] END OF FILE */
