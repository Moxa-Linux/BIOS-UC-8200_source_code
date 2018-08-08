/*
 * Copyright (C) 2015 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX7D SABRESD board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX7D_SABRESD_CONFIG_H
#define __MX7D_SABRESD_CONFIG_H

#include "mx7_common.h"

#define CONFIG_DBG_MONITOR
#define PHYS_SDRAM_SIZE			SZ_2G
#define CONFIG_SYS_MEMTEST_RESERVED     0x800000

/* Uncomment to enable secure boot support */
/* #define CONFIG_SECURE_BOOT */
#define CONFIG_CSF_SIZE			0x4000

/****************************MOXA**************************/
#define CONFIG_CMD_ETHLOOP
#define CONFIG_MUSB_HOST
/*********** MOXA CONFIG START*************/
#define CONFIG_FIT                          // for ITB IMAGE
#define CONFIG_VERSION_VARIABLE             // for BIOS version
#undef CONFIG_DM_SERIAL
#undef CONFIG_DM_GPIO
#define CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW     0x07
#undef USB_BUFSIZ
#define USB_BUFSIZ                    512
#define CONFIG_FAT_WRITE                1
#define CONFIG_I2C_REPEATED_START       1
#define CONFIG_MOXA_CONSOLE             1
#define CONFIG_MOXA_LIB                 1
#define CONFIG_MOXA_LED                 1
#define CONFIG_MOXA_ETH                 1
#define CONFIG_MOXA_SD                  1
#define CONFIG_MOXA_MEM                 1
#define CONFIG_MOXA_USB                 1
#define CONFIG_MOXA_UART                1
#define CONFIG_MOXA_DIO                 1
#define CONFIG_MOXA_WDT                 1
#define CONFIG_MOXA_SIERRA_GPS          1
#define CONFIG_MOXA_USB_SIGNAL_INIT     1
#define CONFIG_MOXA_CELLULAR            1
#define CONFIG_MOXA_OLED                1
#define CONFIG_MOXA_GPIO                1
#define CONFIG_MOXA_TPM                 1
#define CONFIG_MOXA_TPM2                1
#define CONFIG_MOXA_BOOT                1
#define CONFIG_MOXA_UPGRADE             1
#define EMMC_COPY_LIMIT_SIZE            31457280
#define	CONFIG_PHY_TI			1
/* #define CONFIG_BOOTDELAY		2 */
#define CONFIG_MOXA_RTC			1
#define CONFIG_RTC_DS1374		1
#define	CONFIG_CMD_DATE			1
#define	CONFIG_MOXA_TEST		1
#define	I2C_DS1374_ADDR			0x68
#define	I2C_DS1374_BUS			0x1

/*********** MOXA CONFIG END***********************/

/* Network */
#define CONFIG_PHY_GIGE                 1            // enable giga lan
#define CONFIG_CMD_MII
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define CONFIG_FEC_XCV_TYPE             RGMII
#define CONFIG_ETHPRIME                 "FEC"
/* ENET1 */
#define IMX_FEC_BASE			ENET_IPS_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR          1
/* ENET2 */
#define IMX_FEC_BASE2			ENET2_IPS_BASE_ADDR
#define CONFIG_FEC_MXC_PHY2ADDR         2

#define IMX7_USE_SAME_MDIO 

#define CONFIG_PHYLIB
#define CONFIG_PHY_REALTEK


/* MMC Config*/
#define CONFIG_SYS_FSL_ESDHC_ADDR       0

/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE3000
#define CONFIG_POWER_PFUZE3000_I2C_ADDR	0x08

#undef CONFIG_BOOTM_NETBSD
#undef CONFIG_BOOTM_PLAN9
#undef CONFIG_BOOTM_RTEMS

#undef CONFIG_CMD_EXPORTENV
#undef CONFIG_CMD_IMPORTENV

/* I2C configs */
#define CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C4		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_SPEED		100000

#define CONFIG_SUPPORT_EMMC_BOOT	/* eMMC specific */
#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

#define CONFIG_MFG_ENV_SETTINGS \
	"mfgtool_args=setenv bootargs console=${console},${baudrate} " \
		"rdinit=/linuxrc " \
		"g_mass_storage.stall=0 g_mass_storage.removable=1 " \
		"g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "\
		"g_mass_storage.iSerialNumber=\"\" "\
		"clk_ignore_unused "\
		"\0" \
	"initrd_addr=0x83800000\0" \
	"initrd_high=0xffffffff\0" \
	"bootcmd_mfg=run mfgtool_args;bootz ${loadaddr} ${initrd_addr} ${fdt_addr};\0" \

#define CONFIG_DFU_ENV_SETTINGS \
	"dfu_alt_info=image raw 0 0x800000;"\
		"u-boot raw 0 0x4000;"\
		"bootimg part 0 1;"\
		"rootfs part 0 2\0" \

#define CONFIG_EXTRA_ENV_SETTINGS \
	"mm_flags=0\0"\
	"serialnumber=IMOXA1234567\0"\
	"modelname=UC-8200\0"\
        "ethaddr=00:90:e8:33:55:41\0" \
        "eth1addr=00:90:e8:33:55:42\0" \
        "ipaddr=10.144.4.179\0" \
        "serverip=10.144.4.180\0" \
	"script=boot.scr\0" \
	"image=zImage\0" \
	"console=ttymxc0\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" 

#define CONFIG_BOOTCOMMAND \
	   "mmc dev ${mmcdev};" \
	   "mmc dev ${mmcdev}; if mmc rescan; then " \
		   "if run loadbootscript; then " \
			   "run bootscript; " \
		   "else " \
			   "if run loadimage; then " \
				   "run mmcboot; " \
			   "else run netboot; " \
			   "fi; " \
		   "fi; " \
	   "else run netboot; fi"

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + 0x20000000)

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ			1000

#define CONFIG_STACKSIZE		SZ_128K

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH
/*#define CONFIG_ENV_SIZE			SZ_8K
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_OFFSET		(8 * SZ_64K)*/
#define CONFIG_SYS_MAX_FLASH_BANKS	1

#define CONFIG_MXC_SPI
#define CONFIG_ENV_SIZE			(128 << 10)  //128KiB
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_ENV_SPI_MAX_HZ		CONFIG_SF_DEFAULT_SPEED
#define CONFIG_ENV_SECT_SIZE		(0x10000) /* 4 KB sectors */
#define CONFIG_ENV_OFFSET		0x180000 /* 1536 KiB in */
#define CONFIG_ENV_OFFSET_REDUND	0x1A0000 /* 896 KiB in */
#define MTDIDS_DEFAULT			"nor0=m25p80-flash.0"
#define MTDPARTS_DEFAULT		"mtdparts=m25p80-flash.0:128k(SPL)," \
					"512k(u-boot),128k(u-boot-env1)," \
					"128k(u-boot-env2),3464k(kernel)," \
					"-(rootfs)"

#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_MMC_ENV_DEV		0   /* USDHC1 */
#define CONFIG_SYS_MMC_ENV_PART		0	/* user area */
#define CONFIG_MMCROOT			"/dev/mmcblk0p2"  /* USDHC1 */

/* USB Configs */
#define CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX7
#define CONFIG_USB_STORAGE
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC  (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS   0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2

#define CONFIG_IMX_THERMAL

#define CONFIG_CI_UDC
#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_DUALSPEED

#define CONFIG_USB_GADGET
#define CONFIG_CMD_USB_MASS_STORAGE
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#define CONFIG_USB_GADGET_DOWNLOAD
#define CONFIG_USB_GADGET_VBUS_DRAW	2

#define CONFIG_G_DNL_VENDOR_NUM		0x0525
#define CONFIG_G_DNL_PRODUCT_NUM	0xa4a5
#define CONFIG_G_DNL_MANUFACTURER	"FSL"

/* USB Device Firmware Update support */
#define CONFIG_CMD_DFU
#define CONFIG_USB_FUNCTION_DFU
#define CONFIG_DFU_MMC
#define CONFIG_DFU_RAM

/* System EEPROM */
#define CONFIG_CMD_EEPROM
#define CONFIG_ENV_EEPROM_IS_ON_I2C
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x50	/* Main EEPROM */
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	1
#define CONFIG_SYS_EEPROM_PAGE_WRITE_DELAY_MS 10

#if 0
#define CONFIG_VIDEO
#ifdef CONFIG_VIDEO
#define CONFIG_CFB_CONSOLE
#define CONFIG_VIDEO_MXS
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_SW_CURSOR
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#endif
#endif

/* PCA953X */
#define PCA953X_ADDR1		0x20
#define PCA953X_ADDR2		0x21
#define PCA953X_ADDR3		0x24

#define CONFIG_SYS_I2C_PCA953X_ADDR0    0x24
#define CONFIG_SYS_I2C_PCA953X_ADDR1    0x21
#define CONFIG_SYS_I2C_PCA953X_ADDR2    0x22
#define SD_CD_INSERT	(1 << 10)
#define CONFIG_PCA953X	
#define CONFIG_SYS_I2C_PCA953X_WIDTH    { {0x20, 16} }
#define CONFIG_CMD_PCA953X
#define CONFIG_CMD_PCA953X_INFO

#define CONFIG_FSL_QSPI
#ifdef CONFIG_FSL_QSPI
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_MACRONIX
#define CONFIG_SPI_FLASH_BAR
#define CONFIG_SF_DEFAULT_BUS		0
#define CONFIG_SF_DEFAULT_CS		0
#define CONFIG_SF_DEFAULT_SPEED		40000000
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#define FSL_QSPI_FLASH_NUM		1
#define FSL_QSPI_FLASH_SIZE		SZ_8M
#define QSPI0_BASE_ADDR			QSPI1_IPS_BASE_ADDR
#define QSPI0_AMBA_BASE			QSPI0_ARB_BASE_ADDR
#endif

#endif	/* __CONFIG_H */
