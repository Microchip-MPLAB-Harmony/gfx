#ifndef N2D_HAL_H
#define N2D_HAL_H

#include "nano2D_types.h"
#include "nano2D_enum.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function pointer type for timer callbacks used by the GPU driver.
 *
 * This function will be called when a timer expires. The `data` argument is
 * user-defined and passed during timer creation.
 *
 * @param data A pointer to user-defined data.
 */
typedef void (*n2d_timer_func)(n2d_pointer data);

/**
 * @brief Creates a software timer instance.
 *
 * This function allocates and initializes a timer that can be started and stopped.
 *
 * @param[in]  function The callback function to invoke when the timer expires.
 * @param[in]  data     A pointer to user-defined data passed to the callback.
 * @param[out] timer    A pointer to where the created timer handle will be stored.
 *
 * @return N2D_SUCCESS on success, or an appropriate error code on failure.
 */
n2d_error_t n2d_hal_create_timer(n2d_timer_func function, n2d_pointer data, n2d_pointer *timer);

/**
 * @brief Destroys a previously created timer.
 *
 * Stops and frees resources associated with the timer.
 *
 * @param[in] timer Handle to the timer returned by n2d_hal_create_timer().
 *
 * @return N2D_SUCCESS on success, or N2D_INVALID_ARGUMENT if the timer is NULL.
 */
n2d_error_t n2d_hal_destroy_timer(n2d_pointer timer);

/**
 * @brief Starts a timer with a specified delay.
 *
 * Begins the countdown for a one-shot timer. When the delay expires, the
 * registered callback is invoked.
 *
 * @param[in] timer     Timer handle returned by n2d_hal_create_timer().
 * @param[in] delay_ms  Delay in milliseconds before the callback is invoked.
 *
 * @return N2D_SUCCESS on success, or an appropriate error code.
 */
n2d_error_t n2d_hal_start_timer(n2d_pointer timer, uint32_t delay_ms);

/**
 * @brief Stops a running timer.
 *
 * Cancels any pending callback invocation and disables the timer.
 *
 * @param[in] timer Timer handle returned by n2d_hal_create_timer().
 *
 * @return N2D_SUCCESS on success, or N2D_INVALID_ARGUMENT if the timer is NULL.
 */
n2d_error_t n2d_hal_stop_timer(n2d_pointer timer);

/**
 * @brief Delay in microseconds.
 *
 * This function must be implemented by the application for precise GPU delays.
 */
void n2d_hal_delay_us(n2d_uint_t us);

/**
 * @brief Delay in microseconds.
 *
 * This function must be implemented by the application for precise GPU delays.
 */
void n2d_hal_delay_ms(n2d_uint_t ms);

/**
 * @brief Powers the GPU ON or OFF.
 *
 * This function must be implemented by the application. It is called by the GPU driver
 * to enable or disable the hardware power and clock domains.
 *
 * @param power true to enable, false to disable
 */
void n2d_hal_set_gpu_power(n2d_bool_t power);

/**
 * @brief Get the current system tick as a 64-bit value.
 *
 * This function must be implemented by the application. It is used by the GPU driver
 * for timing.
 *
 * @return Current system tick as a 64-bit unsigned integer.
 */
n2d_uint64_t n2d_hal_get_ticks(void);


/**
 * @brief Creates a new signal.
 *
 * Allocates memory for a signal object and initializes it in either manual-reset
 * or auto-reset mode.
 *
 * @param[out] signal       Output pointer to the created signal object.
 * @param[in]  manual_reset If true, signal remains set until explicitly reset.
 *                          If false, signal auto-resets after a wait.
 *
 * @return N2D_SUCCESS on success, N2D_OUT_OF_MEMORY on allocation failure.
 */
n2d_error_t n2d_hal_signal_create(n2d_pointer **signal, n2d_bool_t manual_reset);

/**
 * @brief Destroys a previously created signal object.
 *
 * Frees memory associated with the signal object.
 *
 * @param[in,out] signal Pointer to the signal object handle to destroy.
 *
 * @return N2D_SUCCESS on success, or N2D_INVALID_ARGUMENT if the signal is NULL.
 */
n2d_error_t n2d_hal_signal_destroy(n2d_pointer signal);

/**
 * @brief Waits for a signal to be set.
 *
 * This function waits for the signal to become set. If in auto-reset mode,
 * the signal will automatically clear after one wait.
 *
 * @param[in] signal      Signal object to wait on.
 * @param[in] timeout_ms  Timeout in milliseconds.
 *                        Use 0 for polling, or UINT32_MAX for infinite wait.
 *
 * @return N2D_SUCCESS if signaled, N2D_TIMEOUT if timed out, or N2D_INVALID_ARGUMENT.
 */
n2d_error_t n2d_hal_signal_wait(n2d_pointer signal, uint32_t timeout_ms);

/**
 * @brief Sets or clears the signal state.
 *
 * Setting the signal will notify any waiting task or poll loop. If in
 * auto-reset mode, the signal will clear automatically after one wait.
 *
 * @param[in] signal Signal object to modify.
 * @param[in] state  Set to true to signal, or false to reset.
 *
 * @return N2D_SUCCESS on success, N2D_INVALID_ARGUMENT if the signal is NULL.
 */
n2d_error_t n2d_hal_signal_set(n2d_pointer signal, n2d_bool_t state);

#ifdef __cplusplus
}
#endif

#endif // N2D_HAL_H
