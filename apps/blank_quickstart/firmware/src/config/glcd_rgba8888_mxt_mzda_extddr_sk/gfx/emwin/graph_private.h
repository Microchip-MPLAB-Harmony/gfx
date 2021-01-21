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


#ifndef GRAPH_PRIVATE_H
#define GRAPH_PRIVATE_H

#include "GRAPH.h"
#include "GUI_ARRAY.h"
#include "WIDGET.h"

#if GUI_WINSUPPORT

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct GRAPH_OBJ        GRAPH_OBJ;
typedef struct GRAPH_DATA_OBJ   GRAPH_DATA_OBJ;
typedef struct GRAPH_SCALE_OBJ  GRAPH_SCALE_OBJ;
typedef struct GRAPH_PAINT_OBJ  GRAPH_PAINT_OBJ;

struct GRAPH_PAINT_OBJ {
  void    (* pfOnPaint)  (WM_HMEM hObj, GUI_RECT * pRectInvalid); /* Pointer to paint function */ 
  void    (* pfOnDelete) (WM_HMEM hObj);                          /* Pointer to delete function */
  WM_HWIN hGraph;                                                 /* Handle of graph widget */    
};

typedef struct {
  GUI_COLOR        TextColor;
  const GUI_FONT * pFont;
} GRAPH_SCALE_PROPS;

struct GRAPH_SCALE_OBJ {
  GRAPH_PAINT_OBJ   PaintObj;
  GRAPH_SCALE_PROPS Props;
  int               Pos;
  int               TextAlign;
  unsigned          TickDist;
  int               Off;
  U16               Flags;
  float             Factor;
  int               NumDecs;
};

struct GRAPH_DATA_OBJ {
  GRAPH_PAINT_OBJ PaintObj;
  void         (* pfInvalidateNewItem)(GRAPH_DATA_OBJ * pDataObj); /* Pointer to a function which can be used for invalidating the required area */
  unsigned        NumItems;
  unsigned        MaxNumItems;
  GUI_COLOR       Color;
  int             OffX, OffY;
};

typedef struct {
  GUI_COLOR aColor[4];
  unsigned  GridSpacingX;
  unsigned  GridSpacingY;
  unsigned  GridOffX;
  unsigned  GridOffY;
  unsigned  BorderL;
  unsigned  BorderT;
  unsigned  BorderR;
  unsigned  BorderB;
} GRAPH_PROPS;

struct GRAPH_OBJ {
  WIDGET          Widget;
  GRAPH_PROPS     Props;
  GUI_ARRAY       GraphArray;
  GUI_ARRAY       ScaleArray;
  U8              ShowGrid;
  unsigned        RangeX, RangeY;
  U16             Flags;
  U8              LineStyleV;
  U8              LineStyleH;
  WM_SCROLL_STATE ScrollStateV;
  WM_SCROLL_STATE ScrollStateH;
  void            (* pUserDraw)(WM_HWIN hObj, int Stage);
};

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define GRAPH_INIT_ID(p) (p->Widget.DebugId = GRAPH_ID)
#else
  #define GRAPH_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  GRAPH_OBJ * GRAPH_LockH(GRAPH_Handle h);
  #define GRAPH_LOCK_H(h)   GRAPH_LockH(h)
#else
  #define GRAPH_LOCK_H(h)   (GRAPH_OBJ *)GUI_LOCK_H(h)
#endif

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/
extern GRAPH_PROPS GRAPH__DefaultProps;

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/
void GRAPH__AddValue       (GRAPH_DATA_OBJ * pDataObj, void * pData, void * pValue, int Size);
int  GRAPH__GetValue       (GRAPH_DATA_OBJ * pDataObj, void * pData, void * pValue, int Size, U32 Index);
void GRAPH__InvalidateGraph(GRAPH_Handle hObj);

#endif /* GUI_WINSUPPORT */
#endif /* GRAPH_PRIVATE_H */

/*************************** End of file ****************************/
