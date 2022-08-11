#ifndef LEGATO_DEBUG_H
#define LEGATO_DEBUG_H

#include "gfx/legato/common/legato_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#if LE_DEBUG == 1

#include "gfx/legato/widget/legato_widget.h"

typedef struct leDebugEventListener
{
    void (*widgetPropertyChanged)(const leWidget* wgt);
    void (*widgetAboutToAddChild)(const leWidget* wgt, const leWidget* child, uint32_t idx);
    void (*widgetChildAdded)(const leWidget* wgt, const leWidget* child, uint32_t idx);
    void (*widgetAboutToRemoveChild)(const leWidget* wgt, const leWidget* child, uint32_t idx);
    void (*widgetChildRemoved)(const leWidget* wgt, const leWidget* child, uint32_t idx);
    void (*widgetAboutToReparent)(const leWidget* wgt, const leWidget* oldParent, uint32_t oldIdx, const leWidget* newParent, uint32_t newIdx);
    void (*widgetReparented)(const leWidget* wgt, const leWidget* oldParent, uint32_t oldIdx, const leWidget* newParent, uint32_t newIdx);
} leDebugEventListener;

void _leSetDebugListener(leDebugEventListener* listener);

void _leDebugNotify_WidgetPropertyChanged(const leWidget* wgt);

void _leDebugNotify_WidgetAboutToAddChild(const leWidget* wgt,
                                          const leWidget* child,
                                          uint32_t idx);

void _leDebugNotify_WidgetChildAdded(const leWidget* wgt,
                                     const leWidget* child,
                                     uint32_t idx);

void _leDebugNotify_WidgetAboutToRemoveChild(const leWidget* wgt,
                                             const leWidget* child,
                                             uint32_t idx);

void _leDebugNotify_WidgetChildRemoved(const leWidget* wgt,
                                       const leWidget* child,
                                       uint32_t idx);

void _leDebugNotify_WidgetAboutToReparent(const leWidget* wgt,
                                          const leWidget* oldParent,
                                          uint32_t oldIdx,
                                          const leWidget* newParent,
                                          uint32_t newIdx);

void _leDebugNotify_WidgetReparented(const leWidget* wgt,
                                     const leWidget* oldParent,
                                     uint32_t oldIdx,
                                     const leWidget* newParent,
                                     uint32_t newIdx);

void _leDebug_SetEventsEnabled(leBool b);

#endif // LE_DEBUG

#ifdef __cplusplus
}
#endif

#endif // LEGATO_DEBUG_H