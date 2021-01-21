/*******************************************************************************
  MPLAB Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB application.

  Description:
    This file contains the source code for the MPLAB application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <string.h>
#include "app.h"
#include "hexdecoder.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define USB_ID      1
#define SDCARD_ID   2

HexDecoder CACHE_ALIGN dec;

SYS_FS_HANDLE fileHandle;
long          fileSize;
char          readChar;
uint8_t CACHE_ALIGN      writeBuffer[BUFFER_SIZE];
uint8_t CACHE_ALIGN      verificationBuffer[BUFFER_SIZE];

int32_t       usbDeviceConnected;
int32_t       sdcardDeviceConnected;

char number[12];

uint32_t i;
uint32_t recordCount;

laString str;

float percent;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA CACHE_ALIGN appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event,
                                                 void* eventData,
                                                 uintptr_t context)
{
    switch (event)
    {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
            break;
        default:
            break;
    }
    
    return(USB_HOST_EVENT_RESPONSE_NONE);
}

static void deviceConnectionStateChanged()
{
    if((usbDeviceConnected != 0 || sdcardDeviceConnected != 0) && (appData.state == APP_STATE_DONE))
    {
        laWidget_SetVisible(ErrorMsgPanel, LA_FALSE);
    }
    else
    {
        laWidget_SetVisible(ErrorMsgPanel, LA_TRUE);
    }
    
    laWidget_SetVisible((laWidget*)USBButton, usbDeviceConnected != 0 && appData.state == APP_STATE_DONE);
    laWidget_SetVisible((laWidget*)SDCardButton, sdcardDeviceConnected != 0 && appData.state == APP_STATE_DONE);
}

void APP_SYSFSEventHandler(SYS_FS_EVENT event,
                           void* eventData,
                           uintptr_t context)
{
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:
        {
            if(strcmp((const char *)eventData,"/mnt/usb") == 0)
            {
                usbDeviceConnected = 1;
                LED1_On();
            }
            else if(strcmp((const char *)eventData,"/mnt/sdcard") == 0)
            {
                sdcardDeviceConnected = 1;
                LED2_On();
            }
            
            if (appData.state != APP_STATE_ERROR)
            {
                deviceConnectionStateChanged();
            }
            
            break;
        }    
        case SYS_FS_EVENT_UNMOUNT:
        {
            if(strcmp((const char *)eventData,"/mnt/usb") == 0)
            {
                usbDeviceConnected = 0;
                LED1_Off();
            }
            else if(strcmp((const char *)eventData,"/mnt/sdcard") == 0)
            {
                sdcardDeviceConnected = 0;
                LED2_Off();
            }
            
            deviceConnectionStateChanged();
            
            break;
        }
        default:
            break;
    }
}

void APP_SDCardButtonPressed(laButtonWidget* btn)
{
    SYS_FS_CurrentDriveSet("/mnt/sdcard");

    fileHandle = SYS_FS_FileOpen("SQI.hex", (SYS_FS_FILE_OPEN_READ));
    
    if(fileHandle != SYS_FS_HANDLE_INVALID)
    {
        appData.state = APP_VALIDATE_FILE;
    }
    else
    {
        appData.state = APP_FILE_NOT_FOUND;
    }
}

void APP_USBButtonPressed(laButtonWidget* btn)
{
    SYS_FS_CurrentDriveSet("/mnt/usb");

    fileHandle = SYS_FS_FileOpen("SQI.hex", (SYS_FS_FILE_OPEN_READ));
    
    if(fileHandle != SYS_FS_HANDLE_INVALID)
    {
        appData.state = APP_VALIDATE_FILE;
    }
    else
    {
        appData.state = APP_FILE_NOT_FOUND;
    }
}

void APP_OKButtonPressed(laButtonWidget* btn)
{
    laWidget_SetVisible(InfoPanel, LA_FALSE);
    laWidget_SetVisible(SelectMediumPanel, LA_TRUE);
    laWidget_SetVisible((laWidget*)InfoOKButton, LA_FALSE);
    laWidget_SetVisible((laWidget*)USBButton, LA_TRUE);
    laWidget_SetVisible((laWidget*)SDCardButton, LA_TRUE);
    deviceConnectionStateChanged();
}

int32_t recordReadCB(HexDecoder* dec,
                     uint32_t recordNum,
                     uint8_t record[HEXDECODER_MAX_RECORD_SIZE])
{
    uint32_t i = 0;
    uint32_t size;
    char csize[4];
    
    // unexpected end of file
    if(SYS_FS_FileEOF(fileHandle) == 1)
        return -1;
    
    readChar = 0;
    
    while(readChar != ':')
    {
        SYS_FS_FileRead(fileHandle, &readChar, 1);

        if(SYS_FS_FileEOF(fileHandle) == 1)
            return -1;
    }
    
    // colon
    record[i++] = readChar;
    
    if(SYS_FS_FileEOF(fileHandle) == 1)
        return -1;
    
    // size
    SYS_FS_FileRead(fileHandle, &record[i], 8);
    i += 8;
    
    if(SYS_FS_FileEOF(fileHandle) == 1)
        return -1;
    
    csize[0] = record[1];
    csize[1] = record[2];
    csize[2] = 0;
    csize[3] = 0;
    
    size = strtoul(csize, NULL, 16) * 2;
    
    // data block
    SYS_FS_FileRead(fileHandle, &record[i], size);
    i += size;
    
    if(SYS_FS_FileEOF(fileHandle) == 1)
        return -1;
    
    // checksum
    SYS_FS_FileRead(fileHandle, &record[i], 2);
    i += 2;
    
    return 0;
}

volatile int x;
volatile uint32_t readSize;
volatile uint32_t writeAddress;

int32_t dataWriteCB(HexDecoder* dec, 
                    uint32_t address,
                    uint8_t* buffer,
                    uint32_t size)
{
    if(readSize > BUFFER_SIZE)
    {
        return -1;
    }
    
    memcpy(&appData.writeBuffer[readSize], buffer, size);
    
    readSize += size;

	return 0;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    usbDeviceConnected = 0;
    
    SYS_FS_EventHandlerSet(APP_SYSFSEventHandler, USB_ID);
    USB_HOST_EventHandlerSet(&APP_USBHostEventHandler, 0);
    USB_HOST_BusEnable(0);
    
    laButtonWidget_SetReleasedEventCallback(SDCardButton,
                                            &APP_SDCardButtonPressed);
    
    laButtonWidget_SetReleasedEventCallback(USBButton,
                                            &APP_USBButtonPressed);
    
    laButtonWidget_SetReleasedEventCallback(InfoOKButton,
                                            &APP_OKButtonPressed);
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks ( void )
{
    DRV_SST26_TRANSFER_STATUS transferStatus = DRV_SST26_TRANSFER_ERROR_UNKNOWN;

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            if (DRV_SST26_Status(DRV_SST26_INDEX) == SYS_STATUS_READY)
            {
                    appData.state = APP_INIT_WRITE_MEDIA;
            }
            break;
        }

		case APP_INIT_WRITE_MEDIA:
        {
            appData.handle = DRV_SST26_Open(DRV_SST26_INDEX, DRV_IO_INTENT_READWRITE);

            if (appData.handle != DRV_HANDLE_INVALID)
            {
				appData.state = APP_INIT_GEOMETRY;
            }

            break;
        }

        case APP_INIT_GEOMETRY:
        {
            if (DRV_SST26_GeometryGet(appData.handle, &appData.geometry) != true)
            {
                if (DRV_SST26_ChipErase(appData.handle) != true)
	            {
	                appData.state = APP_STATE_ERROR;
                    break;
	            }
                appData.state = APP_STATE_BOOT_ERASE_WAIT;
            }
            else
            {
                appData.state = APP_STATE_DONE;
            }
            break;
        }

        case APP_STATE_BOOT_ERASE_WAIT:
        {
            transferStatus = DRV_SST26_TransferStatusGet(appData.handle);

            if(transferStatus == DRV_SST26_TRANSFER_COMPLETED)
            {
				appData.state = APP_INIT_GEOMETRY;
            }
            else if (transferStatus == DRV_SST26_TRANSFER_ERROR_UNKNOWN)
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
        }

		case APP_OPEN_FILE:
		{
			fileHandle = SYS_FS_FileOpen("/SQI.hex", SYS_FS_FILE_OPEN_READ);

			if (fileHandle == SYS_FS_HANDLE_INVALID)
			{
				appData.state = APP_STATE_DONE;
			}
			else
			{
				appData.state = APP_VALIDATE_FILE;
			}
            break;
        }

		case APP_FILE_NOT_FOUND:
		{
			laWidget_SetVisible(SelectMediumPanel, LA_FALSE);
			laWidget_SetVisible(InfoPanel, LA_TRUE);

			str = laString_CreateFromID(string_FileNotFound1);
			laLabelWidget_SetText(InfoLabel1, str);

			str = laString_CreateFromID(string_FileNotFound2);
			laLabelWidget_SetText(InfoLabel2, str);

			laWidget_SetVisible((laWidget*)InfoLabel1, LA_TRUE);
			laWidget_SetVisible((laWidget*)InfoLabel2, LA_TRUE);

			appData.state = APP_STATE_DONE;
            break;
        }

		case APP_VALIDATE_FILE:
        {
			laWidget_SetVisible(SelectMediumPanel, LA_FALSE);
			laWidget_SetVisible(FlashingPanel, LA_TRUE);

			laWidget_SetVisible((laWidget*)CurrentRecordLabel, LA_FALSE);
			laWidget_SetVisible((laWidget*)OfLabel, LA_FALSE);
			laWidget_SetVisible((laWidget*)RecordsTotalLabel, LA_FALSE);

			str = laString_CreateFromID(string_RecordCount);
			laLabelWidget_SetText(FlashingLabel, str);

			fileSize = SYS_FS_FileSize(fileHandle);

			if (fileSize <= 0)
			{
				SYS_FS_FileClose(fileHandle);

				str = laString_CreateFromID(string_InvalidFile);
				laLabelWidget_SetText(InfoLabel1, str);

				laWidget_SetVisible((laWidget*)InfoLabel1, LA_TRUE);
				laWidget_SetVisible((laWidget*)InfoLabel2, LA_FALSE);

				appData.state = APP_STATE_DONE;
			}
			else
			{
				appData.state = APP_READ_RECORD_COUNT;
			}
		}

		case APP_READ_RECORD_COUNT:
		{
			recordCount = 0;

			str = laString_CreateFromID(string_RecordCount);
			laLabelWidget_SetText(FlashingLabel, str);

			for (i = 0; i < fileSize; i++)
			{
				SYS_FS_FileRead(fileHandle, &readChar, 1);

				if (readChar == ':')
					recordCount++;
			}

			if (recordCount == 0)
			{
				SYS_FS_FileClose(fileHandle);

				str = laString_CreateFromID(string_InvalidFile);
				laLabelWidget_SetText(InfoLabel1, str);

				laWidget_SetVisible((laWidget*)InfoLabel1, LA_TRUE);
				laWidget_SetVisible((laWidget*)InfoLabel2, LA_FALSE);

				appData.state = APP_STATE_DONE;
			}
			else
			{
				// erase target space
                if (DRV_SST26_ChipErase(appData.handle) != true)
	            {
	                appData.state = APP_STATE_ERROR;
                    break;
	            }

            	appData.state = APP_STATE_ERASE_WAIT;
			}
            break;
        }

        case APP_STATE_ERASE_WAIT:
        {
            transferStatus = DRV_SST26_TransferStatusGet(appData.handle);

            if(transferStatus == DRV_SST26_TRANSFER_COMPLETED)
            {
				appData.state = APP_START_DECODING;
            }
            else if (transferStatus == DRV_SST26_TRANSFER_ERROR_UNKNOWN)
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
        }

		case APP_START_DECODING:
        {
			// update the UI
			str = laString_CreateFromID(string_Flashing);
			laLabelWidget_SetText(FlashingLabel, str);

			laProgressBarWidget_SetValue(FlashingProgressBar, 0);
			laWidget_SetVisible((laWidget*)USBButton, LA_FALSE);
			laWidget_SetVisible((laWidget*)SDCardButton, LA_FALSE);
			laWidget_SetVisible((laWidget*)CurrentRecordLabel, LA_TRUE);
			laWidget_SetVisible((laWidget*)OfLabel, LA_TRUE);
			laWidget_SetVisible((laWidget*)RecordsTotalLabel, LA_TRUE);

#ifdef _STDLIB_ULTOA
			itoa(number, recordCount, 10);
#else
			itoa(recordCount, number, 10);
#endif
			str = laString_CreateFromCharBuffer(number, &Arial_sm);
			laLabelWidget_SetText(RecordsTotalLabel, str);
			laString_Destroy(&str);

            memset(writeBuffer, 0x0, BUFFER_SIZE);

			// initailize hex decoder
			HexDecoder_Initialize(&dec,
				recordCount,
				writeBuffer,
				&recordReadCB,
				&dataWriteCB);

            readSize = 0;
            writeAddress = 0;
            
#ifdef LED_OFF            
            LED_OFF();
#else 
#ifdef LED3_Off
            LED3_Off();
#endif
#endif
            
			// reset file pointer to the start
			SYS_FS_FileSeek(fileHandle, 0, SYS_FS_SEEK_SET);

			appData.state = APP_PRE_DECODE;

                break;
            }

		case APP_PRE_DECODE:
		{
#ifdef _STDLIB_ULTOA
			itoa(number, dec.currentRecord + 1, 10);
#else
			itoa(dec.currentRecord + 1, number, 10);
#endif
            
			str = laString_CreateFromCharBuffer(number, &Arial_sm);
			laLabelWidget_SetText(CurrentRecordLabel, str);
			laString_Destroy(&str);

			percent = ((float)dec.currentRecord / ((float)dec.recordCount - 1)) * 100.0f;

			laProgressBarWidget_SetValue(FlashingProgressBar, (uint32_t)percent);

			appData.state = APP_DECODE_RECORD;

            break;
        }

		case APP_DECODE_RECORD:
		{
			if (dec.currentRecord == dec.recordCount)
            {
				SYS_FS_FileClose(fileHandle);

				laWidget_SetVisible(FlashingPanel, LA_FALSE);
				laWidget_SetVisible(InfoPanel, LA_TRUE);

				str = laString_CreateFromID(string_FlashingComplete);
				laLabelWidget_SetText(InfoLabel1, str);

				laWidget_SetVisible((laWidget*)USBButton, LA_TRUE);
				laWidget_SetVisible((laWidget*)SDCardButton, LA_TRUE);
				laWidget_SetVisible((laWidget*)InfoLabel1, LA_TRUE);
				laWidget_SetVisible((laWidget*)InfoLabel2, LA_FALSE);
				laWidget_SetVisible((laWidget*)InfoOKButton, LA_TRUE);

#ifdef LED_ON            
            LED_ON();
#else 
#ifdef LED3_On
            LED3_On();
#endif
#endif
				// decode complete
				appData.state = APP_STATE_DONE;
            }
			else if (HexDecoder_Decode(&dec) == -1)
            {
				SYS_FS_FileClose(fileHandle);

				laWidget_SetVisible(FlashingPanel, LA_FALSE);
				laWidget_SetVisible(InfoPanel, LA_TRUE);

				str = laString_CreateFromID(string_UnknownError);
				laLabelWidget_SetText(InfoLabel1, str);

				laWidget_SetVisible((laWidget*)InfoLabel1, LA_TRUE);
				laWidget_SetVisible((laWidget*)InfoLabel2, LA_FALSE);

				appData.state = APP_STATE_DONE;
            }
            else
            {
                if (readSize < BUFFER_SIZE)
                {
                    appData.state = APP_PRE_DECODE;
                }
                else
                {
                    appData.write_index = 0;
                    appData.state = APP_STATE_WRITE_TO_SQI;
                }
            }
            break;
        }

        case APP_STATE_WRITE_TO_SQI:
        {
            if (DRV_SST26_PageWrite(appData.handle, (uint32_t *)&appData.writeBuffer[appData.write_index], writeAddress + appData.write_index) != true)
            {
                appData.state = APP_STATE_ERROR;
                break;
            }

            appData.state = APP_STATE_WRITE_WAIT;

            break;
        }
        
		case APP_STATE_WRITE_WAIT:
        {
            transferStatus = DRV_SST26_TransferStatusGet(appData.handle);

            if(transferStatus == DRV_SST26_TRANSFER_COMPLETED)
            {
                appData.write_index += appData.geometry.write_blockSize;
                
                if (appData.write_index < BUFFER_SIZE)
                {
                    appData.state = APP_STATE_WRITE_TO_SQI;
                }
                else
                {
                    appData.state = APP_STATE_VERIFY_READ;
                }
            }
            else if (transferStatus == DRV_SST26_TRANSFER_ERROR_UNKNOWN)
            {
                appData.state = APP_STATE_ERROR;
            }

            break;
        }

        case APP_STATE_VERIFY_READ:
        {
            if(DRV_SST26_Read(appData.handle, (uint32_t *)&verificationBuffer, readSize, writeAddress) == true)
            {
                appData.state = APP_STATE_VERIFY_WAIT;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
        }
        
		case APP_STATE_VERIFY_WAIT:
        {
			transferStatus = DRV_SST26_TransferStatusGet(appData.handle);

			if (transferStatus == DRV_SST26_TRANSFER_COMPLETED)
            {
                appData.state = APP_STATE_VERIFY_DATA;
            }
			else if (transferStatus == DRV_SST26_TRANSFER_ERROR_UNKNOWN)
            {
                appData.state = APP_STATE_ERROR;
            }

            break;
        }

        case APP_STATE_VERIFY_DATA:
        {
            if (memcmp(appData.writeBuffer, verificationBuffer, readSize) == 0)
            {
                writeAddress += readSize;

                readSize = 0;
                appData.state = APP_PRE_DECODE;
            }
            else
            {
                appData.state = APP_STATE_ERROR;
            }
            break;
        }
        
		case APP_STATE_ERROR:
        {
#ifdef LED_ON            
            LED_ON();
#else 
#ifdef RGB_LED_R_On
            RGB_LED_R_On();
#endif
#endif
            break;
        }
		case APP_STATE_DONE:
        default:
		{
            break;
		}
    }
}
