#include <stdlib.h>
#include "nano2D_hal.h"
#include "definitions.h"

typedef struct hal_timer
{
    SYS_TIME_HANDLE handle;
    n2d_timer_func function;
    n2d_pointer data;
} hal_timer_t;

typedef struct n2d_hal_signal
{
    volatile bool signaled;
    bool manual_reset;
} n2d_hal_signal_t;

static void n2d_hal_timer_callback(uintptr_t context)
{
    hal_timer_t *t = (hal_timer_t *)context;
    if (t && t->function)
    {
        t->function(t->data);
    }
}

n2d_error_t n2d_hal_create_timer(n2d_timer_func function, n2d_pointer data, n2d_pointer *timer)
{
    if (!function || !timer)
        return N2D_INVALID_ARGUMENT;

    hal_timer_t *t = (hal_timer_t *)malloc(sizeof(hal_timer_t));
    if (!t)
        return N2D_OUT_OF_RESOURCES;

    t->function = function;
    t->data = data;
    t->handle = SYS_TIME_HANDLE_INVALID;

    *timer = (n2d_pointer)t;
    return N2D_SUCCESS;
}

n2d_error_t n2d_hal_destroy_timer(n2d_pointer timer)
{
    if (!timer)
        return N2D_INVALID_ARGUMENT;

    hal_timer_t *t = (hal_timer_t *)timer;

    if (t->handle != SYS_TIME_HANDLE_INVALID)
    {
        SYS_TIME_TimerDestroy(t->handle);
    }

    free(t);
    return N2D_SUCCESS;
}

n2d_error_t n2d_hal_start_timer(n2d_pointer timer, uint32_t delay_ms)
{
    if (!timer)
        return N2D_INVALID_ARGUMENT;

    hal_timer_t *t = (hal_timer_t *)timer;

    t->handle = SYS_TIME_CallbackRegisterMS(
        n2d_hal_timer_callback,
        (uintptr_t)t,
        delay_ms,
        SYS_TIME_SINGLE
    );

    if (t->handle == SYS_TIME_HANDLE_INVALID)
        return N2D_OUT_OF_RESOURCES;

    return N2D_SUCCESS;
}

n2d_error_t n2d_hal_stop_timer(n2d_pointer timer)
{
    if (!timer)
        return N2D_INVALID_ARGUMENT;

    hal_timer_t *t = (hal_timer_t *)timer;

    if (t->handle != SYS_TIME_HANDLE_INVALID)
    {
        SYS_TIME_TimerDestroy(t->handle);
        t->handle = SYS_TIME_HANDLE_INVALID;
    }

    return N2D_SUCCESS;
}

void n2d_hal_delay_us(n2d_uint_t us)
{
    SYS_TIME_HANDLE hdl = SYS_TIME_HANDLE_INVALID;
    SYS_TIME_DelayUS(us, &hdl);
    while (!SYS_TIME_DelayIsComplete(hdl));
}

void n2d_hal_delay_ms(n2d_uint_t ms)
{
    SYS_TIME_HANDLE hdl = SYS_TIME_HANDLE_INVALID;
    SYS_TIME_DelayMS(ms, &hdl);
    while (!SYS_TIME_DelayIsComplete(hdl));
}

void n2d_hal_set_gpu_power(n2d_bool_t power)
{
    if (power == N2D_TRUE)
    {
        PMC_REGS->PMC_PCR = PMC_PCR_CMD(1) |
                            PMC_PCR_PID(ID_GPU2DC) |
                            PMC_PCR_EN(1) |
                            PMC_PCR_GCLKEN(1) |
                            PMC_PCR_GCLKDIV(0) |
                            PMC_PCR_GCLKCSS(PMC_PCR_GCLKCSS_GPUPLL_Val);
    }
    else
    {
        PMC_REGS->PMC_PCR = PMC_PCR_CMD(1) |
                            PMC_PCR_PID(ID_GPU2DC) |
                            PMC_PCR_EN(0) |
                            PMC_PCR_GCLKEN(0);
    }
}

n2d_uint64_t n2d_hal_get_ticks(void)
{
    return (n2d_uint64_t)SYS_TIME_Counter64Get();
}

n2d_error_t n2d_hal_signal_create(n2d_pointer **signal, n2d_bool_t manual_reset)
{
    if (!signal) return N2D_INVALID_ARGUMENT;

    n2d_hal_signal_t *s = (n2d_hal_signal_t *)malloc(sizeof(n2d_hal_signal_t));
    if (!s) return N2D_OUT_OF_MEMORY;

    s->signaled = false;
    s->manual_reset = manual_reset;

    *signal = (n2d_pointer)s;

    return N2D_SUCCESS;
}

n2d_error_t n2d_hal_signal_destroy(n2d_pointer signal)
{
    if (!signal) return N2D_INVALID_ARGUMENT;

    free(signal);

    return N2D_SUCCESS;
}

n2d_error_t n2d_hal_signal_wait(n2d_pointer signal, uint32_t timeout_ms)
{
    if (!signal) return N2D_INVALID_ARGUMENT;

    n2d_hal_signal_t *s = (n2d_hal_signal_t *)signal;
    uint64_t start = n2d_hal_get_ticks();

    while (!s->signaled)
    {
        if (timeout_ms != UINT32_MAX && ((n2d_hal_get_ticks() - start) >= timeout_ms))
            return N2D_TIMEOUT;
    }

    if (!s->manual_reset)
        s->signaled = false;

    return N2D_SUCCESS;
}

n2d_error_t n2d_hal_signal_set(n2d_pointer signal, n2d_bool_t state)
{
    if (!signal) return N2D_INVALID_ARGUMENT;

    ((n2d_hal_signal_t *)signal)->signaled = state;

    return N2D_SUCCESS;
}
