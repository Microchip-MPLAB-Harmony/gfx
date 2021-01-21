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
  MPLAB Harmony Graphics Composer Generated Implementation File

  File Name:
    libaria_harmony.c

  Summary:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

  Description:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

#include "gfx/libaria/libaria_harmony.h"
#include "gfx/libaria/libaria_init.h"

#include "gfx/libaria/libaria.h"
<#if enableDemoMode?? && enableDemoMode == true>
#include "gfx/libaria/libaria_demo_mode.h"
</#if>

<#if useRTOSExtensions == true>
#include "gfx/libaria/inc/libaria_context_rtos.h"
#include "gfx/libaria/inc/libaria_input_rtos.h"
#include "gfx/libaria/libaria_rtos.h"
</#if>

<#if enableInput?? && enableInput == true>
#include "system/input/sys_input.h"

SYS_INP_InputListener inputListener;

static void touchDownHandler(const SYS_INP_TouchStateEvent* const evt);
static void touchUpHandler(const SYS_INP_TouchStateEvent* const evt);
static void touchMoveHandler(const SYS_INP_TouchMoveEvent* const evt);
</#if>

/*** libaria Object Global ***/
libaria_objects libariaObj;
static LIBARIA_STATES libariaState;

<#if genAriaMediaIntf?? && genAriaMediaIntf == true>
GFXU_MemoryIntf memIntf;

static GFX_Result LibAria_MediaOpenRequest(GFXU_AssetHeader* asset);

static GFX_Result LibAria_MediaReadRequest(GFXU_ExternalAssetReader* reader,
                                           GFXU_AssetHeader* asset,
                                           void* address,
                                           uint32_t readSize,
                                           uint8_t* destBuffer,
                                           GFXU_MediaReadRequestCallback_FnPtr cb);

static void LibAria_MediaCloseRequest(GFXU_AssetHeader* asset);

<#if ARIA_MEDIA_OPEN_FUNCTION?? && ARIA_MEDIA_OPEN_FUNCTION?has_content>
GFX_Result ${ARIA_MEDIA_OPEN_FUNCTION}(GFXU_AssetHeader* asset);
</#if>

<#if ARIA_MEDIA_READ_FUNCTION?? && ARIA_MEDIA_READ_FUNCTION?has_content>
GFX_Result ${ARIA_MEDIA_READ_FUNCTION}(GFXU_ExternalAssetReader* reader,
        GFXU_AssetHeader* asset,
        void* address,
        uint32_t readSize,
        uint8_t* destBuffer,
        GFXU_MediaReadRequestCallback_FnPtr cb);
</#if>

<#if ARIA_MEDIA_CLOSE_FUNCTION?? && ARIA_MEDIA_CLOSE_FUNCTION?has_content>
void ${ARIA_MEDIA_CLOSE_FUNCTION}(GFXU_AssetHeader* asset);
</#if>

</#if>
int32_t LibAria_Initialize(void)
{
    if(laInitialize() == LA_FAILURE)
        return -1;

<#if genAriaMediaIntf?? && genAriaMediaIntf == true>
    memIntf.heap.malloc = (GFX_Malloc_FnPtr) &malloc;
    //memIntf.heap.coherent_alloc = (GFX_Malloc_FnPtr) &__pic32_alloc_coherent; // FIXME for H3
    memIntf.heap.coherent_alloc = (GFX_Malloc_FnPtr) &malloc;
    memIntf.heap.calloc = (GFX_Calloc_FnPtr) &calloc;
    memIntf.heap.free = (GFX_Free_FnPtr) &free;
    memIntf.heap.coherent_free = (GFX_Free_FnPtr) &free;
    //memIntf.heap.coherent_free = (GFX_Free_FnPtr) &__pic32_free_coherent; // FIXME for H3

    memIntf.heap.memcpy = (GFX_Memcpy_FnPtr) &memcpy;
    memIntf.heap.memset = (GFX_Memset_FnPtr) &memset;
    memIntf.heap.realloc = (GFX_Realloc_FnPtr) &realloc;
    memIntf.open = (GFXU_MediaOpenRequest_FnPtr) &LibAria_MediaOpenRequest;
    memIntf.read = (GFXU_MediaReadRequest_FnPtr) &LibAria_MediaReadRequest;
    memIntf.close = (GFXU_MediaCloseRequest_FnPtr) &LibAria_MediaCloseRequest;

<#if ARIA_CONTEXT_COLOR_MODE??>
    libariaObj.context = laContext_Create(0, 0, 0, ${ARIA_CONTEXT_COLOR_MODE}, &memIntf);
<#else>
    libariaObj.context = laContext_Create(0, 0, 0, GFX_COLOR_MODE_RGB_565, &memIntf);
</#if>

<#else>
<#if ARIA_CONTEXT_COLOR_MODE??>
    libariaObj.context = laContext_Create(0, 0, 0, ${ARIA_CONTEXT_COLOR_MODE}, NULL);
<#else>
    libariaObj.context = laContext_Create(0, 0, 0, GFX_COLOR_MODE_RGB_565, NULL);
</#if>

</#if>
    if(libariaObj.context == NULL)
        return -1;

    laContext_SetActive(libariaObj.context);

<#if useRTOSExtensions == true>
    laContext_Create_RTOS(libariaObj.context);
</#if>

<#if genAriaDesign?? && genAriaDesign == true>
    libaria_initialize(); // use auto-generated initialization functions
</#if>

<#if enableInput?? && enableInput == true>
    inputListener.handleTouchDown = &touchDownHandler;
    inputListener.handleTouchUp = &touchUpHandler;
    inputListener.handleTouchMove = &touchMoveHandler;
</#if>

    libariaState = LIBARIA_STATE_INIT;

    return 0;
}

void LibAria_Tasks(void)
{
    switch(libariaState)
    {
        case LIBARIA_STATE_INIT:
        {
<#if preProcessAssetsCode??>
            LIBARIA_STATES nextState = LIBARIA_STATE_PREPROCESS_ASSETS;
<#else>
            LIBARIA_STATES nextState = LIBARIA_STATE_RUNNING;
</#if>

<#if enableInput?? && enableInput == true>
            SYS_INP_AddListener(&inputListener);

            libariaState = nextState;

            break;
<#else>
            libariaState = nextState;
            break;
</#if>
        }
<#if preProcessAssetsCode??>
        case LIBARIA_STATE_PREPROCESS_ASSETS:
        {
            laResult retval = LA_SUCCESS;
            
            if (libariaObj.context->preprocessStateChangedCB != NULL)
            {
                retval = libariaObj.context->preprocessStateChangedCB
                                   (LA_CONTEXT_PREPROCESS_ASSETS_PENDING);
            }
            
            if (retval == LA_SUCCESS)
            {
                if (libariaObj.context->preprocessStateChangedCB != NULL)
                    libariaObj.context->preprocessStateChangedCB
                                (LA_CONTEXT_PREPROCESS_ASSETS_IN_PROGRESS);

                libaria_preprocess_assets();

                if (libariaObj.context->preprocessStateChangedCB != NULL)
                    libariaObj.context->preprocessStateChangedCB
                                (LA_CONTEXT_PREPROCESS_ASSETS_DONE);
            
                libariaState = LIBARIA_STATE_RUNNING;
            }
            else
            {
                //Call update to allow screen updates while there is a pending 
                //pre-process operation
                laUpdate(0);
            }
            
            break;
        }
</#if>
        case LIBARIA_STATE_RUNNING:
        {
            laContext_SetActive(libariaObj.context);

            <#if enableDemoMode?? && enableDemoMode == true>
            LibAria_DemoModeProcessEvents();
            </#if>

<#if useRTOSExtensions == true>
<#if rtosFullBlockingMode == true>
            laUpdate_RTOS(LA_TRUE, 0);
<#else>
            laUpdate_RTOS(LA_FALSE, 0);
</#if>
<#else>
            laUpdate(0);
</#if>

            break;
        }

        default:
        {
            break;
        }
    }
}

<#if enableInput?? && enableInput == true>
void touchDownHandler(const SYS_INP_TouchStateEvent* const evt)
{
<#if useRTOSExtensions == true>
    laInput_SendTouchDown_Ext_RTOS(evt->index, evt->x, evt->y);
<#else>
    laInput_InjectTouchDown(evt->index, evt->x, evt->y);
</#if>
}

void touchUpHandler(const SYS_INP_TouchStateEvent* const evt)
{
<#if useRTOSExtensions == true>
    laInput_SendTouchUp_Ext_RTOS(evt->index, evt->x, evt->y);
<#else>
    laInput_InjectTouchUp(evt->index, evt->x, evt->y);
</#if>
}

void touchMoveHandler(const SYS_INP_TouchMoveEvent* const evt)
{
<#if useRTOSExtensions == true>
    laInput_SendTouchMoved_Ext_RTOS(evt->index, evt->x, evt->y);
<#else>
    laInput_InjectTouchMoved(evt->index, evt->x, evt->y);
</#if>
}
</#if>

<#if genAriaMediaIntf?? && genAriaMediaIntf == true>

static GFX_Result LibAria_MediaOpenRequest(GFXU_AssetHeader* asset)
{
<#if ARIA_MEDIA_OPEN_FUNCTION?? && ARIA_MEDIA_OPEN_FUNCTION?has_content>
    return ${ARIA_MEDIA_OPEN_FUNCTION}(asset);
<#else>
    return GFX_FAILURE;
</#if>
}

static GFX_Result LibAria_MediaReadRequest(GFXU_ExternalAssetReader* reader,
                                           GFXU_AssetHeader* asset,
                                           void* address,
                                           uint32_t readSize,
                                           uint8_t* destBuffer,
                                           GFXU_MediaReadRequestCallback_FnPtr cb)
{
<#if ARIA_MEDIA_READ_FUNCTION?? && ARIA_MEDIA_READ_FUNCTION?has_content>
    return ${ARIA_MEDIA_READ_FUNCTION}(reader, asset, address, readSize, destBuffer, cb);
<#else>
    return GFX_FAILURE;
</#if>
}

static void LibAria_MediaCloseRequest(GFXU_AssetHeader* asset)
{
<#if ARIA_MEDIA_CLOSE_FUNCTION?? && ARIA_MEDIA_CLOSE_FUNCTION?has_content>
    ${ARIA_MEDIA_CLOSE_FUNCTION}(asset);
</#if>
}

</#if>
<#--
/*******************************************************************************
 End of File
*/
-->
