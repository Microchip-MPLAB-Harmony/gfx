<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _LEGATO_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        Legato_Tasks();
             <#if rtosEnableTaskDelay >
    <#lt>        vTaskDelay(${rtosTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>
<#if HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      _LEGATO_Task_TCB;
    <#lt>uint8_t*       _LEGATO_Task_Stk_Ptr;

    <#lt>static void _LEGATO_Tasks( ULONG thread_input )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        Legato_Tasks();
             <#if rtosEnableTaskDelay >
        <#lt>    tx_thread_sleep((ULONG)(${rtosTaskDelay} / (TX_TICK_PERIOD_MS)));
	         </#if>
    <#lt>    }
    <#lt>}
</#if>
