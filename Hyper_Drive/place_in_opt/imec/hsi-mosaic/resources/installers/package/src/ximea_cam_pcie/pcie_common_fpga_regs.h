#pragma once

//-------------------------------------------------------------------------------------------------------------------
// FPGA Register Address Space, common to all PCIe family devices
//-------------------------------------------------------------------------------------------------------------------
// The whole register map is visualized in the document in FPGA Git: FPGA\DOC\API_Register_Map.xlsx

// PCIe block dedicated address 0-7 sub-section 
#define FPGA_PCIE_COMM_REG_TRANSPORT_CTRL           0
#define FPGA_PCIE_COMM_REG_INTERRUPT             	1
#define FPGA_PCIE_COMM_REG_PCIE_STATUS              2
#define FPGA_PCIE_COMM_REG_STREAM_BUFF_DELIVERED    3

// Device registers 8-63 
#define FPGA_PCIE_COMM_REG_TEMPERATURE              8
#define FPGA_PCIE_COMM_REG_USER_ID                  9
#define FPGA_PCIE_COMM_REG_SERIAL_NUMBER            10
#define FPGA_PCIE_COMM_REG_FW_REV                   11
#define FPGA_PCIE_COMM_REG_UNIQUE_ID_REG1           12
#define FPGA_PCIE_COMM_REG_UNIQUE_ID_REG2           13
#define FPGA_PCIE_COMM_REG_TIMESTAMP_REG1           14
#define FPGA_PCIE_COMM_REG_TIMESTAMP_REG2           15
#define FPGA_PCIE_COMM_REG_CLK_DAT                  16
#define FPGA_PCIE_COMM_REG_CLK_CFG                  17
#define FPGA_PCIE_COMM_REG_ACQ_FRAME_COUNT          18
#define FPGA_PCIE_COMM_REG_ACQ_FRAME_FIRED_REG1     19
#define FPGA_PCIE_COMM_REG_ACQ_FRAME_FIRED_REG2     20
#define FPGA_PCIE_COMM_REG_ACQ_FRAME_LOST           21
#define FPGA_PCIE_COMM_REG_ACQ_FRAME_UNDERRUN       22
#define FPGA_PCIE_COMM_REG_ACQ_CTRL                 23
#define FPGA_PCIE_COMM_REG_ACQ_NUM_FRAMES           24
#define FPGA_PCIE_COMM_REG_ACQ_NUM_BURST_FRAMES     25
#define FPGA_PCIE_COMM_REG_IO_SELECT                26
#define FPGA_PCIE_COMM_REG_IO_CONFIG                27
#define FPGA_PCIE_COMM_REG_IO_UART                  28
#define FPGA_PCIE_COMM_REG_USER_OUTPUT              29
#define FPGA_PCIE_COMM_REG_TRIG_CTRL                30
#define FPGA_PCIE_COMM_REG_FLASH_SPI_CMD            31
#define FPGA_PCIE_COMM_REG_FLASH_SPI_DAT            32
#define FPGA_PCIE_COMM_REG_FLASH_ADDR_EXT           95
#define FPGA_PCIE_COMM_REG_INT_DAT                  33
#define FPGA_PCIE_COMM_REG_INT_CFG                  34
#define FPGA_PCIE_COMM_REG_HEADER_CFG               35
#define FPGA_PCIE_COMM_REG_USER_METADATA_ADDR       36
#define FPGA_PCIE_COMM_REG_USER_METADATA_DAT        37
#define FPGA_PCIE_COMM_REG_DMA1_BUF_POOL_ADDRL      38
#define FPGA_PCIE_COMM_REG_DMA1_BUF_POOL_ADDRH      39
#define FPGA_PCIE_COMM_REG_DMA1_BUF_POOL_SIZE       40
#define FPGA_PCIE_COMM_REG_DMA1_BUF_POOL_CTRL       41
#define FPGA_PCIE_COMM_REG_IMAGE_CROP_SIZE_REG1     42
#define FPGA_PCIE_COMM_REG_IMAGE_CROP_SIZE_REG2     43
#define FPGA_PCIE_COMM_REG_PACKER_CFG               44
#define FPGA_PCIE_COMM_REG_STREAM_IMAGE_SIZE        45
#define FPGA_PCIE_COMM_REG_STREAM_IMAGE_CFG         46
#define FPGA_PCIE_COMM_REG_LUT_UPDATE               47
#define FPGA_PCIE_COMM_REG_LUT_CFG                  48
#define FPGA_PCIE_COMM_REG_COLCOR_BRAM_ADDR         49
#define FPGA_PCIE_COMM_REG_COLCOR_BRAM_DAT          50
#define FPGA_PCIE_COMM_REG_COLCOR_CFG               51
#define FPGA_PCIE_COMM_REG_SENSOR_SIM_CFG           52
#define FPGA_PCIE_COMM_REG_SENSOR_SIM_FRAME_SIZE    53
#define FPGA_PCIE_COMM_REG_SENSOR_SIM_EXP_TIME      54
#define FPGA_PCIE_COMM_REG_SENSOR_SIM_FOT_TIME      55
#define FPGA_PCIE_COMM_REG_DIG_GAIN_OFFSET_MULT     56
#define FPGA_PCIE_COMM_REG_DIG_GAIN_OFFSET_ADD      57
#define FPGA_PCIE_COMM_REG_BAD_PIXEL_DAT            58
#define FPGA_PCIE_COMM_REG_BAD_PIXEL_COUNTER        59
#define FPGA_PCIE_COMM_REG_BAD_PIXEL_ADDR           60
#define FPGA_PCIE_COMM_REG_ACQ_FRAME_RATE           61
#define FPGA_REG_TRIG_DELAY                         62
#define FPGA_REG_ACQ_COUNTERS                       63

// FrameBuffer config register
#define FPGA_REG_FRAMEBUF_CFG                       74

// Power sync register
#define FPGA_POWER_STAGE_SYNC						77

// eMMC Flash
#define FPGA_REG_MMC_DAT                             92
#define FPGA_REG_MMC_CMD                             93

// SNE
#define FPGA_REG_SNE_REG_1                          96
#define FPGA_REG_SNE_REG_2                          97
// Row Correction
#define FPGA_REG_ROWCOR_BRAM_ADDR                   98
#define FPGA_REG_ROWCOR_BRAM_DAT                    99
#define FPGA_REG_ROWCOR_CFG                         100
#define FPGA_REG_EXP_PULSE_OUT_CFG                  102
#define FPGA_REG_IO_DEBOUNCE                        103
#define FPGA_REG_DEPADDER_CFG                       104
#define FPGA_REG_IO_INV_OVERRIDE					105
#define FPGA_REG_BINNING                            106

// Misc
#define FPGA_REG_PLTR_CTRL                          70

// Power Stage Configuration
#define FPGA_RD_POWER_STAGE_STAT                    76
#define FPGA_WR_POWER_STAGE_CFG                     76

#define FPGA_RD_POWER_STAGE_STAT_OFFSET_BUSY        0
#define FPGA_RD_POWER_STAGE_STAT_OFFSET_PWR_EN      1

#define FPGA_WR_POWER_STAGE_CFG_OFFSET_VAL          0
#define FPGA_WR_POWER_STAGE_CFG_OFFSET_LDAC         16
#define FPGA_WR_POWER_STAGE_CFG_OFFSET_CHAN         17
#define FPGA_WR_POWER_STAGE_CFG_OFFSET_ADDR         21
#define FPGA_WR_POWER_STAGE_CFG_OFFSET_TYPE         24
#define FPGA_WR_POWER_STAGE_CFG_OFFSET_PWR_EN       27

// Fan Control
#define FPGA_WR_FAN_CONTROL                         78

#define FPGA_WR_FAN_CONTROL_OFFSET_EN               0
#define FPGA_WR_FAN_CONTROL_OFFSET_PWM              1

//-------------------------------------------------------------------------------------------------------------------
// FPGA Register Read/Write Bit Offsets of individual variables
// Note: Registers that are not used for multiple variables - where all 32 bits belong to one variable (e.g. SERIAL_NUMBER == {serial_number[31:0]}) - are not specified
// Note: Register Address Map overview for Read/Write is available in Excel document
//-------------------------------------------------------------------------------------------------------------------

// FPGA_REG_TRANSPORT_CTRL_REG - WRITE Offsets
#define TRANSPORT_CTRL_WR_OFFSET_ABORT_DMA0     0
#define TRANSPORT_CTRL_WR_OFFSET_ABORT_DMA1     1

// FPGA_REG_DMA_CTRL_REG - READ Offsets
#define TRANSPORT_CTRL_RD_OFFSET_ABORT_DMA0     0
#define TRANSPORT_CTRL_RD_OFFSET_STATUS_DMA0    1
#define TRANSPORT_CTRL_RD_OFFSET_ABORT_DMA1     5
#define TRANSPORT_CTRL_RD_OFFSET_STATUS_DMA1    6
#define TRANSPORT_CTRL_RD_OFFSET_STATUS_IDLE    10  // Indicates idle state of the transport module

// FPGA_PCIE_COMM_REG_PCIE_STATUS - READ Offsets
#define PCIE_STATUS_RD_OFFSET_LINK_UP           0   // Indicates the physical layer link up status
#define PCIE_STATUS_RD_OFFSET_LINK_RATE         1   // 0: 2.5Gbps, 1: 5.0Gbps
#define PCIE_STATUS_RD_OFFSET_LINK_WIDTH        2   // 00: 1 lane, 01: 2 lane, 10: 4 lane, 11: 8 lane

// FPGA_REG_UNIQUE_ID_REG1 - WRITE Offsets
#define UNIQUE_ID_REG1_WR_OFFSET_RELOAD         0   // Forces sequence of re-loading Unique ID from DS2401/DS2411 IC through One-Wire communication

// FPGA_PCIE_COMM_REG_TIMESTAMP_REG1 - WRITE Offsets
#define TIMESTAMP_REG1_WR_STORE                 0   //Stores timestamp value in register selected if SELECTOR is set to 1
#define TIMESTAMP_REG1_WR_SELECTOR              1   //Timestamp readout selector 0- standard timestamp runnig (could be feezed for proper readout) 1- stored time stamp (for U3V)
#define TIMESTAMP_REG1_WR_FREEZE                2   //If set to 1 the value in TIMESTAMP_REGx is frozen 

// FPGA_REG_CLK_DAT - WRITE Offsets
#define CLK_DAT_WR_OFFSET_DATA                  0
#define CLK_DAT_WR_OFFSET_DATA_MASK             16

// FPGA_REG_CLK_CFG - WRITE Offsets
#define CLK_CFG_WR_OFFSET_MMCM_RESET            0
#define CLK_CFG_WR_OFFSET_DRP_SET               1
#define CLK_CFG_WR_OFFSET_ADDRESS               2   // 7 bits
#define CLK_CFG_WR_OFFSET_EXT_CLK               31  // Use external clock (not 250MHz)

#define CLK_CFG_WR_MASK                         0x1ff   // clear MMCM_RESET, DRP_SET and ADDRESS

// FPGA_REG_CLK_CFG - READ Offsets
#define CLK_CFG_RD_OFFSET_LOCKED                0   // Bit indicating that the MMCM Clock Generator is Locked
#define CLK_CFG_RD_OFFSET_BUSY                  1   // Bit indicating that the MMCM Clock Generator is Busy
#define CLK_CFG_RD_OFFSET_EXT_CLK_CAP           2   // 4 bits - 0 means internal 250MHz clk; 1 means external 60MHz clk; ...
#define CLK_CFG_RD_OFFSET_EXT_CLK               31  // External clock is used

#define EXT_CLK_CAP_250MHz                      0
#define EXT_CLK_CAP_60MHz_OLD                   1   // Used in old CB042
#define EXT_CLK_CAP_60MHz                       2
#define EXT_CLK_CAP_100MHz                      3


// FPGA_REG_ACQ_CTRL - WRITE Offsets
#define	ACQ_CTRL_WR_OFFSET_EN                   0
#define	ACQ_CTRL_WR_OFFSET_EXP_MODE             1
#define ACQ_CTRL_WR_OFFSET_FORCE_FRAME_CNT_RST  4   //force reset of acq_frame_count
#define ACQ_CTRL_WR_OFFSET_EN_ACQ_FRAME_CNT_RST 5   //enable automatic reset of acq_frame_count by acqusitions start (needed for U3V)
#define ACQ_CTRL_WR_OFFSET_DUAL_EXP_HDR_EN      6   // Enable interline dual exposure HDR mode


// FPGA_REG_ACQ_CTRL - READ Offsets
#define	ACQ_CTRL_RD_OFFSET_EN                   0
#define	ACQ_CTRL_RD_OFFSET_EXP_MODE             1
#define ACQ_CTRL_RD_OFFSET_FORCE_FRAME_CNT_RST  4   //force reset of acq_frame_count
#define ACQ_CTRL_RD_OFFSET_EN_ACQ_FRAME_CNT_RST 5   //disable automatic reset of acq_frame_count by acqusitions start (needed for U3V)
#define ACQ_CTRL_RD_OFFSET_DUAL_EXP_HDR_EN      6   // Enable interline dual exposure HDR mode

// FPGA_REG_ACQ_CTRL - MASK
#define	ACQ_CTRL_RD_MASK_EN                     0x0001
#define	ACQ_CTRL_RD_MASK_EXP_MODE               0x000E
#define ACQ_CTRL_RD_MASK_FORCE_FRAME_CNT_RST    0x0010   //force reset of acq_frame_count
#define ACQ_CTRL_RD_MASK_EN_ACQ_FRAME_CNT_RST   0x0020   //disable automatic reset of acq_frame_count by acqusitions start (needed for U3V)
#define ACQ_CTRL_RD_MASK_DUAL_EXP_HDR_EN        0x0040   // Enable interline dual exposure HDR mode

// FPGA_REG_IO_SELECT - WRITE Offsets
#define	IO_SELECT_WR_OFFSET_LINE_SEL            0       // 4 lower bits of line_selector[3:0]
#define	IO_SELECT_WR_OFFSET_TRIG_SEL            4
#define	IO_SELECT_WR_OFFSET_RXD_SEL             8       // 4 lower bits of uart_rxd_selector[3:0]
#define	IO_SELECT_WR_OFFSET_USER_OUT_SEL        12
#define	IO_SELECT_WR_OFFSET_LINE_SEL_1          16      // 2 higher bits of line_selector[5:4]
#define	IO_SELECT_WR_OFFSET_RXD_SEL_1           18      // 2 higher bits of uart_rxd_selector[5:4]

// FPGA_REG_IO_SELECT - READ Offsets
#define	IO_SELECT_RD_OFFSET_LINE_SEL            0
#define	IO_SELECT_RD_OFFSET_TRIG_SEL            4
#define	IO_SELECT_RD_OFFSET_RXD_SEL             8
#define	IO_SELECT_RD_OFFSET_USER_OUT_SEL        12
#define	IO_SELECT_RD_OFFSET_LINE_SEL_1          16
#define	IO_SELECT_RD_OFFSET_RXD_SEL_1           18

// FPGA_REG_IO_CONFIG - WRITE Offsets
// Sets Invert, Mode and Source parameters of IO Line selected by IO_SELECT
#define	IO_CONFIG_WR_OFFSET_INVERTER            0
#define	IO_CONFIG_WR_OFFSET_MODE		        1
#define	IO_CONFIG_WR_OFFSET_SOURCE              2       // 5 lower bits of io_source_mux[4:0]
//24 bits STATUS RD
#define	IO_CONFIG_WR_OFFSET_SOURCE_1            31      // 1 higher bit of io_source_mux[5]

// FPGA_REG_IO_CONFIG - READ Offsets: 32bit register [sttttttt tttttttt tttttttt tsssssmi]
// i = LineInverter IO_CONFIG_RD_OFFSET_INVERTER    (1 bit   = 0 invertor is off, =1 invertor is active)
// m = LineMode     IO_CONFIG_RD_OFFSET_MODE        (1 bit   = 1 line is output, =0 line is input)
// s = LineSource   IO_CONFIG_RD_OFFSET_SOURCE      (6 bits  = values see PcieFamLineSource)
// t = LineStatus   IO_CONFIG_RD_OFFSET_STATUS      (24 bits = digital status of 16 lines (not all may be physically present on device), bit index = PcieFamLineSelector)
#define	IO_CONFIG_RD_OFFSET_INVERTER            0
#define	IO_CONFIG_RD_OFFSET_MODE		        1
#define	IO_CONFIG_RD_OFFSET_SOURCE              2   // 5 lower bits of io_source_mux[4:0]
#define	IO_CONFIG_RD_OFFSET_STATUS              7   // 24 bits
#define	IO_CONFIG_RD_OFFSET_SOURCE_1            31  // 1 higher bit of io_source_mux[5]

// FPGA_REG_IO_UART - WRITE Offsets
#define	IO_UART_WR_OFFSET_FEAT_DAT			    0   // 23 bits - data
#define	IO_UART_WR_OFFSET_FEAT_DAT_WR			23  // 1 bit - wr/rd
#define	IO_UART_WR_OFFSET_FEAT_SEL			    24  // 8 bit - feature selector

// FPGA_REG_IO_UART - READ Offsets
#define	IO_UART_RD_OFFSET_FEAT_DAT			    0   // 23 bits - data
#define	IO_UART_RD_OFFSET_FEAT_SEL			    24  // 8 bit - feature selector

// UART register feature selector
#define UART_FEAT_SEL_MODE                      0
#define UART_FEAT_SEL_BAUDRATE                  1
#define UART_FEAT_SEL_MSB_FIRST                 2
#define UART_FEAT_SEL_ADD_BITS                  3   // additional bits (start/stop(s)), see additional_control_bits
// ... // reserved
#define UART_FEAT_SEL_COMPOSER_EVENT_NOW        64
#define UART_FEAT_SEL_COMPOSER_PACKET_CRC16     65
#define UART_FEAT_SEL_COMPOSER_USER_DATA        66
#define UART_FEAT_SEL_COMPOSER_USER_ID          67
#define UART_FEAT_SEL_COMPOSER_FRAME_NUM        68
#define UART_FEAT_SEL_COMPOSER_TS_EXP_R         69  // timestamp of exp rising edge
#define UART_FEAT_SEL_COMPOSER_TS_EXP_F         70  // timestamp of exp falling edge
#define UART_FEAT_SEL_COMPOSER_TS_READ_R        71
#define UART_FEAT_SEL_COMPOSER_TS_READ_F        72
#define UART_FEAT_SEL_COMPOSER_TS_TRANSFER_R    73
#define UART_FEAT_SEL_COMPOSER_TS_TRANSFER_F    74
#define UART_FEAT_SEL_COMPOSER_ERROR            75
#define UART_FEAT_SEL_COMPOSER_IMG_CRC          76
// ... // reserved

#define UART_MODE_NORMAL        0
#define UART_MODE_BIPHASE_ENC   1

// DATA offsets when selector is COMPOSER
#define UART_FEAT_SEL_COMPOSER_DATA_OFFSET_EVENT    0 // 8 bits
#define UART_FEAT_SEL_COMPOSER_DATA_OFFSET_EN       8 // 1 bit
#define UART_FEAT_SEL_COMPOSER_DATA_OFFSET_DATA     9 // 8 bits - only if required (e.g. for UART_FEAT_SEL_COMPOSER_USER_DATA)

// enum EVENTS for UART_FEAT_SEL_COMPOSER_DATA_OFFSET_EVENT offset
#define UART_PACKET_COMPOSER_EVENT_NOW              0
#define UART_PACKET_COMPOSER_EVENT_EXP_START        1
#define UART_PACKET_COMPOSER_EVENT_EXP_END          2
#define UART_PACKET_COMPOSER_EVENT_READ_START       3
#define UART_PACKET_COMPOSER_EVENT_READ_END         4
#define UART_PACKET_COMPOSER_EVENT_TRANSFER_START   5
#define UART_PACKET_COMPOSER_EVENT_TRANSFER_END     6

// FPGA_REG_USER_OUTPUT - WRITE Offsets
#define USER_OUTPUT_WR_OFFSET_VALUE             0
#define USER_OUTPUT_WR_OFFSET_VALUE_MASK        16

// FPGA_REG_USER_OUTPUT - READ Offsets
#define USER_OUTPUT_RD_OFFSET_VALUE             0
#define USER_OUTPUT_RD_OFFSET_VALUE_MASK        16

// FPGA_REG_TRIG_CTRL - WRITE Offsets
#define	TRIG_CTRL_WR_OFFSET_SOURCE              0   // 4 lower bits of trig_source[3:0]
#define	TRIG_CTRL_WR_OFFSET_ACTIV   	        4
#define	TRIG_CTRL_WR_OFFSET_MODE   	            7
#define	TRIG_CTRL_WR_OFFSET_OVERLAP  	        8
#define	TRIG_CTRL_WR_OFFSET_SOFT   	            10
#define	TRIG_CTRL_WR_OFFSET_SOURCE_1            11  // 2 higer bits of trig_source[5:4]

// FPGA_REG_TRIG_CTRL - READ Offsets
#define	TRIG_CTRL_RD_OFFSET_SOURCE              0
#define	TRIG_CTRL_RD_OFFSET_ACTIV   	        4
#define	TRIG_CTRL_RD_OFFSET_MODE   	            7
#define	TRIG_CTRL_RD_OFFSET_OVERLAP  	        8
//SW_TRIG - write only
#define	TRIG_CTRL_RD_OFFSET_SOURCE_1            11  // 2 higer bits of trig_source[5:4]

// FPGA_REG_TRIG_DELAY - WRITE Offsets
#define	TRIG_DELAY_WR_OFFSET_VALUE              0   // Delay value 4ns increment - 29 bits
#define	TRIG_DELAY_WR_OFFSET_ENABLE             31  // Enable delay

// FPGA_REG_TRIG_DELAY - READ Offsets
#define	TRIG_DELAY_RD_OFFSET_VALUE              0
#define	TRIG_DELAY_RD_OFFSET_ENABLE             31

// FPGA_REG_ACQ_COUNTERS - WRITE Offsets
#define	ACQ_COUNTERS_WR_OFFSET_SELECTOR         0   // 4 bits
#define	ACQ_COUNTERS_WR_OFFSET_RST              31  // Reset counter cmd - 1 bit

// FPGA_REG_ACQ_COUNTERS - READ Offsets
#define	ACQ_COUNTERS_RD_OFFSET_VALUE            0   // Counter value - 32 bits
// Selectors
#define	ACQ_COUNTER_SEL_LOST_TRIG_BUSY          0   // Lost triggers counter due to busy
#define	ACQ_COUNTER_SEL_LOST_TRIG_FRAME_BUF     1   // Lost triggers counter due to full frame buffer
#define	ACQ_COUNTER_SEL_FRAME_BUF_FULL          2   // Full frame buffer counter

// FPGA_REG_FLASH_SPI_CMD - WRITE Offsets
#define FLASH_SPI_CMD_WR_OFFSET_ADDR	        0
#define FLASH_SPI_CMD_WR_OFFSET_CMD	            24

// FPGA_REG_FLASH_SPI_CMD - READ Offsets
#define FLASH_SPI_CMD_RD_OFFSET_BUSY	        0

// FPGA_PCIE_COMM_REG_FLASH_ADDR_EXT - WRITE Offsets
#define FLASH_SPI_EXT_WR_OFFSET_ADDR            0
#define FLASH_SPI_EXT_WR_OFFSET_BULK            8
#define FLASH_SPI_EXT_WR_OFFSET_RESET           28

// FPGA_PCIE_COMM_REG_FLASH_ADDR_EXT - READ Offsets
#define FLASH_SPI_EXT_RD_OFFSET_COUNT           0
#define FLASH_SPI_EXT_RD_OFFSET_VERSION         30


// FPGA_REG_INT_DAT - WRITE Offsets
#define INT_DAT_WR_OFFSET_EVENT_INT_ENABLED     0

// FPGA_REG_INT_DAT - READ Offsets
#define INT_DAT_RD_OFFSET_EVENT_INFO_BUFFER_ID          0
#define INT_DAT_RD_OFFSET_EVENT_INFO_BUFFER_ID_VALID    12
#define INT_DAT_RD_OFFSET_EVENT_SOURCE          		26

// FPGA_REG_INT_CFG - WRITE Offsets
#define INT_CFG_WR_OFFSET_EVENT_SELECTOR        0
#define INT_CFG_WR_OFFSET_EVENT_RST             6
#define INT_CFG_WR_OFFSET_INT_GLOBAL_ENABLE     7

// FPGA_REG_INT_CFG - READ Offsets
#define INT_CFG_RD_OFFSET_EVENT_INT_ENABLED     0
#define INT_CFG_RD_OFFSET_EVENT_SELECTOR        1
#define INT_CFG_RD_OFFSET_EVENT_COUNT           7
#define INT_CFG_RD_OFFSET_EVENT_OVERFLOW        17
#define INT_CFG_RD_OFFSET_INT_GLOBAL_ENABLE     18

// FPGA_REG_HEADER_CFG - WRITE Offsets
#define HEADER_CFG_OFFSET_SIZE                  0

// FPGA_REG_DMA1_BUF_POOL_CTRL - WRITE Offsets
#define DMA1_BUF_POOL_CTRL_WR_OFFSET_FRAME_NUM  0   // 14 bits
#define DMA1_BUF_POOL_CTRL_WR_OFFSET_BUFFER_ID  14  // 12 bits
#define DMA1_BUF_POOL_CTRL_WR_OFFSET_INT        26  // 1 bit
#define DMA1_BUF_POOL_CTRL_WR_OFFSET_REQUEUE    27  // 1 bit
#define DMA1_BUF_POOL_CTRL_WR_OFFSET_FLUSH      28  // 1 bit
#define DMA1_BUF_POOL_CTRL_WR_OFFSET_ADD        29  // 1 bit

// FPGA_REG_DMA1_BUF_POOL_CTRL - READ Offsets
#define DMA1_BUF_POOL_CTRL_RD_OFFSET_FULL       0
#define DMA1_BUF_POOL_CTRL_RD_OFFSET_FLUSH_BUSY 1
#define DMA1_BUF_POOL_CTRL_RD_OFFSET_ADD_BUSY   2

// FPGA_REG_IMAGE_CROP_SIZE_REG1 - WRITE Offset
#define IMAGE_CROP_SIZE_REG1_WR_OFFSET_START    0
#define IMAGE_CROP_SIZE_REG1_WR_OFFSET_STOP     16

// FPGA_REG_IMAGE_CROP_SIZE_REG2 - WRITE Offset
#define IMAGE_CROP_SIZE_REG1_WR_OFFSET_RECYCLE  0

// FPGA_REG_STREAM_IMAGE_CFG - WRITE Offset
#define STREAM_IMAGE_CFG_WR_OFFSET_ABORT        0
#define STREAM_IMAGE_CFG_WR_OFFSET_STOP         1

// FPGA_REG_STREAM_IMAGE_CFG - READ Offset
#define STREAM_IMAGE_CFG_RD_OFFSET_STOPPED      0

// FPGA_REG_LUT_UPDATE - WRITE Offset
#define LUT_UPDATE_WR_OFFSET_ADDR               0       // lower 12 bits of address
#define LUT_UPDATE_WR_OFFSET_DAT                12      // lower 12 bits of data
#define LUT_UPDATE_WR_OFFSET_ADDR_1             24      // upper 4 bits of address
#define LUT_UPDATE_WR_OFFSET_DAT_1              28      // upper 4 bits of data

// FPGA_REG_LUT_CFG - WRITE Offset
#define LUT_CFG_WR_OFFSET_EN                    0

// FPGA_REG_COLCOR_CFG - WRITE Offset
#define COLCOR_CFG_WR_OFFSET_MAX_COUNT          0   // 13 bits

// FPGA_REG_COLCOR_BRAM_ADDR - WRITE Offset
#define COLCOR_BRAM_ADDR_WR_OFFSET_ADDRA        0   // 11 bits
#define COLCOR_BRAM_ADDR_WR_OFFSET_SEL          11  // 7 bits
#define COLCOR_BRAM_ADDR_WR_OFFSET_ADDRA_2      19  // 2 bits - bits [12:11] of ADDRA

// FPGA_REG_COLCOR_BRAM_DAT - WRITE Offset
#define COLCOR_BRAM_DAT_WR_OFFSET_ADD           0
#define COLCOR_BRAM_DAT_WR_OFFSET_SEL           11
#define COLCOR_BRAM_DAT_WR_OFFSET_MULT          12

// FPGA_REG_ROWCOR_BRAM_ADDR - WRITE Offsets
#define FPGA_WR_ROWCOR_BRAM_ADDR_OFFSET_ADDR    0       // 15 bits
#define FPGA_WR_ROWCOR_BRAM_ADDR_OFFSET_WR      31      // 1 bit
// FPGA_REG_ROWCOR_BRAM_ADDR - READ Offsets
#define FPGA_RD_ROWCOR_BRAM_ADDR_OFFSET_ADDR    0
#define FPGA_RD_ROWCOR_BRAM_ADDR_OFFSET_WR      31

// FPGA_REG_ROWCOR_BRAM_DAT - WRITE Offsets
#define ROWCOR_BRAM_DAT_WR_OFFSET_ADD           0       // 11 bits
#define ROWCOR_BRAM_DAT_WR_OFFSET_SEL           11      // 1 bit
#define ROWCOR_BRAM_DAT_WR_OFFSET_MULT          12      // 20 bits
// FPGA_REG_ROWCOR_BRAM_DAT - READ Offsets
#define ROWCOR_BRAM_DAT_RD_OFFSET_ADD           0
#define ROWCOR_BRAM_DAT_RD_OFFSET_SEL           11
#define ROWCOR_BRAM_DAT_RD_OFFSET_MULT          12

// FPGA_REG_ROWCOR_CFG - WRITE Offsets
#define FPGA_WR_ROWCOR_CFG_OFFSET_LINE_WIDTH    0       // 11 bits
#define FPGA_WR_ROWCOR_CFG_OFFSET_MAX_COUNT     11      // 15 bits
#define FPGA_WR_ROWCOR_CFG_OFFSET_ENABLE        30      // 1 bit
#define FPGA_WR_ROWCOR_CFG_OFFSET_SINGLE_MODE   31      // 1 bit
// FPGA_REG_ROWCOR_CFG - READ Offsets
#define FPGA_RD_ROWCOR_CFG_OFFSET_LINE_WIDTH    0
#define FPGA_RD_ROWCOR_CFG_OFFSET_MAX_COUNT     11
#define FPGA_RD_ROWCOR_CFG_OFFSET_ENABLE        30
#define FPGA_RD_ROWCOR_CFG_OFFSET_SINGLE_MODE   31

// BAD Pixel List Offsets and Definitions
#define BAD_PIXEL_LIST_ADDR_MAX         2048

#define BAD_PIXEL_ADDR_OFFSET_ADDR      0
#define BAD_PIXEL_ADDR_OFFSET_COUNT     30
#define BAD_PIXEL_ADDR_OFFSET_WEA       31

#define BAD_PIXEL_COUNTER_OFFSET_COUNT  0
#define BAD_PIXEL_COUNTER_OFFSET_VALID  31

#define BAD_PIXEL_DAT_OFFSET_PIX0       0
#define BAD_PIXEL_DAT_OFFSET_PIX1       4
#define BAD_PIXEL_DAT_OFFSET_PIX2       8
#define BAD_PIXEL_DAT_OFFSET_PIX3       12
#define BAD_PIXEL_DAT_OFFSET_PIX4       16
#define BAD_PIXEL_DAT_OFFSET_PIX5       20
#define BAD_PIXEL_DAT_OFFSET_PIX6       24
#define BAD_PIXEL_DAT_OFFSET_PIX7       28

#define BAD_PIXEL_CORRECTION_NONE       8
#define BAD_PIXEL_CORRECTION_PIX0       0
#define BAD_PIXEL_CORRECTION_PIX1       1
#define BAD_PIXEL_CORRECTION_PIX2       2
#define BAD_PIXEL_CORRECTION_PIX3       3
#define BAD_PIXEL_CORRECTION_PIX4       4
#define BAD_PIXEL_CORRECTION_PIX5       5
#define BAD_PIXEL_CORRECTION_PIX6       6
#define BAD_PIXEL_CORRECTION_PIX7       7


// FPGA_REG_PLTR_CTRL - WRITE Offset
#define PLTR_CTRL_WR_OFFSET_VOLTAGE_VAL 0   // 16 bits
#define PLTR_CTRL_WR_OFFSET_VOLTAGE_EN  16
#define PLTR_CTRL_WR_OFFSET_LOAD_CMD    17
#define PLTR_CTRL_WR_OFFSET_WD_BARK_CLEAR   25
#define PLTR_CTRL_WR_OFFSET_WD_TOGGLE   26
#define PLTR_CTRL_WR_OFFSET_WD_BARKED   27
#define PLTR_CTRL_WR_OFFSET_RD_ADDR     28  // 3 bits
// FPGA_REG_PLTR_CTRL - READ Offsets
#define PLTR_CTRL_RD_OFFSET_VOLTAGE_VAL 0   // 16 bits
#define PLTR_CTRL_RD_OFFSET_VOLTAGE_EN  16
#define PLTR_CTRL_RD_OFFSET_WD_BARK_CLEAR   25
#define PLTR_CTRL_RD_OFFSET_WD_TOGGLE   26
#define PLTR_CTRL_RD_OFFSET_WD_BARKED   27
#define PLTR_CTRL_RD_OFFSET_RD_ADDR     28  // 3 bits
#define PLTR_CTRL_RD_OFFSET_DAC_BUSY    31  // 1 bit

#define PLTR_CTRL_RD_SET_VAL            0
#define PLTR_CTRL_RD_AIN0               1
#define PLTR_CTRL_RD_AIN1               2
#define PLTR_CTRL_RD_AIN2               3
#define PLTR_CTRL_RD_AIN3               4

#define PLTR_CTRL_VOLTAGE_MIN           0.0
#define PLTR_CTRL_VOLTAGE_MAX           2.5

// eMMC
#define FPGA_WR_MMC_CMD_OFFSET_ADDR     0
#define FPGA_WR_MMC_CMD_OFFSET_CMD      28

#define MMC_API_CMD_RESET               0
#define MMC_API_CMD_IDLE                1
#define MMC_API_CMD_WRITE_TRANS         2
#define MMC_API_CMD_READ_TRANS          3
#define MMC_API_CMD_READ_DATA           4
#define MMC_API_CMD_READ_COUNT          5
#define MMC_API_CMD_COMMAND				6

// FrameBuffer Status
//WR
#define FPGA_WR_FRAMEBUF_CFG_OFFSET_ENABLE          0   // 1 bit
#define FPGA_WR_FRAMEBUF_CFG_OFFSET_AFULL           1   // 26 bits
//RD
#define FPGA_RD_FRAMEBUF_CFG_OFFSET_ENABLE          0   // 1 bit
#define FPGA_RD_FRAMEBUF_CFG_OFFSET_INIT            1   // 1 bit
#define FPGA_RD_FRAMEBUF_CFG_OFFSET_VERSION         29  // 3 bits

// FPGA_REG_EXP_PULSE_OUT_CFG - WRITE Offset
#define FPGA_REG_EXP_PULSE_OUT_CFG_WR_OFFSET_WIDTH  0   // 8 bits
// FPGA_REG_EXP_PULSE_OUT_CFG - READ Offsets
#define FPGA_REG_EXP_PULSE_OUT_CFG_RD_OFFSET_WIDTH  0   // 8 bits

// FPGA_REG_IO_DEBOUNCE - WRITE Offset
#define FPGA_REG_IO_DEBOUNCE_WR_OFFSET_TIME         0   // 20 bits
#define FPGA_REG_IO_DEBOUNCE_WR_OFFSET_TIME_SEL     23  // 1 bit
#define FPGA_REG_IO_DEBOUNCE_WR_OFFSET_TIME_WR      24  // 1 bit
#define FPGA_REG_IO_DEBOUNCE_WR_OFFSET_TIME_ENABLE  25  // 1 bit
//#define FPGA_REG_IO_DEBOUNCE_RD_OFFSET_CAP          26  // 1 bit
// FPGA_REG_IO_DEBOUNCE - READ Offsets
#define FPGA_REG_IO_DEBOUNCE_RD_OFFSET_TIME         0   // 20 bits
#define FPGA_REG_IO_DEBOUNCE_RD_OFFSET_TIME_SEL     23  // 1 bit
#define FPGA_REG_IO_DEBOUNCE_RD_OFFSET_TIME_WR      24  // 1 bit
#define FPGA_REG_IO_DEBOUNCE_RD_OFFSET_TIME_ENABLE  25  // 1 bit
#define FPGA_REG_IO_DEBOUNCE_RD_OFFSET_CAP          26  // 1 bit

// FPGA_REG_DEPADDER_CFG - WRITE Offset
#define FPGA_REG_DEPADDER_CFG_WR_OFFSET_VALUE       0   // 16 bits
#define FPGA_REG_DEPADDER_CFG_WR_OFFSET_SELECTOR    16  // 8 bits
#define FPGA_REG_DEPADDER_CFG_WR_OFFSET_WR          24  // 1 bit
// FPGA_REG_DEPADDER_CFG - READ Offsets
#define FPGA_REG_DEPADDER_CFG_RD_OFFSET_VALUE       0   // 16 bits
#define FPGA_REG_DEPADDER_CFG_RD_OFFSET_SELECTOR    16  // 8 bits
// SELECTOR values
#define DEPADDER_CFG_SEL_ENABLE                     0
#define DEPADDER_CFG_SEL_PIX_SHIFT                  1
#define DEPADDER_CFG_SEL_LINE_DATA_MAX              2

//-------------------------------------------------------------------------------------------------------------------
// FPGA Register Variable Enumerators
//-------------------------------------------------------------------------------------------------------------------

// Column Correction Constants
// ColCor Reload Counter is specified in 8 pixel multiples
#define PCIE_COMMON_COLCOR_RELOAD_COUNT_MIN         1
#define PCIE_COMMON_COLCOR_RELOAD_COUNT_MAX         1536
#define PCIE_COMMON_COLCOR_32x2048_RELOAD_COUNT_MAX 2048
#define PCIE_COMMON_COLCOR_32x3072_RELOAD_COUNT_MAX 3072
// ColCor Column Selector is specified by individual pixel
#define PCIE_COMMON_COLCOR_COLUMN_MIN           0
#define PCIE_COMMON_COLCOR_COLUMN_MAX           (1536*8-1)
#define PCIE_COMMON_COLCOR_32x2048_COLUMN_MAX   (PCIE_COMMON_COLCOR_32x2048_RELOAD_COUNT_MAX*8-1)
#define PCIE_COMMON_COLCOR_32x3072_COLUMN_MAX   (PCIE_COMMON_COLCOR_32x3072_RELOAD_COUNT_MAX*8-1)

// RowCor ranges
#define ROWCOR_ROW_MIN              0
#define ROWCOR_ROW_MAX              6144
#define ROWCOR_ROW_MAX_BRAM_14336   7168

#define ROWCOR_LINE_WIDTH_MIN       0
#define ROWCOR_LINE_WIDTH_MAX       2047*8

#define ROWCOR_ROW_RELOAD_MIN       0
#define ROWCOR_ROW_RELOAD_MAX       12288
#define ROWCOR_ROW_RELOAD_MAX_BRAM_14336    14336

// TriggerDelay constants
#define TRIG_DELAY_FPGA_MIN                     0.080   // 0.080 us

// Timestamp reset 
#define TIMESTAMP_RESET_CTRL  101    
#define TIMESTAMP_RESET_CTRL_SOURCE_OFFSET	0
#define TIMESTAMP_RESET_CTRL_SOURCE_SIZE  	5
	#define TIMESTAMP_RESET_CTRL_SOURCE_OFF	0
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE1	1
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE2    2
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE3    3
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE4	4
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE5	5
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE6	6
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE7	7
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE8	8
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE9	9
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE10	10
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE11	11
	#define TIMESTAMP_RESET_CTRL_SOURCE_LINE12	12
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE13	13
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE14	14
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE15	15
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE16	16
	#define TIMESTAMP_RESET_CTRL_SOURCE_TRIGGER	 17
	#define TIMESTAMP_RESET_CTRL_SOURCE_EXP_ACTIVE  18
    #define TIMESTAMP_RESET_CTRL_SOURCE_FRAME_READ 19
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE17	20
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE18	21
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE19	22
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE20	23
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE21	24
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE22	25
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE23	26
    #define TIMESTAMP_RESET_CTRL_SOURCE_LINE24	27
#define TIMESTAMP_RESET_CTRL_SWRST_OFFSET	7
#define TIMESTAMP_RESET_CTRL_SWRST_SIZE  	1
#define TIMESTAMP_RESET_CTRL_INVERTER_OFFSET	8
#define TIMESTAMP_RESET_CTRL_INVERTER_SIZE  	1
#define TIMESTAMP_RESET_CTRL_OPMODE_OFFSET	9
#define TIMESTAMP_RESET_CTRL_OPMODE_SIZE  	1
#define TIMESTAMP_RESET_CTRL_ARM_OFFSET	12
#define TIMESTAMP_RESET_CTRL_ARM_SIZE  	1

// MX-X4G3-FF Diagnostics register
#define FPGA_REG_MX_X4G3_FF_DIAG                        86
// FPGA_REG_MX_X4G3_FF_DIAG - WRITE Offsets
#define FPGA_REG_MX_X4G3_FF_DIAG_WR_OFFSET_SELECTOR     24  // 8 bits
// FPGA_REG_MX_X4G3_FF_DIAG - READ Offsets
#define FPGA_REG_MX_X4G3_FF_DIAG_RD_OFFSET_DATA         0   // 16 bits
#define FPGA_REG_MX_X4G3_FF_DIAG_RD_OFFSET_SELECTOR     24  // 8 bits
// VOltages, Currents
#define MX_X4G3_FF_DIAG_SELECTOR_PAC1934_CH1_I          0
#define MX_X4G3_FF_DIAG_SELECTOR_PAC1934_CH1_V          1
#define MX_X4G3_FF_DIAG_SELECTOR_PAC1934_CH2_I          2
#define MX_X4G3_FF_DIAG_SELECTOR_PAC1934_CH2_V          3
#define MX_X4G3_FF_DIAG_SELECTOR_PAC1934_CH3_I          4
#define MX_X4G3_FF_DIAG_SELECTOR_PAC1934_CH3_V          5
#define MX_X4G3_FF_DIAG_SELECTOR_PAC1934_CH4_I          6
#define MX_X4G3_FF_DIAG_SELECTOR_PAC1934_CH4_V          7
#define MX_X4G3_FF_DIAG_SELECTOR_ADS1115_VADJ_CH1       8
#define MX_X4G3_FF_DIAG_SELECTOR_ADS1115_VADJ_CH2       9
#define MX_X4G3_FF_DIAG_SELECTOR_ADS1115_VADJ_CH3       10
#define MX_X4G3_FF_DIAG_SELECTOR_ADS1115_VADJ_CH4       11
// Temperatures
#define MX_X4G3_FF_DIAG_SELECTOR_TMP112                 64
#define MX_X4G3_FF_DIAG_SELECTOR_ADS1115_VTEMP_CH1      65
#define MX_X4G3_FF_DIAG_SELECTOR_ADS1115_VTEMP_CH2      66
#define MX_X4G3_FF_DIAG_SELECTOR_ADS1115_VTEMP_CH3      67
#define MX_X4G3_FF_DIAG_SELECTOR_ADS1115_VTEMP_CH4      68

// FPGA_REG_BINNING - WRITE Offsets
#define FPGA_REG_BINNING_WR_OFFSET_ENABLE               0   // 1 bit
#define FPGA_REG_BINNING_WR_OFFSET_H_BIN                1   // 4 bits
#define FPGA_REG_BINNING_WR_OFFSET_V_BIN                5   // 4 bits
#define FPGA_REG_BINNING_WR_OFFSET_H_BIN_PAT            9   // 1 bit
#define FPGA_REG_BINNING_WR_OFFSET_LINE_LENGTH          10  // 12 bits
#define FPGA_REG_BINNING_WR_OFFSET_V_SHIFT              22  // 2 bits
#define FPGA_REG_BINNING_WR_OFFSET_H_SHIFT              24  // 2 bits
#define FPGA_REG_BINNING_WR_OFFSET_BLK_SUB              26  // 1 bit
//#define FPGA_REG_BINNING_WR_                          27  // 3 bits reserved
#define FPGA_REG_BINNING_WR_OFFSET_SELECTOR             30  // 2 bits
// FPGA_REG_BINNING - READ Offsets
#define FPGA_REG_BINNING_RD_OFFSET_ENABLE               0   // 1 bit
#define FPGA_REG_BINNING_RD_OFFSET_H_BIN                1   // 4 bits
#define FPGA_REG_BINNING_RD_OFFSET_V_BIN                5   // 4 bits
#define FPGA_REG_BINNING_RD_OFFSET_H_BIN_PAT            9   // 1 bit
#define FPGA_REG_BINNING_RD_OFFSET_LINE_LENGTH          10  // 12 bits
#define FPGA_REG_BINNING_RD_OFFSET_V_SHIFT              22  // 2 bits
#define FPGA_REG_BINNING_RD_OFFSET_H_SHIFT              24  // 2 bits
#define FPGA_REG_BINNING_RD_OFFSET_BLK_SUB              26  // 1 bit
//#define FPGA_REG_BINNING_RD_                          27  // 3 bits reserved

#define FPGA_REG_BINNING_WR_SELECTOR_DEFAULT            0
#define FPGA_REG_BINNING_WR_SELECTOR_BLK_LEVEL          1

//-------------------------------------------------------------------------------------------------------------------
// IO Selector, Source, Mode, Inverter
//-------------------------------------------------------------------------------------------------------------------
typedef enum
{
	PCIE_COMM_LINE_MODE_INPUT             =0, // The selected physical line is used to Output an electrical signal.
	PCIE_COMM_LINE_MODE_OUTPUT            =1, // The selected physical line is used to Input an electrical signal.
} PcieCommonLineMode;

typedef enum
{
	PCIE_COMM_LINE_INVERTER_FALSE         =0, // The IO Line signal is not inverted.
	PCIE_COMM_LINE_INVERTER_TRUE          =1, // The IO Line signal is inverted.
} PcieCommonLineInverterMode;

// Line Source
typedef enum
{
	PCIE_COMM_LINE_SOURCE_OFF                = 0,  // Low
	PCIE_COMM_LINE_SOURCE_ON                 = 1,  // High
	PCIE_COMM_LINE_SOURCE_BLINK              = 2,  // Blink
	PCIE_COMM_LINE_SOURCE_TXD                = 3,  // UART TxD
	PCIE_COMM_LINE_SOURCE_IMG_EXP            = 4,  // Exposure active
	PCIE_COMM_LINE_SOURCE_IMG_READ           = 5,  // Readout active
	PCIE_COMM_LINE_SOURCE_IMG_BUSY           = 6,  // Exp + FOT + Readout
	PCIE_COMM_LINE_SOURCE_PCIE_SPEED         = 7,  // Link status 1
	PCIE_COMM_LINE_SOURCE_PCIE_LANE          = 8,  // Link status 2
	PCIE_COMM_LINE_SOURCE_FRAME_BUF_FULL     = 9,  // Frame buffer full
	PCIE_COMM_LINE_SOURCE_IMG_EXP_FIRST_ROW  = 10, // Exposure active of the first row
    PCIE_COMM_LINE_SOURCE_IMG_EXP_ALL_ROWS   = 11, // Exposure active of the all rows
	PCIE_COMM_LINE_SOURCE_IO_STATUS_1        = 12, // IO status
	PCIE_COMM_LINE_SOURCE_IO_STATUS_2        = 13, 
	PCIE_COMM_LINE_SOURCE_IO_STATUS_3        = 14, 
	PCIE_COMM_LINE_SOURCE_IO_STATUS_4        = 15, 
	PCIE_COMM_LINE_SOURCE_IO_STATUS_5        = 16, 
	PCIE_COMM_LINE_SOURCE_IO_STATUS_6        = 17, 
	PCIE_COMM_LINE_SOURCE_IO_STATUS_7        = 18, 
	PCIE_COMM_LINE_SOURCE_IO_STATUS_8        = 19, 
	PCIE_COMM_LINE_SOURCE_IO_STATUS_9        = 20, 
	PCIE_COMM_LINE_SOURCE_IO_STATUS_10       = 21, 
	PCIE_COMM_LINE_SOURCE_IO_STATUS_11       = 22, 
	PCIE_COMM_LINE_SOURCE_IO_STATUS_12       = 23, 
	PCIE_COMM_LINE_SOURCE_USER_OUTPUT1       = 24, // User output value
	PCIE_COMM_LINE_SOURCE_USER_OUTPUT2       = 25, 
	PCIE_COMM_LINE_SOURCE_USER_OUTPUT3       = 26, 
	PCIE_COMM_LINE_SOURCE_USER_OUTPUT4       = 27, 
	PCIE_COMM_LINE_SOURCE_USER_OUTPUT5       = 28, 
	PCIE_COMM_LINE_SOURCE_USER_OUTPUT6       = 29, 
	PCIE_COMM_LINE_SOURCE_USER_OUTPUT7       = 30, 
	PCIE_COMM_LINE_SOURCE_USER_OUTPUT8       = 31, 
    PCIE_COMM_LINE_SOURCE_IO_STATUS_13      = 32,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_14      = 33,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_15      = 34,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_16      = 35,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_17      = 36,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_18      = 37,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_19      = 38,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_20      = 39,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_21      = 40,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_22      = 41,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_23      = 42,
    PCIE_COMM_LINE_SOURCE_IO_STATUS_24      = 43,
    PCIE_COMM_LINE_SOURCE_EXPOSURE_PULSE    = 44,
} PCIeCommonLineSource;

//-------------------------------------------------------------------------------------------------------------------
// Trigger Selector, Mode, Source, Activation, Overlap
//-------------------------------------------------------------------------------------------------------------------

// Trigger Selector - defines action to be done on the trigger event - TRIG_CTRL_WR_OFFSET_SOURCE
typedef enum
{
	PCIE_COMM_TRIG_SELECT_ACQ_START          = 0, // Acquisition will be started upon trigger event,
	PCIE_COMM_TRIG_SELECT_FRAME_START        = 1, // One Frame capture will be started upon trigger event,
	PCIE_COMM_TRIG_SELECT_FRAME_BURST_START  = 2, // Burst of Frames capture will be started upon trigger event,
	PCIE_COMM_TRIG_SELECT_EXP_ACTIVE         = 3, // Active Exposure time will be defined by the trigger event,
    PCIE_COMM_TRIG_SELECT_EXP_START          = 4, // Exposure of one Frame will be started upon trigger event,
    PCIE_COMM_TRIG_SELECT_MULTI_SLOPE_CHANGE = 5, // Multi Slope Exposure phase will be changed
	PCIE_COMM_TRIG_SELECT_DUAL_EXP_HDR_INTERLINE = 6, // Dual exposure HDR interline (e.g. CMV50000)
} PCIeCommonTrigSelect;

typedef enum
{
    PCIE_COMM_TRIG_SOURCE_SOFT      =0, //
	PCIE_COMM_TRIG_SOURCE_LINE0     =1, //
	PCIE_COMM_TRIG_SOURCE_LINE1     =2, //
	PCIE_COMM_TRIG_SOURCE_LINE2     =3, //
	PCIE_COMM_TRIG_SOURCE_LINE3     =4, //
	PCIE_COMM_TRIG_SOURCE_LINE4     =5, //
	PCIE_COMM_TRIG_SOURCE_LINE5     =6, //
	PCIE_COMM_TRIG_SOURCE_LINE6     =7, //
	PCIE_COMM_TRIG_SOURCE_LINE7     =8, //
	PCIE_COMM_TRIG_SOURCE_LINE8     =9, //
	PCIE_COMM_TRIG_SOURCE_LINE9     =10,//
	PCIE_COMM_TRIG_SOURCE_LINE10    =11,//
    PCIE_COMM_TRIG_SOURCE_LINE11    = 12,//
    PCIE_COMM_TRIG_SOURCE_LINE12    = 13,//
    PCIE_COMM_TRIG_SOURCE_LINE13    = 14,//
    PCIE_COMM_TRIG_SOURCE_LINE14    = 15,//
    PCIE_COMM_TRIG_SOURCE_LINE15    = 16,//
    PCIE_COMM_TRIG_SOURCE_LINE16    = 17,//
    PCIE_COMM_TRIG_SOURCE_LINE17    = 18,//
    PCIE_COMM_TRIG_SOURCE_LINE18    = 19,//
    PCIE_COMM_TRIG_SOURCE_LINE19    = 20,//
    PCIE_COMM_TRIG_SOURCE_LINE20    = 21,//
    PCIE_COMM_TRIG_SOURCE_LINE21    = 22,//
    PCIE_COMM_TRIG_SOURCE_LINE22    = 23,//
    PCIE_COMM_TRIG_SOURCE_LINE23    = 24,//
} PcieCommonTrigSource;

typedef enum
{
	PCIE_COMM_TRIG_ACTIV_RISING_EDGE       =0, //
	PCIE_COMM_TRIG_ACTIV_FALLING_EDGE      =1, //
	PCIE_COMM_TRIG_ACTIV_ANY_EDGE          =2, //
	PCIE_COMM_TRIG_ACTIV_LEVEL_HIGH        =3, //
	PCIE_COMM_TRIG_ACTIV_LEVEL_LOW         =4, //
} PcieCommonTrigActivation;

typedef enum
{
	PCIE_COMM_TRIG_OVERLAP_OFF             =0, //
	PCIE_COMM_TRIG_OVERLAP_READ            =1, //
	PCIE_COMM_TRIG_OVERLAP_PREVIOUS_FRAME  =2, //
} PcieCommonTrigOverlap;

// Trigger Mode
typedef enum
{
	PCIE_COMM_TRIG_MODE_OFF                  = 0, //
	PCIE_COMM_TRIG_MODE_ON                   = 1, //
} PCIeCommonTrigMode;

// Packer mode
typedef enum
{
    PCIE_COMM_PACK_MONO8        = 0,
    PCIE_COMM_PACK_MONO16       = 1,
    PCIE_COMM_PACK_MONO12       = 2,
    PCIE_COMM_PACK_MONO10       = 3,
    PCIE_COMM_PACK_MONO8P16     = 4,
    PCIE_COMM_PACK_MONO12MSB    = 5,
    PCIE_COMM_PACK_MONO10MSB    = 6,
    PCIE_COMM_PACK_MONO8P16MSB  = 7,
    PCIE_COMM_PACK_MONO10P      = 8,
    PCIE_COMM_PACK_MONO12P      = 9,
    PCIE_COMM_PACK_MONO9P       = 10,
    PCIE_COMM_PACK_MONO11P      = 11,
    PCIE_COMM_PACK_MONO14       = 12,   // 16 bit output - [x x b13 b12 b11 ... b0]
    PCIE_COMM_PACK_MONO14MSB    = 13,   // 16 bit output - [b13 b12 ... b1 b0 x x]
    PCIE_COMM_PACK_MONO14P      = 14,   // 14 bit output - [b13 b12 ... b1 b0]
} PCIeCommonPackerMode;


// Interrupt Events Enumeration - INT_CFG_WR_OFFSET_EVENT_SELECTOR
typedef enum
{
	EVENT_ACQUISITION_TRANSFER_END      = 0, 
	EVENT_ACQUISITION_FRAME_LOST        = 1, 
	EVENT_LINE0_RISING_EDGE             = 32,
} PCIeCommonEventSelect;

// Exposure Mode Enumerator - ACQ_CTRL_RD_OFFSET_EXP_MODE; Note - identifiers are different than FGenTL_ITEM_ExposureMode (e.g. FGenTL_ITEM_ExposureModeEnum_Timed = 1)
typedef enum 
{
	ACQ_EXP_MODE_TIMED                  = 0,   // Timed exposure. The exposure duration is set using the ExposureTime feature
	ACQ_EXP_MODE_TRIG_WIDTH             = 1,   // Uses the width of the current Frame or Line trigger signal(s) to control the exposure duration.
} PCIeCommonAcquisitionExposureMode;

// FPGA binning  horizontal pattern Enumerator - FPGA_REG_BINNING_WR_OFFSET_H_BIN_PAT
typedef enum
{
    FPGA_BIN_H_PATTERN_MONO = 0,    // Mono binning pattern
    FPGA_BIN_H_PATTERN_BAYER = 1,   // Bayer binning pattern
} PCIeCommonFPGABinningHorizontalPattern;


//-------------------------------------------------------------------------------------------------------------------
// Flash Transaction Commands (Write Enable, Sector Erase, Pape Program,...) - FLASH_SPI_CMD_WR_OFFSET_CMD
//-------------------------------------------------------------------------------------------------------------------
#define	SPI_FLASH_CMD_WREN	            0x06
#define	SPI_FLASH_CMD_SE	            0xD8
#define	SPI_FLASH_CMD_READ	            0x03
#define	SPI_FLASH_CMD_RDSR1	            0x05
#define	SPI_FLASH_CMD_PP	            0x02

