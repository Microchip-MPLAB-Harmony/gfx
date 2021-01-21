// <editor-fold defaultstate="collapsed" desc="DRV_GFX2D Initialization Data">


/* GFX2D Lib Interface Initialization */
const DRV_GFX2D_LIB_INTERFACE drvGFX2DLibAPI = {

    /* GFX2D Lib Line function */
    .line = DRV_GFX2D_Line,

    /* GFX2D Lib Fill function */
    .fill = DRV_GFX2D_Fill,

    /* GFX2D Lib Blit function */
    .blit = DRV_GFX2D_Blit,

    /* GFX2D Lib Status function */
    .statusGet = DRV_GFX2D_StatusGet,

    /* GFX2D Lib Callback Register */
    .callbackRegister = DRV_GFX2D_CallbackRegister,
};

/* GFX2D Driver Initialization Data */
const DRV_GFX2D_INIT drvGFX2DInitData =
{
    /* GFX2D Lib API */
    .lib = &drvGFX2DLibAPI,

    /* GFX2D IRQ */
//    .interruptGFX2D = DRV_GFX2D_INT_SRC
};

// </editor-fold>
