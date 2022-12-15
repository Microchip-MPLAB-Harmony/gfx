/**
 * \file
 *
 * \brief Default configuration of CL010 LCD Segment Map.
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

#ifndef CL010_SEGMAP_H_INCLUDED
#define CL010_SEGMAP_H_INCLUDED




<#if BoardSelect == "PIC32CXMTSH_DB">
/**
 * \name LCD component CL010 segment map definitions in PIC32CXMTSH_DB
 * @{
 */
/* [(COM - 1), SEG] ICON Definition */
#define CL010_ICON_COL_1              (1 - 1), 21
#define CL010_ICON_COL_2              (1 - 1), 17
#define CL010_ICON_DOT_1              (1 - 1), 15
#define CL010_ICON_DOT_2              (1 - 1), 12
#define CL010_ICON_DOT_3              (1 - 1), 11
#define CL010_ICON_DOT_4              (8 - 1), 16
#define CL010_ICON_DOT_5              (8 - 1), 20
#define CL010_ICON_DOT_6              (8 - 1), 23
#define CL010_ICON_ARROW_LEFT         (8 - 1), 26
#define CL010_ICON_PHASE_1            (8 - 1), 27
#define CL010_ICON_PHASE_2            (1 - 1), 10
#define CL010_ICON_PHASE_3            (1 - 1), 9
#define CL010_ICON_BATTERY_LOW        (2 - 1), 9
#define CL010_ICON_SWITCH_OPEN        (3 - 1), 9
#define CL010_ICON_COMM_SIGNAL_LOW    (4 - 1), 9
#define CL010_ICON_COMM_SIGNAL_MED    (5 - 1), 9
#define CL010_ICON_COMM_SIGNAL_HIG    (6 - 1), 9
#define CL010_ICON_MICROCHIP          (7 - 1), 9
#define CL010_ICON_LCD                (1 - 1), 7
#define CL010_ICON_P_PLUS             (6 - 1), 6
#define CL010_ICON_Q_PLUS             (5 - 1), 6
#define CL010_ICON_P_MINUS            (7 - 1), 6
#define CL010_ICON_Q_MINUS            (8 - 1), 6
#define CL010_ICON_DATE               (8 - 1), 13
#define CL010_ICON_TIME               (7 - 1), 13
#define CL010_ICON_DST                (6 - 1), 13
#define CL010_ICON_CUM                (5 - 1), 13
#define CL010_ICON_MD                 (4 - 1), 13
#define CL010_ICON_PF                 (3 - 1), 13
#define CL010_ICON_L1                 (1 - 1), 6
#define CL010_ICON_L2                 (2 - 1), 6
#define CL010_ICON_L3                 (3 - 1), 6
#define CL010_ICON_N                  (4 - 1), 6
#define CL010_ICON_UNITS_1            (8 - 1), 8
#define CL010_ICON_UNITS_2            (7 - 1), 8
#define CL010_ICON_UNITS_3            (6 - 1), 8
#define CL010_ICON_UNITS_4            (5 - 1), 8
#define CL010_ICON_UNITS_5            (4 - 1), 8
#define CL010_ICON_UNITS_6            (3 - 1), 8
#define CL010_ICON_UNITS_7            (2 - 1), 8
#define CL010_ICON_UNITS_8            (1 - 1), 8
#define CL010_ICON_UNITS_9            (2 - 1), 7
#define CL010_ICON_UNITS_10           (4 - 1), 7
#define CL010_ICON_UNITS_11           (3 - 1), 7
#define CL010_ICON_UNITS_11_BIS       (5 - 1), 7
#define CL010_ICON_UNITS_12           (6 - 1), 7
#define CL010_ICON_UNITS_13           (7 - 1), 7
#define CL010_ICON_UNITS_14           (8 - 1), 7

/* ((SEG << 3) | (COM - 1)) Symbol Definition */
#define CL010_D1A_SEG_NUM             ((25 << 3) | (8 - 1))
#define CL010_D1B_SEG_NUM             ((25 << 3) | (6 - 1))
#define CL010_D1C_SEG_NUM             ((25 << 3) | (3 - 1))
#define CL010_D1D_SEG_NUM             ((25 << 3) | (2 - 1))
#define CL010_D1E_SEG_NUM             ((25 << 3) | (4 - 1))
#define CL010_D1F_SEG_NUM             ((25 << 3) | (7 - 1))
#define CL010_D1G_SEG_NUM             ((25 << 3) | (5 - 1))
#define CL010_D2A_SEG_NUM             ((21 << 3) | (8 - 1))
#define CL010_D2B_SEG_NUM             ((21 << 3) | (6 - 1))
#define CL010_D2C_SEG_NUM             ((21 << 3) | (3 - 1))
#define CL010_D2D_SEG_NUM             ((21 << 3) | (2 - 1))
#define CL010_D2E_SEG_NUM             ((21 << 3) | (4 - 1))
#define CL010_D2F_SEG_NUM             ((21 << 3) | (7 - 1))
#define CL010_D2G_SEG_NUM             ((21 << 3) | (5 - 1))
#define CL010_D3A_SEG_NUM             ((19 << 3) | (8 - 1))
#define CL010_D3B_SEG_NUM             ((19 << 3) | (6 - 1))
#define CL010_D3C_SEG_NUM             ((19 << 3) | (3 - 1))
#define CL010_D3D_SEG_NUM             ((19 << 3) | (2 - 1))
#define CL010_D3E_SEG_NUM             ((19 << 3) | (4 - 1))
#define CL010_D3F_SEG_NUM             ((19 << 3) | (7 - 1))
#define CL010_D3G_SEG_NUM             ((19 << 3) | (5 - 1))
#define CL010_D4A_SEG_NUM             ((17 << 3) | (8 - 1))
#define CL010_D4B_SEG_NUM             ((17 << 3) | (6 - 1))
#define CL010_D4C_SEG_NUM             ((17 << 3) | (3 - 1))
#define CL010_D4D_SEG_NUM             ((17 << 3) | (2 - 1))
#define CL010_D4E_SEG_NUM             ((17 << 3) | (4 - 1))
#define CL010_D4F_SEG_NUM             ((17 << 3) | (7 - 1))
#define CL010_D4G_SEG_NUM             ((17 << 3) | (5 - 1))
#define CL010_D5A_SEG_NUM             ((15 << 3) | (8 - 1))
#define CL010_D5B_SEG_NUM             ((15 << 3) | (6 - 1))
#define CL010_D5C_SEG_NUM             ((15 << 3) | (3 - 1))
#define CL010_D5D_SEG_NUM             ((15 << 3) | (2 - 1))
#define CL010_D5E_SEG_NUM             ((15 << 3) | (4 - 1))
#define CL010_D5F_SEG_NUM             ((15 << 3) | (7 - 1))
#define CL010_D5G_SEG_NUM             ((15 << 3) | (5 - 1))
#define CL010_D6A_SEG_NUM             ((12 << 3) | (8 - 1))
#define CL010_D6B_SEG_NUM             ((12 << 3) | (6 - 1))
#define CL010_D6C_SEG_NUM             ((12 << 3) | (3 - 1))
#define CL010_D6D_SEG_NUM             ((12 << 3) | (2 - 1))
#define CL010_D6E_SEG_NUM             ((12 << 3) | (4 - 1))
#define CL010_D6F_SEG_NUM             ((12 << 3) | (7 - 1))
#define CL010_D6G_SEG_NUM             ((12 << 3) | (5 - 1))
#define CL010_D7A_SEG_NUM             ((11 << 3) | (8 - 1))
#define CL010_D7B_SEG_NUM             ((11 << 3) | (6 - 1))
#define CL010_D7C_SEG_NUM             ((11 << 3) | (3 - 1))
#define CL010_D7D_SEG_NUM             ((11 << 3) | (2 - 1))
#define CL010_D7E_SEG_NUM             ((11 << 3) | (4 - 1))
#define CL010_D7F_SEG_NUM             ((11 << 3) | (7 - 1))
#define CL010_D7G_SEG_NUM             ((11 << 3) | (5 - 1))
#define CL010_D8A_SEG_NUM             ((10 << 3) | (8 - 1))
#define CL010_D8B_SEG_NUM             ((10 << 3) | (6 - 1))
#define CL010_D8C_SEG_NUM             ((10 << 3) | (3 - 1))
#define CL010_D8D_SEG_NUM             ((10 << 3) | (2 - 1))
#define CL010_D8E_SEG_NUM             ((10 << 3) | (4 - 1))
#define CL010_D8F_SEG_NUM             ((10 << 3) | (7 - 1))
#define CL010_D8G_SEG_NUM             ((10 << 3) | (5 - 1))
#define CL010_D9A_SEG_NUM             ((14 << 3) | (1 - 1))
#define CL010_D9B_SEG_NUM             ((14 << 3) | (2 - 1))
#define CL010_D9C_SEG_NUM             ((14 << 3) | (5 - 1))
#define CL010_D9D_SEG_NUM             ((14 << 3) | (7 - 1))
#define CL010_D9E_SEG_NUM             ((14 << 3) | (6 - 1))
#define CL010_D9F_SEG_NUM             ((14 << 3) | (3 - 1))
#define CL010_D9G_SEG_NUM             ((14 << 3) | (4 - 1))
#define CL010_D10A_SEG_NUM            ((16 << 3) | (1 - 1))
#define CL010_D10B_SEG_NUM            ((16 << 3) | (2 - 1))
#define CL010_D10C_SEG_NUM            ((16 << 3) | (5 - 1))
#define CL010_D10D_SEG_NUM            ((16 << 3) | (7 - 1))
#define CL010_D10E_SEG_NUM            ((16 << 3) | (6 - 1))
#define CL010_D10F_SEG_NUM            ((16 << 3) | (3 - 1))
#define CL010_D10G_SEG_NUM            ((16 << 3) | (4 - 1))
#define CL010_D11A_SEG_NUM            ((18 << 3) | (1 - 1))
#define CL010_D11B_SEG_NUM            ((18 << 3) | (2 - 1))
#define CL010_D11C_SEG_NUM            ((18 << 3) | (5 - 1))
#define CL010_D11D_SEG_NUM            ((18 << 3) | (7 - 1))
#define CL010_D11E_SEG_NUM            ((18 << 3) | (6 - 1))
#define CL010_D11F_SEG_NUM            ((18 << 3) | (3 - 1))
#define CL010_D11G_SEG_NUM            ((18 << 3) | (4 - 1))
#define CL010_D12A_SEG_NUM            ((20 << 3) | (1 - 1))
#define CL010_D12B_SEG_NUM            ((20 << 3) | (2 - 1))
#define CL010_D12C_SEG_NUM            ((20 << 3) | (5 - 1))
#define CL010_D12D_SEG_NUM            ((20 << 3) | (7 - 1))
#define CL010_D12E_SEG_NUM            ((20 << 3) | (6 - 1))
#define CL010_D12F_SEG_NUM            ((20 << 3) | (3 - 1))
#define CL010_D12G_SEG_NUM            ((20 << 3) | (4 - 1))
#define CL010_D13A_SEG_NUM            ((23 << 3) | (1 - 1))
#define CL010_D13B_SEG_NUM            ((23 << 3) | (2 - 1))
#define CL010_D13C_SEG_NUM            ((23 << 3) | (5 - 1))
#define CL010_D13D_SEG_NUM            ((23 << 3) | (7 - 1))
#define CL010_D13E_SEG_NUM            ((23 << 3) | (6 - 1))
#define CL010_D13F_SEG_NUM            ((23 << 3) | (3 - 1))
#define CL010_D13G_SEG_NUM            ((23 << 3) | (4 - 1))
#define CL010_D14A_SEG_NUM            ((26 << 3) | (1 - 1))
#define CL010_D14B_SEG_NUM            ((26 << 3) | (2 - 1))
#define CL010_D14C_SEG_NUM            ((26 << 3) | (5 - 1))
#define CL010_D14D_SEG_NUM            ((26 << 3) | (7 - 1))
#define CL010_D14E_SEG_NUM            ((26 << 3) | (6 - 1))
#define CL010_D14F_SEG_NUM            ((26 << 3) | (3 - 1))
#define CL010_D14G_SEG_NUM            ((26 << 3) | (4 - 1))
#define CL010_D15A_SEG_NUM            ((27 << 3) | (1 - 1))
#define CL010_D15B_SEG_NUM            ((27 << 3) | (2 - 1))
#define CL010_D15C_SEG_NUM            ((27 << 3) | (5 - 1))
#define CL010_D15D_SEG_NUM            ((27 << 3) | (7 - 1))
#define CL010_D15E_SEG_NUM            ((27 << 3) | (6 - 1))
#define CL010_D15F_SEG_NUM            ((27 << 3) | (3 - 1))
#define CL010_D15G_SEG_NUM            ((27 << 3) | (4 - 1))

/* The LCD segment map number */
#define CL010_SEGMAP_NUM_0     0x0EBFFFC0
/* @} */


<#elseif BoardSelect == "PIC32CXMTC_DB">

/**
 * \name LCD component CL010 segment map definitions in PIC32CXMTC_DB (128)
 * @{
 */
/* [(COM - 1), SEG] ICON Definition */
#define CL010_ICON_COL_1              (1 - 1), 24
#define CL010_ICON_COL_2              (1 - 1), 20
#define CL010_ICON_DOT_1              (1 - 1), 15
#define CL010_ICON_DOT_2              (1 - 1), 11
#define CL010_ICON_DOT_3              (1 - 1), 7
#define CL010_ICON_DOT_4              (8 - 1), 17
#define CL010_ICON_DOT_5              (8 - 1), 23
#define CL010_ICON_DOT_6              (8 - 1), 25
#define CL010_ICON_ARROW_LEFT         (8 - 1), 27
#define CL010_ICON_PHASE_1            (8 - 1), 29
#define CL010_ICON_PHASE_2            (1 - 1), 6
#define CL010_ICON_PHASE_3            (1 - 1), 12
#define CL010_ICON_BATTERY_LOW        (2 - 1), 12
#define CL010_ICON_SWITCH_OPEN        (3 - 1), 12
#define CL010_ICON_COMM_SIGNAL_LOW    (4 - 1), 12
#define CL010_ICON_COMM_SIGNAL_MED    (5 - 1), 12
#define CL010_ICON_COMM_SIGNAL_HIG    (6 - 1), 12
#define CL010_ICON_MICROCHIP          (7 - 1), 12
#define CL010_ICON_LCD                (1 - 1), 3
#define CL010_ICON_P_PLUS             (6 - 1), 0
#define CL010_ICON_Q_PLUS             (5 - 1), 0
#define CL010_ICON_P_MINUS            (7 - 1), 0
#define CL010_ICON_Q_MINUS            (8 - 1), 0
#define CL010_ICON_DATE               (8 - 1), 10
#define CL010_ICON_TIME               (7 - 1), 10
#define CL010_ICON_DST                (6 - 1), 10
#define CL010_ICON_CUM                (5 - 1), 10
#define CL010_ICON_MD                 (4 - 1), 10
#define CL010_ICON_PF                 (3 - 1), 10
#define CL010_ICON_L1                 (1 - 1), 0
#define CL010_ICON_L2                 (2 - 1), 0
#define CL010_ICON_L3                 (3 - 1), 0
#define CL010_ICON_N                  (4 - 1), 0
#define CL010_ICON_UNITS_1            (8 - 1), 16
#define CL010_ICON_UNITS_2            (7 - 1), 16
#define CL010_ICON_UNITS_3            (6 - 1), 16
#define CL010_ICON_UNITS_4            (5 - 1), 16
#define CL010_ICON_UNITS_5            (4 - 1), 16
#define CL010_ICON_UNITS_6            (3 - 1), 16
#define CL010_ICON_UNITS_7            (2 - 1), 16
#define CL010_ICON_UNITS_8            (1 - 1), 16
#define CL010_ICON_UNITS_9            (2 - 1), 3
#define CL010_ICON_UNITS_10           (4 - 1), 3
#define CL010_ICON_UNITS_11           (3 - 1), 3
#define CL010_ICON_UNITS_11_BIS       (5 - 1), 3
#define CL010_ICON_UNITS_12           (6 - 1), 3
#define CL010_ICON_UNITS_13           (7 - 1), 3
#define CL010_ICON_UNITS_14           (8 - 1), 3

/* ((SEG << 3) | (COM - 1)) Symbol Definition */
#define CL010_D1A_SEG_NUM             ((26 << 3) | (8 - 1))
#define CL010_D1B_SEG_NUM             ((26 << 3) | (6 - 1))
#define CL010_D1C_SEG_NUM             ((26 << 3) | (3 - 1))
#define CL010_D1D_SEG_NUM             ((26 << 3) | (2 - 1))
#define CL010_D1E_SEG_NUM             ((26 << 3) | (4 - 1))
#define CL010_D1F_SEG_NUM             ((26 << 3) | (7 - 1))
#define CL010_D1G_SEG_NUM             ((26 << 3) | (5 - 1))
#define CL010_D2A_SEG_NUM             ((24 << 3) | (8 - 1))
#define CL010_D2B_SEG_NUM             ((24 << 3) | (6 - 1))
#define CL010_D2C_SEG_NUM             ((24 << 3) | (3 - 1))
#define CL010_D2D_SEG_NUM             ((24 << 3) | (2 - 1))
#define CL010_D2E_SEG_NUM             ((24 << 3) | (4 - 1))
#define CL010_D2F_SEG_NUM             ((24 << 3) | (7 - 1))
#define CL010_D2G_SEG_NUM             ((24 << 3) | (5 - 1))
#define CL010_D3A_SEG_NUM             ((22 << 3) | (8 - 1))
#define CL010_D3B_SEG_NUM             ((22 << 3) | (6 - 1))
#define CL010_D3C_SEG_NUM             ((22 << 3) | (3 - 1))
#define CL010_D3D_SEG_NUM             ((22 << 3) | (2 - 1))
#define CL010_D3E_SEG_NUM             ((22 << 3) | (4 - 1))
#define CL010_D3F_SEG_NUM             ((22 << 3) | (7 - 1))
#define CL010_D3G_SEG_NUM             ((22 << 3) | (5 - 1))
#define CL010_D4A_SEG_NUM             ((20 << 3) | (8 - 1))
#define CL010_D4B_SEG_NUM             ((20 << 3) | (6 - 1))
#define CL010_D4C_SEG_NUM             ((20 << 3) | (3 - 1))
#define CL010_D4D_SEG_NUM             ((20 << 3) | (2 - 1))
#define CL010_D4E_SEG_NUM             ((20 << 3) | (4 - 1))
#define CL010_D4F_SEG_NUM             ((20 << 3) | (7 - 1))
#define CL010_D4G_SEG_NUM             ((20 << 3) | (5 - 1))
#define CL010_D5A_SEG_NUM             ((15 << 3) | (8 - 1))
#define CL010_D5B_SEG_NUM             ((15 << 3) | (6 - 1))
#define CL010_D5C_SEG_NUM             ((15 << 3) | (3 - 1))
#define CL010_D5D_SEG_NUM             ((15 << 3) | (2 - 1))
#define CL010_D5E_SEG_NUM             ((15 << 3) | (4 - 1))
#define CL010_D5F_SEG_NUM             ((15 << 3) | (7 - 1))
#define CL010_D5G_SEG_NUM             ((15 << 3) | (5 - 1))
#define CL010_D6A_SEG_NUM             ((11 << 3) | (8 - 1))
#define CL010_D6B_SEG_NUM             ((11 << 3) | (6 - 1))
#define CL010_D6C_SEG_NUM             ((11 << 3) | (3 - 1))
#define CL010_D6D_SEG_NUM             ((11 << 3) | (2 - 1))
#define CL010_D6E_SEG_NUM             ((11 << 3) | (4 - 1))
#define CL010_D6F_SEG_NUM             ((11 << 3) | (7 - 1))
#define CL010_D6G_SEG_NUM             ((11 << 3) | (5 - 1))
#define CL010_D7A_SEG_NUM             ((7 << 3) | (8 - 1))
#define CL010_D7B_SEG_NUM             ((7 << 3) | (6 - 1))
#define CL010_D7C_SEG_NUM             ((7 << 3) | (3 - 1))
#define CL010_D7D_SEG_NUM             ((7 << 3) | (2 - 1))
#define CL010_D7E_SEG_NUM             ((7 << 3) | (4 - 1))
#define CL010_D7F_SEG_NUM             ((7 << 3) | (7 - 1))
#define CL010_D7G_SEG_NUM             ((7 << 3) | (5 - 1))
#define CL010_D8A_SEG_NUM             ((6 << 3) | (8 - 1))
#define CL010_D8B_SEG_NUM             ((6 << 3) | (6 - 1))
#define CL010_D8C_SEG_NUM             ((6 << 3) | (3 - 1))
#define CL010_D8D_SEG_NUM             ((6 << 3) | (2 - 1))
#define CL010_D8E_SEG_NUM             ((6 << 3) | (4 - 1))
#define CL010_D8F_SEG_NUM             ((6 << 3) | (7 - 1))
#define CL010_D8G_SEG_NUM             ((6 << 3) | (5 - 1))
#define CL010_D9A_SEG_NUM             ((14 << 3) | (1 - 1))
#define CL010_D9B_SEG_NUM             ((14 << 3) | (2 - 1))
#define CL010_D9C_SEG_NUM             ((14 << 3) | (5 - 1))
#define CL010_D9D_SEG_NUM             ((14 << 3) | (7 - 1))
#define CL010_D9E_SEG_NUM             ((14 << 3) | (6 - 1))
#define CL010_D9F_SEG_NUM             ((14 << 3) | (3 - 1))
#define CL010_D9G_SEG_NUM             ((14 << 3) | (4 - 1))
#define CL010_D10A_SEG_NUM            ((17 << 3) | (1 - 1))
#define CL010_D10B_SEG_NUM            ((17 << 3) | (2 - 1))
#define CL010_D10C_SEG_NUM            ((17 << 3) | (5 - 1))
#define CL010_D10D_SEG_NUM            ((17 << 3) | (7 - 1))
#define CL010_D10E_SEG_NUM            ((17 << 3) | (6 - 1))
#define CL010_D10F_SEG_NUM            ((17 << 3) | (3 - 1))
#define CL010_D10G_SEG_NUM            ((17 << 3) | (4 - 1))
#define CL010_D11A_SEG_NUM            ((21 << 3) | (1 - 1))
#define CL010_D11B_SEG_NUM            ((21 << 3) | (2 - 1))
#define CL010_D11C_SEG_NUM            ((21 << 3) | (5 - 1))
#define CL010_D11D_SEG_NUM            ((21 << 3) | (7 - 1))
#define CL010_D11E_SEG_NUM            ((21 << 3) | (6 - 1))
#define CL010_D11F_SEG_NUM            ((21 << 3) | (3 - 1))
#define CL010_D11G_SEG_NUM            ((21 << 3) | (4 - 1))
#define CL010_D12A_SEG_NUM            ((23 << 3) | (1 - 1))
#define CL010_D12B_SEG_NUM            ((23 << 3) | (2 - 1))
#define CL010_D12C_SEG_NUM            ((23 << 3) | (5 - 1))
#define CL010_D12D_SEG_NUM            ((23 << 3) | (7 - 1))
#define CL010_D12E_SEG_NUM            ((23 << 3) | (6 - 1))
#define CL010_D12F_SEG_NUM            ((23 << 3) | (3 - 1))
#define CL010_D12G_SEG_NUM            ((23 << 3) | (4 - 1))
#define CL010_D13A_SEG_NUM            ((25 << 3) | (1 - 1))
#define CL010_D13B_SEG_NUM            ((25 << 3) | (2 - 1))
#define CL010_D13C_SEG_NUM            ((25 << 3) | (5 - 1))
#define CL010_D13D_SEG_NUM            ((25 << 3) | (7 - 1))
#define CL010_D13E_SEG_NUM            ((25 << 3) | (6 - 1))
#define CL010_D13F_SEG_NUM            ((25 << 3) | (3 - 1))
#define CL010_D13G_SEG_NUM            ((25 << 3) | (4 - 1))
#define CL010_D14A_SEG_NUM            ((27 << 3) | (1 - 1))
#define CL010_D14B_SEG_NUM            ((27 << 3) | (2 - 1))
#define CL010_D14C_SEG_NUM            ((27 << 3) | (5 - 1))
#define CL010_D14D_SEG_NUM            ((27 << 3) | (7 - 1))
#define CL010_D14E_SEG_NUM            ((27 << 3) | (6 - 1))
#define CL010_D14F_SEG_NUM            ((27 << 3) | (3 - 1))
#define CL010_D14G_SEG_NUM            ((27 << 3) | (4 - 1))
#define CL010_D15A_SEG_NUM            ((29 << 3) | (1 - 1))
#define CL010_D15B_SEG_NUM            ((29 << 3) | (2 - 1))
#define CL010_D15C_SEG_NUM            ((29 << 3) | (5 - 1))
#define CL010_D15D_SEG_NUM            ((29 << 3) | (7 - 1))
#define CL010_D15E_SEG_NUM            ((29 << 3) | (6 - 1))
#define CL010_D15F_SEG_NUM            ((29 << 3) | (3 - 1))
#define CL010_D15G_SEG_NUM            ((29 << 3) | (4 - 1))

/* The LCD segment map number */
#define CL010_SEGMAP_NUM_0     0x2FF3DCC9
</#if>


#define CL010_ICONS_UNIT_W      CL010_ICON_UNITS_6, CL010_ICON_UNITS_7, CL010_ICON_UNITS_8
#define CL010_ICONS_UNIT_h      CL010_ICON_UNITS_10, CL010_ICON_UNITS_11, CL010_ICON_UNITS_11_BIS
#define CL010_ICONS_UNIT_k      CL010_ICON_UNITS_1, CL010_ICON_UNITS_3, CL010_ICON_UNITS_4
#define CL010_ICONS_UNIT_M      CL010_ICON_UNITS_1, CL010_ICON_UNITS_2, CL010_ICON_UNITS_4, CL010_ICON_UNITS_5
#define CL010_ICONS_UNIT_r      CL010_ICON_UNITS_10
#define CL010_ICONS_UNIT_H      CL010_ICON_UNITS_12, CL010_ICON_UNITS_13
#define CL010_ICONS_UNIT_z      CL010_ICON_UNITS_14
#define CL010_ICONS_UNIT_V      CL010_ICON_UNITS_6
#define CL010_ICONS_UNIT_A      CL010_ICON_UNITS_8, CL010_ICON_UNITS_9
#define CL010_ICONS_UNIT_kW     CL010_ICONS_UNIT_k, CL010_ICONS_UNIT_W
#define CL010_ICONS_UNIT_Wh     CL010_ICONS_UNIT_W, CL010_ICONS_UNIT_h
#define CL010_ICONS_UNIT_kWh    CL010_ICONS_UNIT_k, CL010_ICONS_UNIT_W, CL010_ICONS_UNIT_h
#define CL010_ICONS_UNIT_MWh    CL010_ICONS_UNIT_M, CL010_ICONS_UNIT_W, CL010_ICONS_UNIT_h
#define CL010_ICONS_UNIT_VAh    CL010_ICONS_UNIT_V, CL010_ICONS_UNIT_A, CL010_ICONS_UNIT_h
#define CL010_ICONS_UNIT_kVAh   CL010_ICONS_UNIT_k, CL010_ICONS_UNIT_V, CL010_ICONS_UNIT_A, CL010_ICONS_UNIT_h
#define CL010_ICONS_UNIT_MVAh   CL010_ICONS_UNIT_M, CL010_ICONS_UNIT_V, CL010_ICONS_UNIT_A, CL010_ICONS_UNIT_h
#define CL010_ICONS_UNIT_Ah     CL010_ICONS_UNIT_A, CL010_ICONS_UNIT_h
#define CL010_ICONS_UNIT_kAh    CL010_ICONS_UNIT_k, CL010_ICONS_UNIT_A, CL010_ICONS_UNIT_h
#define CL010_ICONS_UNIT_MAh    CL010_ICONS_UNIT_M, CL010_ICONS_UNIT_A, CL010_ICONS_UNIT_h
#define CL010_ICONS_UNIT_VA     CL010_ICONS_UNIT_V, CL010_ICONS_UNIT_A
#define CL010_ICONS_UNIT_VAr    CL010_ICONS_UNIT_V, CL010_ICONS_UNIT_A, CL010_ICONS_UNIT_r
#define CL010_ICONS_UNIT_VArh   CL010_ICONS_UNIT_V, CL010_ICONS_UNIT_A, CL010_ICONS_UNIT_r, CL010_ICONS_UNIT_H
#define CL010_ICONS_UNIT_kVArh  CL010_ICONS_UNIT_k, CL010_ICONS_UNIT_V, CL010_ICONS_UNIT_A, CL010_ICONS_UNIT_r, CL010_ICONS_UNIT_H
#define CL010_ICONS_UNIT_MVArh  CL010_ICONS_UNIT_M, CL010_ICONS_UNIT_V, CL010_ICONS_UNIT_A, CL010_ICONS_UNIT_r, CL010_ICONS_UNIT_H
#define CL010_ICONS_UNIT_Hz     CL010_ICONS_UNIT_H, CL010_ICONS_UNIT_z
#define CL010_ICONS_UNIT_kHz    CL010_ICONS_UNIT_k, CL010_ICONS_UNIT_H, CL010_ICONS_UNIT_z
#define CL010_ICONS_UNIT_MHz    CL010_ICONS_UNIT_M, CL010_ICONS_UNIT_H, CL010_ICONS_UNIT_z

#endif  /* CL010_SEGMAP_H_INCLUDED */
