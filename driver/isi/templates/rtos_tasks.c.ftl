<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _DRV_ISI_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        DRV_ISI_Tasks();
             <#if RTOSUseDelay >
    <#lt>        vTaskDelay(${RTOSTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>
<#if HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      _DRV_ISI_Task_TCB;
    <#lt>uint8_t*       _DRV_ISI_Task_Stk_Ptr;

    <#lt>static void _DRV_ISI_Tasks( ULONG thread_input )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        DRV_ISI_Tasks();
             <#if RTOSUseDelay >
        <#lt>    tx_thread_sleep((ULONG)(${RTOSTaskDelay} / (TX_TICK_PERIOD_MS)));
	         </#if>
    <#lt>    }
    <#lt>}
</#if>
