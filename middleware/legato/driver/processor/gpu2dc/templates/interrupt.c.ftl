/* ${IP} Interrupt Handler */
void __attribute__((weak)) ${IP}_InterruptHandler (void)
{
    DRV_GPU2DC_Handle_Interrupt();
}