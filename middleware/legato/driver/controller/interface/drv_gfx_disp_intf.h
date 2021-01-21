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

/*******************************************************************************
 Module for Microchip Graphics Library - Hardware Abstraction Layer

  Company:
    Microchip Technology Inc.

  File Name:
    drv_gfx_intf.h

  Summary:
    Contains 4-line SPI GFX interface driver definitions and interface prototypes.

  Description:
    Contains 4-line SPI GFX interface driver definitions and interface prototypes.
*******************************************************************************/

/** \file drv_gfx_disp_intf.h
 * @brief Display driver interface functions and definitions.
 *
 * @details Contains 4-line SPI GFX interface driver definitions and interface prototypes.
 */

#ifndef _DRV_GFX_DISP_INTF_H    /* Guard against multiple inclusion */
#define _DRV_GFX_DISP_INTF_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#define GFX_Disp_Intf    uint32_t

#define DRV_GFX_DEPRECATED __attribute__ ((deprecated))

/** GFX_DISP_INTF_STATUS

  Summary:
    Enum of GFX Intf callback status.
    
*/
/**
 * @brief This enum represents display interface callback status modes
 * @details A call back status indicates the results of a transfer. These
 * are the possible results.
 */
typedef enum
{
    GFX_DISP_INTF_TX_DONE,       //Transfer Done
    GFX_DISP_INTF_ERROR,        //Error
} GFX_DISP_INTF_STATUS;

/** GFX_DISP_INTF_PIN

  Summary:
    Enum of GFX Intf pins.
    
*/
/**
 * @brief This enum represents display interface pins
 * @details A interface pins are use to communicate with external
 * display controllers. The supported pins are defined here.
 */
typedef enum
{
    GFX_DISP_INTF_PIN_CS,        //Chip Select
    GFX_DISP_INTF_PIN_WR,        //Write Strobe
    GFX_DISP_INTF_PIN_RD,        //Read Strobe
    GFX_DISP_INTF_PIN_RSDC,      //Register Select or Data/Command
} GFX_DISP_INTF_PIN;

/** GFX_DISP_INTF_PIN_VALUE

  Summary:
    Enum of GFX Intf pin values.
    
*/
/**
 * @brief This enum represents display interface pin values
 * @details Interface pin values are used to set the status
 * of interface pin. These are defined here.
 */
typedef enum
{
    GFX_DISP_INTF_PIN_CLEAR = 0,
    GFX_DISP_INTF_PIN_SET = 1,
} GFX_DISP_INTF_PIN_VALUE;

/** GFX_Disp_Intf_Callback

  Summary:
    GFX Intf callback function type.
    
*/
/**
 * @brief This function represents display interface callback function.
 * @details The display interface callback is used to communicate the
 * status of transfer.
 */
typedef void (*GFX_Disp_Intf_Callback)(GFX_Disp_Intf,
                                       GFX_DISP_INTF_STATUS, 
                                       void *);

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

/** 
  Function:
    GFX_Disp_Intf_Open(GFX_Context * gfx)

  Summary:
    Opens the interface to the graphics display

  Description:
    This opens the interface to the graphics display (e.g., SPI, parallel).

  Parameters:
    None
 
  Returns:
    * GFX_Intf       - the interface handle, 0 if failed
*/
/**
 * @brief Open an interface to the graphics display.
 * @details Opens an interface to the graphics display (e.g., SPI, parallel).
 * @code
 * GFX_Disp_Intf handle = GFX_Disp_Intf_Open();
 * @endcode
 * @return the interface handle, 0 if failed.
 */
GFX_Disp_Intf GFX_Disp_Intf_Open(void);

/** 
  Function:
    void GFX_Disp_Intf_Close(GFX_Intf intf)

  Summary:
    Closes the interface to the graphics display

  Description:

  Parameters:
    intf    - the interface handle
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Close an interface to the graphics display.
 * @details Closes the the interface <span class="param">intf</span>.
 * @code
 * GFX_Disp_Intf intf;
 * GFX_Disp_Intf_Close(intf);
 * @endcode
 * @return void.
 */

void GFX_Disp_Intf_Close(GFX_Disp_Intf intf);

/** 
  Function:
    int GFX_Disp_Intf_PinControl(GFX_Disp_Intf intf, GFX_INTF_PIN pin, GFX_INTF_PIN_VALUE value)

  Summary:
    Sets the value of the GFX control pin

  Description:

  Parameters:
    intf - the interface handle
    pin - the control pin
    value - the value
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Set value of the GFX control pin.
 * @details Sets the value of <span class="param">pin</span> to
 * <span class="param">value</span> using <span class="param">intf</span>.
 * @code
 * GFX_Disp_Intf intf;
 * int res = GFX_Disp_Intf_PinControl(intf, pin, value);
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_PinControl(GFX_Disp_Intf intf, GFX_DISP_INTF_PIN pin, GFX_DISP_INTF_PIN_VALUE value);

/** 
  Function:
    int GFX_Disp_Intf_WriteCommand(GFX_Disp_Intf intf, uint8_t cmd)

  Summary:
    Asserts the RS/DCx control pin (LOW) and writes the 8-bit command

  Description:

  Parameters:
    intf - the interface handle
    cmd - the command
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Write command.
 * @details Writes a command <span class="param">cmd</span> using <span class="param">intf</span>.
 * @code
 * GFX_Disp_Intf intf;
 * int res = GFX_Disp_Intf_WriteCommand(intf, pin, value);
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_WriteCommand(GFX_Disp_Intf intf, uint8_t cmd);

/** 
  Function:
    int GFX_Disp_Intf_WriteCommandParm(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * parm, int num_parms)

  Summary:
    Writes a command and its parameters to the interface.

  Description:
    Asserts the RS/DCx control pin (LOW), writes the 8-bit command, 
    Deasserts the RS/DCx control pin (HIGH, writes the n-byte parameter)

  Parameters:
    intf - the interface handle
    cmd - the command
    parm - byte array containing the command parameters
    num_parms - the number for parameters
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Write command and parameter.
 * @details Writes a command <span class="param">cmd</span> and parameter <span class="param">parm</span>
 * using <span class="param">intf</span>. The number of parameters are specified
 * by <span class="param">num_param</span>.
 * @code
 * GFX_Disp_Intf intf;
 * uint8_t cmd;
 * uint8_t * parm;
 * int num_parms;
 * int res = GFX_Disp_Intf_WriteCommandParm(intf, cmd, parm, num_param);
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_WriteCommandParm(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * parm, int num_parms);

/** 
  Function:
    int GFX_Disp_Intf_WriteData(GFX_Disp_Intf intf, uint8_t * data, int bytes)

  Summary:
    Deasserts the RS/DCx control pin (HIGH) and writes the byte stream to the interface

  Description:

  Parameters:
    intf - the interface handle
    data - the byte stream
    bytes - the number of bytes in the data stream
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Write data bytes.
 * @details Writes the data byte stream <span class="param">data</span>
 * using <span class="param">intf</span>. The number of bytes are specified
 * by <span class="param">bytes</span>.
 * @remark Deasserts the RS/DCx control pin (HIGH) and writes
 * the byte stream to the interface.
 * @code
 * GFX_Disp_Intf intf;
 * uint8_t * data;
 * int bytes;
 * int res = GFX_Disp_Intf_WriteData(intf, data, bytes);
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_WriteData(GFX_Disp_Intf intf, uint8_t * data, int bytes);

/** 
  Function:
    int GFX_Disp_Intf_WriteData16(GFX_Disp_Intf intf, uint16_t * data, int num)

  Summary:
    Deasserts the RS/DCx control pin (HIGH) and writes the data to the interface
    in 16-bit chunks. Interface must support 16-bit wide data transfers, otherwise
    will return -1

  Description:

  Parameters:
    intf - the interface handle
    data - the data to write
    num - the number of bytes in the data stream
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed or not supported
*/
/**
 * @brief Write data words.
 * @details Writes data words <span class="param">data</span>
 * using <span class="param">intf</span>. The number of words to write are specified
 * by <span class="param">num</span>.
 * @remark Deasserts the RS/DCx control pin (HIGH) and writes the data to the
 * interface  in 16-bit chunks. Interface must support 16-bit wide data transfers,
 * otherwise will return -1.
 * @code
 * GFX_Disp_Intf intf;
 * uint16_t * data;
 * int num;
 * int res = GFX_Disp_Intf_WriteData16(intf, data, num);
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_WriteData16(GFX_Disp_Intf intf, uint16_t * data, int num);

/** 
  Function:
    int GFX_Disp_Intf_ReadData(GFX_Disp_Intf intf, uint8_t * data, int bytes)

  Summary:
    Deasserts the RS/DCx control pin (HIGH) and reads a bytes from the interface

  Description:

  Parameters:
    intf - the interface handle
    data - the destination buffer
    bytes - the number of bytes to read
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Read data words.
 * @details Reads data words <span class="param">data</span>
 * using <span class="param">intf</span>. The number of words to read are specified
 * by <span class="param">num_data</span>.
 * @remark Deasserts the RS/DCx control pin (HIGH) and writes the data to the
 * interface  in 16-bit chunks. Interface must support 16-bit wide data transfers,
 * otherwise will return -1.
 * @code
 * GFX_Disp_Intf intf;
 * uint8_t * data;
 * int bytes;
 * int res = GFX_Disp_Intf_ReadData(intf, data, bytes);
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_ReadData(GFX_Disp_Intf intf, uint8_t * data, int bytes);

/** 
  Function:
    int GFX_Disp_Intf_ReadCommandData(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * data, int num_data)

  Summary:
    Writes a byte command (cmd) to the interface and read num_data bytes from the interface
    RS/DCx control pin is asserted (LOW) when the cmd is sent, and deasserted (HIGH) when the data is read
    The RD strobe is asserted for each byte read.

  Description:

  Parameters:
    intf - the interface handle
    cmd - the 8-bit command to send
    data - the destination byte buffer
    bytes - the number of bytes to read
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Read command data.
 * @details Reads command <span class="param">cmd</span>  and its <span class="param">data</span>
 * using <span class="param">intf</span>. The number of words to read are specified
 * by <span class="param">num_data</span>.
 * @remark Writes a byte command (cmd) to the interface and read num_data bytes from the
 * interface RS/DCx control pin is asserted (LOW) when the cmd is sent, and deasserted
 * (HIGH) when the data is read. The RD strobe is asserted for each byte read.
 * @code
 * GFX_Disp_Intf intf;
 * uint8_t cmd;
 * uint8_t * data;
 * int bytes;
 * int num_data;
 * int res = GFX_Disp_Intf_ReadCommandData(intf, cmd, data, num_data);
 * @param intf
 * @param cmd
 * @param data
 * @param num_data
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_ReadCommandData(GFX_Disp_Intf intf, uint8_t cmd, uint8_t * data, int num_data);

/** 
  Function:
    int GFX_Disp_Intf_ReadData16(GFX_Disp_Intf intf, uint16_t * data, int num)

  Summary:
    Deasserts the RS/DCx control pin (HIGH) and reads 16-bit data chunks from the 
    interface.

  Description:
    Interface must support 16-bit data reads, otherwise -1 will
    be returned.

  Parameters:
    intf - the interface handle
    data - the destination buffer
    bytes - the number of chunks to read
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed or not supported
*/
/**
 * @brief Read command data word.
 * @details Reads command <span class="param">cmd</span>  and words <span class="param">data</span>
 * using <span class="param">intf</span>. The number of words to read are specified
 * by <span class="param">num</span>.
 * @remark Deasserts the RS/DCx control pin (HIGH) and reads 16-bit data chunks
 * from the interface. Interface must support 16-bit data reads, otherwise -1
 * will  be returned.
 * @code
 * GFX_Disp_Intf intf;
 * uint16_t * data;
 * int num;
 * int res = GFX_Disp_Intf_ReadData16(intf, cmd, data, num_data);
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_ReadData16(GFX_Disp_Intf intf, uint16_t * data, int num);

/** 
  Function:
    int GFX_Disp_Intf_Write(GFX_Disp_Intf intf, uint8_t * data, int bytes)

  Summary:
    Writes the byte stream to the interface, control pins are not set

  Description:

  Parameters:
    intf - the interface handle
    data - the byte stream
    bytes - the number of bytes in the data stream
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Write byte stream.
 * @details Writes the byte stream <span class="param">data</span>
 * using <span class="param">intf</span>. The number of words to read are specified
 * by <span class="param">bytes</span>.
 * @code
 * GFX_Disp_Intf intf;
 * uint8_t * data;
 * int bytes;
 * int res = GFX_Disp_Intf_Write(intf, data, bytes);
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_Write(GFX_Disp_Intf intf, uint8_t * data, int bytes);

/** 
  Function:
    int GFX_Disp_Intf_Read(GFX_Disp_Intf intf, uint8_t * data, int bytes)

  Summary:
    Reads a byte stream from the interface, control pins are not set

  Description:

  Parameters:
    intf - the interface handle
    data - the byte stream
    bytes - the number of bytes in the data stream
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Read byte stream.
 * @details Reads the byte stream <span class="param">data</span>
 * using <span class="param">intf</span>. The number of words to read are specified
 * by <span class="param">bytes</span>.
 * @code
 * GFX_Disp_Intf intf;
 * uint8_t * data;
 * int bytes;
 * int res = GFX_Disp_Intf_Read(intf, data, bytes);
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_Read(GFX_Disp_Intf intf, uint8_t * data, int bytes);

/** 
  Function:
    GFX_Disp_Intf_WriteDataByte(GFX_Disp_Intf intf, uint8_t data);

  Summary:
    Writes a single byte, RSDC control pin is set

  Description:

  Parameters:
    intf - the interface handle
    data - the byte stream
    bytes - the number of bytes in the data stream
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Write byte.
 * @details Writes a byte <span class="param">data</span>
 * using <span class="param">intf</span>.
 * @code
 * GFX_Disp_Intf intf;
 * uint8_t * data;
 * int res = GFX_Disp_Intf_WriteDataByte(intf, data);
 * @endcode
 * @return 0 if successful, otherwise -1.
 */
int GFX_Disp_Intf_WriteDataByte(GFX_Disp_Intf intf, uint8_t data);

/** 
  Function:
    int GFX_Disp_Intf_Ready(GFX_Disp_Intf intf)

  Summary:
    Checks if the interface is ready

  Description:

  Parameters:
    intf - the interface handle
 
  Returns:
    * 1      - Interface is idle and ready for transfer
    * 0      - Interface is busy and unavailable
*/
/**
 * @brief Check interface ready status.
 * @details Checks if interface is ready for transfer
 * using <span class="param">intf</span>.
 * @code
 * GFX_Disp_Intf intf;
 * int res = GFX_Disp_Intf_Ready(intf);
 * @endcode
 * @return 0 if busy, otherwise 1 for transfer is ready.
 */
int GFX_Disp_Intf_Ready(GFX_Disp_Intf intf);

/** 
  Function:
    GFX_Disp_Intf_Set_Callback(GFX_Disp_Intf intf, GFX_Disp_Intf_Callback cb, void * parm)

  Summary:
    Registers a callback function to GFX Display Interface. This is supported only by interfaces
    that can do asynchronous/non-blocking transfers.

  Description:

  Parameters:
    intf - the interface handle
    cb - the callback function
    parm - the function parameter
 
  Returns:
    * 0       - Operation successful
    * -1       - Operation failed
*/
/**
 * @brief Set interface callback.
 * @details Sets the interface callback to <span class="param">cb</span> with arguments
 * <span class="param">parm</span>
 * using <span class="param">intf</span>.
 * @code
 * GFX_Disp_Intf intf;
 * GFX_Disp_Intf_Callback cb;
 * void * parm;
 * int res = GFX_Disp_Intf_Set_Callback(intf, cb, parm);
 * @endcode
 * @return 0 if busy, otherwise 1 for transfer is ready.
 */
int GFX_Disp_Intf_Set_Callback(GFX_Disp_Intf intf, GFX_Disp_Intf_Callback cb, void * parm);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _DRV_GFX_DISP_INTF_H */

/* *****************************************************************************
 End of File
 */
