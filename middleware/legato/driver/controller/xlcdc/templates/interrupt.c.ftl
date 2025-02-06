/* ${IP} Interrupt Handler */
void __attribute__((weak)) ${IP}_InterruptHandler (void)
{
<#if IntEnSof || IntEnRow || IntEnFifoError || IntEnBase || IntEnOvr1 || IntEnHeo || IntEnOvr2>
    // Main ISR - ${IP}
    register uint32_t status_m = ${IP}_REGS->LCDC_LCDISR;
</#if>
<#if IntEnSof>

    if(status_m & LCDC_LCDISR_SOF_Msk)
    {
<#if DoubleBuffering>
        DRV_XLCDC_SOF_Interrupt();
<#else>

</#if>
    }
</#if>
<#if IntEnRow>

    if(status_m & LCDC_LCDISR_ROW_Msk)
    {

    }
</#if>
<#if IntEnFifoError>

    if(status_m & LCDC_LCDISR_FIFOERR_Msk)
    {

    }
</#if>
<#if IntEnBase>

    if(status_m & LCDC_LCDISR_BASE_Msk)
    {
        // Low Level ISR - BASE
        register uint32_t status_ll = ${IP}_REGS->LCDC_BASEISR;

        if(status_ll & LCDC_BASEISR_END_Msk)
        {

        }

        if(status_ll & LCDC_BASEISR_ERROR_Msk)
        {

        }

        if(status_ll & LCDC_BASEISR_OVF_Msk)
        {

        }
    }
</#if>
<#if IntEnOvr1>

    if(status_m & LCDC_LCDISR_OVR1_Msk)
    {
        // Low Level ISR - OVR1
        register uint32_t status_ll = ${IP}_REGS->LCDC_OVR1ISR;

        if(status_ll & LCDC_OVR1ISR_END_Msk)
        {

        }

        if(status_ll & LCDC_OVR1ISR_ERROR_Msk)
        {

        }

        if(status_ll & LCDC_OVR1ISR_OVF_Msk)
        {

        }
    }
</#if>
<#if IntEnHeo>

    if(status_m & LCDC_LCDISR_HEO_Msk)
    {
        // Low Level ISR - HEO
        register uint32_t status_ll = ${IP}_REGS->LCDC_HEOISR;

        if(status_ll & LCDC_HEOISR_END_Msk)
        {

        }

        if(status_ll & LCDC_HEOISR_ERROR_Msk)
        {

        }

        if(status_ll & LCDC_HEOISR_OVF_Msk)
        {

        }
    }
</#if>
<#if SupportOVR2 && IntEnOvr2>

    if(status_m & LCDC_LCDISR_OVR2_Msk)
    {
        // Low Level ISR - OVR2
        register uint32_t status_ll = ${IP}_REGS->LCDC_OVR2ISR;

        if(status_ll & LCDC_OVR2ISR_END_Msk)
        {

        }

        if(status_ll & LCDC_OVR2ISR_ERROR_Msk)
        {

        }

        if(status_ll & LCDC_OVR2ISR_OVF_Msk)
        {

        }
    }
</#if>
}