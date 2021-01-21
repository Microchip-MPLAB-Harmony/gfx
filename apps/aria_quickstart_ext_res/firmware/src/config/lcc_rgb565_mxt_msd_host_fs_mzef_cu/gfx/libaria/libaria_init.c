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

/*******************************************************************************
  MPLAB Harmony Graphics Composer Generated Implementation File

  File Name:
    libaria_init.c

  Summary:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

  Description:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

    Created with MPLAB Harmony Version 3.0
*******************************************************************************/

#include "gfx/libaria/libaria_init.h"
#include "gfx/libaria/inc/libaria_utils.h"

laScheme WhiteScheme;
laScheme defaultPaletteScheme;
laButtonWidget* ButtonWidget_Wait;
laLabelWidget* LabelWidget_Wait;
laButtonWidget* ButtonWidget_MainScreen;
laLabelWidget* TitleLabel;
laButtonWidget* SloganButton;
laLabelWidget* LabelWidget1;
laLabelWidget* LabelWidget2;
laLabelWidget* LabelWidget3;
laButtonWidget* LogoButton;
laLabelWidget* ImageTypeLabelWidget;


static void ScreenCreate_LoadingScreen(laScreen* screen);
static void ScreenCreate_MainScreen(laScreen* screen);


int32_t libaria_initialize(void)
{
    laScreen* screen;

    laScheme_Initialize(&WhiteScheme, GFX_COLOR_MODE_RGB_565);
    WhiteScheme.base = 0xFC;
    WhiteScheme.highlight = 0xFC;
    WhiteScheme.highlightLight = 0xF;
    WhiteScheme.shadow = 0x8;
    WhiteScheme.shadowDark = 0xED;
    WhiteScheme.foreground = 0x0;
    WhiteScheme.foregroundInactive = 0xFE;
    WhiteScheme.foregroundDisabled = 0x8;
    WhiteScheme.background = 0xF;
    WhiteScheme.backgroundInactive = 0xFE;
    WhiteScheme.backgroundDisabled = 0xFC;
    WhiteScheme.text = 0x0;
    WhiteScheme.textHighlight = 0xC;
    WhiteScheme.textHighlightText = 0xF;
    WhiteScheme.textInactive = 0xFE;
    WhiteScheme.textDisabled = 0xF6;

    laScheme_Initialize(&defaultPaletteScheme, GFX_COLOR_MODE_RGB_565);
    defaultPaletteScheme.base = 0xFC;
    defaultPaletteScheme.highlight = 0xFC;
    defaultPaletteScheme.highlightLight = 0xF;
    defaultPaletteScheme.shadow = 0x8;
    defaultPaletteScheme.shadowDark = 0xED;
    defaultPaletteScheme.foreground = 0x0;
    defaultPaletteScheme.foregroundInactive = 0xFE;
    defaultPaletteScheme.foregroundDisabled = 0x8;
    defaultPaletteScheme.background = 0xF;
    defaultPaletteScheme.backgroundInactive = 0xFE;
    defaultPaletteScheme.backgroundDisabled = 0xFC;
    defaultPaletteScheme.text = 0x0;
    defaultPaletteScheme.textHighlight = 0xC;
    defaultPaletteScheme.textHighlightText = 0xF;
    defaultPaletteScheme.textInactive = 0xFE;
    defaultPaletteScheme.textDisabled = 0xF6;

    GFX_Set(GFXF_DRAW_PIPELINE_MODE, GFX_PIPELINE_GCUGPU);
    GFX_Set(GFXF_GLOBAL_PALETTE, globalColorPalette);
    laContext_SetStringTable(&stringTable);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_LoadingScreen);
    laContext_AddScreen(screen);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_MainScreen);
    laContext_AddScreen(screen);

    laContext_SetActiveScreen(0);

	return 0;
}

static void ScreenCreate_LoadingScreen(laScreen* screen)
{
    laLayer* layer0;

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 480, 272);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetScheme((laWidget*)layer0, &WhiteScheme);
    laLayer_SetBufferCount(layer0, 1);
    laLayer_SetVSync(layer0, LA_FALSE);

    laScreen_SetLayer(screen, 0, layer0);

    ButtonWidget_Wait = laButtonWidget_New();
    laWidget_SetSize((laWidget*)ButtonWidget_Wait, 480, 272);
    laWidget_SetScheme((laWidget*)ButtonWidget_Wait, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)ButtonWidget_Wait, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)ButtonWidget_Wait, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetText(ButtonWidget_Wait, laString_CreateFromID(string_WaitForUSB));
    laButtonWidget_SetReleasedEventCallback(ButtonWidget_Wait, &ButtonWidget_Wait_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)ButtonWidget_Wait);

    LabelWidget_Wait = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget_Wait, 9, 7);
    laWidget_SetSize((laWidget*)LabelWidget_Wait, 463, 92);
    laWidget_SetScheme((laWidget*)LabelWidget_Wait, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)LabelWidget_Wait, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget_Wait, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget_Wait, laString_CreateFromID(string_WaitForUSBAlt));
    laWidget_AddChild((laWidget*)ButtonWidget_Wait, (laWidget*)LabelWidget_Wait);

    ButtonWidget_MainScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)ButtonWidget_MainScreen, 51, 182);
    laWidget_SetSize((laWidget*)ButtonWidget_MainScreen, 380, 54);
    laWidget_SetVisible((laWidget*)ButtonWidget_MainScreen, LA_FALSE);
    laWidget_SetScheme((laWidget*)ButtonWidget_MainScreen, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)ButtonWidget_MainScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)ButtonWidget_MainScreen, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(ButtonWidget_MainScreen, laString_CreateFromID(string_PressToMainScreen));
    laButtonWidget_SetReleasedEventCallback(ButtonWidget_MainScreen, &ButtonWidget_MainScreen_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)ButtonWidget_MainScreen);

}

static void ScreenCreate_MainScreen(laScreen* screen)
{
    laLayer* layer0;

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 480, 272);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetScheme((laWidget*)layer0, &WhiteScheme);
    laLayer_SetBufferCount(layer0, 1);
    laLayer_SetVSync(layer0, LA_FALSE);

    laScreen_SetLayer(screen, 0, layer0);

    TitleLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)TitleLabel, 13, 4);
    laWidget_SetSize((laWidget*)TitleLabel, 455, 55);
    laWidget_SetScheme((laWidget*)TitleLabel, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)TitleLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)TitleLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(TitleLabel, laString_CreateFromID(string_TitleString));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)TitleLabel);

    SloganButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SloganButton, 14, 207);
    laWidget_SetSize((laWidget*)SloganButton, 450, 50);
    laWidget_SetScheme((laWidget*)SloganButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)SloganButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SloganButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetReleasedEventCallback(SloganButton, &SloganButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)SloganButton);

    LabelWidget1 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget1, 5, 10);
    laWidget_SetSize((laWidget*)LabelWidget1, 78, 30);
    laWidget_SetScheme((laWidget*)LabelWidget1, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)LabelWidget1, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget1, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget1, laString_CreateFromID(string_PressButton));
    laLabelWidget_SetHAlignment(LabelWidget1, LA_HALIGN_RIGHT);
    laWidget_AddChild((laWidget*)SloganButton, (laWidget*)LabelWidget1);

    LabelWidget2 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget2, 7, 10);
    laWidget_SetSize((laWidget*)LabelWidget2, 398, 30);
    laWidget_SetScheme((laWidget*)LabelWidget2, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)LabelWidget2, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget2, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget2, laString_CreateFromID(string_DrawDirectBlit));
    laWidget_AddChild((laWidget*)SloganButton, (laWidget*)LabelWidget2);

    LabelWidget3 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)LabelWidget3, 331, 10);
    laWidget_SetSize((laWidget*)LabelWidget3, 116, 30);
    laWidget_SetScheme((laWidget*)LabelWidget3, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)LabelWidget3, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LabelWidget3, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(LabelWidget3, laString_CreateFromID(string_ImageFromExternal));
    laLabelWidget_SetHAlignment(LabelWidget3, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SloganButton, (laWidget*)LabelWidget3);

    LogoButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)LogoButton, 96, 62);
    laWidget_SetSize((laWidget*)LogoButton, 141, 132);
    laWidget_SetScheme((laWidget*)LogoButton, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)LogoButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)LogoButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(LogoButton, &MHGS_logo_small_raw_direct_blit);
    laButtonWidget_SetReleasedImage(LogoButton, &MHGS_logo_small_raw_direct_blit);
    laButtonWidget_SetPressedOffset(LogoButton, 0);
    laButtonWidget_SetReleasedEventCallback(LogoButton, &LogoButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)LogoButton);

    ImageTypeLabelWidget = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)ImageTypeLabelWidget, 251, 95);
    laWidget_SetSize((laWidget*)ImageTypeLabelWidget, 215, 51);
    laWidget_SetScheme((laWidget*)ImageTypeLabelWidget, &defaultPaletteScheme);
    laWidget_SetBackgroundType((laWidget*)ImageTypeLabelWidget, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)ImageTypeLabelWidget, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsDirectBlit));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)ImageTypeLabelWidget);

}




