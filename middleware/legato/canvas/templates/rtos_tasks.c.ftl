<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _GFX_CANVAS_Task(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        GFX_CANVAS_Task();
             <#if rtosEnableTaskDelay >
    <#lt>        vTaskDelay(${rtosTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>