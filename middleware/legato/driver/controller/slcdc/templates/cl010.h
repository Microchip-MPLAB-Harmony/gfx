/**
 * \file
 *
 * \brief Management of CL010 LCD Glass component.
 *
 * Copyright (c) 2020 Microchip Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Microchip may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Microchip microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY MICROCHIP "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL MICROCHIP BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Microchip Support</a>
 */

#ifndef CL010_H_INCLUDED
#define CL010_H_INCLUDED

#include "cl010_segmap.h"
#include "drv_gfx_slcdc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup cl010_display_group Microchip CL010-1158-132Y-01 LCD Glass component
 *
 * This is a driver for Microchip CL010-1158-132Y-01 LCD component.
 * This component is the custom LCD used for SAM4C-EK and SAM4L-EK.
 * This driver provides functions for initialization and control of the
 * LCD segments.
 *
 * \section dependencies Dependencies
 * This driver depends on the following modules:
 * - \ref group_sam_drivers_lcdca for Liquid Crystal Display (LCD) module.
 * - \ref group_sam_drivers_slcdc for Liquid Crystal Display (LCD) module.
 * @{
 */

/**
 * \name Circular Animation Shift Direction
 * @{
 */
/** Right Direction. */
#define CL010_CSR_RIGHT         1
/** Left Direction. */
#define CL010_CSR_LEFT          0
/** @} */

/**
 * \name Display Lines
 * @{
 */
enum cl010_line {
	/** Up Line (Symbols from 1 to 8) */
	CL010_LINE_UP = 0,
	/** Down Line (Symbols from 9 to 15 */
	CL010_LINE_DOWN,
};
/** @} */

/**
 * \name Value for Communication Icon setting
 * @{
 */
enum cl010_communication_value {
	CL010_COMMUNICATION_NONE = 0,
	CL010_COMMUNICATION_ONE,
	CL010_COMMUNICATION_TWO,
	CL010_COMMUNICATION_THREE,
};
/** @} */

/**
 * \name Display Units
 * @{
 */
/* Wh, kWh, MWh, VAh, kVAh, MVAh, Ah, kAh, MAh, V, A, W, VA, VAr, VArh, kVArh, MVArh, Hz */
enum cl010_unit {
	/** Wh */
	CL010_UNIT_Wh = 0,
	/** kWh */
	CL010_UNIT_kWh,
	/** MWh */
	CL010_UNIT_MWh,
	/** VAh */
	CL010_UNIT_VAh,
	/** kVAh */
	CL010_UNIT_kVAh,
	/** MVAh */
	CL010_UNIT_MVAh,
	/** Ah */
	CL010_UNIT_Ah,
	/** kAh */
	CL010_UNIT_kAh,
	/** MAh */
	CL010_UNIT_MAh,
	/** V */
	CL010_UNIT_V,
	/** A */
	CL010_UNIT_A,
	/** W */
	CL010_UNIT_W,
	/** kW */
	CL010_UNIT_kW,
	/** VA */
	CL010_UNIT_VA,
	/** VAr */
	CL010_UNIT_VAr,
	/** VArh */
	CL010_UNIT_VArh,
	/** kVArh */
	CL010_UNIT_kVArh,
	/** MVArh */
	CL010_UNIT_MVArh,
	/** Hz */
	CL010_UNIT_Hz,
	/** kHz */
	CL010_UNIT_kHz,
	/** MHz */
	CL010_UNIT_MHz,
	/** Number of UNITS */
	CL010_UNIT_NUM
};

typedef struct __attribute__((__packed__)) cl010_unit_pixel {
	uint8_t num_pixels;
	const uint8_t *p_pixels;
} cl010_unit_pixel_t;
/** @} */

/**
 * \name Function Prototypes
 * @{
 */

/**
 * \brief Initialize the CL010 LCD component.
 *
 * This function initializes the LCD driver to control the LCD glass.
 * It perform LCD module initialization according to the CL010 characteristics.
 *
 */
 void DRV_SLCDC_Initialize(void);
 
 /**
 * \brief Update the CL010 LCD component.
 *
 * This function updated the LCD driver to control the LCD glass.
 * It perform LCD module update according to the CL010 characteristics.
 *
 */
void DRV_SLCDC_Update(void);

/**
 * \brief Write string to CL010 LCD glass numeric field.
 *
 * This function will write the input string to the numeric field of the
 * lcd glass.
 *
 * \param data Pointer to the input string(max length is 4)
 */
void cl010_show_numeric_string(enum cl010_line disp_line, const uint8_t *data);

/**
 * \brief Clear a specific icon on the LCD glass.
 *
 * This function will clear a specific icon.
 *
 * \param icon_com  Pixel coordinate - COMx - of the icon.
 * \param icon_seg  Pixel coordinate - SEGy - of the icon.
 *
 * \note Use the icon define in header file.
 */
void cl010_clear_icon(uint8_t icon_com, uint8_t icon_seg);

/**
 * \brief Show a specific icon on the LCD glass.
 *
 * This function will show a specific icon.
 *
 * \param icon_com  Pixel coordinate - COMx - of the icon.
 * \param icon_seg  Pixel coordinate - SEGy - of the icon.
 *
 * \note Use the icon define in header file.
 */
void cl010_show_icon(uint8_t icon_com, uint8_t icon_seg);

/**
 * \brief Blink the current screen content.
 *
 * This function will make the current screen blink.
 *
 */
void cl010_blink_screen(bool inverted);

/**
 * \brief Disable all Blink.
 *
 * This function will disable all Blink content.
 *
 */
void cl010_blink_disable(void);

/**
 * \brief Set the CL010 LCD glass contrast.
 *
 *  This function allows to adjust the contrast of the CL010 LCD glass.
 *
 *  \param contrast  -32 <= signed contrast value <= 31(Using LCDCA)
 *                                0 <= contrast value <= 15(Using SLCDC).
 */
void cl010_set_contrast(int8_t contrast);

/**
 * \brief Show all content of the LCD glass.
 *
 * This function sets all pixels and areas of the LCD glass CL010.
 *
 */
void cl010_show_all(void);

/**
 * \brief Clear all content of the LCD glass.
 *
 * This function clears all pixels and areas of the LCD glass CL010.
 *
 */
void cl010_clear_all(void);

/**
 * \brief Show a decimal numeric value to LCD glass.
 *
 * This function displays an int32 value to the LCD numeric field of the glass.
 *
 * \param value The int32_t value to be displayed
 *
 * \note The value range is [-32768, 32767].
 */
void cl010_show_numeric_value(enum cl010_line disp_line, int32_t value);

/**
 * \brief Show a measurement unit symbol to LCD glass.
 *
 * This function set all pixels needed to show a measurment unit to the LCD glass.
 *
 */
void cl010_show_units(enum cl010_unit disp_unit);

/**
 * \brief Clear all measurement unit symbols of the LCD glass.
 *
 * This function will clear all pixels related to measurment unit of the LCD glass.
 *
 */
void cl010_clear_units(void);

/**
 * \brief Clear line of CL010 LCD glass and the dots icons.
 *
 * This function will clear the full line of the LCD glass.
 */
void cl010_clear_line(enum cl010_line disp_line);

/**
 * \brief All pixels are turned on and the memory content is kept.
 *
 * This function will switch on the full pixels of the LCD glass.
 */
void cl010_force_on(void);

/**
 * \brief All pixels are turned off and the memory content is kept.
 *
 * This function will switch off the full pixels of the LCD glass.
 */
void cl010_force_off(void);

/**
 * \brief All pixels are set in the inverted state as defined in SLCDC memory
 * and the memory content is kept.
 *
 * This function will switch off the full pixels of the LCD glass.
 */
void cl010_set_inverted_mode(void);

/**
 * \brief All pixels are set in the same state as defined in SLCDC memory
 * and the memory content is kept.
 *
 * This function will switch off the full pixels of the LCD glass.
 */
void cl010_set_normal_mode(void);

/**
 * \brief This mode blocks the automatic transfer from the user buffer to the
 * display buffer.
 */
void cl010_select_user_buffer(void);

/**
 * \brief All pixels are alternatively assigned to the state defined in the
 * user buffer then to the state defined in the display buffer.
 */
void cl010_set_swap_mode(void);

/**
 * \brief Show communication condition by the antenna icons on the LCD glass.
 *
 * This function allows to Show communication condition by the antenna icons
 * on the LCD glass..
 *
 * \param val The 0 to 3 value which show the communication condition.
 */
void cl010_communication_value(enum cl010_communication_value val);


status_code_t cl010_init(void);

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

/**
 * \page sam_component_cl010_quick_start Quick Start Guide for the CL010
 * LCD Glass component.
 *
 * This is the quick start guide for the \ref cl010_display_group, with
 * step-by-step instructions on how to configure and use the driver for
 * a specific use case.The code examples can be copied into e.g the main
 * application loop or any other function that will need to control the
 * CL010 LCD Glass component module.
 *
 * \section cl010_qs_use_cases Use cases
 * - \ref cl010_basic
 *
 * \section cl010_basic CL010 LCD Glass basic usage
 *
 * This use case will demonstrate how to initialize the CL010 LCD Glass
 * module.
 *
 *
 * \section cl010_basic_setup Setup steps
 *
 * \subsection cl010_basic_prereq Prerequisites
 *
 * This module requires the following driver
 * - \ref group_sam_drivers_lcdca
 *
 * \subsection cl010_basic_setup_code
 *
 * Add this to the main loop or a setup function:
 * \code
 *      cl010_init();
 * \endcode
 *
 * \subsection cl010_basic_setup_workflow
 *
 * -# Initialize the cl010 module
 *  - \code cl010_init(); \endcode
 *
 * \section cl010_basic_usage Usage steps
 *
 * \subsection cl010_basic_usage_code
 *
 * We can set all the contents by
 * \code
 *      cl010_set_all();;
 * \endcode
 * Or we can clear all the contents by
 * \code
 *      cl010_clear_all();;
 * \endcode
 *
 * We can use the basic show feature by
 * \code
 *      int32_t value = -12345;
 *      cl010_show_icon(CL010_ICON_ARM);
 *      cl010_show_numeric_value(value);
 * \endcode
 *
 * We can use the blink feature by
 * \code
 *      cl010_blink_icon_start(CL010_ICON_COLON);;
 * \endcode
 *
 * We can start/stop the circular animation feature by
 * \code
 *       cl010_circular_animation_start(CL010_CSR_RIGHT, 7, 0x03);
 *
 *       cl010_circular_animation_stop();
 * \endcode
 *
 * We can start/stop the text scrolling feature by
 * \code
 *       uint8_t const scrolling_str[] = "CL010 Example  ";
 *
 *       cl010_text_scrolling_start(scrolling_str,
 *               strlen((char const *)scrolling_str));
 *
 *       cl010_text_scrolling_stop();
 * \endcode
 */

#endif  /* CL010_H_INCLUDED */
