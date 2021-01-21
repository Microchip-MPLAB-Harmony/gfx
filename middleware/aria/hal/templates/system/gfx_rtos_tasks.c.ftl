<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _GFX_HAL_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        GFX_Update();
             <#if GFXRTOSTaskUseDelay >
    <#lt>        vTaskDelay(${GFXRTOSTaskDelayVal} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>