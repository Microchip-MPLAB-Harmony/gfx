<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _drv_touch_ptc_task(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        drv_touch_ptc_task();
             <#if RTOSUseDelay >
    <#lt>        vTaskDelay(${RTOSTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>