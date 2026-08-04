/**
 * @file drv_gfx_EPD_MIKROE_2n13_local.h
 * @brief Information for Mikore 2.13" EPD displays
 * @details
 *   This file is derived from material supplied by the display manufacturer
 *   by email 29 July 2025 and updated 01 August 2025.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#include <stdint.h>

#ifndef DRV_GFX_EPD_MIKRO_2n13_LOCAL_H
#define DRV_GFX_EPD_MIKRO_2n13_LOCAL_H

#include "gfx/interface/drv_gfx_disp_intf.h"
#include "../../drv_gfx_EPD_controller_local.h"
typedef uint32_t err_t;

/**
 * \defgroup error_code Error Code
 * \{
 */
#define MIKROE_2n13_OK                                          0
#define MIKROE_2n13_ERROR                                      -1
/** \} */

/**
 * \defgroup display_resolution Display resolution
 * \{
 */
#define MIKROE_2n13_DISPLAY_WIDTH                               122
#define MIKROE_2n13_DISPLAY_HEIGHT                              250
#define MIKROE_2n13_DISPLAY_RESOLUTIONS                         4000
/** \} */

/**
 * \defgroup display_communication Display Communication
 * \{
 */
#define MIKROE_2n13_COMMUNICATION_SPI                           0
#define MIKROE_2n13_COMMUNICATION_I2C                           1
/** \} */

/**
 * \defgroup fill_screen_color Fill screen color
 * \{
 */
#define MIKROE_2n13_SCREEN_COLOR_WHITE                          0xFF
#define MIKROE_2n13_SCREEN_COLOR_BLACK                          0x00
#define MIKROE_2n13_SCREEN_COLOR_LIGHT_GREY                     0xAA
#define MIKROE_2n13_SCREEN_COLOR_DARK_GREY                      0x55

#define MIKROE_2n13_FO_HORIZONTAL                               0x00
#define MIKROE_2n13_FO_VERTICAL                                 0x01
#define MIKROE_2n13_FO_VERTICAL_COLUMN                          0x02
/** \} */

/**
 * \defgroup display_commands Display Commands
 * \{
 */
#define MIKROE_2n13_CMD_DRIVER_OUTPUT_CONTROL                   0x01
#define MIKROE_2n13_CMD_BOOSTER_SOFT_START_CONTROL              0x0C
#define MIKROE_2n13_CMD_GATE_SCAN_START_POSITION                0x0F
#define MIKROE_2n13_CMD_DEEP_SLEEP_MODE                         0x10
#define MIKROE_2n13_CMD_DATA_ENTRY_MODE_SETTING                 0x11
#define MIKROE_2n13_CMD_SW_RESET                                0x12
#define MIKROE_2n13_CMD_TEMPERATURE_SENSOR_CONTROL              0x1A
#define MIKROE_2n13_CMD_MASTER_ACTIVATION                       0x20
#define MIKROE_2n13_CMD_DISPLAY_UPDATE_CONTROL_1                0x21
#define MIKROE_2n13_CMD_DISPLAY_UPDATE_CONTROL_2                0x22
#define MIKROE_2n13_CMD_WRITE_RAM                               0x24
#define MIKROE_2n13_CMD_WRITE_VCOM_REGISTER                     0x2C
#define MIKROE_2n13_CMD_WRITE_LUT_REGISTER                      0x32
#define MIKROE_2n13_CMD_SET_DUMMY_LINE_PERIOD                   0x3A
#define MIKROE_2n13_CMD_SET_GATE_TIME                           0x3B
#define MIKROE_2n13_CMD_BORDER_WAVEFORM_CONTROL                 0x3C
#define MIKROE_2n13_CMD_SET_RAM_X_ADDRESS_START_END_POSITION    0x44
#define MIKROE_2n13_CMD_SET_RAM_Y_ADDRESS_START_END_POSITION    0x45
#define MIKROE_2n13_CMD_SET_RAM_X_ADDRESS_COUNTER               0x4E
#define MIKROE_2n13_CMD_SET_RAM_Y_ADDRESS_COUNTER               0x4F
#define MIKROE_2n13_CMD_TERMINATE_FRAME_READ_WRITE              0xFF
/** \} */

/** \} */ // End group macro 
// --------------------------------------------------------------- PUBLIC TYPES
/**
 * \defgroup type Types
 * \{
 */

typedef struct 
{
    const uint8_t *p_font;
    uint16_t      color;
    uint8_t       orientation;
    uint16_t      first_char;
    uint16_t      last_char;
    uint16_t      height;

} MIKROE_2n13_Font_t;

typedef struct 
{
    uint16_t x;
    uint16_t y;  

} MIKROE_2n13_cord_t;

typedef struct 
{
    uint16_t x_start;
    uint16_t y_start;
    uint16_t x_end;
    uint16_t y_end;

} MIKROE_2n13_xy_t;

typedef struct 
{
    uint8_t n_char;
    uint16_t text_x; 
    uint16_t text_y;

} MIKROE_2n13_set_text_t;


/**
 * @brief Click configuration structure definition.
 * @note The original example contained all transfer parameters
 *       but we keep only one to define the structure.
 */
typedef struct
{
    // static variable 
    uint32_t spi_speed;
} MIKROE_2n13_cfg_t;

/* Rename for encapsulation */
#define MIKROE_2n13_t   GFX_Disp_Intf

/** \} */ // End types group
// ----------------------------------------------- PUBLIC FUNCTION DECLARATIONS

/**
 * \defgroup public_function Public function
 * \{
 */
#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief Config Object Initialization function.
 *
 * @param cfg  Click configuration structure.
 *
 * @details This function initializes Click configuration structure to init state.
 * @note All used pins will be set to unconnected state.
 */
extern void MIKROE_2n13_cfg_setup ( MIKROE_2n13_cfg_t *cfg );

/**
 * @brief Initialization function.
 *
 * @param ctx Click object.
 * @param cfg Click configuration structure.
 * 
 * @details This function initializes all necessary pins and peripherals used for this Click.
 */
extern err_t MIKROE_2n13_init ( MIKROE_2n13_t *ctx, MIKROE_2n13_cfg_t *cfg );

/**
 * @brief Function for sending a command
 *
 * @param ctx          Click object.
 * @param command      The command to be sent
 */
extern void MIKROE_2n13_send_cmd ( MIKROE_2n13_t *ctx, uint8_t command );

/**
 * @brief Function for sending data
 *
 * @param ctx          Click object.
 * @param c_data       The data to be sent
 */
extern void MIKROE_2n13_send_data(const MIKROE_2n13_t * const ctx, uint8_t c_data );

/**
 * @brief Function for reset chip
 * 
 * @param ctx          Click object.
 */
#define MIKROE_2n13_reset(ctx) DRV_EPD_Reset()

/**
 * @brief Function for go to sleep mode
 * 
 * @param ctx          Click object. 
 */
extern void MIKROE_2n13_sleep_mode ( MIKROE_2n13_t *ctx );

/**
 * @brief Set LUT table
 *
 * @param ctx          Click object.
 * @param lut          Lut table
 * @param n_bytes      Number of bytes in Lut table
 *
 * @Note Changing the "lut table" can change the display's performance.
 */
extern void MIKROE_2n13_set_lut(const MIKROE_2n13_t * const ctx, 
                               const uint8_t * lut, uint8_t n_bytes );

/**
 * @brief Function for setting pointers in memory
 *
 * @param ctx          Click object.
 * @param x            x position
 * @param y            y position
 */
extern void MIKROE_2n13_set_mem_pointer(const MIKROE_2n13_t * const ctx, 
                                        uint16_t x, uint16_t y );

/**
 * @brief Function for setting area in memory
 * 
 * @param ctx          Click object.
 * @param xy           Struct object
 */
extern void MIKROE_2n13_set_mem_area(const MIKROE_2n13_t * const ctx, 
                      const MIKROE_2n13_xy_t * const xy );

/**
 * @brief Functions for update display
 * 
 * @param ctx          Click object.
 */
extern void MIKROE_2n13_update_display(const MIKROE_2n13_t * const ctx );

/**
 * @brief Function that fills the screen
 *
 * @param ctx          Click object.
 * @param color        The color to which the screen will be colored
 *
 * @note Options :
      MIKROE_2n13_SCREEN_COLOR_WHITE
      MIKROE_2n13_SCREEN_COLOR_BLACK
      MIKROE_2n13_SCREEN_COLOR_LIGHT_GREY
      MIKROE_2n13_SCREEN_COLOR_DARK_GREY
 */
extern void MIKROE_2n13_fill_screen(const MIKROE_2n13_t * const ctx, uint8_t color );

/**
 * @brief Display image function
 *
 * @param ctx          Click object.
 * @param image       Buffer containing the image
 *
 * @details The image can be built from VTFT or image2lcd programs
 * @note  The image type must be monochrome bmp
 */
extern void MIKROE_2n13_display_image(const MIKROE_2n13_t * const ctx, 
                            DRV_transferFunction_t * const transfer, 
                                     const uint8_t * image_buffer );

/**
 * @brief Function for draw text on the screen
 *
 * @param ctx          Click object. 
 * @param text         Text buffer
 * @param text_set     Struct object
 *
 */
extern void MIKROE_2n13_text ( MIKROE_2n13_t *ctx, uint8_t *text, MIKROE_2n13_set_text_t *text_set );

/**
 * @brief Set font function
 *
 * @param ctx                Click object.
 * @param cfg_font           Struct object
 */
extern void MIKROE_2n13_set_font ( MIKROE_2n13_t *ctx, MIKROE_2n13_Font_t *cfg_font );

typedef int DRV_transferFunction_t(GFX_Disp_Intf, uint8_t *, int);

#ifdef __cplusplus
}
#endif

#endif // DRV_GFX_EPD_MIKRO_2n13_LOCAL_H
