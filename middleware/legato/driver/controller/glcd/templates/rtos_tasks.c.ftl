<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _GLCD_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        DRV_GLCD_Update();
             <#if rtosEnableTaskDelay >
    <#lt>        vTaskDelay(${rtosTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>
