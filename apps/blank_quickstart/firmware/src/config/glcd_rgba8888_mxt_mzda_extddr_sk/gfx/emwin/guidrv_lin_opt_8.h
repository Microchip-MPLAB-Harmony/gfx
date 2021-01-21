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


/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _FillRectOpt8
*
* Purpose:
*   Optimized filling routine for 8 bpp
*/
static void _FillRectOpt8(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1) {
  DRIVER_CONTEXT * pContext;
  U32 Data, ColorMask, AndMask, Off0, OffLine;
  int NumPixel_0, NumPixel_1, RemPixels, NumLines, RemLines, RemItems;
  LCD_PIXELINDEX ColorIndex;
  U32 * pDest;

  pContext   = (DRIVER_CONTEXT *)pDevice->u.pContext;
  ColorIndex = LCD__GetColorIndex();
  Off0       = XY2OFF32(pContext->vxSizePhys, x0, y0);
  RemPixels  = x1 - x0 + 1;
  NumLines   = y1 - y0 + 1;
  OffLine    = pContext->vxSizePhys >> 2;
  NumPixel_0 = x0 & 3;
  NumPixel_1 = x1 & 3;
  if (GUI_pContext->DrawMode & LCD_DRAWMODE_XOR) {
    //
    // First DWORD
    //
    if (NumPixel_0) {
      for (RemLines = NumLines; RemLines; RemLines--) {
        pDest   = ((U32 *)pContext->VRAMAddr) + Off0 + OffLine * (RemLines - 1);
        AndMask = ~(0xFFFFFFFF << (8 * NumPixel_0));
        if ((RemPixels < 3) && (NumPixel_1)) {
          AndMask |= ~(0xFFFFFFFF >> (8 * (3 - NumPixel_1)));
        }
        #if (LCD_ENDIAN_BIG == 1)
          MIRROR(AndMask);
        #endif
        Data = READ_MEM32P(pDest);
        Data ^= ~AndMask;
        WRITE_MEM32P(pDest, Data);
      }
      Off0++;
      RemPixels -= (4 - NumPixel_0);
    }
    //
    // Complete DWORDS
    //
    if (RemPixels >= 4) {
      for (RemLines = NumLines; RemLines; RemLines--) {
        RemItems = RemPixels;
        pDest    = ((U32 *)pContext->VRAMAddr) + Off0 + OffLine * (RemLines - 1);
        while (RemItems >= 4) {
          Data  = READ_MEM32P(pDest);
          Data ^= 0xFFFFFFFF;
          WRITE_MEM32P(pDest, Data);
          pDest++;
          RemItems -= 4;
        }
      }
      Off0      += (RemPixels >> 2);
      RemPixels -= (RemPixels >> 2) << 2;
    }
    //
    // Last DWORD
    //
    if (RemPixels > 0) {
      for (RemLines = NumLines; RemLines; RemLines--) {
        pDest   = ((U32 *)pContext->VRAMAddr) + Off0 + OffLine * (RemLines - 1);
        AndMask = 0xFFFFFF00 << (8 * NumPixel_1);
        #if (LCD_ENDIAN_BIG == 1)
          MIRROR(AndMask);
        #endif
        Data = READ_MEM32P(pDest);
        Data ^= ~AndMask;
        WRITE_MEM32P(pDest, Data);
      }
    }
  } else {
    //
    // First DWORD
    //
    if (NumPixel_0) {
      for (RemLines = NumLines; RemLines; RemLines--) {
        pDest   = ((U32 *)pContext->VRAMAddr) + Off0 + OffLine * (RemLines - 1);
        AndMask = ~(0xFFFFFFFF << (8 * NumPixel_0));
        if ((RemPixels < 3) && (NumPixel_1)) {
          AndMask |= ~(0xFFFFFFFF >> (8 * (3 - NumPixel_1)));
        }
        ColorMask = (ColorIndex * 0x01010101) & ~AndMask;
        #if (LCD_ENDIAN_BIG == 1)
          MIRROR(AndMask);
          MIRROR(ColorMask);
        #endif
        Data = READ_MEM32P(pDest);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM32P(pDest, Data);
      }
      Off0++;
      RemPixels -= (4 - NumPixel_0);
    }
    //
    // Complete DWORDS
    //
    if (RemPixels >= 4) {
      ColorMask = ColorIndex * 0x01010101;
      for (RemLines = NumLines; RemLines; RemLines--) {
        RemItems = RemPixels;
        pDest    = ((U32 *)pContext->VRAMAddr) + Off0 + OffLine * (RemLines - 1);
        while (RemItems >= 32) {
          WRITE_MEM32P(pDest    , ColorMask);
          WRITE_MEM32P(pDest + 1, ColorMask);
          WRITE_MEM32P(pDest + 2, ColorMask);
          WRITE_MEM32P(pDest + 3, ColorMask);
          WRITE_MEM32P(pDest + 4, ColorMask);
          WRITE_MEM32P(pDest + 5, ColorMask);
          WRITE_MEM32P(pDest + 6, ColorMask);
          WRITE_MEM32P(pDest + 7, ColorMask);
          pDest += 8;
          RemItems -= 32;
        }
        while (RemItems >= 4) {
          WRITE_MEM32P(pDest, ColorMask);
          pDest++;
          RemItems -= 4;
        }
      }
      Off0      += (RemPixels >> 2);
      RemPixels -= (RemPixels >> 2) << 2;
    }
    //
    // Last DWORD
    //
    if (RemPixels > 0) {
      for (RemLines = NumLines; RemLines; RemLines--) {
        pDest   = ((U32 *)pContext->VRAMAddr) + Off0 + OffLine * (RemLines - 1);
        AndMask = 0xFFFFFF00 << (8 * NumPixel_1);
        ColorMask = (ColorIndex * 0x01010101) & ~AndMask;
        #if (LCD_ENDIAN_BIG == 1)
          MIRROR(AndMask);
          MIRROR(ColorMask);
        #endif
        Data = READ_MEM32P(pDest);
        Data &= AndMask;
        Data |= ColorMask;
        WRITE_MEM32P(pDest, Data);
      }
    }
  }
}

/*************************** End of file ****************************/
