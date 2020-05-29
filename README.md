# RIOT OS 2020.04 - STPM3x
RIOT OS driver for STPM3x
Tested on RIOT 2020.04 with STPM33 only with current values reading on channels 1 & 2.
This driver supports only SPI communication.

## Requirements

* Communication between MCU and STPM3x through SPI
* RIOT OS 2020.04

## Installation
1. Apply `patchs/0001_stpm3x_add_driver.patch` to your RIOT OS 2020.04 copy or do the modifications manually
2. Move `drivers/saul/init_devs/auto_init_stpm3x.c` file to `RIOT/drivers/saul/init_devs/` folder
3. Move `stpm3x.h` file to `RIOT/drivers/include/` folder
4. Move subfolder `stpm3x` to `RIOT/drivers/` folder

## GPIO configuration

I had a lot of issue before having reliable SPI communication on my custom board. These issues came from RIOT OS and my custom test board:
* Try to slow the GPIOs slew rate
* Compare your PCB design with EVALSTPM33 schematics. If your design has no pull-up resistors on SPI bus, try to configure your SPI GPIO in push-pull with internal pull-up