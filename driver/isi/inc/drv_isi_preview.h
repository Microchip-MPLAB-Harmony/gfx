#ifndef DRV_ISI_PREVIEW_H
#define DRV_ISI_PREVIEW_H

#include "driver/isi/drv_isi_common.h"

/**
 * @brief Defines the function pointer type needed to get
 *        callbacks from the ISI driver preview path
 */
typedef void (*DRV_ISI_PreviewFrameReady_Callback)(uint8_t* buf);

/**
 * @brief Queries the preview path enabled state
 * @details Queries the preview path enabled state
 * @return true if enabled
 */
bool DRV_ISI_PreviewPathIsEnabled(void);

/**
 * @brief Enables the preview path
 * @details Enables the preview path
 * @return true if successful
 */
bool DRV_ISI_PreviewPathEnable(void);

/**
 * @brief Disables the preview path
 * @details Disables the preview path
 * @return true if successful
 */
bool DRV_ISI_PreviewPathDisable(void);

/**
 * @brief Adds a buffer to the preview frame buffer chain
 * @details The preview pipeline uses a buffer chain for iterative decoding
 *          The number of buffers that can be added is limited by the
 *          definition: DRV_ISI_MAX_FRAMEBUFFER_DESCRIPTOR
 * @param buf the pointer to a buffer to add to the chain
 *            the buffer must be sized properly to match the input sensor
 *            preview data requirements
 * @return true if successful
 */
bool DRV_ISI_AddPreviewFrameBuffer(uint8_t* buf);

/**
 * @brief Clears the preview path buffer chain
 * @details This function clears the existing preview path descriptor information
 * @return true if successful
 */
bool DRV_ISI_ClearPreviewFrameBufferList(void);

/**
 * @brief Clears the preview path buffer chain
 * @details This function clears the existing preview path descriptor information
 * @return true if successful
 */
uint8_t* DRV_ISI_GetLatestPreviewFrame(void);

/**
 * @brief Sets the preview frame ready callback pointer
 * @details The provided callback is invoked when a preview frame is ready to be
 *          handled
 * @return true if successful
 */
bool DRV_ISI_SetPreviewFrameReadyCallback(DRV_ISI_PreviewFrameReady_Callback cb);


#endif // DRV_ISI_PREVIEW_H
