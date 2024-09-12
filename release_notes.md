---
title: Release notes
nav_order: 99
---

![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![MGS Logo](https://mchpgfx.github.io/legato.docs/docs/legato/images/mgs.svg)

# Microchip Graphics Suite Harmony Release Notes

## Graphics Release v3.15.3

### Issues Fixed

* Composer: Fixes dependency errors with Linux binary

### MGS Harmony Composer

* MGS Harmony Composer v3.0.8 (Windows/Linux)

* For more information about how to user the MGS Harmony Graphics Composer, visit the [MGS Harmony Composer User Guide](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/mgs-harmony-guide/composer-ug/) at Microchip Develop Help.

  **NOTE 1:** MGS Harmony Composer UI/UX modernized for Windows and Linux builds only
  * MacOS Build version remains at:
     * Microchip Graphics Composer v1.4.1b
        * Legato Plugin v2.3.15b

  **NOTE 2:** MGS Harmony Composer users on Windows may have missing DLLs on launch: VCRUNTIME140.dll, VCRUNTIME140_1.dll and MSCVP140.dll.
  
  To remedy this, make sure to install the latest [Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-US/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version).


### MGS Simulator

* MGS Simulator v1.1.0

* For information on how to download and install the Simulator as a productivity tools to shorten development time and reduce the overall cost of your embedded GUI designs, visit the [MGS Simulator User Guide](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/mgs-harmony-guide/simulator/) at Microchip Develop Help.


### Development Tools

- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
    - MPLAB® Code Configurator (MCC) Plugin v5.5.1

### Dependent Components

* [Core v3.13.5 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.13.5)
* [Touch v3.14.0 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.14.0)


## Graphics Release v3.15.2

### Issues Fixed

* Composer: Pan/Zoom issues in Screen Designer   
* Composer: Updated splash screen logo to have trademark ®
* Composer: Fixed widget Crash
* Composer: Fixed crash where selecting a widget from the toolbox and then selecting it again in the Screen Tree, before placing it, would cause a crash
* Composer: String manager theme-color issues
* Composer: Docks in Composer look wrong during 1st initialization
* Composer: Various cosmetic fixes
* Simulator: Fixes layer parity check
* Simulator: Fixes an error to notify users about layer count inequality
* XLCDC Driver: Fixes Canvas integration issues
* XLCDC Driver: HEO Layer scaling issues
* XLCDC Driver: RGB565 color issues
* XLCDC Driver: YrBrCr display issues

### MGS Harmony Composer

* MGS Harmony Composer v3.0.8 (Windows/Linux)

* For more information about how to user the MGS Harmony Graphics Composer, visit the [MGS Harmony Composer User Guide](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/mgs-harmony-guide/composer-ug/) at Microchip Develop Help.

  **NOTE 1:** MGS Harmony Composer UI/UX modernized for Windows and Linux builds only
  * MacOS Build version remains at:
     * Microchip Graphics Composer v1.4.1b
        * Legato Plugin v2.3.15b

  **NOTE 2:** MGS Harmony Composer users on Windows may have missing DLLs on launch: VCRUNTIME140.dll, VCRUNTIME140_1.dll and MSCVP140.dll.
  
  To remedy this, make sure to install the latest [Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-US/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version).


### MGS Simulator

* MGS Simulator v1.1.0

* For information on how to download and install the Simulator as a productivity tools to shorten development time and reduce the overall cost of your embedded GUI designs, visit the [MGS Simulator User Guide](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/mgs-harmony-guide/simulator/) at Microchip Develop Help.


### Development Tools

- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
    - MPLAB® Code Configurator (MCC) Plugin v5.5.1

### Dependent Components

* [Core v3.13.5 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.13.5)
* [Touch v3.14.0 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.14.0)


## Graphics Release v3.15.1

### Issues Fixed

* Composer: Created a new layer at an offset and fixed size but the design view shows the layer at 0,0

### MGS Harmony Composer

* MGS Harmony Composer v3.0.1 (Windows/Linux)

* For more information about how to user the MGS Harmony Graphics Composer, visit the [MGS Harmony Composer User Guide](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/mgs-harmony-guide/composer-ug/) at Microchip Develop Help.

  **NOTE 1:** MGS Harmony Composer UI/UX modernized for Windows and Linux builds only
  * MacOS Build version remains at:
     * Microchip Graphics Composer v1.4.1b
        * Legato Plugin v2.3.15b

  **NOTE 2:** MGS Harmony Composer users on Windows may have missing DLLs on launch: VCRUNTIME140.dll, VCRUNTIME140_1.dll and MSCVP140.dll.
  
  To remedy this, make sure to install the latest [Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-US/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version).


### MGS Simulator

* MGS Simulator v1.0.0

* For information on how to download and use the Simulator as a productivity tools to shorten development time and reduce the overall cost of your embedded GUI designs, visit the [MGS Simulator User Guide](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/mgs-harmony-guide/simulator/) at Microchip Develop Help.


### Development Tools

- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
    - MPLAB® Code Configurator (MCC) Plugin v5.5.1

### Dependent Components

* [Core v3.13.4 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.13.4)
* [Touch v3.14.0 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.14.0)


## Graphics Release v3.15.0

### New Features

* MGS Harmony Graphics Composer
  * Updated with modernized user interface
  * Updated new project creation wizard
  * Added screen tree filter and search capability

* MGS Simulator
  * Added Native Mode to allow debugging of MGS Harmony GUI design on desktop using Visual Studio Code

* Added support of the HSMC peripheral to LCC Driver for SAMRHx Family
* Added PIC32CZ CA70 Curiosity driver and template support
* Added SAM A5D29 Curiosity driver and template support
* Added SAM A7D65 Curiosity driver and template support

### Issues Fixed

* LE_IMAGE_FORMAT_COUNT has a wrong definition into legato_image.h
* MIPI Raspberry Pi 7" Does Not Work with FreeRTOS
* Jenkins build warning treated as error for XLCDC driver
* Composer: Autosave Backups every N minutes
* Composer: Stringtable isn't cleared when starting a new design.
* Composer: New project doesn't set screen visible
* Composer: Screen Tabs can't be closed
* Composer Crashes when configuring keypad widget
* EGT compilation problems & Crash
* Composer with a project loaded crashes when requesting a create a new project
* Composer loads project with Global Palette support in gray scale
* Composer missing icons
* Image Widget Alpha Blending malfunctioning
* Composer: Update Graphics Wiki link in About Screen
* Composer: When open after minimized, the Composer always reopens to the main monitor
* Composer: Disabled properties don't look disabled
* GLCD defines not being generated when Canvas mode is enabled
* Alpha blended fills with GPU doesn't work on MZDA
* Composer: Qt Rendering is constantly repainting design

### MGS Harmony Composer

* MGS Harmony Composer v3.0.0 (Windows/Linux)

* For more information about how to user the MGS Harmony Graphics Composer, visit the [MGS Harmony Composer User Guide](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/mgs-harmony-guide/composer-ug/) at Microchip Develop Help.

  **NOTE:** MGS Harmony Composer UI/UX modernized for Windows and Linux builds only
  * MacOS Build version remains at:
     * Microchip Graphics Composer v1.4.1b
        * Legato Plugin v2.3.15b

### MGS Simulator

* MGS Simulator v1.0.0

* For information on how to download and use the Simulator as a productivity tools to shorten development time and reduce the overall cost of your embedded GUI designs, visit the [MGS Simulator User Guide](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/mgs-harmony-guide/simulator/) at Microchip Develop Help.


### Development Tools

- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
    - MPLAB® Code Configurator (MCC) Plugin v5.5.1

### Dependent Components

* [Core v3.13.4 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.13.4)
* [Touch v3.14.0 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.14.0)


## Graphics Release v3.14.1

### Issues Fixed

* Fixed incorrect clipping of label widget damage area
* GLCD: Make sure layer settings are configurable
* Graphics Canvas: fixed segfault using Simulator on x64 platforms
* Fixes MCC lock-up during shutdown
* Fixes new MGS Harmony project generation with MPLABX 6.20+

### MGS Harmony Composer

* Microchip Graphics Composer v1.4.1b
  * Legato Plugin Linux/Windows/Mac Build v2.3.15b

### MGS Simulator Beta Release

* Updated Harmony components to support the Native mode for **MGS Simulator**
	* For information on how to download and use the Simulator as a productivity tools to shorten development time and reduce the overall cost of your embedded GUI designs, visit the [MGS Simulator User Guide](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/mgs/mgs-harmony-guide/simulator/) at Microchip Develop Help.

### Development Tools

- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
    - MPLAB® Code Configurator (MCC) Plugin v5.4.1

### Dependent Components

* [Core v3.13.3 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.13.3)
* [Touch v3.14.0 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.14.0)


## Graphics Release v3.14.0

### New Features

* Added Cut, Copy, & Paste to Microchip Graphics Composer
	* There are hotkeys for each, as well as new buttons under the Edit menu
* Added Undo & Redo to Microchip Graphics Composer
	* There are hotkeys for both, as well as new buttons under the Edit menu
* Added PIC32CX SG41 Family SPI display driver and template support
* Added 4-inch 720x720 Waveshare MIPI round display driver support 
* Added RGB332 color mode support for LCC display driver

* Added Harmony components to support the **Microchip Graphics Suite Simulator**
	* **Note:** The MGS Simulator is in closed-beta testing until release v3.15.0. Please contact your local Microchip Sales Office or submit a question with [Microchip Client Support and Services](https://www.microchip.com/en-us/support/design-help/client-support-services) if you wish to particapte in the closed-beta testing.


### Issues Fixed

* Fixed a bug where editing Keypad Widget cells would cause a crash.
* Keypad Widgets are now limited to a maximum of 100x100 cells.
* Fixed a bug where duplicating a screen would not add it to the Screens window.
* Improved the feel of how Widgets are selected in the Design Window.
* Added a User Preference to make the design auto-save when clicking the Generate Code button.
* StringTable bug fixes:
	* Fixed a bug where importing a StringTable ‘.csv’ file would make the strings in the design disappear.
	* Fixed various issues with double-quotes and non-ASCII characters.
	* Fixed a crash that would happen after importing and then attempting to export a StringTable.
* Fixed a bug where pressing Enter after editing an integer property would close the Property Window, rather than commit the value.
* Updating the Display Size now updates the Design Window checkerboard pattern.
* Pressing the Up/Down arrows in the Screen Tree no longer deselects the selection each time.
* Input Orientation is now deprecated. Instead, use MCC’s Input System Service to configure touch-input orientation.
* When selecting multiple images in the Image Manager, the Data Location property is now available for the selected images.
* Composer will now ensure that there is exactly one assigned Startup Screen at any time.
* Fixed a bug where deleting a selection of multiple Widgets would cause a crash.
* The Slider Widget’s ‘Value’ property now uses the same units as its Min & Max properties, rather than a percentage.
* The “Don’t show this again” checkbox on the Startup Window will now honor the user’s choice.
* Fixed a code-generation error when using the PNG Scratch Address feature.
* “Ctrl + S” is now a hotkey to Save the design.

* Microchip Graphics Composer v1.4.1b
  * Legato Plugin Linux/Windows/Mac Build v2.3.15b

### Development Tools


- [MPLAB® X IDE v6.20](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.35](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
    - MPLAB® Code Configurator (MCC) Plugin v5.4.1


### Dependent Components

* [Core v3.13.3 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.13.3)
* [Touch v3.14.0 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.14.0)



## Graphics Release v3.13.0

### New Features

* Added SAM9x7 MIPI-DSI display interface driver support
* Added SAM9x7 LVDSC display interface driver support
* Added SAM9x7 GFX2D graphics processor support
* Added SAM9x7 Evaluator Board + Raspberry Pi 7" 800x480 Touch Display support
* Added SAM9x7 Evaluator Board + AC69T88A LVDS 800x480 Touch Display support
* Added SAM9x7 Evaluator Board + Waveshare 7.9" 400x1280 Touch Display support
* Added SAM9x7 Evaluator Board + Waveshare 2.8" 480x640 Touch Display support
* Added PIC32CK SG GC Family LCC driver and template support

### Issues Fixed

* MH3-81984	UsageFault_Handler occurs while calculating the baudValue due to divide by zero error
* MH3-53726	MZ DA Curiosity apps documentation talks about 24-bit pass through, but does not talk about jumper position
* MH3-69618	LCDDISP/DISP signal from PIC32MZ development boards must be driven high
* MH3-73607	Add LCC support for PIC32CK SG Unicorn Curiosity Board
* MH3-73608	Add Graphics Template support for PIC32CK SG Unicorn Curiosity Board
* MH3-74247	SLCDC driver has MISRA violations
* MH3-74730	Add LVDS support for SAM 9x7 Early Adopter board
* MH3-74765	Need to revert to SW blits if source buffer address is not 16-byte aligned for PIC32MZ DA GPU
* MH3-74767	Fix build warnings/errors while building legato on emcc/clang
* MH3-75441	GFX: Align all asset data
* MH3-75665	Glyph table access is causing unaligned memory access
* MH3-75738	Percentage math fixes for slider widget
* MH3-80697	Legato library - Memory leakage when using LegatoBarGraph widget
* MH3-81738	gfx canvas effects crash if no callback is defined
* MH3-81739	SPI 4 Wire Driver dereferencing a -1 Pointer


* Microchip Graphics Composer v1.4.1b
  * Legato Plugin Linux/Windows/Mac Build v2.3.15b

### Development Tools


* [MPLAB® X IDE v6.10](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.30](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Code Configurator Plug-In v5.3.7](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)
    * [MPLAB® Code Configurator Core v5.5.7](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)

### Dependent Components

* [Core v3.13.0 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.13.0)
* [Touch v3.13.1 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.13.1)



## Graphics Release v3.12.2

### Issues Fixed

* MH3-73806 LCDC driver not using LCDC_SetPixelClockPolarity PLIB call to set Pixel Clock Signal Polarity
* MH3-73811 The MCC plugin for MGC is still called Legato Graphics Composer as shown in the screenshot
* MH3-74027 Add callback mechanism for SB blit status or completion
* MH3-74169	Composer crash when adding new screen and adding widgets

* Microchip Graphics Composer v1.5.0b
  * Legato Plugin Linux/Windows/Mac Build v2.3.15b


### Development Tools


* [MPLAB® X IDE v6.00](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.20](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Code Configurator (MCC) v5.1.17](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)

	OR
    * [MPLAB® Harmony Configurator (MHC) v3.8.3](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)


### Dependent Components


* [Core v3.11.1 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.11.1)
* [Touch v3.13.1 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.13.1)

## Graphics Release v3.12.1

### Issues Fixed

* MH3-73631 Composer crashes when deleting a layer
* MH3-73702	Composer Crash when opening Image Manager Import Color Map

* Microchip Graphics Composer v1.5.0b
  * Legato Plugin Linux/Windows/Mac Build v2.3.14b

### Development Tools


* [MPLAB® X IDE v6.00](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.20](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Code Configurator (MCC) v5.1.17](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)

	OR
    * [MPLAB® Harmony Configurator (MHC) v3.8.3](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)


### Dependent Components


* [Core v3.11.1 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.11.1)
* [Touch v3.13.1 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.13.1)


## Graphics Release v3.12.0

### New Features

* Added Right-To-Left Font support for languages such as Arabic, Hebrew, Cyrillic
* Added PIC32CX MT Family Segmented SLCDC driver support
* Added SAM9x60 Curiosity Board template support
* Added SAMA5D27-SOM1-EK1 Board template support
* Added ATSAMA5D27-WLSOM1 Board template support
* Added PIC32CZ CA Family driver and template support
* MH3-71833 Add Scheme Accelerator support for list wheel widget

### Issues Fixed

* MH3-68743 GFX: Add conditional check for blending based on color mode
* MH3-70582 bug in the drv_isc.c
* MH3-70594 Button is not drawn properly on click
* MH3-70843 List widget setSize is not forcing recalculation of the outer window height
* DSCAPP-6321 PIC32MZ DA - Color LookUp Table (CLUT) -> RGB565
* MH3-71537 Rounded Rectangle Fix from Github
* MH3-71549 GFX: Mismatched widget label
* MH3-71550 GFX: Widget border doesn't get updated when property change
* MH3-71551 GFX: leGPU_BlitBuffer passes different destination rectangle for GPU interface blitBuffer
* MH3-71628 Fixes to legato_widget_line_graph by apapillon on GitHub
* MH3-71645 maXTouch driver does not support I2C NACK error handling
* MH3-71655 GFX: Add API to change z-order of widget
* MH3-71698 Radial Menu Widget file not being handled in python script
* MH3-71742 Fixed heap memory leak during screen change
* MH3-71793 Legato Composer - New project wizard is failing
* MH3-71948 GFX: Composer Crashes when configuring Keypad Widget
* MH3-71965 Pre-release SLCDC issues
* MH3-72140 GFX Composer: Monochrome color mode is not supported in screen preview
* MH3-72295 Generate multiple asset files and font files generates one single file for each only
* MH3-72316 GFX: Composer Aria Project Import File filter does not work
* MH3-72327 Composer crashes when importing a bad rgb as a palette


* Microchip Graphics Composer v1.4.1b
  * Legato Plugin Linux/Windows/Mac Build v2.3.12b


### Development Tools


* [MPLAB® X IDE v6.00](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.20](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Code Configurator (MCC) v5.1.17](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)

	OR
    * [MPLAB® Harmony Configurator (MHC) v3.8.3](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)


### Dependent Components


* [Core v3.11.1 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.11.1)
* [Touch v3.13.1 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.13.1)

## Graphics Release v3.11.1

### Issues Fixed


* MH3-70249	Composer does not correctly show Global Palette enabled design
* MH3-70248	leGetRenderState() is deprecated without replacement
* MH3-63712	Legato PNG decoder doesn't work properly part 2
* MH3-70533 Rendering issue with 270 degree code
* MH3-70535 Graphics artifacts when using layers with transparent background (RGBA)
* MH3-70860 Legato Composer - Screens not saving event enable states


* Microchip Graphics Composer v1.3.0a
  * Legato Plugin Linux/Windows/Mac Build v2.3.1b


## Graphics Release v3.11.0

### New Features

* Replaced Legato Graphics Composer with **Microchip Graphics Composer with Legato**
* SAMA5D2: Added support for Image Sensor Controller (ISC)
* Added SLCC: Low-Cost Controllerless driver for Segmented LCD
* Added support for OV5640 image sensor
* Added support for Azure RTOS ThreadX

### Issues Fixed


* MH3-44161 Legato button toggle-able behavior
* MH3-45241 Better user experience with adding layers in Legato Composer
* MH3-45460 Legato Composer - remember last location for importing assets
* MH3-55204 png & RGBA images display incorrectly in legato
* MH3-61178 Embed videos to Gfx canvas documentation
* MH3-62199 Legato Composer - Alpha blending doesn't work
* MH3-62217 very hard to check/uncheck the checkbox
* MH3-63790 Legato - LineGraph code generated doesn't build
* MH3-68565 RGDB BSP LED switches defaults to analog, needs to be output/input
* MH3-68613 Harmony 3 - Legato: toChar pointer method of leTableString doesn't have any method assigned
* MH3-68614 Harmony 3 - Legato: leSetLanguageChangedCallback() is not present
* MH3-69308 5" Display MHC Component Timing is incorrect
* MH3-69618 LCDDISP/DISP signal from PIC32MZ development boards must be driven high
* MH3-69638 Point Rotation Is Incorrect in 270 mode
* MH3-69769 Bug in the keypad widget in harmony graphics
* MH3-69853 Direct Blit causes exception
* MH3-69854 Unable to delete multiple selected images in Composer
* MH3-69925 Add support for Global alpha for LCDC driver


* Microchip Graphics Composer v1.3.0a
  * Legato Plugin Linux/Windows/Mac Build v2.2.4b


## Graphics Release v3.10.0

### New Features

* SAM9X60: Added support for Image Sensor Interface (ISI)
* Anti-alias Font Fast-Rendering System

### Issues Fixed

* MH3-44161	Legato button toggle-able behavior
* MH3-44200	Legato generic controller driver interrupt mode is needed
* MH3-44862	PNG images are not rendered correctly
* MH3-45241	Better user experience with adding layers in Legato Composer
* MH3-45248	wrong register in LCDC_SetHorizontalBackPorchWidth
* MH3-45460	Legato Composer - remember last location for importing assets
* MH3-45526	MHC generate wrong code for HVsync polarity in legato display manager
* MH3-51678	Optimized legato_color_blend.c
* MH3-53112	LCDC peripheral missing APIs for DBLO and BLEN bits for the Layer Config
* MH3-53113	LCDC Driver: Layer System Bus Burst Support
* MH3-54591	GFX2D implementation has a bug causing the images to be displayed very glitchy
* MH3-59698	old legato project build error with GFX 3.9.4 and text display incorrectly in legato
* MH3-60482	legato displays garbage data during system initialization in PIC32MZDA with canvas mode
* MH3-61252	Missing declaration in drv_gfx_ssd1963.c
* MH3-61255	maXTouch Driver lacking runtime re-initialization capability
* MH3-61362	Legato GFX Composer crashes in certain situations
* MH3-62205	In “legato_tablestring.h” the keyword "this" causes compiler error
* MH3-62206	le_gen_harmony.h is missing C++ guard for C++ compilation
* MH3-62776	Widget Drag and Drop not working on Mac
* MH3-62918	SAM 9x60 LCDC Driver Swap Interrupt not aligned with V-sync
* MH3-63144	clean cache before doing a DMA transfer instead of assigning frame buffer in no cache region
* MH3-63194	Legato - The text on the button widget won't update when the user use setString


* Legato Composer Linux/PC Build v1.25.2b
* Legato Composer Mac Build v1.25.2b


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
* MISRA 2012 Rule 9.1 Compliance Deviation at legato_error.c line 74

For a list of post release issues that affect this release, refer to MPLAB Harmony [GFX Issues and Errata](https://github.com/mchpgfx/legato.docs/wiki/Issues-and-Errata).

### Development Tools


* [MPLAB® X IDE v6.00](https://www.microchip.com/mplab/mplab-x-ide)
* [MPLAB® XC32 C/C++ Compiler v4.20](https://www.microchip.com/mplab/compilers)
* MPLAB® X IDE plug-ins:
    * [MPLAB® Code Configurator (MCC) v5.1.17](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)

	OR
    * [MPLAB® Harmony Configurator (MHC) v3.8.3](https://github.com/Microchip-MPLAB-Harmony/mplabx-plugin)


### Dependent Components


* [Core v3.11.1 ](https://github.com/Microchip-MPLAB-Harmony/core/releases/tag/v3.11.1)
* [Touch v3.13.1 ](https://github.com/Microchip-MPLAB-Harmony/touch/releases/tag/v3.13.1)




