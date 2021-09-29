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
/********************************************************************************
  LCF0300633GGU00 Display Support Source File

  Company:
    Microchip Technology Inc.

  File Name:
    disp_gfx_LCF0300633GGU00.c.ftl

  Summary:
    Source code to support LCF0300633GGU00 display initialization

  Description:
    This file contains the source code for supporting the LCF0300633GGU00 Display.
	
*******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Local Static Functions
// *****************************************************************************
// *****************************************************************************

static void DelayX100us(unsigned int i)
{
    unsigned int loop1, loop2;
    for(loop1=0;loop1<i;loop1++)
    {
        for(loop2=0;loop2<2500;loop2++) { _nop(); }
    }      
}

static void DelayX1ms(unsigned int i)
{
    unsigned int loop1, loop2, loop3;
    for(loop1=0;loop1<i;loop1++)
    {
        for(loop2=0;loop2<2000;loop2++) { }
            for(loop3=0;loop3<20000;loop3++) { _nop(); }
    }   
}

static void SPI_Start(void)
{
    unsigned int i;

    CSX_Clear();
    for(i=0;i<10000;i++) { _nop(); }    
}

static void SPI_Stop(void)
{
    unsigned int i;

    CSX_Set();
    for(i=0;i<10000;i++) { _nop(); }        
    
}

static void SPI_3W_SET_CMD(unsigned char regData)
{
    unsigned char SPICount;
    unsigned char SPIData;

    SCK_Clear();
    
    SPIData = regData;
    
    SDO_Clear();  // for command mode
    DelayX100us(1);
    SCK_Set();
    DelayX100us(1);
    SCK_Clear();
    DelayX100us(1);
    
    for (SPICount = 0; SPICount < 8; SPICount++)
    {
        if (SPIData & 0x80)
            SDO_Set();
        else
            SDO_Clear();

        DelayX100us(1);
        SCK_Set();
        DelayX100us(1);
        SCK_Clear();
        DelayX100us(1);
        SPIData <<= 1;
    }          
    //SDO_Clear();
}

static void SPI_3W_SET_PAs(unsigned char regData)
{
    unsigned char SPICount;
    unsigned char SPIData;

    SCK_Clear();
    
    SPIData = regData;
    
    SDO_Set();  // for command mode
    DelayX100us(1);
    SCK_Set();
    DelayX100us(1);
    SCK_Clear();
    DelayX100us(1);
    
    for (SPICount = 0; SPICount < 8; SPICount++)
    {
        if (SPIData & 0x80)
            SDO_Set();
        else
            SDO_Clear();

        DelayX100us(1);
        SCK_Set();
        DelayX100us(1);
        SCK_Clear();
        DelayX100us(1);
        SPIData <<= 1;
    }          
    //SDO_Clear();
}

void DISP_LCF0300633GGU00_Initialize(void)
{
    BACKLIGHT_TYPE_1_Clear();
    
    CSX_Set();
    RESX_Clear();
    DelayX1ms(100);

    RESX_Set();
    
    DelayX1ms(200);
       
    SPI_Start();
    SPI_3W_SET_CMD(0xB9);
    SPI_3W_SET_PAs(0xFF);
    SPI_3W_SET_PAs(0x83);
    SPI_3W_SET_PAs(0x63);
    SPI_Stop();
    DelayX1ms(1);
    SPI_Start();
    SPI_3W_SET_CMD(0x11);
    SPI_Stop();
    DelayX1ms(120);
    SPI_Start();
    SPI_3W_SET_CMD(0xB9);
    SPI_3W_SET_PAs(0xFF);
    SPI_3W_SET_PAs(0x83);
    SPI_3W_SET_PAs(0x63);
    SPI_Stop();
    DelayX1ms (1);
    SPI_Start();
    SPI_3W_SET_CMD(0xB1);
    SPI_3W_SET_PAs(0x78);//
    SPI_3W_SET_PAs(0x34);//
    SPI_3W_SET_PAs(0x07);//BT=7h 
    SPI_3W_SET_PAs(0x33);//
    SPI_3W_SET_PAs(0x02);//
    SPI_3W_SET_PAs(0x13);//
    SPI_3W_SET_PAs(0x10);//
    SPI_3W_SET_PAs(0x10);//
    SPI_3W_SET_PAs(0x2C);//
    SPI_3W_SET_PAs(0x34);//
    SPI_3W_SET_PAs(0x22);//
    SPI_3W_SET_PAs(0x22);//
    SPI_Stop();
    DelayX1ms (1);
    SPI_Start();
    SPI_3W_SET_CMD(0x3A);
    SPI_3W_SET_PAs(0x70);
    SPI_Stop();
    SPI_Start();
    SPI_3W_SET_CMD(0xB3);
    SPI_3W_SET_PAs(0x01);
    SPI_Stop();
    
    SPI_Start();
    SPI_3W_SET_CMD(0xB4);
    SPI_3W_SET_PAs(0x00);
    SPI_3W_SET_PAs(0x12);
    SPI_3W_SET_PAs(0x72);
    SPI_3W_SET_PAs(0x12);
    SPI_3W_SET_PAs(0x06);
    SPI_3W_SET_PAs(0x03);
    SPI_3W_SET_PAs(0x54);
    SPI_3W_SET_PAs(0x03);
    SPI_3W_SET_PAs(0x4E);
    SPI_Stop();
    SPI_Start();
    SPI_3W_SET_CMD(0xB6);
    SPI_3W_SET_PAs(0x36);
    SPI_Stop();
    SPI_Start();
    SPI_3W_SET_CMD(0xCC);
    SPI_3W_SET_PAs(0x07);
    SPI_Stop();
    DelayX1ms(1);
    SPI_Start();
    SPI_3W_SET_CMD(0xE0);
    SPI_3W_SET_PAs(0x00);
    SPI_3W_SET_PAs(0x00);
    SPI_3W_SET_PAs(0x00);
    SPI_3W_SET_PAs(0x1F);
    SPI_3W_SET_PAs(0x3E);
    SPI_3W_SET_PAs(0x3F);
    SPI_3W_SET_PAs(0x05);
    SPI_3W_SET_PAs(0x0B);
    SPI_3W_SET_PAs(0x0D);
    SPI_3W_SET_PAs(0xCF);
    SPI_3W_SET_PAs(0x10);
    SPI_3W_SET_PAs(0x90);
    SPI_3W_SET_PAs(0xD1);
    SPI_3W_SET_PAs(0x5C);
    SPI_3W_SET_PAs(0x1F);
    SPI_3W_SET_PAs(0x00);
    SPI_3W_SET_PAs(0x00);
    SPI_3W_SET_PAs(0x00);
    SPI_3W_SET_PAs(0x1F);
    SPI_3W_SET_PAs(0x3E);
    SPI_3W_SET_PAs(0x3F);
    SPI_3W_SET_PAs(0x05);
    SPI_3W_SET_PAs(0x0B);
    SPI_3W_SET_PAs(0x0D);   
    SPI_3W_SET_PAs(0xCF);
    SPI_3W_SET_PAs(0x10);
    SPI_3W_SET_PAs(0x90);
    SPI_3W_SET_PAs(0xD1);
    SPI_3W_SET_PAs(0x5C);
    SPI_3W_SET_PAs(0x1F);
    SPI_Stop();
    DelayX1ms(1);
    SPI_Start();
    SPI_3W_SET_CMD(0x29);
    SPI_Stop();    
    BACKLIGHT_TYPE_1_Set();
}


void DISP_LCF0300633GGU00_Reset(void)
{
    unsigned int i;

    RESX_Set();
    for(i=0;i<50000;i++) { _nop(); }
    RESX_Clear();
    for(i=0;i<100000;i++) { _nop(); }
    for(i=0;i<100000;i++) { _nop(); }
    RESX_Set();
    for(i=0;i<50000;i++) { _nop(); }
}
    
/*******************************************************************************
 End of File
*/
