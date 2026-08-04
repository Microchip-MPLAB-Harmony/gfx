// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C)  Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#include "gfx/legato/generated/le_gen_harmony.h"

#include "definitions.h"
#include <string.h>

<#if enableInputEvents == true>
#include "system/input/sys_input.h"
</#if>

#if LE_USAGE_MONITOR_PERIOD_SECS && !LE_TASK_UPDATE_PERIOD_MS
#error "Set LE_UPDATE_PERIOD_MS to > 0 for usage monitoring"
#endif

#if LE_TASK_UPDATE_PERIOD_MS
static SYS_TIME_HANDLE legatoTimer = SYS_TIME_HANDLE_INVALID;
static volatile uint32_t systemTickMS = 0;
#endif

#if LE_USAGE_MONITOR_PERIOD_SECS
/* Performance metrics instance (type defined in le_gen_harmony.h) */
static LegatoPerformanceMetrics_t perfMetrics;

/* High-resolution timing using SYS_TIME counter */
static uint32_t timerFreqHz = 0;

static inline uint32_t GetTimeUs(void)
{
    uint64_t count = SYS_TIME_CounterGet();
    /* Convert to microseconds: (count * 1000000) / freq */
    /* Use 64-bit math to avoid overflow */
    return (uint32_t)((count * 1000000ULL) / timerFreqHz);
}

static inline uint32_t ElapsedUs(uint32_t startUs)
{
    uint32_t now = GetTimeUs();
    return (now >= startUs) ? (now - startUs) : (0xFFFFFFFF - startUs + now + 1);
}

/* Get performance metrics (call from application) */
const LegatoPerformanceMetrics_t* Legato_GetPerformanceMetrics(void)
{
    return &perfMetrics;
}
#endif

#if LE_TASK_UPDATE_PERIOD_MS
static void Legato_TimerCallback ( uintptr_t context)
{
    systemTickMS++;
}
#endif

<#if enableInputEvents == true>
// Input System Service interface code
SYS_INP_InputListener inputListener;

static void touchDownHandler(const SYS_INP_TouchStateEvent* const evt);
static void touchUpHandler(const SYS_INP_TouchStateEvent* const evt);
static void touchMoveHandler(const SYS_INP_TouchMoveEvent* const evt);
</#if>

static LegatoState legatoState;

void Legato_Initialize(void)
{

    gfxIOCTLArg_Palette pal;

    leInitialize(&gfxDriverInterface, &gfxGPUInterface);

    pal.palette = leGlobalPalette.header.address;
    pal.mode = (int)leGlobalPalette.colorMode;
    pal.colorCount = leGlobalPalette.colorCount;

    gfxDriverInterface.ioctl(GFX_IOCTL_SET_PALETTE, &pal);


    legato_initializeScreenState();

<#if enableInputEvents == true>
    inputListener.handleTouchDown = &touchDownHandler;
    inputListener.handleTouchUp = &touchUpHandler;
    inputListener.handleTouchMove = &touchMoveHandler;    
</#if>    

    legatoState = LEGATO_STATE_INIT;
}

void Legato_Tasks(void)
{
    switch(legatoState)
    {
        case LEGATO_STATE_INIT:
        {
<#if enableInputEvents == true>
            SYS_INP_AddListener(&inputListener);
</#if>               

#if LE_TASK_UPDATE_PERIOD_MS
            /* Register and start tick timers */
            legatoTimer = SYS_TIME_CallbackRegisterMS(Legato_TimerCallback,
                                                1,
                                                1,
                                                SYS_TIME_PERIODIC);
#endif

#if LE_USAGE_MONITOR_PERIOD_SECS
            /* Get timer frequency for high-resolution timing */
            timerFreqHz = SYS_TIME_FrequencyGet();
            if (timerFreqHz == 0) timerFreqHz = 1; /* Prevent divide by zero */

            /* Initialize metrics */
            memset(&perfMetrics, 0, sizeof(perfMetrics));
            perfMetrics.frameTimeMinUs = 0xFFFFFFFF;
#endif

            legatoState = LEGATO_STATE_RUNNING;
            break;
        }

        case LEGATO_STATE_RUNNING:
        {
#if LE_TASK_UPDATE_PERIOD_MS
            static uint32_t dtSinceLastRun = 0;
            static uint32_t lastTickMS = 0;

#if LE_USAGE_MONITOR_PERIOD_SECS
            /* Accumulated timing for the monitoring period */
            static uint32_t periodStartUs = 0;
            static uint32_t totalBusyUs = 0;
            static uint32_t totalUpdateUs = 0;
            static uint32_t totalRenderUs = 0;
            static uint32_t frameCount = 0;
            static uint32_t frameTimeAccumUs = 0;
            static uint32_t frameTimeMinUs = 0xFFFFFFFF;
            static uint32_t frameTimeMaxUs = 0;
            static uint32_t lastFrameDrawCount = 0;

            /* Initialize period start time on first run */
            if (periodStartUs == 0)
            {
                periodStartUs = GetTimeUs();
                lastFrameDrawCount = leRenderer_GetDrawCount();
            }
#endif

            /* Track elapsed time since last tick */
            if (systemTickMS > lastTickMS)
            {
                dtSinceLastRun += (systemTickMS - lastTickMS);
                lastTickMS = systemTickMS;
            }
            else if (systemTickMS < lastTickMS)
            {
                /* Handle tick counter wrap */
                lastTickMS = systemTickMS;
                dtSinceLastRun = 0;
            }

            /* Run Legato update at configured interval */
            if (dtSinceLastRun >= LE_TASK_UPDATE_PERIOD_MS)
            {
#if LE_USAGE_MONITOR_PERIOD_SECS
                uint32_t frameStartUs = GetTimeUs();
                uint32_t updateStartUs, updateEndUs;
                uint32_t renderStartUs, renderEndUs;
#endif

                /* Screen state update phase */
#if LE_USAGE_MONITOR_PERIOD_SECS
                updateStartUs = GetTimeUs();
#endif
                legato_updateScreenState();
#if LE_USAGE_MONITOR_PERIOD_SECS
                updateEndUs = GetTimeUs();
                totalUpdateUs += ElapsedUs(updateStartUs);
#endif

                /* Render phase */
#if LE_USAGE_MONITOR_PERIOD_SECS
                renderStartUs = GetTimeUs();
#endif
                leUpdate(dtSinceLastRun);
                
#if LE_USAGE_MONITOR_PERIOD_SECS
                renderEndUs = GetTimeUs();
                totalRenderUs += ElapsedUs(renderStartUs);

                /* Track frame timing */
                {
                    uint32_t frameTimeUs = ElapsedUs(frameStartUs);
                    totalBusyUs += frameTimeUs;
                    frameTimeAccumUs += frameTimeUs;
                    frameCount++;

                    /* Track min/max */
                    if (frameTimeUs < frameTimeMinUs) frameTimeMinUs = frameTimeUs;
                    if (frameTimeUs > frameTimeMaxUs) frameTimeMaxUs = frameTimeUs;

                    /* Store last frame time */
                    perfMetrics.lastFrameTimeUs = frameTimeUs;
                }
#endif

                dtSinceLastRun = 0;
            }

#if LE_USAGE_MONITOR_PERIOD_SECS
            /* Calculate and report metrics at end of monitoring period */
            {
                uint32_t periodElapsedUs = ElapsedUs(periodStartUs);
                uint32_t periodThresholdUs = LE_USAGE_MONITOR_PERIOD_SECS * 1000000UL;

                if (periodElapsedUs >= periodThresholdUs)
                {
                    uint32_t drawCount = leRenderer_GetDrawCount();
                    uint32_t framesRendered = drawCount - lastFrameDrawCount;

                    /* Calculate FPS (frames rendered / elapsed seconds) */
                    perfMetrics.fps = (float)framesRendered / LE_USAGE_MONITOR_PERIOD_SECS;
                    perfMetrics.frameCount = framesRendered;

                    /* CPU usage: (busy time / total time) * 100 */
                    /* Using x100 integer math: (busyUs * 10000) / periodUs */
                    if (periodElapsedUs > 0)
                    {
                        perfMetrics.cpuUsagePct100 = (uint32_t)((uint64_t)totalBusyUs * 10000ULL / periodElapsedUs);
                        perfMetrics.idlePct100 = 10000 - perfMetrics.cpuUsagePct100;
                        if (perfMetrics.cpuUsagePct100 > 10000) perfMetrics.cpuUsagePct100 = 10000;

                        /* Breakdown: update vs render */
                        if (totalBusyUs > 0)
                        {
                            perfMetrics.updateTimePct100 = (uint32_t)((uint64_t)totalUpdateUs * 10000ULL / periodElapsedUs);
                            perfMetrics.renderTimePct100 = (uint32_t)((uint64_t)totalRenderUs * 10000ULL / periodElapsedUs);
                        }
                    }

                    /* Frame timing stats */
                    if (frameCount > 0)
                    {
                        perfMetrics.frameTimeAvgUs = frameTimeAccumUs / frameCount;
                    }
                    perfMetrics.frameTimeMinUs = frameTimeMinUs;
                    perfMetrics.frameTimeMaxUs = frameTimeMaxUs;

#if LE_PRINT_USAGE_MONITOR
                    printf("FPS: %.1f | CPU: %lu.%02lu%% (update %lu.%02lu%%, render %lu.%02lu%%) | "
                           "Frame: avg %lu us, min %lu us, max %lu us\r\n",
                           perfMetrics.fps,
                           perfMetrics.cpuUsagePct100 / 100, perfMetrics.cpuUsagePct100 % 100,
                           perfMetrics.updateTimePct100 / 100, perfMetrics.updateTimePct100 % 100,
                           perfMetrics.renderTimePct100 / 100, perfMetrics.renderTimePct100 % 100,
                           perfMetrics.frameTimeAvgUs,
                           perfMetrics.frameTimeMinUs,
                           perfMetrics.frameTimeMaxUs);
#endif

                    /* Reset for next period */
                    periodStartUs = GetTimeUs();
                    lastFrameDrawCount = drawCount;
                    totalBusyUs = 0;
                    totalUpdateUs = 0;
                    totalRenderUs = 0;
                    frameCount = 0;
                    frameTimeAccumUs = 0;
                    frameTimeMinUs = 0xFFFFFFFF;
                    frameTimeMaxUs = 0;
                }
            }
#endif

#else
            /* No periodic update - run every call */
            legato_updateScreenState();
            leUpdate(0);
#endif
            break;
        }

        default:
        {
            break;
        }
    }
}

<#if enableInputEvents == true>
void touchDownHandler(const SYS_INP_TouchStateEvent* const evt)
{
    leInput_InjectTouchDown(evt->index, evt->x, evt->y);
}

void touchUpHandler(const SYS_INP_TouchStateEvent* const evt)
{
    leInput_InjectTouchUp(evt->index, evt->x, evt->y);
}

void touchMoveHandler(const SYS_INP_TouchMoveEvent* const evt)
{
    leInput_InjectTouchMoved(evt->index, evt->x, evt->y);
}
</#if>

