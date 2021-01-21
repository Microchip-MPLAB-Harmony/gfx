// <editor-fold defaultstate="collapsed" desc="DRV_2DGPU Initialization Data">


/* 2DGPU Lib Interface Initialization */
const DRV_2DGPU_LIB_INTERFACE drv2DGPULibAPI = {

    /* 2DGPU Lib Line function */
    .line = DRV_2DGPU_Line,

    /* 2DGPU Lib Fill function */
    .fill = DRV_2DGPU_Fill,

    /* 2DGPU Lib Blit function */
    .blit = DRV_2DGPU_Blit,

    /* 2DGPU Lib Status function */
    .statusGet = DRV_2DGPU_StatusGet,

    /* 2DGPU Lib Callback Register */
    .callbackRegister = DRV_2DGPU_CallbackRegister,
};

/* 2DGPU Driver Initialization Data */
const DRV_2DGPU_INIT drv2DGPUInitData =
{
    /* 2DGPU Lib API */
    .lib = &drv2DGPULibAPI,

    /* 2DGPU IRQ */
//    .interrupt2DGPU = DRV_2DGPU_INT_SRC
};

// </editor-fold>
