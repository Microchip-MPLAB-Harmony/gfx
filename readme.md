---
title: Harmony 3 Graphics Package
nav_order: 1
---

# ![Microchip Technology](./images/mhgs.png) MPLAB® Harmony 3 Graphics Support Package

MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating
embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller
and microprocessor devices.  Refer to the following links for more information:
 - [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit)
 - [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus)
 - [Microchip MPLAB® X IDE](https://www.microchip.com/mplab/mplab-x-ide)
 - [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony)
 - [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/)
 - [MPLAB® Discover](https://mplab-discover.microchip.com/v1/itemtype/com.microchip.ide.project?s0=Legato)

This repository contains the MPLAB® Harmony Graphics Suite.  The
suite supports a free fast to market, graphics software development environment for Microchip MPLAB® 32-bit SAM and PIC® microprocessor devices.  Refer to
the following graphics links for release notes, home page, training materials, framework and application help.
Graphics application examples can be found at various [device-family specific repositories](https://microchip-mplab-harmony.github.io/gfx/apps/) and at [MPLAB® Discover](https://mplab-discover.microchip.com/v1/itemtype/com.microchip.ide.project?s0=Legato)
 - [Release Notes](https://microchip-mplab-harmony.github.io/gfx/release_notes.html)
 - [MPLAB® Harmony License](https://microchip-mplab-harmony.github.io/gfx/mplab_harmony_license.html)
 - [MPLAB® Harmony 3 Graphics User Guides Wiki](https://github.com/mchpgfx/legato.docs/wiki)
 - [MPLAB® Harmony 3 Graphics API Help](https://mchpgfx.github.io/legato.docs/html/index.html)
 - [MPLAB® Harmony 3 Graphics Applications](https://microchip-mplab-harmony.github.io/gfx/apps/readme.html)
 - [MPLAB® Harmony 3 Graphics Videos](https://www.youtube.com/channel/UCwGbrIuty-pHCyjuVRAyP5Q)

# Features

The key features of the MPLAB® Harmony Graphics Suite are the following:

- Hardware optimized for use with Microchip 32-bit SAM and PIC® devices
- Compatible component for use with MPLAB Code Configurator (MCC)
- **Microchip Graphics Composer with Legato** WYSWYG UI designer tool for Windows, Linux and Mac
- Written in C with MISRA C (Mandatory) compliancy
- RTOS (FreeRTOS, ThreadX) and non-RTOS support
- Configurable widget building blocks buttons, labels, lists, sliders, and images
- Rich tool set: Heap Estimator, Event Manager, Palette generator, Asset Manager, String and Font interface, DDR memory organizer
- Hardware integrated for Microchip GPU and display controller peripherals
- Support for single and double frame buffers
- Multi-language font support
- External input support for use with capacitive, resistive and other devices
- Fluid-UI support for alpha-blending, animations, scaling
- Pixel format support for RGBA8888, RGB888, RGB565, RGB5551, RGB332, GS8
- Multi-image support for industry standard formats, compression choices, external memory accessibility
- Fully functional demonstrations and quick-starts to enable new development
- Low memory and low power configurable
- Segmented LCD support
- LVDS-interface display support
- MIPI-DSI display support
- Image Sensor support


# Contents Summary

***
## Legato Graphics
***
The performance-improved graphics for MPLAB® Harmony Graphics Suite. Supports Microchip PIC32 and SAM microcontrollers and microprocessors

| Category | Item | Description | Release Status |
| --- | --- | ---- |---- |
| drivers|  glcd | Driver for the GLCD display controller peripheral | ![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |
|      |   2dgpu | Driver for the 2DGPU graphics processor |![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |
|      |   lcc |  Driver for the LCC software display controller| ![app-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
|      |   gfx2d | Driver for the GFX2D graphics processor| ![app-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
|      |   lcdc | Driver for the LCDC display controller peripheral | ![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |
|      |   external controller |  Driver generator for external display controllers | ![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |
|      |   ssd1963 | Driver for the ssd1963 display controller | ![app-beta](https://img.shields.io/badge/driver-deprecated-red?style=plastic) |
|      |   ili9488  | Display Driver for the ili9488 controller | ![app-beta](https://img.shields.io/badge/driver-deprecated-red?style=plastic) |
|      |   parallel_ebi | Interface to the parallel EBI registers | ![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |
|      |   parallel_portgroup | Inteface to the parallel portgroup registers | ![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |
|      |   parallel_smc | Interface to the parallel smc registers | ![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |
|      |   spi | Interface to the spi registers | ![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |
|      |   xlcdc | Driver for the XLCDC display controller peripheral | ![app-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
|      |   dsi | Driver for the MIPI-DSI display interface | ![app-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
|      |   lvdsc | Driver for the LVDS display interface | ![app-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) |
| library    | legato | Graphics library | ![app-beta](https://img.shields.io/badge/library-beta-orange?style=plastic) |
| designer | MGC Legato | Microchip Graphics Composer with Legato| ![app-beta](https://img.shields.io/badge/tool-beta-orange?style=plastic) |

**NOTE:** ssd1963 and the ili9488 drivers are deprecated. There functionality are replaced by external controller. All applications using these drivers must transition to external controller.

***
## Blank Library Interface
***
**Blank Library Interface** - the library interface which easily allows a third-party graphics library direct access to the display framebuffer.  Blank library applications are available at various [device-family specific repositories](https://microchip-mplab-harmony.github.io/gfx/apps/readme.html).
***
## Image Sensor Support
***
Image Sensor Controller Drivers for SAM microprocessors and Image Sensor Drivers

**NOTE:** Starting from Harmony 3 Graphics release v3.14.0 and over the next several releases, image sensor support will be gradually removed from the Graphics release and repositioned in releases of [Harmony 3 Vision Package](https://github.com/Microchip-MPLAB-Harmony/vision).

This chart shows which drivers are located in which repository.

| Category | Item | Description | Release Status | Repository 
| --- | --- | ---- |---- |---- |
| drivers|  isi | Driver for the Image Sensor Interface peripheral | ![app-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) | ![app-beta](https://img.shields.io/badge/repository-gfx-green?style=plastic) |
|      |   isc | Driver for the Image Sensor Controller peripheral | ![app-beta](https://img.shields.io/badge/drive-beta-orange?style=plastic) | ![app-beta](https://img.shields.io/badge/repository-vision-blue?style=plastic) |
|      |   cam | Driver for the OV5640 image sensor module |![app-beta](https://img.shields.io/badge/driver-beta-orange?style=plastic) | ![app-beta](https://img.shields.io/badge/repository-gfx-green?style=plastic) |
***
***
## Segmented LCD Support
***
Controller Drivers for Segemented LCDs for SAM microcontrollers

| Category | Item | Description | Release Status |
| --- | --- | ---- |---- |
| drivers|  slcc | Low-Cost Controllerless Driver for the Segmented LCD displays | ![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |
|      |   slcd | Driver for the SAM L22 Segmented LCD display controller peripheral | ![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |
|      |   slcdc | Driver for the PIC32CX MT Segmented LCD display controller peripheral | ![app-beta](https://img.shields.io/badge/driver-production-green?style=plastic) |

***
## Aria Graphics

***
The legacy graphics facilities for MPLAB® Harmony Graphics Suite. Supports Microchip PIC32 and SAM microcontrollers.

As of Graphics release v3.8.0, Aria Graphics has been deprecated and removed from this repository.  A snapshot of all previously published applications, tools, middleware and drivers related to Harmony 3 Aria Graphics are available at this [archive repository](https://github.com/Microchip-MPLAB-Harmony/gfx_aria). 

____

[![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/gfx/blob/master/mplab_harmony_license.md)
[![Latest release](https://img.shields.io/github/release/mchpgfx/legato.docs.svg)](https://github.com/Microchip-MPLAB-Harmony/gfx/tree/v3.14.0)
[![Latest release date](https://img.shields.io/github/release-date/mchpgfx/legato.docs.svg)](https://github.com/Microchip-MPLAB-Harmony/gfx/tree/v3.14.0)
[![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/gfx.svg)](https://github.com/Microchip-MPLAB-Harmony/gfx/graphs/commit-activity)
[![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/gfx.svg)]()

____

[![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology)
[![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology)
[![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/)
[![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech)

[![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/gfx.svg?style=social)]()
[![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/gfx.svg?style=social)]()

