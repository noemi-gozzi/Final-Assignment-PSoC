# FINAL ASSIGNMENT 
## Electronic Technologies and Sensors Laboratory
Students: 

**Francioli** Lorenzo 

**Gozzi** Noemi
### Requirements
  **Version:** <i> PSoC Creator 4.3 </i> 
  
  Board: PSoC 5LP (CY8C5888LTI-LP097) <a href="https://www.cypress.com/products/psoc-creator-integrated-design-environment-ide"> [<i>PSoC Creator</i>] </a>
  
  Additional Hardware: 
  1. LIS3DH breakout board <a href="https://cdn-learn.adafruit.com/assets/assets/000/085/846/original/lis3dh.pdf?1576396666"> [<i>LIS3DH datasheet</i>] </a> in FIFO modality 
  2. EEPROM 25LC256 <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/25AA256-25LC256-256K-SPI-Bus-Serial-EEPROM-20001822H.pdf"> [<i>25LC256 datasheet</i>] </a> 
  3. RGB common anode led
  4. 3 resistors 330/220 Ohm
  5. Potentiometer
  6. On Board button
  7. On board Blue LED
  8. Additional external RED LED
  
  Additional softwares tools: <a href=https://freeware.the-meiers.org/> [<i>Cool Term</i>] </a>  , Bridge Control Panel 1.20.0
                  
                              
## Hardware 
 ![alt text](https://github.com/noemi-gozzi/Final-Assignment-PSoC/blob/master/Images/HW_picture.jpg)

## System overview

In this project, we designed and developed a system that controls the blinking frequency of an RGB LED based on the data read from the LIS3DH (3-axis accelerometer). Each channel of the sensor controls one color of the LED.

The main objectives of this system are: 

 • acquire data from the accelerometer using its built-in FIFO to reduce the load of the CPU 

 • process the raw acceleration data to control the blinking frequency of the RGB LED 

 • provide the end user with a hardware menu to configure some settings related to the program 

## CONFIGURATION COMMANDS
***double click*** (interval time interclicks<500ms): START/STOP conditions

***long pressure click*** (duration time single click>1500ms) : CONFIGURATION mode. If the system is in configuration it is necessary to exit this modality before starting/stopping the device again. Another long pressure click will exit configuration mode, and the system will go back to the previous state (START or STOP)

In Configuration mode, the value of the potentiometer is used to set a verbose flag to send raw sensors data via UART to the Bridge Control Panel (default to 0, set to 1 for verbose data logging). If the value is higher than FSR/2, the verbose flag is set to 1, otherwise is set to 0.




