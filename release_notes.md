# Microchip MPLAB® Harmony 3 Graphics Release Notes
## Graphics Release v3.6.0

- **Graphics Contents** - This graphics release consists of graphics libraries, applications, utilities, drivers, hardware abstractions, input, middleware, templates, and documentation. The following table lists the contents:

**Legato graphics library** - the performance improved graphics library for MPLAB Harmony Graphics Suite. Supports PIC32 and SAM microcontrollers.

| Category | Item | Description | Release Type |
| --- | --- | ---- |---- |
|  apps | legato_quickstart | Legato UI library quickstart example | Beta |
|      |  legato_quickstart_ext_res | Legato UI library external resource example | Beta|
| drivers|  external_controller | User generate-able external display driver | Beta |
|      |   ili9488 | Display Driver for the ili9488 Controller |Beta |
|      |   LCC | Display Driver for the LCC software Controller| Beta |
|      |   ssd1963 | Display Driver for the ssd1963 Controller | Beta |
|      |   parallel_ebi | Interface to the parallel EBI registers | Beta |
|      |   parallel_portgroup | Inteface to the parallel portgroup registers | Beta |
|      |   parallel_smc | Interface to the parallel smc registers | Beta |
|      |   spi | Interface to the spi registers | Beta |
| library    | legato | Graphics Library | Beta |
| designer | Legato MHGC |Harmony Graphics Composer for Legato| Beta |

**Aria graphics library** - the feature-complete production library within MPLAB Harmony Graphics Suite. Supports PIC32 and SAM microcontroller and microprocessors.

| Category | Item | Description | Release Type |
| --- | --- | ---- |---- |
| apps |  aria_flash  | Aria UI library flash writer | Production |
|      |  aria_quickstart|Aria UI library quickstart example | Production|
|      |  aria_quickstart_ext_res    | Aria UI library external resource example | Production|
|drivers|  external_controller | User generate-able external display driver | Production |
|      |  glcd | Graphics 3 Layer Display Driver | Production |
|      |  ili9488| Display Driver for the ili9488 Controller |Production |
|      |  LCC | Display Driver for the LCC software Controller| Production |
|      |  LCDC | Display Driver for the LCDC Controller| Production |
|      |  ssd1306 | Display Driver for the ssd1306 Controller  | Production |
|      |  ssd1309 | Display Driver for the ssd1309 Controller  | Production |
|      |  ssd1963 | Display Driver for the ssd1963 Controller | Production |
|      |  parallel_ebi | Display interface using parallel EBI registers | Production |
|      | parallel_portgroup | Display interface using parallel portgroup registers | Production |
|      | parallel_smc | Display interface using parallel smc registers | Production |
|      | spi | Display interface using spi registers | Production |
|      | 2dgpu| Graphics Processor Driver for the 2DGPU peripheral |Production |
|      | gfx2d | Graphics Processor Driver for the GFX2D peripheral |Production |
| hal     | hal | Aria Hardware Abstraction Layer | Production |
| library    | aria | Graphics Library | Production |
| designer | Aria MHGC  | Harmony Graphics Composer for Aria| Production|

**Blank library interface** - the library interface which easily allows a third-party graphics library direct access to the display framebuffer.

| Category | Item | Description | Release Type |
| --- | --- | ---- |---- |
| apps | blank_quickstart | Blank UI-less library quickstart example | Beta |
| interface| gfx_driver | Interface abstraction between a graphics library and graphics drivers | Beta |



**Misc items** - items associated with the entire graphics suite.

| Category | Item | Description | Release Type |
| --- | --- | ---- |---- |
|input | generic | Generic Touch Input Driver | Production |
|      | maxtouch | Microchip maXTouch Touch Input Driver | Production |
| doc | Help | Application Help Documentation| Production |
| docs| Help | Application Help Documentation HTML| Production |
| templates   |  middleware | Board configuration scripts| Beta |
| display | boards | Productized display board support | Beta |

### Bug fixes and library updates for v3.6.0

- **Driver support** - The following table provides the list of updates for graphics and input drivers.

| Driver | Description |
| --- | --- |
| LCC | MH3-23405 Graphics LCC driver hard codes DMA channel to zero  |
| GLCD | MH3-24427 Default GLCD pixel clock divider produces distorted display |
| GLCD | MH3-28757 Add PIC32MZ DA GLCD legato support to H3    |
| GLCD | MH3-24689 GLCD: 8-bit LUT code is not being generated  |
| GFX2D | MH3-24677 Build warnings in GFX2D code when building SAM9x60 project using XC32  |
| all | MH3-29666 Add gfx_driver interface between library and drivers    |
| 2DGPU | MH3-28778 Add PIC32MZ DA GPU legato support to H3  |
| all | MH3-30018 Modify legato graphic controller drivers to support blank/3rd party graphics library        |

- **Application support** - The following table provides the list of updates for application configurations.

| Application |  Description |
| --- | --- |
|aria_quickstart external| MH3-22086 Added support for SAM 9x60 |
|aria_quickstart| MH3-28592 Added support for SAM 9x60 |
|legato_flash | MH3-18376 Added support for SAM E54 Curiosity |
|legato_quickstart | MH3-24641 Added support for SAM E54 Curiosity |
|legato_quickstart | MH3-29665 Added support for MZ DA MEBII |
|legato_quickstart | MH3-29790 Added support for SAM C21 |
|blank_quickstart | MH3-24623 Added support for SAM E54 Curiosity |
|blank_quickstart | MH3-24624 Added support for SAM E70 Curiosity |
|blank_quickstart | MH3-24625  Added support for MZ EF Curiosity 2.0 |


- **Middleware/HAL support** - The following table provides the list of updates for middleware and HAL content.

| Middleware/HAL | Description |
| --- | --- |
| aria | MH3-17681 GitHub Customer Report: Include 'app.h' in libaria_events does not take into account app name and multiple apps |
| aria | MH3-24440 aria keypad widget potentially crashes on language change    |
| legato | MH3-17908 GFX project fails to build if no fonts are added  |
| legato | MH3-18059 Keypad editor dialog does not change the active view when editing a cell value   |
| legato | MH3-21974 signal timing parameters of display manager should be uint32_t  |
| legato | MH3-22087 Graphics project fails to build if no fonts/strings are used   |
| legato | MH3-22205 Adding NotoMono_Regular.ttf font throws a Java error in MHGC   |
| legato | MH3-22285 Unaligned stringTable access causes exception in Arm M0 MCU (C21)    |
| legato | MH3-22305 string unique character count not working    |
| legato | MH3-22306 Widget Vector Tables take too much memory    |
| legato | MH3-23465 Cannot add widgets to new screens in Legato Composer    |
| legato | MH3-24294 Changing Pixel Clock in Display Driver Component does not update the Pixel Clock in Display Manager    |
| legato | MH3-24508 Graphics Composer always exporting screen even when generate is not checked    |
| legato | MH3-24635 Compile errors in Legato for IAR    |
| legato | MH3-24673 Line graph widget requires a scheme assigned to it to build properly, it requires a default scheme    |
| legato | MH3-28567 Add multi-line text support for external font assets  |
| legato | MH3-28769 Out of bounds contents are not cropped by a grandparent widget |
| legato | MH3-29174 Add option to switch rendering from width-first to heigh-first rectangle slices  |
| legato | MH3-29667 Add Input Driver component for PTC     |
| legato | MH3-29689 SAMA5D2: PLib: 2DGFX: Plib driver can be selected.     |
| legato | MH3-29770 Disabled languages still being generated      |
| legato | MH3-29778 Legato not saving some widget enable states    |
| legato | MH3-29779 SAM9x60: I2C peripheral and pins need to change for revB eval boards.     |
| legato | MH3-29831 GFX - Add multiline string support to legato java composer      |
| legato | MH3-29842 Screen On Hide and On Show 'Declare Only' Event Handler Generation is broken       |
| legato | MH3-29894 Deprecate GFX_DrawCircle()        |
| legato | MH3-30020 Compile Errors with Legato streaming interface        |

- **Display Board** - The following table provides the list of updates for graphics boards use within MHC.

|Board | Description |
| --- | --- |
| PDA TM7000B 800x480 | MH3-24075 Add display component for PDA TM7000B (7" WVGA display) |

- **Graphics Application Templates** - The following table provides the list of updates for graphics templates use within MHC.

|Library| Template | Description |
| --- | --- |---|
|legato|legato_gfx_pda_tm4301b|MH3-29663 Add MZ DA MEB-2 template for Legato  |

- **Board Support Packages (BSP)s** - The following table provides the list of updates for board support packages for use within MHC.

| BSP | Description |
| --- | --- |
| none | |

- **Development Interfaces** - The following table provides the list of updates for interface tools.

| Interface Tool | Description |
| --- | --- |
| Display Manager |MH3-29484 Add Display Manager support for Legato |

- **Development kit support** - The following table lists  applications available for different development kits.

| Applications | [SAM D21N Xplained Pro](https://www.microchip.com/DevelopmentTools/ProductDetails/ATSAMD21-XPRO) | [SAM C21N Xplained Pro](https://www.microchip.com/developmenttools/ProductDetails/PartNO/ATSAMC21-XPRO) | [SAM E54 Curiosity Ultra](https://www.microchip.com/Developmenttools/ProductDetails/DM320210)  | [SAM E70 Xplained Ultra](https://www.microchip.com/developmenttools/ProductDetails/PartNO/ATSAME70-XPLD) | [SAM A5D2 Xplained Ultra](https://www.microchip.com/developmenttools/ProductDetails/atsama5d2c-xult)  | [Multimedia Expansion Board II EF](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320005-5)  | [Multimedia Expansion Board II DA](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/DM320005-5) | [SAM9X60-EK Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/DT100126) | [Curiosity PIC32MZ EF 2.0](https://www.microchip.com/Developmenttools/ProductDetails/DM320209) |
| --- | --- |--- | --- | --- | --- | --- | --- |--- | --- |
| legato_quickstart_ext_res    |   |   | x |   |   |   |   |   |   |
| aria_flash                   |   |   |   | x |   | x | x |   | x |
| aria_quickstart              | x | x | x | x | x | x | x | x | x |
| aria_quickstart_ext_res      |   |   |   | x |   |   | x |   | x |
| blank_quickstart             |   |   | x | x |   |   | x |   | x |
| legato_quickstart            |   |   | x | x |   | x | x | x | x |
| legato_flash                 |   |   | x |   |   |   |   |   |   |
| legato_quickstart_ext_res    |   |   | x |   |   |   |   |   |   |

### KNOWN ISSUES

Current known issues are as follows:

* Code is compliant to MISRA C 2012 Mandatory guidelines, with the exception of Rule 9.1 (Code 530).
In gfx.c the variable args is falsely detected in violation of Code 530: &quot;Symbol not initialized&quot; at line 358.  In fact, va_start at line 358 is exactly where args is initialized.
* Interactive help using the Show User Manual Entry in the Right-click menu for configuration options provided by this module is not yet available from within the MPLAB Harmony Configurator (MHC).  Please see the *Configuring the Library* section in the help documentation in the doc folder for this Harmony 3 module instead.  Help is available in both CHM and PDF formats.
* Configurations using xplained pro display do not support touch events. For example, aria_quickstart_e70_xult_xpro_spi.
* The Heap Estimator can be inaccurate with estimating PNG images that have high pixel fidelity.
* Project regeneration from command line can remove Aria generated screens and widgets.
* MHGC GAC does not generated a 2DGPU comaptible palette table. A translation step is required to create word values from little endian byte array.
* When regenerating demo applications, keep all code between comments “//CUSTOM CODE…” and “//END OF CUSTOM CODE…”. Custom code is added to perform specific functionality.
* PKOB4 debugging requires MPLAB v5.20.
* Applications running on SAM E70 in combination with LCC will observe visual rendering artifacts on display during SD card R/W access. There is no loss in SD Card data.
* Applications formatting SQI FAT in Linux use the following command: mkfs.fat -F12 /dev/$(device name)
* aria_quickstart_ext_res applications using mzda may show external memory starter kit image. The demonstration only uses internal stack DDR starter kit.
* FreeRTOS OSAL has an incorrect include path that causes build failures in non-Windows environments.
* SAM E54/D51 must be debugged using MPLABX v5.25
* For a list of current issues that affect this release, refer to MPLAB Harmony [GFX Issues and Errata](https://github.com/Microchip-MPLAB-Harmony/gfx/wiki/Issues-and-Errata)

### DEVELOPMENT TOOLS

| Tool | Version |
| --- | --- |
| [MPLAB® X IDE](https://www.microchip.com/mplab/mplab-x-ide) | v5.30 |
| [MPLAB® XC32 C/C++ Compiler](https://www.microchip.com/mplab/compilers)      | v2.40 |
| MPLAB® X IDE plug-ins          |  |
| MPLAB® Harmony Configurator (MHC) plug-in   | v3.3.1 |
| [Harmony 3 BSP](https://github.com/Microchip-MPLAB-Harmony/bsp)   | v3.6.0 |
| [Harmony 3 CSP](https://github.com/Microchip-MPLAB-Harmony/csp)  | v3.6.0 |
| [Harmony 3 Core](https://github.com/Microchip-MPLAB-Harmony/core)  | v3.6.0 |
| [Harmony 3 Graphics ](https://github.com/Microchip-MPLAB-Harmony/gfx)   | v3.6.0 |
| [Harmony 3 Dev_Packs](https://github.com/Microchip-MPLAB-Harmony/dev_packs)   | v3.6.0 |
| [Harmony 3 USB](https://github.com/Microchip-MPLAB-Harmony/usb)   | v3.4.0 |
| [Harmony 3 CMSIS-FreeRTOS](https://github.com/ARM-software/CMSIS-FreeRTOS)   | v10.2.0 |
| [IAR Embedded WorkBench ARM](https://www.iar.com/iar-embedded-workbench/#!?architecture=Arm)   | v8.40.1 |
