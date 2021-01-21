/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

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

static char number[12];

uint32_t i;
uint32_t recordCount;

float percent;

static leTableString ts_NoValidMedium;
static leTableString ts_FileNotFound1;
static leTableString ts_FileNotFound2;
static leTableString ts_RecordCount;
static leTableString ts_InvalideFile;
static leTableString ts_Flashing;
static leTableString ts_UnknownError;
static leTableString ts_FlashingComplete;

static leChar counterStrBuff[12] = {0};
static leChar totalStrBuff[12] = {0};
static leFixedString counterStr;
static leFixedString totalStr;

#define FILENAME             "/SQI.hex"
#define USB_MOUNT_NAME       "/mnt/usb"
#define USB_DEV_NAME         "/dev/sda1"
#define SDCARD_MOUNT_NAME       "/mnt/myDrive1"
#define SDCARD_DEV_NAME         "/dev/mmcblka1"

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
        ErrorMsgPanel->fn->setVisible(ErrorMsgPanel, LE_FALSE);
    }
    else
    {
        ErrorMsgPanel->fn->setVisible(ErrorMsgPanel, LE_TRUE);
    }

    if (usbDeviceConnected != 0)
    {
        USBButton->fn->setVisible(USBButton, LE_TRUE);
    }
    else
    {
        USBButton->fn->setVisible(USBButton, LE_FALSE);
    }
    
    if (sdcardDeviceConnected != 0)
    {
        SDCardButton->fn->setVisible(SDCardButton, LE_TRUE);
    }
    else
    {
        SDCardButton->fn->setVisible(SDCardButton, LE_FALSE);
    }
}

void APP_SYSFSEventHandler(SYS_FS_EVENT event,
                           void* eventData,
                           uintptr_t context)
{
    switch(event)
    {
        case SYS_FS_EVENT_MOUNT:
        {
            if(strcmp((const char *)eventData, USB_MOUNT_NAME) == 0)
            {
                usbDeviceConnected = 1;
                LED1_On();
            }
            else if(strcmp((const char *)eventData, SDCARD_MOUNT_NAME) == 0)
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
            if(strcmp((const char *)eventData, USB_MOUNT_NAME) == 0)
            {
                usbDeviceConnected = 0;
                LED1_Off();
            }
            else if(strcmp((const char *)eventData, SDCARD_MOUNT_NAME) == 0)
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

void APP_SDCardButtonPressed(leButtonWidget* btn)
{
    SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME);

    fileHandle = SYS_FS_FileOpen( FILENAME, (SYS_FS_FILE_OPEN_READ));
    
    if(fileHandle != SYS_FS_HANDLE_INVALID)
    {
        appData.state = APP_VALIDATE_FILE;
    }
    else
    {
        appData.state = APP_FILE_NOT_FOUND;
    }
}

void APP_USBButtonPressed(leButtonWidget* btn)
{
    SYS_FS_CurrentDriveSet(USB_MOUNT_NAME);

    fileHandle = SYS_FS_FileOpen( FILENAME, (SYS_FS_FILE_OPEN_READ));
    
    if(fileHandle != SYS_FS_HANDLE_INVALID)
    {
        appData.state = APP_VALIDATE_FILE;
    }
    else
    {
        appData.state = APP_FILE_NOT_FOUND;
    }
}

void APP_OKButtonPressed(leButtonWidget* btn)
{
    InfoPanel->fn->setVisible(InfoPanel, LE_FALSE);
    SelectMediumPanel->fn->setVisible(SelectMediumPanel, LE_TRUE);
    InfoOKButton->fn->setVisible(InfoOKButton, LE_FALSE);
    USBButton->fn->setVisible(USBButton, LE_TRUE);
    SDCardButton->fn->setVisible(SDCardButton, LE_TRUE);

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
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


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
    
    appData.screenShown = false;    
}


void default_OnShow()
{
    SDCardButton->fn->setReleasedEventCallback(SDCardButton, &APP_SDCardButtonPressed);
    USBButton->fn->setReleasedEventCallback(USBButton, &APP_USBButtonPressed);
    InfoOKButton->fn->setReleasedEventCallback(InfoOKButton, &APP_OKButtonPressed);
    
    leFixedString_Constructor(&totalStr, totalStrBuff, 12);
    leFixedString_Constructor(&counterStr, counterStrBuff, 12);
    counterStr.fn->setFont(&counterStr, leStringTable_GetStringFont(leGetState()->stringTable,
                                                            string_RecordCount,
                                                            0));
    totalStr.fn->setFont(&totalStr, leStringTable_GetStringFont(leGetState()->stringTable,
                                                            string_RecordCount,
                                                            0));
}

void default_OnUpdate()
{
    appData.screenShown = true;
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
                appData.state = APP_STATE_BUS_ENABLE;
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

        case APP_STATE_BUS_ENABLE:
        {
            // set the event handler and enable the bus
            //SYS_FS_EventHandlerSet(&APP_SYSFSEventHandler, (uintptr_t)NULL);
            USB_HOST_EventHandlerSet(&APP_USBHostEventHandler, 0);
            USB_HOST_BusEnable(0);
            
            appData.state = APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE;
            break;
        }
		
        case APP_STATE_WAIT_FOR_BUS_ENABLE_COMPLETE:
        {
            if(USB_HOST_BusIsEnabled(0))
            {
                appData.state = APP_STATE_DONE;
            }
            break;
        }
		
		case APP_OPEN_FILE:
		{
			fileHandle = SYS_FS_FileOpen( FILENAME, SYS_FS_FILE_OPEN_READ);

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
            SelectMediumPanel->fn->setVisible(SelectMediumPanel, LE_FALSE);
            InfoPanel->fn->setVisible(InfoPanel, LE_TRUE);

            leTableString_Constructor(&ts_FileNotFound1, string_FileNotFound1); 
            leTableString_Constructor(&ts_FileNotFound2, string_FileNotFound2); 
            
            InfoLabel1->fn->setString(InfoLabel1, (leString*)&ts_FileNotFound1);
            InfoLabel2->fn->setString(InfoLabel2, (leString*)&ts_FileNotFound2);
    
            InfoLabel1->fn->setVisible(InfoLabel1, LE_TRUE);
            InfoLabel2->fn->setVisible(InfoLabel2, LE_TRUE);

			appData.state = APP_STATE_DONE;
            break;
        }

		case APP_VALIDATE_FILE:
        {
            SelectMediumPanel->fn->setVisible(SelectMediumPanel, LE_FALSE);
            FlashingPanel->fn->setVisible(FlashingPanel, LE_TRUE);

            CurrentRecordLabel->fn->setVisible(CurrentRecordLabel, LE_FALSE);
            OfLabel->fn->setVisible(OfLabel, LE_FALSE);
            RecordsTotalLabel->fn->setVisible(RecordsTotalLabel, LE_FALSE);
            
            leTableString_Constructor(&ts_RecordCount, string_RecordCount); 
            FlashingLabel->fn->setString(FlashingLabel, (leString*)&ts_RecordCount);
            FlashingLabel->fn->setVisible(FlashingLabel, LE_TRUE);

			fileSize = SYS_FS_FileSize(fileHandle);

			if (fileSize <= 0)
			{
				SYS_FS_FileClose(fileHandle);

                leTableString_Constructor(&ts_NoValidMedium, string_InvalidFile); 
                InfoLabel1->fn->setString(InfoLabel1, (leString*)&ts_NoValidMedium);

                InfoLabel1->fn->setVisible(InfoLabel1, LE_TRUE);
                InfoLabel2->fn->setVisible(InfoLabel2, LE_FALSE);

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

            leTableString_Constructor(&ts_RecordCount, string_RecordCount); 
            FlashingLabel->fn->setString(FlashingLabel, (leString*)&ts_RecordCount);

			for (i = 0; i < fileSize; i++)
			{
				SYS_FS_FileRead(fileHandle, &readChar, 1);

				if (readChar == ':')
					recordCount++;
			}

			if (recordCount == 0)
			{
				SYS_FS_FileClose(fileHandle);

                leTableString_Constructor(&ts_InvalideFile, string_InvalidFile); 
                InfoLabel1->fn->setString(InfoLabel1, (leString*)&ts_InvalideFile);

                InfoLabel1->fn->setVisible(InfoLabel1, LE_TRUE);
                InfoLabel2->fn->setVisible(InfoLabel2, LE_FALSE);

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
            leTableString_Constructor(&ts_Flashing, string_Flashing); 
            FlashingLabel->fn->setString(FlashingLabel, (leString*)&ts_Flashing);

            FlashingProgressBar->fn->setValue(FlashingProgressBar, 0);
            
            USBButton->fn->setVisible(USBButton, LE_FALSE);
            SDCardButton->fn->setVisible(SDCardButton, LE_FALSE);
            CurrentRecordLabel->fn->setVisible(CurrentRecordLabel, LE_TRUE);
            OfLabel->fn->setVisible(OfLabel, LE_TRUE);
            RecordsTotalLabel->fn->setVisible(RecordsTotalLabel, LE_TRUE);

#ifdef _STDLIB_ULTOA
			itoa(number, recordCount, 10);
#else
			itoa(recordCount, number, 10);
#endif
            
            counterStr.fn->setFromCStr(&totalStr, number);
            RecordsTotalLabel->fn->setString(RecordsTotalLabel, (leString*)&totalStr);

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
            
            counterStr.fn->setFromCStr(&counterStr, number);
            CurrentRecordLabel->fn->setString(CurrentRecordLabel, (leString*)&counterStr);

			percent = ((float)dec.currentRecord / ((float)dec.recordCount - 1)) * 100.0f;

            FlashingProgressBar->fn->setValue(FlashingProgressBar, (uint32_t)percent);

			appData.state = APP_DECODE_RECORD;

            break;
        }

		case APP_DECODE_RECORD:
		{
			if (dec.currentRecord == dec.recordCount)
            {
				SYS_FS_FileClose(fileHandle);

                FlashingPanel->fn->setVisible(FlashingPanel, LE_FALSE);
                InfoPanel->fn->setVisible(InfoPanel, LE_TRUE);

                leTableString_Constructor(&ts_FlashingComplete, string_FlashingComplete); 
                InfoLabel1->fn->setString(InfoLabel1, (leString*)&ts_FlashingComplete);

                USBButton->fn->setVisible(USBButton, LE_TRUE);
                SDCardButton->fn->setVisible(SDCardButton, LE_TRUE);
                InfoLabel1->fn->setVisible(InfoLabel1, LE_TRUE);
                InfoLabel2->fn->setVisible(InfoLabel2, LE_FALSE);
                InfoOKButton->fn->setVisible(InfoOKButton, LE_TRUE);

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

                FlashingPanel->fn->setVisible(FlashingPanel, LE_FALSE);
                InfoPanel->fn->setVisible(InfoPanel, LE_TRUE);

                leTableString_Constructor(&ts_UnknownError, string_UnknownError); 
                InfoLabel1->fn->setString(InfoLabel1, (leString*)&ts_UnknownError);

                InfoLabel1->fn->setVisible(InfoLabel1, LE_TRUE);
                InfoLabel2->fn->setVisible(InfoLabel2, LE_FALSE);

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
        {
            if( SYS_FS_Mount( USB_DEV_NAME, USB_MOUNT_NAME, FAT, 0, NULL) == SYS_FS_RES_SUCCESS )
            {
                usbDeviceConnected = 1;
                LED1_On();
            }
            
            if( SYS_FS_Mount( SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 1, NULL) == SYS_FS_RES_SUCCESS )
            {
                sdcardDeviceConnected = 1;
                LED2_On();
            }

            deviceConnectionStateChanged();
            break;
        }
        
        default:
		{
            break;
		}
    }
}


/*******************************************************************************
 End of File
 */
