# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\noemi\Desktop\university\university\poli\lab sensors\final assignment\Final-Assignment-PSoC\Final_Assignment\Final_assignment_01.cydsn\Final_assignment_01.cyprj
# Date: Wed, 20 May 2020 12:02:53 GMT
#set_units -time ns
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {SPIM_1_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 13 25} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {UART_1_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 53 105} -nominal_period 2166.6666666666665 [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {PWM_Clock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 1201 2401} [list [get_pins {ClockBlock/dclk_glb_2}]]


# Component constraints for C:\Users\noemi\Desktop\university\university\poli\lab sensors\final assignment\Final-Assignment-PSoC\Final_Assignment\Final_assignment_01.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\noemi\Desktop\university\university\poli\lab sensors\final assignment\Final-Assignment-PSoC\Final_Assignment\Final_assignment_01.cydsn\Final_assignment_01.cyprj
# Date: Wed, 20 May 2020 12:02:24 GMT
