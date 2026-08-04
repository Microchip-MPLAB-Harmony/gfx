/** 
 * @file drv_gfx_EPD_MIKROE_2n13.c
 * @brief Custom controller for Mikroe 2.13" EPD displays
 * @details
 *    This file contains the source code for using the
 *    UC8253 controller with Legato.
 * 
 * Note that this is an older controller
 * which is not compatible with the more recent V3 nor V4 variants.
 * From: https://github.com/MikroElektronika/mikrosdk_click_v2
 * Extracted from .\clicks\eink213inch\example\main.c
 *
 * The data transfer operations send_cmd() and send_data() are
 * somewhat transliterated from the original source code.
 * Accordingly, command operations are not optimal on the SPI bus.
 */

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2026 Microchip Technology Inc. and its subsidiaries.
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

#include "./drv_gfx_EPD_MIKROE_2n13_local.h"
#include "./drv_gfx_EPD_MIKROE_2n13_lut.h"
#include "../../drv_gfx_EPD_ioctl.h"
#include "../../drv_gfx_EPD_controller_local.h"

#include "system/time/sys_time.h"

#if ENABLE_HALF_DUPLEX
#include "../../PIC32CM_magic/PIC32CM_SPI_helpers.h"
#endif

#include <stdint.h>

#define ALENGTH(x)  (sizeof(x)/sizeof(x[0]))

// ------------------------------------------------------------- PRIVATE MACROS 

#define MIKROE_2n13_DUMMY 0

#define NOP()   __builtin_nop()

#define MIKROE_delayMS(ms)   DRV_EPD_DelayMS(ms)
#define Delay_1ms()     MIKROE_delayMS(1)
#define Delay_80ms()    MIKROE_delayMS(80)
#define Delay_100ms()   MIKROE_delayMS(100)

#define MIKROE_2n13_NCSAssert(intf)   DRV_EPD_NCSAssert(intf)
#define MIKROE_2n13_NCSDeassert(intf) DRV_EPD_NCSDeassert(intf)

#define IMAGE_MODE_ONLY // suppress character rendering

// ---------------------------------------------- PRIVATE FUNCTION DECLARATIONS 

static void wait_until_idle(const MIKROE_2n13_t * const ctx );
static void display_delay(void);

#ifndef IMAGE_MODE_ONLY
static void frame_px ( MIKROE_2n13_t *ctx, uint16_t x, uint16_t y, uint8_t font_col );
static void char_wr ( MIKROE_2n13_t *ctx, uint16_t ch_idx );
#endif


/** Send a single command byte to the display
 * @note The original code sends bytes individually. */
#define MIKROE_2n13_send_cmd(ctx,command)   \
                    GFX_Disp_Intf_WriteCommand(*ctx,command)

/** Send a single byte of data to the display
 * @details Encapsulation allows constant data values 
 *          to be sent even though the framework requires a pointer.
 * @note The original code sends bytes individually. */
inline void MIKROE_2n13_send_data(const MIKROE_2n13_t * const ctx, uint8_t data )
{
    GFX_Disp_Intf_WriteData(*ctx,(uint8_t*)&data,1);
}

#if ENABLE_HALF_DUPLEX
/** Read data from the EPD display
 * @param ctx display interface context
 * @param data pointer to receive buffer
 * @param bytes number of bytes to fetch
 * @details This function is not normally available for EPD displays
 *          because it requires a half-duplex (one-data-wire) interface.
 * @note This may be considered experimental */
void MIKROE_2n13_get_data(GFX_Disp_Intf * ctx, uint8_t * data, int bytes)
{
    /* The DI signal wire can be noisy while it is tristated,
     * so try to get this done quickly. */
    PIC32CM_set_SPI_DxPO(&SERCOM_REGS->SPIM, 
            PIC32CM_SPI_DOPO_tristate, PIC32CM_SPI_DIPO_PAD0);
    GFX_Disp_Intf_ReadData(*ctx, data, bytes);
    PIC32CM_set_SPI_DxPO(&SERCOM_REGS->SPIM, 
            PIC32CM_SPI_DOPO_drive, PIC32CM_SPI_DIPO_PAD0);
}
#endif

/* EV80P12A Curiosity PCB info:
 * PIC32CM5164LE00100 100p i/o group C
 * PB00  97 CS      GPIO (SERCOM3.PAD2)
 * PB01  98 MISO    SERCOM3.PAD3
 * PB02  99 MOSI    SERCOM3.PAD0
 * PB03 100 SCK     SERCOM3.PAD1
 *    */

 #if ENABLE_HALF_DUPLEX
 /** Request and read data from the EPD display
 * @param ctx display interface context
 * @param command request for specific data
 * @param data pointer to receive buffer
 * @param bytes number of bytes to fetch
 * @details This function is not normally available for EPD displays
 *          because it requires a half-duplex (one-data-wire) interface.
 * @note This may be considered experimental */
void MIKROE_2n13_commandReadData(MIKROE_2n13_t * ctx, 
        uint8_t command, uint8_t * buffer, uint8_t bytes)
{
    MIKROE_2n13_send_cmd( ctx, command /* OTP register read */ );
    MIKROE_2n13_get_data( ctx, buffer, bytes);
}
#endif

#if ENABLE_HALF_DUPLEX
static struct // organized by command code
{
    uint8_t c2F[1];  // status
    uint8_t c2E[10]; // user ID (probably zero))
    uint8_t c2D[11]; // display options
    uint8_t c35[2];  // CRC
} MIKROE_2n13_info;
#define B(b)    b,ALENGTH(b)
static void MIKROE_2n13_gatherDisplayInfo(MIKROE_2n13_t *ctx)
{
    MIKROE_2n13_commandReadData(ctx, 0x2f, B(MIKROE_2n13_info.c2F));
    MIKROE_2n13_commandReadData(ctx, 0x2e, B(MIKROE_2n13_info.c2E));
    MIKROE_2n13_commandReadData(ctx, 0x2d, B(MIKROE_2n13_info.c2D));
    MIKROE_2n13_commandReadData(ctx, 0x35, B(MIKROE_2n13_info.c35));
    NOP(); NOP(); NOP();
}
#endif


void MIKROE_2n13_sleep_mode ( MIKROE_2n13_t *ctx )
{
    MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_DEEP_SLEEP_MODE );
    wait_until_idle( ctx );
}

/** Send the LUT table to configure the display
 * @details Only one LUT table is provided in the original source
 *          code so there are no options for fast operation. */
void MIKROE_2n13_set_lut(const MIKROE_2n13_t * const ctx, 
                               const uint8_t *lut, uint8_t n_bytes )
{
    uint8_t cnt;
    
    MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_WRITE_LUT_REGISTER );
    
    for ( cnt = 0; cnt < n_bytes; cnt++ )
    {
        MIKROE_2n13_send_data( ctx, lut[ cnt ] );
    }
}


void MIKROE_2n13_start_config ( MIKROE_2n13_t *ctx )
{

#if ENABLE_HALF_DUPLEX    
    /* Preset the SPI pads for DO=PAD0 and DI=PAD0
     * This is dependant on PIC32 device, SERCOMx.SPI and the PCB
     * The purpose of this line is to restore normal writing
     * even if a read operation was interrupted by debugger reset. */
    PIC32CM_set_SPI_DxPO(&SERCOM_REGS->SPIM, 
            PIC32CM_SPI_DOPO_drive, PIC32CM_SPI_DIPO_PAD0);

    /* The purpose of this line is to prove that reading works */
    MIKROE_2n13_gatherDisplayInfo(ctx);
#endif
    
    MIKROE_2n13_reset( ctx );
    MIKROE_2n13_send_cmd( ctx, /* 0x01 */ MIKROE_2n13_CMD_DRIVER_OUTPUT_CONTROL );
    MIKROE_2n13_send_data( ctx, ( ( MIKROE_2n13_DISPLAY_HEIGHT - 1 ) & 0xFF ) );
    MIKROE_2n13_send_data( ctx, ( ( ( MIKROE_2n13_DISPLAY_HEIGHT - 1 ) >> 8 ) & 0xFF ) );   

    MIKROE_2n13_send_cmd( ctx, /* 0x3a */ MIKROE_2n13_CMD_SET_DUMMY_LINE_PERIOD );                    
    MIKROE_2n13_send_data( ctx, 0x06 );
    MIKROE_2n13_send_cmd( ctx, /* 0x3b */ MIKROE_2n13_CMD_SET_GATE_TIME );                            
    MIKROE_2n13_send_data( ctx, 0x0B );
    
    MIKROE_2n13_send_cmd( ctx, /* 0x11 */ MIKROE_2n13_CMD_DATA_ENTRY_MODE_SETTING );                  
    MIKROE_2n13_send_data( ctx, 0x01 );
    
    MIKROE_2n13_send_cmd( ctx, /* 0x44 */ MIKROE_2n13_CMD_SET_RAM_X_ADDRESS_START_END_POSITION );     
    MIKROE_2n13_send_data( ctx, 0x00 );                                                     
    MIKROE_2n13_send_data( ctx, 0x0F );                                                     
    MIKROE_2n13_send_cmd( ctx, /* 0x45 */ MIKROE_2n13_CMD_SET_RAM_Y_ADDRESS_START_END_POSITION );     
    MIKROE_2n13_send_data( ctx, 0xF9 );                                                     
    MIKROE_2n13_send_data( ctx, 0x00 );                                                     

    MIKROE_2n13_send_cmd( ctx, /* 0x2c */ MIKROE_2n13_CMD_WRITE_VCOM_REGISTER );                      
    MIKROE_2n13_send_data( ctx, 0x4B ); 
    
    MIKROE_2n13_send_cmd( ctx, /* 0x3c */ MIKROE_2n13_CMD_BORDER_WAVEFORM_CONTROL );                  
    MIKROE_2n13_send_data( ctx, 0x33) ; 

    display_delay( );
}

void MIKROE_2n13_set_mem_pointer(const MIKROE_2n13_t * const ctx, 
                                          uint16_t x, uint16_t y )
{
    MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_SET_RAM_X_ADDRESS_COUNTER );
    MIKROE_2n13_send_data( ctx, ( ( x >> 3 ) & 0xFF ) );
    MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_SET_RAM_Y_ADDRESS_COUNTER );
    MIKROE_2n13_send_data( ctx, ( y & 0xFF ) );
    MIKROE_2n13_send_data( ctx, ( ( y >> 8 ) & 0xFF ) );
}

void MIKROE_2n13_set_mem_area(const MIKROE_2n13_t * const ctx, 
                      const MIKROE_2n13_xy_t * const xy )
{
    MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_SET_RAM_X_ADDRESS_START_END_POSITION );
    MIKROE_2n13_send_data( ctx, ( xy->x_start >> 3 ) & 0xFF );
    MIKROE_2n13_send_data( ctx, ( xy->x_end >> 3 ) & 0xFF );
    MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_SET_RAM_Y_ADDRESS_START_END_POSITION );
    MIKROE_2n13_send_data( ctx, xy->y_start & 0xFF );
    MIKROE_2n13_send_data( ctx, ( xy->y_start >> 8 ) & 0xFF );
    MIKROE_2n13_send_data( ctx, xy->y_end & 0xFF );
    MIKROE_2n13_send_data( ctx, ( xy->y_end >> 8 ) & 0xFF );
}

#define MIKROE_2n13_DISPLAY_UPDATE_MODE  0
void MIKROE_2n13_update_display(const MIKROE_2n13_t * const ctx )
{
    Delay_100ms( ); // commands 0x22+1 0x20 0xFF
    MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_DISPLAY_UPDATE_CONTROL_2 );
    
    if (0 == MIKROE_2n13_DISPLAY_UPDATE_MODE)
    { MIKROE_2n13_send_data( ctx, 0xC7 ); }
    else
    { MIKROE_2n13_send_data( ctx, 0xCF ); }

    MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_MASTER_ACTIVATION );
    MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_TERMINATE_FRAME_READ_WRITE );
    wait_until_idle( ctx );
}

void MIKROE_2n13_fill_screen(const MIKROE_2n13_t * const ctx, uint8_t color )
{
    uint16_t cnt_x;
    uint16_t cnt_y;
    MIKROE_2n13_xy_t xy;
    
    xy.x_start = 0;
    xy.y_start = 0; 
    xy.x_end = MIKROE_2n13_DISPLAY_WIDTH - 1;
    xy.y_end = MIKROE_2n13_DISPLAY_HEIGHT - 1;

    MIKROE_2n13_set_mem_area( ctx, &xy );
    for ( cnt_y = 0; cnt_y < MIKROE_2n13_DISPLAY_HEIGHT; cnt_y++ )
    {
        MIKROE_2n13_set_mem_pointer( ctx, 0, cnt_y );
        MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_WRITE_RAM );

        for ( cnt_x = 0; cnt_x < 16; cnt_x++ )
        {
            MIKROE_2n13_send_data( ctx, color );
        }
    }

    display_delay( );
    MIKROE_2n13_update_display( ctx );
}


#define BYTES_ACROSS_WIDTH  ((MIKROE_2n13_DISPLAY_WIDTH+7)/8)
void MIKROE_2n13_display_image(const MIKROE_2n13_t * const ctx, 
                            DRV_transferFunction_t * const transfer, 
                                     const uint8_t * image_buffer )
{
//    uint16_t cnt_x;
    uint16_t cnt_y;
//    uint16_t pos;
    MIKROE_2n13_xy_t xy;
    uint16_t remainingBytes = 
            (MIKROE_2n13_DISPLAY_WIDTH*MIKROE_2n13_DISPLAY_HEIGHT)/8;
    uint8_t * dataPointer = (uint8_t*)image_buffer;
    
    xy.x_start = 0;
    xy.y_start = 0; 
    xy.x_end = MIKROE_2n13_DISPLAY_WIDTH - 1;
    xy.y_end = MIKROE_2n13_DISPLAY_HEIGHT - 1;
    MIKROE_2n13_set_mem_area( ctx, &xy );
    
    for ( cnt_y = 0; cnt_y < MIKROE_2n13_DISPLAY_HEIGHT; cnt_y++ )
    {
        MIKROE_2n13_set_mem_pointer( ctx, 0, cnt_y );
        MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_WRITE_RAM );
        
        uint8_t bytesToWrite = (remainingBytes>BYTES_ACROSS_WIDTH)?
                    BYTES_ACROSS_WIDTH : remainingBytes;

        dataPointer += transfer(*ctx, dataPointer, bytesToWrite);
        remainingBytes -= bytesToWrite;
    }
    MIKROE_2n13_update_display( ctx );
}

#ifndef IMAGE_MODE_ONLY
void MIKROE_2n13_text ( MIKROE_2n13_t *ctx, uint8_t *text, MIKROE_2n13_set_text_t *text_set )
{
    uint16_t cnt;
    uint16_t cnt_x;
    uint16_t cnt_y;
    uint16_t pos;
    MIKROE_2n13_xy_t xy;
    
    if ( ( text_set->text_x >= MIKROE_2n13_DISPLAY_WIDTH ) || ( text_set->text_y >= MIKROE_2n13_DISPLAY_HEIGHT ) )
    {
        return;
    }
    
    xy.x_start = 0;
    xy.y_start = 0; 
    xy.x_end = MIKROE_2n13_DISPLAY_WIDTH - 1;
    xy.y_end = MIKROE_2n13_DISPLAY_HEIGHT - 1;
    
    ctx->dev_cord.x = text_set->text_x;
    ctx->dev_cord.y = text_set->text_y;

    for ( cnt = 0; cnt < text_set->n_char; cnt++ )
    {
        char_wr( ctx, text[ cnt ] );
    }

    MIKROE_2n13_set_mem_area( ctx, &xy );
    for ( cnt_y = 0; cnt_y < MIKROE_2n13_DISPLAY_HEIGHT; cnt_y++ )
    {
        MIKROE_2n13_set_mem_pointer( ctx, 0, cnt_y );
        MIKROE_2n13_send_cmd( ctx, MIKROE_2n13_CMD_WRITE_RAM );
        for ( cnt_x = 0; cnt_x < 16; cnt_x++ )
        {
            pos = cnt_x + ( cnt_y * 16 );
            MIKROE_2n13_send_data( ctx, ctx->frame[ pos ] );
        }
    }

    display_delay( );
    MIKROE_2n13_update_display( ctx );
}
void MIKROE_2n13_set_font ( MIKROE_2n13_t *ctx, MIKROE_2n13_Font_t *cfg_font )
{
    ctx->dev_font.p_font        = cfg_font->p_font;
    ctx->dev_font.first_char    = cfg_font->p_font[ 2 ] + ( cfg_font->p_font[ 3 ] << 8 );
    ctx->dev_font.last_char     = cfg_font->p_font[ 4 ] + ( cfg_font->p_font[ 5 ] << 8 );
    ctx->dev_font.height        = cfg_font->p_font[ 6 ];
    ctx->dev_font.color         = cfg_font->color;
    ctx->dev_font.orientation   = cfg_font->orientation;
}
#endif

// ----------------------------------------------- PRIVATE FUNCTION DEFINITIONS

static void wait_until_idle(const MIKROE_2n13_t * const ctx )
{
    uint8_t state;
    do
    {
        NOP(); NOP(); NOP();
//        state = digital_in_read( &ctx->bsy );
        state = nBUSY_Get(); // active LOW = BUSY
        if (state == 1)
            break;
        Delay_100ms( );
    } 
    while(1);
    NOP(); NOP(); NOP();
}

#ifndef IMAGE_MODE_ONLY
static void frame_px ( MIKROE_2n13_t *ctx, uint16_t x, uint16_t y, uint8_t font_col )
{
    uint16_t off;
    uint16_t pos;

    pos = ( y * ( MIKROE_2n13_DISPLAY_WIDTH / 8 ) ) + ( x / 4 );
    off = ( 3 - ( x % 4 ) ) * 2;

    ctx->frame[ pos ] &= ~( 0x03 << off );
    ctx->frame[ pos ] |= ( ( font_col & 0x03 ) << off );
}

static void char_wr ( MIKROE_2n13_t *ctx, uint16_t ch_idx )
{
    uint8_t   ch_width = 0;
    uint8_t   x_cnt;
    uint8_t   y_cnt;
    uint16_t  x = 0;
    uint16_t  y = 0;
    uint16_t  tmp;
    uint8_t   temp = 0;
    uint8_t   mask = 0;
    uint32_t  offset;
    const uint8_t *ch_table;
    const uint8_t *ch_bitmap;

    if ( ch_idx < ctx->dev_font.first_char )
    {
        return;
    }
    if ( ch_idx > ctx->dev_font.last_char )
    {
        return;
    }

    offset = 0;
    tmp = ( ch_idx - ctx->dev_font.first_char ) << 2;
    ch_table = ( const uint8_t* ) ( ctx->dev_font.p_font + ( 8 + tmp ) );
    ch_width = *ch_table;

    offset = ( uint32_t ) ch_table[ 1 ] + ( ( uint32_t ) ch_table [ 2 ] << 8 ) + ( ( uint32_t ) ch_table[ 3 ] << 16 );

    ch_bitmap = ctx->dev_font.p_font + offset;

    if ( ( ctx->dev_font.orientation == MIKROE_2n13_FO_HORIZONTAL ) ||
        ( ctx->dev_font.orientation == MIKROE_2n13_FO_VERTICAL_COLUMN ) )
    {
        y = ctx->dev_cord.y;
        for ( y_cnt = 0; y_cnt < ctx->dev_font.height; y_cnt++ )
        {
            x = ctx->dev_cord.x;
            mask = 0;
            for ( x_cnt = 0; x_cnt < ch_width; x_cnt++ )
            {
                if ( !mask )
                {
                    temp = *ch_bitmap++;
                    mask = 0x01;
                }

                if ( temp & mask )
                {
                    frame_px( ctx, x, y, ctx->dev_font.color );
                    Delay_80us( );
                }

                x++;
                mask <<= 1;
            }
            y++;
        }

        if ( ctx->dev_font.orientation == MIKROE_2n13_FO_HORIZONTAL )
        {
            ctx->dev_cord.x = x + 1;
        }
        else
        {
            ctx->dev_cord.y = y;
        }
    }
    else
    {
        y = ctx->dev_cord.x;

        for ( y_cnt = 0; y_cnt < ctx->dev_font.height; y_cnt++ )
        {
            x = ctx->dev_cord.y;
            mask = 0;

            for ( x_cnt = 0; x_cnt < ch_width; x_cnt++ )
            {
                if ( mask == 0 )
                {
                    temp = *ch_bitmap++;
                    mask = 0x01;
                }

                if ( temp & mask )
                {
                    frame_px( ctx, x, y, ctx->dev_font.color );
                    Delay_80us( );
                }

                x--;
                mask <<= 1;
            }
            y++;
        }
        ctx->dev_cord.y = x - 1;
    }
}
#endif

static void display_delay ( )
{
    Delay_1ms( );
    Delay_1ms( );
}


gfxResult MIKROE_2n13_blit(const GFX_Disp_Intf intf, 
             DRV_transferFunction_t * const transfer, // delegate
                     const avdisplay_load_t fullPartial, 
                   const avdisplay_buffer_t target,
    const int32_t x, const int32_t y, const gfxPixelBuffer * const buffer)
{
   if (buffer->mode != GFX_COLOR_MODE_MONOCHROME)
   { return GFX_FAILURE; }
    
    (void)target;
    MIKROE_2n13_NCSAssert(intf);

    /* This display requires that the given frame consists 
     * of whole bytes-full of pixel bits. The screen can't
     * handler partial bytes.
     * The Y dimension is not part of this decision. */
    if ((0 != (x%8)) || (7 != ((x+buffer->size.width-1)%8)))
    { return GFX_FAILURE; }
    
#if PIXEL_BUFFER_COLOR_MODE == GFX_COLOR_MODE_MONOCHROME
    if (0 != (x%8)) // must be byte-aligned horizontally
    { while(1); }
    NOP(); NOP(); NOP();
#endif
    
    if (AVDISPLAY_LOAD_NONE == fullPartial)
    {
        /* Simply write the image to the display. */
        // GFX_Disp_Intf_WriteCommand(intf, target);
        MIKROE_2n13_update_display(&intf);
    }
    else 
    {
        if (AVDISPLAY_LOAD_PARTIAL == fullPartial)
        {
            MIKROE_2n13_xy_t window = {
                .x_start = x,
                .x_end = x + buffer->size.width - 1,
//                .y_start = y, // the row in which we begin
//                .y_end = y + buffer->size.height - 1
            };
            
            uint8_t * b = (uint8_t*)buffer->pixels;
            
            /* The 2n13 does not wrap row addresses correctly 
             * so we only ever write to one specific row at a time.
             * Newer models might not have this inconvenience. */
            for (uint32_t image_row = 0; image_row < buffer->size.height; image_row++)
            {
                window.y_start = // y + image_row;
                window.y_end   = y + image_row;
                MIKROE_2n13_set_mem_area(&intf,&window);
                MIKROE_2n13_set_mem_pointer(&intf,window.x_start,window.y_start);

                MIKROE_2n13_send_cmd(&intf, MIKROE_2n13_CMD_WRITE_RAM);
                b += transfer(intf, b, (buffer->size.width+7u)/8u);
            }

            /* Unspeakable hack! 
             * If we just loaded the 250th row then we refresh. */
            if(MIKROE_2n13_DISPLAY_HEIGHT == y + buffer->size.height)
            { MIKROE_2n13_update_display(&intf); }
        }
        else if (AVDISPLAY_LOAD_FULL_DISPLAY == fullPartial)
        {
            MIKROE_2n13_display_image(&intf, transfer, buffer->pixels);
        }
    }
    
    /* Send data using the provided transfer mode. */
    
#if READ_STOP
    uint8_t stopData; (void)stopData;
    GFX_Disp_Intf_WriteCommand(intf, 0x11); // Display stop
    GFX_Disp_Intf_ReadData(intf,&stopData,1);
#endif

    /* Important: update the display while still in Partial mode
     * (if we're there at all), lest you do a full refresh. */
//    MIKROE_2n13_update_display(&intf);
    
    MIKROE_2n13_NCSDeassert(intf);
    return GFX_SUCCESS;
}
