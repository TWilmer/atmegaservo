/usr/local/gcc/4.9.1-avr/bin/avr-none-eabi-gcc -DF_CPU=12000000 -O3 -mmcu=atmega328   -o test.elf oddebug.c  usbdrvasm.S  usbdrv.c  main.c display.c

/usr/local/gcc/4.9.1-avr/bin/avr-none-eabi-gcc -fshort-enums -DF_CPU=12000000 -O3 -mmcu=atmega328   -o demo.elf   demo.c display.c

/usr/local/gcc/4.9.1-avr/bin/avr-none-eabi-objcopy  -j .text -j .data  -R .eeprom -O ihex  test.elf  a.hex
/usr/local/gcc/4.9.1-avr/bin/avr-none-eabi-objcopy  -j .text -j .data  -R .eeprom -O ihex  demo.elf  demo.hex

