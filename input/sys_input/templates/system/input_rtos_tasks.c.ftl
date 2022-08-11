<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _SYS_INPUT_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        SYS_INP_Tasks();
             <#if RTOSUseDelay >
    <#lt>        vTaskDelay(${RTOSTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>
<#if HarmonyCore.SELECT_RTOS == "ThreadX">
    <#lt>TX_THREAD      _SYS_INPUT_Task_TCB;
    <#lt>uint8_t*       _SYS_INPUT_Task_Stk_Ptr;

    <#lt>static void _SYS_INPUT_Tasks( ULONG thread_input )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        SYS_INP_Tasks();
             <#if RTOSUseDelay >
        <#lt>    tx_thread_sleep((ULONG)(${RTOSTaskDelay} / (TX_TICK_PERIOD_MS)));
	         </#if>
    <#lt>    }
    <#lt>}
</#if>
