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


#include <gfx/legato/legato.h>
#include "gfx/legato/widget/list/legato_widget_list.h"

#if LE_LIST_WIDGET_ENABLED == 1 && LE_SCROLLBAR_WIDGET_ENABLED == 1

#include "gfx/legato/common/legato_error.h"
#include "gfx/legato/common/legato_utils.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/legato/string/legato_string.h"
#include "gfx/legato/widget/list/legato_widget_list.h"
#include "gfx/legato/widget/scrollbar/legato_widget_scrollbar.h"
#include "gfx/legato/widget/legato_widget.h"


#if LE_DEBUG == 1
#include "gfx/legato/core/legato_debug.h"
#endif


#define DEFAULT_WIDTH           100
#define DEFAULT_HEIGHT          100

#define DEFAULT_SCROLL_WIDTH    20
#define DEFAULT_STEP_SIZE       10

#define DEFAULT_MARGIN          10

static
#if LE_DYNAMIC_VTABLES == 0
const
#endif
leListWidgetVTable listWidgetVTable;

void _leListWidget_GetListRect(const leListWidget* _this,
                               leRect* rect);
                               
void _leListWidget_GetRowRect(const leListWidget* _this,
                              uint32_t idx,
                              leRect* rect);
                              
void _leListWidget_GetTextRect(const leListWidget* _this,
                               uint32_t idx,
                               leRect* textRect,
                               leRect* drawRect);       
                               
void _leListWidget_GetIconRect(const leListWidget* _this,
                               uint32_t idx,
                               leRect* iconRect,
                               leRect* imgSrcRect);     
                               
void _leListWidget_GetLogicalRect(const leListWidget* lst, leRect* rect);    

void _leListWidget_RecalculateRowRect(leListWidget* lst,
                                      uint32_t idx);   

static void _invalidateListArea(const leListWidget* _this)
{
    leRect listArea;
    
    _leListWidget_GetListRect(_this, &listArea);

    leUtils_RectToScreenSpace((leWidget*)_this->widget.parent, &listArea);
    
    _this->fn->_damageArea(_this, &listArea);
}                         

static void _recalculateScrollBarValues(const leListWidget* _this)
{
    leRect totalRect;
    
    _leListWidget_GetLogicalRect(_this, &totalRect);
    
    if(totalRect.height <= _this->widget.rect.height)
    {
        if(_this->scrollbar->fn->getVisible(_this->scrollbar) == LE_TRUE)
        {
            _this->fn->invalidate(_this);
        }
        
        _this->scrollbar->fn->setVisible(_this->scrollbar, LE_FALSE);
    }
    else
    {
        if(_this->scrollbar->fn->getVisible(_this->scrollbar) == LE_FALSE)
        {
            _this->fn->invalidate(_this);
        }
            
        _this->scrollbar->fn->setVisible(_this->scrollbar, LE_TRUE);
        _this->scrollbar->fn->setMaximumValue(_this->scrollbar, totalRect.height - _this->widget.rect.height);
    }
}

static int32_t _getScrollBarRowLocation(const leListWidget* _this, uint32_t idx)
{
    uint32_t row = 0;
    leRect rect;
    int32_t height = 0;
    int32_t y = 0;
    
    int32_t max = _this->scrollbar->max;
    
    float percent;
    
    // avoid divide by zero
    if(max == 0)
        return 0;
        
    if(idx == _this->items.size - 1)
        return max;
    
    for(row = 0; row < _this->items.size; row++)
    {
        _leListWidget_GetRowRect(_this, row, &rect);
        
        if(row == idx)
        {
            y = height;
        }
        
        height += rect.height;
    }
    
    if(height == 0)
        return 0;
    
    percent = (float)y / (float)height;
    
    percent *= (float)max;
    
    return (int32_t)percent;
}

static void _scrollBarMoved(leScrollBarWidget* bar)
{
    _invalidateListArea((leListWidget*)bar->widget.parent);
}

static int32_t _getRowAtPoint(const leListWidget* _this,
                              const lePoint* pnt)
{
    int32_t idx;
    leRect row;
    
    for(idx = 0; idx < (int32_t)_this->items.size; idx++)
    {
        _leListWidget_GetRowRect(_this, idx, &row);
        
        if(leRectContainsPoint(&row, pnt) == LE_TRUE)
            return idx;
    }
    
    return -1;
}

static void _invalidateRow(const leListWidget* _this, int32_t row)
{
    leRect rowArea;
    
    _leListWidget_GetRowRect(_this, row, &rowArea);
    
    _this->fn->_damageArea(_this, &rowArea);
}

static void stringPreinvalidate(const leString* str,
                                leListWidget* lst)
{
    (void)str; // unused

    _invalidateListArea(lst);
}

static void stringInvalidate(const leString* str,
                             leListWidget* lst)
{
    (void)str; // unused

    _invalidateListArea(lst);
}

static void handleLanguageChangeEvent(leListWidget* _this)
{
    LE_ASSERT_THIS();

    _invalidateListArea(_this);
}

void leListWidget_Constructor(leListWidget* _this)
{
    leWidget_Constructor((leWidget*)_this);
    
    _this->widget.fn = (void*)&listWidgetVTable;
    _this->fn = &listWidgetVTable;
    
    _this->widget.type = LE_WIDGET_LIST;

    // override base class methods
    _this->widget.rect.width = DEFAULT_WIDTH;
    _this->widget.rect.height = DEFAULT_HEIGHT;

    _this->widget.style.borderType = LE_WIDGET_BORDER_BEVEL;
    _this->widget.style.backgroundType = LE_WIDGET_BACKGROUND_FILL;
    _this->itemDown = -1;
    _this->mode = LE_LIST_WIDGET_SELECTION_MODE_SINGLE;
    _this->allowEmpty = LE_FALSE;

    leArray_Create(&_this->items);
    
    // create scrollbar
    
    _this->scrollbar = leScrollBarWidget_New();
    _this->scrollbar->fn->setValueChangedEventCallback(_this->scrollbar, &_scrollBarMoved);
    
    _this->fn->addChild(_this, (leWidget*)_this->scrollbar);
    
    _this->scrollbar->fn->setSize(_this->scrollbar,
                                  DEFAULT_SCROLL_WIDTH,
                                  _this->widget.rect.height);
                     
    _this->scrollbar->fn->setPosition(_this->scrollbar,
                                    _this->widget.rect.width - DEFAULT_SCROLL_WIDTH,
                                    0);
                         
    _this->scrollbar->fn->setStepSize(_this->scrollbar, DEFAULT_STEP_SIZE);
                         
    _recalculateScrollBarValues(_this);
    
    _this->widget.style.halign = LE_HALIGN_CENTER;
    _this->iconMargin = DEFAULT_MARGIN;
    _this->iconPos = LE_RELATIVE_POSITION_LEFTOF;

    _this->cb = NULL;
}

void _leWidget_Destructor(leWidget* wgt);

static void destructor(leListWidget* _this)
{
    // free all item memory
    _this->fn->removeAllItems(_this);

    _leWidget_Destructor((leWidget*)_this);
}

leListWidget* leListWidget_New()
{
    leListWidget* lst = NULL;

    lst = LE_MALLOC(sizeof(leListWidget));

    leListWidget_Constructor(lst);

    return lst;
}

static void resized(leListWidget* _this, leWidget_ResizeEvent* evt)
{
    (void)evt; // unused

    _recalculateScrollBarValues(_this);

    _this->scrollbar->fn->setSize(_this->scrollbar,
                                  _this->scrollbar->widget.rect.width,
                                  _this->widget.rect.height);
                     
    _this->scrollbar->fn->setPosition(_this->scrollbar,
                                      _this->widget.rect.width - _this->scrollbar->widget.rect.width,
                                      0);
}



static leListWidget_SelectionMode getSelectionMode(const leListWidget* _this)
{
    LE_ASSERT_THIS();
        
    return _this->mode;
}

static leResult setSelectionMode(leListWidget* _this, 
                                 leListWidget_SelectionMode mode)
{
    LE_ASSERT_THIS();
        
    if(_this->mode == mode)
        return LE_SUCCESS;
        
    _this->mode = mode;
    
    _this->fn->deselectAll(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getAllowEmptySelection(const leListWidget* _this)
{
    LE_ASSERT_THIS();
    
    return _this->allowEmpty;
}

static leResult setAllowEmptySelection(leListWidget* _this,
                                       leBool allow)
{
    LE_ASSERT_THIS();
        
    if(_this->allowEmpty == allow)
        return LE_SUCCESS;
        
    _this->allowEmpty = allow;
    
    // automatically select if needed
    if(allow == LE_FALSE && _this->fn->getSelectionCount(_this) == 0)
    {
        _this->fn->setItemSelected(_this, 0, LE_TRUE);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leRelativePosition getIconPosition(const leListWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->iconPos;
}
                                               
static leResult setIconPosition(leListWidget* _this,
                                leRelativePosition pos)
{   
    LE_ASSERT_THIS();
    
    if(pos != LE_RELATIVE_POSITION_LEFTOF && pos != LE_RELATIVE_POSITION_RIGHTOF)
        return LE_FAILURE;
        
    if(_this->iconPos == pos)
        return LE_SUCCESS;

    _this->iconPos = pos;

    _invalidateListArea(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static uint32_t getIconMargin(const leListWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->iconMargin;
}
                                               
static leResult setIconMargin(leListWidget* _this,
                              uint32_t mg)
{
    LE_ASSERT_THIS();
        
    if(_this->iconMargin == mg)
        return LE_SUCCESS;

    _this->iconMargin = mg;

    _invalidateListArea(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}
                                                        
static uint32_t getItemCount(const leListWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->items.size;
}

static int32_t appendItem(leListWidget* _this)
{
    leListItem* item;
    
    LE_ASSERT_THIS();
        
    item = LE_MALLOC(sizeof(leListItem));
    
    if(item == NULL)
        return -1;

    item->enabled = LE_TRUE;
    item->string = NULL;
    item->icon = NULL;
    item->selected = LE_FALSE;
        
    leArray_PushBack(&_this->items, item);
    
    if(_this->allowEmpty == LE_FALSE && _this->fn->getSelectionCount(_this) == 0)
    {
        _this->fn->setItemSelected(_this,
                                   _this->items.size - 1,
                                   LE_TRUE);
    }
    
    _leListWidget_RecalculateRowRect(_this, _this->items.size - 1);

    _recalculateScrollBarValues(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return _this->items.size - 1;
}

static int32_t insertItem(leListWidget* _this,
                          int32_t idx)
{
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0)
        return LE_FAILURE;
        
    item = LE_MALLOC(sizeof(leListItem));
    
    if(item == NULL)
        return -1;
        
    item->enabled = LE_TRUE;
    item->string = NULL;
    item->icon = NULL;
    item->selected = LE_FALSE;    
        
    leArray_InsertAt(&_this->items, idx, item);
    
    if(_this->allowEmpty == LE_FALSE && _this->fn->getSelectionCount(_this) == 0)
    {
        _this->fn->setItemSelected(_this, idx, LE_TRUE);
    }
    
    _leListWidget_RecalculateRowRect(_this, idx);

    _recalculateScrollBarValues(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return idx;
}

static leResult removeItem(leListWidget* _this,
                           int32_t idx)
{
    leResult res;
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
        
    item = _this->items.values[idx];
    
    if(item == NULL)
        return LE_FAILURE;
    
    item->string = NULL;
        
    res = leArray_RemoveAt(&_this->items, idx);
    
    LE_FREE(item);

    if(res == LE_SUCCESS)
    {
        _invalidateListArea(_this);
    }
        
    _recalculateScrollBarValues(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return res;
}

static leResult removeAllItems(leListWidget* _this)
{
    leListItem* item;
    uint32_t i;

    LE_ASSERT_THIS();

    for(i = 0; i < _this->items.size; i++)
    {
        item = _this->items.values[i];
        
        if(item != NULL)
        {
            LE_FREE(item);
        }
    }
        
    leArray_Clear(&_this->items);
    
    _this->fn->invalidate(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getItemSelected(const leListWidget* _this,
                              int32_t idx)
{
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FALSE;
    
    item = (leListItem*)_this->items.values[idx];
    
    if(item == NULL)
        return LE_FALSE;
        
    return item->selected;
}

static leResult setItemSelected(leListWidget* _this,
                                int32_t idx,
                                leBool selected)
{
    leListItem *item, *old;
    uint32_t firstIdx, lastIdx, count;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
        
    count = _this->fn->getSelectionCount(_this);
        
    item = (leListItem*)_this->items.values[idx];
        
    if(item == NULL)
        return LE_FAILURE;
        
    //Prevent a disabled item from being selected
    if (item->enabled == LE_FALSE)
    {
        selected = LE_FALSE;
    }
        
    // nothing to do
    if(item->selected == selected)
        return LE_SUCCESS;
        
    if(_this->mode == LE_LIST_WIDGET_SELECTION_MODE_SINGLE)
    {
        if(_this->allowEmpty == LE_FALSE && selected == LE_FALSE)
            return LE_FAILURE;
          
        if(count > 0)
        {
            firstIdx = _this->fn->getFirstSelectedItem(_this);
            
            old = _this->items.values[firstIdx];
            
            old->selected = LE_FALSE;
            
            _invalidateRow(_this, firstIdx);
            //leListWidget_DeselectAll(_this);
        }
        
        item->selected = selected;
        
        if(_this->cb != NULL)
        {
            _this->cb(_this, idx, selected);
        }
            
        _invalidateRow(_this, idx);
    }
    else if(_this->mode == LE_LIST_WIDGET_SELECTION_MODE_MULTIPLE)
    {
        if(_this->allowEmpty == LE_FALSE &&
           selected == LE_FALSE &&
           _this->fn->getSelectionCount(_this) == 1)
        {
            return LE_FAILURE;
        }
            
        item = _this->items.values[idx];
        
        item->selected = selected;
        
        if(_this->cb != NULL)
        {
            _this->cb(_this, idx, selected);
        }
            
        _invalidateRow(_this, idx);
    }
    else if(_this->mode == LE_LIST_WIDGET_SELECTION_MODE_CONTIGUOUS)
    {
        if(_this->allowEmpty == LE_FALSE &&
           selected == LE_FALSE &&
           _this->fn->getSelectionCount(_this) == 1)
        {
            return LE_FAILURE;
        }
            
        // empty selection case
        if(count == 0 && selected == LE_TRUE)
        {
            item->selected = selected;
            
            if(_this->cb != NULL)
            {
                _this->cb(_this, idx, selected);
            }
                
            _invalidateRow(_this, idx);
        }
        // test range
        else
        {
            firstIdx = _this->fn->getFirstSelectedItem(_this);
            lastIdx = _this->fn->getLastSelectedItem(_this);
        
            if(selected == LE_TRUE)
            {
                // selection is contiguous, add to selection        
                if((uint32_t)idx == firstIdx - 1 || (uint32_t)idx == lastIdx + 1)
                {
                    item->selected = selected;
                
                    if(_this->cb != NULL)
                    {
                        _this->cb(_this, idx, selected);
                    }
                        
                    _invalidateRow(_this, idx);
                }
                // outside of range, deselect all and select target
                else
                {
                    if (item->enabled == LE_TRUE)
                    {
                        _this->fn->deselectAll(_this);

                        item->selected = selected;

                        if(_this->cb != NULL)
                        {
                            _this->cb(_this, idx, selected);
                        }

                        _invalidateListArea(_this);
                    }
                }
            }
            else
            {
                // selection is contiguous, remove from end caps        
                if((uint32_t)idx == firstIdx || (uint32_t)idx == lastIdx)
                {
                    item->selected = selected;
                
                    if(_this->cb != NULL)
                    {
                        _this->cb(_this, idx, selected);
                    }
                        
                    _invalidateListArea(_this);
                }
                // outside of range, deselect all and select target
                else
                {
                    if (item->enabled == LE_TRUE)
                    {
                        _this->fn->deselectAll(_this);

                        if(_this->allowEmpty == LE_FALSE)
                        {
                            item->selected = LE_TRUE;

                            if(_this->cb != NULL)
                            {
                                _this->cb(_this, idx, LE_TRUE);
                            }
                            
                            _invalidateListArea(_this);
                        }
                    }
                }
            }
        }
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}
                                                 
static leResult toggleItemSelected(leListWidget* _this,
                                   int32_t idx)
{
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
    
    item = (leListItem*)_this->items.values[idx];
    
    if(item == NULL)
        return LE_FAILURE;

    return _this->fn->setItemSelected(_this, idx, !item->selected);
}

static leResult selectAll(leListWidget* _this)
{
    leListItem* item;
    uint32_t i;
    uint32_t count = 0;
    
    LE_ASSERT_THIS();
        
    if(_this->mode == LE_LIST_WIDGET_SELECTION_MODE_SINGLE)
    {
        _this->fn->setItemSelected(_this, 0, LE_TRUE);
        
        return LE_SUCCESS;
    }
    
    for(i = 0; i < _this->items.size; i++)
    {
        item = _this->items.values[i];
        
        if (item->enabled == LE_TRUE)
        {
            if(item->selected == LE_FALSE)
            {
                item->selected = LE_TRUE;

                if(_this->cb != NULL)
                {
                    _this->cb(_this, i, LE_TRUE);
                }
                
                count++;
            }
        }
    }
    
    if(count > 0)
    {
        _invalidateListArea(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult deselectAll(leListWidget* _this)
{
    leListItem* item;
    uint32_t i;
    uint32_t count = 0;
    
    LE_ASSERT_THIS();
    
    for(i = 0; i < _this->items.size; i++)
    {
        item = _this->items.values[i];
        
        if(item->selected == LE_TRUE)
        {
            item->selected = LE_FALSE;
            
            if(_this->cb != NULL)
            {
                _this->cb(_this, i, LE_FALSE);
            }
                
            count++;
        }
    }
    
    if(count > 0)
    {
        _invalidateListArea(_this);
    }

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static int32_t getFirstSelectedItem(const leListWidget* _this)
{
    int32_t idx;
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    for(idx = 0; idx < (int32_t)_this->items.size; idx++)
    {
        item = _this->items.values[idx];
        
        if(item->selected == LE_TRUE)
            return idx;
    }
    
    return -1;
}

static int32_t getLastSelectedItem(const leListWidget* _this)
{
    int32_t idx;
    int32_t last = -1;
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    for(idx = 0; idx < (int32_t)_this->items.size; idx++)
    {
        item = _this->items.values[idx];
        
        if(item->selected == LE_TRUE)
        {
            last = idx;
        }
    }
    
    return last;
}

static uint32_t getSelectionCount(const leListWidget* _this)
{
    uint32_t idx, count = 0;
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    for(idx = 0; idx < _this->items.size; idx++)
    {
        item = _this->items.values[idx];
        
        if(item->selected == LE_TRUE)
        {
            count++;
        }
    }
    
    return count;
}

static leString* getItemString(const leListWidget* _this,
                               int32_t idx)
{
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return NULL;
        
    item = _this->items.values[idx];
    
    return (leString*)item->string;
}
                                               
static leResult setItemString(leListWidget* _this,
                              int32_t idx,
                              const leString* str)
{
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
       
    item = _this->items.values[idx];

    if(item->string != NULL)
    {
        _invalidateListArea(_this);

        item->string->fn->setPreInvalidateCallback((leString*)item->string,
                                                   NULL,
                                                   NULL);

        item->string->fn->setInvalidateCallback((leString*)item->string,
                                                NULL,
                                                NULL);
    }
    
    item->string = str;

    item->string->fn->setPreInvalidateCallback((leString*)item->string,
                                               (leString_InvalidateCallback)stringPreinvalidate,
                                               _this);

    item->string->fn->setInvalidateCallback((leString*)item->string,
                                            (leString_InvalidateCallback)stringInvalidate,
                                            _this);
    
    _leListWidget_RecalculateRowRect(_this, idx);

    _invalidateListArea(_this);
    
    _recalculateScrollBarValues(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}
                                               
static leImage* getItemIcon(const leListWidget* _this,
                            int32_t idx)
{
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return NULL;
        
    item = _this->items.values[idx];
        
    return (leImage*)item->icon;
}
                                                              
static leResult setItemIcon(leListWidget* _this,
                            int32_t idx,
                            const leImage* img)
{
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
        
    item = _this->items.values[idx];
        
    item->icon = img;
    
    _leListWidget_RecalculateRowRect(_this, idx);

    _recalculateScrollBarValues(_this);

    _invalidateListArea(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leBool getItemEnable(const leListWidget* _this,
                            int32_t idx)
{
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
        
    item = _this->items.values[idx];
    
    return item->enabled;
}

static leResult setItemEnable(leListWidget* _this,
                              int32_t idx,
                              leBool enable)
{
    leListItem* item;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
        
    item = _this->items.values[idx];
    
    if (item->enabled == enable)
        return LE_SUCCESS;
        
    item->enabled = enable;
    
    if (enable == LE_FALSE)
    {
        item->selected = LE_FALSE;
    }
    
    _invalidateListArea(_this);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leResult setItemVisible(leListWidget* _this,
                               int32_t idx)
{
    int32_t y;
    
    LE_ASSERT_THIS();
    
    if(idx < 0 || idx >= (int32_t)_this->items.size)
        return LE_FAILURE;
        
    y = _getScrollBarRowLocation(_this, idx);
    
    _this->scrollbar->fn->setScrollValue(_this->scrollbar, y);

#if LE_DEBUG == 1
    _leDebugNotify_WidgetPropertyChanged((leWidget*)_this);
#endif

    return LE_SUCCESS;
}

static leListWidget_SelectedItemChangedEvent getSelectedItemChangedEventCallback(const leListWidget* _this)
{
    LE_ASSERT_THIS();

    return _this->cb;
}

static leResult setSelectedItemChangedEventCallback(leListWidget* _this,
                                                    leListWidget_SelectedItemChangedEvent cb)
{
    LE_ASSERT_THIS();
    
    if(_this->cb == cb)
        return LE_FAILURE;

    _this->cb = cb;
    
    return LE_SUCCESS;
}

static void handleTouchDownEvent(leListWidget* _this,
                                 leWidgetEvent_TouchDown* evt)
{
    lePoint pnt;
    int32_t idx;
    
    LE_ASSERT_THIS();
    
    pnt.x = evt->x;
    pnt.y = evt->y;
    
    idx = _getRowAtPoint(_this, &pnt);
    
    if(idx >= 0 && idx < (int32_t)_this->items.size)
    {
        _this->itemDown = idx;
        
        _invalidateRow(_this, idx);
    }
    else if(_this->itemDown != -1)
    {
        _this->itemDown = -1;
     
        _invalidateRow(_this, idx);
    }

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

static void handleTouchUpEvent(leListWidget* _this,
                               leWidgetEvent_TouchUp* evt)
{
    lePoint pnt;
    int32_t idx;
    
    LE_ASSERT_THIS();
    
    pnt.x = evt->x;
    pnt.y = evt->y;
    
    idx = _getRowAtPoint(_this, &pnt);
    
    if(idx == _this->itemDown)
    {
        _this->fn->toggleItemSelected(_this, idx); 
    }
    else if(_this->itemDown != -1)
    {
        _invalidateRow(_this, _this->itemDown);
    }
    
    _this->itemDown = -1;

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

static void handleTouchMovedEvent(leListWidget* _this,
                                  leWidgetEvent_TouchMove* evt)
{
    lePoint pnt;
    int32_t idx;
    
    LE_ASSERT_THIS();
    
    pnt.x = evt->x;
    pnt.y = evt->y;
    
    idx = _getRowAtPoint(_this, &pnt);
    
    if(idx >= 0 && idx < (int32_t)_this->items.size)
    {
        if(_this->itemDown != idx)
        {
            if (_this->itemDown != -1)
            {
                _invalidateRow(_this, _this->itemDown);
            }
            
            _this->itemDown = idx;
        
            _invalidateRow(_this, _this->itemDown);
        }
    }
    else if(_this->itemDown != -1)
    {
        _invalidateRow(_this, _this->itemDown);
        
        _this->itemDown = -1;
    }

    leWidgetEvent_Accept((leWidgetEvent*)evt, (leWidget*)_this);
}

void _leListWidget_Paint(leListWidget* _this);

#if LE_DYNAMIC_VTABLES == 1
void _leWidget_FillVTable(leWidgetVTable* tbl);

void _leListWidget_GenerateVTable()
{
    _leWidget_FillVTable((void*)&listWidgetVTable);
    
    /* overrides from base class */
    listWidgetVTable._destructor = destructor;
    listWidgetVTable._paint = _leListWidget_Paint;
    listWidgetVTable.resizeEvent = resized;
    listWidgetVTable.touchDownEvent = handleTouchDownEvent;
    listWidgetVTable.touchUpEvent = handleTouchUpEvent;
    listWidgetVTable.touchMoveEvent = handleTouchMovedEvent;
    listWidgetVTable.languageChangeEvent = handleLanguageChangeEvent;
    
    /* member functions */
    listWidgetVTable.getSelectionMode = getSelectionMode;
    listWidgetVTable.setSelectionMode = setSelectionMode;
    listWidgetVTable.getAllowEmptySelection = getAllowEmptySelection;
    listWidgetVTable.setAllowEmptySelection = setAllowEmptySelection;
    listWidgetVTable.getIconPosition = getIconPosition;
    listWidgetVTable.setIconPosition = setIconPosition;
    listWidgetVTable.getIconMargin = getIconMargin;
    listWidgetVTable.setIconMargin = setIconMargin;
    listWidgetVTable.getItemCount = getItemCount;
    listWidgetVTable.appendItem = appendItem;
    listWidgetVTable.insertItem = insertItem;
    listWidgetVTable.removeItem = removeItem;
    listWidgetVTable.removeAllItems = removeAllItems;
    listWidgetVTable.getItemSelected = getItemSelected;
    listWidgetVTable.setItemSelected = setItemSelected;
    listWidgetVTable.toggleItemSelected = toggleItemSelected;
    listWidgetVTable.selectAll = selectAll;
    listWidgetVTable.deselectAll = deselectAll;
    listWidgetVTable.getFirstSelectedItem = getFirstSelectedItem;
    listWidgetVTable.getLastSelectedItem = getLastSelectedItem;
    listWidgetVTable.getSelectionCount = getSelectionCount;
    listWidgetVTable.getItemString = getItemString;
    listWidgetVTable.setItemString = setItemString;
    listWidgetVTable.getItemIcon = getItemIcon;
    listWidgetVTable.setItemIcon = setItemIcon;
    listWidgetVTable.getItemEnable = getItemEnable;
    listWidgetVTable.setItemEnable = setItemEnable;
    listWidgetVTable.setItemVisible = setItemVisible;
    listWidgetVTable.getSelectedItemChangedEventCallback = getSelectedItemChangedEventCallback;
    listWidgetVTable.setSelectedItemChangedEventCallback = setSelectedItemChangedEventCallback;
}

void _leListWidget_FillVTable(leListWidgetVTable* tbl)
{
    *tbl = listWidgetVTable;
}
#else
static const leListWidgetVTable listWidgetVTable =
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

    .moveEvent = (void*)_leWidget_MoveEvent,
    .focusLostEvent = (void*)_leWidget_FocusLostEvent,
    .focusGainedEvent = (void*)_leWidget_FocusGainedEvent,

    ._handleEvent = (void*)_leWidget_HandleEvent,
    ._validateChildren = (void*)_leWidget_ValidateChildren,
    ._increaseDirtyState = (void*)_leWidget_IncreaseDirtyState,
    ._setDirtyState = (void*)_leWidget_SetDirtyState,
    ._clearDirtyState = (void*)_leWidget_ClearDirtyState,
    ._invalidateBorderAreas = (void*)_leWidget_InvalidateBorderAreas,
    ._damageArea = (void*)_leWidget_DamageArea,

    /* overrides from base class */
    ._destructor = destructor,
    ._paint = _leListWidget_Paint,
    .resizeEvent = resized,
    .touchDownEvent = handleTouchDownEvent,
    .touchUpEvent = handleTouchUpEvent,
    .touchMoveEvent = handleTouchMovedEvent,
    .languageChangeEvent = handleLanguageChangeEvent,

    /* member functions */
    .getSelectionMode = getSelectionMode,
    .setSelectionMode = setSelectionMode,
    .getAllowEmptySelection = getAllowEmptySelection,
    .setAllowEmptySelection = setAllowEmptySelection,
    .getIconPosition = getIconPosition,
    .setIconPosition = setIconPosition,
    .getIconMargin = getIconMargin,
    .setIconMargin = setIconMargin,
    .getItemCount = getItemCount,
    .appendItem = appendItem,
    .insertItem = insertItem,
    .removeItem = removeItem,
    .removeAllItems = removeAllItems,
    .getItemSelected = getItemSelected,
    .setItemSelected = setItemSelected,
    .toggleItemSelected = toggleItemSelected,
    .selectAll = selectAll,
    .deselectAll = deselectAll,
    .getFirstSelectedItem = getFirstSelectedItem,
    .getLastSelectedItem = getLastSelectedItem,
    .getSelectionCount = getSelectionCount,
    .getItemString = getItemString,
    .setItemString = setItemString,
    .getItemIcon = getItemIcon,
    .setItemIcon = setItemIcon,
    .getItemEnable = getItemEnable,
    .setItemEnable = setItemEnable,
    .setItemVisible = setItemVisible,
    .getSelectedItemChangedEventCallback = getSelectedItemChangedEventCallback,
    .setSelectedItemChangedEventCallback = setSelectedItemChangedEventCallback,
};
#endif

#endif // LE_LIST_WIDGET_ENABLED && LE_SCROLLBAR_WIDGET_ENABLED
