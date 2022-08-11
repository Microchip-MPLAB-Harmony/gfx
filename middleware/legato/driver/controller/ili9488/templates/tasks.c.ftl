<#--
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
-->
<#if HarmonyCore.SELECT_RTOS == "BareMetal">
    <#lt>    DRV_ILI9488_Update();
<#elseif HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>    xTaskCreate( _ILI9488_Tasks,
    <#lt>        "ILI9488_Tasks",
    <#lt>        ${rtosTaskSize},
    <#lt>        (void*)NULL,
    <#lt>        ${rtosTaskPriority},
    <#lt>        (TaskHandle_t*)NULL
    <#lt>    );
<#elseif HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>    /* Allocate the stack for ILI9488 thread */
    <#lt>    tx_byte_allocate(&byte_pool_0,
    <#lt>        (VOID **) &_ILI9488_Task_Stk_Ptr,
    <#lt>        ${rtosTaskSize},
    <#lt>        TX_NO_WAIT
    <#lt>    );

    <#lt>    /* create the ILI9488 thread */
    <#lt>    tx_thread_create(&_ILI9488_Task_TCB,
    <#lt>        "_ILI9488_Tasks",
    <#lt>        _ILI9488_Tasks,
    <#lt>        31,
    <#lt>        _ILI9488_Task_Stk_Ptr,
    <#lt>        ${rtosTaskSize},
    <#lt>        ${rtosTaskPriority},
    <#lt>        ${rtosTaskPriority},
    <#lt>        TX_NO_TIME_SLICE,
    <#lt>        TX_AUTO_START
    <#lt>    );
</#if>
<#--
/*******************************************************************************
 End of File
*/
-->
