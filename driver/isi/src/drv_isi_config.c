#include "driver/isi/drv_isi.h"

#include "driver/isi/drv_isi_config.h"

void __attribute__((weak)) DRV_ISI_Config(void)
{
    
}

uint32_t DRV_ISI_GetStartOfFrameDelay(void)
{   
    return PLIB_ISI_CFG1_SFD_Get();
}

bool DRV_ISI_SetStartOfFrameDelay(uint32_t dly)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_SLD_Set(dly);
    
    return true;
}

uint32_t DRV_ISI_GetStartOfLineDelay(void)
{
    return PLIB_ISI_CFG1_SLD_Get();
}

bool DRV_ISI_SetStartOfLineDelay(uint32_t dly)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_SLD_Set(dly);
    
    return true;
}

DRV_ISI_ThresholdMask DRV_ISI_GetThresholdMask(void)
{
    return PLIB_ISI_CFG1_THMASK_Get();
}

bool DRV_ISI_SetThresholdMask(DRV_ISI_ThresholdMask msk)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_THMASK_Set((ISI_THMASK_Value)msk);
    
    return true;
}

bool DRV_ISI_GetFullMode(void)
{
    return (bool)PLIB_ISI_CFG1_FULL_Get();
}

bool DRV_ISI_SetFullMode(bool fm)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_FULL_Set((uint32_t)fm);
    
    return true;
}

bool DRV_ISI_GetCodecRequestEnabled(void)
{
    return (bool)PLIB_ISI_CFG1_DISCR_Get();
}

bool DRV_ISI_SetCodecRequestEnabled(bool dis)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_DISCR_Set((uint32_t)dis);
    
    return true;
}

uint32_t DRV_ISI_GetFrameSkip(void)
{
    return PLIB_ISI_CFG1_FRATE_Get();
}

bool DRV_ISI_SetFrameSkip(uint32_t fr)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_FRATE_Set(fr);
    
    return true;
}

bool DRV_ISI_GetCRCEnabled(void)
{
    return PLIB_ISI_CFG1_CRC_SYNC_Get();
}

bool DRV_ISI_SetCRCEnabled(bool en)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_CRC_SYNC_Set((uint32_t)en);
    
    return true;
}

DRV_ISI_EmbeddedSynchronizationMode DRV_ISI_GetEmbeddedSynchronizationMode(void)
{
    return (DRV_ISI_EmbeddedSynchronizationMode)PLIB_ISI_CFG1_EMB_SYNC_Get();
}

bool DRV_ISI_SetEmbeddedSynchronizationMode(DRV_ISI_EmbeddedSynchronizationMode md)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_EMB_SYNC_Set((uint32_t)md);
    
    return true;
}

DRV_ISI_PixelClockPolarity DRV_ISI_GetPixelClockPolarity(void)
{
    return (DRV_ISI_PixelClockPolarity)PLIB_ISI_CFG1_PIXCLK_POL_Get();
}

bool DRV_ISI_SetPixelClockPolarity(DRV_ISI_PixelClockPolarity pol)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_PIXCLK_POL_Set((uint32_t)pol);
    
    return true;
}

DRV_ISI_SyncPolarity DRV_ISI_GetVSyncPolarity(void)
{
    return (DRV_ISI_SyncPolarity)PLIB_ISI_CFG1_VSYNC_POL_Get();
}

bool DRV_ISI_SetVSyncPolarity(DRV_ISI_SyncPolarity pol)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_VSYNC_POL_Set((uint32_t)pol);
    
    return true;
}

DRV_ISI_SyncPolarity DRV_ISI_GetHSyncPolarity(void)
{
    return (DRV_ISI_SyncPolarity)PLIB_ISI_CFG1_HSYNC_POL_Get();
}

bool DRV_ISI_SetHSyncPolarity(DRV_ISI_SyncPolarity pol)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_CFG1_HSYNC_POL_Set((uint32_t)pol);
    
    return true;
}

DRV_ISI_InputColorMode DRV_ISI_GetInputColorMode(void)
{
    uint32_t yccSwap = PLIB_ISI_CFG2_YCC_SWAP_Get();
    uint32_t colSpace = PLIB_ISI_CFG2_COL_SPACE_Get();
    uint32_t rgbSwap = PLIB_ISI_CFG2_RGB_SWAP_Get();
    uint32_t gs = PLIB_ISI_CFG2_GRAYSCALE_Get();
    uint32_t rgbMode = PLIB_ISI_CFG2_RGB_MODE_Get();
    uint32_t gsMode = PLIB_ISI_CFG2_GS_MODE_Get();
    uint32_t rgbCfg = PLIB_ISI_CFG2_RGB_CFG_Get();
    
    if(gs == 1) // grayscale
    {
        if(gsMode == 0)
        {
            return DRV_ISI_INPCOLORMODE_GS_12BIT;
        }
        else
        {
            return DRV_ISI_INPCOLORMODE_GS_8BIT;
        }    
    }
    else if(colSpace == 1) // rgb
    {
        if(rgbMode == 0)
        {
            if(rgbSwap == 0)
            {
                return DRV_ISI_INPCOLORMODE_RGB_888_DEFAULT + rgbCfg;
            }
            else
            {
                return DRV_ISI_INPCOLORMODE_RGB_888_DEFAULT_SWAP + rgbCfg;
            }
        }
        else
        {
            if(rgbSwap == 0)
            {
                return DRV_ISI_INPCOLORMODE_RGB_565_DEFAULT + rgbCfg;
            }
            else
            {
                return DRV_ISI_INPCOLORMODE_RGB_565_DEFAULT_SWAP + rgbCfg;
            }
        }
    }
    else // ycbcr
    {
        return (DRV_ISI_InputColorMode)yccSwap;
    }
}

bool DRV_ISI_SetInputColorMode(DRV_ISI_InputColorMode mode)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    // reset registers to default
    PLIB_ISI_CFG2_RGB_CFG_Set(0);
    PLIB_ISI_CFG2_YCC_SWAP_Set(0);
    PLIB_ISI_CFG2_COL_SPACE_Set(0);
    PLIB_ISI_CFG2_RGB_SWAP_Set(0);
    PLIB_ISI_CFG2_GRAYSCALE_Set(0);
    PLIB_ISI_CFG2_RGB_MODE_Set(0);
    PLIB_ISI_CFG2_GS_MODE_Set(0);
    
    // ycbcr
    if(mode >= DRV_ISI_INPCOLORMODE_YCC_422_DEFAULT &&
       mode <= DRV_ISI_INPCOLORMODE_YCC_422_MODE3)
    {
        PLIB_ISI_CFG2_YCC_SWAP_Set((uint32_t)mode & 0x3);
    }
    else if(mode >= DRV_ISI_INPCOLORMODE_RGB_888_DEFAULT &&
            mode <= DRV_ISI_INPCOLORMODE_RGB_888_MODE3)
    {
        PLIB_ISI_CFG2_COL_SPACE_Set(1); // rgb
        PLIB_ISI_CFG2_RGB_CFG_Set((uint32_t)((mode - DRV_ISI_INPCOLORMODE_RGB_888_DEFAULT) & 0x3)); 
    }
    else if(mode >= DRV_ISI_INPCOLORMODE_RGB_888_DEFAULT_SWAP &&
            mode <= DRV_ISI_INPCOLORMODE_RGB_888_MODE3_SWAP)
    {
        PLIB_ISI_CFG2_COL_SPACE_Set(1); // rgb
        PLIB_ISI_CFG2_RGB_CFG_Set((uint32_t)((mode - DRV_ISI_INPCOLORMODE_RGB_888_DEFAULT_SWAP) & 0x3));
        PLIB_ISI_CFG2_RGB_SWAP_Set(1);
    }
    else if(mode >= DRV_ISI_INPCOLORMODE_RGB_565_DEFAULT &&
            mode <= DRV_ISI_INPCOLORMODE_RGB_565_MODE3)
    {
        PLIB_ISI_CFG2_COL_SPACE_Set(1); // rgb
        PLIB_ISI_CFG2_RGB_MODE_Set(1); // 565
        PLIB_ISI_CFG2_RGB_CFG_Set((uint32_t)((mode - DRV_ISI_INPCOLORMODE_RGB_565_DEFAULT) & 0x3)); 
    }
    else if(mode >= DRV_ISI_INPCOLORMODE_RGB_565_DEFAULT_SWAP &&
            mode <= DRV_ISI_INPCOLORMODE_RGB_565_MODE3_SWAP)
    {
        PLIB_ISI_CFG2_COL_SPACE_Set(1); // rgb
        PLIB_ISI_CFG2_RGB_MODE_Set(1); // 565
        PLIB_ISI_CFG2_RGB_CFG_Set((uint32_t)((mode - DRV_ISI_INPCOLORMODE_RGB_565_DEFAULT) & 0x3));
        PLIB_ISI_CFG2_RGB_SWAP_Set(1);
    }
    else if(mode == DRV_ISI_INPCOLORMODE_GS_8BIT)
    {
        PLIB_ISI_CFG2_GRAYSCALE_Set(1);
        PLIB_ISI_CFG2_GS_MODE_Set(1);
    }
    else if(mode == DRV_ISI_INPCOLORMODE_GS_12BIT)
    {
        PLIB_ISI_CFG2_GRAYSCALE_Set(1);
    }
    else
    {
		return false;
	}
	
	return true;
}

bool DRV_ISI_GetInputSize(uint32_t* w,
                          uint32_t* h)
{
    uint32_t tw, th;
    
    if(w == NULL || h == NULL)
        return false;
    
    tw = PLIB_ISI_CFG2_IM_HSIZE_Get();
    th = PLIB_ISI_CFG2_IM_VSIZE_Get();
    
    if(PLIB_ISI_CFG2_GRAYSCALE_Get() == 1)
    {
        *w = (tw + 1) * 2;
        *h = (th + 1) * 2;
    }
    else
    {
        *w = tw + 1;
        *h = th + 1;
    }
    
    return true;
}

bool DRV_ISI_SetInputSize(uint32_t w,
                          uint32_t h)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    // if 8-bit Grayscale mode is enabled, IM_HSIZE = (h / 2) - 1
    // else IM_HSIZE = h - 1
    if(PLIB_ISI_CFG2_GRAYSCALE_Get() == 1)
    {
        PLIB_ISI_CFG2_IM_HSIZE_Set(((w % 2048) / 2) - 1);
    }
    else
    {
        PLIB_ISI_CFG2_IM_HSIZE_Set((w % 2048) - 1);
    }
    
    PLIB_ISI_CFG2_IM_VSIZE_Set((h % 2048) - 1);
    
    return true;
}

bool DRV_ISI_GetPreviewSize(uint32_t* w,
                            uint32_t* h)
{
    if(w == NULL || h == NULL)
        return false;
    
    *w = PLIB_ISI_PSIZE_PREV_HSIZE_Get() + 1;
    *h = PLIB_ISI_PSIZE_PREV_VSIZE_Get() + 1;
    
    return true;
}

bool DRV_ISI_SetPreviewSize(uint32_t w,
                            uint32_t h)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    if(PLIB_ISI_CFG2_GRAYSCALE_Get() == 1)
    {
        if(w > 2048)
        {
            w = 2048;
        }
        
        if(h > 2048)
        {
            h = 2048;
        }
        
        PLIB_ISI_PSIZE_PREV_HSIZE_Set(w - 1);
        PLIB_ISI_PSIZE_PREV_VSIZE_Set(h - 1);
    }
    else
    {
        if(w > 640)
        {
            w = 640;
        }
        
        if(h > 480)
        {
            h = 480;
        }
        
        // rgb is limited to 640x480
        PLIB_ISI_PSIZE_PREV_HSIZE_Set(w - 1);
        PLIB_ISI_PSIZE_PREV_VSIZE_Set(h - 1);
    }
    
    return true;
}

uint32_t DRV_ISI_GetDecimationFactor(void)
{
    return PLIB_ISI_PDECF_DEC_FACTOR_Get();
}

bool DRV_ISI_SetDecimationFactor(uint32_t dec)
{
    if(DRV_ISI_IsEnabled() == true)
        return false;
    
    PLIB_ISI_PDECF_DEC_FACTOR_Set(dec & 0xFF);
    
    return true;
}

uint32_t DRV_ISI_CalculateDecimationFactor(uint32_t sensorW,
                                           uint32_t sensorH,
                                           uint32_t prevW,
                                           uint32_t prevH)
{
	uint32_t ratiow, ratioh, ratio;

	ratiow = (1600 * sensorW) / prevW;
	ratioh = (1600 * sensorH) / prevH;
	ratio = (ratiow > ratioh) ? ratioh : ratiow;
	ratio = (ratio > 16) ? ratio : 16;
	
	return ratio / 100;
}

bool DRV_ISI_GetYCCtoRGBConversionConfiguration(DRV_ISI_YCCtoRGBConfiguration* cfg)
{
    if(cfg == NULL)
        return false;
    
    cfg->c0 = PLIB_ISI_Y2R_SET0_C0_Get();
    cfg->c1 = PLIB_ISI_Y2R_SET0_C1_Get();
    cfg->c2 = PLIB_ISI_Y2R_SET0_C2_Get();
    cfg->c3 = PLIB_ISI_Y2R_SET0_C3_Get();
    cfg->c4 = PLIB_ISI_Y2R_SET1_C4_Get();
    cfg->cbOffset = PLIB_ISI_Y2R_SET1_Cboff_Get();
    cfg->crOffset = PLIB_ISI_Y2R_SET1_Croff_Get();
    cfg->yOffset = PLIB_ISI_Y2R_SET1_Yoff_Get();
    
    return true;
}

bool DRV_ISI_SetYCCtoRGBConversionConfiguration(const DRV_ISI_YCCtoRGBConfiguration* cfg)
{
    if(DRV_ISI_IsEnabled() == true || cfg == NULL)
        return false;
    
    PLIB_ISI_Y2R_SET0_C0_Set(cfg->c0);
    PLIB_ISI_Y2R_SET0_C1_Set(cfg->c1);
    PLIB_ISI_Y2R_SET0_C2_Set(cfg->c2);
    PLIB_ISI_Y2R_SET0_C3_Set(cfg->c3);
    PLIB_ISI_Y2R_SET1_C4_Set(cfg->c4);
    PLIB_ISI_Y2R_SET1_Cboff_Set(cfg->cbOffset);
    PLIB_ISI_Y2R_SET1_Croff_Set(cfg->crOffset);
    PLIB_ISI_Y2R_SET1_Yoff_Set(cfg->yOffset);
    
    return true;
}

bool DRV_ISI_GetRGBtoYCCConversionConfiguration(DRV_ISI_RGBtoYCCConfiguration* cfg)
{
    if(cfg == NULL)
        return false;
    
    cfg->c0 = PLIB_ISI_R2Y_SET0_C0_Get();
    cfg->c1 = PLIB_ISI_R2Y_SET0_C1_Get();
    cfg->c2 = PLIB_ISI_R2Y_SET0_C2_Get();
    cfg->c3 = PLIB_ISI_R2Y_SET1_C3_Get();
    cfg->c4 = PLIB_ISI_R2Y_SET1_C4_Get();
    cfg->c5 = PLIB_ISI_R2Y_SET1_C5_Get();
    cfg->c6 = PLIB_ISI_R2Y_SET2_C6_Get();
    cfg->c7 = PLIB_ISI_R2Y_SET2_C7_Get();
    cfg->c8 = PLIB_ISI_R2Y_SET2_C8_Get();
    cfg->rOffset = PLIB_ISI_R2Y_SET0_Roff_Get();
    cfg->gOffset = PLIB_ISI_R2Y_SET1_Goff_Get();
    cfg->bOffset = PLIB_ISI_R2Y_SET2_Boff_Get();
    
    return true;
}

bool DRV_ISI_SetRGBtoYCCConversionConfiguration(const DRV_ISI_RGBtoYCCConfiguration* cfg)
{
    if(DRV_ISI_IsEnabled() == true || cfg == NULL)
        return false;
    
    PLIB_ISI_R2Y_SET0_C0_Set(cfg->c0);
    PLIB_ISI_R2Y_SET0_C1_Set(cfg->c1);
    PLIB_ISI_R2Y_SET0_C2_Set(cfg->c2);
    PLIB_ISI_R2Y_SET1_C3_Set(cfg->c3);
    PLIB_ISI_R2Y_SET1_C4_Set(cfg->c4);
    PLIB_ISI_R2Y_SET1_C5_Set(cfg->c5);
    PLIB_ISI_R2Y_SET2_C6_Set(cfg->c6);
    PLIB_ISI_R2Y_SET2_C7_Set(cfg->c7);
    PLIB_ISI_R2Y_SET2_C8_Set(cfg->c8);
    PLIB_ISI_R2Y_SET0_Roff_Set(cfg->rOffset);
    PLIB_ISI_R2Y_SET1_Goff_Set(cfg->gOffset);
    PLIB_ISI_R2Y_SET2_Boff_Set(cfg->bOffset);
    
    return true;
}
