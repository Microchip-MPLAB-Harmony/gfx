/*******************************************************************************
 Touch ADC Driver Interface File

  File Name:
    drv_touch_adc_static.c

  Summary:
    Touch ADC Driver interface file.

  Description:
    This is a simple 4-wire resistive touch screen driver. The file consist of
    touch controller ADC driver interfaces. It implements the driver interfaces
    which read the touch input data from display overlay through the ADC peripheral.

    Note: This driver is based on the MPLAB Harmony ADC driver.
 ******************************************************************************/

/*******************************************************************************
Copyright (c) 2021 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 ******************************************************************************/

#include "definitions.h"
#include "driver/input/drv_touch_adc.h"

<#if gfx_hal_le??>

<#assign Val_Width = gfx_hal_le.DisplayWidth>
<#assign Val_Height = gfx_hal_le.DisplayHeight>
<#else>

<#assign Val_Width = DisplayWidth>
<#assign Val_Height = DisplayHeight>
</#if>

#define DISPLAY_WIDTH   ${Val_Width}
#define DISPLAY_HEIGHT  ${Val_Height}


<#assign Val_CAL_ULX_SAMPLE = CAL_ULX_SAMPLE>
<#assign Val_CAL_ULY_SAMPLE = CAL_ULY_SAMPLE>
<#assign Val_CAL_URX_SAMPLE = CAL_URX_SAMPLE>
<#assign Val_CAL_URY_SAMPLE = CAL_URY_SAMPLE>

<#assign Val_CAL_LRX_SAMPLE = CAL_LRX_SAMPLE>
<#assign Val_CAL_LRY_SAMPLE = CAL_LRY_SAMPLE>
<#assign Val_CAL_LLX_SAMPLE = CAL_LLX_SAMPLE>
<#assign Val_CAL_LLY_SAMPLE = CAL_LLY_SAMPLE>


<#assign CONFIG_DRV_ADCHS_ANALOG_INPUT_ID_IDX1 = 1>
<#assign CONFIG_DRV_ADCHS_ANALOG_INPUT_ID_IDX0 = 2>
<#assign CONFIG_DRV_TOUCH_ADC_PRESS_THRESHOLD  = 4>

//////////////////////// GLOBAL VARIABLES ////////////////////////////

#define SCALE_FACTOR (1<<8)
#define CAL_X_INSET    (((DISPLAY_WIDTH)*(${20}>>1))/100)
#define CAL_Y_INSET    (((DISPLAY_HEIGHT)*(${20}>>1))/100)
#define DRV_TOUCH_ADC_SAMPLE_POINTS   4

// *****************************************************************************

typedef enum
{
    /* */
    DRV_TOUCH_ADC_STATE_IDLE,

    /* */
    DRV_TOUCH_ADC_STATE_SET_X,

    /* */
    DRV_TOUCH_ADC_STATE_RUN_X,

    /* */
    DRV_TOUCH_ADC_STATE_GET_X,

    /* */
    DRV_TOUCH_ADC_STATE_RUN_CHECK_X,

    /* */
    DRV_TOUCH_ADC_STATE_CHECK_X,

    /* */
    DRV_TOUCH_ADC_STATE_SET_Y,

    /* */
    DRV_TOUCH_ADC_STATE_RUN_Y,

    /* */
    DRV_TOUCH_ADC_STATE_GET_Y,

    /* */
    DRV_TOUCH_ADC_STATE_CHECK_Y,

    /* */
    DRV_TOUCH_ADC_STATE_SET_VALUES,

    /* */
    DRV_TOUCH_ADC_STATE_GET_POT,

    /* */
    DRV_TOUCH_ADC_STATE_RUN_POT

} DRV_TOUCH_ADC_TOUCH_STATES;

// *****************************************************************************

typedef struct
{
    DRV_TOUCH_ADC_TOUCH_STATES state;
} DRV_TOUCH_ADC_OBJECT;

//******************************************************************************
// Local functions
//******************************************************************************

// Current ADC values for X and Y channels
volatile short adcX = -1;
volatile short adcY = -1;

//   coefficient for screen to display coordinate translation
volatile long coefA;
volatile long coefB;
volatile long coefC;
volatile long coefD;

// copy of the stored or sampled raw points (this is the calibration data stored)
/*      xRawTouch[0] - x sample from upper left corner; 
        xRawTouch[1] - x sample from upper right corner
        xRawTouch[2] - x sample from lower right corner
        xRawTouch[3] - x sample from lower left corner
        yRawTouch[0] - y sample from upper left corner; 
        yRawTouch[1] - y sample from upper right corner
        yRawTouch[2] - y sample from lower right corner
        yRawTouch[3] - y sample from lower left corner
 */

/* Driver instance object */
static DRV_TOUCH_ADC_OBJECT        sADCDriverInstances;

short xRawTouch[DRV_TOUCH_ADC_SAMPLE_POINTS] = { ${Val_CAL_ULX_SAMPLE},
                                                 ${Val_CAL_LRX_SAMPLE},
                                                 ${Val_CAL_URX_SAMPLE},
                                                 ${Val_CAL_LLX_SAMPLE} };

short yRawTouch[DRV_TOUCH_ADC_SAMPLE_POINTS] = { ${Val_CAL_ULY_SAMPLE},
                                                 ${Val_CAL_LRY_SAMPLE},
                                                 ${Val_CAL_URY_SAMPLE},
                                                 ${Val_CAL_LLY_SAMPLE} };

<#if PollingMode == "Interrupt">
/* This function is called after period matches in Timer  (32-bit timer) */
void ${TOUCH_ADC_TMR_INSTANCE}_InterruptSvcRoutine(uint32_t status, uintptr_t context)
{
    /* Detect position  */
    DRV_Touch_ADC_PositionDetect();
}
</#if>

SYS_MODULE_OBJ DRV_Touch_ADC_Initialize(void)
{
    DRV_TOUCH_ADC_OBJECT * pDrvInstance = (DRV_TOUCH_ADC_OBJECT *) NULL;
    DRV_TOUCH_SAMPLE_POINTS samplePoints;

    pDrvInstance = ( DRV_TOUCH_ADC_OBJECT *)&sADCDriverInstances;

    // set the state of the statemachine to start the sampling
    pDrvInstance->state = DRV_TOUCH_ADC_STATE_SET_X;
    
    samplePoints.touchCalUlx = ${Val_CAL_ULX_SAMPLE};
    samplePoints.touchCalUly = ${Val_CAL_ULY_SAMPLE};

    samplePoints.touchCalUrx = ${Val_CAL_URX_SAMPLE};
    samplePoints.touchCalUry = ${Val_CAL_URY_SAMPLE};

    samplePoints.touchCalLrx = ${Val_CAL_LRX_SAMPLE};
    samplePoints.touchCalLry = ${Val_CAL_LRY_SAMPLE};

    samplePoints.touchCalLlx = ${Val_CAL_LLX_SAMPLE};
    samplePoints.touchCalLly = ${Val_CAL_LLY_SAMPLE};

    DRV_Touch_ADC_CalibrationSet(&samplePoints);

    <#if PollingMode == "Interrupt">
    ${TOUCH_ADC_TMR_INSTANCE}_CallbackRegister(${TOUCH_ADC_TMR_INSTANCE}_InterruptSvcRoutine, NULL);
    ${TOUCH_ADC_TMR_INSTANCE}_Start();
    </#if>

    return (SYS_MODULE_OBJ)pDrvInstance;
}

void DRV_Touch_ADC_Tasks(void)
{
    static short    prevX = -1;
    static short    prevY = -1;

    short           xpos, ypos;

    // detect a new position
    <#if PollingMode == "Task">
    if ( ! DRV_Touch_ADC_PositionDetect() )
        return ;
    </#if>

    xpos = DRV_Touch_ADC_TouchGetX(0);
    ypos = DRV_Touch_ADC_TouchGetY(0);

    if((xpos == -1) || (ypos == -1))
    {
        ypos = -1;
        xpos = -1;

    }

    if((prevX == xpos) && (prevY == ypos) && (xpos != -1) && (ypos != -1))
    {
        return;
    }

    if((prevX != -1) || (prevY != -1))
    {
        if((xpos != -1) && (ypos != -1))
        {

            // Move
            SYS_INP_InjectTouchMove(0, xpos, ypos);
        }
        else
        {

            // Released
            SYS_INP_InjectTouchUp(0, prevX, prevY);
            prevX = xpos;
            prevY = ypos;
            return;
        }
    }
    else
    {
        if((xpos != -1) && (ypos != -1))
        {

            // Pressed
            SYS_INP_InjectTouchDown(0, xpos, ypos);
        }
        else
        {

            // No message
        }
    }

    prevX = xpos;
    prevY = ypos;
}

short DRV_Touch_ADC_PositionDetect(void)
{
    DRV_TOUCH_ADC_OBJECT * pDrvInstance = (DRV_TOUCH_ADC_OBJECT *) &sADCDriverInstances;
    static short tempX;
    static short tempY;
    short temp;

    switch ( pDrvInstance->state ) 
    {
        case DRV_TOUCH_ADC_STATE_IDLE:
        {
            adcX = -1;
            adcY = -1;
            
            break;
        }
        
        case DRV_TOUCH_ADC_STATE_SET_VALUES:
        {            
            if ( !ADCHS_ChannelResultIsReady(${AN_CHANNEL_Y}) )
            {
                break; 
            }
            if ( (uint16_t) ${TOUCH_ADC_PRESS_THRESHOLD} <
                (uint16_t) ADCHS_ChannelResultGet(${AN_CHANNEL_Y})>>2 )
            {
                adcX = -1;
                adcY = -1;
            } 
            else 
            {
                adcX = tempX;
                adcY = tempY;
            }
            
            // If the hardware supports an analog pot, if not skip it
            pDrvInstance->state = DRV_TOUCH_ADC_STATE_SET_X;
            return 1; // touch screen acquisition is done

        }
        
        case DRV_TOUCH_ADC_STATE_SET_X:
        {
            ADCHS_ModulesDisable(${ADCModuleSetting}_MASK);

            //x+
            BSP_ADC_TOUCH_XPLUS_InputEnable();

            //y+
            BSP_ADC_TOUCH_YPLUS_InputEnable();

            //x-
            BSP_ADC_TOUCH_XMINUS_InputEnable();

            //y-
            BSP_ADC_TOUCH_YMINUS_Clear();
            
            //y-
            BSP_ADC_TOUCH_YMINUS_OutputEnable();

            ADCHS_ModulesEnable(${ADCModuleSetting}_MASK);

            ADCHS_GlobalEdgeConversionStart();

            pDrvInstance->state = DRV_TOUCH_ADC_STATE_CHECK_X;

            break;
        }

        case DRV_TOUCH_ADC_STATE_CHECK_X:
        {
            if ( !ADCHS_ChannelResultIsReady(${AN_CHANNEL_X}) )
            {
                break;
            }

            if ((uint16_t) ${TOUCH_ADC_PRESS_THRESHOLD} >
                (uint16_t) ADCHS_ChannelResultGet(${AN_CHANNEL_X})>>2)
            {
                //y+
                BSP_ADC_TOUCH_YPLUS_Set();

                BSP_ADC_TOUCH_YPLUS_OutputEnable();

                tempX = -1;
                pDrvInstance->state = DRV_TOUCH_ADC_STATE_RUN_X;
                
            } 
            else 
            {
                adcX = -1;
                adcY = -1;

                pDrvInstance->state = DRV_TOUCH_ADC_STATE_SET_X;
                return 1; // touch screen acquisition is done
    
            }
            break;
        }

        case DRV_TOUCH_ADC_STATE_CHECK_Y:
        {
            if ( !ADCHS_ChannelResultIsReady(${AN_CHANNEL_Y}) )
            {
                break;
            }

            if ((uint16_t) ${TOUCH_ADC_PRESS_THRESHOLD} >
                (uint16_t) ADCHS_ChannelResultGet(${AN_CHANNEL_Y})>>2)
            {
                //x+
                BSP_ADC_TOUCH_XPLUS_Set();

                BSP_ADC_TOUCH_XPLUS_OutputEnable();

                tempY = -1;
                pDrvInstance->state = DRV_TOUCH_ADC_STATE_RUN_Y;
            } 
            else 
            {
                adcX = -1;
                adcY = -1;

                pDrvInstance->state = DRV_TOUCH_ADC_STATE_SET_X;
                return 1; // touch screen acquisition is done
    
                break;
            }
        }

        case DRV_TOUCH_ADC_STATE_RUN_X:
        case DRV_TOUCH_ADC_STATE_RUN_Y:
        {
            ADCHS_GlobalEdgeConversionStart();

            pDrvInstance->state = (pDrvInstance->state == DRV_TOUCH_ADC_STATE_RUN_X) ?
                DRV_TOUCH_ADC_STATE_GET_X : DRV_TOUCH_ADC_STATE_GET_Y;

            break;
        }

        case DRV_TOUCH_ADC_STATE_GET_X:
        {
            if (!ADCHS_ChannelResultIsReady(${AN_CHANNEL_X}))
            {
                break;
            }
            temp = (uint16_t) ADCHS_ChannelResultGet(${AN_CHANNEL_X})>>2;
            if (temp != tempX) 
            {
                tempX = temp;
                pDrvInstance->state = DRV_TOUCH_ADC_STATE_RUN_X;
                break;
            }

            //y+
            BSP_ADC_TOUCH_YPLUS_InputEnable();
            
            ADCHS_GlobalEdgeConversionStart();

            pDrvInstance->state = DRV_TOUCH_ADC_STATE_SET_Y;
            
            break;
        }
        
        case DRV_TOUCH_ADC_STATE_GET_Y:
        {

            if (!ADCHS_ChannelResultIsReady(${AN_CHANNEL_Y}))
            {
                break;
            }

            temp = (uint16_t) ADCHS_ChannelResultGet(${AN_CHANNEL_Y})>>2;

            if (temp != tempY) 
            {
                tempY = temp;
                pDrvInstance->state = DRV_TOUCH_ADC_STATE_RUN_Y;
                break;
            }

            //x+
            BSP_ADC_TOUCH_XPLUS_InputEnable();

            ADCHS_GlobalEdgeConversionStart();

            pDrvInstance->state = DRV_TOUCH_ADC_STATE_SET_VALUES;
            
            break;
        }

        case DRV_TOUCH_ADC_STATE_SET_Y:
        {
            if (!ADCHS_ChannelResultIsReady(${AN_CHANNEL_X}))
            {
                break;
            }
                   
            if ( (uint16_t) ${TOUCH_ADC_PRESS_THRESHOLD} <
                (uint16_t) ADCHS_ChannelResultGet(${AN_CHANNEL_X})>>2 )
            {
                adcX = -1;
                adcY = -1;
                pDrvInstance->state = DRV_TOUCH_ADC_STATE_SET_X;
                return 1; // touch screen acquisition is done
                break;
            }

            ADCHS_ModulesDisable(${ADCModuleSetting}_MASK);

            //x+
            BSP_ADC_TOUCH_XPLUS_InputEnable();

            //y+
            BSP_ADC_TOUCH_YPLUS_InputEnable();

            //x-
            BSP_ADC_TOUCH_XMINUS_Clear();
            
            BSP_ADC_TOUCH_XMINUS_OutputEnable();
            
            //y-
            BSP_ADC_TOUCH_YMINUS_InputEnable();

            ADCHS_ModulesEnable(${ADCModuleSetting}_MASK);

            ADCHS_GlobalEdgeConversionStart();

            pDrvInstance->state = DRV_TOUCH_ADC_STATE_CHECK_Y;
            break;
        }
        
        default:
        {
            pDrvInstance->state = DRV_TOUCH_ADC_STATE_SET_X;
            return 1; // touch screen acquisition is done
        }
    }

    return 0; // touch screen acquisition is not done
}

short DRV_Touch_ADC_TouchGetX( uint8_t touchNumber )
{
    long result;


    result = DRV_Touch_ADC_TouchGetRawX();

    if (result >= 0) {
        result = (long) ((((long) coefC * result) + coefD) >> 8);
    }
    return ((short) result);
}

short DRV_Touch_ADC_TouchGetRawX(void)
{
    return adcX;
}

short DRV_Touch_ADC_TouchGetY( uint8_t touchNumber )
{
    long result;

    result = DRV_Touch_ADC_TouchGetRawY();

    if (result >= 0) {
        result = (long) ((((long) coefA * result) + (long) coefB) >> 8);
    }
    return ((short) result);
}

short DRV_Touch_ADC_TouchGetRawY(void)
{

    return adcY;
}

void DRV_Touch_ADC_CalibrationSet(DRV_TOUCH_SAMPLE_POINTS * samplePoints)
{

    xRawTouch[0] = samplePoints->touchCalUlx;
    yRawTouch[0] = samplePoints->touchCalUly;

    xRawTouch[1] = samplePoints->touchCalUrx;
    yRawTouch[1] = samplePoints->touchCalUry;

    xRawTouch[3] = samplePoints->touchCalLlx;
    yRawTouch[3] = samplePoints->touchCalLly;

    xRawTouch[2] = samplePoints->touchCalLrx;
    yRawTouch[2] = samplePoints->touchCalLry;

    DRV_Touch_ADC_CoefficientSet(xRawTouch, yRawTouch);
}

// *****************************************************************************
// *****************************************************************************
// Section: Local functinos
// *****************************************************************************
// *****************************************************************************

void DRV_Touch_ADC_CoefficientSet(short xraw[], short yraw[])
{
    long trA, trB, trC, trD;
    long trAhold, trBhold, trChold, trDhold;
    long test1, test2;

    short xPoint[DRV_TOUCH_ADC_SAMPLE_POINTS];
    short yPoint[DRV_TOUCH_ADC_SAMPLE_POINTS];

    // set expected user input 10% from corners
    yPoint[0] = yPoint[1] = CAL_Y_INSET;
    yPoint[2] = yPoint[3] = (${Val_Height} - CAL_Y_INSET);
    xPoint[0] = xPoint[3] = CAL_X_INSET;
    xPoint[1] = xPoint[2] = (${Val_Width} - CAL_X_INSET);

    // get slopes for expected and actual  m = (y2-y1/x2-x1)
    // get y-intercepts

    test1 = (long) yPoint[0] - (long) yPoint[3];
    test2 = (long) yraw[0] - (long) yraw[3];

    trA = ((long) ((long) test1 * SCALE_FACTOR) / test2);
    trB = ((long) ((long) yPoint[0] * SCALE_FACTOR) - (trA * (long) yraw[0]));

    test1 = (long) xPoint[0] - (long) xPoint[2];
    test2 = (long) xraw[0] - (long) xraw[2];

    trC = ((long) ((long) test1 * SCALE_FACTOR) / test2);
    trD = ((long) ((long) xPoint[0] * SCALE_FACTOR) - (trC * (long) xraw[0]));

    // temp hold slopes and y-intercepts
    trAhold = trA;
    trBhold = trB;
    trChold = trC;
    trDhold = trD;

    // get slopes for expected and actual  m = (y2-y1/x2-x1)
    // get y-intercepts

    test1 = (long) yPoint[1] - (long) yPoint[2];
    test2 = (long) yraw[1] - (long) yraw[2];

    trA = ((long) (test1 * SCALE_FACTOR) / test2);
    trB = ((long) ((long) yPoint[1] * SCALE_FACTOR) - (trA * (long) yraw[1]));

    test1 = (long) xPoint[1] - (long) xPoint[3];
    test2 = (long) xraw[1] - (long) xraw[3];

    trC = ((long) ((long) test1 * SCALE_FACTOR) / test2);
    trD = ((long) ((long) xPoint[1] * SCALE_FACTOR) - (trC * (long) xraw[1]));

    // save the average slopes and y-intercepts and use the average slopes
    coefA = (trA + trAhold) >> 1;
    coefB = (trB + trBhold) >> 1;
    coefC = (trC + trChold) >> 1;
    coefD = (trD + trDhold) >> 1;

}

