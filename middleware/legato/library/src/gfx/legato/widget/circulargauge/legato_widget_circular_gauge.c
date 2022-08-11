// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


#include "gfx/legato/widget/arc/legato_widget_arc.h"
#include "gfx/legato/widget/circulargauge/legato_widget_circular_gauge.h"

#if LE_CIRCULARGAUGE_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/widget/legato_widget.h"

#include "gfx/legato/common/legato_math.h"

#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif

#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          100

#define DEFAULT_RADIUS          40
#define DEFAULT_START_ANGLE     0
#define DEFAULT_CENTER_ANGLE    180
#define DEFAULT_VALUE           50
#define DEFAULT_HAND_LENGTH     100

#define DEFAULT_CENTER_CIRCLE_RADIUS 5
#define DEFAULT_CENTER_CIRCLE_THICKNESS 2

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leCircularGaugeWidgetVTable circularGaugeWidgetVTable;

void leCircularGaugeWidget_Constructor(leCircularGaugeWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&circularGaugeWidgetVTable;
    _this->fn = &circularGaugeWidgetVTable;

    _this->widget.type = LE_WIDGET_CIRCULAR_GAUGE;
    
    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_NONE;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_NONE;

    // default gauge values
    _this->value = DEFAULT_VALUE;
    _this->radius = DEFAULT_RADIUS;
    _this->startAngle = DEFAULT_START_ANGLE;
    _this->centerAngle = DEFAULT_CENTER_ANGLE;

    // default hand properties
    _this->handVisible = LE_TRUE;
    _this->handRadius = DEFAULT_HAND_LENGTH;
    
    _this->centerCircleVisible = LE_TRUE;
    _this->centerCircleRadius = DEFAULT_CENTER_CIRCLE_RADIUS;
    _this->centerCircleThickness = DEFAULT_CENTER_CIRCLE_THICKNESS;
    
    // advanced configuration
    leArray_Create(&_this->arcsArray);
    leArray_Create(&_this->ticksArray);
    leArray_Create(&_this->labelsArray);

    _this->cb = NULL;
}

void _leWidget_Destructor(leWidget* wgt);

void _leCircularGaugeWidget_Destructor(leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
    
    _this->fn->deleteAllArcs(_this);
    _this->fn->deleteAllTickRanges(_this);
    _this->fn->deleteAllLabelRanges(_this);
    
    _leWidget_Destructor((leWidget*)_this);
}

leCircularGaugeWidget* leCircularGaugeWidget_New()
{
    leCircularGaugeWidget* gauge = NULL;

    gauge = LE_MALLOC(sizeof(leCircularGaugeWidget));

    if(gauge == NULL)
        return NULL;
    
    leCircularGaugeWidget_Constructor(gauge);

    return gauge;
}

static uint32_t getRadius(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->radius;
}

static leResult setRadius(leCircularGaugeWidget* _this,
                          uint32_t rad)
{
    LE_ASSERT_THIS();
        
    if(_this->radius == rad)
        return LE_SUCCESS;
       
    _this->radius = rad;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getStartAngle(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->startAngle;
    
    return LE_SUCCESS;
}

static leResult setStartAngle(leCircularGaugeWidget* _this,
                              int32_t angle)
{
    LE_ASSERT_THIS();
        
    if(_this->startAngle == angle)
        return LE_SUCCESS;
        
    _this->startAngle = angle;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getCenterAngle(const leCircularGaugeWidget* _this)
{
    if(_this == NULL)
        return 0;
        
    return _this->centerAngle;
}

static leResult setCenterAngle(leCircularGaugeWidget* _this,
                               int32_t angle)
{
    LE_ASSERT_THIS();

    if(angle < -360)
    {
        angle = -360;
    }

    if(angle > 360)
    {
        angle = 360;
    }

    if(_this->centerAngle == angle)
        return LE_SUCCESS;

    _this->centerAngle = angle;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getValue(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->value;
}

static leResult setValue(leCircularGaugeWidget* _this,
                         uint32_t val)
{
    LE_ASSERT_THIS();

    if(val > 100)
    {
        val = 100;
    }

    if(_this->value == val)
        return LE_SUCCESS;

    _this->value = val;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t addArc(leCircularGaugeWidget* _this,
                      uint32_t startAngle,
                      uint32_t endAngle,
                      int32_t offset,
                      const leScheme* scheme,
                      uint32_t thickness)
{
    leCircularGaugeArc * arc;
    int32_t idx = -1;
    
    LE_ASSERT_THIS();

    if (startAngle > endAngle)
        return -1;

    arc = LE_MALLOC(sizeof(leCircularGaugeArc));
    
    if (arc == NULL)
        return -1;

    if(startAngle > endAngle)
    {
        startAngle = endAngle;
    }

    if(endAngle < startAngle)
    {
        endAngle = startAngle;
    }

    arc->enabled = LE_TRUE;
    arc->startAngle = startAngle;
    arc->endAngle = endAngle;
    arc->offset = offset;
    arc->scheme = scheme;
    arc->thickness = thickness;

    idx = (int32_t)_this->arcsArray.size;

    leArray_PushBack(&_this->arcsArray, arc);

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return idx;
}

static leResult deleteArc(leCircularGaugeWidget* _this,
                          uint32_t idx)
{
    leResult res;
    struct leCircularGaugeArc* arc;

    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FAILURE;

    arc = leArray_Get(&_this->arcsArray, idx);

    res = leArray_RemoveAt(&_this->arcsArray, idx);

    LE_FREE(arc);

    if(res == LE_SUCCESS)
    {
        _this->fn->invalidate(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return res;
}

static leResult deleteAllArcs(leCircularGaugeWidget* _this)
{
    leCircularGaugeArc* arc;
    size_t idx;
    
    LE_ASSERT_THIS();
    
    for(idx = 0; idx < _this->arcsArray.size; idx++)
    {
        arc = (leCircularGaugeArc*)_this->arcsArray.values[idx];
        
        LE_FREE(arc);
    }
    
    leArray_Clear(&_this->arcsArray);

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t arcCount(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->arcsArray.size;
}

static leBool getArcEnabled(const leCircularGaugeWidget* _this,
                            uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    return arc->enabled;
}

static leResult setArcEnabled(leCircularGaugeWidget* _this,
                              uint32_t idx,
                              leBool en)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    if(arc->enabled == en)
        return LE_SUCCESS;

    arc->enabled = en;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getArcStartAngle(const leCircularGaugeWidget* _this,
                                 uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    return arc->startAngle;
}

static leResult setArcStartAngle(leCircularGaugeWidget* _this,
                                 uint32_t idx,
                                 uint32_t ang)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    if(arc->startAngle > arc->endAngle)
        return LE_FAILURE;

    if(arc->startAngle == ang)
        return LE_SUCCESS;

    arc->startAngle = ang;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getArcEndAngle(const leCircularGaugeWidget* _this,
                               uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    return arc->endAngle;
}

static leResult setArcEndAngle(leCircularGaugeWidget* _this,
                               uint32_t idx,
                               uint32_t ang)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    if(arc->endAngle > arc->startAngle)
        return LE_FAILURE;

    if(arc->endAngle == ang)
        return LE_SUCCESS;

    arc->endAngle = ang;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getArcOffset(const leCircularGaugeWidget* _this,
                            uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    return arc->offset;
}

static leResult setArcOffset(leCircularGaugeWidget* _this,
                             uint32_t idx,
                             int32_t offs)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    if(arc->offset == offs)
        return LE_SUCCESS;

    arc->offset = offs;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static const leScheme* getArcScheme(const leCircularGaugeWidget* _this,
                                    uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    return arc->scheme;
}

static leResult setArcScheme(leCircularGaugeWidget* _this,
                             uint32_t idx,
                             const leScheme* schm)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    if(arc->scheme == schm)
        return LE_SUCCESS;

    arc->scheme = schm;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getArcThickness(const leCircularGaugeWidget* _this,
                                uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    return arc->thickness;
}

static leResult setArcThickness(leCircularGaugeWidget* _this,
                                uint32_t idx,
                                uint32_t thk)
{
    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FALSE;

    leCircularGaugeArc* arc = (leCircularGaugeArc*)leArray_Get(&_this->arcsArray, idx);

    if(arc->thickness == thk)
        return LE_SUCCESS;

    arc->thickness = thk;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t addTickRange(leCircularGaugeWidget* _this,
                            uint32_t startAngle,
                            uint32_t endAngle,
                            int32_t offset,
                            const leScheme* schm,
                            uint32_t thickness,
                            uint32_t div)
{
    leCircularGaugeTickRange* tick;
    int32_t idx = -1;
    
    LE_ASSERT_THIS();

    if(startAngle > endAngle)
    {
        startAngle = endAngle;
    }

    if(endAngle < startAngle)
    {
        endAngle = startAngle;
    }

    if(startAngle > 100)
    {
        startAngle = 100;
    }

    if(endAngle > 100)
    {
        endAngle = 100;
    }

    if(startAngle > endAngle)
        return -1;

    tick = LE_MALLOC(sizeof(leCircularGaugeTickRange));
    
    if (tick == NULL)
        return -1;

    tick->enabled = LE_TRUE;
    tick->startAngle = startAngle;
    tick->endAngle = endAngle;
    tick->offset = offset;
    tick->scheme = schm;
    tick->thickness = thickness;
    tick->divisions = div;

    idx = (int32_t)_this->ticksArray.size;

    leArray_PushBack(&_this->ticksArray, tick);

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return idx;
}

static leResult deleteTickRange(leCircularGaugeWidget* _this,
                                uint32_t idx)
{
    leResult res;
    struct leCircularGaugeArc* arc;

    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FAILURE;

    arc = leArray_Get(&_this->arcsArray, idx);

    res = leArray_RemoveAt(&_this->arcsArray, idx);

    LE_FREE(arc);

    if(res == LE_SUCCESS)
    {
        _this->fn->invalidate(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return res;
}

static leResult deleteAllTickRanges(leCircularGaugeWidget* _this)
{
    leCircularGaugeTickRange* tick;
    size_t idx;

    LE_ASSERT_THIS();

    for(idx = 0; idx < _this->ticksArray.size; idx++)
    {
        tick = (leCircularGaugeTickRange*)_this->ticksArray.values[idx];

        LE_FREE(tick);
    }

    leArray_Clear(&_this->ticksArray);

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t tickRangeCount(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->ticksArray.size;
}

static leBool getTickRangeEnabled(const leCircularGaugeWidget* _this,
                            uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    return tick->enabled;
}

static leResult setTickRangeEnabled(leCircularGaugeWidget* _this,
                              uint32_t idx,
                              leBool en)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    if(tick->enabled == en)
        return LE_SUCCESS;

    tick->enabled = en;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getTickRangeStartAngle(const leCircularGaugeWidget* _this,
                                 uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    return tick->startAngle;
}

static leResult setTickRangeStartAngle(leCircularGaugeWidget* _this,
                                 uint32_t idx,
                                 uint32_t ang)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    if(tick->startAngle > tick->endAngle)
        return LE_FAILURE;

    if(tick->startAngle == ang)
        return LE_SUCCESS;

    tick->startAngle = ang;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getTickRangeEndAngle(const leCircularGaugeWidget* _this,
                               uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    return tick->endAngle;
}

static leResult setTickRangeEndAngle(leCircularGaugeWidget* _this,
                               uint32_t idx,
                               uint32_t ang)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    if(tick->endAngle < tick->startAngle)
        return LE_FAILURE;

    if(tick->endAngle == ang)
        return LE_SUCCESS;

    tick->endAngle = ang;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getTickRangeOffset(const leCircularGaugeWidget* _this,
                            uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    return tick->offset;
}

static leResult setTickRangeOffset(leCircularGaugeWidget* _this,
                             uint32_t idx,
                             int32_t offs)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    if(tick->offset == offs)
        return LE_SUCCESS;

    tick->offset = offs;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static const leScheme* getTickRangeScheme(const leCircularGaugeWidget* _this,
                                    uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    return tick->scheme;
}

static leResult setTickRangeScheme(leCircularGaugeWidget* _this,
                             uint32_t idx,
                             const leScheme* schm)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    if(tick->scheme == schm)
        return LE_SUCCESS;

    tick->scheme = schm;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getTickRangeThickness(const leCircularGaugeWidget* _this,
                                uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    return tick->thickness;
}

static leResult setTickRangeThickness(leCircularGaugeWidget* _this,
                                uint32_t idx,
                                uint32_t thk)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    if(tick->thickness == thk)
        return LE_SUCCESS;

    tick->thickness = thk;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getTickRangeDivisions(const leCircularGaugeWidget* _this,
                                      uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    return tick->divisions;
}

static leResult setTickRangeDivisions(leCircularGaugeWidget* _this,
                                      uint32_t idx,
                                      uint32_t div)
{
    LE_ASSERT_THIS();

    if(idx >= _this->ticksArray.size)
        return LE_FALSE;

    leCircularGaugeTickRange* tick = (leCircularGaugeTickRange*)leArray_Get(&_this->ticksArray, idx);

    if(tick->divisions == div)
        return LE_SUCCESS;

    tick->divisions = div;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t addLabelRange(leCircularGaugeWidget* _this,
                              uint32_t startAngle,
                              uint32_t endAngle,
                              int32_t offset,
                              const leScheme* schm,
                              const leFont* fnt,
                              uint32_t div)
{
    leCircularGaugeLabelRange* label;
    int32_t idx = -1;

    LE_ASSERT_THIS();

    if(startAngle > 100)
    {
        startAngle = 100;
    }

    if(endAngle > 100)
    {
        endAngle = 100;
    }

    if(startAngle > endAngle)
    {
        startAngle = endAngle;
    }

    if(endAngle < startAngle)
    {
        endAngle = startAngle;
    }

    label = LE_MALLOC(sizeof(leCircularGaugeLabelRange));

    if (label == NULL)
        return -1;

    label->enabled = LE_TRUE;
    label->startAngle = startAngle;
    label->endAngle = endAngle;
    label->offset = offset;
    label->scheme = schm;
    label->font = fnt;
    label->divisions = div;

    idx = (int32_t)_this->labelsArray.size;

    leArray_PushBack(&_this->labelsArray, label);

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return idx;
}

static leResult deleteLabelRange(leCircularGaugeWidget* _this,
                                uint32_t idx)
{
    leResult res;
    struct leCircularGaugeArc* arc;

    LE_ASSERT_THIS();

    if(idx >= _this->arcsArray.size)
        return LE_FAILURE;

    arc = leArray_Get(&_this->arcsArray, idx);

    res = leArray_RemoveAt(&_this->arcsArray, idx);

    LE_FREE(arc);

    if(res == LE_SUCCESS)
    {
        _this->fn->invalidate(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return res;
}

static leResult deleteAllLabelRanges(leCircularGaugeWidget* _this)
{
    leCircularGaugeLabelRange* label;
    size_t idx;

    LE_ASSERT_THIS();

    for(idx = 0; idx < _this->labelsArray.size; idx++)
    {
        label = (leCircularGaugeLabelRange*)_this->labelsArray.values[idx];

        LE_FREE(label);
    }

    leArray_Clear(&_this->labelsArray);

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t labelRangeCount(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->labelsArray.size;
}

static leBool getLabelRangeEnabled(const leCircularGaugeWidget* _this,
                                   uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    return label->enabled;
}

static leResult setLabelRangeEnabled(leCircularGaugeWidget* _this,
                                     uint32_t idx,
                                     leBool en)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    if(label->enabled == en)
        return LE_SUCCESS;

    label->enabled = en;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getLabelRangeStartAngle(const leCircularGaugeWidget* _this,
                                        uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    return label->startAngle;
}

static leResult setLabelRangeStartAngle(leCircularGaugeWidget* _this,
                                        uint32_t idx,
                                        uint32_t ang)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    if(label->startAngle > label->endAngle)
        return LE_FAILURE;

    if(label->startAngle == ang)
        return LE_SUCCESS;

    label->startAngle = ang;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getLabelRangeEndAngle(const leCircularGaugeWidget* _this,
                                      uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    return label->endAngle;
}

static leResult setLabelRangeEndAngle(leCircularGaugeWidget* _this,
                                      uint32_t idx,
                                      uint32_t ang)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    if(label->endAngle == ang)
        return LE_SUCCESS;

    label->endAngle = ang;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getLabelRangeOffset(const leCircularGaugeWidget* _this,
                                   uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    return label->offset;
}

static leResult setLabelRangeOffset(leCircularGaugeWidget* _this,
                                    uint32_t idx,
                                    int32_t offs)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    if(label->endAngle < label->startAngle)
        return LE_FAILURE;

    if(label->offset == offs)
        return LE_SUCCESS;

    label->offset = offs;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static const leScheme* getLabelRangeScheme(const leCircularGaugeWidget* _this,
                                           uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    return label->scheme;
}

static leResult setLabelRangeScheme(leCircularGaugeWidget* _this,
                                    uint32_t idx,
                                    const leScheme* schm)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    if(label->scheme == schm)
        return LE_SUCCESS;

    label->scheme = schm;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static const leFont* getLabelRangeFont(const leCircularGaugeWidget* _this,
                                       uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    return label->font;
}

static leResult setLabelRangeFont(leCircularGaugeWidget* _this,
                                  uint32_t idx,
                                  const leFont* fnt)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    if(label->font == fnt)
        return LE_SUCCESS;

    label->font = fnt;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getLabelRangeDivisions(const leCircularGaugeWidget* _this,
                                       uint32_t idx)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    return label->divisions;
}

static leResult setLabelRangeDivisions(leCircularGaugeWidget* _this,
                                       uint32_t idx,
                                       uint32_t div)
{
    LE_ASSERT_THIS();

    if(idx >= _this->labelsArray.size)
        return LE_FALSE;

    leCircularGaugeLabelRange* label = (leCircularGaugeLabelRange*)leArray_Get(&_this->labelsArray, idx);

    if(label->divisions == div)
        return LE_SUCCESS;

    label->divisions = div;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getHandVisible(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->handVisible;
}

static leResult setHandVisible(leCircularGaugeWidget* _this, 
                               leBool visible)
{
    LE_ASSERT_THIS();

    if (_this->handVisible == visible)
        return LE_SUCCESS;

    _this->handVisible = visible;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getHandRadius(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->handRadius;
}

static leResult setHandRadius(leCircularGaugeWidget* _this, 
                              uint32_t length)
{
    LE_ASSERT_THIS();
        
    if(_this->handRadius == length)
        return LE_SUCCESS;
    
    if (length > _this->radius)
        return LE_FAILURE;
        
    _this->handRadius = length;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getCenterCircleVisible(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->centerCircleVisible;
}

static leResult setCenterCircleVisible(leCircularGaugeWidget* _this, 
                                       leBool visible)
{
    LE_ASSERT_THIS();

    if (_this->centerCircleVisible == visible)
        return LE_SUCCESS;

    _this->centerCircleVisible = visible;

    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getCenterCircleRadius(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->centerCircleRadius;
}

static leResult setCenterCircleRadius(leCircularGaugeWidget* _this, 
                                      uint32_t rad)
{
    LE_ASSERT_THIS();
        
    if(_this->centerCircleRadius == rad)
        return LE_SUCCESS;
    
    if (rad > _this->radius)
        return LE_FAILURE;
    
    _this->centerCircleRadius = rad;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getCenterCircleThickness(const leCircularGaugeWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->centerCircleThickness;
}

static leResult setCenterCircleThickness(leCircularGaugeWidget* _this,
                                         uint32_t thickness)
{
    LE_ASSERT_THIS();

    if (thickness > _this->centerCircleRadius)
        return LE_FAILURE;

    if(_this->centerCircleThickness == thickness)
        return LE_SUCCESS;
        
    _this->centerCircleThickness = thickness;
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult setValueChangedEventCallback(leCircularGaugeWidget* _this,
                                             leCircularGaugeWidget_ValueChangedEvent cb)
{
    LE_ASSERT_THIS();
    
    _this->cb = cb;
    
    return LE_SUCCESS;
}

void _leCircularGaugeWidget_Paint(leCircularGaugeWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leCircularGaugeWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&circularGaugeWidgetVTable);
    
    /* overrides from base class */
    circularGaugeWidgetVTable._destructor = _leCircularGaugeWidget_Destructor;
    circularGaugeWidgetVTable._paint = _leCircularGaugeWidget_Paint;

    
    /* member functions */
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getRadius);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setRadius);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getStartAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setStartAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getCenterAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setCenterAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getValue);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setValue);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,addArc);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,deleteArc);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,deleteAllArcs);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,arcCount);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getArcEnabled);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setArcEnabled);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getArcStartAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setArcStartAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getArcEndAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setArcEndAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getArcOffset);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setArcOffset);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getArcScheme);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setArcScheme);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getArcThickness);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setArcThickness);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,addTickRange);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,deleteTickRange);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,deleteAllTickRanges);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,tickRangeCount);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getTickRangeEnabled);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setTickRangeEnabled);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getTickRangeStartAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setTickRangeStartAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getTickRangeEndAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setTickRangeEndAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getTickRangeOffset);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setTickRangeOffset);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getTickRangeScheme);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setTickRangeScheme);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getTickRangeThickness);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setTickRangeThickness);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getTickRangeDivisions);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setTickRangeDivisions);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,addLabelRange);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,deleteLabelRange);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,deleteAllLabelRanges);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,labelRangeCount);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getLabelRangeEnabled);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setLabelRangeEnabled);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getLabelRangeStartAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setLabelRangeStartAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getLabelRangeEndAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setLabelRangeEndAngle);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getLabelRangeOffset);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setLabelRangeOffset);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setArcThickness);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getLabelRangeScheme);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setLabelRangeScheme);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getLabelRangeFont);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setLabelRangeFont);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getLabelRangeDivisions);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setLabelRangeDivisions);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getHandVisible);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setHandVisible);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getHandRadius);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setHandRadius);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getCenterCircleVisible);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setCenterCircleVisible);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getCenterCircleRadius);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setCenterCircleRadius);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,getCenterCircleThickness);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setCenterCircleThickness);
    ASSIGN_VTABLE_FUNCTION(circularGaugeWidgetVTable,setValueChangedEventCallback);
}

void _leCircularGaugeWidget_FillVTable(leCircularGaugeWidgetVTable* tbl)
{
    *tbl = circularGaugeWidgetVTable;
}
#else
static const leCircularGaugeWidgetVTable circularGaugeWidgetVTable =
{
    // base class
    .getType = (void*)_leWidget_GetType,
    .getX = (void*)_leWidget_GetX,
    .setX = (void*)_leWidget_SetX,
    .getY = (void*)_leWidget_GetY,
    .setY = (void*)_leWidget_SetY,
    .setPosition = (void*)_leWidget_SetPosition,
    .translate = (void*)_leWidget_Translate,
    .getWidth = (void*)_leWidget_GetWidth,
    .setWidth = (void*)_leWidget_SetWidth,
    .getHeight = (void*)_leWidget_GetHeight,
    .setHeight = (void*)_leWidget_SetHeight,
    .setSize = (void*)_leWidget_SetSize,
    .resize = (void*)_leWidget_Resize,
    .getAlphaEnabled = (void*)_leWidget_GetAlphaEnabled,
    .getCumulativeAlphaEnabled = (void*)_leWidget_GetCumulativeAlphaEnabled,
    .setAlphaEnabled = (void*)_leWidget_SetAlphaEnabled,
    .getAlphaAmount = (void*)_leWidget_GetAlphaAmount,
    .getCumulativeAlphaAmount = (void*)_leWidget_GetCumulativeAlphaAmount,
    .setAlphaAmount = (void*)_leWidget_SetAlphaAmount,
    .isOpaque = (void*)_leWidget_IsOpaque,
    .getEnabled = (void*)_leWidget_GetEnabled,
    .setEnabled = (void*)_leWidget_SetEnabled,
    .getVisible = (void*)_leWidget_GetVisible,
    .setVisible = (void*)_leWidget_SetVisible,
    .localRect = (void*)_leWidget_LocalRect,
    .rectToParent = (void*)_leWidget_RectToParentSpace,
    .rectToScreen = (void*)_leWidget_RectToScreenSpace,
    .addChild = (void*)_leWidget_AddChild,
    .removeChild = (void*)_leWidget_RemoveChild,
    .removeAllChildren = (void*)_leWidget_RemoveAllChildren,
    .getRootWidget = (void*)_leWidget_GetRootWidget,
    .setParent = (void*)_leWidget_SetParent,
    .getChildCount = (void*)_leWidget_GetChildCount,
    .getChildAtIndex = (void*)_leWidget_GetChildAtIndex,
    .getIndexOfChild = (void*)_leWidget_GetIndexOfChild,
    .containsDescendant = (void*)_leWidget_ContainsDescendant,
    .getScheme = (void*)_leWidget_GetScheme,
    .setScheme = (void*)_leWidget_SetScheme,
    .getBorderType = (void*)_leWidget_GetBorderType,
    .setBorderType = (void*)_leWidget_SetBorderType,
    .getBackgroundType = (void*)_leWidget_GetBackgroundType,
    .setBackgroundType = (void*)_leWidget_SetBackgroundType,
    .getHAlignment = (void*)_leWidget_GetHAlignment,
    .setHAlignment = (void*)_leWidget_SetHAlignment,
    .getVAlignment = (void*)_leWidget_GetVAlignment,
    .setVAlignment = (void*)_leWidget_SetVAlignment,
    .getMargins = (void*)_leWidget_GetMargins,
    .setMargins = (void*)_leWidget_SetMargins,
    .getCornerRadius = (void*)_leWidget_GetCornerRadius,
    .setCornerRadius = (void*)_leWidget_SetCornerRadius,
    .hasFocus = (void*)_leWidget_HasFocus,
    .setFocus = (void*)_leWidget_SetFocus,
    .invalidate = (void*)_leWidget_Invalidate,
    .invalidateContents = (void*)_leWidget_InvalidateContents,
    .installEventFilter = (void*)_leWidget_InstallEventFilter,
    .removeEventFilter = (void*)_leWidget_RemoveEventFilter,

    .update = (void*)_leWidget_Update,

    .touchDownEvent = (void*)_leWidget_TouchDownEvent,
    .touchUpEvent = (void*)_leWidget_TouchUpEvent,
    .touchMoveEvent = (void*)_leWidget_TouchMoveEvent,
    .moveEvent = (void*)_leWidget_MoveEvent,
    .resizeEvent = (void*)_leWidget_ResizeEvent,
    .focusLostEvent = (void*)_leWidget_FocusLostEvent,
    .focusGainedEvent = (void*)_leWidget_FocusGainedEvent,
    .languageChangeEvent = (void*)_leWidget_LanguageChangeEvent,

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = _leCircularGaugeWidget_Destructor,
    ._paint = _leCircularGaugeWidget_Paint,

    /* member functions */
    ASSIGN_VIRTUAL_FUNCTION(getRadius),
    ASSIGN_VIRTUAL_FUNCTION(setRadius),
    ASSIGN_VIRTUAL_FUNCTION(getStartAngle),
    ASSIGN_VIRTUAL_FUNCTION(setStartAngle),
    ASSIGN_VIRTUAL_FUNCTION(getCenterAngle),
    ASSIGN_VIRTUAL_FUNCTION(setCenterAngle),
    ASSIGN_VIRTUAL_FUNCTION(getValue),
    ASSIGN_VIRTUAL_FUNCTION(setValue),
    ASSIGN_VIRTUAL_FUNCTION(addArc),
    ASSIGN_VIRTUAL_FUNCTION(deleteArc),
    ASSIGN_VIRTUAL_FUNCTION(deleteAllArcs),
    ASSIGN_VIRTUAL_FUNCTION(arcCount),
    ASSIGN_VIRTUAL_FUNCTION(getArcEnabled),
    ASSIGN_VIRTUAL_FUNCTION(setArcEnabled),
    ASSIGN_VIRTUAL_FUNCTION(getArcStartAngle),
    ASSIGN_VIRTUAL_FUNCTION(setArcStartAngle),
    ASSIGN_VIRTUAL_FUNCTION(getArcEndAngle),
    ASSIGN_VIRTUAL_FUNCTION(setArcEndAngle),
    ASSIGN_VIRTUAL_FUNCTION(getArcOffset),
    ASSIGN_VIRTUAL_FUNCTION(setArcOffset),
    ASSIGN_VIRTUAL_FUNCTION(getArcScheme),
    ASSIGN_VIRTUAL_FUNCTION(setArcScheme),
    ASSIGN_VIRTUAL_FUNCTION(getArcThickness),
    ASSIGN_VIRTUAL_FUNCTION(setArcThickness),
    ASSIGN_VIRTUAL_FUNCTION(addTickRange),
    ASSIGN_VIRTUAL_FUNCTION(deleteTickRange),
    ASSIGN_VIRTUAL_FUNCTION(deleteAllTickRanges),
    ASSIGN_VIRTUAL_FUNCTION(tickRangeCount),
    ASSIGN_VIRTUAL_FUNCTION(getTickRangeEnabled),
    ASSIGN_VIRTUAL_FUNCTION(setTickRangeEnabled),
    ASSIGN_VIRTUAL_FUNCTION(getTickRangeStartAngle),
    ASSIGN_VIRTUAL_FUNCTION(setTickRangeStartAngle),
    ASSIGN_VIRTUAL_FUNCTION(getTickRangeEndAngle),
    ASSIGN_VIRTUAL_FUNCTION(setTickRangeEndAngle),
    ASSIGN_VIRTUAL_FUNCTION(getTickRangeOffset),
    ASSIGN_VIRTUAL_FUNCTION(setTickRangeOffset),
    ASSIGN_VIRTUAL_FUNCTION(getTickRangeScheme),
    ASSIGN_VIRTUAL_FUNCTION(setTickRangeScheme),
    ASSIGN_VIRTUAL_FUNCTION(getTickRangeThickness),
    ASSIGN_VIRTUAL_FUNCTION(setTickRangeThickness),
    ASSIGN_VIRTUAL_FUNCTION(getTickRangeDivisions),
    ASSIGN_VIRTUAL_FUNCTION(setTickRangeDivisions),
    ASSIGN_VIRTUAL_FUNCTION(addLabelRange),
    ASSIGN_VIRTUAL_FUNCTION(deleteLabelRange),
    ASSIGN_VIRTUAL_FUNCTION(deleteAllLabelRanges),
    ASSIGN_VIRTUAL_FUNCTION(labelRangeCount),
    ASSIGN_VIRTUAL_FUNCTION(getLabelRangeEnabled),
    ASSIGN_VIRTUAL_FUNCTION(setLabelRangeEnabled),
    ASSIGN_VIRTUAL_FUNCTION(getLabelRangeStartAngle),
    ASSIGN_VIRTUAL_FUNCTION(setLabelRangeStartAngle),
    ASSIGN_VIRTUAL_FUNCTION(getLabelRangeEndAngle),
    ASSIGN_VIRTUAL_FUNCTION(setLabelRangeEndAngle),
    ASSIGN_VIRTUAL_FUNCTION(getLabelRangeOffset),
    ASSIGN_VIRTUAL_FUNCTION(setLabelRangeOffset),
    ASSIGN_VIRTUAL_FUNCTION(getLabelRangeScheme),
    ASSIGN_VIRTUAL_FUNCTION(setLabelRangeScheme),
    ASSIGN_VIRTUAL_FUNCTION(getLabelRangeFont),
    ASSIGN_VIRTUAL_FUNCTION(setLabelRangeFont),
    ASSIGN_VIRTUAL_FUNCTION(getLabelRangeDivisions),
    ASSIGN_VIRTUAL_FUNCTION(setLabelRangeDivisions),
    ASSIGN_VIRTUAL_FUNCTION(getHandVisible),
    ASSIGN_VIRTUAL_FUNCTION(setHandVisible),
    ASSIGN_VIRTUAL_FUNCTION(getHandRadius),
    ASSIGN_VIRTUAL_FUNCTION(setHandRadius),
    ASSIGN_VIRTUAL_FUNCTION(getCenterCircleVisible),
    ASSIGN_VIRTUAL_FUNCTION(setCenterCircleVisible),
    ASSIGN_VIRTUAL_FUNCTION(getCenterCircleRadius),
    ASSIGN_VIRTUAL_FUNCTION(setCenterCircleRadius),
    ASSIGN_VIRTUAL_FUNCTION(getCenterCircleThickness),
    ASSIGN_VIRTUAL_FUNCTION(setCenterCircleThickness),
    ASSIGN_VIRTUAL_FUNCTION(setValueChangedEventCallback),
};
#endif

#endif // LE_CIRCULARGAUGE_WIDGET_ENABLED
