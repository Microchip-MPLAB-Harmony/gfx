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


#include "driver/input/drv_maxtouch.h"
#include "definitions.h"
#include "driver/i2c/drv_i2c.h"
#include "system/input/sys_input.h"
#include "system/time/sys_time.h"

#include <string.h>
#include <stdio.h>

#define DEFAULT_XRES               479
#define DEFAULT_YRES               271

#define	ENOMEM		12	/* Out of Memory */
#define	EINVAL		22	/* Invalid argument */
#define ENOSPC		28	/* No space left on device */
#define	EBUSY		16	/* Device or resource busy */

/*****************************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

typedef enum
{
    /* Driver Initialize state */
    DEVICE_STATE_OPEN = 0,
            
    DEVICE_STATE_INIT,
                        
    /* reset the address */
    DEVICE_STATE_INIT_RESET,        
                        
    /* Driver ready state */
    DEVICE_STATE_READY,   
                     
    DEVICE_STATE_WAIT,
            
    DEVICE_STATE_MXT_INITIALIZE,
            
    /* In error state */        
    DEVICE_STATE_ERROR,
            
    DEVICE_POR,
            
} MXT_DEVICE_STATE;

typedef enum
{
    /* Application's API event states. */
    APP_DRV_MAXTOUCH_None=0,
    APP_DRV_MAXTOUCH_ResetCallback,
    /* TODO: Define states used by the application state machine. */

} API_EVENTS;

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


/* Firmware files */
#define MXT_FW_NAME         "maxtouch.fw"
#define MXT_CFG_NAME		"maxtouch.cfg"
#define MXT_CFG_MAGIC		"OBP_RAW V1"

#define	EIO                 5	/* I/O error */

/* Registers */
#define MXT_OBJECT_START        0x07
#define MXT_OBJECT_SIZE         6
#define MXT_INFO_CHECKSUM_SIZE	3
#define MXT_MAX_BLOCK_WRITE     254

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

/* MXT_GEN_MESSAGE_T5 object */
#define MXT_RPTID_NOMSG                 0xff

/* MXT_GEN_COMMAND_T6 field */
#define MXT_COMMAND_RESET       0
#define MXT_COMMAND_BACKUPNV	1
#define MXT_COMMAND_CALIBRATE	2
#define MXT_COMMAND_REPORTALL	3
#define MXT_COMMAND_DIAGNOSTIC	5

/* Define for T6 status byte */
#define MXT_T6_STATUS_RESET     (1 << 7)
#define MXT_T6_STATUS_OFL       (1 << 6)
#define MXT_T6_STATUS_SIGERR	(1 << 5)
#define MXT_T6_STATUS_CAL       (1 << 4)
#define MXT_T6_STATUS_CFGERR	(1 << 3)
#define MXT_T6_STATUS_COMSERR	(1 << 2)

/* MXT_GEN_POWER_T7 field */
struct t7_config {
	uint8_t idle;
	uint8_t active;
} __attribute__((packed));

#define MXT_POWER_CFG_RUN           0
#define MXT_POWER_CFG_DEEPSLEEP		1

/* MXT_TOUCH_MULTI_T9 field */
#define MXT_T9_CTRL         0
#define MXT_T9_XSIZE		3
#define MXT_T9_YSIZE		4
#define MXT_T9_ORIENT		9
#define MXT_T9_RANGE		18

/* MXT_TOUCH_MULTI_T9 status */
#define MXT_T9_UNGRIP		(1 << 0)
#define MXT_T9_SUPPRESS		(1 << 1)
#define MXT_T9_AMP          (1 << 2)
#define MXT_T9_VECTOR		(1 << 3)
#define MXT_T9_MOVE         (1 << 4)
#define MXT_T9_RELEASE		(1 << 5)
#define MXT_T9_PRESS		(1 << 6)
#define MXT_T9_DETECT		(1 << 7)

struct t9_range {
	uint16_t x;
	uint16_t y;
} __attribute__((packed));

/* MXT_TOUCH_MULTI_T9 orient */
#define MXT_T9_ORIENT_SWITCH	(1 << 0)
#define MXT_T9_ORIENT_INVERTX	(1 << 1)
#define MXT_T9_ORIENT_INVERTY	(1 << 2)

/* MXT_SPT_COMMSCONFIG_T18 */
#define MXT_COMMS_CTRL		0
#define MXT_COMMS_CMD		1

/* MXT_DEBUG_DIAGNOSTIC_T37 */
#define MXT_DIAGNOSTIC_PAGEUP	0x01
#define MXT_DIAGNOSTIC_DELTAS	0x10
#define MXT_DIAGNOSTIC_REFS     0x11
#define MXT_DIAGNOSTIC_SIZE     128

#define MXT_FAMILY_1386             160
#define MXT1386_COLUMNS             3
#define MXT1386_PAGES_PER_COLUMN	8

struct t37_debug {
#ifdef CONFIG_TOUCHSCREEN_MXT_T37
	uint8_t mode;
	uint8_t page;
	uint8_t data[MXT_DIAGNOSTIC_SIZE];
#endif
};

/* Define for MXT_GEN_COMMAND_T6 */
#define MXT_BOOT_VALUE		0xa5
#define MXT_RESET_VALUE		0x01
#define MXT_BACKUP_VALUE	0x55

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

/* Delay times */
#define MXT_BACKUP_TIME         50      /* msec */
#define MXT_RESET_GPIO_TIME     20      /* msec */
#define MXT_RESET_INVALID_CHG	100     /* msec */
#define MXT_RESET_TIME          200     /* msec */
#define MXT_RESET_TIMEOUT       3000	/* msec */
#define MXT_CRC_TIMEOUT         1000	/* msec */
#define MXT_FW_RESET_TIME       3000	/* msec */
#define MXT_FW_CHG_TIMEOUT      300     /* msec */

/* Command to unlock bootloader */
#define MXT_UNLOCK_CMD_MSB	0xaa
#define MXT_UNLOCK_CMD_LSB	0xdc

/* Bootloader mode status */
#define MXT_WAITING_BOOTLOAD_CMD	0xc0	/* valid 7 6 bit only */
#define MXT_WAITING_FRAME_DATA      0x80	/* valid 7 6 bit only */
#define MXT_FRAME_CRC_CHECK         0x02
#define MXT_FRAME_CRC_FAIL          0x03
#define MXT_FRAME_CRC_PASS          0x04
#define MXT_APP_CRC_FAIL            0x40	/* valid 7 8 bit only */
#define MXT_BOOT_STATUS_MASK        0x3f
#define MXT_BOOT_EXTENDED_ID        (1 << 5)
#define MXT_BOOT_ID_MASK            0x1f

/* Touchscreen absolute values */
#define MXT_MAX_AREA		0xff

#define MXT_PIXELS_PER_MM	20

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

/* Each client has this additional data */
struct mxt_data {
	struct i2c_client *client;
	struct input_dev *input_dev;
	char phys[64];		/* device physical location */
	struct mxt_object *object_table;
	struct mxt_info *info;
	void *raw_info_block;
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
	uint8_t *msg_buf;
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
    
    unsigned int xRes;
    unsigned int yRes;

    SYS_STATUS status;
    MXT_DEVICE_STATE deviceState;
    bool isExclusive;
    API_EVENTS apiEvent;
};

enum irqreturn {
	IRQ_NONE            = (0 << 0),
	IRQ_HANDLED         = (1 << 0),
	IRQ_WAKE_THREAD		= (1 << 1),
};

struct input_dev {
	const char *name;
};

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

struct firmware {
	size_t size;
    const uint8_t *data;
};

static void mxt_config_cb(const struct firmware *cfg, void *ctx);
static int mxt_initialize(struct mxt_data *data);
static int mxt_interrupt(struct mxt_data *data);
static int mxt_init_t7_power_cfg(struct mxt_data *data);

static void _mxt_DelayMS(int ms)
{
	SYS_TIME_HANDLE timer = SYS_TIME_HANDLE_INVALID;

	if (SYS_TIME_DelayMS(ms, &timer) != SYS_TIME_SUCCESS)
	return;
	
	while (SYS_TIME_DelayIsComplete(timer) == false);
} 

static SYS_TIME_HANDLE resetTimer;

static void resetTimer_Callback ( uintptr_t context )
{
    struct mxt_data * obj = (struct mxt_data *) context;
    
    switch ( obj->apiEvent )
    {          
        case APP_DRV_MAXTOUCH_ResetCallback:
            obj->deviceState = DEVICE_STATE_MXT_INITIALIZE;
            break;
            
        default:
            break;
    }
    SYS_TIME_TimerDestroy(resetTimer);
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
//******************************************************************************

static uint32_t MXT_INTERRUPT_PIN_VALUE_GET(void)
{
#ifndef BSP_MAXTOUCH_CHG_Get
#error " BSP_MAXTOUCH_CHG_Get is not defined. Please use Pin Settings Tab in MHC to define the Touch Interrupt pin with the name 'MAXTOUCH_INT' as GPIO_IN"   
#else
return(BSP_MAXTOUCH_CHG_Get());
#endif
/* temporary CHG pin read REMOVE END */
}


<#if ChgMode == "Interrupt">
<#if __PROCESSOR?matches("PIC32M.*") == false>
void mxt_chg_callback ( PIO_PIN pin, uintptr_t context)
<#else>
void mxt_chg_callback ( GPIO_PIN pin, uintptr_t context)
</#if>
{
    struct mxt_data * obj = (struct mxt_data *) context;
    obj->deviceState = DEVICE_STATE_READY;
}
</#if>

// *****************************************************************************

///* MAXTOUCH Driver instance object */
struct mxt_data sMAXTOUCHDriverInstances[DRV_MAXTOUCH_INDEX_COUNT];
struct mxt_data * pDrvInstance;

//DRV_I2C_TRANSFER_SETUP i2cSetup;


SYS_MODULE_OBJ DRV_MAXTOUCH_Initialize(const SYS_MODULE_INDEX index,
                                      const SYS_MODULE_INIT * const init)
{
    const DRV_MAXTOUCH_INIT *pInit = NULL;
    
    if ( index >= DRV_MAXTOUCH_INDEX_COUNT )
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Attempting to initialize an instance number greater than the max");
        return SYS_MODULE_OBJ_INVALID;
    }

    pDrvInstance = ( struct mxt_data *)&sMAXTOUCHDriverInstances[index];

    if ( pDrvInstance->inUse == true )
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Attempting to reinitialize a driver instance that is already in use");
        return SYS_MODULE_OBJ_INVALID;
    }

    pInit = (const DRV_MAXTOUCH_INIT* const)init;

    /* */
    pDrvInstance->inUse   = true;
    pDrvInstance->drvOpen = pInit->drvOpen;
    pDrvInstance->xRes    = pInit->horizontalResolution - 1;
    pDrvInstance->yRes    = pInit->verticalResolution - 1;
    pDrvInstance->status  = SYS_STATUS_BUSY;
    pDrvInstance->deviceState = DEVICE_STATE_OPEN;

    <#if ChgMode == "Interrupt">
    /* enable CHG interrupt handler */
    <#if __PROCESSOR?matches("PIC32M.*") == false>
    PIO_PinInterruptCallbackRegister(BSP_MAXTOUCH_CHG_PIN, mxt_chg_callback, (uintptr_t)pDrvInstance);
    PIO_PinInterruptEnable(BSP_MAXTOUCH_CHG_PIN);
    <#else>
    GPIO_PinInterruptCallbackRegister(BSP_MAXTOUCH_CHG_PIN, mxt_chg_callback, (uintptr_t)pDrvInstance);
    GPIO_PinInterruptEnable(BSP_MAXTOUCH_CHG_PIN);
    </#if>
    </#if>

    return (SYS_MODULE_OBJ)pDrvInstance;
}

void DRV_MAXTOUCH_Deinitialize ( SYS_MODULE_OBJ object )
{
    struct mxt_data * pDrvInstance = (struct mxt_data *)object;
        
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

    mxt_config_cb(0,0);
    
    pDrvInstance->inUse                 = false;
    pDrvInstance->status                = SYS_STATUS_UNINITIALIZED;

    return;
}

SYS_STATUS DRV_MAXTOUCH_Status(SYS_MODULE_OBJ object)
{
    struct mxt_data * pDrvInstance = (struct mxt_data *)object;
        
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
    
    if (index >= DRV_MAXTOUCH_INDEX_COUNT)
    {
        SYS_ASSERT(false, "MaxTouch Driver: Attempting to open an instance" \
                          "number greater than the max");
        return DRV_HANDLE_INVALID;
    }
        
	pDrvInstance->client = OSAL_Malloc(sizeof(struct i2c_client));
    
    /* Open the bus driver */
    if(pDrvInstance->drvOpen == NULL)
    {
        SYS_ASSERT(false, "MaxTouch Driver: Bus driver init parameter missing");
        return DRV_HANDLE_INVALID;
    }
        
    pDrvInstance->client->drvI2CHandle = DRV_I2C_Open( ${DRV_MAXTOUCH_I2C_INDEX}, DRV_IO_INTENT_READWRITE);
    
    if(pDrvInstance->client->drvI2CHandle == DRV_HANDLE_INVALID)
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Bus driver initialization failed");
        return DRV_HANDLE_INVALID;
    }

    if ((intent & DRV_IO_INTENT_EXCLUSIVE) == DRV_IO_INTENT_EXCLUSIVE)
    {
        pDrvInstance->isExclusive = true;
    }         
            
    pDrvInstance->client->addr = 0x4A;
    
    pDrvInstance->status = SYS_STATUS_BUSY;  
    
    return (DRV_HANDLE)pDrvInstance;
}

void DRV_MAXTOUCH_Close (DRV_HANDLE handle)
{
    struct mxt_data * pDrvObject = (struct mxt_data *)handle;

    if(pDrvObject == NULL)
    {
        SYS_ASSERT(false, "MAXTOUCH Driver: Trying to close a client with invalid driver object");
        
        return;
    }
    
    /* move driver to the idle state to stop any processes */
    pDrvObject->deviceState = DEVICE_STATE_INIT;    
    
}

void DRV_MAXTOUCH_Tasks ( SYS_MODULE_OBJ object )
{
    struct mxt_data * pDrvObject = (struct mxt_data *)object;
        
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
        case DEVICE_STATE_MXT_INITIALIZE: /* Request information block */
        {            
            mxt_initialize(pDrvInstance);                         
            
            pDrvInstance->status = SYS_STATUS_READY;  
            pDrvObject->deviceState = DEVICE_STATE_READY;
            break;
        }
                
        case DEVICE_STATE_READY: /* Read information block */
        {
      
			/* Check for ~CHG line asserted in case interrupt line did not come back up high */
            /* send a read request to the message processor object T5 */ 
            if(MXT_INTERRUPT_PIN_VALUE_GET() == false)
            {
                mxt_interrupt(pDrvInstance);  
            }
            <#if ChgMode == "Interrupt">
            pDrvObject->deviceState = DEVICE_STATE_WAIT;
            </#if>
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

static inline void input_report_abs(struct input_dev *dev, unsigned int code, int value)
{
}

bool input_mt_report_slot_state(struct input_dev *dev,
				unsigned int tool_type, bool active)
{
    return true;
}

static inline void input_mt_slot(struct input_dev *dev, int slot)
{
}

int input_mt_init_slots(struct input_dev *dev, unsigned int num_slots,
			unsigned int flags)
{   
    return 0;
}

static inline uint16_t get_unaligned_le16(const void *p)
{
	const uint8_t *_p = p;
	return _p[0] | _p[1] << 8;
}

//
static size_t mxt_obj_size(const struct mxt_object *obj)
{
	return obj->size_minus_one + 1;
}

static size_t mxt_obj_instances(const struct mxt_object *obj)
{
	return obj->instances_minus_one + 1;
}

//static bool mxt_object_readable(unsigned int type)
//{
//	switch (type) {
//	case MXT_GEN_COMMAND_T6:
//	case MXT_GEN_POWER_T7:
//	case MXT_GEN_ACQUIRE_T8:
//	case MXT_GEN_DATASOURCE_T53:
//	case MXT_TOUCH_MULTI_T9:
//	case MXT_TOUCH_KEYARRAY_T15:
//	case MXT_TOUCH_PROXIMITY_T23:
//	case MXT_TOUCH_PROXKEY_T52:
//	case MXT_TOUCH_MULTITOUCHSCREEN_T100:
//	case MXT_PROCI_GRIPFACE_T20:
//	case MXT_PROCG_NOISE_T22:
//	case MXT_PROCI_ONETOUCH_T24:
//	case MXT_PROCI_TWOTOUCH_T27:
//	case MXT_PROCI_GRIP_T40:
//	case MXT_PROCI_PALM_T41:
//	case MXT_PROCI_TOUCHSUPPRESSION_T42:
//	case MXT_PROCI_STYLUS_T47:
//	case MXT_PROCG_NOISESUPPRESSION_T48:
//	case MXT_SPT_COMMSCONFIG_T18:
//	case MXT_SPT_GPIOPWM_T19:
//	case MXT_SPT_SELFTEST_T25:
//	case MXT_SPT_CTECONFIG_T28:
//	case MXT_SPT_USERDATA_T38:
//	case MXT_SPT_DIGITIZER_T43:
//	case MXT_SPT_CTECONFIG_T46:
//		return true;
//	default:
//		return false;
//	}
//}

static void mxt_dump_message(struct mxt_data *data, uint8_t *message)
{
//	dev_dbg(&data->client->dev, "message: %*ph\n",
//		data->T5_msg_size, message);
}

//static int mxt_wait_for_completion(struct mxt_data *data,
//				   struct completion *comp,
//				   unsigned int timeout_ms)
//{
//	struct device *dev = &data->client->dev;
//	unsigned long timeout = msecs_to_jiffies(timeout_ms);
//	long ret;
//
//	ret = wait_for_completion_interruptible_timeout(comp, timeout);
//	if (ret < 0) {
//		return ret;
//	} else if (ret == 0) {
//		dev_err(dev, "Wait for completion timed out.\n");
//		return -ETIMEDOUT;
//	}
//	return 0;
//}
//
//static int mxt_bootloader_read(struct mxt_data *data,
//			       u8 *val, unsigned int count)
//{
//	int ret;
//	struct i2c_msg msg;
//
//	msg.addr = data->bootloader_addr;
//	msg.flags = data->client->flags & I2C_M_TEN;
//	msg.flags |= I2C_M_RD;
//	msg.len = count;
//	msg.buf = val;
//
//	ret = i2c_transfer(data->client->adapter, &msg, 1);
//	if (ret == 1) {
//		ret = 0;
//	} else {
//		ret = ret < 0 ? ret : -EIO;
//		dev_err(&data->client->dev, "%s: i2c recv failed (%d)\n",
//			__func__, ret);
//	}
//
//	return ret;
//}
//
//static int mxt_bootloader_write(struct mxt_data *data,
//				const u8 * const val, unsigned int count)
//{
//	int ret;
//	struct i2c_msg msg;
//
//	msg.addr = data->bootloader_addr;
//	msg.flags = data->client->flags & I2C_M_TEN;
//	msg.len = count;
//	msg.buf = (u8 *)val;
//
//	ret = i2c_transfer(data->client->adapter, &msg, 1);
//	if (ret == 1) {
//		ret = 0;
//	} else {
//		ret = ret < 0 ? ret : -EIO;
//		dev_err(&data->client->dev, "%s: i2c send failed (%d)\n",
//			__func__, ret);
//	}
//
//	return ret;
//}
//
//static int mxt_lookup_bootloader_address(struct mxt_data *data, bool retry)
//{
//	u8 appmode = data->client->addr;
//	u8 bootloader;
//	u8 family_id = data->info ? data->info->family_id : 0;
//
//	switch (appmode) {
//	case 0x4a:
//	case 0x4b:
//		/* Chips after 1664S use different scheme */
//		if (retry || family_id >= 0xa2) {
//			bootloader = appmode - 0x24;
//			break;
//		}
//		/* Fall through for normal case */
//	case 0x4c:
//	case 0x4d:
//	case 0x5a:
//	case 0x5b:
//		bootloader = appmode - 0x26;
//		break;
//
//	default:
//		dev_err(&data->client->dev,
//			"Appmode i2c address 0x%02x not found\n",
//			appmode);
//		return -EINVAL;
//	}
//
//	data->bootloader_addr = bootloader;
//	return 0;
//}
//
//static int mxt_probe_bootloader(struct mxt_data *data, bool alt_address)
//{
//	struct device *dev = &data->client->dev;
//	int error;
//	u8 val;
//	bool crc_failure;
//
//	error = mxt_lookup_bootloader_address(data, alt_address);
//	if (error)
//		return error;
//
//	error = mxt_bootloader_read(data, &val, 1);
//	if (error)
//		return error;
//
//	/* Check app crc fail mode */
//	crc_failure = (val & ~MXT_BOOT_STATUS_MASK) == MXT_APP_CRC_FAIL;
//
//	dev_err(dev, "Detected bootloader, status:%02X%s\n",
//			val, crc_failure ? ", APP_CRC_FAIL" : "");
//
//	return 0;
//}
//
//static u8 mxt_get_bootloader_version(struct mxt_data *data, u8 val)
//{
//	struct device *dev = &data->client->dev;
//	u8 buf[3];
//
//	if (val & MXT_BOOT_EXTENDED_ID) {
//		if (mxt_bootloader_read(data, &buf[0], 3) != 0) {
//			dev_err(dev, "%s: i2c failure\n", __func__);
//			return val;
//		}
//
//		dev_dbg(dev, "Bootloader ID:%d Version:%d\n", buf[1], buf[2]);
//
//		return buf[0];
//	} else {
//		dev_dbg(dev, "Bootloader ID:%d\n", val & MXT_BOOT_ID_MASK);
//
//		return val;
//	}
//}
//
//static int mxt_check_bootloader(struct mxt_data *data, unsigned int state,
//				bool wait)
//{
//	struct device *dev = &data->client->dev;
//	u8 val;
//	int ret;
//
//recheck:
//	if (wait) {
//		/*
//		 * In application update mode, the interrupt
//		 * line signals state transitions. We must wait for the
//		 * CHG assertion before reading the status byte.
//		 * Once the status byte has been read, the line is deasserted.
//		 */
//		ret = mxt_wait_for_completion(data, &data->bl_completion,
//					      MXT_FW_CHG_TIMEOUT);
//		if (ret) {
//			/*
//			 * TODO: handle -ERESTARTSYS better by terminating
//			 * fw update process before returning to userspace
//			 * by writing length 0x000 to device (iff we are in
//			 * WAITING_FRAME_DATA state).
//			 */
//			dev_err(dev, "Update wait error %d\n", ret);
//			return ret;
//		}
//	}
//
//	ret = mxt_bootloader_read(data, &val, 1);
//	if (ret)
//		return ret;
//
//	if (state == MXT_WAITING_BOOTLOAD_CMD)
//		val = mxt_get_bootloader_version(data, val);
//
//	switch (state) {
//	case MXT_WAITING_BOOTLOAD_CMD:
//	case MXT_WAITING_FRAME_DATA:
//	case MXT_APP_CRC_FAIL:
//		val &= ~MXT_BOOT_STATUS_MASK;
//		break;
//	case MXT_FRAME_CRC_PASS:
//		if (val == MXT_FRAME_CRC_CHECK) {
//			goto recheck;
//		} else if (val == MXT_FRAME_CRC_FAIL) {
//			dev_err(dev, "Bootloader CRC fail\n");
//			return -EINVAL;
//		}
//		break;
//	default:
//		return -EINVAL;
//	}
//
//	if (val != state) {
//		dev_err(dev, "Invalid bootloader state %02X != %02X\n",
//			val, state);
//		return -EINVAL;
//	}
//
//	return 0;
//}
//
//static int mxt_send_bootloader_cmd(struct mxt_data *data, bool unlock)
//{
//	int ret;
//	u8 buf[2];
//
//	if (unlock) {
//		buf[0] = MXT_UNLOCK_CMD_LSB;
//		buf[1] = MXT_UNLOCK_CMD_MSB;
//	} else {
//		buf[0] = 0x01;
//		buf[1] = 0x01;
//	}
//
//	ret = mxt_bootloader_write(data, buf, 2);
//	if (ret)
//		return ret;
//
//	return 0;
//}

static int __mxt_read_reg(struct i2c_client *client,
			       uint16_t reg, uint16_t len, void *val)
{
	uint8_t buf[2];
	int ret;

	buf[0] = reg & 0xff;
	buf[1] = (reg >> 8) & 0xff;
    
    ret = DRV_I2C_WriteReadTransfer ( client->drvI2CHandle, client->addr, buf, 2, val, len);
    ret = 0;

	return ret;
}

static int __mxt_write_reg(struct i2c_client *client, uint16_t reg, uint16_t len,
			   const void *val)
{
	uint8_t *buf;
	size_t count;

	count = len + 2;
	buf = OSAL_Malloc(count);
	if (!buf)
		return -ENOMEM;

	buf[0] = reg & 0xff;
	buf[1] = (reg >> 8) & 0xff;
	memcpy(&buf[2], val, len);

    DRV_I2C_WriteTransfer( client->drvI2CHandle, client->addr, buf, count);

	OSAL_Free(buf);
	return 0;
}

static int mxt_write_reg(struct i2c_client *client, uint16_t reg, uint8_t val)
{
	return __mxt_write_reg(client, reg, 1, &val);
}

static struct mxt_object *
mxt_get_object(struct mxt_data *data, uint8_t type)
{
	struct mxt_object *object;
	int i;

	for (i = 0; i < data->info->object_num; i++) {
		object = data->object_table + i;
		if (object->type == type)
			return object;
	}

	return NULL;
}

static void mxt_proc_t6_messages(struct mxt_data *data, uint8_t *msg)
{
//	struct device *dev = &data->client->dev;
	uint8_t status = msg[1];
	uint32_t crc = msg[2] | (msg[3] << 8) | (msg[4] << 16);

//	complete(&data->crc_completion);

	if (crc != data->config_crc) {
		data->config_crc = crc;
		dev_dbg(dev, "T6 Config Checksum: 0x%06X\n", crc);
	}

	/* Detect reset */
//	if (status & MXT_T6_STATUS_RESET)
//		complete(&data->reset_completion);

	/* Output debug if status has changed */
	if (status != data->t6_status)
		dev_dbg(dev, "T6 Status 0x%02X%s%s%s%s%s%s%s\n",
			status,
			status == 0 ? " OK" : "",
			status & MXT_T6_STATUS_RESET ? " RESET" : "",
			status & MXT_T6_STATUS_OFL ? " OFL" : "",
			status & MXT_T6_STATUS_SIGERR ? " SIGERR" : "",
			status & MXT_T6_STATUS_CAL ? " CAL" : "",
			status & MXT_T6_STATUS_CFGERR ? " CFGERR" : "",
			status & MXT_T6_STATUS_COMSERR ? " COMSERR" : "");

	/* Save current status */
	data->t6_status = status;
}

//static int mxt_write_object(struct mxt_data *data,
//				 uint8_t type, uint8_t offset, uint8_t val)
//{
//	struct mxt_object *object;
//	uint32_t reg;
//
//	object = mxt_get_object(data, type);
//	if (!object || offset >= mxt_obj_size(object))
//		return -EINVAL;
//
//	reg = object->start_address;
//	return mxt_write_reg(data->client, reg + offset, val);
//}

static void mxt_input_button(struct mxt_data *data, uint8_t *message)
{
//	struct input_dev *input = data->input_dev;
	int i;

	for (i = 0; i < data->t19_num_keys; i++) {
		if (data->t19_keymap[i] == KEY_RESERVED)
			continue;

		/* Active-low switch */
//		input_report_key(input, data->t19_keymap[i],
//				 !(message[1] & BIT(i)));
	}
}

static void mxt_input_sync(struct mxt_data *data)
{
//	input_mt_report_pointer_emulation(data->input_dev,
//					  data->t19_num_keys);
//	input_sync(data->input_dev);
}

static void mxt_proc_t9_message(struct mxt_data *data, uint8_t *message)
{
//	struct device *dev = &data->client->dev;
	struct input_dev *input_dev = data->input_dev;
	int id;
	uint8_t status;
	int x;
	int y;
	int area;
	int amplitude;

	id = message[0] - data->T9_reportid_min;
	status = message[1];
	x = (message[2] << 4) | ((message[4] >> 4) & 0xf);
	y = (message[3] << 4) | ((message[4] & 0xf));

	/* Handle 10/12 bit switching */
	if (data->max_x < 1024)
		x >>= 2;
	if (data->max_y < 1024)
		y >>= 2;

	area = message[5];
	amplitude = message[6];

	dev_dbg(dev,
		"[%u] %c%c%c%c%c%c%c%c x: %5u y: %5u area: %3u amp: %3u\n",
		id,
		(status & MXT_T9_DETECT) ? 'D' : '.',
		(status & MXT_T9_PRESS) ? 'P' : '.',
		(status & MXT_T9_RELEASE) ? 'R' : '.',
		(status & MXT_T9_MOVE) ? 'M' : '.',
		(status & MXT_T9_VECTOR) ? 'V' : '.',
		(status & MXT_T9_AMP) ? 'A' : '.',
		(status & MXT_T9_SUPPRESS) ? 'S' : '.',
		(status & MXT_T9_UNGRIP) ? 'U' : '.',
		x, y, area, amplitude);

	input_mt_slot(input_dev, id);

	if (status & MXT_T9_DETECT) {
		/*
		 * Multiple bits may be set if the host is slow to read
		 * the status messages, indicating all the events that
		 * have happened.
		 */
		if (status & MXT_T9_RELEASE) {
//			input_mt_report_slot_state(input_dev,
//						   MT_TOOL_FINGER, 0);
//			mxt_input_sync(data);
		}

		/* Touch active */
		input_mt_report_slot_state(input_dev, MT_TOOL_FINGER, 1);
		input_report_abs(input_dev, ABS_MT_POSITION_X, x);
		input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
		input_report_abs(input_dev, ABS_MT_PRESSURE, amplitude);
		input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, area);
	} else {
		/* Touch no longer active, close out slot */
		input_mt_report_slot_state(input_dev, MT_TOOL_FINGER, 0);
	}

	data->update_input = true;
}

static void mxt_proc_t100_message(struct mxt_data *data, uint8_t *message)
{
	struct input_dev *input_dev = data->input_dev;
	int id;
	uint8_t status;
	uint8_t type = 0;
	uint16_t x;
	uint16_t y;
	int distance = 0;
	int tool = 0;
	uint8_t major = 0;
	uint8_t pressure = 0;
	uint8_t orientation = 0;
    uint8_t event;

	id = message[0] - data->T100_reportid_min - 2;

	/* ignore SCRSTATUS events */
	if (id < 0)
		return;

	status = message[1];
	x = get_unaligned_le16(&message[2]);
	y = get_unaligned_le16(&message[4]);
    event = status & 0xF;

	if (status & MXT_T100_DETECT) {
		type = (status & MXT_T100_TYPE_MASK) >> 4;

		switch (type) {
		case MXT_T100_TYPE_HOVERING_FINGER:
			tool = MT_TOOL_FINGER;
			distance = MXT_DISTANCE_HOVERING;

			if (data->t100_aux_vect)
				orientation = message[data->t100_aux_vect];

			break;

		case MXT_T100_TYPE_FINGER:
		case MXT_T100_TYPE_GLOVE:
			tool = MT_TOOL_FINGER;
			distance = MXT_DISTANCE_ACTIVE_TOUCH;

			if (data->t100_aux_area)
				major = message[data->t100_aux_area];

			if (data->t100_aux_ampl)
				pressure = message[data->t100_aux_ampl];

			if (data->t100_aux_vect)
				orientation = message[data->t100_aux_vect];

			break;

		case MXT_T100_TYPE_PASSIVE_STYLUS:
			tool = MT_TOOL_PEN;

			/*
			 * Passive stylus is reported with size zero so
			 * hardcode.
			 */
			major = MXT_TOUCH_MAJOR_DEFAULT;

			if (data->t100_aux_ampl)
				pressure = message[data->t100_aux_ampl];

			break;

		case MXT_T100_TYPE_LARGE_TOUCH:
			/* Ignore suppressed touch */
			break;

		default:
			dev_dbg(dev, "Unexpected T100 type\n");
			return;
		}
	}

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
    return;
    
	/*
	 * Values reported should be non-zero if tool is touching the
	 * device
	 */
	if (!pressure && type != MXT_T100_TYPE_HOVERING_FINGER)
		pressure = MXT_PRESSURE_DEFAULT;

	input_mt_slot(input_dev, id);

	if (status & MXT_T100_DETECT) {
		dev_dbg(dev, "[%u] type:%u x:%u y:%u a:%02X p:%02X v:%02X\n",
			id, type, x, y, major, pressure, orientation);

		input_mt_report_slot_state(input_dev, tool, 1);
		input_report_abs(input_dev, ABS_MT_POSITION_X, x);
		input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
		input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, major);
		input_report_abs(input_dev, ABS_MT_PRESSURE, pressure);
		input_report_abs(input_dev, ABS_MT_DISTANCE, distance);
		input_report_abs(input_dev, ABS_MT_ORIENTATION, orientation);
	} else {
		dev_dbg(dev, "[%u] release\n", id);

		/* close out slot */
		input_mt_report_slot_state(input_dev, 0, 0);
	}

	data->update_input = true;
}

static int mxt_proc_message(struct mxt_data *data, uint8_t *message)
{
	uint8_t report_id = message[0];

	if (report_id == MXT_RPTID_NOMSG)
		return 0;

	if (report_id == data->T6_reportid) {
		mxt_proc_t6_messages(data, message);
//	} else if (!data->input_dev) {
//		/*
//		 * Do not report events if input device
//		 * is not yet registered.
//		 */
//		mxt_dump_message(data, message);
	} else if (report_id >= data->T9_reportid_min &&
		   report_id <= data->T9_reportid_max) {
		mxt_proc_t9_message(data, message);
	} else if (report_id >= data->T100_reportid_min &&
		   report_id <= data->T100_reportid_max) {
		mxt_proc_t100_message(data, message);
	} else if (report_id == data->T19_reportid) {
		mxt_input_button(data, message);
		data->update_input = true;
	} else {
		mxt_dump_message(data, message);
	}

	return 1;
}

static int mxt_read_and_process_messages(struct mxt_data *data, uint8_t count)
{
//	struct device *dev = &data->client->dev;
	int ret;
	int i;
	uint8_t num_valid = 0;

	/* Safety check for msg_buf */
	if (count > data->max_reportid)
		return -EINVAL;

	/* Process remaining messages if necessary */
	ret = __mxt_read_reg(data->client, data->T5_address,
				data->T5_msg_size * count, data->msg_buf);
	if (ret) {
		dev_err(dev, "Failed to read %u messages (%d)\n", count, ret);
		return ret;
	}

	for (i = 0;  i < count; i++) {
		ret = mxt_proc_message(data,
			data->msg_buf + data->T5_msg_size * i);

		if (ret == 1)
			num_valid++;
	}

	/* return number of messages read */
	return num_valid;
}

static int mxt_process_messages_t44(struct mxt_data *data)
{
//	struct device *dev = &data->client->dev;
	int ret;
	uint8_t count, num_left;

	/* Read T44 and T5 together */
	ret = __mxt_read_reg(data->client, data->T44_address,
		data->T5_msg_size + 1, data->msg_buf);
	if (ret) {
		dev_err(dev, "Failed to read T44 and T5 (%d)\n", ret);
		return IRQ_NONE;
	}

	count = data->msg_buf[0];

	/*
	 * This condition may be caused by the CHG line being configured in
	 * Mode 0. It results in unnecessary I2C operations but it is benign.
	 */
	if (count == 0)
		return IRQ_NONE;

	if (count > data->max_reportid) {
		dev_warn(dev, "T44 count %d exceeded max report id\n", count);
		count = data->max_reportid;
	}

	/* Process first message */
	ret = mxt_proc_message(data, data->msg_buf + 1);
	if (ret < 0) {
		dev_warn(dev, "Unexpected invalid message\n");
		return IRQ_NONE;
	}

	num_left = count - 1;

	/* Process remaining messages if necessary */
	if (num_left) {
		ret = mxt_read_and_process_messages(data, num_left);
		if (ret < 0)
			goto end;
		else if (ret != num_left)
			dev_warn(dev, "Unexpected invalid message\n");
            ;
	}

end:
	if (data->update_input) {
		mxt_input_sync(data);
		data->update_input = false;
	}

	return IRQ_HANDLED;
}

//static int mxt_process_messages_until_invalid(struct mxt_data *data)
//{
////	struct device *dev = &data->client->dev;
//	int count, read;
//	uint8_t tries = 2;
//
//	count = data->max_reportid;
//
//	/* Read messages until we force an invalid */
//	do {
//		read = mxt_read_and_process_messages(data, count);
//		if (read < count)
//			return 0;
//	} while (--tries);
//
//	if (data->update_input) {
//		mxt_input_sync(data);
//		data->update_input = false;
//	}
//
////	dev_err(dev, "CHG pin isn't cleared\n");
//	return -EBUSY;
//}

static int mxt_process_messages(struct mxt_data *data)
{
	int total_handled, num_handled;
	uint8_t count = data->last_message_count;

	if (count < 1 || count > data->max_reportid)
		count = 1;

	/* include final invalid message */
	total_handled = mxt_read_and_process_messages(data, count + 1);
	if (total_handled < 0)
		return IRQ_NONE;
	/* if there were invalid messages, then we are done */
	else if (total_handled <= count)
		goto update_count;

	/* keep reading two msgs until one is invalid or reportid limit */
	do {
		num_handled = mxt_read_and_process_messages(data, 2);
		if (num_handled < 0)
			return IRQ_NONE;

		total_handled += num_handled;

		if (num_handled < 2)
			break;
	} while (total_handled < data->num_touchids);

update_count:
	data->last_message_count = total_handled;

	if (data->update_input) {
		mxt_input_sync(data);
		data->update_input = false;
	}

	return IRQ_HANDLED;
}

int mxt_interrupt(struct mxt_data *data)
{
	if (!data->object_table)
		return -1;

	if (data->T44_address) {
		return mxt_process_messages_t44(data);
	} else {
		return mxt_process_messages(data);
	}
}

static int mxt_t6_command(struct mxt_data *data, uint16_t cmd_offset,
			  uint8_t value, bool wait)
{
	uint16_t reg;
	uint8_t command_register;
	int timeout_counter = 0;
	int ret;

	reg = data->T6_address + cmd_offset;

	ret = mxt_write_reg(data->client, reg, value);
	if (ret)
		return ret;

	if (!wait)
		return 0;

	do {
		_mxt_DelayMS(20);
		ret = __mxt_read_reg(data->client, reg, 1, &command_register);
		if (ret)
			return ret;
	} while (command_register != 0 && timeout_counter++ <= 100);

	if (timeout_counter > 100) {
		dev_err(&data->client->dev, "Command failed!\n");
		return -EIO;
	}

	return 0;
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

static int mxt_soft_reset(struct mxt_data *data)
{
//	struct device *dev = &data->client->dev;
	int ret;

	dev_info(dev, "Resetting device\n");

//	disable_irq(data->irq);

//	reinit_completion(&data->reset_completion);

	ret = mxt_t6_command(data, MXT_COMMAND_RESET, MXT_RESET_VALUE, false);
	if (ret)
		return ret;

	/* Ignore CHG line for 100ms after reset */
	_mxt_DelayMS(MXT_RESET_INVALID_CHG);

//	mxt_acquire_irq(data);

//	ret = mxt_wait_for_completion(data, &data->reset_completion,
//				      MXT_RESET_TIMEOUT);
	if (ret)
		return ret;

	return ret;
}

static void mxt_update_crc(struct mxt_data *data, uint8_t cmd, uint8_t value)
{
	/*
	 * On failure, CRC is set to 0 and config will always be
	 * downloaded.
	 */
	data->config_crc = 0;
//	reinit_completion(&data->crc_completion);

	mxt_t6_command(data, cmd, value, true);

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

static uint32_t mxt_calculate_crc(uint8_t *base, off_t start_off, off_t end_off)
{
	uint32_t crc = 0;
	uint8_t *ptr = base + start_off;
	uint8_t *last_val = base + end_off - 1;

	if (end_off < start_off)
		return -EINVAL;

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

static int mxt_prepare_cfg_mem(struct mxt_data *data,
			       const struct firmware *cfg,
			       char * ptr,
			       unsigned int cfg_start_ofs,
			       uint8_t *config_mem,
			       size_t config_mem_size)
{
//	struct device *dev = &data->client->dev;
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
		if (ret == EOF) {
			/* EOF */
			break;
		} else if (ret != 3) {
			dev_err(dev, "Bad format: failed to parse object\n");
			return -EINVAL;
		}
        /* increment past object header */
		ptr+=15;

		object = mxt_get_object(data, type);
		if (!object) {
			/* Skip object */
			for (i = 0; i < size; i++) {
				ret = sscanf(ptr, "%2x",
					     &scan);
				if (ret != 1) {
					dev_err(dev, "Bad format in T%d at %d\n",
						type, i);
					return -EINVAL;
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
			return -EINVAL;
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
				return -EINVAL;
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
				return -EINVAL;
			}
		}
	}

	return 0;
}

static int mxt_upload_cfg_mem(struct mxt_data *data, unsigned int cfg_start,
			      uint8_t *config_mem, size_t config_mem_size)
{
	unsigned int byte_offset = 0;
//	int error;

	/* Write configuration as blocks */
	while (byte_offset < config_mem_size) {
		unsigned int size = config_mem_size - byte_offset;

		if (size > MXT_MAX_BLOCK_WRITE)
			size = MXT_MAX_BLOCK_WRITE;

		__mxt_write_reg(data->client,
					cfg_start + byte_offset,
					size, config_mem + byte_offset);
//		if (error) {
////			dev_err(&data->client->dev,
////				"Config write error, ret=%d\n", error);
//			return error;
//		}

		byte_offset += size;
	}

	return 0;
}

/*
 * mxt_update_cfg - download configuration to chip
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
static int mxt_update_cfg(struct mxt_data *data, const struct firmware *cfg)
{
//	struct device *dev = &data->client->dev;
	struct mxt_info cfg_info;
	int ret=0;
//	int i;
	int cfg_start_ofs;
	uint32_t info_crc, config_crc, calculated_crc;
	uint8_t *config_mem;
	size_t config_mem_size;

	mxt_update_crc(data, MXT_COMMAND_REPORTALL, 1);
    
    char * ptr = strtok((char*)cfg->data, "\n");
    
	if (strncmp(ptr, MXT_CFG_MAGIC, strlen(MXT_CFG_MAGIC))) {
		dev_err(dev, "Unrecognised config file\n");
		return -EINVAL;
	}
	ptr = strtok(NULL, "\n");
    
	/* Load information block and check */
//	for (i = 0; i < sizeof(struct mxt_info); i++) {
    ret = sscanf(ptr, "%2x %2x %2x %2x %2x %2x %2x",
            (unsigned int *)&cfg_info.family_id, (unsigned int *)&cfg_info.variant_id,
            (unsigned int *)&cfg_info.version, (unsigned int *)&cfg_info.build,
            (unsigned int *)&cfg_info.matrix_xsize, (unsigned int *)&cfg_info.matrix_ysize,
            (unsigned int *)&cfg_info.object_num
            );
        
    if (ret != sizeof(struct mxt_info)) {
        dev_err(dev, "Bad format\n");
        return -EINVAL;
    }

    ptr = strtok(NULL, "\n");
//	}

	if (cfg_info.family_id != data->info->family_id) {
		dev_err(dev, "Family ID mismatch!\n");
		return -EINVAL;
	}

	if (cfg_info.variant_id != data->info->variant_id) {
		dev_err(dev, "Variant ID mismatch!\n");
		return -EINVAL;
	}

	/* Read CRCs */
	ret = sscanf(ptr, "%x", (unsigned int*)&info_crc);
	if (ret != 1) {
		dev_err(dev, "Bad format: failed to parse Info CRC\n");
		return -EINVAL;
	}
    ptr = strtok(NULL, "\n");

	ret = sscanf(ptr, "%x", (unsigned int*)&config_crc);
	if (ret != 1) {
		dev_err(dev, "Bad format: failed to parse Config CRC\n");
		return -EINVAL;
	}
    ptr = strtok(NULL, "\n");

	/*
	 * The Info Block CRC is calculated over mxt_info and the object
	 * table. If it does not match then we are trying to load the
	 * configuration from a different chip or firmware version, so
	 * the configuration CRC is invalid anyway.
	 */
	if (info_crc == data->info_crc) {
		if (config_crc == 0 || data->config_crc == 0) {
			dev_info(dev, "CRC zero, attempting to apply config\n");
		} else if (config_crc == data->config_crc) {
			dev_dbg(dev, "Config CRC 0x%06X: OK\n",
				 data->config_crc);
			return 0;
		} else {
			dev_info(dev, "Config CRC 0x%06X: does not match file 0x%06X\n",
				 data->config_crc, config_crc);
		}
	} else {
		dev_warn(dev,
			 "Warning: Info CRC error - device=0x%06X file=0x%06X\n",
			 data->info_crc, info_crc);
	}

	/* Malloc memory to store configuration */
	cfg_start_ofs = MXT_OBJECT_START +
			data->info->object_num * sizeof(struct mxt_object) +
			MXT_INFO_CHECKSUM_SIZE;
	config_mem_size = data->mem_size - cfg_start_ofs;
	config_mem = OSAL_Malloc(config_mem_size);
	if (!config_mem) {
		dev_err(dev, "Failed to allocate memory\n");
		return -ENOMEM;
	}

	ret = mxt_prepare_cfg_mem(data, cfg, ptr, cfg_start_ofs,
				  config_mem, config_mem_size);
	if (ret)
		goto release_mem;

	/* Calculate crc of the received configs (not the raw config file) */
	if (data->T7_address < cfg_start_ofs) {
		dev_err(dev, "Bad T7 address, T7addr = %x, config offset %x\n",
			data->T7_address, cfg_start_ofs);
		ret = 0;
		goto release_mem;
	}

	calculated_crc = mxt_calculate_crc(config_mem,
					   data->T71_address - cfg_start_ofs,
					   config_mem_size);
            
	if (config_crc > 0 && config_crc != calculated_crc)
		dev_warn(dev, "Config CRC error, calculated=%06X, file=%06X\n",
			 calculated_crc, config_crc);

	ret = mxt_upload_cfg_mem(data, cfg_start_ofs,
				 config_mem, config_mem_size);
	if (ret)
		goto release_mem;

//    mxt_update_crc(data, MXT_COMMAND_BACKUPNV, MXT_BACKUP_VALUE);

	ret = mxt_soft_reset(data);
	if (ret)
		goto release_mem;

	dev_info(dev, "Config successfully updated\n");

//	/* T7 config may have changed */
//	mxt_init_t7_power_cfg(data);

release_mem:
	OSAL_Free(config_mem);
	return 0;
}

//static void mxt_free_input_device(struct mxt_data *data)
//{
//	if (data->input_dev) {
////		input_unregister_device(data->input_dev);
//		data->input_dev = NULL;
//	}
//}

/*
 * mxt_update_cfg - download configuration to chip
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
//static int mxt_update_cfg_xcfg(struct mxt_data *data, const struct firmware *cfg)
//{
////	struct device *dev = &data->client->dev;
//	struct mxt_info cfg_info;
//	int ret=0;
////	int i;
//	int cfg_start_ofs;
//	uint32_t info_crc, config_crc, calculated_crc;
//	uint8_t *config_mem;
//	size_t config_mem_size;
//
//	mxt_update_crc(data, MXT_COMMAND_REPORTALL, 1);
//
////    FILE *fp;
//    int c;
//    char object[255];
//    char tmp[255];
//    char *substr;
//    int object_id;
//    int instance;
//    int object_address;
//    int object_size;
//    int data;
//    int file_read = 0;
//    bool ignore_line = false;
//
//
//    cfg->config_type = CONFIG_XCFG;
//
//    while (true) {
//		/* Read type, instance, length */
//		ret = sscanf(ptr, "%x %x %x",
//			     &type, &instance, &size);
//		if (ret == EOF) {
//            
//        }
//    while (!file_read) {
//    /* First character is expected to be '[' - skip empty lines and spaces  */
//    c = getc(fp);
//    while ((c == '\n') || (c == '\r') || (c == 0x20))
//      c = getc(fp);
//
//    if (c != '[') {
//      if (c == EOF)
//        break;
//
//      /* If we are ignoring the current section then look for the next section */
//      if (ignore_line) {
//        continue;
//      }
//
//      dev_err(ctx, "Parse error: expected '[', read ascii char %c!", c);
//      ret = MXT_ERROR_FILE_FORMAT;
//      goto close;
//    }
//
//    if (fscanf(fp, "%[^] ]", object) != 1) {
//      dev_err(ctx, "Object parse error");
//      ret = MXT_ERROR_FILE_FORMAT;
//      goto close;
//    }
//
//    /* Ignore the comments and file header sections */
//    if (!strcmp(object, "COMMENTS")
//        || !strcmp(object, "APPLICATION_INFO_HEADER")) {
//      ignore_line = true;
//      dev_dbg(ctx, "Skipping %s", object);
//      continue;
//    }
//
//    ignore_line = false;
//
//    /* Extract the checksum */
//    if (!strcmp(object, "VERSION_INFO_HEADER")) {
//      while (false == ignore_line) {
//        if (fscanf(fp, "%s", tmp) != 1) {
//          dev_err(ctx, "Version info header parse error");
//          ret = MXT_ERROR_FILE_FORMAT;
//          goto close;
//        }
//        if (!strncmp(tmp, "CHECKSUM", 8)) {
//          sscanf(tmp, "%[^'=']=%x", object, &cfg->config_crc);
//          dev_dbg(ctx, "Config CRC from file: %s", tmp);
//          ignore_line = true;
//        }
//      }
//      continue;
//    }
//
//    if (fscanf(fp, "%s", tmp) != 1) {
//      dev_err(ctx, "Instance parse error");
//      ret = MXT_ERROR_FILE_FORMAT;
//      goto close;
//    }
//
//    if (strcmp(tmp, "INSTANCE")) {
//      dev_err(ctx, "Parse error, expected INSTANCE, got %s", tmp);
//      ret = MXT_ERROR_FILE_FORMAT;
//      goto close;
//    }
//
//    if (fscanf(fp, "%d", &instance) != 1) {
//      dev_err(ctx, "Instance number parse error");
//      ret = MXT_ERROR_FILE_FORMAT;
//      goto close;
//    }
//
//    /* Read rest of header section */
//    while(c != ']') {
//      c = getc(fp);
//      if (c == '\n') {
//        dev_err(ctx, "Parse error, expected ] before end of line");
//        ret = MXT_ERROR_FILE_FORMAT;
//        goto close;
//      }
//    }
//
//    while(c != '\n')
//      c = getc(fp);
//
//    while ((c != '=') && (c != EOF))
//      c = getc(fp);
//
//    if (fscanf(fp, "%d", &object_address) != 1) {
//      dev_err(ctx, "Object address parse error");
//      ret = MXT_ERROR_FILE_FORMAT;
//      goto close;
//    }
//
//    c = getc(fp);
//    while((c != '=') && (c != EOF))
//      c = getc(fp);
//
//    if (fscanf(fp, "%d", &object_size) != 1) {
//      mxt_err(ctx, "Object size parse error");
//      ret = MXT_ERROR_FILE_FORMAT;
//      goto close;
//    }
//
//    c = getc(fp);
//
//    /* Find object type ID number at end of object string */
//    substr = strrchr(object, '_');
//    if (substr == NULL || (*(substr + 1) != 'T')) {
//      dev_err(ctx, "Parse error, could not find T number in %s", object);
//      ret = MXT_ERROR_FILE_FORMAT;
//      goto close;
//    }
//
//    if (sscanf(substr + 2, "%d", &object_id) != 1) {
//      dev_err(ctx, "Unable to get object type ID for %s", object);
//      ret = MXT_ERROR_FILE_FORMAT;
//      goto close;
//    }
//
//    dev_dbg(ctx, "%s T%u OBJECT_ADDRESS=%d OBJECT_SIZE=%d",
//            object, object_id, object_address, object_size);
//
//    objcfg = calloc(1, sizeof(struct mxt_object_config));
//    if (!objcfg) {
//      ret = MXT_ERROR_NO_MEM;
//      goto close;
//    }
//
//    objcfg->type = object_id;
//    objcfg->size = object_size;
//    objcfg->instance = instance;
//    objcfg->start_position = object_address;
//
//    *next = objcfg;
//    next = &objcfg->next;
//
//    /* Allocate memory to store configuration */
//    objcfg->data = calloc(object_size, sizeof(uint8_t));
//    if (!objcfg->data) {
//      dev_err(ctx, "Failed to allocate memory");
//      ret = MXT_ERROR_NO_MEM;
//      goto close;
//    }
//
//    while (true) {
//      int offset;
//      int width;
//
//      /* Find next line, check first character valid and rewind */
//      c = getc(fp);
//      while((c == '\n') || (c == '\r') || (c == 0x20))
//        c = getc(fp);
//
//      fseek(fp, -1, SEEK_CUR);
//
//      /* End of object */
//      if (c == '[')
//        break;
//
//      /* End of file */
//      if (c == EOF)
//        break;
//
//      /* Read address */
//      if (fscanf(fp, "%d", &offset) != 1) {
//        dev_err(ctx, "Address parse error");
//        ret = MXT_ERROR_FILE_FORMAT;
//        goto close;
//      }
//
//      /* Read byte count of this register (max 2) */
//      if (fscanf(fp, "%d", &width) != 1) {
//        dev_err(ctx, "Byte count parse error");
//        ret = MXT_ERROR_FILE_FORMAT;
//        goto close;
//      }
//
//      while((c != '=') && (c != EOF)) {
//        c = getc(fp);
//      }
//
//      if (fscanf(fp, "%d", &data) != 1) {
//        dev_err(ctx, "Data parse error");
//        ret = MXT_ERROR_FILE_FORMAT;
//        goto close;
//      }
//
//      c = getc(fp);
//
//      switch (width) {
//      case 1:
//        objcfg->data[offset] = (char) data;
//        break;
//      case 2:
//        objcfg->data[offset] = (char) data & 0xFF;
//        objcfg->data[offset + 1] = (char) ((data >> 8) & 0xFF);
//        break;
//      case 4:
//        objcfg->data[offset] = (char) data & 0xFF;
//        objcfg->data[offset + 1] = (char) ((data >> 8) & 0xFF);
//        objcfg->data[offset + 2] = (char) ((data >> 16) & 0xFF);
//        objcfg->data[offset + 3] = (char) ((data >> 24) & 0xFF);
//        break;
//      default:
//        dev_err(ctx, "Only 1, 2 and 4 byte config values are supported");
//        ret = MXT_ERROR_FILE_FORMAT;
//        goto close;
//      }
//    }
//    }
//
//    ret = MXT_SUCCESS;
//    mxt_info(ctx, "Configuration read from %s in XCFG format", filename);
//
//close:
//  fclose(fp);
//  return ret;
//}

static void mxt_free_object_table(struct mxt_data *data)
{
	data->object_table = NULL;
	data->info = NULL;
	OSAL_Free(data->raw_info_block);
	data->raw_info_block = NULL;
	OSAL_Free(data->msg_buf);
	data->msg_buf = NULL;
	data->T5_address = 0;
	data->T5_msg_size = 0;
	data->T6_reportid = 0;
	data->T7_address = 0;
	data->T9_reportid_min = 0;
	data->T9_reportid_max = 0;
	data->T19_reportid = 0;
	data->T44_address = 0;
	data->T100_reportid_min = 0;
	data->T100_reportid_max = 0;
	data->max_reportid = 0;
}

static int mxt_parse_object_table(struct mxt_data *data,
				  struct mxt_object *object_table)
{
//	struct i2c_client *client = data->client;
	int i;
	uint8_t reportid;
	uint16_t end_address;

	/* Valid Report IDs start counting from 1 */
	reportid = 1;
	data->mem_size = 0;
	for (i = 0; i < data->info->object_num; i++) {
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
            data->T71_address = object->start_address;
            break;
        
		case MXT_GEN_MESSAGE_T5:
			if (data->info->family_id == 0x80 &&
			    data->info->version < 0x20) {
				/*
				 * On mXT224 firmware versions prior to V2.0
				 * read and discard unused CRC byte otherwise
				 * DMA reads are misaligned.
				 */
				data->T5_msg_size = mxt_obj_size(object);
			} else {
				/* CRC not enabled, so skip last byte */
				data->T5_msg_size = mxt_obj_size(object) - 1;
			}
			data->T5_address = object->start_address;
			break;
		case MXT_GEN_COMMAND_T6:
			data->T6_reportid = min_id;
			data->T6_address = object->start_address;
			break;
		case MXT_GEN_POWER_T7:
			data->T7_address = object->start_address;
			break;
		case MXT_TOUCH_MULTI_T9:
			data->multitouch = MXT_TOUCH_MULTI_T9;
			data->T9_reportid_min = min_id;
			data->T9_reportid_max = max_id;
			data->num_touchids = object->num_report_ids
						* mxt_obj_instances(object);
			break;
		case MXT_SPT_MESSAGECOUNT_T44:
			data->T44_address = object->start_address;
			break;
		case MXT_SPT_GPIOPWM_T19:
			data->T19_reportid = min_id;
			break;
		case MXT_TOUCH_MULTITOUCHSCREEN_T100:
			data->multitouch = MXT_TOUCH_MULTITOUCHSCREEN_T100;
			data->T100_reportid_min = min_id;
			data->T100_reportid_max = max_id;
			/* first two report IDs reserved */
			data->num_touchids = object->num_report_ids - 2;
			break;
		}

		end_address = object->start_address
			+ mxt_obj_size(object) * mxt_obj_instances(object) - 1;

		if (end_address >= data->mem_size)
			data->mem_size = end_address + 1;
	}

	/* Store maximum reportid */
	data->max_reportid = reportid;

	/* If T44 exists, T5 position has to be directly after */
	if (data->T44_address && (data->T5_address != data->T44_address + 1)) {
		return -EINVAL;
	}

	data->msg_buf = OSAL_Malloc(data->T5_msg_size);
	if (!data->msg_buf)
		return -ENOMEM;

	return 0;
}

static int mxt_read_info_block(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	bool error;
	size_t size;
	void *id_buf, *buf;
	uint8_t num_objects;
	uint32_t calculated_crc;
	uint8_t *crc_ptr;

	/* If info block already allocated, free it */
	if (data->raw_info_block)
		mxt_free_object_table(data);

	/* Read 7-byte ID information block starting at address 0 */
	size = sizeof(struct mxt_info);
	id_buf = OSAL_Malloc(size);
	if (!id_buf)
		return -ENOMEM;

	error = __mxt_read_reg(client, 0, size, id_buf);
	if (error)
		goto err_free_mem;
    
	/* Resize buffer to give space for rest of info block */
	num_objects = ((struct mxt_info *)id_buf)->object_num;
	size += (num_objects * sizeof(struct mxt_object))
		+ MXT_INFO_CHECKSUM_SIZE;

//    OSAL_Free(id_buf);
	buf = OSAL_Malloc(size);
	if (!buf) {
		error = -ENOMEM;
		goto err_free_mem;
	}
	memcpy(buf, id_buf, sizeof(struct mxt_info));
    id_buf = buf;
        
	error = __mxt_read_reg(client, MXT_OBJECT_START,
			       size - MXT_OBJECT_START,
                               (uint8_t*)id_buf + MXT_OBJECT_START);
	if (error)
		goto err_free_mem;
    
	/* Extract & calculate checksum */
        crc_ptr = (uint8_t*)id_buf + size - MXT_INFO_CHECKSUM_SIZE;
	data->info_crc = crc_ptr[0] | (crc_ptr[1] << 8) | (crc_ptr[2] << 16);

	calculated_crc = mxt_calculate_crc(id_buf, 0,
					   size - MXT_INFO_CHECKSUM_SIZE);
    
	/*
	 * CRC mismatch can be caused by data corruption due to I2C comms
	 * issue or else device is not using Object Based Protocol (eg i2c-hid)
	 */
	if ((data->info_crc == 0) || (data->info_crc != calculated_crc)) {
		error = -EIO;
		goto err_free_mem;
	}

	data->raw_info_block = id_buf;
	data->info = (struct mxt_info *)id_buf;

	/* Parse object table information */
        error = mxt_parse_object_table(data, (struct mxt_object *)((uint8_t*)id_buf + MXT_OBJECT_START));
	if (error) {
		mxt_free_object_table(data);
		goto err_free_mem;
	}

        data->object_table = (struct mxt_object *)((uint8_t*)id_buf + MXT_OBJECT_START);

	return 0;

err_free_mem:
	OSAL_Free(id_buf);
	return error;
}

static int mxt_read_t9_resolution(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	int error;
	struct t9_range range;
	unsigned char orient;
	struct mxt_object *object;

	object = mxt_get_object(data, MXT_TOUCH_MULTI_T9);
	if (!object)
		return -EINVAL;

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T9_XSIZE,
			       sizeof(data->xsize), &data->xsize);
	if (error)
		return error;

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T9_YSIZE,
			       sizeof(data->ysize), &data->ysize);
	if (error)
		return error;

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T9_RANGE,
			       sizeof(range), &range);
	if (error)
		return error;

	data->max_x = get_unaligned_le16(&range.x);
	data->max_y = get_unaligned_le16(&range.y);

	error = __mxt_read_reg(client,
				object->start_address + MXT_T9_ORIENT,
				1, &orient);
	if (error)
		return error;

	data->xy_switch = orient & MXT_T9_ORIENT_SWITCH;
	data->invertx = orient & MXT_T9_ORIENT_INVERTX;
	data->inverty = orient & MXT_T9_ORIENT_INVERTY;

	return 0;
}

static int mxt_read_t100_config(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	int error;
	struct mxt_object *object;
	uint16_t range_x, range_y;
	uint8_t cfg, tchaux;
	uint8_t aux;
    uint8_t numtch;

	object = mxt_get_object(data, MXT_TOUCH_MULTITOUCHSCREEN_T100);
	if (!object)
		return -EINVAL;

    /* read number of reported touches */
	error = __mxt_read_reg(client,
			       object->start_address + MXT_T100_NUMTCH,
			       sizeof(numtch), &numtch);
    
	/* read touchscreen dimensions */
	error = __mxt_read_reg(client,
			       object->start_address + MXT_T100_XRANGE,
			       sizeof(range_x), &range_x);
	if (error)
		return error;

	data->max_x = get_unaligned_le16(&range_x);

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T100_YRANGE,
			       sizeof(range_y), &range_y);
	if (error)
		return error;

	data->max_y = get_unaligned_le16(&range_y);

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T100_XSIZE,
			       sizeof(data->xsize), &data->xsize);
	if (error)
		return error;

	error = __mxt_read_reg(client,
			       object->start_address + MXT_T100_YSIZE,
			       sizeof(data->ysize), &data->ysize);
	if (error)
		return error;

	/* read orientation config */
	error =  __mxt_read_reg(client,
				object->start_address + MXT_T100_CFG1,
				1, &cfg);
	if (error)
		return error;

	data->xy_switch = cfg & MXT_T100_CFG_SWITCHXY;
	data->invertx = cfg & MXT_T100_CFG_INVERTX;
	data->inverty = cfg & MXT_T100_CFG_INVERTY;

	/* allocate aux bytes */
	error =  __mxt_read_reg(client,
				object->start_address + MXT_T100_TCHAUX,
				1, &tchaux);
	if (error)
		return error;

	aux = 6;

	if (tchaux & MXT_T100_TCHAUX_VECT)
		data->t100_aux_vect = aux++;

	if (tchaux & MXT_T100_TCHAUX_AMPL)
		data->t100_aux_ampl = aux++;

	if (tchaux & MXT_T100_TCHAUX_AREA)
		data->t100_aux_area = aux++;

	dev_dbg(&client->dev,
		"T100 aux mappings vect:%u ampl:%u area:%u\n",
		data->t100_aux_vect, data->t100_aux_ampl, data->t100_aux_area);

	return 0;
}

static int mxt_write_t100_config(struct mxt_data *data)
{
	struct i2c_client *client = data->client;
	int error;
	struct mxt_object *object;

	object = mxt_get_object(data, MXT_TOUCH_MULTITOUCHSCREEN_T100);
	if (!object)
		return -EINVAL;

    /* write number of reported touches */
	//error = __mxt_write_reg(client,
	//		       object->start_address + MXT_T100_NUMTCH,
	//		       sizeof(numtch), &numtch);
    
	/* read touchscreen dimensions */
	error = __mxt_write_reg(client,
			       object->start_address + MXT_T100_XRANGE,
			       sizeof(data->xRes), &data->xRes);

	if (error)
		return error;

	error = __mxt_write_reg(client,
			       object->start_address + MXT_T100_YRANGE,
			       sizeof(data->yRes), &data->yRes);
	if (error)
		return error;

	/* read orientation config */
	//error =  __mxt_read_reg(client,
	//			object->start_address + MXT_T100_CFG1,
	//			1, &cfg);
//	if (error)
//		return error;

	//data->xy_switch = cfg & MXT_T100_CFG_SWITCHXY;
	//data->invertx = cfg & MXT_T100_CFG_INVERTX;
	//data->inverty = cfg & MXT_T100_CFG_INVERTY;

	return 0;
}

static int mxt_initialize_input_device(struct mxt_data *data)
{
//	struct device *dev = &data->client->dev;
	struct input_dev *input_dev=0;
	int error;
	unsigned int num_mt_slots;
	unsigned int mt_flags = 0;

	switch (data->multitouch) {
	case MXT_TOUCH_MULTI_T9:
		num_mt_slots = data->T9_reportid_max - data->T9_reportid_min + 1;
		error = mxt_read_t9_resolution(data);
		if (error)
//			dev_warn(dev, "Failed to initialize T9 resolution\n");
		break;

	case MXT_TOUCH_MULTITOUCHSCREEN_T100:
		num_mt_slots = data->num_touchids;
		error = mxt_read_t100_config(data);
//		if (error)
//			dev_warn(dev, "Failed to read T100 config\n");
		break;

	default:
//		dev_err(dev, "Invalid multitouch object\n");
		return -EINVAL;
	}

	/* Handle default values and orientation switch */
	if (data->max_x == 0)
		data->max_x = 480;

	if (data->max_y == 0)
		data->max_y = 272;
//
//	if (data->xy_switch)
//		swap(data->max_x, data->max_y);

//	dev_info(dev, "Touchscreen size X%uY%u\n", data->max_x, data->max_y);

//	/* For multi touch */
	error = input_mt_init_slots(input_dev, num_mt_slots, mt_flags);
//	if (error) {
//		dev_err(dev, "Error %d initialising slots\n", error);
//		goto err_free_mem;
//	}
//
//	if (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100) {
//		input_set_abs_params(input_dev, ABS_MT_TOOL_TYPE,
//				     0, MT_TOOL_MAX, 0, 0);
//		input_set_abs_params(input_dev, ABS_MT_DISTANCE,
//				     MXT_DISTANCE_ACTIVE_TOUCH,
//				     MXT_DISTANCE_HOVERING,
//				     0, 0);
//	}
//
//	input_set_abs_params(input_dev, ABS_MT_POSITION_X,
//			     0, data->max_x, 0, 0);
//	input_set_abs_params(input_dev, ABS_MT_POSITION_Y,
//			     0, data->max_y, 0, 0);
//
//	if (data->multitouch == MXT_TOUCH_MULTI_T9 ||
//	    (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100 &&
//	     data->t100_aux_area)) {
//		input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR,
//				     0, MXT_MAX_AREA, 0, 0);
//	}
//
//	if (data->multitouch == MXT_TOUCH_MULTI_T9 ||
//	    (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100 &&
//	     data->t100_aux_ampl)) {
//		input_set_abs_params(input_dev, ABS_MT_PRESSURE,
//				     0, 255, 0, 0);
//	}
//
//	if (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100 &&
//	    data->t100_aux_vect) {
//		input_set_abs_params(input_dev, ABS_MT_ORIENTATION,
//				     0, 255, 0, 0);
//	}
//
//	if (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100 &&
//	    data->t100_aux_ampl) {
//		input_set_abs_params(input_dev, ABS_MT_PRESSURE,
//				     0, 255, 0, 0);
//	}
//
//	if (data->multitouch == MXT_TOUCH_MULTITOUCHSCREEN_T100 &&
//	    data->t100_aux_vect) {
//		input_set_abs_params(input_dev, ABS_MT_ORIENTATION,
//				     0, 255, 0, 0);
//	}
//
//	input_set_drvdata(input_dev, data);
//
//	error = input_register_device(input_dev);
//	if (error) {
//		dev_err(dev, "Error %d registering input device\n", error);
//		goto err_free_mem;
//	}
//
//	data->input_dev = input_dev;

	return 0;

//err_free_mem:
//	input_free_device(input_dev);
	return error;
}

static int mxt_initialize(struct mxt_data *data)
{
//	struct i2c_client *client = data->client;
//	int recovery_attempts = 0;
	int error;

	while (1) {
		error = mxt_read_info_block(data);
		if (!error)
			break;
	}

    /* read current configuration */
    mxt_read_t100_config(data);
    
    /* write non-persistent configuration */
    mxt_write_t100_config(data);
    
    /* we are not going to acquire irq - polling */
//	error = mxt_acquire_irq(data);
//	if (error)
//		return error;

    /* perform configuration update */
//	error = request_firmware_nowait(THIS_MODULE, true, MXT_CFG_NAME,
//					&client->dev, GFP_KERNEL, data,
//					mxt_config_cb);
//	if (error) {
//		dev_err(&client->dev, "Failed to invoke firmware loader: %d\n",
//			error);
//		return error;
//	}

	return 0;
}

static int mxt_set_t7_power_cfg(struct mxt_data *data, uint8_t sleep)
{
//	struct device *dev = &data->client->dev;
	int error;
	struct t7_config *new_config;
	struct t7_config deepsleep = { .active = 0, .idle = 0 };

	if (sleep == MXT_POWER_CFG_DEEPSLEEP)
		new_config = &deepsleep;
	else
		new_config = &data->t7_cfg;

	error = __mxt_write_reg(data->client, data->T7_address,
				sizeof(data->t7_cfg), new_config);
	if (error)
		return error;

	dev_dbg(dev, "Set T7 ACTV:%d IDLE:%d\n",
		new_config->active, new_config->idle);

	return 0;
}

static int mxt_init_t7_power_cfg(struct mxt_data *data)
{
//	struct device *dev = &data->client->dev;
//	int error;
	bool retry = false;

    return 0; 
    
recheck:
	__mxt_read_reg(data->client, data->T7_address,
				sizeof(data->t7_cfg), &data->t7_cfg);
//	if (error)
//		return error;

	if (data->t7_cfg.active == 0 || data->t7_cfg.idle == 0) {
		if (!retry) {
			dev_dbg(dev, "T7 cfg zero, resetting\n");
			mxt_soft_reset(data);
			retry = true;
			goto recheck;
		} else {
			dev_dbg(dev, "T7 cfg zero after reset, overriding\n");
			data->t7_cfg.active = 20;
			data->t7_cfg.idle = 100;
			return mxt_set_t7_power_cfg(data, MXT_POWER_CFG_RUN);
		}
	}

	dev_dbg(dev, "Initialized power cfg: ACTV %d, IDLE %d\n",
		data->t7_cfg.active, data->t7_cfg.idle);
	return 0;
}

//#ifdef CONFIG_TOUCHSCREEN_MXT_T37
//static u16 mxt_get_debug_value(struct mxt_data *data, unsigned int x,
//			       unsigned int y)
//{
//	struct mxt_info *info = data->info;
//	struct mxt_dbg *dbg = &data->dbg;
//	unsigned int ofs, page;
//	unsigned int col = 0;
//	unsigned int col_width;
//
//	if (info->family_id == MXT_FAMILY_1386) {
//		col_width = info->matrix_ysize / MXT1386_COLUMNS;
//		col = y / col_width;
//		y = y % col_width;
//	} else {
//		col_width = info->matrix_ysize;
//	}
//
//	ofs = (y + (x * col_width)) * sizeof(u16);
//	page = ofs / MXT_DIAGNOSTIC_SIZE;
//	ofs %= MXT_DIAGNOSTIC_SIZE;
//
//	if (info->family_id == MXT_FAMILY_1386)
//		page += col * MXT1386_PAGES_PER_COLUMN;
//
//	return get_unaligned_le16(&dbg->t37_buf[page].data[ofs]);
//}
//
//static int mxt_convert_debug_pages(struct mxt_data *data, u16 *outbuf)
//{
//	struct mxt_dbg *dbg = &data->dbg;
//	unsigned int x = 0;
//	unsigned int y = 0;
//	unsigned int i, rx, ry;
//
//	for (i = 0; i < dbg->t37_nodes; i++) {
//		/* Handle orientation */
//		rx = data->xy_switch ? y : x;
//		ry = data->xy_switch ? x : y;
//		rx = data->invertx ? (data->xsize - 1 - rx) : rx;
//		ry = data->inverty ? (data->ysize - 1 - ry) : ry;
//
//		outbuf[i] = mxt_get_debug_value(data, rx, ry);
//
//		/* Next value */
//		if (++x >= (data->xy_switch ? data->ysize : data->xsize)) {
//			x = 0;
//			y++;
//		}
//	}
//
//	return 0;
//}
//
//static int mxt_read_diagnostic_debug(struct mxt_data *data, u8 mode,
//				     u16 *outbuf)
//{
//	struct mxt_dbg *dbg = &data->dbg;
//	int retries = 0;
//	int page;
//	int ret;
//	u8 cmd = mode;
//	struct t37_debug *p;
//	u8 cmd_poll;
//
//	for (page = 0; page < dbg->t37_pages; page++) {
//		p = dbg->t37_buf + page;
//
//		ret = mxt_write_reg(data->client, dbg->diag_cmd_address,
//				    cmd);
//		if (ret)
//			return ret;
//
//		retries = 0;
//		msleep(20);
//wait_cmd:
//		/* Read back command byte */
//		ret = __mxt_read_reg(data->client, dbg->diag_cmd_address,
//				     sizeof(cmd_poll), &cmd_poll);
//		if (ret)
//			return ret;
//
//		/* Field is cleared once the command has been processed */
//		if (cmd_poll) {
//			if (retries++ > 100)
//				return -EINVAL;
//
//			msleep(20);
//			goto wait_cmd;
//		}
//
//		/* Read T37 page */
//		ret = __mxt_read_reg(data->client, dbg->t37_address,
//				     sizeof(struct t37_debug), p);
//		if (ret)
//			return ret;
//
//		if (p->mode != mode || p->page != page) {
//			dev_err(&data->client->dev, "T37 page mismatch\n");
//			return -EINVAL;
//		}
//
//		dev_dbg(&data->client->dev, "%s page:%d retries:%d\n",
//			__func__, page, retries);
//
//		/* For remaining pages, write PAGEUP rather than mode */
//		cmd = MXT_DIAGNOSTIC_PAGEUP;
//	}
//
//	return mxt_convert_debug_pages(data, outbuf);
//}
//
//static int mxt_queue_setup(struct vb2_queue *q,
//		       unsigned int *nbuffers, unsigned int *nplanes,
//		       unsigned int sizes[], struct device *alloc_devs[])
//{
//	struct mxt_data *data = q->drv_priv;
//	size_t size = data->dbg.t37_nodes * sizeof(u16);
//
//	if (*nplanes)
//		return sizes[0] < size ? -EINVAL : 0;
//
//	*nplanes = 1;
//	sizes[0] = size;
//
//	return 0;
//}

//static void mxt_debug_init(struct mxt_data *data)
//{
//	struct mxt_info *info = data->info;
//	struct mxt_dbg *dbg = &data->dbg;
//	struct mxt_object *object;
//	int error;
//
//	object = mxt_get_object(data, MXT_GEN_COMMAND_T6);
//	if (!object)
//		goto error;
//
//	dbg->diag_cmd_address = object->start_address + MXT_COMMAND_DIAGNOSTIC;
//
//	object = mxt_get_object(data, MXT_DEBUG_DIAGNOSTIC_T37);
//	if (!object)
//		goto error;
//
//	if (mxt_obj_size(object) != sizeof(struct t37_debug)) {
//		dev_warn(&data->client->dev, "Bad T37 size");
//		goto error;
//	}
//
//	dbg->t37_address = object->start_address;
//
//	/* Calculate size of data and allocate buffer */
//	dbg->t37_nodes = data->xsize * data->ysize;
//
//	if (info->family_id == MXT_FAMILY_1386)
//		dbg->t37_pages = MXT1386_COLUMNS * MXT1386_PAGES_PER_COLUMN;
//	else
//		dbg->t37_pages = DIV_ROUND_UP(data->xsize *
//					      info->matrix_ysize *
//					      sizeof(u16),
//					      sizeof(dbg->t37_buf->data));
//
//	dbg->t37_buf = devm_kmalloc_array(&data->client->dev, dbg->t37_pages,
//					  sizeof(struct t37_debug), GFP_KERNEL);
//	if (!dbg->t37_buf)
//		goto error;
//
//	/* init channel to zero */
//	mxt_set_input(data, 0);
//
//	/* register video device */
//	snprintf(dbg->v4l2.name, sizeof(dbg->v4l2.name), "%s", "atmel_mxt_ts");
//	error = v4l2_device_register(&data->client->dev, &dbg->v4l2);
//	if (error)
//		goto error;
//
//	/* initialize the queue */
//	mutex_init(&dbg->lock);
//	dbg->queue = mxt_queue;
//	dbg->queue.drv_priv = data;
//	dbg->queue.lock = &dbg->lock;
//	dbg->queue.dev = &data->client->dev;
//
//	error = vb2_queue_init(&dbg->queue);
//	if (error)
//		goto error_unreg_v4l2;
//
//	dbg->vdev = mxt_video_device;
//	dbg->vdev.v4l2_dev = &dbg->v4l2;
//	dbg->vdev.lock = &dbg->lock;
//	dbg->vdev.vfl_dir = VFL_DIR_RX;
//	dbg->vdev.queue = &dbg->queue;
//	video_set_drvdata(&dbg->vdev, data);
//
//	error = video_register_device(&dbg->vdev, VFL_TYPE_TOUCH, -1);
//	if (error)
//		goto error_unreg_v4l2;
//
//	return;
//
//error_unreg_v4l2:
//	v4l2_device_unregister(&dbg->v4l2);
//error:
//	dev_warn(&data->client->dev, "Error initializing T37\n");
//}
//#else
//static void mxt_debug_init(struct mxt_data *data)
//{
//}
//#endif

static int mxt_configure_objects(struct mxt_data *data,
				 const struct firmware *cfg)
{
//	struct device *dev = &data->client->dev;
	int error;

	error = mxt_init_t7_power_cfg(data);
	if (error) {
		dev_err(dev, "Failed to initialize power cfg\n");
		return error;
	}

	if (cfg) {
		error = mxt_update_cfg(data, cfg);
		if (error)
			dev_warn(dev, "Error %d updating config\n", error);
	}

	if (data->multitouch) {
		error = mxt_initialize_input_device(data);
		if (error)
			return error;
	} else {
		dev_warn(dev, "No touch object detected\n");
	}

//	mxt_debug_init(data);

	return 0;
}

///* Firmware Version is returned as Major.Minor.Build */
//static ssize_t mxt_fw_version_show(struct device *dev,
//				   struct device_attribute *attr, char *buf)
//{
//	struct mxt_data *data = dev_get_drvdata(dev);
//	struct mxt_info *info = data->info;
//	return scnprintf(buf, PAGE_SIZE, "%u.%u.%02X\n",
//			 info->version >> 4, info->version & 0xf, info->build);
//}
//
///* Hardware Version is returned as FamilyID.VariantID */
//static ssize_t mxt_hw_version_show(struct device *dev,
//				   struct device_attribute *attr, char *buf)
//{
//	struct mxt_data *data = dev_get_drvdata(dev);
//	struct mxt_info *info = data->info;
//	return scnprintf(buf, PAGE_SIZE, "%u.%u\n",
//			 info->family_id, info->variant_id);
//}
//
//static ssize_t mxt_show_instance(char *buf, int count,
//				 struct mxt_object *object, int instance,
//				 const u8 *val)
//{
//	int i;
//
//	if (mxt_obj_instances(object) > 1)
//		count += scnprintf(buf + count, PAGE_SIZE - count,
//				   "Instance %u\n", instance);
//
//	for (i = 0; i < mxt_obj_size(object); i++)
//		count += scnprintf(buf + count, PAGE_SIZE - count,
//				"\t[%2u]: %02x (%d)\n", i, val[i], val[i]);
//	count += scnprintf(buf + count, PAGE_SIZE - count, "\n");
//
//	return count;
//}
//
//static ssize_t mxt_object_show(struct device *dev,
//				    struct device_attribute *attr, char *buf)
//{
//	struct mxt_data *data = dev_get_drvdata(dev);
//	struct mxt_object *object;
//	int count = 0;
//	int i, j;
//	int error;
//	u8 *obuf;
//
//	/* Pre-allocate buffer large enough to hold max sized object. */
//	obuf = kmalloc(256, GFP_KERNEL);
//	if (!obuf)
//		return -ENOMEM;
//
//	error = 0;
//	for (i = 0; i < data->info->object_num; i++) {
//		object = data->object_table + i;
//
//		if (!mxt_object_readable(object->type))
//			continue;
//
//		count += scnprintf(buf + count, PAGE_SIZE - count,
//				"T%u:\n", object->type);
//
//		for (j = 0; j < mxt_obj_instances(object); j++) {
//			u16 size = mxt_obj_size(object);
//			u16 addr = object->start_address + j * size;
//
//			error = __mxt_read_reg(data->client, addr, size, obuf);
//			if (error)
//				goto done;
//
//			count = mxt_show_instance(buf, count, object, j, obuf);
//		}
//	}
//
//done:
//	kfree(obuf);
//	return error ?: count;
//}
//
//static int mxt_check_firmware_format(struct device *dev,
//				     const struct firmware *fw)
//{
//	unsigned int pos = 0;
//	char c;
//
//	while (pos < fw->size) {
//		c = *(fw->data + pos);
//
//		if (c < '0' || (c > '9' && c < 'A') || c > 'F')
//			return 0;
//
//		pos++;
//	}
//
//	/*
//	 * To convert file try:
//	 * xxd -r -p mXTXXX__APP_VX-X-XX.enc > maxtouch.fw
//	 */
//	dev_err(dev, "Aborting: firmware file must be in binary format\n");
//
//	return -EINVAL;
//}
//
//static int mxt_load_fw(struct device *dev, const char *fn)
//{
//	struct mxt_data *data = dev_get_drvdata(dev);
//	const struct firmware *fw = NULL;
//	unsigned int frame_size;
//	unsigned int pos = 0;
//	unsigned int retry = 0;
//	unsigned int frame = 0;
//	int ret;
//
//	ret = request_firmware(&fw, fn, dev);
//	if (ret) {
//		dev_err(dev, "Unable to open firmware %s\n", fn);
//		return ret;
//	}
//
//	/* Check for incorrect enc file */
//	ret = mxt_check_firmware_format(dev, fw);
//	if (ret)
//		goto release_firmware;
//
//	if (!data->in_bootloader) {
//		/* Change to the bootloader mode */
//		data->in_bootloader = true;
//
//		ret = mxt_t6_command(data, MXT_COMMAND_RESET,
//				     MXT_BOOT_VALUE, false);
//		if (ret)
//			goto release_firmware;
//
//		msleep(MXT_RESET_TIME);
//
//		/* Do not need to scan since we know family ID */
//		ret = mxt_lookup_bootloader_address(data, 0);
//		if (ret)
//			goto release_firmware;
//
//		mxt_free_input_device(data);
//		mxt_free_object_table(data);
//	} else {
//		enable_irq(data->irq);
//	}
//
//	reinit_completion(&data->bl_completion);
//
//	ret = mxt_check_bootloader(data, MXT_WAITING_BOOTLOAD_CMD, false);
//	if (ret) {
//		/* Bootloader may still be unlocked from previous attempt */
//		ret = mxt_check_bootloader(data, MXT_WAITING_FRAME_DATA, false);
//		if (ret)
//			goto disable_irq;
//	} else {
//		dev_info(dev, "Unlocking bootloader\n");
//
//		/* Unlock bootloader */
//		ret = mxt_send_bootloader_cmd(data, true);
//		if (ret)
//			goto disable_irq;
//	}
//
//	while (pos < fw->size) {
//		ret = mxt_check_bootloader(data, MXT_WAITING_FRAME_DATA, true);
//		if (ret)
//			goto disable_irq;
//
//		frame_size = ((*(fw->data + pos) << 8) | *(fw->data + pos + 1));
//
//		/* Take account of CRC bytes */
//		frame_size += 2;
//
//		/* Write one frame to device */
//		ret = mxt_bootloader_write(data, fw->data + pos, frame_size);
//		if (ret)
//			goto disable_irq;
//
//		ret = mxt_check_bootloader(data, MXT_FRAME_CRC_PASS, true);
//		if (ret) {
//			retry++;
//
//			/* Back off by 20ms per retry */
//			msleep(retry * 20);
//
//			if (retry > 20) {
//				dev_err(dev, "Retry count exceeded\n");
//				goto disable_irq;
//			}
//		} else {
//			retry = 0;
//			pos += frame_size;
//			frame++;
//		}
//
//		if (frame % 50 == 0)
//			dev_dbg(dev, "Sent %d frames, %d/%zd bytes\n",
//				frame, pos, fw->size);
//	}
//
//	/* Wait for flash. */
//	ret = mxt_wait_for_completion(data, &data->bl_completion,
//				      MXT_FW_RESET_TIME);
//	if (ret)
//		goto disable_irq;
//
//	dev_dbg(dev, "Sent %d frames, %d bytes\n", frame, pos);
//
//	/*
//	 * Wait for device to reset. Some bootloader versions do not assert
//	 * the CHG line after bootloading has finished, so ignore potential
//	 * errors.
//	 */
//	mxt_wait_for_completion(data, &data->bl_completion, MXT_FW_RESET_TIME);
//
//	data->in_bootloader = false;
//
//disable_irq:
//	disable_irq(data->irq);
//release_firmware:
//	release_firmware(fw);
//	return ret;
//}
//
//static ssize_t mxt_update_fw_store(struct device *dev,
//					struct device_attribute *attr,
//					const char *buf, size_t count)
//{
//	struct mxt_data *data = dev_get_drvdata(dev);
//	int error;
//
//	error = mxt_load_fw(dev, MXT_FW_NAME);
//	if (error) {
//		dev_err(dev, "The firmware update failed(%d)\n", error);
//		count = error;
//	} else {
//		dev_info(dev, "The firmware update succeeded\n");
//
//		error = mxt_initialize(data);
//		if (error)
//			return error;
//	}
//
//	return count;
//}

//static void mxt_start(struct mxt_data *data)
//{
//	switch (data->suspend_mode) {
//	case MXT_SUSPEND_T9_CTRL:
//		mxt_soft_reset(data);
//
//		/* Touch enable */
//		/* 0x83 = SCANEN | RPTEN | ENABLE */
//		mxt_write_object(data,
//				MXT_TOUCH_MULTI_T9, MXT_T9_CTRL, 0x83);
//		break;
//
//	case MXT_SUSPEND_DEEP_SLEEP:
//	default:
//		mxt_set_t7_power_cfg(data, MXT_POWER_CFG_RUN);
//
//		/* Recalibrate since chip has been in deep sleep */
//		mxt_t6_command(data, MXT_COMMAND_CALIBRATE, 1, false);
//		break;
//	}
//}
//
//static void mxt_stop(struct mxt_data *data)
//{
//	switch (data->suspend_mode) {
//	case MXT_SUSPEND_T9_CTRL:
//		/* Touch disable */
//		mxt_write_object(data,
//				MXT_TOUCH_MULTI_T9, MXT_T9_CTRL, 0);
//		break;
//
//	case MXT_SUSPEND_DEEP_SLEEP:
//	default:
//		mxt_set_t7_power_cfg(data, MXT_POWER_CFG_DEEPSLEEP);
//		break;
//	}
//}


//int mxt_probe()
//{
//	int error=0;

//	pDrvInstance = OSAL_Malloc(sizeof(struct mxt_data));
//	if (!pDrvInstance)
//		return -ENOMEM;

//	init_completion(&data->reset_completion);
//	init_completion(&data->crc_completion);

//	data->suspend_mode = dmi_check_system(chromebook_T9_suspend_dmi) ?
//		MXT_SUSPEND_T9_CTRL : MXT_SUSPEND_DEEP_SLEEP;

//	mxt_initialize(pDrvInstance);
//	if (error)
//		return error;
//
//	return 0;
//}

static void mxt_config_cb(const struct firmware *cfg, void *ctx)
{
//    return;
	mxt_configure_objects(ctx, cfg);
//	release_firmware(cfg);
}
