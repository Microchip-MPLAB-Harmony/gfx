# eink_controller 2nd Generation

This package provides an interface between the
Legato graphics suite and electronic paper displays (EPD).

EPD is peculiar in these ways:
1. black and white uses 1 bit per pixel, 8 pixels per byte;
some displays have red and yellow
provided by separate memory loads, 
2. very long update times, 
often 500 milliseconds to 3 seconds or more to refresh the screen
after a comparatively quick data transfer,
3. a driver-supplied look-up table (LUT) is required in most models,
4. commands and register settings are often
incompatible when moving from one display
to another; not just vendor to vendor, but
different models within a vendor, and
5. a source of high-voltage is required for operation, often +/-20VDC
(that is, 40V differential around Vss).

For the charge pump, the display provides the pump pulse
and regulates the loop to 
achieve the required voltages, but the components are too large
to be included in the display glass itself
and must be provided by the larger system.

The LUT data are generally provided by the display vendor without documentation.
Separate LUTs are used for NORMAL, PARTIAL and FAST update modes.
The LUT controls a sort-of state machine that conducts the update of the display,
setting both sequence order and duration of the required steps.

## Attention

With regards to E-ink technology the terms "EDP" or "e-paper" are preferred.
While often used colloquially to describe any electronic paper display, "E&nbsp;Ink" refers specifically to the technology developed by the E&nbsp;Ink Corporation.

* Trademark Status: "E&nbsp;Ink" is a registered trademark of E&nbsp;Ink Corporation, which was founded in 1997 to commercialize electronic paper technology. The company protects its trademarks, logos, and brand identities.
* "Eink" vs. "E-Ink": While often used generically, the proper trademarked name is usually rendered as "E&nbsp;Ink" (with a space) or "E&nbsp;Ink Corporation." The E&nbsp;Ink logotype is a registered trademark, and the name and design are protected by law.
* Usage Guidelines: E&nbsp;Ink Corporation specifies that "E&nbsp;Ink" should have an upper case 'E', an upper case 'I', and no hyphen.
* Proprietary Technology: The underlying microcapsule technology, often referred to as "e-paper," is a proprietary technology developed by the company.


## API

There are only 5 entry points, 
3 of which are dictated through a data structure of well-known name.
The structure is instantiated with the entry points
that are specific to the chosen controller.
Details are in the Legato documentation.

File: **drv_gfx_EPD_controller.h**
```
extern void DRV_einkClick_Reset(void);
extern int DRV_Eink_Initialize(void);
extern gfxResult DRV_Eink_BlitBuffer(int32_t x, int32_t y, gfxPixelBuffer* buf);
extern gfxDriverIOCTLResponse DRV_Eink_IOCTL(gfxDriverIOCTLRequest req, void* arg);
extern gfxDriverIOCTLResponse DRV_Eink_IOCTL(gfxDriverIOCTLRequest req, void* arg);
```

File: definitions specific to the display
```
const gfxDisplayDriver gfxDriverInterface =
{
    .update = DRV_Eink_Update,                       
    .blitBuffer = DRV_Eink_BlitBuffer,               
    .ioctl = DRV_Eink_IOCTL,                
};
```

## Customization

For each display type, separate filesets are provided that have
the custom display driver code.
The correct fileset should be selected by the build-time environment.
An effort is made for each fileset to have unique object names
for the occasion that multiple displays might be operated simultaneously.

There is also a display-specific .h file that provides 
top-level information like X and Y dimensions
and the necessary links to allow the top-most .h file to be generic.
The correct file is specified in ```drv_gfx_eink_customize.h```


### File layout
Discriminated by display controller
because that suggests the ability to re-use code.
```
.
|-- EPD_controller\
    |-- drv_gfx_EPD_controller.h
    |-- WaveShare\
    |     |
    |     |-- MIKROE_122x250\   ; 2.13"
    |     |-- MIKROE_296x128\   ; 2.9"
    |
    |-- UltraChip_UC8253\
    |     |
    |     |-- AMP_240x360\      ; 3.52" 
    |     |-- AMP_240x416\ 
```

## Controllers

Beware export restrictions.

See also Solomon Systech (SSD168x), Jadard Technology (JD7966x, Chinese), Fitipower Integrated Technology (EK79xxx); Waveshare displays might have any one of these. TODO: research this!

### [Waveshare](https://www.waveshare.com) (not really a controller; TBD)

Waveshare displays are very popular with hobbyists
and on-line sales, but Waveshare is not really a controller brand. 
There are least 3 different versions that have subtle differences,
so you have to know which is in service for your display.
Mikroe uses the oldest which does not have a version#.
Others (e.g., Adafruit) use either V3 or V4.

### [Ultrachip](http://www.ultrachip.com)

Harder to get documentation.
