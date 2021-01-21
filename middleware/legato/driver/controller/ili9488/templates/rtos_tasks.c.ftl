<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _ILI9488_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        DRV_ILI9488_Update();
             <#if rtosEnableTaskDelay >
    <#lt>        vTaskDelay(${rtosTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>