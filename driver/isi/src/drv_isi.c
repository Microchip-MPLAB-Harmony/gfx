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

#include "peripheral/isi/plib_isi.h"
#include "system/int/sys_int.h"
#include "system/time/sys_time.h"

#include "device.h"

#define TRY_COUNT 20

typedef enum
{
    STATE_UNINITIALIZED,
    STATE_RUN,
    STATE_PREVIEWTRANSFERDONE,
    STATE_CODECTRANSFERDONE,
            
} DriverState;

static DriverState _state = STATE_UNINITIALIZED;

static volatile bool _resetLock;

void DRV_ISI_Config(void);
void _DRV_ISI_PreviewInit();
void _DRV_ISI_PreviewTasks();
void _DRV_ISI_PreviewTransferComplete();
void _DRV_ISI_CodecInit();
void _DRV_ISI_CodecTasks();
void _DRV_ISI_CodecTransferComplete();

void DRV_ISI_Sleep(int ms)
{
    SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

    if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
        return;
    
    while (SYS_TIME_DelayIsComplete(timer) == false);
}

void DRV_ISI_Init(void)
{
    _state = STATE_UNINITIALIZED;
    
    DRV_ISI_Sleep(500);
        
    DRV_ISI_Reset();
    
    DRV_ISI_Config();
 
    _DRV_ISI_PreviewInit();
    _DRV_ISI_CodecInit();
    
    SYS_INT_SourceEnable(ISI_IRQn);
    
    DRV_ISI_Enable();
}

void DRV_ISI_Tasks()
{
    switch(_state)
    {
        case STATE_UNINITIALIZED:
        {
            _state = STATE_RUN;
            
            break;
        }
        case STATE_PREVIEWTRANSFERDONE:
        {
            _DRV_ISI_PreviewTasks();

            _state = STATE_RUN;
            
            break;
        }
        case STATE_CODECTRANSFERDONE:
        {
            _DRV_ISI_CodecTasks();

            _state = STATE_RUN;
            
            break;
        }
        default:
        {
            break;
        }
    }
}

bool DRV_ISI_IsEnabled(void)
{
    return (bool)PLIB_ISI_SR_ENABLE_Get();
}

bool DRV_ISI_Enable(void)
{
    if(PLIB_ISI_SR_ENABLE_Get() == 1)
        return true;
    
    PLIB_ISI_CR_ISI_EN_Set(1);
    
    while(PLIB_ISI_SR_ENABLE_Get() == 0);
    
    return true;
}

bool DRV_ISI_Disable(void)
{
    if(PLIB_ISI_SR_ENABLE_Get() == 0)
        return true;
    
    PLIB_ISI_CR_ISI_DIS_Set(1);
    
    while(PLIB_ISI_SR_DIS_DONE_Get() == 0);
        
    return true;
}

bool DRV_ISI_Reset(void)
{
    bool b = SYS_INT_SourceIsEnabled(ISI_IRQn);
    
    _resetLock = 1;
    
    SYS_INT_SourceEnable(ISI_IRQn);
    PLIB_ISI_IER_SRST_Set(1); // enable reset interrupt
    
    PLIB_ISI_CR_ISI_SRST_Set(1);
    PLIB_ISI_CR_ISI_DIS_Set(1);
    
    while(_resetLock == 1)
    {
        DRV_ISI_Sleep(1);
    }
    
    PLIB_ISI_IDR_SRST_Set(1); // disable reset interrupt
    
    if(b == false)
    {
        SYS_INT_SourceDisable(ISI_IRQn);
    }
    
    return true;
}

void ISI_Handler(void)
{
    uint32_t status = ISI_REGS->ISI_SR;
    
    if((status & ISI_SR_SRST_Msk) >> ISI_SR_SRST_Pos)
    {
        _resetLock = 0;
    }
    
    if((status & ISI_SR_PXFR_DONE_Msk) >> ISI_SR_PXFR_DONE_Pos)
    {
        _DRV_ISI_PreviewTransferComplete();
        
        _state = STATE_PREVIEWTRANSFERDONE;
    }
    
    if((status & ISI_SR_CXFR_DONE_Msk) >> ISI_SR_CXFR_DONE_Pos)
    {
        _DRV_ISI_CodecTransferComplete();
        
        _state = STATE_CODECTRANSFERDONE;
    }
}
