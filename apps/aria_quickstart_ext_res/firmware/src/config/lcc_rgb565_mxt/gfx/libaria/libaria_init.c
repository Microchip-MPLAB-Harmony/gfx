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

laLabelWidget* TitleLabel;
laButtonWidget* SloganButton;
laButtonWidget* LogoButton;
laLabelWidget* ImageTypeLabelWidget;


static void ScreenCreate_screen1(laScreen* screen);


int32_t libaria_initialize(void)
{
    laScreen* screen;

    GFX_Set(GFXF_DRAW_PIPELINE_MODE, GFX_PIPELINE_GCUGPU);
    laContext_SetStringTable(&stringTable);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_screen1);
    laContext_AddScreen(screen);

    laContext_SetActiveScreen(0);

	return 0;
}

static void ScreenCreate_screen1(laScreen* screen)
{
    laLayer* layer0;

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 480, 272);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laLayer_SetBufferCount(layer0, 1);

    laScreen_SetLayer(screen, 0, layer0);

    TitleLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)TitleLabel, 13, 4);
    laWidget_SetSize((laWidget*)TitleLabel, 455, 55);
    laWidget_SetBackgroundType((laWidget*)TitleLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)TitleLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(TitleLabel, laString_CreateFromID(string_TitleString));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)TitleLabel);

    SloganButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SloganButton, 9, 204);
    laWidget_SetSize((laWidget*)SloganButton, 464, 47);
    laWidget_SetBackgroundType((laWidget*)SloganButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SloganButton, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(SloganButton, laString_CreateFromID(string_DrawRLE));
    laButtonWidget_SetReleasedEventCallback(SloganButton, &SloganButton_ReleasedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)SloganButton);

    LogoButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)LogoButton, 82, 62);
    laWidget_SetSize((laWidget*)LogoButton, 150, 130);
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
    laWidget_SetBackgroundType((laWidget*)ImageTypeLabelWidget, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)ImageTypeLabelWidget, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(ImageTypeLabelWidget, laString_CreateFromID(string_ImageIsDirectBlit));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)ImageTypeLabelWidget);

}




