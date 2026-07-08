// AbortStreaming
//**********************************************************************************************
unsigned int PCIe_AbortStreaming(void* ctx)
{
	unsigned int reg = 0;
    int retry = 500;
	// Stop transfer by aborting DMA
    reg = (1 << TRANSPORT_CTRL_WR_OFFSET_ABORT_DMA0) + (1 << TRANSPORT_CTRL_WR_OFFSET_ABORT_DMA1);
	if(PCIe_SetReg(ctx, FPGA_PCIE_COMM_REG_TRANSPORT_CTRL, reg)) return 0;

    // Disable interrupts
    reg = 	(1 << INT_CFG_WR_OFFSET_EVENT_RST) + (0 << INT_CFG_WR_OFFSET_INT_GLOBAL_ENABLE);
	if(PCIe_SetReg(ctx, FPGA_PCIE_COMM_REG_INT_CFG, reg)) return 0;

    // Disable acquisition
    if(PCIe_GetReg(ctx, FPGA_PCIE_COMM_REG_ACQ_CTRL, &reg)) return 0;
    reg &= ~(1 << ACQ_CTRL_WR_OFFSET_EN);
    if(PCIe_SetReg (ctx, FPGA_PCIE_COMM_REG_ACQ_CTRL, reg)) return 0;

    // Reset stream
	reg = (1 << STREAM_IMAGE_CFG_WR_OFFSET_ABORT);
    if(PCIe_SetReg(ctx, FPGA_PCIE_COMM_REG_STREAM_IMAGE_CFG, reg)) return 0;

	while (--retry)
	{
		if(PCIe_GetReg(ctx, FPGA_PCIE_COMM_REG_TRANSPORT_CTRL, &reg)) return 0;
		if (1 == ((reg >> TRANSPORT_CTRL_RD_OFFSET_STATUS_IDLE)&0x1))
			break;
		iSleep(1);
	};
	if(0 == retry) return 0;

	// Flush buffer queue
	retry = 500;
	reg = 1 << DMA1_BUF_POOL_CTRL_WR_OFFSET_FLUSH;
	if(PCIe_SetReg(ctx, FPGA_PCIE_COMM_REG_DMA1_BUF_POOL_CTRL, reg)) return 0;
	while (--retry)
	{
		if(PCIe_GetReg(ctx, FPGA_PCIE_COMM_REG_DMA1_BUF_POOL_CTRL, &reg)) return 0;
		if (0 == ((reg >> DMA1_BUF_POOL_CTRL_RD_OFFSET_FLUSH_BUSY)&0x1))
			break;
		iSleep(1);
	};
    return retry;
}
