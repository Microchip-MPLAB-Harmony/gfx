#ifndef DRV_ISI_CONFIG_H
#define DRV_ISI_CONFIG_H

#include "peripheral/isi/plib_isi.h"

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Struct defining the input sensor color data mode
 * @details Indicates what kind of data the image sensor will be sending
 *          Refer to the datasheet for more details
 */
typedef enum
{
    DRV_ISI_INPCOLORMODE_YCC_422_DEFAULT = 0,
    DRV_ISI_INPCOLORMODE_YCC_422_MODE1,
    DRV_ISI_INPCOLORMODE_YCC_422_MODE2,
    DRV_ISI_INPCOLORMODE_YCC_422_MODE3,
    DRV_ISI_INPCOLORMODE_RGB_888_DEFAULT,
    DRV_ISI_INPCOLORMODE_RGB_888_MODE1,
    DRV_ISI_INPCOLORMODE_RGB_888_MODE2,
    DRV_ISI_INPCOLORMODE_RGB_888_MODE3,
    DRV_ISI_INPCOLORMODE_RGB_888_DEFAULT_SWAP,
    DRV_ISI_INPCOLORMODE_RGB_888_MODE1_SWAP,
    DRV_ISI_INPCOLORMODE_RGB_888_MODE2_SWAP,
    DRV_ISI_INPCOLORMODE_RGB_888_MODE3_SWAP,
    DRV_ISI_INPCOLORMODE_RGB_565_DEFAULT,
    DRV_ISI_INPCOLORMODE_RGB_565_MODE1,
    DRV_ISI_INPCOLORMODE_RGB_565_MODE2,
    DRV_ISI_INPCOLORMODE_RGB_565_MODE3,
    DRV_ISI_INPCOLORMODE_RGB_565_DEFAULT_SWAP,
    DRV_ISI_INPCOLORMODE_RGB_565_MODE1_SWAP,
    DRV_ISI_INPCOLORMODE_RGB_565_MODE2_SWAP,
    DRV_ISI_INPCOLORMODE_RGB_565_MODE3_SWAP,
    DRV_ISI_INPCOLORMODE_GS_8BIT,
    DRV_ISI_INPCOLORMODE_GS_12BIT,
} DRV_ISI_InputColorMode;

/**
 * @brief Struct defining the AHB burst mode setting
 * @details Struct defining the AHB burst mode setting
 */
typedef enum
{
    DRV_ISI_THMASK_BEATS_4 = ISI_THMASK_BEATS_4,
    DRV_ISI_THMASK_BEATS_8 = ISI_THMASK_BEATS_8,
    DRV_ISI_THMASK_BEATS_16 = ISI_THMASK_BEATS_16,            
} DRV_ISI_ThresholdMask;

/**
 * @brief Struct defining the sensor synchronization mode
 * @details Struct defining the sensor synchronization mode
 */
typedef enum
{
    DRV_ISI_EMSYNC_HSYNC_VSYNC = 0,
    DRV_ISI_EMSYNC_SAV_EAV = 1,
} DRV_ISI_EmbeddedSynchronizationMode;

/**
 * @brief Struct defining the pixel clock polarity mode
 * @details Struct defining the pixel clock polarity mode
 */
typedef enum
{
    DRV_ISI_PIXELCLOCKPOL_SAMPLE_RISING = 0,
    DRV_ISI_PIXELCLOCKPOL_SAMPLE_FALLING = 1,            
} DRV_ISI_PixelClockPolarity;

/**
 * @brief Struct defining synchronization modes
 * @details Struct defining synchronization modes
 */
typedef enum
{
    DRV_ISI_SYNC_ACTIVE_HIGH = 0,
    DRV_ISI_SYNC_ACTIVE_LOW = 1,
} DRV_ISI_SyncPolarity;

/**
 * @brief Struct defining the YCC/YUV to RGB conversion matrix
 * @details Struct defining the YCC/YUV to RGB conversion matrix
 */
typedef struct
{
    uint8_t c0;
    uint8_t c1;
    uint8_t c2;
    uint8_t c3;
    uint8_t c4;
    bool cbOffset; // true = 128 offset
    bool crOffset; // true = 128 offset
    bool yOffset; // true = 16 offset
} DRV_ISI_YCCtoRGBConfiguration;

/**
 * @brief Struct defining the RGB to YCC/YUV conversion matrix
 * @details Struct defining the RGB to YCC/YUV conversion matrix
 */
typedef struct
{
    uint8_t c0;
    uint8_t c1;
    uint8_t c2;
    uint8_t c3;
    uint8_t c4;
    uint8_t c5;
    uint8_t c6;
    uint8_t c7;
    uint8_t c8;
    bool rOffset; // true = 16 offset
    bool gOffset; // true = 128 offset
    bool bOffset; // true = 128 offset
    
} DRV_ISI_RGBtoYCCConfiguration;

/**
 * @brief Gets the start of frame delay setting
 * @details Gets the start of frame delay setting
 * @return the SFD setting value
 */
uint32_t DRV_ISI_GetStartOfFrameDelay(void);

/**
 * @brief Sets the start of frame delay setting
 * @details Sets the start of frame delay setting
 * @param dly the SFD setting value
 * @return true if successful
 */
bool DRV_ISI_SetStartOfFrameDelay(uint32_t dly);

/**
 * @brief Gets the start of line delay setting
 * @details Gets the start of line delay setting
 * @return the SLD setting value
 */
uint32_t DRV_ISI_GetStartOfLineDelay(void);

/**
 * @brief Sets the start of line delay setting
 * @details Sets the start of line delay setting
 * @param dly the SLD setting value
 * @return true if successful
 */
bool DRV_ISI_SetStartOfLineDelay(uint32_t dly);

/**
 * @brief Gets the threshold mask setting
 * @details Gets the threshold mask setting
 * @return the threshold setting value
 */
DRV_ISI_ThresholdMask DRV_ISI_GetThresholdMask(void);

/**
 * @brief Sets the threshold mask setting
 * @details Sets the threshold mask setting
 * @param msk the threshold mask value
 * @return true if successful
 */
bool DRV_ISI_SetThresholdMask(DRV_ISI_ThresholdMask msk);

/**
 * @brief Gets the Full Mode setting value
 * @details Gets the Full Mode setting value
 * @return the full mode setting value
 */
bool DRV_ISI_GetFullMode(void);

/**
 * @brief Sets the Full Mode setting value
 * @details Sets the Full Mode setting value
 * @param fm true if enable
 * @return true if successful
 */
bool DRV_ISI_SetFullMode(bool fm);

/**
 * @brief Gets the automatic codec request mode setting
 * @details Gets the automatic codec request mode setting
 * @return the request mode setting value
 */
bool DRV_ISI_GetCodecRequestDisabled(void);

/**
 * @brief Sets automatic codec request mode value
 * @details If disabled then the codec pipeline requires a manual restart
 *          each time it is used
 * @param fm true if disable
 * @return true if successful
 */
bool DRV_ISI_SetCodecRequestDisabled(bool dis);

/**
 * @brief Gets the frame skip setting
 * @details Gets the frame skip setting
 * @return the frame skip value
 */
uint32_t DRV_ISI_GetFrameSkip(void);

/**
 * @brief Sets the frame skip setting
 * @details Sets the frame skip setting
 * @param fr the number of frames to skip each operation
 * @return true if successful
 */
bool DRV_ISI_SetFrameSkip(uint32_t fr);

/**
 * @brief Gets the CRC setting
 * @details Gets the CRC setting
 * @return true if CRC mode is enabled
 */
bool DRV_ISI_GetCRCEnabled(void);

/**
 * @brief Sets the CRC setting
 * @details Sets the CRC setting
 * @param en true to enable CRC
 * @return true if successful
 */
bool DRV_ISI_SetCRCEnabled(bool en);

/**
 * @brief Gets the embedded synchronization mode setting
 * @details Gets the embedded synchronization mode setting
 * @return the sync mode setting
 */
DRV_ISI_EmbeddedSynchronizationMode DRV_ISI_GetEmbeddedSynchronizationMode(void);

/**
 * @brief Sets the embedded synchronization mode setting
 * @details Sets the embedded synchronization mode setting
 * @param md the synchronization mode
 * @return true if successful
 */
bool DRV_ISI_SetEmbeddedSynchronizationMode(DRV_ISI_EmbeddedSynchronizationMode md);

/**
 * @brief Gets the pixel clock polarity setting
 * @details Gets the pixel clock polarity setting
 * @return the pixel clock polarity mode
 */
DRV_ISI_PixelClockPolarity DRV_ISI_GetPixelClockPolarity(void);

/**
 * @brief Sets the pixel clock polarity setting
 * @details Sets the pixel clock polarity setting
 * @param pol the polarity mode
 * @return true if successful
 */
bool DRV_ISI_SetPixelClockPolarity(DRV_ISI_PixelClockPolarity pol);

/**
 * @brief Gets the vsync polarity setting
 * @details Gets the vsync polarity setting
 * @return the vsync polarity mode
 */
DRV_ISI_SyncPolarity DRV_ISI_GetVSyncPolarity(void);

/**
 * @brief Sets the vsync polarity setting
 * @details Sets the vsync polarity setting
 * @param pol the polarity mode
 * @return true if successful
 */
bool DRV_ISI_SetVSyncPolarity(DRV_ISI_SyncPolarity pol);

/**
 * @brief Gets the hsync polarity setting
 * @details Gets the hsync polarity setting
 * @return the hsync polarity mode
 */
DRV_ISI_SyncPolarity DRV_ISI_GetHSyncPolarity(void);

/**
 * @brief Sets the hsync polarity setting
 * @details Sets the hsync polarity setting
 * @param pol the polarity mode
 * @return true if successful
 */
bool DRV_ISI_SetHSyncPolarity(DRV_ISI_SyncPolarity pol);

/**
 * @brief Gets the image sensor input color mode
 * @details Gets the image sensor input color mode
 * @return the input color mode
 */
DRV_ISI_InputColorMode DRV_ISI_GetInputColorMode(void);

/**
 * @brief Sets the image sensor input color mode
 * @details Sets the image sensor input color mode
 * @param mode the polarity mode
 * @return true if successful
 */
bool DRV_ISI_SetInputColorMode(DRV_ISI_InputColorMode mode);

/**
 * @brief Gets the image sensor input image size
 * @details Gets the image sensor input image size
 * @param w pointer to an uint to hold the width
 * @param h pointer to an uint to hold the height
 * @return true if the size was returned
 */
bool DRV_ISI_GetInputSize(uint32_t* w,
                          uint32_t* h);

/**
 * @brief Sets the image sensor input image size
 * @details Sets the image sensor input image size
 * @param w the image sensor width
 * @param h the image sensor height
 * @return true if successful
 */
bool DRV_ISI_SetInputSize(uint32_t w,
                          uint32_t h);

/**
 * @brief Gets the preview image size
 * @details Gets the preview image size
 * @param w pointer to an uint to hold the width
 * @param h pointer to an uint to hold the height
 * @return true if the size was returned
 */
bool DRV_ISI_GetPreviewSize(uint32_t* w,
                            uint32_t* h);

/**
 * @brief Sets the preview image size
 * @details Sets the preview image size
 * @param w the desired preview image width
 * @param h the desired preview image height
 * @return true if successful
 */
bool DRV_ISI_SetPreviewSize(uint32_t w,
                            uint32_t h);

/**
 * @brief Gets the codec to preview image decimation factor
 * @details The preview image can be reduced in size to reduce bus bandwidth
 *          and draw time.  This value is based on a 1/16 reduction factor
 *          e.g. 32 == 50% reduction
 * @return uint32_t the decimation factor value
 */
uint32_t DRV_ISI_GetDecimationFactor(void);

/**
 * @brief Sets the codec to preview image decimation factor
 * @details The preview image can be reduced in size to reduce bus bandwidth
 *          and draw time.  This value is based on a 1/16 reduction factor
 *          e.g. 32 == 50% reduction
 * @param dec the decimation factor value
 * @return true if the size was returned
 */
bool DRV_ISI_SetDecimationFactor(uint32_t dec);

/**
 * @brief Decimation factor calculation helper function
 * @details This function will calculate the appropriate decimation factor
 *          given the image sensor and preview image sizes
 * @param sensorW the sensor image width
 * @param sensorH the sensor image height
 * @param prevW the preview image width
 * @param prevH the preview image height
 * @return uint32_t the decimation factor value
 */
uint32_t DRV_ISI_CalculateDecimationFactor(uint32_t sensorW,
                                           uint32_t sensorH,
                                           uint32_t prevW,
                                           uint32_t prevH);

/**
 * @brief Gets the current YCC to RGB conversion matrix
 * @details Gets the current YCC to RGB conversion matrix
 * @param cfg pointer to the configuration matrix in which to store the values
 * @return true if cfg holds valid values
 */
bool DRV_ISI_GetYCCtoRGBConversionConfiguration(DRV_ISI_YCCtoRGBConfiguration* cfg);

/**
 * @brief Sets the current YCC to RGB conversion matrix
 * @details Sets the current YCC to RGB conversion matrix
 * @param cfg pointer to the configuration matrix from which to load the values
 * @return true if successful
 */
bool DRV_ISI_SetYCCtoRGBConversionConfiguration(const DRV_ISI_YCCtoRGBConfiguration* cfg);

/**
 * @brief Gets the current RGB to YCC conversion matrix
 * @details Gets the current RGB to YCC conversion matrix
 * @param cfg pointer to the configuration matrix in which to store the values
 * @return true if cfg holds valid values
 */
bool DRV_ISI_GetRGBtoYCCConversionConfiguration(DRV_ISI_RGBtoYCCConfiguration* cfg);

/**
 * @brief Gets the current RGB to YCC conversion matrix
 * @details Gets the current RGB to YCC conversion matrix
 * @param cfg pointer to the configuration matrix from which to load the values
 * @return true if successful
 */
bool DRV_ISI_SetRGBtoYCCConversionConfiguration(const DRV_ISI_RGBtoYCCConfiguration* cfg);

#endif // DRV_ISI_CONFIG_H
