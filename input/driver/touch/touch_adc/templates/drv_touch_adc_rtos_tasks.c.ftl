<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">
    <#lt>void _DRV_Touch_ADC_Tasks(  void *pvParameters  )
    <#lt>{
    <#lt>    while(1)
    <#lt>    {
    <#lt>        DRV_Touch_ADC_Tasks(sysObj.drvTouchADC);
             <#if RTOSUseDelay >
    <#lt>        vTaskDelay(${RTOSTaskDelay} / portTICK_PERIOD_MS);
             </#if>
    <#lt>    }
    <#lt>}
</#if>
