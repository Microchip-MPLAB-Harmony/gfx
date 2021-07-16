/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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

/*******************************************************************************
 Module for Microchip Graphics Library - Input Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    drv_touch_adc.h

  Summary:
    Touch controller ADC driver interface file.

Description:
    This file consist of touch controller ADC driver interfaces. It
    implements the driver interfaces which reads the touch input data from
    resistive touch panel through the ADC controller.
*******************************************************************************/

/** \file drv_touch_adc.h
 * @brief Touch controller ADC driver interface file.
 *
 * @details This file consist of touch controller ADC driver interfaces. It
 * implements the driver interfaces which reads the touch input data from
 * resistive touch panel through the ADC controller.
 */

#ifndef _DRV_TOUCH_ADC_H
#define _DRV_TOUCH_ADC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief This struct represents the four calibration coefficient points.
 * @details .
 */
typedef struct
{
    short touchCalUlx;
    short touchCalUly;
    short touchCalUrx;
    short touchCalUry;
    short touchCalLrx;
    short touchCalLry;
    short touchCalLlx;
    short touchCalLly;
} DRV_TOUCH_SAMPLE_POINTS;

/**
 * @brief This enum represents the current status of the current touch point.
 * @details This enumeration is the return type for the status routine
 for the current touch point.
 */
typedef enum
{
    /*An unspecified error has occurred.*/
    DRV_TOUCH_POSITION_ERROR  = -1,

    // The module position is not avaliable
    DRV_TOUCH_POSITION_NONE = 0,

    // The module has a single touch point
    DRV_TOUCH_POSITION_SINGLE = 2

} DRV_TOUCH_POSITION_STATUS;


/**
 * @brief This enum represents the the current state of the touch.
 * @details This enumeration is the return type for the TouchGetPen routine..
 */
typedef enum
{
    /* Pen up state */
    DRV_TOUCH_PEN_UNKNOWN  = -1,

    /* Pen up state */
    DRV_TOUCH_PEN_UP  = 0,

    /* Pen down state */
    DRV_TOUCH_PEN_DOWN = 1,

} DRV_TOUCH_PEN_STATE;

/**
 * @brief This struct represents the data required to initialize or reinitialize the Touch ADC Driver.
 * @details This data type defines the data required to initialize or reinitialize the
 * Touch ADC Driver.
 */
typedef struct _DRV_TOUCH_ADC_INIT
{
    void                   (*drvCalibrationSet) (  DRV_TOUCH_SAMPLE_POINTS * samplePoints );
    short                  (*drvTouchGetX)( uint8_t touchNumber);
    short                  (*drvTouchGetY)( uint8_t touchNumber);
    DRV_TOUCH_POSITION_STATUS  (*drvTouchStatus)( const SYS_MODULE_INDEX index );

} DRV_TOUCH_ADC_INIT;


/**
 * @brief Initialize the Resistive Touch ADC Driver.
 * @details Initialzes the resistive touch driver.
 * @code
 * DRV_Touch_ADC_Initialize();
 * @endcode
 * @remark This routine must be called before any other DRV_TOUCH_ADC routine is called.
 * @return the system module object.
 */
SYS_MODULE_OBJ DRV_Touch_ADC_Initialize(void);

/**
 * @brief Inject available touch position.
 * @details Injects the available touch position to the System Input Service.
 * @code
 * void SYS_Tasks ( void )
 * {
 *      DRV_Touch_ADC_Tasks();
 * }
 * @endcode
 * @remark This routine is automatically inserted into the SYS_Tasks() function for Harmony applications.
 */
void DRV_Touch_ADC_Tasks (void);

/**
 * @brief Get current touch status.
 * @details Gets the current touch input status position.
 * @code
 * DRV_TOUCH_POSITION_STATUS status
 * status = DRV_Touch_ADC_TouchStatus();
 * @endcode
 */
DRV_TOUCH_POSITION_STATUS DRV_Touch_ADC_TouchStatus(void);


/**
 * @brief Get X coordinate.
 * @details Gets the X coordinate value of the last touch position.
 * @code
 * short xPosition
 * xPosition = DRV_Touch_ADC_TouchGetX();
 * @endcode
 * @returns x coordinate if last position, otherwise -1.
 */
short DRV_Touch_ADC_TouchGetX( uint8_t touchNumber );

/**
 * @brief Get raw x coordinate.
 * @details Gets the raw x coordinate value of the last touch position.
 * @code
 * short xRawPosition
 * xRawPosition = DRV_Touch_ADC_TouchGetRawX();
 * @endcode
 * @remark The raw position is useful for performing calibrations.
 * @returns x coordinate if last position, otherwise -1.
 */
short DRV_Touch_ADC_TouchGetRawX(void);

/**
 * @brief Get Y coordinate.
 * @details Gets the Y coordinate value of the last touch position.
 * @code
 * short yPosition
 * yPosition = DRV_Touch_ADC_TouchGetY();
 * @endcode
 * @returns y coordinate if last position, otherwise -1.
 */
short DRV_Touch_ADC_TouchGetY( uint8_t touchNumber );

/**
 * @brief Get raw y coordinate.
 * @details Gets the raw y coordinate value of the last touch position.
 * @code
 * short yRawPosition
 * yRawPosition = DRV_Touch_ADC_TouchGetRawY();
 * @endcode
 * @remark The raw position is useful for performing calibrations.
 * @returns y coordinate if last position, otherwise -1.
 */
short DRV_Touch_ADC_TouchGetRawY( void );

/**
 * @brief Load calibration parameters from Non-volatile Memory.
 * @details Loads calibration parameters from Non-volatile Memory for operation.
 * @code
 * DRV_Touch_ADC_CalibrationSet();
 * @endcode
 * @remark Uses NVM functions.
 */
void DRV_Touch_ADC_CalibrationSet(DRV_TOUCH_SAMPLE_POINTS * samplePoints);

/**
 * @brief Read a new position.
 * @details Reads a new position if available.
 * @code
 * short positionRead;
 * positionRead = DRV_Touch_ADC_PositionDetect();
 * @endcode
 * @remark Implements the timer interrupt callback.
 */
short DRV_Touch_ADC_PositionDetect(void);

/**
 * @brief Set calibration coefficients.
 * @details Sets calibration coefficient for this driver position detection.
 * @code
 * DRV_Touch_ADC_CoefficientSet();
 * @endcode
 */
void DRV_Touch_ADC_CoefficientSet(short rawValuesX[], short rawValuesY[]);

#ifdef __cplusplus
    }
#endif
    
#endif //_DRV_Touch_ADC_H
