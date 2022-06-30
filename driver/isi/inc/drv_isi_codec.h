#ifndef DRV_ISI_CODEC_H
#define DRV_ISI_CODEC_H

#include "driver/isi/drv_isi_common.h"

/**
 * @brief Defines the function pointer type needed to get
 *        callbacks from the ISI driver codec path
 */
typedef void (*DRV_ISI_CodecFrameReady_Callback)(uint8_t* buf);

/**
 * @brief Queries the codec path enabled state
 * @details Queries the codec path enabled state
 * @return true if enabled
 */
bool DRV_ISI_CodecPathIsEnabled(void);

/**
 * @brief Enables the codec path
 * @details Enables the codec path
 * @return true if successful
 */
bool DRV_ISI_CodecPathEnable(void);

/**
 * @brief Disables the codec path
 * @details Disables the codec path
 * @return true if successful
 */
bool DRV_ISI_CodecPathDisable(void);

/**
 * @brief Adds a buffer to the codec frame buffer chain
 * @details The codec pipeline uses a buffer chain for iterative decoding
 *          The number of buffers that can be added is limited by the
 *          definition: DRV_ISI_MAX_FRAMEBUFFER_DESCRIPTOR
 * @param buf the pointer to a buffer to add to the chain
 *            the buffer must be sized properly to match the input sensor
 *            codec data requirements
 * @return true if successful
 */
bool DRV_ISI_AddCodecFrameBuffer(uint8_t* buf);

/**
 * @brief Clears the codec path buffer chain
 * @details This function clears the existing codec path descriptor information
 * @return true if successful
 */
bool DRV_ISI_ClearCodecFrameBufferList(void);

/**
 * @brief Clears the codec path buffer chain
 * @details This function clears the existing codec path descriptor information
 * @return true if successful
 */
uint8_t* DRV_ISI_GetLatestCodecFrame(void);

/**
 * @brief Sets the codec frame ready callback pointer
 * @details The provided callback is invoked when a codec frame is ready to be
 *          handled
 * @return true if successful
 */
bool DRV_ISI_SetCodecFrameReadyCallback(DRV_ISI_CodecFrameReady_Callback cb);


#endif // DRV_ISI_CODEC_H
