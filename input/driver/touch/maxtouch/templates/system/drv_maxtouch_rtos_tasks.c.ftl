<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _DRV_MAXTOUCH_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        DRV_MAXTOUCH_Tasks(sysObj.drvMAXTOUCH);
             <#if RTOSUseDelay >
    <#lt>        vTaskDelay(${RTOSTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>