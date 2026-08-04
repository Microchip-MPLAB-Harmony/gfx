/*******************************************************************************
  EPD Display Configuration

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_EPD_config.h

  Summary:
    Auto-generated EPD configuration from MCC.

  Description:
    This file contains display configuration parameters generated from the
    MCC component settings. All EPD driver files should include this header
    for consistent access to display dimensions and vendor-specific settings.
*******************************************************************************/

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

#ifndef DRV_GFX_EPD_CONFIG_H
#define DRV_GFX_EPD_CONFIG_H

/* ************************************************************************** */
/* Display Dimensions                                                          */
/* ************************************************************************** */

#define DISPLAY_WIDTH  ${EPDWidth}u
#define DISPLAY_HEIGHT  ${EPDHeight}u

/* ************************************************************************** */
/* Vendor Configuration                                                        */
/* ************************************************************************** */

<#assign DisplayVendor = DisplayVendor>

<#if DisplayVendor == "AMP">
#include "UltraChip/UC8253/drv_gfx_EPD_AMP_UC8253.h"
<#elseif DisplayVendor == "MikroE">
#include "Waveshare/MIKROE_2n13/drv_gfx_EPD_MIKROE_2n13.h"
<#else>
/* No vendor selected */
</#if>

<#if DisplayVendor == "AMP">
<#assign AMPDisplay = AMPDisplay>
<#if AMPDisplay == "E0352A01-AF">
#define DISPLAY_VENDOR_AMP          1
#define DISPLAY_VENDOR_AMP_240x416  1
#define DISPLAY_CONTROLLER_UC8253   1
<#elseif AMPDisplay == "AXE37A3AFS">
#define DISPLAY_VENDOR_AMP          1
#define DISPLAY_VENDOR_AMP_240x360  1
#define DISPLAY_CONTROLLER_UC8253   1
</#if>
<#elseif DisplayVendor == "MikroE">
#define DISPLAY_VENDOR_MIKROE       1
#define DISPLAY_VENDOR_MIKROE_2N13  1
<#else>
/* No vendor selected */
</#if>

/* ************************************************************************** */
/* Feature Flags                                                               */
/* ************************************************************************** */

#ifndef LOG_LEGATO
#define LOG_LEGATO  0
#endif

#endif /* DRV_GFX_EPD_CONFIG_H */
