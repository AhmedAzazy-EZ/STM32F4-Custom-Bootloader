# STM32F4-Custom-Bootloader
This is a simple boot loader project . through it , you can flash a new image into flash , erase specific flash sectors.


###### In this project the Bootloader program will run only if we pressed the on board user button during 30 seconds from reset , then you can follow the bootloader Instructions and  do whatever you want with it .
###### the Bootloader is communicating with the host through USART1 . 


###### *Please Note that :*


**1- The Bootloader code runs from address 0x08000000**

**2- The user Code runs from sector2 at address (0x08008000) So you have to make two changes before compiling your application code**


        * i - editing the FLASH Origin address in the application linker Script to start from 0x08008000 *
        * ii - edit your startup file by mapping VTOR Register to the address 0x08008000 *
        
        

###### -After making the previous Steps you can send instructions to the bootloader using USB to UART Converter , then you can burn the produced binary file using any terminal software I used This one :https://www.narom.no/undervisningsressurser/the-cansat-book/the-primary-mission/using-the-radio/terminal-program/ .

