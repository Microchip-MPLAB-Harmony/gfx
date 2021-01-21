/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
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
 Module for Microchip Graphics Library - Legato User Interface Library

  Company:
    Microchip Technology Inc.

  File Name:
    legato_widget_pie_chart.h

  Summary:


  Description:
    This module implements pie chart drawing widget functions.
*******************************************************************************/
/** \file legato_widget_pie_chart.h
 * @brief Pie chart functions and definitions.
 *
 * @details This module implements pie chart drawing widget functions.
 */

#ifndef LEGATO_WIDGET_PIE_CHART_H
#define LEGATO_WIDGET_PIE_CHART_H

#include "gfx/legato/common/legato_common.h"

#if LE_PIECHART_WIDGET_ENABLED == 1

#include "gfx/legato/font/legato_font.h"
#include "gfx/legato/widget/legato_widget.h"

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************
/**
 * @brief Used to define a pie chart pie.
 * @details .
 */
typedef struct lePieChartPie
{
    uint32_t value;
    uint32_t radius;
    uint32_t offset;
    const leScheme* scheme;
    uint32_t startAngle;
    uint32_t spanAngle;
    uint32_t percentOffset;
    uint32_t percentValue;
} lePieChartPie;

typedef struct lePieChartWidget lePieChartWidget;

// *****************************************************************************
/* Function Pointer:
    lePieChartWidget_PressedEvent

  Summary:
    Chart pressed event function callback type
*/
/**
 * @brief Used to define a pie chart pressed event callback function.
 * @details .
 */
typedef void (*lePieChartWidget_PressedEvent)(lePieChartWidget*, uint32_t);


/* internal use only */
/**
  * @cond INTERNAL
  *
  */
typedef struct lePieChartWidget lePieChartWidget;

#define LE_PIECHARTWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    int32_t             (*getStartAngle)(const THIS_TYPE* _this); \
    leResult            (*setStartAngle)(THIS_TYPE* _this, int32_t ang); \
    int32_t             (*getCenterAngle)(const THIS_TYPE* _this); \
    leResult            (*setCenterAngle)(THIS_TYPE* _this, int32_t ang); \
    int32_t             (*addEntry)(THIS_TYPE* _this); \
    leResult            (*clear)(THIS_TYPE* _this); \
    uint32_t            (*getEntryValue)(const THIS_TYPE* _this, int32_t idx); \
    leResult            (*setEntryValue)(THIS_TYPE* _this, int32_t idx, uint32_t val); \
    uint32_t            (*getEntryRadius)(const THIS_TYPE* _this, int32_t idx); \
    leResult            (*setEntryRadius)(THIS_TYPE* _this, int32_t idx, uint32_t rad); \
    uint32_t            (*getEntryOffset)(const THIS_TYPE* _this, int32_t idx); \
    leResult            (*setEntryOffset)(THIS_TYPE* _this, int32_t idx, uint32_t offs); \
    leScheme*           (*getEntryScheme)(const THIS_TYPE* _this, int32_t idx); \
    leResult            (*setEntryScheme)(THIS_TYPE* _this, int32_t idx, const leScheme* schm); \
    lePieChartWidget_PressedEvent (*getPressedEventCallback)(const THIS_TYPE* _this); \
    leResult            (*setPressedEventCallback)(THIS_TYPE* _this, lePieChartWidget_PressedEvent cb); \
    leFont*             (*getLabelFont)(const THIS_TYPE* _this); \
    leResult            (*setLabelFont)(THIS_TYPE* _this, const leFont* fnt); \
    leBool              (*getLabelsVisible)(const THIS_TYPE* _this); \
    leResult            (*setLabelsVisible)(THIS_TYPE* _this, leBool vis); \
    uint32_t            (*getLabelsOffset)(const THIS_TYPE* _this); \
    leResult            (*setLabelsOffset)(THIS_TYPE* _this, uint32_t offs); \
    
typedef struct lePieChartWidgetVTable
{
	LE_PIECHARTWIDGET_VTABLE(lePieChartWidget)
} lePieChartWidgetVTable; 

    /**
      * @endcond
      *
      */

// *****************************************************************************
/**
 * @brief This struct represents a pie chart widget
 * @details A chart widget draws a chart of the specified origin and radius inside the
 * widget bounds.  All coordinates are expressed in local widget space. The color of
 * the chart is determined by the widget scheme's 'foreground' color.
 */
typedef struct lePieChartWidget
{
    leWidget widget; // base widget header

    const lePieChartWidgetVTable* fn;

    uint32_t startAngle;                    //the start angle of the chart
    int32_t centerAngle;                    //the center angle of the chart

    leArray pieArray;                       //list of pie/data

    //Label properties
    leBool labelsVisible;                   // are labels visible
    uint32_t labelsOffset;                  // offset of labels from center of pie
    const leFont* labelFont;                // label

    lePieChartWidget_PressedEvent pressedCallback;
} lePieChartWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new lePieChartWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * lePieChartWidget* wgt = lePieChartWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
lePieChartWidget* lePieChartWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the lePieChartWidget <span class="param">wgt</span>.
 * @code
 * lePieChartWidget* wgt;
 * lePieChartWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void lePieChartWidget_Constructor(lePieChartWidget* wgt);


#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/* Virtual Member Function:
    int32_t getStartAngle(const lePieChartWidget* _this)

  Summary:
     Gets the start angle

  Description:
     Gets the start angle

  Parameters:
    const lePieChartWidget* _this - The pie chart widget to operate on

  Remarks:
    Usage - _this->fn->getStartAngle(_this);

  Returns:
    int32_t - the angle value
*/
/**
 * @brief Get start angle.
 * @details Gets the start angle using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * int32_t ang = _this->fn->getStartAngle(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the angle value;
 */
virtual int32_t getStartAngle(const lePieChartWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult setStartAngle(lePieChartWidget* _this,
                           int32_t ang)

  Summary:
     Sets the start angle

  Description:
     Sets the start angle

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on
    int32_t ang - the angle value

  Remarks:
    Usage - _this->fn->setStartAngle(_this, ang);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set start angle.
 * @details Sets the start angle to <span class="param">cnt</span>
 * using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * leResult res = _this->fn->setVisibleItemCount(_this, cnt);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param cnt is the item count.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setVisibleItemCount(lePieChartWidget* _this,
                                     uint32_t cnt);


// *****************************************************************************
/* Virtual Member Function:
    int32_t getCenterAngle(const lePieChartWidget* _this)

  Summary:
     Gets the center angle

  Description:
     Gets the center angle

  Parameters:
    const lePieChartWidget* _this - The pie chart widget to operate on

  Remarks:
    Usage - _this->fn->getCenterAngle(_this);

  Returns:
    int32_t - the angle value
*/
/**
 * @brief Get center angle.
 * @details Gets the center angle using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * int32_t ang = _this->fn->getStartAngle(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the angle value;
 */
virtual int32_t getCenterAngle(const lePieChartWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setCenterAngle(lePieChartWidget* _this,
                            int32_t ang)

  Summary:
     Sets the start angle

  Description:
     Sets the start angle

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on
    int32_t ang - the angle value

  Remarks:
    Usage - _this->fn->setCenterAngle(_this, ang);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set center angle.
 * @details Sets the center angle to <span class="param">ang</span>
 * using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * leResult res = _this->fn->setVisibleItemCount(_this, ang);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param ang is the angle value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setVisibleItemCount(lePieChartWidget* _this,
                                     uint32_t cnt);

// *****************************************************************************
/* Virtual Member Function:
    int32_t addEntry(lePieChartWidget* _this)

  Summary:
     Adds a pie entry

  Description:
     Adds a pie entry

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on

  Remarks:
    Usage - _this->fn->addEntry(_this);

  Returns:
    int32_t - the index of the new entry
*/
/**
 * @brief Get center angle.
 * @details Gets the center angle using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * int32_t ang = _this->fn->getStartAngle(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the index of the new entry;
 */
virtual int32_t getCenterAngle(const lePieChartWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult clear(lePieChartWidget* _this)

  Summary:
     Clears all pie entries

  Description:
     Clears all pie entries

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on

  Remarks:
    Usage - _this->fn->clear(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Clear all pie entries.
 * @details Clears all pie entries using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * leResult res = _this->fn->clear(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult clear(lePieChartWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getEntryValue(const lePieChartWidget* _this,
                           int32_t idx)

  Summary:
     Gets an entry value

  Description:
     Gets an entry value

  Parameters:
    const lePieChartWidget* _this - The pie chart widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getEntryValue(_this, idx);

  Returns:
    uint32_t - the value
*/
/**
 * @brief Get entry value.
 * @details Gets the entry value at <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * uint32_t val = _this->fn->getEntryValue(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the value;
 */
virtual uint32_t getEntryValue(const lePieChartWidget* _this,
                               int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setEntryValue(lePieChartWidget* _this,
                           int32_t idx,
                           uint32_t val)

  Summary:
     Sets an entry value

  Description:
     Sets an entry value

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on
    int32_t idx - the index
    uint32_t val - the value

  Remarks:
    Usage - _this->fn->setEntryValue(_this, idx, val);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set entry value.
 * @details Sets the entry value at <span class="param">idx</span>
 * to <span class="param">val</span> using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * leResult res = _this->fn->setEntryValue(_this, idx, val);
 * @endcode
 * @param _this is the widget pointer to query.
 * @paaram idx is the index
 * @param val is the value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setEntryValue(lePieChartWidget* _this,
                               int32_t idx,
                               uint32_t val);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getEntryRadius(const lePieChartWidget* _this,
                            int32_t idx)

  Summary:
     Gets an entry radius

  Description:
     Gets an entry radius

  Parameters:
    const lePieChartWidget* _this - The pie chart widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getEntryRadius(_this, idx);

  Returns:
    uint32_t - the radius value
*/
/**
 * @brief Get entry radius.
 * @details Gets the entry radius at <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * uint32_t rad = _this->fn->getEntryRadius(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the radius value;
 */
virtual uint32_t getEntryRadius(const lePieChartWidget* _this,
                               int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setEntryRadius(lePieChartWidget* _this,
                            int32_t idx,
                            uint32_t rad)

  Summary:
     Sets an entry radius

  Description:
     Sets an entry radius

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on
    int32_t idx - the index
    uint32_t rad - the radius value

  Remarks:
    Usage - _this->fn->setEntryRadius(_this, idx, rad);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set entry radius.
 * @details Sets the entry radius at <span class="param">idx</span>
 * to <span class="param">rad</span> using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leResult res = _this->fn->setEntryRadius(_this, idx, rad);
 * @endcode
 * @param _this is the widget pointer to query.
 * @paaram idx is the index
 * @param rad is the value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setEntryRadius(lePieChartWidget* _this,
                                int32_t idx,
                                uint32_t rad);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getEntryOffset(const lePieChartWidget* _this,
                            int32_t idx)

  Summary:
     Gets an entry offset

  Description:
     Gets an entry offset

  Parameters:
    const lePieChartWidget* _this - The pie chart widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getEntryOffset(_this, idx);

  Returns:
    uint32_t - the offset value
*/
/**
 * @brief Get entry offset.
 * @details Gets the entry offset at <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * uint32_t off = _this->fn->getEntryRadius(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param idx the index
 * @returns the offset value;
 */
virtual uint32_t getEntryOffset(const lePieChartWidget* _this,
                                int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setEntryOffset(lePieChartWidget* _this,
                            int32_t idx,
                            uint32_t offs)

  Summary:
     Sets an entry offset

  Description:
     Sets an entry offset

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on
    int32_t idx - the index
    uint32_t offs - the offset value

  Remarks:
    Usage - _this->fn->setEntryOffset(_this, idx, offs);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set entry offset.
 * @details Sets the entry offset at <span class="param">idx</span>
 * to <span class="param">offs</span> using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * leResult res = _this->fn->setEntryRadius(_this, idx, rad);
 * @endcode
 * @param _this is the widget pointer to query.
 * @paaram idx is the index
 * @param offs is the offset value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setEntryOffset(lePieChartWidget* _this,
                                int32_t idx,
                                uint32_t offs);

// *****************************************************************************
/* Virtual Member Function:
    leScheme* getEntryScheme(const lePieChartWidget* _this,
                             int32_t idx)

  Summary:
     Gets an entry scheme

  Description:
     Gets an entry scheme

  Parameters:
    const lePieChartWidget* _this - The pie chart widget to operate on
    int32_t idx - the index

  Remarks:
    Usage - _this->fn->getEntryScheme(_this, idx);

  Returns:
    leScheme* - the scheme pointer
*/
/**
 * @brief Get entry scheme.
 * @details Gets the scheme offset at <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * leScheme* sch = _this->fn->getEntryScheme(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param idx the index
 * @returns the scheme pointer;
 */
virtual leScheme* getEntryScheme(const lePieChartWidget* _this,
                                 int32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setEntryScheme(lePieChartWidget* _this,
                            int32_t idx,
                            const leScheme* schm)

  Summary:
     Sets an entry scheme

  Description:
     Sets an entry scheme

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on
    int32_t idx - the index
    const leScheme* schm - the scheme pointer

  Remarks:
    Usage - _this->fn->setEntryScheme(_this, idx, schm);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set entry scheme.
 * @details Sets the entry scheme at <span class="param">idx</span>
 * to <span class="param">schm</span> using <span class="param">_this</span>.
 * @code
 * const leListWheelWidget* _this;
 * int32_t idx;
 * leScheme* schm;
 * leResult res = _this->fn->setEntryScheme(_this, idx, schm);
 * @endcode
 * @param _this is the widget pointer to query.
 * @paaram idx is the index
 * @param schm is the offset value.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setEntryScheme(lePieChartWidget* _this,
                                int32_t idx,
                                const leScheme* schm);

// *****************************************************************************
/* Virtual Member Function:
    leFont* getLabelFont(const lePieChartWidget* _this)

  Summary:
     Gets the label font

  Description:
     Gets the label font

  Parameters:
    const lePieChartWidget* _this - The pie chart widget to operate on

  Remarks:
    Usage - _this->fn->getLabelFont(_this);

  Returns:
    leFont* - the font pointer
*/
/**
 * @brief Get label font.
 * @details Gets the label font using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * leScheme* sch = _this->fn->getLabelFont(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the font pointer;
 */
virtual leFont* getLabelFont(const lePieChartWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setLabelFont(lePieChartWidget* _this,
                          const leFont* fnt)

  Summary:
     Sets the label font

  Description:
     Sets the label font

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on
    const leFont* fnt - the font pointer

  Remarks:
    Usage - _this->fn->setLabelFont(_this, fnt);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set label font.
 * @details Sets the label font
 * to <span class="param">fnt</span> using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * int32_t idx;
 * leFont* fnt;
 * leResult res = _this->fn->setLabelFont(_this, fnt);
 * @endcode
 * @param _this is the widget pointer to query.
 * @paaram fnt the font pointer
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setLabelFont(lePieChartWidget* _this,
                              const leFont* fnt);

// *****************************************************************************
/* Virtual Member Function:
    leBool getLabelsVisible(const lePieChartWidget* _this)

  Summary:
     Gets the label visible setting value

  Description:
     Gets the label visible setting value

  Parameters:
    const lePieChartWidget* _this - The pie chart widget to operate on

  Remarks:
    Usage - _this->fn->getLabelsVisible(_this);

  Returns:
    leBool - the visibility setting
*/
/**
 * @brief Get label visible setting value.
 * @details Gets the label visible setting value using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * leBool setting = _this->fn->getLabelsVisible(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE.
 */
virtual leBool getLabelsVisible(const lePieChartWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setLabelsVisible(lePieChartWidget* _this,
                              leBool vis)

  Summary:
     Sets the label visible setting value

  Description:
     Sets the label visible setting value

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on
    leBool vis - the visibility setting

  Remarks:
    Usage - _this->fn->setLabelsVisible(_this, vis);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set label visible setting value.
 * @details Sets the label visible setting value
 * to <span class="param">fnt</span> using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * leBool vis;
 * leResult res = _this->fn->setLabelsVisible(_this, vis);
 * @endcode
 * @param _this is the widget pointer to query.
 * @paaram vis  the visibility setting
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setLabelsVisible(lePieChartWidget* _this,
                                  leBool vis);

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getLabelsOffset(const lePieChartWidget* _this)

  Summary:
     Gets the label offset value

  Description:
     Gets the label offset value

  Parameters:
    const lePieChartWidget* _this - The pie chart widget to operate on

  Remarks:
    Usage - _this->fn->getLabelsOffset(_this);

  Returns:
    uint32_t - the offset value
*/
/**
 * @brief Get label font.
 * @details Gets the label font using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * uint32_t val = _this->fn->getLabelsOffset(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns the offset value;
 */
virtual uint32_t getLabelsOffset(const lePieChartWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setLabelsOffset(lePieChartWidget* _this,
                             uint32_t offs)

  Summary:
     Sets the label offset value

  Description:
     Sets the label offset value

  Parameters:
    lePieChartWidget* _this - The pie chart widget to operate on
    uint32_t offs - the offset value

  Remarks:
    Usage - _this->fn->setLabelsOffset(_this, offs);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set label visible setting value.
 * @details Sets the label visible setting value
 * to <span class="param">fnt</span> using <span class="param">_this</span>.
 * @code
 * const lePieChartWidget* _this;
 * uint32_t offs;
 * leResult res = _this->fn->setLabelsOffset(_this, offs);
 * @endcode
 * @param _this is the widget pointer to query.
 * @paaram offs the offset value
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setLabelsOffset(lePieChartWidget* _this,
                                 uint32_t offs);

/**
 * @brief Get pressed event callback pointer.
 * @details Gets the pressed event callback pointer using <span class="param">_this</span>.
 * @code
 * lePieChartWidget* _this;
 * lePieChartWidget_PressedEvent cb = _this->fn->getPressedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual lePieChartWidget_PressedEvent getPressedEventCallback
                                        (const lePieChartWidget* _this);

/**
 * @brief Set pressed event callback pointer.
 * @details Sets the pressed event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * lePieChartWidget* _this;
 * lePieChartWidget_PressedEvent cb;
 * leResult res = _this->fn->setPressedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setPressedEventCallback(lePieChartWidget* _this,
                                         lePieChartWidget_PressedEvent cb);

#undef THIS_TYPE
#endif

#endif // LE_PIECHART_WIDGET_ENABLED
#endif /* LEGATO_WIDGET_PIE_CHART_H */
