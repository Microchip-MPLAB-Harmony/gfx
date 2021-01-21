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


#include "definitions.h"
#include "gfx/emwin/gui.h"

// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include "gfx/emwin/guidrv_lin.h"

#define DRIVER_TYPE			GUIDRV_LIN_32
#define COLOR_CONVERSION	GUICC_M8888I



// *****************************************************************************
// *****************************************************************************
// Section: Layer Configuration 
// *****************************************************************************
// *****************************************************************************
/* TODO: Add any necessary Layer Configuration. 
*/

	static GFX_ColorMode colorModeConvertToGFX(void)
	{
		return GFX_COLOR_MODE_ARGB_8888;
	}

/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*   
*/

void LCD_X_Config(void) 
{


	

		GFX_Buffer buffer = NULL;
		GFX_ColorMode LCDColorMode;	
		GUI_DEVICE_CreateAndLink( DRIVER_TYPE, COLOR_CONVERSION, 0, 0);
		
		
		
		if (LCD_GetSwapXY()) 
		{
			LCD_SetSizeEx (0, 272, 480);
			LCD_SetVSizeEx(0, 272, 480);
		} 
		else 
		{
			LCD_SetSizeEx (0, 480, 272);
			LCD_SetVSizeEx(0, 480, 272);
		}
		

		//Set the Active Layer to be Layer 0
		GFX_Set(GFXF_LAYER_ACTIVE, 0);
		//Enable the active layer
        GFX_Set(GFXF_LAYER_ENABLED, GFX_TRUE);
		//Set the default layer as one for DA GLCD
		GFX_Set(GFXF_LAYER_BUFFER_COUNT, 1);

		//Translate the color mode again segger -> GFX    
		LCDColorMode = colorModeConvertToGFX();
				
		//Overwrite with the segger relevant color mode
		GFX_Set(GFXF_COLOR_MODE, LCDColorMode);
		
		//Get the buffer pointer from the gfx layer (HAL)
		//And pass the frame buffer pointer to the segger library to write to.
		GFX_Get(GFXF_LAYER_BUFFER_ADDRESS, 0, &buffer);
		LCD_SetVRAMAddrEx( 0, ( void * )buffer );
		
    
    return;
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if 
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/

int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) 
{

    return 0;

}

/*******************************************************************************
 End of File
 */

