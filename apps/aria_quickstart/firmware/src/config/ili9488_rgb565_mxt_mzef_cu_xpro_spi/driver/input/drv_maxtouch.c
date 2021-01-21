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
 Touch controller MAXTOUCH driver file

  File Name:
    drv_maxtouch.c

  Summary:
    Touch controller MAXTOUCH driver interface file.

  Description:
    This file consist of touch controller MAXTOUCH driver interfaces. It
    implements the driver interfaces which read the touch input data from
    MAXTOUCH through I2C bus.
 ******************************************************************************/


#include "definitions.h"
#include "driver/i2c/drv_i2c.h"
#include "driver/input/drv_maxtouch.h"
#include "system/input/sys_input.h"
#include "system/time/sys_time.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Firmware config definitions */
#define MXT_CFG_MAGIC		 "OBP_RAW V1"

/* Registers */
#define MXT_OBJECT_START        0x07
#define MXT_OBJECT_SIZE         6
#define MXT_INFO_CHECKSUM_SIZE	3
#define MXT_MAX_BLOCK_WRITE     253

#define MXT_DATA_BUFFER_SIZE    256
#define MXT_MSG_BUFFER_SIZE     256

//#define DEBUG_ENABLE

#define I2C_FRAME_SIZE             32
#define I2C_READ_ID_FRAME_SIZE     sizeof(MAXTOUCH_Message)

// defines MAXTOUCH command register I2C write address
#define I2C_MASTER_WRITE_ID        0x4A 

// defines MAXTOUCH command register I2C read address
#define I2C_MASTER_READ_ID         0x4A

// multitouch screen resolution configuration registers for T100
#define T100_XRANGE                0xD
#define T100_YRANGE                0x18


#define DEFAULT_XRES               479
#define DEFAULT_YRES               271

#define DRV_MAXTOUCH_NUM_QUEUE      2

/* MXT_GEN_COMMAND_T6 field */
#define MXT_COMMAND_RESET       0
#define MXT_COMMAND_BACKUPNV	1
#define MXT_COMMAND_CALIBRATE	2
#define MXT_COMMAND_REPORTALL	3
#define MXT_COMMAND_DIAGNOSTIC	5

/* Define for MXT_GEN_COMMAND_T6 */
#define MXT_BOOT_VALUE          0xa5
#define MXT_RESET_VALUE         0x01
#define MXT_BACKUP_VALUE        0x55

/* Delay times */
#define MXT_BACKUP_TIME             50      /* msec */
#define MXT_RESET_GPIO_TIME         20      /* msec */
#define MXT_RESET_INVALID_CHG       100     /* msec */
#define MXT_RESET_TIME              200     /* msec */
#define MXT_RESET_TIMEOUT           3000	/* msec */
#define MXT_CRC_TIMEOUT             1000	/* msec */
#define MXT_FW_RESET_TIME           3000	/* msec */
#define MXT_FW_CHG_TIMEOUT          300     /* msec */
#define MXT_POWER_CFG_RUN           0
#define MXT_POWER_CFG_DEEPSLEEP		1
#define MXT_PROGRAM_DELAY           500     /*msec */

/* MXT_GEN_MESSAGE_T5 object */
#define MXT_RPTID_NOMSG             0xff

/* T100 Multiple Touch Touchscreen */
#define MXT_T100_CTRL		0
#define MXT_T100_CFG1		1
#define MXT_T100_TCHAUX		3
#define MXT_T100_NUMTCH     6
#define MXT_T100_XSIZE		9
#define MXT_T100_XRANGE		13
#define MXT_T100_YSIZE		20
#define MXT_T100_YRANGE		24

#define MXT_T100_CFG_SWITCHXY	(1 << 5) //BIT(5)
#define MXT_T100_CFG_INVERTY	(1 << 6) //BIT(6)
#define MXT_T100_CFG_INVERTX	(1 << 7) //BIT(7)

#define MXT_T100_TCHAUX_VECT	(1 << 0) //BIT(0)
#define MXT_T100_TCHAUX_AMPL	(1 << 1) //BIT(1)
#define MXT_T100_TCHAUX_AREA	(1 << 2) //BIT(2)

#define MXT_T100_DETECT         (1 << 7) //BIT(7)
#define MXT_T100_TYPE_MASK      0x70

#define KEY_RESERVED		0
#define BTN_TOOL_PEN		0x140
#define BTN_TOOL_RUBBER		0x141
#define BTN_TOOL_BRUSH		0x142
#define BTN_TOOL_PENCIL		0x143
#define BTN_TOOL_AIRBRUSH	0x144
#define BTN_TOOL_FINGER		0x145

#define ABS_MT_SLOT		    0x2f	/* MT slot being modified */
#define ABS_MT_TOUCH_MAJOR	0x30	/* Major axis of touching ellipse */
#define ABS_MT_TOUCH_MINOR	0x31	/* Minor axis (omit if circular) */
#define ABS_MT_WIDTH_MAJOR	0x32	/* Major axis of approaching ellipse */
#define ABS_MT_WIDTH_MINOR	0x33	/* Minor axis (omit if circular) */
#define ABS_MT_ORIENTATION	0x34	/* Ellipse orientation */
#define ABS_MT_POSITION_X	0x35	/* Center X touch position */
#define ABS_MT_POSITION_Y	0x36	/* Center Y touch position */
#define ABS_MT_TOOL_TYPE	0x37	/* Type of touching device */
#define ABS_MT_BLOB_ID		0x38	/* Group a set of packets as a blob */
#define ABS_MT_TRACKING_ID	0x39	/* Unique ID of initiated contact */
#define ABS_MT_PRESSURE		0x3a	/* Pressure on contact area */
#define ABS_MT_DISTANCE		0x3b	/* Contact hover distance */
#define ABS_MT_TOOL_X		0x3c	/* Center X tool position */
#define ABS_MT_TOOL_Y		0x3d	/* Center Y tool position */

#define MT_TOOL_FINGER		0
#define MT_TOOL_PEN         1
#define MT_TOOL_PALM		2
#define MT_TOOL_MAX         2


enum t100_type {
	MXT_T100_TYPE_FINGER            = 1,
	MXT_T100_TYPE_PASSIVE_STYLUS	= 2,
	MXT_T100_TYPE_HOVERING_FINGER	= 4,
	MXT_T100_TYPE_GLOVE             = 5,
	MXT_T100_TYPE_LARGE_TOUCH       = 6,
};

#define MXT_DISTANCE_ACTIVE_TOUCH	0
#define MXT_DISTANCE_HOVERING		1

#define MXT_TOUCH_MAJOR_DEFAULT		1
#define MXT_PRESSURE_DEFAULT		1

/* Object types */
#define MXT_DEBUG_DIAGNOSTIC_T37        37
#define MXT_GEN_MESSAGE_T5              5
#define MXT_GEN_COMMAND_T6              6
#define MXT_GEN_POWER_T7                7
#define MXT_GEN_ACQUIRE_T8              8
#define MXT_GEN_DATASOURCE_T53          53
#define MXT_TOUCH_MULTI_T9              9
#define MXT_TOUCH_KEYARRAY_T15          15
#define MXT_TOUCH_PROXIMITY_T23         23
#define MXT_TOUCH_PROXKEY_T52           52
#define MXT_PROCI_GRIPFACE_T20          20
#define MXT_PROCG_NOISE_T22             22
#define MXT_PROCI_ONETOUCH_T24          24
#define MXT_PROCI_TWOTOUCH_T27          27
#define MXT_PROCI_GRIP_T40              40
#define MXT_PROCI_PALM_T41              41
#define MXT_PROCI_TOUCHSUPPRESSION_T42	42
#define MXT_PROCI_STYLUS_T47            47
#define MXT_PROCG_NOISESUPPRESSION_T48	48
#define MXT_SPT_COMMSCONFIG_T18         18
#define MXT_SPT_GPIOPWM_T19             19
#define MXT_SPT_SELFTEST_T25            25
#define MXT_SPT_CTECONFIG_T28           28
#define MXT_SPT_USERDATA_T38            38
#define MXT_SPT_DIGITIZER_T43           43
#define MXT_SPT_MESSAGECOUNT_T44        44
#define MXT_SPT_CTECONFIG_T46           46
#define MXT_TOUCH_MULTITOUCHSCREEN_T100 100
#define MXT_SPT_DYNAMICCONFIGURATIONCONTAINER_T71 71

struct mxt_info {
	uint8_t family_id;
	uint8_t variant_id;
	uint8_t version;
	uint8_t build;
	uint8_t matrix_xsize;
	uint8_t matrix_ysize;
	uint8_t object_num;
};

struct mxt_object {
	uint8_t type;
	uint16_t start_address;
	uint8_t size_minus_one;
	uint8_t instances_minus_one;
	uint8_t num_report_ids;
} __attribute__((packed));

struct mxt_dbg {
	uint16_t t37_address;
	uint16_t diag_cmd_address;
	struct t37_debug *t37_buf;
	unsigned int t37_pages;
	unsigned int t37_nodes;
	int input;
};

enum v4l_dbg_inputs {
	MXT_V4L_INPUT_DELTAS,
	MXT_V4L_INPUT_REFS,
	MXT_V4L_INPUT_MAX,
};

enum mxt_suspend_mode {
	MXT_SUSPEND_DEEP_SLEEP	= 0,
	MXT_SUSPEND_T9_CTRL	= 1,
};

struct i2c_client {
  unsigned short flags;
  unsigned short addr;
  DRV_HANDLE drvI2CHandle;
};

/* MXT_GEN_POWER_T7 field */
struct t7_config {
	uint8_t idle;
	uint8_t active;
} __attribute__((packed));

/* Each client has this additional data */
struct mxt_data {
	struct i2c_client *client;
//	struct input_dev *input_dev;
	struct mxt_object *object_table;
	struct mxt_info info;
	uint8_t raw_info_block[MXT_DATA_BUFFER_SIZE];
	unsigned int max_x;
	unsigned int max_y;
	bool invertx;
	bool inverty;
	bool xy_switch;
	uint8_t xsize;
	uint8_t ysize;
	bool in_bootloader;
	uint16_t mem_size;
	uint8_t t100_aux_ampl;
	uint8_t t100_aux_area;
	uint8_t t100_aux_vect;
	uint8_t max_reportid;
	uint32_t config_crc;
	uint32_t info_crc;
	uint8_t bootloader_addr;
	uint8_t msg_buf[MXT_MSG_BUFFER_SIZE];
	uint8_t t6_status;
	bool update_input;
	uint8_t last_message_count;
	uint8_t num_touchids;
	uint8_t multitouch;
	struct t7_config t7_cfg;
	struct mxt_dbg dbg;
	struct gpio_desc *reset_gpio;

	/* Cached parameters from object table */
	uint16_t T5_address;
	uint8_t T5_msg_size;
	uint8_t T6_reportid;
	uint16_t T6_address;
	uint16_t T7_address;
	uint8_t T9_reportid_min;
	uint8_t T9_reportid_max;
	uint8_t T19_reportid;
	uint16_t T44_address;
    uint16_t T71_address;
	uint8_t T100_reportid_min;
	uint8_t T100_reportid_max;
    uint16_t T100_address;

//	/* for fw update in bootloader */
//	struct completion bl_completion;
//
//	/* for reset handling */
//	struct completion reset_completion;
//
//	/* for config update handling */
//	struct completion crc_completion;

	uint32_t *t19_keymap;
	unsigned int t19_num_keys;

	enum mxt_suspend_mode suspend_mode;

    bool inUse;
        /* Callback for I2C Driver Open call */
    DRV_HANDLE (*drvOpen)(const SYS_MODULE_INDEX index,
                          const DRV_IO_INTENT intent);
    
    uint16_t xRes;
    uint16_t yRes;

    SYS_STATUS status;
    bool isExclusive;
    
    int cfg_start_ofs;
    uint8_t config_mem[2000];
    size_t config_mem_size;
    int num_left;
    int count;
    uint32_t progress;
};

// this is the maximum message size the message processor may issue
// hardcoded for simplicity but this value may change between part versions
#define MESSAGE_DATA_SIZE 8

typedef struct MAXTOUCH_Message_t
{
    uint8_t reportID;
    uint8_t data[MESSAGE_DATA_SIZE];
#ifdef MESSAGE_CHECKSUM_ENABLED
    uint8_t checksum;
#endif
} MAXTOUCH_Message;

// struct describing an MAXTOUCH touch event
typedef struct MAXTOUCH_TouchEvent_t
{
    uint8_t tchstatus;
    uint8_t x_lo;
    uint8_t x_hi;
    uint8_t y_lo;
    uint8_t y_hi;
    uint8_t aux[4];
} MAXTOUCH_TouchEvent;

// MAXTOUCH Driver Module Index Count
typedef enum {

    ID_1 = 0,
    NUMBER_OF_MODULES
            
} DRV_MODULE_ID;

typedef enum
{
    /* Driver Initialize state */
    DEVICE_STATE_OPEN = 0,
            
    DEVICE_STATE_REQUEST_ID_BLOCK,
            
    /* reset the address */
    DEVICE_STATE_READ_ID_BLOCK,        
            
    /* Request object block */
    DEVICE_STATE_REQUEST_OBJECT_TABLE,
            
    /* Read object block */
    DEVICE_STATE_READ_OBJECT_TABLE,
            
    /* Hanld addresses and checksum */
    DEVICE_STATE_READ_CRC_VALUE,
            
    /* Write Resolution configs*/        
    DEVICE_STATE_CONFIGURE_RESOLUTION,
    DEVICE_STATE_WRITE_T100_XRANGE,
    
    DEVICE_STATE_WRITE_T100_YRANGE,
            
    /* Driver ready state */
    DEVICE_STATE_READY,   
            
    /* read a specified object from the device */        
    DEVICE_STATE_READ_MESSAGE_OBJECT,
            
    DEVICE_STATE_HANDLE_MESSAGE_OBJECT,
            
    DEVICE_STATE_WAIT,
            
    DEVICE_STATE_CONFIG_PARSE,
            
    /* In error state */        
    DEVICE_STATE_ERROR, 
            
    DEVICE_STATE_MOUNT_FS,
            
    DEVICE_STATE_WRITE_RESET,
            
    DEVICE_STATE_READ_RESET,
    DEVICE_STATE_CONFIG_LOAD,
    DEVICE_STATE_CONFIG_SAVE,
    DEVICE_STATE_REQUEST_REMAINING_MESSAGES,
    DEVICE_STATE_READ_REMAINING_MESSAGES,
    DEVICE_STATE_READ_T5_MESSAGE,
    DEVICE_STATE_READ_T44_MESSAGE,
    DEVICE_STATE_REQUEST_T5_MESSAGE,
    DEVICE_STATE_REQUEST_T44_MESSAGE,
    DEVICE_STATE_READ_T44_T5_MESSAGE,
    DEVICE_STATE_T44_T5_MESSAGE_REQUEST,
    DEVICE_STATE_T44_T5_MESSAGE_READ,
    DEVICE_STATE_PROCESS_T44_T5_MESSAGE,
    DEVICE_STATE_PROCESS_T5_MESSAGE,

    DEVICE_STATE_CONFIG_SOFTRESET
            
} DEVICE_STATE;

typedef enum
{
    /* Application's API event states. */
    APP_DRV_MAXTOUCH_None=0,
    APP_DRV_MAXTOUCH_ConfigLoad,
    APP_DRV_MAXTOUCH_ConfigParse,
    APP_DRV_MAXTOUCH_ConfigSave,
    APP_DRV_MAXTOUCH_ResetCallback,
    /* TODO: Define states used by the application state machine. */

} API_EVENTS;

typedef enum
{
    CONFIG_STATE_MXT_COMMAND_BACKUPNV_WRITE,
    CONFIG_STATE_MXT_COMMAND_BACKUPNV_READ,
    CONFIG_STATE_MXT_COMMAND_BACKUPNV_HANDLE,
    CONFIG_STATE_MXT_COMMAND_RESET_WRITE,
    CONFIG_STATE_MXT_COMMAND_RESET_READ,
    CONFIG_STATE_MXT_COMMAND_RESET_HANDLE,
} CONFIG_STATE;

// defines an MAXTOUCH driver object instance
struct DEVICE_OBJECT
{
    /* The status of the driver */
    SYS_STATUS status;
    
    /* Driver state */
    DEVICE_STATE deviceState;
    
    API_EVENTS apiEvent;
    
    /* Config State*/
    CONFIG_STATE configState;

    /* Save the index of the driver. Important to know this
    as we are using reference based accessing */
    SYS_MODULE_INDEX drvIndex;

    /* Flag to indicate instance in use  */
    bool inUse;

    /* Flag to indicate module used in exclusive access mode */
    bool isExclusive;

    /* Callback for I2C Driver Open call */
    DRV_HANDLE (*drvOpen)(const SYS_MODULE_INDEX index,
                          const DRV_IO_INTENT intent);

    /* Touch input interrupt source */
    INT_SOURCE interruptSource;
    
    /* interrupt pin for driver instance */
//    PORTS_BIT_POS interruptPin;
//    
//    /* port channel for interrupt instance */
//    PORTS_CHANNEL interruptChannel;
//       
//    /* reset pin for driver instance */
//    PORTS_BIT_POS resetPin;
//    
//    /* port channel for reset pin */
//    PORTS_CHANNEL resetChannel;

    /* I2C bus driver handle */
    DRV_HANDLE drvI2CHandle;
    
    MAXTOUCH_Message mxtMsg;
    
    uint8_t reportIDBase;
    
    uint16_t xRes;
    uint16_t yRes;
    
    uint32_t readRequest;
    
    /* I2C Buffer handle */
    DRV_I2C_TRANSFER_HANDLE hInformationBlockRequest;
    DRV_I2C_TRANSFER_HANDLE hInformationBlockRead;
    DRV_I2C_TRANSFER_HANDLE hObjectBlockRequest;
    DRV_I2C_TRANSFER_HANDLE hObjectBlockRead;
    DRV_I2C_TRANSFER_HANDLE hXRangeWrite;
    DRV_I2C_TRANSFER_HANDLE hYRangeWrite;
    DRV_I2C_TRANSFER_HANDLE hMessageT5ObjWrite;
    DRV_I2C_TRANSFER_HANDLE hMessageT5ObjRead;
    DRV_I2C_TRANSFER_HANDLE hConfigUpload;
    DRV_I2C_TRANSFER_HANDLE hResetWrite;
    DRV_I2C_TRANSFER_HANDLE hResetRead;
    DRV_I2C_TRANSFER_HANDLE hMessageT44ObjWrite;
    DRV_I2C_TRANSFER_HANDLE hMessageT44ObjRead;
    DRV_I2C_TRANSFER_HANDLE hMessageT44T5ObjWrite;
    DRV_I2C_TRANSFER_HANDLE hMessageT44T5ObjRead;
    DRV_I2C_TRANSFER_HANDLE hMessageObjWrite;
    DRV_I2C_TRANSFER_HANDLE hMessageObjRead;
    
    DRV_I2C_TRANSFER_HANDLE hBackupNVWrite;
    DRV_I2C_TRANSFER_HANDLE hBackupNVRead;
     
    DRV_MAXTOUCH_Firmware * firmware;
    struct mxt_data data;
    DRV_MAXTOUCH_ConfigProgress_FnPtr callback;
    DRV_MAXTOUCH_ConfigFileReader_FnPtr reader;
    DRV_MAXTOUCH_ConfigFileEof_FnPtr eof;
};

//uint8_t CONFIG_MEM[0x800];
static uint32_t progress[9] = { 12, 24, 36, 48, 60, 72, 94, 100, 0 };
static uint8_t buf[255];

// *****************************************************************************

/* send reset command to device */
//static void _SendResetCommand(struct DEVICE_OBJECT* pDrvObject);

/* Send request to read message processor object */
static void _MessageObjectRead(struct DEVICE_OBJECT* pDrvObject);

/* Send request to read message processor object */
//static void _RegRead(struct DEVICE_OBJECT* pDrvObject, uint8_t reg);
static void _RegWrite8(struct DEVICE_OBJECT *pDrvObject, uint8_t reg, uint8_t val, DRV_I2C_TRANSFER_HANDLE * transferHandle);
static void _RegWrite16(struct DEVICE_OBJECT* pDrvObject, uint8_t reg, uint16_t val, DRV_I2C_TRANSFER_HANDLE * transferHandle);
static void _RegRead(struct DEVICE_OBJECT *pDrvObject, void* val, size_t size, DRV_I2C_TRANSFER_HANDLE * transferHandle);

bool MXT_INTERRUPT_PIN_VALUE_GET(void)
{
#ifndef BSP_MAXTOUCH_CHG_Get
#error "BSP_MAXTOUCH_CHG_Get is not defined. Please use Pin Settings Tab in MHC to define the Touch Interrupt pin with the name 'BSP_MAXTOUCH_CHG' as GPIO_IN"   
#else
return(BSP_MAXTOUCH_CHG_Get());
#endif
}


static void _handleTouchMessage(uint8_t touchID, MAXTOUCH_TouchEvent* tchEvt);
static bool mxt_init_t7_power_cfg(struct DEVICE_OBJECT* pDeviceObject);
static bool mxt_load_xcfg_file(struct DEVICE_OBJECT* pDeviceObject, const char *filename);
static bool mxt_load_raw_file(struct DEVICE_OBJECT* pDeviceObject, char * cfg);
static bool mxt_load_raw_flash(struct DEVICE_OBJECT* pDeviceObject, char * cfg);
static bool mxt_soft_reset(struct DEVICE_OBJECT* pDeviceObject);
static bool mxt_configure_objects(struct DEVICE_OBJECT* pDeviceObject, DRV_MAXTOUCH_Firmware *firmware);
static uint32_t mxt_calculate_crc(uint8_t *base, long start_off, long end_off);
static bool mxt_parse_object_table(struct DEVICE_OBJECT* pDeviceObject,
				  struct mxt_object *object_table);
static int mxt_upload_cfg_mem(struct DEVICE_OBJECT* pDeviceObject);
static bool mxt_proc_message(struct mxt_data *data, uint8_t *message);
static bool mxt_load_xcfg_file(struct DEVICE_OBJECT *pDrvObject, const char *filename);


#ifdef MESSAGE_CHECKSUM_ENABLED
static uint8_t checksumMessage(uint8_t* msg);
#endif

/* MAXTOUCH Driver instance object */
struct DEVICE_OBJECT sMAXTOUCHDriverInstances[DRV_MAXTOUCH_INDEX_COUNT];

static SYS_TIME_HANDLE resetTimer;

static void _mxt_DelayMS(int ms)
{
	SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

	if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
	return;
	
	while (SYS_TIME_DelayIsComplete(timer) == false);
} 

static void resetTimer_Callback ( uintptr_t context )
{
    struct DEVICE_OBJECT* obj = (struct DEVICE_OBJECT*) context;
    
    switch ( obj->apiEvent )
    {
        case APP_DRV_MAXTOUCH_ConfigLoad:
            obj->deviceState = DEVICE_STATE_CONFIG_LOAD;
            break;
            
        case APP_DRV_MAXTOUCH_ConfigSave:
            obj->deviceState = DEVICE_STATE_CONFIG_SAVE;
            obj->configState = CONFIG_STATE_MXT_COMMAND_BACKUPNV_WRITE;
            break;
            
        case APP_DRV_MAXTOUCH_ResetCallback:
            obj->deviceState = DEVICE_STATE_REQUEST_ID_BLOCK;
            break;
            
        case APP_DRV_MAXTOUCH_ConfigParse:
            obj->deviceState = DEVICE_STATE_CONFIG_PARSE;
            break;
            
        default:
            break;
    }
    SYS_TIME_TimerDestroy(resetTimer);
}

void DRV_MAXTOUCH_I2CEventHandler ( DRV_I2C_TRANSFER_EVENT  event,
                           DRV_I2C_TRANSFER_HANDLE transferHandle, 
                           uintptr_t               context )
{    
    struct DEVICE_OBJECT* obj = (struct DEVICE_OBJECT*) context;
    
    /* Checks for valid buffer handle */
    if( transferHandle == DRV_I2C_TRANSFER_HANDLE_INVALID )
    {
        return;
    }
    
    if( transferHandle == obj->hInformationBlockRequest &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE )
    {
        obj->deviceState = DEVICE_STATE_READ_ID_BLOCK;
    }
    
    if( transferHandle == obj->hInformationBlockRead &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE )
    {
        obj->deviceState = DEVICE_STATE_REQUEST_OBJECT_TABLE;
    }
        
    if( transferHandle == obj->hObjectBlockRequest &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE )
    {
        obj->deviceState = DEVICE_STATE_READ_OBJECT_TABLE;
    }
        
    if( transferHandle == obj->hObjectBlockRead &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE )
    {
        obj->deviceState = DEVICE_STATE_READ_CRC_VALUE;
    }
    
    if( transferHandle == obj->hXRangeWrite &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE )
    {
        obj->deviceState = DEVICE_STATE_WRITE_T100_XRANGE;
    }
  
    if( transferHandle == obj->hYRangeWrite &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE )
    {
        obj->deviceState = DEVICE_STATE_WRITE_T100_YRANGE;
    }

    if( transferHandle == obj->hMessageT5ObjWrite &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_READ_T5_MESSAGE;
    }
    
    if( transferHandle == obj->hMessageT5ObjRead &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_PROCESS_T5_MESSAGE;
    }
    
    if( transferHandle == obj->hMessageT5ObjRead &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_PROCESS_T5_MESSAGE;
    }
 
    if( transferHandle == obj->hMessageT44ObjWrite &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_READ_T44_MESSAGE;
    }
    
    if( transferHandle == obj->hMessageT44ObjRead &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_READ_T44_MESSAGE;
    }
    
        if( transferHandle == obj->hMessageT44T5ObjWrite &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_T44_T5_MESSAGE_READ;
    }
    
    if( transferHandle == obj->hMessageT44T5ObjRead &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_PROCESS_T44_T5_MESSAGE;
    }
    
    
    if ( transferHandle == obj->hConfigUpload &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_CONFIG_LOAD;
    }
    
    if( transferHandle == obj->hResetWrite &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE )
    {
        obj->deviceState = DEVICE_STATE_CONFIG_SOFTRESET;
        obj->configState = CONFIG_STATE_MXT_COMMAND_RESET_READ;
    }
    
    if ( transferHandle == obj->hResetRead &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_CONFIG_SOFTRESET;
        obj->configState = CONFIG_STATE_MXT_COMMAND_RESET_HANDLE;
    }  
    
    if ( transferHandle == obj->hMessageObjRead &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_HANDLE_MESSAGE_OBJECT;
    }
    
    if ( transferHandle == obj->hBackupNVWrite &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_CONFIG_SAVE;
        obj->configState = CONFIG_STATE_MXT_COMMAND_BACKUPNV_READ;
    }
    
    if ( transferHandle == obj->hBackupNVRead &&
        event == DRV_I2C_TRANSFER_EVENT_COMPLETE  )
    {
        obj->deviceState = DEVICE_STATE_CONFIG_SAVE;
        obj->configState = CONFIG_STATE_MXT_COMMAND_BACKUPNV_HANDLE;
    }
}

SYS_MODULE_OBJ DRV_MAXTOUCH_Initialize(const SYS_MODULE_INDEX index,
                                      const SYS_MODULE_INIT * const init)
{
    const DRV_MAXTOUCH_INIT *pInit = NULL;
    
#ifdef DEBUG_ENABLE    
    SYS_DEBUG_Print("MXT Init");
#endif
    
    if ( index >= DRV_MAXTOUCH_INDEX_COUNT )
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Attempting to initialize an instance number greater than the max");
        return SYS_MODULE_OBJ_INVALID;
    }

    struct DEVICE_OBJECT * pDrvInstance =
                ( struct DEVICE_OBJECT *)&sMAXTOUCHDriverInstances[index];

    if ( pDrvInstance->inUse == true )
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Attempting to reinitialize a driver instance that is already in use");
        return SYS_MODULE_OBJ_INVALID;
    }

    pDrvInstance->inUse = true;

    pInit = (const DRV_MAXTOUCH_INIT* const)init;

    /* */
    pDrvInstance->drvOpen         = pInit->drvOpen;
    pDrvInstance->drvI2CHandle    = DRV_HANDLE_INVALID;
//    infoBlockData = NULL;
    
    if(pInit->horizontalResolution > 0)
        pDrvInstance->xRes            = pInit->horizontalResolution - 1;
    else
        pDrvInstance->xRes = DEFAULT_XRES;
    
    if(pInit->verticalResolution > 0)
        pDrvInstance->yRes            = pInit->verticalResolution - 1;
    else
        pDrvInstance->yRes = DEFAULT_YRES;
    
    pDrvInstance->deviceState = DEVICE_STATE_OPEN;
    pDrvInstance->data.progress = 0;
    pDrvInstance->apiEvent = APP_DRV_MAXTOUCH_None;

    
    return (SYS_MODULE_OBJ)pDrvInstance;
}

void DRV_MAXTOUCH_Deinitialize ( SYS_MODULE_OBJ object )
{
    struct DEVICE_OBJECT * pDrvInstance =
                                        (struct DEVICE_OBJECT *)object;

    if( pDrvInstance == NULL )
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Attempting to deinitialize a NULL object");
        return;
    }

    if ( pDrvInstance->inUse == false )
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Attempting to deinitialize a driver instance that is not in use");
        return;
    }

    SYS_INT_SourceDisable(pDrvInstance->interruptSource);

    pDrvInstance->inUse                 = false;
    pDrvInstance->status                = SYS_STATUS_UNINITIALIZED;

    return;
}

SYS_STATUS DRV_MAXTOUCH_Status(SYS_MODULE_OBJ object)
{
    struct DEVICE_OBJECT* pDrvInstance = (struct DEVICE_OBJECT *)object;

    if ( object == SYS_MODULE_OBJ_INVALID )
    {
        //SYS_ASSERT( " Handle is invalid " );
        return SYS_STATUS_ERROR;
    }
    
    return pDrvInstance->status;
}

DRV_HANDLE DRV_MAXTOUCH_Open(const SYS_MODULE_INDEX index,
                            const DRV_IO_INTENT intent)
{ 
#ifdef DEBUG_ENABLE
    SYS_DEBUG_Print("MXT Open");
#endif
    
    if (index >= DRV_MAXTOUCH_INDEX_COUNT)
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Attempting to open an instance" \
                          "number greater than the max");
        return DRV_HANDLE_INVALID;
    }
    
    struct DEVICE_OBJECT * pDrvInstance = (struct DEVICE_OBJECT*)&sMAXTOUCHDriverInstances[index];

    /* Open the bus driver */
    if(pDrvInstance->drvOpen == NULL)
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Bus driver init parameter missing");
        return DRV_HANDLE_INVALID;
    }
    
    if(pDrvInstance->drvI2CHandle == DRV_HANDLE_INVALID)
    {
        pDrvInstance->drvI2CHandle = pDrvInstance->drvOpen(0,
                                                           DRV_IO_INTENT_READWRITE);
    }
    
    if(pDrvInstance->drvI2CHandle == DRV_HANDLE_INVALID)
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Bus driver initialization failed");
        return DRV_HANDLE_INVALID;
    }
    
    /* Register the I2C Driver event Handler */
    DRV_I2C_TransferEventHandlerSet( pDrvInstance->drvI2CHandle, 
                                     DRV_MAXTOUCH_I2CEventHandler, 
                                     (uintptr_t)pDrvInstance );

    if ((intent & DRV_IO_INTENT_EXCLUSIVE) == DRV_IO_INTENT_EXCLUSIVE)
    {
        pDrvInstance->isExclusive = true;
    }
          
    pDrvInstance->status = SYS_STATUS_BUSY;
        
    return (DRV_HANDLE)pDrvInstance;
}

void DRV_MAXTOUCH_Close (DRV_HANDLE handle)
{
    struct DEVICE_OBJECT* pDrvObject = (struct DEVICE_OBJECT*)handle;

    if(pDrvObject == NULL)
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Trying to close a client with invalid driver object");
        
        return;
    }
    
    /* move driver to the idle state to stop any processes */
    pDrvObject->deviceState = DEVICE_STATE_REQUEST_ID_BLOCK;
}

void DRV_MAXTOUCH_ConfigParse ( SYS_MODULE_OBJ object, DRV_MAXTOUCH_Firmware * firmware )
{
    struct DEVICE_OBJECT* pDrvObject = (struct DEVICE_OBJECT *)object;

    pDrvObject->callback = firmware->progress;
    pDrvObject->reader = firmware->reader;
    pDrvObject->eof = firmware->eof;

    mxt_configure_objects(pDrvObject, firmware);
    firmware->mem = &pDrvObject->data.config_mem;
    firmware->mem_size = pDrvObject->data.config_mem_size;
    
    SYS_TIME_TimerDestroy(resetTimer);

    resetTimer = SYS_TIME_CallbackRegisterMS(resetTimer_Callback, 
                        (uintptr_t)pDrvObject,
                        MXT_RESET_INVALID_CHG,
                        SYS_TIME_SINGLE);
    
    pDrvObject->deviceState = DEVICE_STATE_WAIT;
    pDrvObject->apiEvent = APP_DRV_MAXTOUCH_ConfigParse;

}

void DRV_MAXTOUCH_ConfigLoad ( SYS_MODULE_OBJ object, DRV_MAXTOUCH_Firmware * firmware )
{
    struct DEVICE_OBJECT* pDrvObject = (struct DEVICE_OBJECT *)object;

    pDrvObject->callback = firmware->progress;
    pDrvObject->reader = firmware->reader;
    pDrvObject->eof = firmware->eof;

    mxt_configure_objects(pDrvObject, firmware);
    firmware->mem = &pDrvObject->data.config_mem;
    firmware->mem_size = pDrvObject->data.config_mem_size;

    SYS_TIME_TimerDestroy(resetTimer);

    resetTimer = SYS_TIME_CallbackRegisterMS(resetTimer_Callback, 
                        (uintptr_t)pDrvObject,
                        MXT_RESET_INVALID_CHG,
                        SYS_TIME_SINGLE);

    pDrvObject->deviceState = DEVICE_STATE_WAIT;
    pDrvObject->apiEvent = APP_DRV_MAXTOUCH_ConfigLoad;
}

void DRV_MAXTOUCH_ConfigSave (SYS_MODULE_OBJ object, DRV_MAXTOUCH_ConfigProgress_FnPtr ptr )
{
    struct DEVICE_OBJECT* pDrvObject = (struct DEVICE_OBJECT *)object;

    SYS_TIME_TimerDestroy(resetTimer);

    resetTimer = SYS_TIME_CallbackRegisterMS(resetTimer_Callback, 
                        (uintptr_t)pDrvObject,
                        MXT_RESET_INVALID_CHG,
                        SYS_TIME_SINGLE);
    
    pDrvObject->callback = ptr;
    pDrvObject->deviceState = DEVICE_STATE_WAIT;
    pDrvObject->apiEvent = APP_DRV_MAXTOUCH_ConfigSave;

//	/* T7 config may have changed */
//	mxt_init_t7_power_cfg(data);
}

static void handleMessage(struct DEVICE_OBJECT* pDrvObject);

void DRV_MAXTOUCH_Tasks ( SYS_MODULE_OBJ object )
{
    
    struct DEVICE_OBJECT* pDrvObject = (struct DEVICE_OBJECT *)object;

    if(object == SYS_MODULE_OBJ_INVALID)
        return;
    
    /* MAXTOUCH Driver state machine */
    switch (pDrvObject->deviceState)
    {
        case DEVICE_STATE_WAIT:
        {
            break;
        }
                
        case DEVICE_STATE_OPEN:
        {
            DRV_MAXTOUCH_Open(0, DRV_IO_INTENT_EXCLUSIVE);
                    
            SYS_TIME_TimerDestroy(resetTimer);

            resetTimer = SYS_TIME_CallbackRegisterMS(resetTimer_Callback, 
                                (uintptr_t)pDrvObject,
                                MXT_RESET_INVALID_CHG,
                                SYS_TIME_SINGLE);
            
            pDrvObject->apiEvent = APP_DRV_MAXTOUCH_ResetCallback;
            pDrvObject->deviceState = DEVICE_STATE_WAIT;

            break;
        }
        case DEVICE_STATE_REQUEST_ID_BLOCK: /* Request information block */
        {
#ifdef DEBUG_ENABLE           
            SYS_DEBUG_Print("MXT State Init\n");
#endif           
			
            /* set information block address read */
            buf[0] = 0;
            buf[1] = 0;
            
            /* request info block from the controller */
            DRV_I2C_WriteTransferAdd(pDrvObject->drvI2CHandle,
                                       I2C_MASTER_WRITE_ID, 
                                       &buf[0],
                                       2,
                                       &pDrvObject->hInformationBlockRequest);

            /* wait for read complete callback from I2C */
            pDrvObject->deviceState = DEVICE_STATE_WAIT;
            
            break;
        }
        case DEVICE_STATE_READ_ID_BLOCK: /* Read information block */
        {

#ifdef DEBUG_ENABLE            
            SYS_DEBUG_Print("MXT Reset\n");
#endif

            /* clock in the information block */
            /* read  7-byte ID information block starting at address 0 */
            DRV_I2C_ReadTransferAdd(pDrvObject->drvI2CHandle,
                                      I2C_MASTER_READ_ID,
                                      &pDrvObject->data.info,
                                      sizeof(struct mxt_info),
                                      &pDrvObject->hInformationBlockRead);            
            
            /* wait for read complete callback from I2C */
            pDrvObject->deviceState = DEVICE_STATE_WAIT;
                        
            break;          
        }
        
        case DEVICE_STATE_REQUEST_OBJECT_TABLE: /* Request object table */
        {
            /* set object block address read */
            buf[0] = sizeof(struct mxt_info);
            buf[1] = 0;
            
            /* request object block from the controller */
            DRV_I2C_WriteTransferAdd(pDrvObject->drvI2CHandle,
                                       I2C_MASTER_WRITE_ID, 
                                       &buf[0],
                                       2,
                                       &pDrvObject->hObjectBlockRequest);

            /* wait for write complete callback from I2C */
            pDrvObject->deviceState = DEVICE_STATE_WAIT;
            break;
        }
        
        case DEVICE_STATE_READ_OBJECT_TABLE: /* Read object table */
        {
#ifdef DEBUG_ENABLE            
            SYS_DEBUG_Print("MXT Read Info Block\n");
#endif
            
            /* size of info */
            int size = sizeof(struct mxt_info);

            /* Resize buffer to give space for rest of info block */
            uint8_t num_objects = pDrvObject->data.info.object_num;
            size += (num_objects * sizeof(struct mxt_object)) + MXT_INFO_CHECKSUM_SIZE;
            
            memcpy(pDrvObject->data.raw_info_block, &pDrvObject->data.info, sizeof(struct mxt_info));
            
            pDrvObject->data.object_table = (struct mxt_object*)((uint8_t*)pDrvObject->data.raw_info_block + sizeof(struct mxt_info));
                                  
#ifdef DEBUG_ENABLE            
            SYS_DEBUG_Print("Message data size: %d\n", messageDataSize);
#endif
            
            /* clock in the object table */
            /* read starting at object table */
            DRV_I2C_ReadTransferAdd(pDrvObject->drvI2CHandle,
                                    I2C_MASTER_READ_ID,
                                    pDrvObject->data.object_table,
                                    size-sizeof(struct mxt_info),
                                    &pDrvObject->hObjectBlockRead);
            
            /* wait for read complete callback from I2C */
            pDrvObject->deviceState = DEVICE_STATE_WAIT;

            break;            
        }
        
        case DEVICE_STATE_READ_CRC_VALUE: /* Wait for object table to be read */
        {

#ifdef DEBUG_ENABLE            
            SYS_DEBUG_Print("MXT Read Object Table\n");
#endif
                        
            /* size of info */
            int size = sizeof(struct mxt_info);

            /* Resize buffer to give space for rest of info block */
            uint8_t num_objects = pDrvObject->data.info.object_num;
            size += (num_objects * sizeof(struct mxt_object))
                + MXT_INFO_CHECKSUM_SIZE;
    
            /* Extract & calculate checksum */
            uint8_t * crc_ptr = (uint8_t *)pDrvObject->data.raw_info_block + size - MXT_INFO_CHECKSUM_SIZE;
            pDrvObject->data.info_crc = crc_ptr[0] | (crc_ptr[1] << 8) | (crc_ptr[2] << 16);

            uint32_t calculated_crc = mxt_calculate_crc((uint8_t*)pDrvObject->data.raw_info_block, 0,
                               size - MXT_INFO_CHECKSUM_SIZE);

            /*
             * CRC mismatch can be caused by data corruption due to I2C comms
             * issue or else device is not using Object Based Protocol (eg i2c-hid)
             */
            if ((pDrvObject->data.info_crc == 0) || (pDrvObject->data.info_crc != calculated_crc)) {
                pDrvObject->deviceState = DEVICE_STATE_ERROR;
//                break;
            }                        
            
            // turn on message crc8 checksum
#ifdef MESSAGE_CHECKSUM_ENABLED
            messageProcessor->i2c_address |= 0x8000;
#endif                                     

            /* Parse object table information */
            mxt_parse_object_table(pDrvObject, pDrvObject->data.object_table);// + MXT_OBJECT_START);
    
            /* new state is config resolution */
            pDrvObject->deviceState = DEVICE_STATE_CONFIGURE_RESOLUTION;

            break;
        }   
        
        case DEVICE_STATE_CONFIGURE_RESOLUTION:
        {
#ifdef DEBUG_ENABLE            
            SYS_DEBUG_Print("MXT Configure Resolution\n");
#endif
            
            _RegWrite16(pDrvObject, T100_XRANGE, pDrvObject->xRes, &pDrvObject->hXRangeWrite );
            
            /* wait for response from I2C then new state wll be T100_XRANGE */
            pDrvObject->deviceState = DEVICE_STATE_WAIT;

            break;
        }
        
        case DEVICE_STATE_WRITE_T100_XRANGE:
        {

#ifdef DEBUG_ENABLE            
            SYS_DEBUG_Print("MXT Write T100 XRange %d\n", pDrvObject->taskQueue[0].drvI2CFrameData[2] | pDrvObject->taskQueue[0].drvI2CFrameData[3] << 8);
#endif
           
            _RegWrite16(pDrvObject, T100_YRANGE, pDrvObject->yRes, &pDrvObject->hYRangeWrite);
    
            /* wait for response from I2C then new state will be T100_YRANGE */
            pDrvObject->deviceState = DEVICE_STATE_WAIT;

            break;
        } 
        
        case DEVICE_STATE_WRITE_T100_YRANGE:
        {

#ifdef DEBUG_ENABLE           
            SYS_DEBUG_Print("MXT Write T100 YRange %d\n", pDrvObject->taskQueue[0].drvI2CFrameData[2] | pDrvObject->taskQueue[0].drvI2CFrameData[3] << 8);
#endif
            
            /* say we are ready and go to ready state */
            pDrvObject->status = SYS_STATUS_READY;
            pDrvObject->deviceState = DEVICE_STATE_READY;
            
            /* this for T44 T5 message count method  */
////            pDrvObject->deviceState = DEVICE_STATE_T44_T5_MESSAGE_REQUEST;
            
            break;
        }
    
        case DEVICE_STATE_CONFIG_LOAD:
        {
            _mxt_DelayMS(MXT_PROGRAM_DELAY);

            int index = mxt_upload_cfg_mem(pDrvObject);
            if ( index == 0 ) {
                pDrvObject->deviceState = DEVICE_STATE_CONFIG_SOFTRESET;
                pDrvObject->configState = CONFIG_STATE_MXT_COMMAND_RESET_WRITE;
            } else
            {
                pDrvObject->callback(progress[index]);
            }
            break;
        }
                
        case DEVICE_STATE_CONFIG_SAVE:
        {
            switch ( pDrvObject->configState )
            {
                case CONFIG_STATE_MXT_COMMAND_BACKUPNV_WRITE:
                {
                    uint16_t reg = pDrvObject->data.T6_address + MXT_COMMAND_BACKUPNV;
                    _RegWrite8(pDrvObject, reg, MXT_BACKUP_VALUE, &pDrvObject->hBackupNVWrite);
                    pDrvObject->deviceState = DEVICE_STATE_WAIT;
                    break;
                }
                
                case CONFIG_STATE_MXT_COMMAND_BACKUPNV_READ:
                {
                    _mxt_DelayMS(MXT_PROGRAM_DELAY);
                    _RegRead(pDrvObject, pDrvObject->data.msg_buf, 1, &pDrvObject->hBackupNVRead);
                    pDrvObject->deviceState = DEVICE_STATE_WAIT;
                    break;
                }
                
                case CONFIG_STATE_MXT_COMMAND_BACKUPNV_HANDLE:
                {
                    if ( pDrvObject->data.msg_buf[0] != 0 )
                    {
                        pDrvObject->configState = CONFIG_STATE_MXT_COMMAND_BACKUPNV_WRITE;
                    }
                    else
                    {
                        pDrvObject->callback(50);
                        pDrvObject->deviceState = DEVICE_STATE_CONFIG_SOFTRESET;
                        pDrvObject->configState = CONFIG_STATE_MXT_COMMAND_RESET_WRITE;
                    }
                    break;
                }
                
                default:
                    break;
            }
            break;
        }
        
        case DEVICE_STATE_CONFIG_SOFTRESET:
        {
            switch ( pDrvObject->configState )
            {
                case CONFIG_STATE_MXT_COMMAND_RESET_WRITE:
                {
                    uint16_t reg = pDrvObject->data.T6_address + MXT_COMMAND_RESET;
                    _RegWrite8(pDrvObject, reg, MXT_RESET_VALUE, &pDrvObject->hResetWrite);
                    pDrvObject->deviceState = DEVICE_STATE_WAIT;
                    break;
                }
                
                case CONFIG_STATE_MXT_COMMAND_RESET_READ:
                {
                    /* Ignore CHG line for 100ms after reset */
                    /* no need to read response - go to ready state */
                    _mxt_DelayMS(MXT_PROGRAM_DELAY);
                    pDrvObject->callback(100);
//                    _RegRead(pDrvObject, pDrvObject->data.msg_buf, 1, &pDrvObject->hResetRead);
//                    pDrvObject->deviceState = DEVICE_STATE_WAIT;
                    pDrvObject->deviceState = DEVICE_STATE_READY;
                    break;
                }
                
                case CONFIG_STATE_MXT_COMMAND_RESET_HANDLE:
                {
                    if ( pDrvObject->data.msg_buf[0] != 0 )
                    {
                        pDrvObject->configState = CONFIG_STATE_MXT_COMMAND_RESET_WRITE;
                    }
                    else
                    {
                        pDrvObject->callback(100);
                        pDrvObject->deviceState = DEVICE_STATE_READY;
                    }
                    break;
                }
                
                default:
                    break;
            }
            break;
        }
        
        case DEVICE_STATE_WRITE_RESET:
        {
            uint8_t buf[3];
            uint16_t reg;
            uint8_t val = MXT_RESET_VALUE;

            reg = pDrvObject->data.T6_address + MXT_COMMAND_RESET;
            
            buf[0] = reg & 0xff;
            buf[1] = (reg >> 8) & 0xff;
            memcpy(&buf[2], &val, 1);

            DRV_I2C_WriteTransferAdd(pDrvObject->drvI2CHandle, I2C_MASTER_WRITE_ID, buf, 3, &pDrvObject->hResetWrite); 

            pDrvObject->deviceState = DEVICE_STATE_WAIT;
            break;
        }
       
        case DEVICE_STATE_READ_RESET:
        {
            uint8_t command_register;
            _mxt_DelayMS(MXT_PROGRAM_DELAY);

            DRV_I2C_ReadTransferAdd(pDrvObject->drvI2CHandle, I2C_MASTER_READ_ID, &command_register, 1, &pDrvObject->hResetRead);

            /* Ignore CHG line for 100ms after reset */
            _mxt_DelayMS(MXT_RESET_INVALID_CHG);
            pDrvObject->deviceState = DEVICE_STATE_WAIT;
            break;
        }
      
        case DEVICE_STATE_READY: /* Driver ready state */
        { 
            /* Check for ~CHG line asserted in case interrupt line did not come back up high */
            /* send a read request to the message processor object T5 */ 
            /* use multi-message - NOT IMPLEMENTED */
//            if(MXT_INTERRUPT_PIN_VALUE_GET() == false)
//                pDrvObject->deviceState = DEVICE_STATE_T44_T5_MESSAGE_REQUEST;
//            else
//                break;
            
            /* use single message */
            if(MXT_INTERRUPT_PIN_VALUE_GET() == false)
            {
                _MessageObjectRead(pDrvObject);
            }
            break; 
        }
        
        case DEVICE_STATE_READ_MESSAGE_OBJECT:
        {
            _MessageObjectRead(pDrvObject);
            pDrvObject->deviceState = DEVICE_STATE_WAIT;

//            pDrvObject->deviceState = DEVICE_STATE_HANDLE_MESSAGE_OBJECT;
//            pDrvObject->deviceState = DEVICE_STATE_WAIT;
            
            break;
        }
                        
        case DEVICE_STATE_T44_T5_MESSAGE_REQUEST: /* Driver ready state */
        {   
            if (pDrvObject == NULL)
                break;

            if (!pDrvObject->data.object_table)
                break ;

            /* write the address of the message processor object to the device */     
            buf[0] = pDrvObject->data.T44_address & 0xFF;  
            buf[1] = pDrvObject->data.T44_address >> 8;

            DRV_I2C_WriteTransferAdd(pDrvObject->drvI2CHandle,
                                        I2C_MASTER_WRITE_ID, 
                                        &buf[0],
                                        2,
                                        &pDrvObject->hMessageT44T5ObjWrite);

            pDrvObject->deviceState = DEVICE_STATE_WAIT;

            break;
        }
            
        case DEVICE_STATE_T44_T5_MESSAGE_READ:
        {  

            /* schedule a read of the message processor object */
            memset(&pDrvObject->mxtMsg, 0, sizeof(MAXTOUCH_Message));

            DRV_I2C_ReadTransferAdd(pDrvObject->drvI2CHandle,
                                    I2C_MASTER_READ_ID,
                                    pDrvObject->data.msg_buf,
                                    pDrvObject->data.T5_msg_size + 1,
                                    &pDrvObject->hMessageT44T5ObjRead);

            pDrvObject->deviceState = DEVICE_STATE_WAIT;

            break; 
        }
        
        case DEVICE_STATE_PROCESS_T44_T5_MESSAGE:
        {
            pDrvObject->data.count = pDrvObject->data.msg_buf[0];
            
            /*
             * This condition may be caused by the CHG line being configured in
             * Mode 0. It results in unnecessary I2C operations but it is benign.
             */
            if (pDrvObject->data.count == 0)
                break;

            if (pDrvObject->data.count > pDrvObject->data.max_reportid) {
                pDrvObject->data.count = pDrvObject->data.max_reportid;
            }
            
            /* Process first message */
            int ret = mxt_proc_message(&pDrvObject->data, pDrvObject->data.msg_buf + 1);
            if (ret < 0) {
//                dev_warn(dev, "Unexpected invalid message\n");
                break ;
            }

            pDrvObject->data.num_left = pDrvObject->data.count - 1;
                       
            /* Process remaining messages if necessary */
            if (pDrvObject->data.num_left) 
            {
                /* Safety check for msg_buf */
                if (pDrvObject->data.count > pDrvObject->data.max_reportid)
                    break;

                /* write the address of the message processor object to the device */     
                buf[0] = pDrvObject->data.T5_address & 0xFF;  
                buf[1] = pDrvObject->data.T5_address >> 8;

                DRV_I2C_WriteTransferAdd(pDrvObject->drvI2CHandle,
                                            I2C_MASTER_WRITE_ID, 
                                            &buf[0],
                                            2,
                                            &pDrvObject->hMessageT5ObjWrite);
            
                pDrvObject->deviceState = DEVICE_STATE_WAIT;
            } else
            {
                pDrvObject->deviceState = DEVICE_STATE_READY;
                pDrvObject->status = SYS_STATUS_READY;

            }
            
            break;
        }
 
        case DEVICE_STATE_READ_T5_MESSAGE:
        {
            /* Safety check for msg_buf */
            if (pDrvObject->data.count > pDrvObject->data.max_reportid)
                break;

            /* Process remaining messages if necessary */
            DRV_I2C_ReadTransferAdd(pDrvObject->drvI2CHandle,
                                    I2C_MASTER_READ_ID,
                                    pDrvObject->data.msg_buf,
                                    pDrvObject->data.T5_msg_size  * pDrvObject->data.count,
                                    &pDrvObject->hMessageT5ObjRead);
            
            pDrvObject->deviceState = DEVICE_STATE_WAIT;

            break;
        }
                
        case DEVICE_STATE_PROCESS_T5_MESSAGE:
        {
            int num_valid = 0;
            int i;
            int ret;
            for (i = 0;  i < pDrvObject->data.num_left; i++) 
            {
                ret = mxt_proc_message(&pDrvObject->data,
                    pDrvObject->data.msg_buf + pDrvObject->data.T5_msg_size * i);

                if (ret == 1)
                    num_valid++;
            } 
            pDrvObject->deviceState = DEVICE_STATE_READY;
            break;
        }
        
        case DEVICE_STATE_REQUEST_T44_MESSAGE:
        {
            _MessageObjectRead(pDrvObject);
            
            pDrvObject->deviceState = DEVICE_STATE_HANDLE_MESSAGE_OBJECT;
            pDrvObject->deviceState = DEVICE_STATE_WAIT;

            
            break;
        }
        
        case DEVICE_STATE_READ_T44_MESSAGE: /* read a specified object from the device */
        {

#ifdef DEBUG_ENABLE            
            SYS_DEBUG_Print("MXT message id: %d\n", pDrvObject->mxtMsg.reportID);
#endif
            
            pDrvObject->data.count = pDrvObject->data.msg_buf[0];
            
            /*
             * This condition may be caused by the CHG line being configured in
             * Mode 0. It results in unnecessary I2C operations but it is benign.
             */
            if (pDrvObject->data.count == 0)
                break;

            if (pDrvObject->data.count > pDrvObject->data.max_reportid) {
//                dev_warn(dev, "T44 count %d exceeded max report id\n", count);
                pDrvObject->data.count = pDrvObject->data.max_reportid;
            }
            
            /* Process first message */
            int ret = mxt_proc_message(&pDrvObject->data, pDrvObject->data.msg_buf + 1);
            if (ret < 0) {
//                dev_warn(dev, "Unexpected invalid message\n");
                break ;
            }

            pDrvObject->data.num_left = pDrvObject->data.count - 1;
            
            
            /* Process remaining messages if necessary */
            if (pDrvObject->data.num_left) {
                pDrvObject->deviceState = DEVICE_STATE_REQUEST_T5_MESSAGE;
            } else
            {
                pDrvObject->deviceState = DEVICE_STATE_READY;
            }
            break;
        }
        
        case DEVICE_STATE_HANDLE_MESSAGE_OBJECT: /* read a specified object from the device */
        {
            if(pDrvObject->mxtMsg.reportID == 0xFF)
            {
                pDrvObject->deviceState = DEVICE_STATE_READY; 
                
                return;
            }
            
#ifdef MESSAGE_CHECKSUM_ENABLED            
            if(checksumMessage((uint8_t*)&pDrvObject->mxtMsg) == 0)
            {
                handleMessage(pDrvObject);
            }
            else
            {
#ifdef DEBUG_ENABLE                
                SYS_DEBUG_Print("Checksum failed\n");
#endif
            }
#else
            handleMessage(pDrvObject);
#endif            
            pDrvObject->deviceState = DEVICE_STATE_READ_MESSAGE_OBJECT;    

            break;
        }
        
        case DEVICE_STATE_CONFIG_PARSE:
        {
            static int val = 1;
            pDrvObject->callback(val);

            if ( ++val > 100 )
            {
                pDrvObject->deviceState = DEVICE_STATE_READY;
                val = 1;
            }
            _mxt_DelayMS(MXT_RESET_GPIO_TIME);
            break;
        }
        
        case DEVICE_STATE_ERROR: /* In error state */
        {
            pDrvObject->status = SYS_STATUS_ERROR;
            break;
        }
        
        default:
        {
            break;
        }
    }
    
    return;
}

// *****************************************************************************
// *****************************************************************************
// Section: Internal functions used by this module only
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Send request to read message processor object */
static void _MessageObjectRead(struct DEVICE_OBJECT *pDrvObject)
{
    //size_t msgSize;

    if (pDrvObject == NULL)
        return;
    
    if (!pDrvObject->data.object_table)
        return ;
    
    /* write the address of the message processor object to the device */     
    buf[0] = pDrvObject->data.T5_address & 0xFF;  
    buf[1] = pDrvObject->data.T5_address >> 8;
    
    DRV_I2C_WriteTransferAdd(pDrvObject->drvI2CHandle,
                                I2C_MASTER_WRITE_ID, 
                                &buf[0],
                                2,
                                &pDrvObject->hMessageObjWrite);

    /* schedule a read of the message processor object */
    memset(&pDrvObject->mxtMsg, 0, sizeof(MAXTOUCH_Message));

    DRV_I2C_ReadTransferAdd(pDrvObject->drvI2CHandle,
                            I2C_MASTER_READ_ID,
                            &pDrvObject->mxtMsg,
                            I2C_READ_ID_FRAME_SIZE,
                            &pDrvObject->hMessageObjRead);
    
    pDrvObject->deviceState = DEVICE_STATE_WAIT;
                
}

#if 0
static void _SendResetCommand(struct DEVICE_OBJECT *pDrvObject)
{
    if (pDrvObject == NULL)
        return;
    
    /* write the address of the command processor object to the device */     
    memset(pDrvObject->taskQueue[0].drvI2CFrameData, 0, I2C_FRAME_SIZE);
    
    pDrvObject->taskQueue[0].drvI2CFrameData[0] = commandProcessor->i2c_address & 0xFF;  
    pDrvObject->taskQueue[0].drvI2CFrameData[1] = commandProcessor->i2c_address >> 8;
    
    pDrvObject->taskQueue[0].drvI2CFrameData[2] = 0x1; // nonzero = reset command
    
    pDrvObject->taskQueue[0].drvI2CBufferHandle = DRV_I2C_Transmit(pDrvObject->drvI2CHandle,
                                                                   I2C_MASTER_WRITE_ID, 
                                                                   &pDrvObject->taskQueue[0].drvI2CFrameData[0],
                                                                   9, // address plus 7 bytes of command data
                                                                   NULL); 
}
#endif

/* Send request to read message processor object */
static void _RegWrite8(struct DEVICE_OBJECT *pDrvObject, uint8_t reg, uint8_t val, DRV_I2C_TRANSFER_HANDLE * transferHandle)
{
    uint16_t pReg;

    /* write the address of the object register to the device */     
    pReg = pDrvObject->data.T6_address + MXT_COMMAND_BACKUPNV;

    buf[0] = pReg & 0xFF;  
    buf[1] = pReg >> 8;
    buf[2] = val;

    DRV_I2C_WriteTransferAdd(pDrvObject->drvI2CHandle,
                                I2C_MASTER_WRITE_ID,
                                &buf[0],
                                3,
                                transferHandle);
}

/* Send request to read message processor object */
static void _RegWrite16(struct DEVICE_OBJECT *pDrvObject, uint8_t reg, uint16_t val, DRV_I2C_TRANSFER_HANDLE * transferHandle)
{
    uint16_t pReg;

    if (pDrvObject == NULL)
        return;
    
    /* write the address of the object register to the device */     
    pReg = pDrvObject->data.T100_address + reg;
    
    buf[0] = pReg & 0xFF;  
    buf[1] = pReg >> 8;
    buf[2] = val & 0xFF;  
    buf[3] = val >> 8;
              
    DRV_I2C_WriteTransferAdd(pDrvObject->drvI2CHandle,
                                I2C_MASTER_WRITE_ID,
                                &buf[0],
                                4,
                                transferHandle);
}

/* Send request to read message processor object */
static void _RegRead(struct DEVICE_OBJECT *pDrvObject, void* val, size_t size, DRV_I2C_TRANSFER_HANDLE * transferHandle)
{
    if (pDrvObject == NULL)
        return;
            
    DRV_I2C_ReadTransferAdd(pDrvObject->drvI2CHandle,
                            I2C_MASTER_READ_ID,
                            pDrvObject->data.msg_buf,
                            size,
                            transferHandle);
}

static void handleMessage(struct DEVICE_OBJECT* pDrvObject)
{    
    if ((pDrvObject->mxtMsg.reportID == 0x00) || (pDrvObject->mxtMsg.reportID == 0xFF))
    {
        /* invalid message */
        return;
    }   
    
    // multitouch message?
    if(pDrvObject->mxtMsg.reportID >= pDrvObject->data.T100_reportid_min &&
       pDrvObject->mxtMsg.reportID <= pDrvObject->data.T100_reportid_max)
    {
        _handleTouchMessage(pDrvObject->mxtMsg.reportID - pDrvObject->data.T100_reportid_min-2,
                            (MAXTOUCH_TouchEvent*)pDrvObject->mxtMsg.data);
    }
    else
    {
#ifdef DEBUG_ENABLE        
        SYS_DEBUG_Print("MXT unhandled id: %d\n", pDrvObject->mxtMsg.reportID);
#endif
    }
}

#ifdef MESSAGE_CHECKSUM_ENABLED
static uint8_t crc8(uint8_t crc, uint8_t data)
{
    static const uint8_t crcpoly = 0x8C;
    
    uint8_t index;
    uint8_t fb;
    
    index = 8;
    
    do
    {
        fb = (crc ^ data) & 0x01;
        
        data >>= 1;
        crc >>= 1;
        
        if(fb)
            crc ^= crcpoly;
    }
    while(--index);
    
    return crc;
}

static uint8_t checksumMessage(uint8_t* msg)
{
    uint8_t crc = 0;
    uint8_t data_in;
    uint32_t i;
    
    for(i = 0; i < sizeof(MAXTOUCH_Message) - 1; i++)
    {
        data_in = msg[i];
        crc = crc8(crc, data_in);
    }
    
    return crc;
}
#endif

static void _handleTouchMessage(uint8_t touchID, MAXTOUCH_TouchEvent* tchEvt)
{
    //uint8_t detect;
    //uint8_t type;
    uint8_t event;
    uint16_t xpos = 0;
    uint16_t ypos = 0;

    //detect = tchEvt->tchstatus >> 7;
    //type = tchEvt->tchstatus >> 6;
    event = tchEvt->tchstatus & 0xF;
    xpos = tchEvt->x_lo;
    xpos |= tchEvt->x_hi << 8;
    ypos = tchEvt->y_lo;
    ypos |= tchEvt->y_hi << 8;
    
    if(event == 0)
        return;
    
    //HACK TO GET TOUCH WORKING. Driver sends touchID = 0xF7 which aria rejects
//    touchID = 0;
    
#ifdef DEBUG_ENABLE      
    SYS_DEBUG_Print("Touch event - id: %d, detect: %d, type: %d, event: %d, xpos: %d, ypos: %d\n", touchID, detect, type, event, xpos, ypos);
#endif
    
    switch(event)
    {
        case 0x4: // touch down
        {            
            SYS_INP_InjectTouchDown(touchID, xpos, ypos);
            
            break;
        }
        case 0x5: // touch up
        {            
            SYS_INP_InjectTouchUp(touchID, xpos, ypos);
            
            break;
        }
        case 0x1: // touch move
        {            
            SYS_INP_InjectTouchMove(touchID, xpos, ypos);
            
            break;
        }
    }
}

struct mxt_object *
mxt_get_object(struct DEVICE_OBJECT *pDrvObject, uint8_t type)
{
	struct mxt_object *object;
	int i;

	for (i = 0; i < pDrvObject->data.info.object_num; i++) {
		object = pDrvObject->data.object_table + i;
		if (object->type == type)
			return object;
	}

	return NULL;
}

//
size_t mxt_obj_size(const struct mxt_object *obj)
{
	return obj->size_minus_one + 1;
}

size_t mxt_obj_instances(const struct mxt_object *obj)
{
	return obj->instances_minus_one + 1;
}

#define dev_dbg(dev, format, ...)		     \
do {						     \
} while (0)

#define dev_err(dev, format, ...)		     \
do {						     \
} while (0)

#define dev_warn(dev, format, ...)		     \
do {						     \
} while (0)

#define dev_info(dev, format, ...)		     \
do {						     \
} while (0)

static bool __mxt_read_reg(struct i2c_client *client,
			       uint16_t reg, uint16_t len, void *val)
{
//	uint8_t buf[2];
//	int ret;
//
//	buf[0] = reg & 0xff;
//	buf[1] = (reg >> 8) & 0xff;
//    
////    ret = DRV_I2C_WriteReadTransfer ( client->drvI2CHandle, client->addr, buf, 2, val, len);
//    ret = 0;

	return true;
}

static int __mxt_write_reg(struct i2c_client *client, uint16_t reg, uint16_t len,
			   const void *val)
{
//	uint8_t *buf;
//	size_t count;
//	int ret;
//
//	count = len + 2;
//	buf = OSAL_Malloc(count);
//	if (!buf)
//		return 0;
//
//	buf[0] = reg & 0xff;
//	buf[1] = (reg >> 8) & 0xff;
//	memcpy(&buf[2], val, len);
//
////    ret = DRV_I2C_WriteTransfer( client->drvI2CHandle, client->addr, buf, count);
//	if (ret == count) {
//		ret = 0;
//	} else {
//		if (ret >= 0)
//			ret = 0;
//	}
//
//	OSAL_Free(buf);
	return 0;
}

static int mxt_write_reg(struct i2c_client *client, uint16_t reg, uint8_t val)
{
	return __mxt_write_reg(client, reg, 1, &val);
}

static bool mxt_t6_command(struct DEVICE_OBJECT *pDrvObject, uint16_t cmd_offset,
			  uint8_t value, bool wait)
{
	uint16_t reg;
	uint8_t command_register;
	int timeout_counter = 0;

    return true;
    
	reg = pDrvObject->data.T6_address + cmd_offset;

	mxt_write_reg(pDrvObject->data.client, reg, value);

	if (!wait)
		return true;

	do {
		_mxt_DelayMS(MXT_RESET_GPIO_TIME);
		__mxt_read_reg(pDrvObject->data.client, reg, 1, &command_register);

	} while (command_register != 0 && timeout_counter++ <= 100);

	if (timeout_counter > 100) {
		dev_err(&pDrvObject->data.client->dev, "Command failed!\n");
		return false;
	}

	return true;
}

//static int mxt_acquire_irq(struct mxt_data *data)
//{
//	int error;
//
////	enable_irq(data->irq);
//
//	error = mxt_process_messages_until_invalid(data);
//	if (error)
//		return error;
//
//	return 0;
//}

bool mxt_soft_reset(struct DEVICE_OBJECT *pDrvObject)
{
//	disable_irq(data->irq);

//	reinit_completion(&data->reset_completion);

	if ( mxt_t6_command(pDrvObject, MXT_COMMAND_RESET, MXT_RESET_VALUE, false) == false )
		return false;

	/* Ignore CHG line for 100ms after reset */
	_mxt_DelayMS(MXT_RESET_INVALID_CHG);

//	mxt_acquire_irq(data);

//	ret = mxt_wait_for_completion(data, &data->reset_completion,
//				      MXT_RESET_TIMEOUT);

	return true;
}

static void mxt_update_crc(struct DEVICE_OBJECT *pDrvObject, uint8_t cmd, uint8_t value)
{
	/*
	 * On failure, CRC is set to 0 and config will always be
	 * downloaded.
	 */
	pDrvObject->data.config_crc = 0;
//	reinit_completion(&data->crc_completion);

	mxt_t6_command(pDrvObject, cmd, value, true);

	/*
	 * Wait for crc message. On failure, CRC is set to 0 and config will
	 * always be downloaded.
	 */
//	mxt_wait_for_completion(data, &data->crc_completion, MXT_CRC_TIMEOUT);
}

static void mxt_calc_crc24(uint32_t *crc, uint8_t firstbyte, uint8_t secondbyte)
{
	static const unsigned int crcpoly = 0x80001B;
	uint32_t result;
	uint32_t data_word;

	data_word = (secondbyte << 8) | firstbyte;
	result = ((*crc << 1) ^ data_word);

	if (result & 0x1000000)
		result ^= crcpoly;

	*crc = result;
}

uint32_t mxt_calculate_crc(uint8_t *base, long start_off, long end_off)
{
	uint32_t crc = 0;
	uint8_t *ptr = base + start_off;
	uint8_t *last_val = base + end_off - 1;

	if (end_off < start_off)
		return 0;

	while (ptr < last_val) {
		mxt_calc_crc24(&crc, *ptr, *(ptr + 1));
		ptr += 2;
	}

	/* if len is odd, fill the last byte with 0 */
	if (ptr == last_val)
		mxt_calc_crc24(&crc, *ptr, 0);

	/* Mask to 24-bit */
	crc &= 0x00FFFFFF;

	return crc;
}

bool mxt_parse_object_table(struct DEVICE_OBJECT *pDrvObject,
				  struct mxt_object *object_table)
{
	int i;
	uint8_t reportid;
	uint16_t end_address;

	/* Valid Report IDs start counting from 1 */
	reportid = 1;
	pDrvObject->data.mem_size = 0;
	for (i = 0; i < pDrvObject->data.info.object_num; i++) {
		struct mxt_object *object = object_table + i;
		uint8_t min_id, max_id;

//		le16_to_cpus(&object->start_address);        

		if (object->num_report_ids) {
			min_id = reportid;
			reportid += object->num_report_ids *
					mxt_obj_instances(object);
			max_id = reportid - 1;
		} else {
			min_id = 0;
			max_id = 0;
		}

		switch (object->type) {
        
        case MXT_SPT_DYNAMICCONFIGURATIONCONTAINER_T71:
            pDrvObject->data.T71_address = object->start_address;
            break;
        
		case MXT_GEN_MESSAGE_T5:
			if (pDrvObject->data.info.family_id == 0x80 &&
			    pDrvObject->data.info.version < 0x20) {
				/*
				 * On mXT224 firmware versions prior to V2.0
				 * read and discard unused CRC byte otherwise
				 * DMA reads are misaligned.
				 */
				pDrvObject->data.T5_msg_size = mxt_obj_size(object);
			} else {
				/* CRC not enabled, so skip last byte */
				pDrvObject->data.T5_msg_size = mxt_obj_size(object) - 1;
			}
			pDrvObject->data.T5_address = object->start_address;
			break;
		case MXT_GEN_COMMAND_T6:
			pDrvObject->data.T6_reportid = min_id;
			pDrvObject->data.T6_address = object->start_address;
			break;
		case MXT_GEN_POWER_T7:
			pDrvObject->data.T7_address = object->start_address;
			break;
		case MXT_TOUCH_MULTI_T9:
			pDrvObject->data.multitouch = MXT_TOUCH_MULTI_T9;
			pDrvObject->data.T9_reportid_min = min_id;
			pDrvObject->data.T9_reportid_max = max_id;
			pDrvObject->data.num_touchids = object->num_report_ids
						* mxt_obj_instances(object);
			break;
		case MXT_SPT_MESSAGECOUNT_T44:
			pDrvObject->data.T44_address = object->start_address;
			break;
		case MXT_SPT_GPIOPWM_T19:
			pDrvObject->data.T19_reportid = min_id;
			break;
		case MXT_TOUCH_MULTITOUCHSCREEN_T100:
			pDrvObject->data.multitouch = MXT_TOUCH_MULTITOUCHSCREEN_T100;
			pDrvObject->data.T100_reportid_min = min_id;
			pDrvObject->data.T100_reportid_max = max_id;
            pDrvObject->data.T100_address = object->start_address;
			/* first two report IDs reserved */
			pDrvObject->data.num_touchids = object->num_report_ids - 2;
			break;
		}

		end_address = object->start_address
			+ mxt_obj_size(object) * mxt_obj_instances(object) - 1;

		if (end_address >= pDrvObject->data.mem_size)
			pDrvObject->data.mem_size = end_address + 1;
	}

	/* Store maximum reportid */
	pDrvObject->data.max_reportid = reportid;

	/* If T44 exists, T5 position has to be directly after */
	if (pDrvObject->data.T44_address && (pDrvObject->data.T5_address != pDrvObject->data.T44_address + 1)) {
		return false;
	}

    assert(pDrvObject->data.T5_msg_size <= MXT_MSG_BUFFER_SIZE);

	return true;
}

static bool mxt_prepare_cfg_mem(struct DEVICE_OBJECT *pDrvObject,
			       char * ptr,
			       unsigned int cfg_start_ofs,
			       uint8_t *config_mem,
			       size_t config_mem_size)
{
	struct mxt_object *object;
	unsigned int type, instance, size, byte_offset;
	int ret;
	int i;
	uint16_t reg;
    uint8_t byte;
    int scan;

	while (true) {
		/* Read type, instance, length */
		ret = sscanf(ptr, "%x %x %x",
			     &type, &instance, &size);
		if (ret == EOF || type == 0x0 ) {
			/* EOF */
			break;
		} else if (ret != 3) {
			dev_err(dev, "Bad format: failed to parse object\n");
			return false;
		}
        /* increment past object header */
		ptr+=15;

		object = mxt_get_object(pDrvObject, type);
		if (!object) {
			/* Skip object */
			for (i = 0; i < size; i++) {
				ret = sscanf(ptr, "%2x",
					     &scan);
				if (ret != 1) {
					dev_err(dev, "Bad format in T%d at %d\n",
						type, i);
					return false;
				}
                /* increment two characters */
                ptr+=3;
			}
			continue;
		}

		if (size > mxt_obj_size(object)) {
			/*
			 * Either we are in fallback mode due to wrong
			 * config or config from a later fw version,
			 * or the file is corrupt or hand-edited.
			 */
			dev_warn(dev, "Discarding %zu byte(s) in T%u\n",
				 size - mxt_obj_size(object), type);
		} else if (mxt_obj_size(object) > size) {
			/*
			 * If firmware is upgraded, new bytes may be added to
			 * end of objects. It is generally forward compatible
			 * to zero these bytes - previous behaviour will be
			 * retained. However this does invalidate the CRC and
			 * will force fallback mode until the configuration is
			 * updated. We warn here but do nothing else - the
			 * malloc has zeroed the entire configuration.
			 */
			dev_warn(dev, "Zeroing %zu byte(s) in T%d\n",
				 mxt_obj_size(object) - size, type);
		}

		if (instance >= mxt_obj_instances(object)) {
			dev_err(dev, "Object instances exceeded!\n");
			return false;
		}

		reg = object->start_address + mxt_obj_size(object) * instance;
        
		for (i = 0; i < size; i++) {
			ret = sscanf(ptr, "%2x",
				     &scan
				     );
            byte = (uint8_t)scan;
            
			if (ret != 1) {
				dev_err(dev, "Bad format in T%d at %d\n",
					type, i);
				return false;
			}
            /* increment two chars */
			ptr+=3;

			if (i > mxt_obj_size(object))
				continue;

			byte_offset = reg + i - cfg_start_ofs;

			if (byte_offset >= 0 && byte_offset < config_mem_size) {
				*(config_mem + byte_offset) = byte;
			} else {
				dev_err(dev, "Bad object: reg:%d, T%d, ofs=%d\n",
					reg, object->type, byte_offset);
				return false;
			}
		}
        ptr+=1;
	}

	return true;
}

int mxt_upload_cfg_mem(struct DEVICE_OBJECT* pDeviceObject)
{
	static unsigned int byte_offset = 0;
    static int index=0;
       
    /* write configuration as block */
    if ( byte_offset < pDeviceObject->data.config_mem_size )
    {
        unsigned int size = pDeviceObject->data.config_mem_size - byte_offset;
        if (size > MXT_MAX_BLOCK_WRITE)
			size = MXT_MAX_BLOCK_WRITE;
        
        buf[0] = (pDeviceObject->data.cfg_start_ofs + byte_offset) & 0xff;
        buf[1] = ((pDeviceObject->data.cfg_start_ofs + byte_offset) >> 8) & 0xff;
        memcpy((void*)&buf[2], pDeviceObject->data.config_mem+byte_offset, size);

        DRV_I2C_WriteTransferAdd(pDeviceObject->drvI2CHandle, I2C_MASTER_READ_ID, buf, size+2, &pDeviceObject->hConfigUpload); 
        pDeviceObject->deviceState = DEVICE_STATE_WAIT;
        
        byte_offset += size;
        index++;
    } else
    {
        byte_offset = 0;
        index =0;
    }

    return index;
}

static int mxt_set_t7_power_cfg(struct DEVICE_OBJECT* pDeviceObject, uint8_t sleep)
{
	int error;
	struct t7_config *new_config;
	struct t7_config deepsleep = { .active = 0, .idle = 0 };

	if (sleep == MXT_POWER_CFG_DEEPSLEEP)
		new_config = &deepsleep;
	else
		new_config = &pDeviceObject->data.t7_cfg;

	error = __mxt_write_reg(pDeviceObject->data.client, pDeviceObject->data.T7_address,
				sizeof(pDeviceObject->data.t7_cfg), new_config);
	if (error)
		return error;

	dev_dbg(dev, "Set T7 ACTV:%d IDLE:%d\n",
		new_config->active, new_config->idle);

	return 0;
}

static bool mxt_init_t7_power_cfg(struct DEVICE_OBJECT* pDeviceObject)
{
//	struct device *dev = &data->client->dev;
//	int error;
	bool retry = false;

    return true; 
    
recheck:
	__mxt_read_reg(pDeviceObject->data.client, pDeviceObject->data.T7_address,
				sizeof(pDeviceObject->data.t7_cfg), &pDeviceObject->data.t7_cfg);

	if (pDeviceObject->data.t7_cfg.active == 0 || pDeviceObject->data.t7_cfg.idle == 0) {
		if (!retry) {
			dev_dbg(dev, "T7 cfg zero, resetting\n");
			mxt_soft_reset(pDeviceObject);
			retry = true;
			goto recheck;
		} else {
			dev_dbg(dev, "T7 cfg zero after reset, overriding\n");
			pDeviceObject->data.t7_cfg.active = 20;
			pDeviceObject->data.t7_cfg.idle = 100;
			return mxt_set_t7_power_cfg(pDeviceObject, MXT_POWER_CFG_RUN);
		}
	}

	dev_dbg(dev, "Initialized power cfg: ACTV %d, IDLE %d\n",
		pDeviceObject->data.t7_cfg.active, pDeviceObject->data.t7_cfg.idle);
	return 0;
}

bool mxt_configure_objects(struct DEVICE_OBJECT* pDeviceObject, DRV_MAXTOUCH_Firmware *firmware)
{    
	mxt_init_t7_power_cfg(pDeviceObject);

    if ( firmware->type == DRV_MAXTOUCH_RAW_FLASH )
    {
        mxt_load_raw_flash(pDeviceObject, (char *)firmware->data);
    } else 
    {
        char *extension = strrchr((const char*)firmware->data, '.');
        if (extension && !strcmp(extension, ".xcfg")) {
          mxt_load_xcfg_file(pDeviceObject, (const char*)firmware->data);
        } else {
          mxt_load_raw_file(pDeviceObject, (char*)firmware->data);
        }
    }
    
//	if (data->multitouch) {
//		mxt_initialize_input_device(data);
//	} else {
//		dev_warn(dev, "No touch object detected\n");
//	}

//	mxt_debug_init(data);

	return true;
}


/*
 * mxt_update_cfg_xcfg - download configuration to chip
 *
 * Microchip XCFG Config File Format
 *
 * The first four lines of the raw config file contain:
 * [COMMENTS]
 * Date and time: Thu Mar 29 21:13:59 2018
 * [VERSION_INFO_HEADER]
 * FAMILY_ID=164
 * VARIANT=21
 * VERSION=33
 * BUILD=170
 * CHECKSUM=0xCCDA0F
 * INFO_BLOCK_CHECKSUM=0x8DDD0D
 * [APPLICATION_INFO_HEADER]
 * NAME=libmaxtouch
 * VERSION=1.27-7-g6d84bb0cd4b5
 *
 * The rest of the file consists of one line per object instance:
 *   <TYPE> <INSTANCE> <ADDRESS> <SIZE> <CONTENTS
 *
 *   <TYPE> - example [DEBUG_DIAGNOSTIC_T37 INSTANCE 0]
 *   <INSTANCE> - object instance number as decimal
 *   <ADDRESS> - object address as decimal
 *   <SIZE> - object size as decimal
 *   <CONTENTS> - attribute value pairs offset data
 */
bool mxt_load_xcfg_file(struct DEVICE_OBJECT * pDeviceObject, const char *filename)
{
    char c;
	char item[255];
	char readline[255];
    char *substr;
    int instance;
    int object_address;
    bool ignore_line = true;
    int i;
    struct mxt_object * object;
    unsigned int type, last_type=0, size=0;
    int object_num=0;
    unsigned int byte_offset;
    uint16_t reg;
    uint32_t config_crc = 0, calculated_crc = 0;

    i = 0;
	item[0] = '\0';
	readline[0] = '\0';
    
    /* Malloc memory to store configuration */
    pDeviceObject->data.cfg_start_ofs = MXT_OBJECT_START +
            pDeviceObject->data.info.object_num * sizeof(struct mxt_object) +
            MXT_INFO_CHECKSUM_SIZE;
    pDeviceObject->data.config_mem_size = pDeviceObject->data.mem_size - pDeviceObject->data.cfg_start_ofs;
    
    while ( pDeviceObject->eof() == false ) 
    {
            
        if ( ignore_line )
        {
            /* read line */
            pDeviceObject->reader(readline);
        }
            
        /* First character is expected to be '[' - skip empty lines and spaces  */
        c = readline[i];
        while ((c == '\n') || (c == '\r') || (c == 0x20))
            c = readline[i++];
            
        if (c != '[') 
        {
            if (c == EOF)
                break;

            /* If we are ignoring the current section then look for the next section */
            if (ignore_line) 
            {
                continue;
            }
//                mxt_err(ctx, "Parse error: expected '[', read ascii char %c!", c);
            return false;
        }

        if (sscanf(readline, "[%s^]", item) != 1) 
        {
//              mxt_err(ctx, "Object parse error");
            return false;
        }

        /* Ignore the comments and file header sections */
        if (!strncmp(item, "COMMENTS", 8)
            || !strncmp(item, "APPLICATION_INFO_HEADER", 23)) {
          ignore_line = true;
//              mxt_dbg(ctx, "Skipping %s", item);
          continue;
        }

        ignore_line = false;
                       
        char * p;
        /* Extract the checksum */
        if (!strncmp(item, "VERSION_INFO_HEADER", 19)) {
            while (false == ignore_line) {
                /* read line */
            pDeviceObject->reader(readline);

                p = strtok(readline, "=");
                if (!strncmp(p, "CHECKSUM", 8)) {
                    p = strtok(NULL, "\n");
//                      sscanf( p, "%x", (unsigned int*)&cfg->config_crc);
//                      mxt_dbg(ctx, "Config CRC from file: %s", tmp);
                  ignore_line = true;
                }
            }
            continue;
        }
            
        char indicator[15];

        if (sscanf(readline, "%s %s %d", item, indicator, &instance) != 3) {
//            mxt_err(ctx, "Instance parse error");
            return false;
        }

        /* read line */
            pDeviceObject->reader(readline);

        p = strtok(readline, "=");
        if (!strncmp(p, "OBJECT_ADDRESS", 14)) 
        {
            p = strtok(NULL, "\n");
            sscanf( p, "%x", (unsigned int*)&object_address);
//                      mxt_dbg(ctx, "Config CRC from file: %s", tmp);
        }
                       
        /* Find item type ID number at end of object string */
        substr = strrchr(item, '_');
        if (substr == NULL || (*(substr + 1) != 'T')) {
//              mxt_err(ctx, "Parse error, could not find T number in %s", tmp);
            return false;
        }
    
        if (sscanf(substr + 2, "%d", &type) != 1) {
//                mxt_err(ctx, "Unable to get object type ID for %s", tmp);
            return false;
        }
            
        /* read line */
            pDeviceObject->reader(readline);

        p = strtok(readline, "=");
        if (!strncmp(p, "OBJECT_SIZE", 11)) {
            p = strtok(NULL, "\n");
            sscanf( p, "%x", (unsigned int*)&size);
//                      mxt_dbg(ctx, "Config CRC from file: %s", tmp);
        }
                 
        if ( type != last_type )
        {
            object_num++;
            last_type = type;
        }

        object = mxt_get_object(pDeviceObject, (uint8_t)type);
        if (!object) {
            while (true)
            {
                /* read line */
            pDeviceObject->reader(readline);               

                /* End of object */
                if (readline[0] == '[')
                    break;
            }
            continue;
        }

        if (size > mxt_obj_size(object)) {
            /*
             * Either we are in fallback mode due to wrong
             * config or config from a later fw version,
             * or the file is corrupt or hand-edited.
             */
            dev_warn(dev, "Discarding %zu byte(s) in T%u\n",
                 size - mxt_obj_size(object), type);
        } else if (mxt_obj_size(object) > size) {
            /*
             * If firmware is upgraded, new bytes may be added to
             * end of objects. It is generally forward compatible
             * to zero these bytes - previous behaviour will be
             * retained. However this does invalidate the CRC and
             * will force fallback mode until the configuration is
             * updated. We warn here but do nothing else - the
             * malloc has zeroed the entire configuration.
             */
            dev_warn(dev, "Zeroing %zu byte(s) in T%d\n",
                 mxt_obj_size(object) - size, type);
        }
                            
        while (true)
        {
            int offset;
            int width;

            /* read line */
            pDeviceObject->reader(readline);             

            /* End of object */
            if (readline[0] == '[')
                break;

            /* End of file */
            if (readline[0] == EOF)
                break;

            if (sscanf(readline, "%d %d", &offset, &width) != 2) {
//              mxt_err(ctx, "Instance parse error");
                return false;
            }
                
            char * ptr = strtok(readline, "=");
            ptr = strtok(NULL, "\n");

            uint8_t byte;
            int scan;
            sscanf(ptr, "%d", &scan);
            byte = (uint8_t)scan;                

            if (instance >= mxt_obj_instances(object)) {
                dev_err(dev, "Object instances exceeded!\n");
                return false;
            }

            reg = object->start_address + mxt_obj_size(object) * instance;

            byte_offset = reg + offset - pDeviceObject->data.cfg_start_ofs;

            if (byte_offset >= 0 && byte_offset < pDeviceObject->data.config_mem_size) {
                *(pDeviceObject->data.config_mem + byte_offset) = byte;
            } else {
                dev_err(dev, "Bad object: reg:%d, T%d, ofs=%d\n",
                    reg, object->type, byte_offset);
                return false;
            }

            if ( pDeviceObject->eof() == true )
                break;

        }
                   
    }
    /* Calculate crc of the received configs (not the raw config file) */
    if (pDeviceObject->data.T7_address < pDeviceObject->data.cfg_start_ofs) 
    {
        dev_err(dev, "Bad T7 address, T7addr = %x, config offset %x\n",
            pDeviceObject->data.T7_address, pDeviceObject->data.cfg_start_ofs);
    }

    calculated_crc = mxt_calculate_crc(pDeviceObject->data.config_mem,
                       pDeviceObject->data.T71_address - pDeviceObject->data.cfg_start_ofs,
                       pDeviceObject->data.config_mem_size);

    if (config_crc > 0 && config_crc != calculated_crc)
        dev_warn(dev, "Config CRC error, calculated=%06X, file=%06X\n",
             calculated_crc, config_crc);

    return true;
}

/*
 * mxt_load_raw_flash - download configuration to chip
 *
 * Microchip Raw Config File Format
 *
 * The first four lines of the raw config file contain:
 *  1) Version
 *  2) Chip ID Information (first 7 bytes of device memory)
 *  3) Chip Information Block 24-bit CRC Checksum
 *  4) Chip Configuration 24-bit CRC Checksum
 *
 * The rest of the file consists of one line per object instance:
 *   <TYPE> <INSTANCE> <SIZE> <CONTENTS>
 *
 *   <TYPE> - 2-byte object type as hex
 *   <INSTANCE> - 2-byte object instance number as hex
 *   <SIZE> - 2-byte object size as hex
 *   <CONTENTS> - array of <SIZE> 1-byte hex values
 */
static bool mxt_load_raw_flash(struct DEVICE_OBJECT* pDeviceObject, char * flash)
{
	struct mxt_info cfg_info;
	int ret=0;
	uint32_t info_crc, config_crc, calculated_crc;
    
	mxt_update_crc(pDeviceObject, MXT_COMMAND_REPORTALL, 1);
    
    char * ptr = strtok(flash, "\n");
    
	if (strncmp(ptr, MXT_CFG_MAGIC, strlen(MXT_CFG_MAGIC))) {
		dev_err(dev, "Unrecognised config file\n");
		return false;
	}
	ptr = strtok(NULL, "\n");
    
	/* Load information block and check */
    ret = sscanf(ptr, "%2x %2x %2x %2x %2x %2x %2x",
            (unsigned int *)&cfg_info.family_id, (unsigned int *)&cfg_info.variant_id,
            (unsigned int *)&cfg_info.version, (unsigned int *)&cfg_info.build,
            (unsigned int *)&cfg_info.matrix_xsize, (unsigned int *)&cfg_info.matrix_ysize,
            (unsigned int *)&cfg_info.object_num
            );
        
    if (ret != sizeof(struct mxt_info)) {
        dev_err(dev, "Bad format\n");
        return false;
    }

    ptr = strtok(NULL, "\n");

	if (cfg_info.family_id != pDeviceObject->data.info.family_id) {
		dev_err(dev, "Family ID mismatch!\n");
		return false;
	}

	if (cfg_info.variant_id != pDeviceObject->data.info.variant_id) {
		dev_err(dev, "Variant ID mismatch!\n");
		return false;
	}

	/* Read CRCs */
	ret = sscanf(ptr, "%x", (unsigned int*)&info_crc);
	if (ret != 1) {
		dev_err(dev, "Bad format: failed to parse Info CRC\n");
		return false;
	}
    ptr = strtok(NULL, "\n");

	ret = sscanf(ptr, "%x", (unsigned int*)&config_crc);
	if (ret != 1) {
		dev_err(dev, "Bad format: failed to parse Config CRC\n");
		return false;
	}
    ptr = strtok(NULL, "\n");

	/*
	 * The Info Block CRC is calculated over mxt_info and the object
	 * table. If it does not match then we are trying to load the
	 * configuration from a different chip or firmware version, so
	 * the configuration CRC is invalid anyway.
	 */
	if (info_crc == pDeviceObject->data.info_crc) {
		if (config_crc == 0 || pDeviceObject->data.config_crc == 0) {
			dev_info(dev, "CRC zero, attempting to apply config\n");
		} else if (config_crc == pDeviceObject->data.config_crc) {
			dev_dbg(dev, "Config CRC 0x%06X: OK\n",
				 pDeviceObject->data.config_crc);
			return false;
		} else {
			dev_info(dev, "Config CRC 0x%06X: does not match file 0x%06X\n",
				 pDeviceObject->data.config_crc, config_crc);
		}
	} else {
		dev_warn(dev,
			 "Warning: Info CRC error - device=0x%06X file=0x%06X\n",
			 pDeviceObject->data.info_crc, info_crc);
	}
    
	/* Malloc memory to store configuration */
	pDeviceObject->data.cfg_start_ofs = MXT_OBJECT_START +
			pDeviceObject->data.info.object_num * sizeof(struct mxt_object) +
			MXT_INFO_CHECKSUM_SIZE;
	pDeviceObject->data.config_mem_size = pDeviceObject->data.mem_size - pDeviceObject->data.cfg_start_ofs;
    
	mxt_prepare_cfg_mem(pDeviceObject, ptr, pDeviceObject->data.cfg_start_ofs,
				  pDeviceObject->data.config_mem, pDeviceObject->data.config_mem_size);

	/* Calculate crc of the received configs (not the raw config file) */
	if (pDeviceObject->data.T7_address < pDeviceObject->data.cfg_start_ofs) {
		dev_err(dev, "Bad T7 address, T7addr = %x, config offset %x\n",
			pDeviceObject->data.T7_address, pDeviceObject->data.cfg_start_ofs);
		return false;
    }

	calculated_crc = mxt_calculate_crc(pDeviceObject->data.config_mem,
					   pDeviceObject->data.T71_address - pDeviceObject->data.cfg_start_ofs,
					   pDeviceObject->data.config_mem_size);
            
	if (config_crc > 0 && config_crc != calculated_crc)
		dev_warn(dev, "Config CRC error, calculated=%06X, file=%06X\n",
			 calculated_crc, config_crc);

	return false;
}


bool mxt_load_raw_file(struct DEVICE_OBJECT* pDeviceObject, char * cfg)
{
	struct mxt_info cfg_info;
	int ret=0;
	uint32_t info_crc, config_crc, calculated_crc;
    
	mxt_update_crc(pDeviceObject, MXT_COMMAND_REPORTALL, 1);
        
    char * ptr = strtok((char*)cfg, "\n");
    
	if (strncmp(ptr, MXT_CFG_MAGIC, strlen(MXT_CFG_MAGIC))) {
		dev_err(dev, "Unrecognised config file\n");
		return false;
	}
	ptr = strtok(NULL, "\n");
    
	/* Load information block and check */
    ret = sscanf(ptr, "%2x %2x %2x %2x %2x %2x %2x",
            (unsigned int *)&cfg_info.family_id, (unsigned int *)&cfg_info.variant_id,
            (unsigned int *)&cfg_info.version, (unsigned int *)&cfg_info.build,
            (unsigned int *)&cfg_info.matrix_xsize, (unsigned int *)&cfg_info.matrix_ysize,
            (unsigned int *)&cfg_info.object_num
            );
        
    if (ret != sizeof(struct mxt_info)) {
        dev_err(dev, "Bad format\n");
        return false;
    }

    ptr = strtok(NULL, "\n");

	if (cfg_info.family_id != pDeviceObject->data.info.family_id) {
		dev_err(dev, "Family ID mismatch!\n");
		return false;
	}

	if (cfg_info.variant_id != pDeviceObject->data.info.variant_id) {
		dev_err(dev, "Variant ID mismatch!\n");
		return false;
	}

	/* Read CRCs */
	ret = sscanf(ptr, "%x", (unsigned int*)&info_crc);
	if (ret != 1) {
		dev_err(dev, "Bad format: failed to parse Info CRC\n");
		return false;
	}
    ptr = strtok(NULL, "\n");

	ret = sscanf(ptr, "%x", (unsigned int*)&config_crc);
	if (ret != 1) {
		dev_err(dev, "Bad format: failed to parse Config CRC\n");
		return false;
	}
    ptr = strtok(NULL, "\n");

	/*
	 * The Info Block CRC is calculated over mxt_info and the object
	 * table. If it does not match then we are trying to load the
	 * configuration from a different chip or firmware version, so
	 * the configuration CRC is invalid anyway.
	 */
	if (info_crc == pDeviceObject->data.info_crc) {
		if (config_crc == 0 || pDeviceObject->data.config_crc == 0) {
			dev_info(dev, "CRC zero, attempting to apply config\n");
		} else if (config_crc == pDeviceObject->data.config_crc) {
			dev_dbg(dev, "Config CRC 0x%06X: OK\n",
				 pDeviceObject->data.config_crc);
			return false;
		} else {
			dev_info(dev, "Config CRC 0x%06X: does not match file 0x%06X\n",
				 pDeviceObject->data.config_crc, config_crc);
		}
	} else {
		dev_warn(dev,
			 "Warning: Info CRC error - device=0x%06X file=0x%06X\n",
			 pDeviceObject->data.info_crc, info_crc);
	}
    
	/* Malloc memory to store configuration */
	pDeviceObject->data.cfg_start_ofs = MXT_OBJECT_START +
			pDeviceObject->data.info.object_num * sizeof(struct mxt_object) +
			MXT_INFO_CHECKSUM_SIZE;
	pDeviceObject->data.config_mem_size = pDeviceObject->data.mem_size - pDeviceObject->data.cfg_start_ofs;
    
	mxt_prepare_cfg_mem(pDeviceObject, ptr, pDeviceObject->data.cfg_start_ofs,
				  pDeviceObject->data.config_mem, pDeviceObject->data.config_mem_size);

	/* Calculate crc of the received configs (not the raw config file) */
	if (pDeviceObject->data.T7_address < pDeviceObject->data.cfg_start_ofs) {
		dev_err(dev, "Bad T7 address, T7addr = %x, config offset %x\n",
			pDeviceObject->data.T7_address, pDeviceObject->data.cfg_start_ofs);
		return false;
    }

	calculated_crc = mxt_calculate_crc(pDeviceObject->data.config_mem,
					   pDeviceObject->data.T71_address - pDeviceObject->data.cfg_start_ofs,
					   pDeviceObject->data.config_mem_size);
            
	if (config_crc > 0 && config_crc != calculated_crc)
		dev_warn(dev, "Config CRC error, calculated=%06X, file=%06X\n",
			 calculated_crc, config_crc);

	return false;
}

static inline uint16_t get_unaligned_le16(const void *p)
{
	const uint8_t *_p = p;
	return _p[0] | _p[1] << 8;
}

static void mxt_proc_t100_message(struct mxt_data *data, uint8_t *message)
{
	int id;
	uint8_t status;
	uint8_t type = 0;
	uint16_t x;
	uint16_t y;
//	int distance = 0;
//	int tool = 0;
//	uint8_t major = 0;
//	uint8_t pressure = 0;
//	uint8_t orientation = 0;

	id = message[0] - data->T100_reportid_min - 2;

	/* ignore SCRSTATUS events */
	if (id < 0)
		return;

	status = message[1];
	x = get_unaligned_le16(&message[2]);
	y = get_unaligned_le16(&message[4]);

	if (status & MXT_T100_DETECT) {
		type = (status & MXT_T100_TYPE_MASK) >> 4;

		switch (type) {
		case MXT_T100_TYPE_HOVERING_FINGER:
//			tool = MT_TOOL_FINGER;
//			distance = MXT_DISTANCE_HOVERING;

//			if (data->t100_aux_vect)
//				orientation = message[data->t100_aux_vect];

			break;

		case MXT_T100_TYPE_FINGER:
		case MXT_T100_TYPE_GLOVE:
//			tool = MT_TOOL_FINGER;
//			distance = MXT_DISTANCE_ACTIVE_TOUCH;

//			if (data->t100_aux_area)
//				major = message[data->t100_aux_area];
//
//			if (data->t100_aux_ampl)
//				pressure = message[data->t100_aux_ampl];
//
//			if (data->t100_aux_vect)
//				orientation = message[data->t100_aux_vect];

			break;

		case MXT_T100_TYPE_PASSIVE_STYLUS:
//			tool = MT_TOOL_PEN;

			/*
			 * Passive stylus is reported with size zero so
			 * hardcode.
			 */
//			major = MXT_TOUCH_MAJOR_DEFAULT;
//
//			if (data->t100_aux_ampl)
//				pressure = message[data->t100_aux_ampl];

			break;

		case MXT_T100_TYPE_LARGE_TOUCH:
			/* Ignore suppressed touch */
			break;

		default:
			dev_dbg(dev, "Unexpected T100 type\n");
			return;
		}
	}

//	/*
//	 * Values reported should be non-zero if tool is touching the
//	 * device
//	 */
//	if (!pressure && type != MXT_T100_TYPE_HOVERING_FINGER)
//		pressure = MXT_PRESSURE_DEFAULT;

    uint8_t event;
    event = status & 0xF;

    switch(event)
    {
        case 0x4: // touch down
        {            
            SYS_INP_InjectTouchDown(id, x, y);
            
            break;
        }
        case 0x5: // touch up
        {            
            SYS_INP_InjectTouchUp(id, x, y);
            
            break;
        }
        case 0x1: // touch move
        {            
            SYS_INP_InjectTouchMove(id, x, y);
            
            break;
        }
    }
        
	data->update_input = true;
}

bool mxt_proc_message(struct mxt_data *data, uint8_t *message)
{
	uint8_t report_id = message[0];

	if (report_id == MXT_RPTID_NOMSG)
		return false;

//	if (report_id == data->T6_reportid) {
//		mxt_proc_t6_messages(data, message);
//	} else if (!data->input_dev) {
//		/*
//		 * Do not report events if input device
//		 * is not yet registered.
//		 */
//		mxt_dump_message(data, message);
//	} else if (report_id >= data->T9_reportid_min &&
//		   report_id <= data->T9_reportid_max) {
//		mxt_proc_t9_message(data, message);
//	} else if (report_id >= data->T100_reportid_min &&
//		   report_id <= data->T100_reportid_max) {
		mxt_proc_t100_message(data, message);
//	} else if (report_id == data->T19_reportid) {
//		mxt_input_button(data, message);
//		data->update_input = true;
//	} else {
//		mxt_dump_message(data, message);
//	}

	return true;
}
