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

//#include "compiler.h"
#include <device.h>
#include <stdio.h>
#include "drv_gfx_slcdc.h"
#include "component/supc.h"
#include "cl010.h"
#include "cl010_font.h"

/** ASCII definition for char space. */
#define ASCII_SP                        0x20
/** ASCII definition for minus sign */
#define ASCII_MINUS                     0x2d
/** ASCII char definition for char '0'. */
#define ASCII_0                         0x30
/** ASCII char definition for char 'A'. */
#define ASCII_BIG_A                     0x41
/** ASCII char definition for char 'a'. */
#define ASCII_LITTLE_A                  0x61

/** Definition for maximum length to display as string. */
#define MAX_STRING_LEN            8

/** Definition of length of symbols */
#define NUM_SYMBOLS_UP            8
#define NUM_SYMBOLS_DOWN          7
#define SYMBOL_LEN                7

/* Symbol pixel array definition */
const enum cl010_symbol_pixel symbol_up1[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_1;
const enum cl010_symbol_pixel symbol_up2[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_2;
const enum cl010_symbol_pixel symbol_up3[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_3;
const enum cl010_symbol_pixel symbol_up4[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_4;
const enum cl010_symbol_pixel symbol_up5[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_5;
const enum cl010_symbol_pixel symbol_up6[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_6;
const enum cl010_symbol_pixel symbol_up7[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_7;
const enum cl010_symbol_pixel symbol_up8[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_8;

const enum cl010_symbol_pixel symbol_up[NUM_SYMBOLS_UP][SYMBOL_LEN] = {
	CL010_PATTERN_SYMBOL_1,
	CL010_PATTERN_SYMBOL_2,
	CL010_PATTERN_SYMBOL_3,
	CL010_PATTERN_SYMBOL_4,
	CL010_PATTERN_SYMBOL_5,
	CL010_PATTERN_SYMBOL_6,
	CL010_PATTERN_SYMBOL_7,
	CL010_PATTERN_SYMBOL_8
};

const enum cl010_symbol_pixel symbol_down1[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_9;
const enum cl010_symbol_pixel symbol_down2[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_10;
const enum cl010_symbol_pixel symbol_down3[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_11;
const enum cl010_symbol_pixel symbol_down4[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_12;
const enum cl010_symbol_pixel symbol_down5[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_13;
const enum cl010_symbol_pixel symbol_down6[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_14;
const enum cl010_symbol_pixel symbol_down7[SYMBOL_LEN] = CL010_PATTERN_SYMBOL_15;

const enum cl010_symbol_pixel symbol_down[NUM_SYMBOLS_DOWN][SYMBOL_LEN] = {
	CL010_PATTERN_SYMBOL_9,
	CL010_PATTERN_SYMBOL_10,
	CL010_PATTERN_SYMBOL_11,
	CL010_PATTERN_SYMBOL_12,
	CL010_PATTERN_SYMBOL_13,
	CL010_PATTERN_SYMBOL_14,
	CL010_PATTERN_SYMBOL_15
};

/* Symbol plot value definition */
const uint8_t plot_number[10] = {CL010_PLOT_NUMBER_0, CL010_PLOT_NUMBER_1,
				 CL010_PLOT_NUMBER_2, CL010_PLOT_NUMBER_3, CL010_PLOT_NUMBER_4,
				 CL010_PLOT_NUMBER_5, CL010_PLOT_NUMBER_6, CL010_PLOT_NUMBER_7,
				 CL010_PLOT_NUMBER_8, CL010_PLOT_NUMBER_9};

/* Symbol units definition */
const uint8_t plot_unit_Wh[]    = {CL010_ICONS_UNIT_Wh};
const uint8_t plot_unit_kWh[]   = {CL010_ICONS_UNIT_kWh};
const uint8_t plot_unit_MWh[]   = {CL010_ICONS_UNIT_MWh};
const uint8_t plot_unit_VAh[]   = {CL010_ICONS_UNIT_VAh};
const uint8_t plot_unit_kVAh[]  = {CL010_ICONS_UNIT_kVAh};
const uint8_t plot_unit_MVAh[]  = {CL010_ICONS_UNIT_MVAh};
const uint8_t plot_unit_Ah[]    = {CL010_ICONS_UNIT_Ah};
const uint8_t plot_unit_kAh[]   = {CL010_ICONS_UNIT_kAh};
const uint8_t plot_unit_MAh[]   = {CL010_ICONS_UNIT_MAh};
const uint8_t plot_unit_VA[]    = {CL010_ICONS_UNIT_VA};
const uint8_t plot_unit_VAr[]   = {CL010_ICONS_UNIT_VAr};
const uint8_t plot_unit_VArh[]  = {CL010_ICONS_UNIT_VArh};
const uint8_t plot_unit_kVArh[] = {CL010_ICONS_UNIT_kVArh};
const uint8_t plot_unit_MVArh[] = {CL010_ICONS_UNIT_MVArh};
const uint8_t plot_unit_Hz[]    = {CL010_ICONS_UNIT_Hz};
const uint8_t plot_unit_kHz[]   = {CL010_ICONS_UNIT_kHz};
const uint8_t plot_unit_MHz[]   = {CL010_ICONS_UNIT_MHz};
const uint8_t plot_unit_V[]     = {CL010_ICONS_UNIT_V};
const uint8_t plot_unit_A[]     = {CL010_ICONS_UNIT_A};
const uint8_t plot_unit_W[]     = {CL010_ICONS_UNIT_W};
const uint8_t plot_unit_kW[]    = {CL010_ICONS_UNIT_kW};

const cl010_unit_pixel_t symbol_units[CL010_UNIT_NUM] = {
	/** Wh */
	{sizeof(plot_unit_Wh), plot_unit_Wh},
	/** kWh */
	{sizeof(plot_unit_kWh), plot_unit_kWh},
	/** MWh */
	{sizeof(plot_unit_MWh), plot_unit_MWh},
	/** VAh */
	{sizeof(plot_unit_VAh), plot_unit_VAh},
	/** kVAh */
	{sizeof(plot_unit_kVAh), plot_unit_kVAh},
	/** MVAh */
	{sizeof(plot_unit_MVAh), plot_unit_MVAh},
	/** Ah */
	{sizeof(plot_unit_Ah), plot_unit_Ah},
	/** kAh */
	{sizeof(plot_unit_kAh), plot_unit_kAh},
	/** MAh */
	{sizeof(plot_unit_MAh), plot_unit_MAh},
	/** V */
	{sizeof(plot_unit_V), plot_unit_V},
	/** A */
	{sizeof(plot_unit_A), plot_unit_A},
	/** W */
	{sizeof(plot_unit_W), plot_unit_W},
	/** kW */
	{sizeof(plot_unit_kW), plot_unit_kW},
	/** VA */
	{sizeof(plot_unit_VA), plot_unit_VA},
	/** VAr */
	{sizeof(plot_unit_VAr), plot_unit_VAr},
	/** VArh */
	{sizeof(plot_unit_VArh), plot_unit_VArh},
	/** kVArh */
	{sizeof(plot_unit_kVArh), plot_unit_kVArh},
	/** MVArh */
	{sizeof(plot_unit_MVArh), plot_unit_MVArh},
	/** Hz */
	{sizeof(plot_unit_Hz), plot_unit_Hz},
	/** kHz */
	{sizeof(plot_unit_kHz), plot_unit_kHz},
	/** MHz */
	{sizeof(plot_unit_MHz), plot_unit_MHz}
};

/**
 * \brief Get supply controller status.
 *
 * \param p_supc Pointer to a SUPC instance.
 *
 * \return The status of supply controller.
 */
static uint32_t supc_get_status(supc_registers_t *p_supc)
{
	return p_supc->SUPC_SR;
}

/**
 * \brief Get SLCDC power mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 *
 * \return The mode of SLCDC.
 */
enum slcdc_power_mode supc_get_slcdc_power_mode(supc_registers_t *p_supc)
{
	return (enum slcdc_power_mode)(p_supc->SUPC_MR & SUPC_MR_LCDMODE_Msk);
}

/**
 * \brief Set SLCDC power mode.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param mode The mode of SLCDC.
 */
static void supc_set_slcdc_power_mode(supc_registers_t *p_supc, enum slcdc_power_mode mode)
{
	enum slcdc_power_mode pre_mode;
	uint32_t tmp;

	pre_mode = supc_get_slcdc_power_mode(p_supc);

	if ((pre_mode == SLCDC_POWER_MODE_LCDON_EXTVR) && (mode == SLCDC_POWER_MODE_LCDON_INVR)) {
		return;
	} else if ((pre_mode == SLCDC_POWER_MODE_LCDON_INVR) && (mode == SLCDC_POWER_MODE_LCDON_EXTVR)) {
		return;
	}

	tmp = p_supc->SUPC_MR;
	tmp &= ~SUPC_MR_LCDMODE_Msk;
	tmp |=  SUPC_MR_KEY_PASSWD | mode;
	p_supc->SUPC_MR = tmp;

	if (mode == SLCDC_POWER_MODE_LCDOFF) {
		while(supc_get_status(p_supc) & SUPC_SR_LCDS_Msk);
	} else {
		while(!(supc_get_status(p_supc) & SUPC_SR_LCDS_Msk));
	}
}

/**
 * \brief Set LCD Voltage Regulator Output.
 *
 * \param p_supc Pointer to a SUPC instance.
 * \param vol  The voltage of Regulator Output.
 */
static void supc_set_slcdc_vol(supc_registers_t *p_supc, uint32_t vol)
{
	uint32_t tmp= p_supc->SUPC_MR;
	tmp &= ~SUPC_MR_LCDOUT_Msk;
	tmp |=  SUPC_MR_KEY_PASSWD |  SUPC_MR_LCDOUT(vol);
	p_supc->SUPC_MR = tmp;
}

/**
 * \brief Set pixel in LCD display memory.
 *
 * This function sets a pixel in LCD display memory. If a parameter
 * is out of range, then the function doesn't set any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel.
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel.
 */
static void cl010_slcdc_set_pixel(slcdc_registers_t *p_slcdc,
		uint8_t pix_com, uint8_t pix_seg)
{
	slcdc_set_lsb_memory(p_slcdc, pix_com, 1 << pix_seg);
}

/**
 * \brief Clear pixel in LCD display memory.
 *
 * This function clears a pixel in LCD display memory. If a parameter
 * is out of range, then the function doesn't clear any bit.
 *
 * \param  pix_com  Pixel coordinate - COMx - of the pixel.
 * \param  pix_seg  Pixel coordinate - SEGy - of the pixel.
 */
static void cl010_slcdc_clear_pixel(slcdc_registers_t *p_slcdc,
		uint8_t pix_com, uint8_t pix_seg)
{
	slcdc_clear_lsb_memory(p_slcdc, pix_com, 1 << pix_seg);
}

/**
 * \brief Turn off a symbol.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param p_symbol  Pointer to an symbol_pixel array.
 * \param number   Symbol pixel array number.
 */
static void cl010_slcdc_efface_symbol(slcdc_registers_t *p_slcdc,
		enum cl010_symbol_pixel *p_symbol, uint8_t number)
{
	while (number--) {
		cl010_slcdc_clear_pixel(p_slcdc, (*(p_symbol + number)) & 0x07,
				(*(p_symbol + number)) >> 3);
	}
}

/**
 * \brief Turn on a symbol according to symbol plot definition.
 *
 * \param p_slcdc Pointer to an SLCDC instance.
 * \param p_symbol  Pointer to an symbol_pixel array.
 * \param number Symbol pixel array number.
 * \param plot   Symbol plot definition value
 */
static void cl010_slcdc_display_symbol(slcdc_registers_t *p_slcdc,
		enum cl010_symbol_pixel *p_symbol, uint8_t number, uint32_t plot)
{
	while (number--) {
		if (plot & (1 << number)) {
			cl010_slcdc_set_pixel(p_slcdc, (*(p_symbol + number)) & 0x07,
					(*(p_symbol + number)) >> 3);
		} else {
			cl010_slcdc_clear_pixel(p_slcdc, (*(p_symbol + number)) & 0x07,
					(*(p_symbol + number)) >> 3);
		}
	}
}

/**
 * \brief Display a string in lcd glass numeric field.
 *
 * This function will write the input string to the numeric field of the
 * lcd glass.
 *
 * \param data Pointer to the input string(max length is 4)
 */
static void cl010_slcdc_display_num_string(enum cl010_line disp_line, const uint8_t *data)
{
	enum cl010_symbol_pixel *p_symbol;
	uint8_t max_length;

	if (disp_line == CL010_LINE_UP) {
		p_symbol = (enum cl010_symbol_pixel *)symbol_up;
		max_length = NUM_SYMBOLS_UP;
	} else {
		p_symbol = (enum cl010_symbol_pixel *)symbol_down;
		max_length = NUM_SYMBOLS_DOWN;
	}

	for (uint8_t i = 0; i < max_length; i++, p_symbol += SYMBOL_LEN) {
		if ((data[i] == ASCII_SP) || (data[i] == 0)) {
			cl010_slcdc_efface_symbol(SLCDC_REGS, p_symbol, SYMBOL_LEN);
		} else if (data[i] == ASCII_MINUS) {
			cl010_slcdc_display_symbol(SLCDC_REGS, p_symbol, SYMBOL_LEN,
					CL010_PLOT_NUMBER_MINUS);
		} else {
			cl010_slcdc_display_symbol(SLCDC_REGS, p_symbol, SYMBOL_LEN,
					plot_number[data[i] - ASCII_0]);
		}
	}
}

void DRV_SLCDC_Update(void)
{
	/* satisfies Harmony3 code regen */
}

void DRV_SLCDC_Initialize(void)
{
	/* Do nothing. Allow the application to initialize the LCD */
}

status_code_t cl010_init(void)
{
	
	/* SLCDC configuration */
	struct slcdc_config slcdc_cfg;
	
	slcdc_get_config_defaults(&slcdc_cfg);

	/* Set LCD power mode */
	supc_set_slcdc_power_mode(SUPC_REGS, slcdc_cfg.controller_power_mode);
	
	if (slcdc_cfg.controller_power_mode == SUPC_MR_LCDMODE_LCDON_INVR) {
		
		supc_set_slcdc_vol(SUPC_REGS, 8);
	}

	/* Reset SLCDC */
	slcdc_reset(SLCDC_REGS);

	/* SLCDC initialization */
	if (slcdc_init(SLCDC_REGS, &slcdc_cfg) != STATUS_OK) 
	{
		return STATUS_ERR_BUSY;
	}
	
	/* LCD seg mapped on SEGx I/O pin */
	slcdc_set_segmap0(SLCDC_REGS, CL010_SEGMAP_NUM_0);

	/* Enable SLCDC */
	slcdc_enable(SLCDC_REGS);
	while (!slcdc_get_status(SLCDC_REGS)) {
	}
		
	return STATUS_OK;
}

void cl010_show_numeric_string(enum cl010_line disp_line, const uint8_t *data)
{
	cl010_slcdc_display_num_string(disp_line, data);
}

void cl010_show_numeric_value(enum cl010_line disp_line, int32_t value)
{
	uint8_t lcd_num[NUM_SYMBOLS_UP + 1];

	if (disp_line == CL010_LINE_UP) {
		sprintf((char *)lcd_num, "%8d", (int)value);
	} else {
		sprintf((char *)lcd_num, "%7d", (int)value);
	}

	cl010_slcdc_display_num_string(disp_line, (uint8_t const *)&lcd_num);
}

void cl010_show_units(enum cl010_unit disp_unit)
{
	cl010_unit_pixel_t *p_unit;
	uint8_t *p_icon;

	p_unit = (cl010_unit_pixel_t *)&symbol_units[disp_unit];
	
	cl010_clear_units();

	p_icon = (uint8_t *)p_unit->p_pixels;
	for (uint8_t i = 0; i < p_unit->num_pixels; i += 2, p_icon += 2) {
		cl010_show_icon(*p_icon, *(p_icon + 1));
	}
}

void cl010_clear_units(void)
{
	cl010_clear_icon(CL010_ICON_UNITS_1);
	cl010_clear_icon(CL010_ICON_UNITS_2);
	cl010_clear_icon(CL010_ICON_UNITS_3);
	cl010_clear_icon(CL010_ICON_UNITS_4);
	cl010_clear_icon(CL010_ICON_UNITS_5);
	cl010_clear_icon(CL010_ICON_UNITS_6);
	cl010_clear_icon(CL010_ICON_UNITS_7);
	cl010_clear_icon(CL010_ICON_UNITS_8);
	cl010_clear_icon(CL010_ICON_UNITS_9);
	cl010_clear_icon(CL010_ICON_UNITS_10);
	cl010_clear_icon(CL010_ICON_UNITS_11);
	cl010_clear_icon(CL010_ICON_UNITS_11_BIS);
	cl010_clear_icon(CL010_ICON_UNITS_12);
	cl010_clear_icon(CL010_ICON_UNITS_13);
	cl010_clear_icon(CL010_ICON_UNITS_14);
}

void cl010_clear_icon(uint8_t icon_com, uint8_t icon_seg)
{
	cl010_slcdc_clear_pixel(SLCDC_REGS, icon_com, icon_seg);
}

void cl010_show_icon(uint8_t icon_com, uint8_t icon_seg)
{
	cl010_slcdc_set_pixel(SLCDC_REGS, icon_com, icon_seg);
}

void cl010_blink_screen(bool inverted)
{
	if (inverted) {
		slcdc_set_display_mode(SLCDC_REGS, SLCDC_DISPMODE_INVERTED_BLINK);
	} else {
		slcdc_set_display_mode(SLCDC_REGS, SLCDC_DISPMODE_BLINKING);
	}

	slcdc_set_blink_freq(SLCDC_REGS, 2, true);
}

void cl010_blink_disable(void)
{
	slcdc_set_display_mode(SLCDC_REGS, SLCDC_DISPMODE_NORMAL);
}

void cl010_set_contrast(int8_t contrast)
{
	//Assert((0 <= contrast) && (contrast < 16));
	uint8_t voltage[16] = {7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8};

	supc_set_slcdc_vol(SUPC_REGS, voltage[contrast]);
}

void cl010_show_all(void)
{
	slcdc_set_display_memory(SLCDC_REGS);
}

void cl010_clear_all(void)
{
	slcdc_clear_display_memory(SLCDC_REGS);
}

void cl010_clear_line(enum cl010_line disp_line)
{
	if (disp_line == CL010_LINE_UP) {
		cl010_clear_icon(CL010_ICON_COL_1);
		cl010_clear_icon(CL010_ICON_COL_2);
		cl010_clear_icon(CL010_ICON_DOT_1);
		cl010_clear_icon(CL010_ICON_DOT_2);
		cl010_clear_icon(CL010_ICON_DOT_3);
	} else {
		cl010_clear_icon(CL010_ICON_DOT_4);
		cl010_clear_icon(CL010_ICON_DOT_5);
		cl010_clear_icon(CL010_ICON_DOT_6);
	}

	cl010_slcdc_display_num_string(disp_line, (const uint8_t *)"        ");
}

void cl010_force_on(void)
{
	slcdc_set_display_mode(SLCDC_REGS, SLCDC_DISPMODE_FORCE_ON);
}

void cl010_force_off(void)
{
	slcdc_set_display_mode(SLCDC_REGS, SLCDC_DISPMODE_FORCE_OFF);
}

void cl010_set_inverted_mode(void)
{
	slcdc_set_display_mode(SLCDC_REGS, SLCDC_DISPMODE_INVERTED);
}

void cl010_set_normal_mode(void)
{
	slcdc_set_display_mode(SLCDC_REGS, SLCDC_DISPMODE_NORMAL);
}

void cl010_select_user_buffer(void)
{
	uint8_t ev_cnt = 0;

	/* Wait two ENDFRAME events */
	while (ev_cnt < 2) {
		if (slcdc_get_interrupt_status(SLCDC_REGS) & SLCDC_ISR_ENDFRAME_Msk) {
			ev_cnt++;
		}
	}

	slcdc_set_display_mode(SLCDC_REGS, SLCDC_DISPMODE_USER_BUFFER);

	/* Wait ENDFRAME event */
	while (!(slcdc_get_interrupt_status(SLCDC_REGS) & SLCDC_ISR_ENDFRAME_Msk)) {
	}
}

void cl010_set_swap_mode(void)
{
	slcdc_set_blink_freq(SLCDC_REGS, 2, true);

	slcdc_set_display_mode(SLCDC_REGS, SLCDC_DISPMODE_BUFFERS_SWAP);
}

void cl010_communication_value(enum cl010_communication_value val)
{
	cl010_slcdc_clear_pixel(SLCDC_REGS, CL010_ICON_COMM_SIGNAL_LOW);
	cl010_slcdc_clear_pixel(SLCDC_REGS, CL010_ICON_COMM_SIGNAL_MED);
	cl010_slcdc_clear_pixel(SLCDC_REGS, CL010_ICON_COMM_SIGNAL_HIG);

	if (val > CL010_COMMUNICATION_TWO) {
		cl010_slcdc_set_pixel(SLCDC_REGS, CL010_ICON_COMM_SIGNAL_HIG);
		cl010_slcdc_set_pixel(SLCDC_REGS, CL010_ICON_COMM_SIGNAL_MED);
		cl010_slcdc_set_pixel(SLCDC_REGS, CL010_ICON_COMM_SIGNAL_LOW);
	} else if (val > CL010_COMMUNICATION_ONE) {
		cl010_slcdc_set_pixel(SLCDC_REGS, CL010_ICON_COMM_SIGNAL_MED);
		cl010_slcdc_set_pixel(SLCDC_REGS, CL010_ICON_COMM_SIGNAL_LOW);
	} else if (val > CL010_COMMUNICATION_NONE) {
		cl010_slcdc_set_pixel(SLCDC_REGS, CL010_ICON_COMM_SIGNAL_LOW);
	}
}
