// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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


/*******************************************************************************
 Input driver for the Peripheral Touch Controller Library

  File Name:
    drv_touch_ptc.c

  Summary:
    Input driver for the peripheral touch controller.

  Description:
    This file consist the input driver implementation for the peripheral touch
    controller. This driver fetches the touch input information from the PTC
    library and sends the input events to the input system service.
 ******************************************************************************/


#include "definitions.h"
#include "drv_touch_ptc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if (PTC_SURFACE_CS_RESOLUTION_BITS == 12)
#define MAX_POS_VALUE 4095
#elif (PTC_SURFACE_CS_RESOLUTION_BITS == 10)
#define MAX_POS_VALUE 1023
#elif (PTC_SURFACE_CS_RESOLUTION_BITS == 8)
#define MAX_POS_VALUE 255
#else
#error "Invalid CS resolution"
#endif

typedef struct 
{
    int xpos;
    int ypos;
    DRV_TOUCH_PTC_TOUCH_STATE touch_state;
    DRV_TOUCH_PTC_STATE drv_state;
} DRV_TOUCH_PTC_OBJ;

DRV_TOUCH_PTC_OBJ drv_touch_ptc_obj;

extern qtm_surface_cs_control_t qtm_surface_cs_control1;
extern volatile uint8_t time_to_measure_touch_var;

//Wrapper function that returns true if there is an active touch input
static inline bool drv_touch_ptc_get_surface_active_status(void)
{
    return qtm_surface_cs_control1.qtm_surface_contact_data[0].qt_surface_status & TOUCH_ACTIVE;
}

static inline uint8_t drv_touch_ptc_get_surface_data_x_value(void)
{
#ifdef FLIP_X
    return (MAX_POS_VALUE - qtm_surface_cs_control1.qtm_surface_contact_data->h_position);
#else
    return qtm_surface_cs_control1.qtm_surface_contact_data->h_position;
#endif
}

static inline uint8_t drv_touch_ptc_get_surface_data_y_value(void)
{
#ifdef FLIP_Y
    return (MAX_POS_VALUE - qtm_surface_cs_control1.qtm_surface_contact_data->v_position);
#else
    return qtm_surface_cs_control1.qtm_surface_contact_data->v_position;
#endif
}

void drv_touch_ptc_init (void)
{
    drv_touch_ptc_obj.drv_state = DRV_TOUCH_PTC_INIT;
    drv_touch_ptc_obj.touch_state = DRV_TOUCH_PTC_TOUCH_RELEASED;
    drv_touch_ptc_obj.xpos = 0;
    drv_touch_ptc_obj.ypos = 0;
}

void drv_touch_ptc_task (void)
{
    switch (drv_touch_ptc_obj.drv_state)
    {
        case DRV_TOUCH_PTC_INIT:
        {
            drv_touch_ptc_obj.drv_state = DRV_TOUCH_PTC_CHECK;
            break;
        }
        case DRV_TOUCH_PTC_CHECK:
        {
            touch_process();
            
            if (drv_touch_ptc_get_surface_active_status() != 0)
            {
                drv_touch_ptc_obj.drv_state = DRV_TOUCH_PTC_PROCESS;
            }
            else
            {
                if (drv_touch_ptc_obj.touch_state == DRV_TOUCH_PTC_TOUCH_PRESSED)
                {
                    drv_touch_ptc_obj.touch_state = DRV_TOUCH_PTC_TOUCH_RELEASED;
                    SYS_INP_InjectTouchUp(PTC_TOUCH_ID, 
                                          drv_touch_ptc_obj.xpos,
                                          drv_touch_ptc_obj.ypos);
                }
            }
            
            break;
        }
        
        case DRV_TOUCH_PTC_PROCESS:
        {
            int x, y, delta;
            
            x = drv_touch_ptc_get_surface_data_x_value();
            y = drv_touch_ptc_get_surface_data_y_value();
            
            delta = x - (MAX_POS_VALUE/2);
            delta = (delta * TOUCH_SCREEN_ACTIVE_WIDTH)/MAX_POS_VALUE;
            x = TOUCH_SCREEN_ACTIVE_WIDTH/2 + delta; 

            delta = y - (MAX_POS_VALUE/2);
            delta = (delta * TOUCH_SCREEN_ACTIVE_HEIGHT)/MAX_POS_VALUE;
            y = TOUCH_SCREEN_ACTIVE_HEIGHT/2 + delta;
            
            if (drv_touch_ptc_obj.touch_state == DRV_TOUCH_PTC_TOUCH_PRESSED)
            {
                //check if moved
                if ((x != drv_touch_ptc_obj.xpos) ||
                    (y != drv_touch_ptc_obj.ypos))
                {
                    drv_touch_ptc_obj.xpos = x;
                    drv_touch_ptc_obj.ypos = y;
                    
                    SYS_INP_InjectTouchMove(PTC_TOUCH_ID,
                                            drv_touch_ptc_obj.xpos,
                                            drv_touch_ptc_obj.ypos);
                }
            }
            else if (drv_touch_ptc_obj.touch_state == DRV_TOUCH_PTC_TOUCH_RELEASED)
            {
                drv_touch_ptc_obj.touch_state = DRV_TOUCH_PTC_TOUCH_PRESSED;
                
                drv_touch_ptc_obj.xpos = x;
                drv_touch_ptc_obj.ypos = y;
                
                SYS_INP_InjectTouchDown(PTC_TOUCH_ID,
                                        drv_touch_ptc_obj.xpos,
                                        drv_touch_ptc_obj.ypos);
            }
            
            drv_touch_ptc_obj.drv_state = DRV_TOUCH_PTC_CHECK;
            
            break;
        }
            
        default:
            break;
    }
}