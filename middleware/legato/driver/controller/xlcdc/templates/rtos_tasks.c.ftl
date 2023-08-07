<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _XLCDC_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        DRV_XLCDC_Update();
             <#if RTOSEnableTaskDelay >
    <#lt>        vTaskDelay(${RTOSTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>
<#if HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      _XLCDC_Task_TCB;
    <#lt>uint8_t*       _XLCDC_Task_Stk_Ptr;

    <#lt>static void _XLCDC_Tasks( ULONG thread_input )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        DRV_XLCDC_Update();
             <#if RTOSEnableTaskDelay >
        <#lt>    tx_thread_sleep((ULONG)(${RTOSTaskDelay} / (TX_TICK_PERIOD_MS)));
	         </#if>
    <#lt>    }
    <#lt>}
</#if>
