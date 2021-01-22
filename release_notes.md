---
title: Release notes
nav_order: 99
---

![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## Graphics Release v3.8.3
### Issues Fixed

* MH3-45463 Fixes wrong apps/readme.md file publish in v3.8.2

## Graphics Release v3.8.2
### Issues Fixed

* MH3-45434 Composer Font Import C-Friendly field
* MH3-45242 Customer Suggested Fixes for Legato Library
* MH3-45223 Fixes Composer Legato Jave Project Import Default Language
* MH3-45206	Legato Composer: Set Composer window to foreground when closed thru MPLABX/MHC
* MH3-45183	le_string callback variable using LE_MALLOC uninitialized
* MH3-45173	Event Manager Typo: On_Update typo as On_Hide
* MH3-45161	gfx2d and libnano compile errors with -wlong-long flag
* MH3-44905	Allow user to disable image decoders
* MH3-44890	Replace casting of lePixelBuffer to gfxPixelBuffer in gfx_driver
* MH3-44692	Legato Composer - can't pan or zoom designer screen if design dimension exceeds designer screen size

## Graphics Release v3.8.1
### Issues Fixed

* MH3-44910 Fixes issue with application page rendering with Jerkyll
* MH3-44911 Fixes issue with Composer failing to launch in Windows 10

## Graphics Release v3.8.0
### New Features

* MH3-44697 Remove Aria applications, middleware and tools from repo
* MH3-44700 Remove Aria documentation from repo
* MH3-44483 Add GFX2D setGlobalAlpha stub interface
* MH3-44301 Add asynchronous blit interface to 2D GPU
* MH3-31839 Put device filters in various modules such that only the ones supported are loaded
* MH3-44244 added some more coherency flags generation
* MH3-44162 updated leString_Delete code documentation
* MH3-38627 added screen events to screen manager window
* MH3-44290 fixes text field widget cursor placement issue
* MH3-43778 fixes image decoders when using GPU
* MH3-44463 fixes usage of leStream_Read
* MH3-44352 composer v1.4.0b added rectangle padding method
* MH3-44421 composer v1.3.1b
* MH3-38039 Add Legato Demo Mode support in Input Service
* MH3-44416 PIC32MZ DA: Add double buffer support
* MH3-44417 Update GFX Canvas to use new IOCTL driver interface
* MH3-44244 cache coherency update for legato
* MH3-44242 jpeg decoder initialization in streaming mode
* MH3-44245 legato double buffering support, gfx ioctl interface
* MH3-18081 legato multiple scratch buffer support
* MH3-44334 fixes compiler warnings for legato stack
* MH3-43587 Add vblank sync'd scratch buffer GPU blits to GLCD driver
* MH3-36222 Add config for SAME70 and PIC32 MZ EF LCC DMA scratch buffer blit
* MH3-43851 Fix num of commands and params for external controller when set to maximum

### Known Issues

* Applications running on SAM E70 in combination with LCC will observe visual rendering artifacts on display during SD card R/W access. There is no loss in SD Card data.
* legato_flash SAM E54 configuration support for USB is non-functional. SDCARD is functional.
* Legato Composer is not supported in this release for OSX operating systems.
* For applications on SAM E54 + CPRO with the 24-bit passthrough board, Pin 7 of the EXT1 connector should drive the backlight. However, on rev1.0 of the board, it is not connected to any pin on the MCU. As a workaround, it needs to be connected to a v3.3 pin.

For a list of post release issues that affect this release, refer to MPLAB Harmony [GFX Issues and Errata](https://github.com/Microchip-MPLAB-Harmony/gfx/wiki/Issues-and-Errata).

### Development Tools

* [MPLAB® X IDE v5.45 or above](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.50](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * MPLAB® Harmony Configurator (MHC) v3.6.0 and above.

### Dependent Components

* [BSP v3.8.1](https://github.com/Microchip-MPLAB-Harmony/bsp/releases/tag/v3.8.1)
* [Core v3.8.1 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.8.1)
* [dev_packs v3.8.0 ](https://github.com/Microchip-MPLAB-Harmony/dev_packs/releases/tag/v3.8.0)
* [Harmony 3 USB v3.6.0](https://github.com/Microchip-MPLAB-Harmony/usb/releases/tag/v3.6.0)
* [Harmony 3 CMSIS-FreeRTOS v10.3.1](https://github.com/ARM-software/CMSIS-FreeRTOS)
* [IAR Embedded WorkBench ARM v8.40.1](https://www.iar.com/iar-embedded-workbench/#!?architecture=Arm)




