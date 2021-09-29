---
title: Release notes
nav_order: 99
---

![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## Graphics Release v3.9.5

### New Features

* Displays and Template support for PIC32MZ DA Radial Development Board
* Duplicate Screen Button in Graphics Composer

### Issues Fixed

* MH3-59974 MZ DA GPU incorrect in blit-ing images with semi-transparent pixels
* MH3-50911	MZ DA GPU blit preprocessed images has artifacts with width not multiple of 4
* MH3-51057	Composer Project Wizard: Automatically enable scratch buffer padding when MZ DA profile is selected
* MH3-51060	MZDA image pre-processing with GPU blit does not work for non x4 wide images 
* MH3-51454	Add an option to byte-swap generated assets to support external controller with opposite endianness
* MH3-51464	Unable to set pre-process for multiple image assets
* MH3-51546	GFX: Legato: Buttons may not paint when orientation is set at 90 and 270
* MH3-51677	Add option to not store rasterized glyph data in Composer save data (glyphs.json)
* MH3-52808	Legato paints only to visible area of a layer that have areas clip from view
* MH3-55278	Legato Line Graph is not drawn if the axis is entirely below origin
* MH3-55403	button text not vertically centered
* MH3-59264	RGB565 GPU support for both PIC32MZ DA and SAM9x60 needs word aligned addressing
* MH3-59468	Latest Gfx release breaks existing 8-bit applications
* MH3-53723	Display Manager / GFX Core LE does not save resolution and timing values for custom display
* MH3-51215	Master clock value in GLCD component does not match out value from REFCLK5
* MH3-59484	GFX: PIC32MZDA - Clock Setting in LE GLCD Configuration Options window does not get updated

* Legato Composer Linux/PC Build v1.20.2b
* Legato Composer Mac Build v1.20.2b


## Graphics Release v3.9.4

### New Features

* MH3-53713	LCDC HEO Layer Support

### Issues Fixed

* MH3-51543	GFX Composer: Memory mapped locations ID are not set to 0
* MH3-51576	Can't load a Aria Composer Project using Legato Composer
* MH3-53057	Composer crashes when importing Aria project
* MH3-53111	touch down event missing rect-to-layer space translation
* MH3-54915	Legato Checkbox Widget Bevel Generates Exception, doesn't respond to Touch
* MH3-54996	Legato leWidget* root variables issue
* Fixed LCDC driver double buffer issue
 
* Legato Composer Linux/PC Build v1.16.6b
* Legato Composer Mac Build v1.16.6b


## Graphics Release v3.9.3

### New Features

* MH3-51808	Add Integrated Graphics Touch Driver support

### Issues Fixed

* MH3-45527	add touch rotation to generic touch controller
* MH3-51417	Evaluate GPU blit of pre-processed images
* MH3-51419	Evaluate how GFX2D handles image blits with non-opaque pixels differently with Base Layer vs OVR1 and OVR2
* MH3-51547	Enable Scratch Buffer padding if MZDA is selected in project wizard
* MH3-51669	Display polarity settings not changing
* MH3-51749	Support LCD backlight brightness control
* MH3-51796	duplicate 'const' declaration specifier in sys_input_listener.c
* MH3-51809	libnano compile error: 'memset' used with length equal to number of elements without multiplication by element size
* MH3-51845	LCDC Driver Layer DMA interrupt not firing
* MH3-52004	Add Layer IRQ Callback in LCDC driver
* MH3-52089	Double buffering feature appears broken for LCDC (SAM 9x60/A5D2)
 
* Legato Composer Linux/PC Build v1.14.4b
* Legato Composer Mac Build v1.14.4b


## Graphics Release v3.9.2

### New Features

* MH3-50862	Add Image Preprocessing UI and code generation support
* MH3-51432	Add 8-bit LUT support for LCC on Legato for SAM E70
* MH3-46945	H3 Segment LCD support for SAML22

### Issues Fixed

* MH3-46974	Reset sequence not proper for OLED C click
* MH3-46942	No test for invalid index parameter in list widget code
* MH3-50874	Legato Radio button widget doesn't work after destroying it
* MH3-50912	Scroll Bar Widget "swapDimensions" incorrect
* MH3-50961	Selecting more than one widget in the designer crashes Composer
* MH3-50971	Line Graph Widget Skin Classic Compile Error
* MH3-50972	Legato Widget InvalidateBorderAreas function has unused code
* MH3-51106	Legato GFX - The Radio button CallbackEvent function are not generated
* MH3-51130	Legato Composer - Image Manager Tree Drag Crash
* MH3-51402	GFX Legato "Uses Cache" option for LCC doesn't work if is unchecked for ATSAME70Q21B

* Legato Composer Linux/PC Build v1.14.4b
* Legato Composer Mac Build v1.14.4b


## Graphics Release v3.9.1

### IMPORTANT UPDATE

**The GIT log (history of all commits) has been rewritten, including the release version tags, in order to reduce the time required to publish new releases.**
**To update to the new version, the existing copy (clone) of this repository on your development system will need to be deleted and you will need to download (clone) the new repository version in its entirety. We are sorry for any inconvenience this causes.**

### New Features

* MH3-47394 Add 8-bit LUT support for LCC on Legato for MZ EF

### Issues Fixed

* MH3-47099 Legato GFX Composer crashes when clicking on a Bar Graph widget or Line Graph widget
* MH3-47054 Legato Composer: Parented widgets gets foreground in screen preview
* Fixed Textfield Widget use str instead of hintText for getting string charRect
* Fixed Circular Widget C99 compiler error
* Legato Composer Linux/PC Build v1.13.5b
* Legato Composer Mac Build v1.13.5b

## Graphics Release v3.9.0

### Issues Fixed

* MH3-45475 Update LCDC IOCTLS for Canvas Support
* MH3-45728 Windows Path/Java Path resolve issue
* MH3-45722 Add GPU interface for setting orientation
* MH3-45765 Fixes blending of alpha channels
* MH3-45432 Add Legato Composer Mac OS support
* MH3-45433 LNF for property buttons and line entries
* MH3-45614 button setstring nullptr
* MH3-24676 Composer: Allow multi-image select and change property using Image Asset Manager
* MH3-45619 Add Legato double-buffer & multi-layer support for 9X60
* MH3-45678 Missing leString_Free function
* MH3-45636 fixes hierarchy culling in pre-rendering stages
* MH3-46556 Legato Composer Radial Menu widget does not save the items number
* MH3-46491 Unable to change the root screen size
* MH3-46628 Add screen orientation feature for legato on sam9x60
* MH3-45721 Composer: Add hide/show option for layers
* MH3-46528 Legato Composer crashes when adding a new string (in some condition)
* MH3-45763 Legato Composer - RLE image data is in the wrong color mode
* MH3-45761 Legato - Asynchronous Screen State Machine
* MH3-45762 Legato - Harmony Delta Time support
* MH3-45541 Composer Image Manager does not support import image with chinese path or filename
* MH3-45459 Composer crashes when using arrow keys to move widgets
* MH3-45680 Legato - Circular Gauge Widget Live Update
* MH3-36848 add Batch image manipulation for MHGC
* MH3-45679 Legato - Group selection box arrow keys don't move all selected widgets
* MH3-45525 legato composer crash when dragging& moving widget in screen tree with mouse
* MH3-45530 legato composer crash when after adding data category for bar graph widget
* MH3-44633 Strings window under the Assets Tab in Legato Graphics Composer does not close
* MH3-44333 GFX - composer fails to load palettized PNGs
* MH3-18067 Legato Composer - Virtual sliders for properties
* Fix canvas objects limit check
* Fix IOCTL range check in GLCD driver
* Fixes CRLF broken CJK fonts
* Legato Composer Linux/PC Build v1.13.3
* Legato Composer Mac Build v1.13.3

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
* For applications on SAM E54 + CPRO with the 24-bit passthrough board, Pin 7 of the EXT1 connector should drive the backlight. However, on rev1.0 of the board, it is not connected to any pin on the MCU. As a workaround, it needs to be connected to a v3.3 pin.

For a list of post release issues that affect this release, refer to MPLAB Harmony [GFX Issues and Errata](https://github.com/Microchip-MPLAB-Harmony/gfx/wiki/Issues-and-Errata).

### Development Tools

* [MPLAB® X IDE v5.50 or above](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v2.50 or above](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Harmony Configurator (MHC) v3.8.1](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin).

### Dependent Components

* [Core v3.10.0 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.10.0)
* [Touch v3.10.1 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.10.1)
* [IAR Embedded WorkBench ARM v8.40.1](https://www.iar.com/iar-embedded-workbench/#!?architecture=Arm)




