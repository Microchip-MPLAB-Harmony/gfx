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
    legato_widget_imagesequence.h

  Summary:


  Description:
    This module implements image sequence (slide show) widget drawing functions.
*******************************************************************************/

/** \file legato_widget_imagesequence.h
 * @brief Imagesequence widget functions and definitions.
 *
 * @details This module implements image sequence (slide show) widget drawing functions.
 */

#ifndef LEGATO_IMAGESEQUENCE_H
#define LEGATO_IMAGESEQUENCE_H

#include "gfx/legato/common/legato_common.h"

#if LE_IMAGESEQUENCE_WIDGET_ENABLED

#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/widget/legato_widget.h"
#include "gfx/legato/string/legato_string.h"

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
#define LE_IMAGESEQ_RESTART -1
/**
  * @endcond
  *
  */

typedef struct leImageSequenceWidget leImageSequenceWidget;

// *****************************************************************************
/* Function Pointer:
    leImageSequenceImageChangedEvent_FnPtr

  Summary:
    Image changed event function callback type
*/
/**
 * @brief This function represents a image change event callback.
 * @details The callback is used indicate a image has changed.
 * @details .
 */
typedef void (*leImageSequenceImageChangedEvent_FnPtr)(leImageSequenceWidget*);

// *****************************************************************************
// *****************************************************************************
// Section: Data Types and Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Enumeration:
    leImageSequenceEntry

  Summary:
    Image sequence entry definition

  Description:
    Defines a single entry for the image sequence widget

  Remarks:
    None.
*/
/**
 * @brief This struct represents a image sequence entry.
 * @details This is used to define a single entry for the image sequence widget
 */
typedef struct leImageSequenceEntry
{
    const leImage* image; // image asset pointer

    uint32_t delay; // how many time units to display this entry

    leHAlignment halign; // the horizontal alignment for this entry
    leVAlignment valign; // the vertical alignment for this entry
} leImageSequenceEntry;

/* internal use only */
/**
  * @cond INTERNAL
  *
  */
#define LE_IMAGESEQUENCEWIDGET_VTABLE(THIS_TYPE) \
    LE_WIDGET_VTABLE(THIS_TYPE) \
    \
    uint32_t      (*getImageCount)(const THIS_TYPE* _this); \
    leResult      (*setImageCount)(THIS_TYPE* _this, uint32_t cnt); \
    leImage*      (*getImage)(const THIS_TYPE* _this, uint32_t idx); \
    leResult      (*setImage)(THIS_TYPE* _this, uint32_t idx, const leImage* img); \
    uint32_t      (*getImageDelay)(const THIS_TYPE* _this, uint32_t idx); \
    leResult      (*setImageDelay)(THIS_TYPE* _this, uint32_t idx, uint32_t dly); \
    leHAlignment  (*getImageHAlignment)(const THIS_TYPE* _this, uint32_t idx); \
    leResult      (*setImageHAlignment)(THIS_TYPE* _this, uint32_t idx, leHAlignment align); \
    leVAlignment  (*getImageVAlignment)(const THIS_TYPE* _this, uint32_t idx); \
    leResult      (*setImageVAlignment)(THIS_TYPE* _this, uint32_t idx, leVAlignment align); \
    leResult      (*stop)(THIS_TYPE* _this); \
    leResult      (*play)(THIS_TYPE* _this); \
    leResult      (*rewind)(THIS_TYPE* _this); \
    leBool        (*isPlaying)(const THIS_TYPE* _this); \
    leBool        (*getRepeat)(const THIS_TYPE* _this); \
    leResult      (*setRepeat)(THIS_TYPE* _this, leBool rpt); \
    leResult      (*showImage)(THIS_TYPE* _this, uint32_t idx); \
    leResult      (*showNextImage)(THIS_TYPE* _this); \
    leResult      (*showPreviousImage)(THIS_TYPE* _this); \
    leImageSequenceImageChangedEvent_FnPtr (*getImageChangedEventCallback)(const THIS_TYPE* _this); \
    leResult      (*setImageChangedEventCallback)(THIS_TYPE* _this, leImageSequenceImageChangedEvent_FnPtr cb); \
    
typedef struct leImageSequenceWidgetVTable
{
	LE_IMAGESEQUENCEWIDGET_VTABLE(leImageSequenceWidget)
} leImageSequenceWidgetVTable; 

/**
  * @endcond
  *
  */

// *****************************************************************************
/**
 * @brief This struct represents a image sequence widget
 * @details An image sequence widget is similar to an image widget with the
 * additional capability of showing a sequence of images and automating
 * the transition between them. This widget is dependent on the time value
 * provided to leUpdate.  If leUpdate is not provided with time  able to
 * automatically cycle.
 */
typedef struct leImageSequenceWidget
{
    leWidget widget; // widget base class

    const leImageSequenceWidgetVTable* fn;

    uint32_t count; // number of image entries for this widget
    leImageSequenceEntry* images; // image entry array

    int32_t activeIdx; // currently displayed entry

    leBool playing; // indicates that the widget is automatically cycling
    uint32_t time; // current cycle time

    leBool repeat; // indicates that the sequence should repeat when it
                   // reaches the end of the sequence

    leImageSequenceImageChangedEvent_FnPtr cb; // callback when the image changes
} leImageSequenceWidget;

// *****************************************************************************
// *****************************************************************************
// Section: Routines
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Create widget.
 * @details Creates a new leImageSequenceWidget and allocates memory for the widget through the
 * current active context.  Application is responsible for managing the widget
 * pointer until the widget is added to a widget tree.
 * @remark use leWidget_Delete() to free this pointer.
 * @code
 * leImageSequenceWidget* wgt = leImageSequenceWidget_New();
 * @endcode
 * @return a widget object pointer.
 */
leImageSequenceWidget* leImageSequenceWidget_New(void);

/**
 * @brief Initialize widget.
 * @details Initializes the leImageSequenceWidget <span class="param">wgt</span>.
 * @code
 * leImageSequenceWidget* wgt;
 * leImageSequenceWidget_Constructor(wgt);
 * @endcode
 * @param wgt is the widget to initialize
 * @return void.
 */
void leImageSequenceWidget_Constructor(leImageSequenceWidget* wgt);

#ifdef _DOXYGEN_
#define THIS_TYPE struct leWidget

// *****************************************************************************
/* Virtual Member Function:
    uint32_t getImageCount(const leImageSequenceWidget* _this)

  Summary:
     Gets the image count

  Description:
     Gets the image count

  Parameters:
    const leImageSequenceWidget* _this - The image sequence widget to operate on

  Remarks:
    Usage - _this->fn->getImageCount(_this);

  Returns:
    uint32_t - the image count
*/
/**
 * @brief Get image count.
 * @details Gets the image count using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * uint32_t cnt = _this->fn->getImageCount(_this);
 * @endcode
 * @param _this is the widget to query
 * @return the image count.
 */
virtual uint32_t getImageCount(const leImageSequenceWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leResult setImageCount(leImageSequenceWidget* _this,
                           uint32_t cnt)

  Summary:
     Sets the image count

  Description:
     Sets the image count

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on
    uint32_t cnt - the image count

  Remarks:
    Usage - _this->fn->setImageCount(_this, cnt);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image count.
 * @details Sets the image count to <span class="param">cnt</span>
 * using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * uint32_t cnt;
 * leResult res = _this->fn->setImageCount(_this, cnt);
 * @endcode
 * @param _this is the widget to modify
 * @param cnt the image count.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImageCount(leImageSequenceWidget* _this,
                               uint32_t cnt);

// *****************************************************************************
/* Virtual Member Function:
    leImage* getImage(const leImageSequenceWidget* _this,
                      uint32_t idx)

  Summary:
     Gets the image pointer

  Description:
     Gets the image pointer

  Parameters:
    const leImageSequenceWidget* _this - The image sequence widget to operate on
    uint32_t idx - the index

  Remarks:
    Usage - _this->fn->getImage(_this, idx);

  Returns:
    leImage* - the image pointer
*/
/**
 * @brief Get image count.
 * @details Gets the image count using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * uint32_t cnt = _this->fn->getImage(_this, idx);
 * @endcode
 * @param _this is the widget to query
 * @return idx the image pointer.
 */
virtual leImage* getImage(const leImageSequenceWidget* _this,
                          uint32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setImage(leImageSequenceWidget* _this,
                      uint32_t idx,
                      const leImage* img)

  Summary:
     Sets the image pointer

  Description:
     Sets the image pointer

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on
    uint32_t idx - the index
    const leImage* img - the image pointer

  Remarks:
    Usage - _this->fn->setImage(_this, idx, img);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image count.
 * @details Sets the image count to <span class="param">cnt</span>
 * using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * uint32_t idx;
 * leImage* img;
 * leResult res = _this->fn->setImage(_this, idx, img);
 * @endcode
 * @param _this is the widget to modify
 * @param idx the index.
 * @param img the image pointer.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImage(leImageSequenceWidget* _this,
                          uint32_t idx,
                          const leImage* img);


// *****************************************************************************
/* Virtual Member Function:
    uint32_t getImageDelay(const leImageSequenceWidget* _this,
                           uint32_t idx)

  Summary:
     Gets the image cycle delay

  Description:
     Gets the image cycle delay

  Parameters:
    const leImageSequenceWidget* _this - The image sequence widget to operate on
    uint32_t idx - the index

  Remarks:
    Usage - _this->fn->getImageDelay(_this, idx);

  Returns:
    uint32_t - the image delay
*/
/**
 * @brief Get image count.
 * @details Gets the image count using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * uint32_t delay = _this->fn->getImageDelay(_this, idx);
 * @endcode
 * @param _this is the widget to query
 * @return idx the image pointer.
 */
virtual uint32_t getImageDelay(const leImageSequenceWidget* _this,
                               uint32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setImageDelay(leImageSequenceWidget* _this,
                           uint32_t idx,
                           uint32_t dly)

  Summary:
     Sets the image cycle delay

  Description:
     Sets the image cycle delay

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on
    uint32_t idx - the index
    uint32_t dly - the image delay

  Remarks:
    Usage - _this->fn->setImageDelay(_this, idx, dly);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image cycle delay.
 * @details Sets the image cycle delay to <span class="param">dly</span>
 * at <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * uint32_t idx;
 * uint32_t dly;
 * leResult res = _this->fn->setImageDelay(_this, idx, dly);
 * @endcode
 * @param _this is the widget to modify
 * @param idx the index.
 * @param dly the image delay.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImageDelay(leImageSequenceWidget* _this,
                               uint32_t idx,
                               uint32_t dly);


// *****************************************************************************
/* Virtual Member Function:
    leHAlignment getImageHAlignment(const leImageSequenceWidget* _this,
                                    uint32_t idx)

  Summary:
     Gets the image horizontal alignment

  Description:
     Gets the image horizontal alignment

  Parameters:
    const leImageSequenceWidget* _this - The image sequence widget to operate on
    uint32_t idx - the index

  Remarks:
    Usage - _this->fn->getImageHAlignment(_this, idx);

  Returns:
    leHAlignment - the alignment
*/
/**
 * @brief Get image horizontal alignment.
 * @details Gets the image horizontal alignmentt using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * uint32_t idx;
 * leHAlignment align = _this->fn->getImageHAlignment(_this, idx);
 * @endcode
 * @param _this is the widget to query.
 * @param idx the index.
 * @return align the alignment.
 */
virtual leHAlignment getImageHAlignment(const leImageSequenceWidget* _this,
                                        uint32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setImageHAlignment(leImageSequenceWidget* _this,
                                uint32_t idx,
                                leHAlignment align)

  Summary:
     Sets the image horizontal alignment

  Description:
     Sets the image horizontal alignment

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on
    uint32_t idx - the index
    leHAlignment align - the alignment

  Remarks:
    Usage - _this->fn->setImageHAlignment(_this, idx, align);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image horizontal alignment.
 * @details Sets the image horizontal alignment to <span class="param">align</span>
 * at <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * uint32_t idx;
 * leHAlignment align;
 * leResult res = _this->fn->setImageHAlignment(_this, idx, align);
 * @endcode
 * @param _this is the widget to modify
 * @param idx the index.
 * @param align the alignment.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImageHAlignment(leImageSequenceWidget* _this,
                                    uint32_t idx,
                                    leHAlignment align);

// *****************************************************************************
/* Virtual Member Function:
    leVAlignment getImageVAlignment(const leImageSequenceWidget* _this,
                                    uint32_t idx)

  Summary:
     Gets the image vertical alignment

  Description:
     Gets the image vertical alignment

  Parameters:
    const leImageSequenceWidget* _this - The image sequence widget to operate on
    uint32_t idx - the index

  Remarks:
    Usage - _this->fn->getImageVAlignment(_this, idx);

  Returns:
    leVAlignment - the alignment
*/
/**
 * @brief Get image vertical alignment.
 * @details Gets the image vertical alignment using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * uint32_t idx;
 * leHAlignment align = _this->fn->getImageHAlignment(_this, idx);
 * @endcode
 * @param _this is the widget to query.
 * @param idx the index.
 * @return align the alignment.
 */
virtual leVAlignment getImageVAlignment(const leImageSequenceWidget* _this,
                                        uint32_t idx);

// *****************************************************************************
/* Virtual Member Function:
    leResult setImageVAlignment(leImageSequenceWidget* _this,
                                uint32_t idx,
                                leVAlignment align)

  Summary:
     Sets the image vertical alignment

  Description:
     Sets the image vertical alignment

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on
    uint32_t idx - the index
    leVAlignment align - the alignment

  Remarks:
    Usage - _this->fn->setImageVAlignment(_this, idx, align);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set image vertical alignment.
 * @details Sets the image vertical alignment to <span class="param">align</span>
 * at <span class="param">idx</span>
 * using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * uint32_t idx;
 * leHAlignment align;
 * leResult res = _this->fn->setImageVAlignment(_this, idx, align);
 * @endcode
 * @param _this is the widget to modify
 * @param idx the index.
 * @param align the alignment.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImageVAlignment(leImageSequenceWidget* _this,
                                    uint32_t idx,
                                    leVAlignment align);

// *****************************************************************************
/* Virtual Member Function:
    leResult stop(leImageSequenceWidget* _this)

  Summary:
     Stops the sequence from automatically cycling

  Description:
     Stops the sequence from automatically cycling

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on

  Remarks:
    Usage - _this->fn->stop(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Stop sequence from automatically cycling.
 * @details Stops the sequence from automatically cycling using
 * <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * leResult res = _this->fn->stop(_this);
 * @endcode
 * @param _this is the widget to query.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult stop(leImageSequenceWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult play(leImageSequenceWidget* _this)

  Summary:
     Starts the sequence automatic cycle

  Description:
     Starts the sequence automatic cycle

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on

  Remarks:
    Usage - _this->fn->play(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Start sequence automatic cycle.
 * @details Starts the sequence automatic cycle using
 * <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * leResult res = _this->fn->play(_this);
 * @endcode
 * @param _this is the widget to query.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult play(leImageSequenceWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult rewind(leImageSequenceWidget* _this)

  Summary:
     Returns the sequence to the first image

  Description:
     Returns the sequence to the first image

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on

  Remarks:
    Usage - _this->fn->rewind(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Return sequence to the first image.
 * @details Returns the sequence to the first image using
 * <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * leResult res = _this->fn->play(_this);
 * @endcode
 * @param _this is the widget to query.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
leResult play(leImageSequenceWidget* _this);


// *****************************************************************************
/* Virtual Member Function:
    leBool isPlaying(const leImageSequenceWidget* _this)

  Summary:
     Indicates of the sequence is automatically cycling

  Description:
     Indicates of the sequence is automatically cycling

  Parameters:
    const leImageSequenceWidget* _this - The image sequence widget to operate on

  Remarks:
    Usage - _this->fn->isPlaying(_this);

  Returns:
    leBool - the setting value
*/
/**
 * @brief Get play state.
 * @details Gets the play state for <span class="param">_this</span>.
 * @code
 * const leImageSequenceWidget* _this;
 * leBool play = _this->fn->isPlaying(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool isPlaying(const leImageSequenceWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leBool getRepeat(const leImageSequenceWidget* _this)

  Summary:
     Indicates if the sequence will repeat the cycle

  Description:
     Indicates if the sequence will repeat the cycle

  Parameters:
    const leImageSequenceWidget* _this - The image sequence widget to operate on

  Remarks:
    Usage - _this->fn->getRepeat(_this);

  Returns:
    leBool - the setting value
*/
/**
 * @brief Get repeat state.
 * @details Gets the repeat state for <span class="param">_this</span>.
 * @code
 * const leImageSequenceWidget* _this;
 * leBool repeat = _this->fn->getRepeat(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_TRUE if set, otherwise LE_FALSE;
 */
virtual leBool getRepeat(const leImageSequenceWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult setRepeat(leImageSequenceWidget* _this,
                       leBool rpt)

  Summary:
     Sets the repeat flag

  Description:
     Sets the repeat flag

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on
    leBool rpt - the setting value

  Remarks:
    Usage - _this->fn->setRepeat(_this, rpt);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set repeat flag.
 * @details Sets the repeat flag for <span class="param">_this</span>.
 * @code
 * const leImageSequenceWidget* _this;
 * leBool rpt;
 * leResult res = _this->fn->setRepeat(_this, rpt);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param rpt is the setting.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setRepeat(leImageSequenceWidget* _this,
                           leBool rpt);

// *****************************************************************************
/* Virtual Member Function:
    leResult showImage(leImageSequenceWidget* _this,
                       uint32_t idx)

  Summary:
     Sets the current visible image index

  Description:
     Sets the current visible image index

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on
    uint32_t idx - the index

  Remarks:
    Usage - _this->fn->showImage(_this, idx);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Set repeat flag.
 * @details Sets the repeat flag for <span class="param">_this</span>.
 * @code
 * const leImageSequenceWidget* _this;
 * uint32_t idx;
 * leResult res = _this->fn->showImage(_this, idx);
 * @endcode
 * @param _this is the widget pointer to query.
 * @param idx is the setting.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult showImage(leImageSequenceWidget* _this,
                           uint32_t idx);


// *****************************************************************************
/* Virtual Member Function:
    leResult showNextImage(leImageSequenceWidget* _this)

  Summary:
     Advance to the next image

  Description:
     Advance to the next image

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on

  Remarks:
    Usage - _this->fn->showNextImage(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Show next image.
 * @details Shows the next image for <span class="param">_this</span>.
 * @code
 * const leImageSequenceWidget* _this;
 * leResult res = _this->fn->showNextImage(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult showNextImage(leImageSequenceWidget* _this);

// *****************************************************************************
/* Virtual Member Function:
    leResult showPreviousImage(leImageSequenceWidget* _this)

  Summary:
     Return to the previous image

  Description:
     Return to the previous image

  Parameters:
    leImageSequenceWidget* _this - The image sequence widget to operate on

  Remarks:
    Usage - _this->fn->showPreviousImage(_this);

  Returns:
    leResult - the result of the operation
*/
/**
 * @brief Show previous image.
 * @details Shows the previous image for <span class="param">_this</span>.
 * @code
 * const leImageSequenceWidget* _this;
 * leResult res = _this->fn->showPreviousImage(_this);
 * @endcode
 * @param _this is the widget pointer to query.
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult showPreviousImage(leImageSequenceWidget* _this);

/**
 * @brief Get image chagned event callback pointer.
 * @details Gets the image chagned event callback pointer using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * leImageSequenceImageChangedEvent_FnPtr cb = _this->fn->getImageChangedEventCallback(_this);
 * @endcode
 * @param _this is the widget to modify
 * @returns the callback pointer.
 */
virtual leImageSequenceImageChangedEvent_FnPtr getImageChangedEventCallback(const leImageSequenceWidget* _this);

/**
 * @brief Set image chagned event callback pointer.
 * @details Sets the image chagned event callback pointer to <span class="param">cb</span>
 * using <span class="param">_this</span>.
 * @code
 * leImageSequenceWidget* _this;
 * leImageSequenceImageChangedEvent_FnPtr cb;
 * leResult res = _this->fn->setImageChangedEventCallback(_this, cb);
 * @endcode
 * @param _this is the widget to modify
 * @param cb is the callback func
 * @returns LE_SUCCESS if set, otherwise LE_FAILURE.
 */
virtual leResult setImageChangedEventCallback(leImageSequenceWidget* _this,
                                              leImageSequenceImageChangedEvent_FnPtr cb);

#undef THIS_TYPE
#endif

#endif // LE_IMAGESEQUENCE_WIDGET_ENABLED
#endif /* LEGATO_IMAGESEQUENCE_H */
