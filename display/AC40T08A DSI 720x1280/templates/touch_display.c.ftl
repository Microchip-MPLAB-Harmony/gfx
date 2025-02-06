/*******************************************************************************
  MPLAB Harmony Generated Display Driver Sync Implementation File

  File Name:
    touch_display.c

  Summary:
    Implements display driver support for the
    AC40T08A DSI LCD Touch Display.
    https://www.microchip.com/en-us/development-tool/ac40t08a

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#include "string.h"
#include "gfx/display/touch_display.h"
#include "gfx/driver/controller/xlcdc/bridge/dsi/plib_dsi.h"

/* Manufacturer specific commands sent via DSI, listed in HX8394-F datasheet */
#define HX8394_CMD_SETSEQUENCE          0xb0
#define HX8394_CMD_SETPOWER             0xb1
#define HX8394_CMD_SETDISP              0xb2
#define HX8394_CMD_SETCYC               0xb4
#define HX8394_CMD_SETVCOM              0xb6
#define HX8394_CMD_SETTE                0xb7
#define HX8394_CMD_SETSENSOR            0xb8
#define HX8394_CMD_SETEXTC              0xb9
#define HX8394_CMD_SETMIPI              0xba
#define HX8394_CMD_SETOTP               0xbb
#define HX8394_CMD_SETREGBANK           0xbd
#define HX8394_CMD_UNKNOWN1             0xc0
#define HX8394_CMD_SETDGCLUT            0xc1
#define HX8394_CMD_SETID                0xc3
#define HX8394_CMD_SETDDB               0xc4
#define HX8394_CMD_UNKNOWN2             0xc6
#define HX8394_CMD_SETCABC              0xc9
#define HX8394_CMD_SETCABCGAIN          0xca
#define HX8394_CMD_SETPANEL             0xcc
#define HX8394_CMD_SETOFFSET            0xd2
#define HX8394_CMD_SETGIP0              0xd3
#define HX8394_CMD_UNKNOWN3             0xd4
#define HX8394_CMD_SETGIP1              0xd5
#define HX8394_CMD_SETGIP2              0xd6
#define HX8394_CMD_SETGPO               0xd6
#define HX8394_CMD_SETGIP3              0xd8
#define HX8394_CMD_SETSCALING           0xdd
#define HX8394_CMD_SETIDLE              0xdf
#define HX8394_CMD_SETGAMMA             0xe0
#define HX8394_CMD_SETCHEMODE_DYN       0xe4
#define HX8394_CMD_SETCHE               0xe5
#define HX8394_CMD_SETCESEL             0xe6
#define HX8394_CMD_SET_SP_CMD           0xe9
#define HX8394_CMD_SETREADINDEX         0xfe
#define HX8394_CMD_GETSPIREAD           0xff
#define HX8394_DCS_EXIT_SLEEP_MODE      0x11
#define HX8394_DCS_SET_DISPLAY_ON       0x29

/* Driver Data */
DISP_DATA disp_data;

/* Local Driver Functions */
static void DSI_DCS_Write(uint8_t * data, size_t size) {
    DSI_GENERIC_HEADER header;
    DSI_GENERIC_PAYLOAD payload[32];

    memset(&header, 0, sizeof(header));
    memset(payload, 0, sizeof(payload));

    switch (size) {
        case 0:
        {
            break;
        }
        case 1:
        {
            header.shortPacket.dataType = 0x05; // DCS SP, no parameter
            header.shortPacket.data[0] = data[0];
            DSI_Write(&header, payload);

            break;
        }
        case 2:
        {
            header.shortPacket.dataType = 0x15; // DCS SP, 1 parameter
            header.shortPacket.data[0] = data[0];
            header.shortPacket.data[1] = data[1];
            DSI_Write(&header, payload);

            break;
        }
        default:
        {
            header.longPacket.dataType = 0x39; // DCS LP, no parameter
            header.longPacket.size = size;
            memcpy(payload, data, size);
            DSI_Write(&header, payload);

            break;
        }
    }
}

/* Driver Helper for DCS Sequences */
#define DSI_DCS_WriteSequence(seq...)\
({\
	static uint8_t x[] = { seq };\
	DSI_DCS_Write(x, sizeof(x));\
})

void DISP_Initialize(void)
{
    memset(&disp_data, 0, sizeof(disp_data));

    disp_data.state = DISP_STATE_CONFIGURE;
}

void DISP_Update(void)
{
    switch(disp_data.state)
    {
        case DISP_STATE_CONFIGURE:
        {
            DSI_CommandMode();

            /* 5.19.8 SETEXTC: Set extension command (B9h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETEXTC,
                                   0xff, 0x83, 0x94);

            /* 5.19.9 SETMIPI: Set MIPI control (BAh) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETMIPI,
                                   0x63, 0x03, 0x68, 0x6b, 0xb2, 0xc0);

            /* 5.19.2 SETPOWER: Set power (B1h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETPOWER,
                                   0x48, 0x12, 0x72, 0x09, 0x32, 0x54,
                                   0x71, 0x71, 0x57, 0x47);

            /* 5.19.3 SETDISP: Set display related register (B2h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETDISP,
                                   0x00, 0x80, 0x64, 0x0c, 0x0d, 0x2f,
                                   0x00, 0x00, 0x00, 0x00, 0x00);

            /* 5.19.4 SETCYC: Set display waveform cycles (B4h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETCYC,
                                   0x73, 0x74, 0x73, 0x74, 0x73, 0x74,
                                   0x01, 0x0C, 0x86, 0x75, 0x00, 0x3F,
                                   0x73, 0x74, 0x73, 0x74, 0x73, 0x74,
                                   0x01, 0x0C, 0x86);

            /* 5.19.5 SETVCOM: Set VCOM voltage (B6h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETVCOM,
                                   0x6e, 0x6e);

            /* 5.19.19 SETGIP0: Set GIP Option0 (D3h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETGIP0,
                                   0x00, 0x00, 0x07, 0x07, 0x40, 0x07,
                                   0x0c, 0x00, 0x08, 0x10, 0x08, 0x00,
                                   0x08, 0x54, 0x15, 0x0a, 0x05, 0x0a,
                                   0x02, 0x15, 0x06, 0x05, 0x06, 0x47,
                                   0x44, 0x0a, 0x0a, 0x4b, 0x10, 0x07,
                                   0x07, 0x0c, 0x40);

            /* 5.19.20 Set GIP Option1 (D5h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETGIP1,
                                   0x1C, 0x1C, 0x1D, 0x1D, 0x00, 0x01,
                                   0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                   0x08, 0x09, 0x0A, 0x0B, 0x24, 0x25,
                                   0x18, 0x18, 0x26, 0x27, 0x18, 0x18,
                                   0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
                                   0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
                                   0x18, 0x18, 0x20, 0x21, 0x18, 0x18,
                                   0x18, 0x18);

            /* 5.19.21 Set GIP Option2 (D6h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETGIP2,
                                   0x1C, 0x1C, 0x1D, 0x1D, 0x07, 0x06,
                                   0x05, 0x04, 0x03, 0x02, 0x01, 0x00,
                                   0x0B, 0x0A, 0x09, 0x08, 0x21, 0x20,
                                   0x18, 0x18, 0x27, 0x26, 0x18, 0x18,
                                   0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
                                   0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
                                   0x18, 0x18, 0x25, 0x24, 0x18, 0x18,
                                   0x18, 0x18);

            /* 5.19.25 SETGAMMA: Set gamma curve related setting (E0h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETGAMMA,
                                   0x00, 0x0A, 0x15, 0x1B, 0x1E, 0x21,
                                   0x24, 0x22, 0x47, 0x56, 0x65, 0x66,
                                   0x6E, 0x82, 0x88, 0x8B, 0x9A, 0x9D,
                                   0x98, 0xA8, 0xB9, 0x5D, 0x5C, 0x61,
                                   0x66, 0x6A, 0x6F, 0x7F, 0x7F, 0x00,
                                   0x0A, 0x15, 0x1B, 0x1E, 0x21, 0x24,
                                   0x22, 0x47, 0x56, 0x65, 0x65, 0x6E,
                                   0x81, 0x87, 0x8B, 0x98, 0x9D, 0x99,
                                   0xA8, 0xBA, 0x5D, 0x5D, 0x62, 0x67,
                                   0x6B, 0x72, 0x7F, 0x7F);

            /* Unknown command, not listed in the HX8394-F datasheet (C0H) */
            DSI_DCS_WriteSequence(HX8394_CMD_UNKNOWN1,
                                   0x1f, 0x73);

            /* Set CABC control (C9h)*/
            DSI_DCS_WriteSequence(HX8394_CMD_SETCABC,
                                   0x76, 0x00, 0x30);

            /* 5.19.17 SETPANEL (CCh) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETPANEL,
                                   0x0b);

            /* Unknown command, not listed in the HX8394-F datasheet (D4h) */
            DSI_DCS_WriteSequence(HX8394_CMD_UNKNOWN3,
                                   0x02);

            /* 5.19.11 Set register bank (BDh) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETREGBANK,
                                   0x02);

            /* 5.19.11 Set register bank (D8h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETGIP3,
                                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF);

            /* 5.19.11 Set register bank (BDh) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETREGBANK,
                                   0x00);

            /* 5.19.11 Set register bank (BDh) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETREGBANK,
                                   0x01);

            /* 5.19.2 SETPOWER: Set power (B1h) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETPOWER,
                                   0x00);

            /* 5.19.11 Set register bank (BDh) */
            DSI_DCS_WriteSequence(HX8394_CMD_SETREGBANK,
                                   0x00);

            /* Unknown command, not listed in the HX8394-F datasheet (C6h) */
            DSI_DCS_WriteSequence(HX8394_CMD_UNKNOWN2,
                                   0xed);

            DSI_DCS_WriteSequence(HX8394_DCS_EXIT_SLEEP_MODE);

            DSI_DCS_WriteSequence(HX8394_DCS_SET_DISPLAY_ON);

            DSI_VideoMode();

            /* Next State */
            disp_data.state = DISP_STATE_IDLE;

            break;
        }

        case DISP_STATE_IDLE:
        {
            break;
        }

        default:
        {
            break;
        }
    }
}
