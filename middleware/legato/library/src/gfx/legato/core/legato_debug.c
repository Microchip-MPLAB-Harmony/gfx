#include "gfx/legato/core/legato_debug.h"

#include "gfx/legato/widget/legato_widget.h"

#if LE_DEBUG == 1

static leDebugEventListener* _listener = NULL;
static leBool _eventsEnabled = LE_TRUE;

void _leSetDebugListener(leDebugEventListener* listener)
{
    _listener = listener;
}

void _leDebugNotify_WidgetPropertyChanged(const leWidget* wgt)
{
    if(_eventsEnabled == LE_FALSE ||
       _listener == NULL ||
       _listener->widgetPropertyChanged == NULL)
        return;

    _listener->widgetPropertyChanged(wgt);
}

void _leDebugNotify_WidgetAboutToAddChild(const leWidget* wgt,
                                          const leWidget* child,
                                          uint32_t idx)
{
    if(_eventsEnabled == LE_FALSE ||
       _listener == NULL ||
       _listener->widgetAboutToAddChild == NULL)
        return;

    _listener->widgetAboutToAddChild(wgt, child, idx);
}

void _leDebugNotify_WidgetChildAdded(const leWidget* wgt,
                                     const leWidget* child,
                                     uint32_t idx)
{
    if(_eventsEnabled == LE_FALSE ||
       _listener == NULL ||
       _listener->widgetAboutToAddChild == NULL)
        return;

    _listener->widgetChildAdded(wgt, child, idx);
}

void _leDebugNotify_WidgetAboutToRemoveChild(const leWidget* wgt,
                                             const leWidget* child,
                                             uint32_t idx)
{
    if(_eventsEnabled == LE_FALSE ||
       _listener == NULL ||
       _listener->widgetAboutToRemoveChild == NULL)
        return;

    _listener->widgetAboutToRemoveChild(wgt, child, idx);
}

void _leDebugNotify_WidgetChildRemoved(const leWidget* wgt,
                                       const leWidget* child,
                                       uint32_t idx)
{
    if(_eventsEnabled == LE_FALSE ||
       _listener == NULL ||
       _listener->widgetChildRemoved == NULL)
        return;

    _listener->widgetChildRemoved(wgt, child, idx);
}

void _leDebugNotify_WidgetAboutToReparent(const leWidget* wgt,
                                          const leWidget* oldParent,
                                          uint32_t oldIdx,
                                          const leWidget* newParent,
                                          uint32_t newIdx)
{
    if(_eventsEnabled == LE_FALSE ||
       _listener == NULL ||
       _listener->widgetAboutToReparent == NULL)
        return;

    _listener->widgetAboutToReparent(wgt, oldParent, oldIdx, newParent, newIdx);
}

void _leDebugNotify_WidgetReparented(const leWidget* wgt,
                                     const leWidget* oldParent,
                                     uint32_t oldIdx,
                                     const leWidget* newParent,
                                     uint32_t newIdx)
{
    if(_eventsEnabled == LE_FALSE ||
       _listener == NULL ||
       _listener->widgetReparented == NULL)
        return;

    _listener->widgetReparented(wgt, oldParent, oldIdx, newParent, newIdx);
}

void _leDebug_SetEventsEnabled(leBool b)
{
    _eventsEnabled = b;
}

#endif // LE_DEBUG == 1