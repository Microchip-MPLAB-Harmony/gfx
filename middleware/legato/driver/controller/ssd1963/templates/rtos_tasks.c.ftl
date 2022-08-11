<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _SSD1963_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        DRV_SSD1963_Update();
             <#if rtosEnableTaskDelay >
    <#lt>        vTaskDelay(${rtosTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>
<#if HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      _SSD1963_Task_TCB;
    <#lt>uint8_t*       _SSD1963_Task_Stk_Ptr;

    <#lt>static void _SSD1963_Tasks( ULONG thread_input )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        DRV_SSD1963_Update();
             <#if rtosEnableTaskDelay >
        <#lt>    tx_thread_sleep((ULONG)(${rtosTaskDelay} / (TX_TICK_PERIOD_MS)));
	         </#if>
    <#lt>    }
    <#lt>}
</#if>
