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
    libaria_init.c

  Summary:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

  Description:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

#include "gfx/libaria/libaria_init.h"
#include "gfx/libaria/inc/libaria_utils.h"
<#if useRTOSExtensions == true>
#include "gfx/libaria/inc/libaria_context_rtos.h"
</#if>

<#if ARIA_INIT_LOCAL_VARS??>
${ARIA_INIT_LOCAL_VARS}

</#if>
<#if ARIA_INIT_LOCAL_FUNC_DEFS??>
${ARIA_INIT_LOCAL_FUNC_DEFS}

</#if>
int32_t libaria_initialize(void)
{
<#if ARIA_INIT_VARS??>
${ARIA_INIT_VARS}
</#if>
<#if ARIA_INIT_CODE??>
${ARIA_INIT_CODE}
</#if>
	return 0;
}

<#if ARIA_INIT_LOCAL_FUNCS??>
${ARIA_INIT_LOCAL_FUNCS}
</#if>

<#if preProcessAssetsCode??>
int32_t libaria_preprocess_assets(void)
{
${preProcessAssetsCode}
    return 0;
}
</#if>

<#--
/*******************************************************************************
 End of File
*/
-->
