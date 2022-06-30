//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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
//DOM-IGNORE-END

#ifndef DRV_ISI_H
#define DRV_ISI_H

#include "driver/driver.h"
#include "driver/driver_common.h"

#include "driver/isi/drv_isi_init.h"
#include "driver/isi/drv_isi_codec.h"
#include "driver/isi/drv_isi_config.h"
#include "driver/isi/drv_isi_preview.h"

#include "peripheral/isi/plib_isi.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif
// DOM-IGNORE-END

/**
 * @brief Initializes the ISI driver
 * @details Initializes the ISI driver
 */
void DRV_ISI_Init(void);

/**
 * @brief Runs the ISI tasks function
 * @details Runs the ISI tasks function
 */
void DRV_ISI_Tasks(void);

/**
 * @brief Queries the ISI peripheral enabled state
 * @details Queries the ISI peripheral enabled state
 * @return returns true if the ISI peripheral is enabled
 */
bool DRV_ISI_IsEnabled(void);

/**
 * @brief Enables the ISI peripheral
 * @details Enables the ISI peripheral
 * @return returns true if successful
 */
bool DRV_ISI_Enable(void);

/**
 * @brief Disables the ISI peripheral
 * @details Disables the ISI peripheral
 * @return returns true if successful
 */
bool DRV_ISI_Disable(void);

/**
 * @brief Resets the ISI peripheral
 * @details Resets the ISI peripheral
 * @return returns true if successful
 */
bool DRV_ISI_Reset(void);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END    
    
#endif // DRV_ISI_H
