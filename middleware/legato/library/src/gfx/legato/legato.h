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
    legato.h

  Summary:
    The header file joins all header files used in the graphics object library.

  Description:
    This header file includes all the header files required to use the
    Harmony Graphics Object Library. 
*******************************************************************************/
/** \file legato.h
 * @brief The header file joins all header files used in the graphics object library.
 *
 * @details This header file includes all the header files required to use
 * the Harmony Graphics Object Library.
 */

#ifndef LEGATO_H
#define LEGATO_H

////////////////////////////// INCLUDES //////////////////////////////
#include "gfx/legato/common/legato_common.h"

#include "gfx/legato/common/legato_color.h"
#include "gfx/legato/common/legato_math.h"
#include "gfx/legato/core/legato_input.h"
#include "gfx/legato/core/legato_scheme.h"
#include "gfx/legato/core/legato_state.h"
#include "gfx/legato/core/legato_stream.h"
#include "gfx/legato/font/legato_font.h"
#include "gfx/legato/image/legato_image.h"
#include "gfx/legato/image/legato_palette.h"
#include "gfx/legato/memory/legato_memory.h"
#include "gfx/driver/gfx_driver.h"
#include "gfx/legato/renderer/legato_renderer.h"
#include "gfx/legato/string/legato_dynamicstring.h"
#include "gfx/legato/string/legato_fixedstring.h"
#include "gfx/legato/string/legato_tablestring.h"

#include "gfx/legato/widget/legato_editwidget.h"

#if LE_ARC_WIDGET_ENABLED
#include "gfx/legato/widget/arc/legato_widget_arc.h"
#endif

#if LE_BARGRAPH_WIDGET_ENABLED
#include "gfx/legato/widget/bargraph/legato_widget_bar_graph.h"
#endif

#if LE_BUTTON_WIDGET_ENABLED
#include "gfx/legato/widget/button/legato_widget_button.h"
#endif

#if LE_CHECKBOX_WIDGET_ENABLED
#include "gfx/legato/widget/checkbox/legato_widget_checkbox.h"
#endif

#if LE_CIRCLE_WIDGET_ENABLED
#include "gfx/legato/widget/circle/legato_widget_circle.h"
#endif

#if LE_CIRCULARGAUGE_WIDGET_ENABLED
#include "gfx/legato/widget/circulargauge/legato_widget_circular_gauge.h"
#endif

#if LE_CIRCULARSLIDER_WIDGET_ENABLED
#include "gfx/legato/widget/circularslider/legato_widget_circular_slider.h"
#endif

#if LE_DRAWSURFACE_WIDGET_ENABLED
#include "gfx/legato/widget/drawsurface/legato_widget_drawsurface.h"
#endif

#if LE_GRADIENT_WIDGET_ENABLED
#include "gfx/legato/widget/gradient/legato_widget_gradient.h"
#endif

#if LE_GROUPBOX_WIDGET_ENABLED
#include "gfx/legato/widget/groupbox/legato_widget_groupbox.h"
#endif

#if LE_IMAGE_WIDGET_ENABLED
#include "gfx/legato/widget/image/legato_widget_image.h"
#endif

#if LE_IMAGEROTATE_WIDGET_ENABLED
#include "gfx/legato/widget/imagerotate/legato_widget_imagerotate.h"
#endif

#if LE_IMAGESCALE_WIDGET_ENABLED
#include "gfx/legato/widget/imagescale/legato_widget_imagescale.h"
#endif

#if LE_IMAGESEQUENCE_WIDGET_ENABLED
#include "gfx/legato/widget/imagesequence/legato_widget_imagesequence.h"
#endif

#if LE_KEYPAD_WIDGET_ENABLED
#include "gfx/legato/widget/keypad/legato_widget_keypad.h"
#endif

#if LE_LABEL_WIDGET_ENABLED
#include "gfx/legato/widget/label/legato_widget_label.h"
#endif

#if LE_LINE_WIDGET_ENABLED
#include "gfx/legato/widget/line/legato_widget_line.h"
#endif

#if LE_LINEGRAPH_WIDGET_ENABLED
#include "gfx/legato/widget/linegraph/legato_widget_line_graph.h"
#endif

#if LE_LIST_WIDGET_ENABLED
#include "gfx/legato/widget/list/legato_widget_list.h"
#endif

#if LE_LISTWHEEL_WIDGET_ENABLED
#include "gfx/legato/widget/listwheel/legato_widget_listwheel.h"
#endif

#if LE_PIECHART_WIDGET_ENABLED
#include "gfx/legato/widget/piechart/legato_widget_pie_chart.h"
#endif

#if LE_PROGRESSBAR_WIDGET_ENABLED
#include "gfx/legato/widget/progressbar/legato_widget_progressbar.h"
#endif

#if LE_RADIOBUTTON_WIDGET_ENABLED
#include "gfx/legato/widget/radiobutton/legato_widget_radiobutton.h"
#endif

#if LE_RADIALMENU_WIDGET_ENABLED
#include "gfx/legato/widget/radialmenu/legato_widget_radial_menu.h"
#endif

#if LE_RECTANGLE_WIDGET_ENABLED
#include "gfx/legato/widget/rectangle/legato_widget_rectangle.h"
#endif

#if LE_SCROLLBAR_WIDGET_ENABLED
#include "gfx/legato/widget/scrollbar/legato_widget_scrollbar.h"
#endif

#if LE_SLIDER_WIDGET_ENABLED
#include "gfx/legato/widget/slider/legato_widget_slider.h"
#endif

#if LE_TEXTFIELD_WIDGET_ENABLED
#include "gfx/legato/widget/textfield/legato_widget_textfield.h"
#endif

#if LE_TOUCHTEST_WIDGET_ENABLED
#include "gfx/legato/widget/touchtest/legato_widget_touchtest.h"
#endif

#if LE_WINDOW_WIDGET_ENABLED
#include "gfx/legato/widget/window/legato_widget_window.h"
#endif






    

    
#endif // LEGATO_H
