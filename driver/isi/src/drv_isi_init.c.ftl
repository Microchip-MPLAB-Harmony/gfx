//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

#include "driver/isi/drv_isi.h"

#include <stdint.h>
#include <stddef.h>

#include <toolchain_specifics.h>

#include "system/int/sys_int.h"

#define ISI_PREVIEW_WIDTH    ${PrevHSize}
#define ISI_PREVIEW_HEIGHT   ${PrevVSize}
#define ISI_PREVIEW_SIZE     2

#define ISI_CAPTURE_WIDTH    ${InputHSize}
#define ISI_CAPTURE_HEIGHT   ${InputVSize}
#define ISI_CAPTURE_SIZE     2

<#if PrevBufCnt gte 1>
static uint16_t CACHE_ALIGN __ALIGNED(32) previewBuffer0[ISI_PREVIEW_WIDTH * ISI_PREVIEW_HEIGHT];
</#if>
<#if PrevBufCnt gte 2>
static uint16_t CACHE_ALIGN __ALIGNED(32) previewBuffer1[ISI_PREVIEW_WIDTH * ISI_PREVIEW_HEIGHT];
</#if>
<#if PrevBufCnt == 3>
static uint16_t CACHE_ALIGN __ALIGNED(32) previewBuffer2[ISI_PREVIEW_WIDTH * ISI_PREVIEW_HEIGHT];
</#if>

<#if CodecBufCnt gte 1>
static uint16_t CACHE_ALIGN __ALIGNED(32) captureBuffer0[ISI_CAPTURE_WIDTH * ISI_CAPTURE_HEIGHT];
</#if>
<#if CodecBufCnt gte 2>
static uint16_t CACHE_ALIGN __ALIGNED(32) captureBuffer1[ISI_CAPTURE_WIDTH * ISI_CAPTURE_HEIGHT];
</#if>
<#if CodecBufCnt == 3>
static uint16_t CACHE_ALIGN __ALIGNED(32) captureBuffer2[ISI_CAPTURE_WIDTH * ISI_CAPTURE_HEIGHT];
</#if>

void DRV_ISI_Config(void)
{
	uint32_t dec;
<#if YUVtoRGB == true>
	DRV_ISI_YCCtoRGBConfiguration YCCToRGBMatrix;
</#if>
<#if RGBtoYUV == true>
	DRV_ISI_RGBtoYCCConfiguration RGBToYCCMatrix;
</#if>
	
    DRV_ISI_Reset();
    
    PLIB_ISI_DMA_CHER_P_CH_EN_Set(0);
    PLIB_ISI_DMA_CHER_C_CH_EN_Set(0);
    
<#if SFD gt 0>
    DRV_ISI_SetStartOfFrameDelay(${SFD});
</#if>    
<#if SLD gt 0>
    DRV_ISI_SetStartOfLineDelay(${SLD});
</#if>
<#if ThresholdMask == "BEATS_8" || ThresholdMask == "BEATS_16">
	DRV_ISI_SetThresholdMask(DRV_ISI_THMASK_${ThresholdMask});
</#if>
<#if FullMode == true>	
	DRV_ISI_SetFullMode(true);
</#if>
<#if CodecRequest == true>	
	DRV_ISI_SetCodecRequestDisabled(false);
</#if>
<#if FrameSkip gt 0>
    DRV_ISI_SetFrameSkip(${FrameSkip});
</#if>
<#if CRCCorrection == true>	
	DRV_ISI_SetCRCEnabled(true);
</#if>
<#if EmbededSyncMode == "SAV_EAV">
	DRV_ISI_SetEmbeddedSynchronizationMode(DRV_ISI_EMSYNC_SAV_EAV);
</#if>
<#if ClockPol == "FALLING">
	DRV_ISI_SetEmbeddedSynchronizationMode(DRV_ISI_PIXELCLOCKPOL_SAMPLE_FALLING);
</#if>
<#if VSYNCPol == "ACTIVE_LOW">
	DRV_ISI_SetVSyncPolarity(DRV_ISI_SYNC_ACTIVE_LOW);
</#if>
<#if HSYNCPol == "ACTIVE_LOW">
	DRV_ISI_SetHSyncPolarity(DRV_ISI_SYNC_ACTIVE_LOW);
</#if>
<#if InputColorMode != "YCC_422_DEFAULT">
	DRV_ISI_SetInputColorMode(DRV_ISI_INPCOLORMODE_${InputColorMode});
</#if>

	DRV_ISI_SetInputSize(ISI_CAPTURE_WIDTH, ISI_CAPTURE_HEIGHT);
	DRV_ISI_SetPreviewSize(ISI_PREVIEW_WIDTH, ISI_PREVIEW_HEIGHT);
	
	dec = DRV_ISI_CalculateDecimationFactor(ISI_CAPTURE_WIDTH,
	                                        ISI_CAPTURE_HEIGHT,
	                                        ISI_PREVIEW_WIDTH,
	                                        ISI_PREVIEW_HEIGHT);
	
	DRV_ISI_SetDecimationFactor(dec);
	
<#if YUVtoRGB == true>
	YCCToRGBMatrix.c0 = ${YUVC0};
	YCCToRGBMatrix.c1 = ${YUVC1};
	YCCToRGBMatrix.c2 = ${YUVC2};
	YCCToRGBMatrix.c3 = ${YUVC3};
	YCCToRGBMatrix.c4 = ${YUVC4};
	YCCToRGBMatrix.crOffset = ${YUVCroff?then(1, 0)};
	YCCToRGBMatrix.cbOffset = ${YUVCboff?then(1, 0)};
	YCCToRGBMatrix.yOffset = ${YUVYoff?then(1, 0)};
	
	DRV_ISI_SetYCCtoRGBConversionConfiguration(&YCCToRGBMatrix);
</#if>
	
<#if RGBtoYUV == true>
	RGBToYCCMatrix.c0 = ${RGBC0};
	RGBToYCCMatrix.c1 = ${RGBC1};
	RGBToYCCMatrix.c2 = ${RGBC2};
	RGBToYCCMatrix.c3 = ${RGBC3};
	RGBToYCCMatrix.c4 = ${RGBC4};
	RGBToYCCMatrix.c5 = ${RGBC5};
	RGBToYCCMatrix.c6 = ${RGBC6};
	RGBToYCCMatrix.c7 = ${RGBC7};
	RGBToYCCMatrix.c8 = ${RGBC8};
	RGBToYCCMatrix.rOffset = ${RGBROFF?then(1, 0)};
	RGBToYCCMatrix.gOffset = ${RGBGOFF?then(1, 0)};
	RGBToYCCMatrix.bOffset = ${RGBBOFF?then(1, 0)};
	
	DRV_ISI_SetRGBtoYCCConversionConfiguration(&RGBToYCCMatrix);
</#if>
		
<#if PrevBufCnt gte 1>
	DRV_ISI_AddPreviewFrameBuffer((uint8_t*)previewBuffer0);
</#if>
<#if PrevBufCnt gte 2>
    DRV_ISI_AddPreviewFrameBuffer((uint8_t*)previewBuffer1);
</#if>
<#if PrevBufCnt == 3>
    DRV_ISI_AddPreviewFrameBuffer((uint8_t*)previewBuffer2);
</#if>

<#if CodecBufCnt gte 1>
	DRV_ISI_AddCodecFrameBuffer((uint8_t*)captureBuffer0);
</#if>
<#if CodecBufCnt gte 2>
    DRV_ISI_AddCodecFrameBuffer((uint8_t*)captureBuffer1);
</#if>
<#if CodecBufCnt == 3>
    DRV_ISI_AddCodecFrameBuffer((uint8_t*)captureBuffer2);
</#if>
}
