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

/*******************************************************************************
  MPLAB Harmony SLCD Generated Driver Implementation File

  File Name:
    drv_gfx_SLCD.c

  Summary:
    Build-time generated implementation for the SLCD Driver for PIC32C MPU/MCUs.

  Description:
    Build-time generated implementation for the SLCD Driver for PIC32C MPU/MCUs.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "gfx/driver/gfx_driver.h"
#include "gfx/driver/controller/slcd/drv_gfx_slcd.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

<#assign val_LowResistanceDuration = LowResistanceDuration>
<#assign val_BiasBufferDuration = BiasBufferDuration>

#define LOW_RESISTANCE_DURATION ${val_LowResistanceDuration}
#define BIAS_BUFFER_DURATION ${val_BiasBufferDuration}
<#if SLCD_ClockSource == "OSCULP32K">
#define CONF_SLCD_CLOCK_SOURCE    0
<#elseif SLCD_ClockSource == "XOSC32K">
#define CONF_SLCD_CLOCK_SOURCE    1
</#if>

<#if SLCD_Duty == "Static">
#define CONF_SLCD_DUTY    0
<#elseif SLCD_Duty == "1/2">
#define CONF_SLCD_DUTY    1
<#elseif SLCD_Duty == "1/3">
#define CONF_SLCD_DUTY    2
<#elseif SLCD_Duty == "1/4">
#define CONF_SLCD_DUTY    3
<#elseif SLCD_Duty == "1/6">
#define CONF_SLCD_DUTY    4
<#elseif SLCD_Duty == "1/8">
#define CONF_SLCD_DUTY    5
</#if>

<#if SLCD_Bias == "Static">
#define CONF_SLCD_BIAS    0
<#elseif SLCD_Bias == "1/2">
#define CONF_SLCD_BIAS    1
<#elseif SLCD_Bias == "1/3">
#define CONF_SLCD_BIAS    2
<#elseif SLCD_Bias == "1/4">
#define CONF_SLCD_BIAS    3
</#if>

<#if FramePrescaler == "16">
#define CONF_SLCD_PVAL    0
<#elseif FramePrescaler == "32">
#define CONF_SLCD_PVAL    1
<#elseif FramePrescaler == "64">
#define CONF_SLCD_PVAL    2
<#elseif FramePrescaler == "128">
#define CONF_SLCD_PVAL    3
</#if>

#define CONF_SLCD_CKDIV    ${FrameDivider}

<#if SLCD_ClockSource == "OSCULP32K">
#define CONF_SLCD_VLCD_SEL    0
<#elseif SLCD_ClockSource == "XOSC32K">
#define CONF_SLCD_VLCD_SEL    1
</#if>

<#if BiasFreq == "2KHz">
#define CONF_SLCD_REF_REFRESH_FREQ    0
<#elseif BiasFreq == "1KHz">
#define CONF_SLCD_REF_REFRESH_FREQ    1
<#elseif BiasFreq == "500Hz">
#define CONF_SLCD_REF_REFRESH_FREQ    2
<#elseif BiasFreq == "250Hz">
#define CONF_SLCD_REF_REFRESH_FREQ    3
<#elseif BiasFreq == "125Hz">
#define CONF_SLCD_REF_REFRESH_FREQ    4
<#elseif BiasFreq == "62.5Hz">
#define CONF_SLCD_REF_REFRESH_FREQ    5
</#if>

<#if ChargeFreq == "2KHz">
#define CONF_SLCD_POWER_REFRESH_FREQ    0
<#elseif ChargeFreq == "1KHz">
#define CONF_SLCD_POWER_REFRESH_FREQ    1
<#elseif ChargeFreq == "500Hz">
#define CONF_SLCD_POWER_REFRESH_FREQ    2
<#elseif ChargeFreq == "250Hz">
#define CONF_SLCD_POWER_REFRESH_FREQ    3
</#if>

<#if PowerMode == "Auto">
#define CONF_SLCD_POWER_MODE    0
<#elseif PowerMode == "StepUp">
#define CONF_SLCD_POWER_MODE    1
<#elseif PowerMode == "StepDown">
#define CONF_SLCD_POWER_MODE    2
</#if>


#define CONF_SLCD_PIN_L_MASK       ${PinMaskLow}
#define CONF_SLCD_PIN_H_MASK       ${PinMaskHigh}

/**** Hardware Abstraction Interfaces ****/
typedef enum
{
    INIT = 0,
    DRAW,
    SWAP,
    SYNC,
} DRV_STATE;

const char* DRIVER_NAME = "SLCD";

typedef struct
{
  uint32_t addr;
  uint32_t ctrl;
  uint32_t next;
} SLCD_DMA_DESC;

static volatile DRV_STATE state;

static struct slcd_config config;


#if !defined(CONF_SLCD_CLOCK_SOURCE)
#  warning  SLCD_CLOCK_SOURCE is not defined, assuming 0.
#  define CONF_SLCD_CLOCK_SOURCE 0
#endif

#if !defined(CONF_SLCD_DUTY)
#  warning  CONF_SLCD_DUTY is not defined, assuming 0.
#  define CONF_SLCD_DUTY 0
#endif

#if !defined(CONF_SLCD_BIAS)
#  warning  CONF_SLCD_BIAS is not defined, assuming 0.
#  define CONF_SLCD_BIAS 0
#endif

#if !defined(CONF_SLCD_PVAL)
#  warning  CONF_SLCD_PVAL is not defined, assuming 0.
#  define CONF_SLCD_PVAL 0
#endif

#if !defined(CONF_SLCD_CKDIV)
#  warning  CONF_SLCD_CKDIV is not defined, assuming 0.
#  define CONF_SLCD_CKDIV 0
#endif

#if !defined(CONF_SLCD_VLCD_SEL)
#  warning  CONF_SLCD_VLCD_SEL is not defined, assuming 0.
#  define CONF_SLCD_VLCD_SEL 0
#endif

#if !defined(CONF_SLCD_REF_REFRESH_FREQ)
#  warning  CONF_SLCD_REF_REFRESH_FREQ is not defined, assuming 0.
#  define CONF_SLCD_REF_REFRESH_FREQ 0
#endif

#if !defined(CONF_SLCD_POWER_REFRESH_FREQ)
#  warning  CONF_SLCD_POWER_REFRESH_FREQ is not defined, assuming 0.
#  define CONF_SLCD_POWER_REFRESH_FREQ 0
#endif

#if !defined(CONF_SLCD_POWER_MODE)
#  warning  CONF_SLCD_POWER_MODE is not defined, assuming 0.
#  define CONF_SLCD_POWER_MODE 0
#endif

#if !defined(CONF_SLCD_PIN_L_MASK)
#  warning  CONF_SLCD_PIN_L_MASK is not defined, assuming 0.
#  define CONF_SLCD_PIN_L_MASK 0
#endif

#if !defined(CONF_SLCD_PIN_H_MASK)
#  warning  CONF_SLCD_PIN_H_MASK is not defined, assuming 0.
#  define CONF_SLCD_PIN_H_MASK 0
#endif

/**
 * \brief Initializes SLCD configurations struct to defaults
 *
 * Initailizes SLCD configuration struct to predefined safe default settings.
 *
 * \param[in] config Pointer to an instance of \ref struct slcd_config
 *
 */
void slcd_get_config_defaults()
{
    <#if RunInStandby == false>
    config.run_in_standby = false;
    <#else>
    config.run_in_standby = true;
    </#if>
    <#if WaveFormMode == "LOW_POWER">
    config.waveform_mode = SLCD_LOW_POWER_WAVEFORM_MODE;
    <#elseif WaveFormMode == "STANDARD">
    config.waveform_mode = SLCD_STANDARD_WAVEFORM_MODE;
    </#if>
    config.low_resistance_duration = ${val_LowResistanceDuration};
    <#if EnableLowResistance == false>
    config.enable_low_resistance = false;
    <#else>
    config.enable_low_resistance = true;
    </#if>
    config.bias_buffer_duration = ${val_BiasBufferDuration};
    <#if EnableBiasBuffer == false>
    config.enable_bias_buffer = false;
    <#else>
    config.enable_bias_buffer = true;
    </#if>
}

void DRV_SLCD_Update()
{
    switch(state)
    {
        case INIT:
        {
            state = DRAW;
            break;
        }

        case DRAW:
        default:
            break;
    }
}

gfxResult DRV_SLCD_Initialize()
{
    slcd_get_config_defaults();
    //if (!config) {
    //    return STATUS_ERR_INVALID_ARG;
    //}
    //system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, MCLK_APBCMASK_SLCD);
    MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_SLCD_Msk;

    /* Select SLCD clock */
    OSC32KCTRL_REGS->OSC32KCTRL_SLCDCTRL = CONF_SLCD_CLOCK_SOURCE & OSC32KCTRL_SLCDCTRL_Msk;

    slcd_disable();
    slcd_reset();

    SLCD_REGS->SLCD_CTRLA = SLCD_CTRLA_DUTY(CONF_SLCD_DUTY) | SLCD_CTRLA_BIAS(CONF_SLCD_BIAS)
                                     | SLCD_CTRLA_PRESC(CONF_SLCD_PVAL) | SLCD_CTRLA_CKDIV(CONF_SLCD_CKDIV)
                                     | (CONF_SLCD_VLCD_SEL << SLCD_CTRLA_XVLCD_Pos)
                                     | (config.run_in_standby << SLCD_CTRLA_RUNSTDBY_Pos)
                                     | SLCD_CTRLA_RRF(CONF_SLCD_REF_REFRESH_FREQ)
                                     | SLCD_CTRLA_PRF(CONF_SLCD_POWER_REFRESH_FREQ)
                                     | (config.waveform_mode << SLCD_CTRLA_WMOD_Pos);
    SLCD_REGS->SLCD_CTRLB = SLCD_CTRLB_BBD(config.bias_buffer_duration)
                                    | (config.enable_bias_buffer << SLCD_CTRLB_BBEN_Pos)
                                    | SLCD_CTRLB_LRD(config.low_resistance_duration)
                                    | (config.enable_low_resistance << SLCD_CTRLB_LREN_Pos);


    SLCD_REGS->SLCD_CTRLC |= SLCD_CTRLC_LPPM(CONF_SLCD_POWER_MODE) | SLCD_CTRLC_CTST(0x0F);

    SLCD_REGS->SLCD_LPENL = CONF_SLCD_PIN_L_MASK & SLCD_LPENL_Msk;
    SLCD_REGS->SLCD_LPENH = CONF_SLCD_PIN_H_MASK & SLCD_LPENH_Msk;
    
    return GFX_SUCCESS;
}



/**
 * \brief Enables the SLCD module
 *
 * Enables the SLCD module once it has been configured, ready for use. Most
 * module configuration parameters cannot be altered while the module is enabled.
 */

void slcd_enable(void)
{
        SLCD_REGS->SLCD_CTRLA |= SLCD_CTRLA_ENABLE_Msk;

        while (slcd_is_syncing()) {
                /* Wait for synchronization */
        }

        while (!slcd_get_vlcd_ready_status()) {
        }
}

/**
 * \brief Disables the SLCD module
 *
 * Disables the SLCD module.
 */
void slcd_disable(void)
{
        SLCD_REGS->SLCD_INTENCLR = SLCD_INTENCLR_Msk;
        SLCD_REGS->SLCD_INTFLAG = SLCD_INTFLAG_Msk;
        SLCD_REGS->SLCD_CTRLA &= ~(SLCD_CTRLA_ENABLE_Msk);
        while (slcd_is_syncing()) {
                /* Wait for synchronization */
        }
}

/**
 * \brief Check if SLCD module is enabled or not
 *
 * Check if SLCD module is enabled or not.
 *
 * \return Enable status.
 * \retval true SLCD module is enabled
 * \retval false SLCD module is disabled
 */

bool slcd_is_enabled(void)
{
        return ((SLCD_REGS->SLCD_CTRLA & SLCD_CTRLA_ENABLE_Msk) == SLCD_CTRLA_ENABLE_Msk);
}

/**
 * \brief Reset the SLCD module
 *
 * Reset the SLCD module.
 */
void slcd_reset(void)
{
        slcd_disable();
        SLCD_REGS->SLCD_CTRLA |= SLCD_CTRLA_SWRST_Msk;
        while (slcd_is_syncing()) {
                /* Wait for synchronization */
        }
}

/**
 * \brief Set the SLCD fine contrast
 *
 * The LCD contrast is defined by the value of VLCD voltage. The higher is the
 * VLCD voltage, the higher is the contrast. The software contrast adjustment
 * is only possible in internal supply mode.
 * In internal supply mode, VLCD is in the range 2.5V to 3.5V. VLCD can be
 * adjusted with 16 steps, each step is 60 mV.The contrast value can be written
 * at any time.
 *
 * \param[in] contrast Contrast value
 *
 * \return Status of set contrast.
 * \retval STATUS_OK               SLCD contrast set successful
 * \retval STATUS_ERR_INVALID_ARG  SLCD is not working in internal supply mode
 */
enum status_code slcd_set_contrast(uint8_t contrast)
{

        //if (SLCD_REGS->SLCD_CTRLA.bit.XVLCD) {
    if((SLCD_REGS->SLCD_CTRLA & SLCD_CTRLA_XVLCD_Msk) == SLCD_CTRLA_XVLCD_Msk) {
                return STATUS_ERR_INVALID_ARG;
        }
        uint16_t temp = SLCD_REGS->SLCD_CTRLC;

        temp &= ~ SLCD_CTRLC_CTST(0xf);
        temp |= SLCD_CTRLC_CTST(contrast);

        SLCD_REGS->SLCD_CTRLC = temp;
        return STATUS_OK;
}

/**
 * \brief Initializes SLCD blink configurations struct to defaults
 *
 * Initailizes SLCD blink configuration struct to predefined safe default settings.
 *
 * \param[in] config Pointer to an instance of \ref struct slcd_blink_config
 *
 */
void slcd_blink_get_config_defaults(struct slcd_blink_config *blink_config)
{
//	Assert(blink_config);

        blink_config->fc = SLCD_FRAME_COUNTER_0;
        blink_config->blink_all_seg = true;
}

/**
 * \brief Set SLCD blink mode
 *
 * Set SLCD blink mode.
 *
 *  \note SLCD blink cannot be set while module or blink is enabled.
 *
 * \param[in] config  Pointer to an SLCD blink configuration structure
 *
 * \return Status of the configuration procedure.
 * \retval STATUS_OK               SLCD blink configuration went successful
 * \retval STATUS_ERR_INVALID_ARG  If blink configuration failed
 */

enum status_code  slcd_blink_set_config(struct slcd_blink_config *const blink_config)
{
        if (!blink_config) {
                return STATUS_ERR_INVALID_ARG;
        }

        //SLCD_REGS->SLCD_BCFG.bit.MODE = (!(blink_config->blink_all_seg) << SLCD_BCFG_MODE_Pos);
    SLCD_REGS->SLCD_BCFG |= SLCD_BCFG_MODE(!(blink_config->blink_all_seg));
        //SLCD_REGS->SLCD_BCFG.bit.FCS	= SLCD_BCFG_FCS(blink_config->fc);
    SLCD_REGS->SLCD_BCFG |= SLCD_BCFG_FCS(blink_config->fc);
        return STATUS_OK;
}

/**
 * \brief Start an SLCD pixel/segment blinking
 *
 * \param[in] pix_com Pixel/segment COM coordinate
 * \param[in] pix_seg Pixel/segment SEG coordinate (range 0 to 1 inclusive)
 */
void slcd_set_blink_pixel(
                uint8_t pix_com,
                uint8_t pix_seg)
{
        /* Validate parameters. */
//	Assert(pix_seg<=1);

        if (pix_seg == 0) {
                SLCD_REGS->SLCD_BCFG |= SLCD_BCFG_BSS0(1 << pix_com);
        }

        if (pix_seg == 1) {
                SLCD_REGS->SLCD_BCFG |= SLCD_BCFG_BSS1(1 << pix_com);
        }
}

/**
 * \brief Stop a specified SLCD pixel/segment from blinking
 *
 * \param[in] pix_com Pixel/segment COM coordinate
 * \param[in] pix_seg Pixel/segment SEG coordinate (range 0 to 1 inclusive)
 */
void slcd_clear_blink_pixel(
                uint8_t pix_com,
                uint8_t pix_seg)
{
        /* Validate parameters. */
//	Assert(pix_seg<=1);

        if (pix_seg == 0) {
                SLCD_REGS->SLCD_BCFG &= ~ SLCD_BCFG_BSS0(1 << pix_com);
        }

        if (pix_seg == 1) {
                SLCD_REGS->SLCD_BCFG &= ~ SLCD_BCFG_BSS1(1 << pix_com);
        }
}

/**
 * \brief Stop all SLCD pixels/segments from blinking
 */
void slcd_clear_blink_all_pixel(void)
{
        //SLCD_REGS->SLCD_BCFG.bit.BSS0 = 0;
    SLCD_REGS->SLCD_BCFG &= ~SLCD_BCFG_BSS0_Msk;
        //SLCD_REGS->SLCD_BCFG.bit.BSS1 = 0;
    SLCD_REGS->SLCD_BCFG &= ~SLCD_BCFG_BSS1_Msk;
}


/**
 * \brief Set all bits in the SLCD display memory high
 */
void slcd_set_display_memory(void)
{
        SLCD_REGS->SLCD_SDATAH0 = SLCD_SDATAH0_Msk;
        SLCD_REGS->SLCD_SDATAL0 = SLCD_SDATAL0_Msk;
        SLCD_REGS->SLCD_SDATAH1 = SLCD_SDATAH1_Msk;
        SLCD_REGS->SLCD_SDATAL1 = SLCD_SDATAL1_Msk;
        SLCD_REGS->SLCD_SDATAH2 = SLCD_SDATAH2_Msk;
        SLCD_REGS->SLCD_SDATAL2 = SLCD_SDATAL2_Msk;
        SLCD_REGS->SLCD_SDATAH3 = SLCD_SDATAH3_Msk;
        SLCD_REGS->SLCD_SDATAL3 = SLCD_SDATAL3_Msk;
        SLCD_REGS->SLCD_SDATAH4 = SLCD_SDATAH4_Msk;
        SLCD_REGS->SLCD_SDATAL4 = SLCD_SDATAL4_Msk;
        SLCD_REGS->SLCD_SDATAH5 = SLCD_SDATAH5_Msk;
        SLCD_REGS->SLCD_SDATAL5 = SLCD_SDATAL5_Msk;
        SLCD_REGS->SLCD_SDATAH6 = SLCD_SDATAH6_Msk;
        SLCD_REGS->SLCD_SDATAL6 = SLCD_SDATAL6_Msk;
        SLCD_REGS->SLCD_SDATAH7 = SLCD_SDATAH7_Msk;
        SLCD_REGS->SLCD_SDATAL7 = SLCD_SDATAL7_Msk;
}


/**
 * \brief Enable the specified pixel/segment in the SLCD display memory
 *
 * \param[in] pix_com Pixel/segment COM coordinate,within [0-7]
 * \param[in] pix_seg Pixel/segment SEG coordinate within [0-43]
 */
 void slcd_set_pixel(
                uint8_t pix_com,
                uint8_t pix_seg)
{
        if ((pix_com < SLCD_MAX_COM) &&
                        (pix_seg < SLCD_MAX_SEG)) {
                switch(pix_com){
                        case 0:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH0 |= (1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL0 |= 1 <<pix_seg;
                                }
                                break;
                        case 1:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH1 |= (1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL1 |= 1 <<pix_seg;
                                }
                                break;
                        case 2:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH2 |= (1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL2 |= 1 <<pix_seg;
                                }
                                break;
                        case 3:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH3 |= (1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL3 |= 1 <<pix_seg;
                                }
                                break;
                        case 4:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH4 |= (1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL4 |= 1 <<pix_seg;
                                }
                                break;
                        case 5:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH5 |= (1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL5 |= 1 <<pix_seg;
                                }
                                break;
                        case 6:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH6 |= (1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL6 |= 1 <<pix_seg;
                                }
                                break;
                        case 7:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH7 |= (1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL7 |= 1 <<pix_seg;
                                }
                                break;
                }
                while (slcd_get_char_writing_status()) {
                }
        }
}

/**
 * \brief Disable the specified pixel/segment in the SLCD display memory
 *
 * \param[in] pix_com Pixel/segment COM coordinate
 * \param[in] pix_seg Pixel/segment SEG coordinate
 */
 void slcd_clear_pixel(uint8_t pix_com, uint8_t pix_seg)
{
        if ((pix_com < SLCD_MAX_COM) &&
                        (pix_seg < SLCD_MAX_SEG)) {
                switch(pix_com){
                        case 0:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH0 &= ~(1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL0 &= ~(1 <<pix_seg);
                                }
                                break;
                        case 1:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH1 &= ~(1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL1 &= ~(1 <<pix_seg);
                                }
                                break;
                        case 2:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH2 &= (1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL2 &= ~(1 <<pix_seg);
                                }
                                break;
                        case 3:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH3 &= ~(1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL3 &= ~(1 <<pix_seg);
                                }
                                break;
                        case 4:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH4 &= ~(1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL4 &= ~(1 <<pix_seg);
                                }
                                break;
                        case 5:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH5 &= ~(1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL5 &= ~(1 <<pix_seg);
                                }
                                break;
                        case 6:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH6 &= ~(1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL6 &= ~(1 <<pix_seg);
                                }
                                break;
                        case 7:
                                if (pix_seg >= 32) {
                                        SLCD_REGS->SLCD_SDATAH7 &= ~(1 <<(pix_seg-32));
                                } else {
                                        SLCD_REGS->SLCD_SDATAL7 &= ~(1 <<pix_seg);
                                }
                                break;
                }
                while (slcd_get_char_writing_status()) {
                }
        }
}

/**
 * \brief Set the specified segment in the SLCD display memory
 *
 * \param[in] pix_seg     Pixel/segment SEG coordinate
 * \param[in] byte_offset Byte offset in display memory
 * \param[in] seg_mask    Byte offset in display memory
 */
void slcd_set_seg_data(uint8_t seg_data,uint8_t byte_offset,uint8_t seg_mask)
{
        SLCD_REGS->SLCD_ISDATA = SLCD_ISDATA_SDATA(seg_data)
                                         | SLCD_ISDATA_OFF(byte_offset)
                                         | SLCD_ISDATA_SDMASK(seg_mask);

        while (slcd_get_char_writing_status()) {
        }
}

/**
 * \brief Initializes SLCD Automated Character configurations struct to defaults
 *
 * Initailizes SLCD Automated Character configuration struct to predefined safe default settings.
 *
 * \param[in] config Pointer to an instance of \ref struct slcd_automated_char_config
 *
 */
void slcd_automated_char_get_config_default(
                struct slcd_automated_char_config *config)
{
//	Assert(config);

        config->order = SLCD_AUTOMATED_CHAR_START_FROM_BOTTOM_RIGHT;
        config->fc = SLCD_FRAME_COUNTER_0;
        config->mode = SLCD_AUTOMATED_CHAR_SEQ;
        config->seg_line_num = 0;
        config->start_seg_line = 0;
        config->row_digit_num = 1;
        config->digit_num = 0;
        config->scrolling_step = 1;
        config->com_line_num = 1;
        config->data_mask = 0;

}

/**
 * \brief Set SLCD automated character
 *
 * Set automated character mode.
 *
 *  \note SLCD automated character mode cannot be set while module or
 * automated character is enabled.
 *
 * \param[in] config  Pointer to an SLCD automated character configuration structure
 *
 * \return Status of the configuration procedure.
 * \retval STATUS_OK               SLCD automated character configuration went successful
 * \retval STATUS_ERR_INVALID_ARG  If automated character  configuration failed
 */
enum status_code slcd_automated_char_set_config(
                struct slcd_automated_char_config *const config)
{
        if (!config) {
                return STATUS_ERR_INVALID_ARG;
        }
        SLCD_REGS->SLCD_CMCFG = SLCD_CMCFG_NSEG(config->seg_line_num)
                                         | (config->order  << SLCD_CMCFG_DEC_Pos);
        SLCD_REGS->SLCD_ACMCFG = SLCD_ACMCFG_FCS(config->fc)
                                         | (config->mode << SLCD_ACMCFG_MODE_Pos)
                                         | SLCD_ACMCFG_STSEG(config->start_seg_line)
                                         | SLCD_ACMCFG_NDROW(config->row_digit_num)
                                         | SLCD_ACMCFG_NDIG(config->digit_num)
                                         | SLCD_ACMCFG_STEPS(config->scrolling_step)
                                         | SLCD_ACMCFG_NCOM(config->com_line_num);

        SLCD_REGS->SLCD_CMDMASK = SLCD_CMDMASK_SDMASK(config->data_mask);

        return STATUS_OK;
}

/**
 * \brief Set SLCD character mapping
 *
 * Set Character mode amd SEG line per digit.
 *
 * \param[in] order         Mapping order in char mode
 * \param[in] seg_line_num  Define the number of SEG line per digit,
 *                          it equal to number of SEG line - 1
 */
void slcd_character_map_set(
                enum slcd_automated_char_order order,
                uint8_t seg_line_num)
{
        SLCD_REGS->SLCD_CMCFG = SLCD_CMCFG_NSEG(seg_line_num)
                                         | (order  << SLCD_CMCFG_DEC_Pos);
}

/**
 * \brief Write segments data to display memory in character mode
 *
 * \param[in] seg_data       Pixel/segment data
 * \param[in] data_mask      Segments data mask
 * \param[in] com_line_index COM line index
 * \param[in] seg_line_index Segments line index
 */
void slcd_character_write_data(uint8_t com_line_index,
                                                                        uint8_t seg_line_index,
                                                                        uint32_t seg_data,uint32_t data_mask)
{

        SLCD_REGS->SLCD_CMINDEX = SLCD_CMINDEX_SINDEX(seg_line_index)
                                                | SLCD_CMINDEX_CINDEX(com_line_index);
        SLCD_REGS->SLCD_CMDMASK = SLCD_CMDMASK_SDMASK(data_mask);
        SLCD_REGS->SLCD_CMDATA = SLCD_CMDATA_SDATA(seg_data);
        while (slcd_get_char_writing_status()) {
        }
}

/**
 * \brief Initializes circular shift configurations struct to defaults
 *
 * Initailizes circular shift configuration struct to predefined safe default settings.
 *
 * \param[in] config Pointer to an instance of \ref struct slcd_circular_shift_config
 *
 */
void slcd_circular_shift_get_config_defaults(
                struct slcd_circular_shift_config *const config)
{
//	Assert(config);

        config->fc = SLCD_FRAME_COUNTER_0;
        config->dir = SLCD_CIRCULAR_SHIFT_LEFT;
        config->size = 0;
        config->data = 0;
}

/**
 * \brief Set SLCD circular shift
 *
 * Set circular shift mode.
 *
 * \note SLCD circular shift mode cannot be set while module or circular shift is enabled.
 *
 * \param[in] config  Pointer to an SLCD circular shift configuration structure
 *
 * \return Status of the configuration procedure.
 * \retval STATUS_OK               SLCD circular shift configuration went successful
 * \retval STATUS_ERR_INVALID_ARG  If circular shift configuration failed
 */

enum status_code slcd_circular_shift_set_config(
                struct slcd_circular_shift_config *const config)
{
        if (!config) {
                return STATUS_ERR_INVALID_ARG;
        }
        SLCD_REGS->SLCD_CSRCFG = SLCD_CSRCFG_FCS(config->fc)
                                         | (config->dir << SLCD_CSRCFG_DIR_Pos)
                                         | SLCD_CSRCFG_SIZE(config->size)
                                         | SLCD_CSRCFG_DATA(config->data);

        return STATUS_OK;
}

/*******************************************************************************
 End of File
*/
