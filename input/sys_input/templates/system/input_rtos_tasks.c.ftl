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