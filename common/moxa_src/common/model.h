/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.
*/
/*
    model.h

    2017-09-09	HsienWen tsai.			Create it.
*/

#ifndef _MODEL_H
#define _MODEL_H

#define K4B4G1646B 1
#define K4B2G1646B 2
#define K4B4G1646D 3

#define VersionMajor 1
#define VersionMinor 0

enum {
	AM335X_UC5101 = 0,
	AM335X_UC5102,
	AM335X_UC5111,
	AM335X_UC5112,
};

/*#define DDR_TYPE       K4B4G1646D
#include <asm/arch/ddr_defs.h>*/

#define	Update_BIOS_Ver	                1
const char __weak BIOS_version_string[] = "1.0.0S09";

#define PCA9535_LED_UC8100ME 

#define PCA9535_UART_MODE_W6000

#define PCA9535_UART_MODE_SHIFT_1 (0)
#define PCA9535_UART_MODE_SHIFT_2 (4)
#define PCA9535_UART_MODE_SHIFT_3 (8)
#define PCA9535_UART_MODE_SHIFT_4 (12)

/*int _serial_tstc(const int port);
void _serial_puts(const char *s, const int port);
int _serial_getc(const int port);
void _serial_setbrg(const int port);*/

#define UART_PORT4                  4
#define UART_PORT3                  3

/*********************************************************
 *			MPTEST DEFINE
 *********************************************************/
#define SDRAM_MP				1
#define LAN_10M_CROSSCABLE_MP			2
#define LAN_100M_CROSSCABLE_MP			3
//#define DIO_MP                                  4
#define	RS232_MP				5
#define	RS422_MP				6
#define	RS4852W_MP				7
#define	LED_MP					8
#define RTC_MP					9
#define	SD_MP					10
#define	USBH_MP					11
#define	USB_CELLULAR_MP				12
#define	TPM2_MP					13
//#define	TPM_MP					11
#define WDT_MP					14
#define DCAN_MP                                 15
#define CELLULAR_GPS_MP			1
#define CELLULAR_GSM_MP			2

#define SD_0					1
#define LAN1_10M				1
#define LAN2_10M				1
#define LAN1_100M				1
#define LAN2_100M				1
#define RTC_DS1374				3
#define RTC_TEST_CHIP				RTC_DS1374
#define	SD_LED_T				0
#define	Zigbee_LED				0
#define	USB_PLUG_LED				0
#define BIOS_DARAM_TEST				1
#define BIOS_RTC_TEST				1
#define BIOS_USBHOST_TEST			1
#define BIOS_10M_TEST				1
#define BIOS_100M_TEST				1


/*****************************************************
 *			GPIO DEFINE
 ****************************************************/
#define IOX_BEEP                IMX_GPIO_NR(1, 12)

#define IOX_USB1_SW             IMX_GPIO_NR(6, 13)
//#define IOX_USB1_PWR IMX_GPIO_NR(1, 5)
#define IOX_USB1_OC             IMX_GPIO_NR(1, 4)

#define IOX_USB2_HUB_RST        IMX_GPIO_NR(4, 6)
//#define IOX_USB2_PWR IMX_GPIO_NR(1, 7)
#define IOX_USB2_OC             IMX_GPIO_NR(1, 6)

#define IOX_MCPIE2_DIS          IMX_GPIO_NR(4, 5)
#define IOX_WIFI_PWR_EN         IMX_GPIO_NR(4, 20)
#define IOX_BT_EN               IMX_GPIO_NR(4, 23)
#define IOX_LTE_PWR_EN          IMX_GPIO_NR(4, 7)
#define IOX_USB_OTG1_PWWR       IMX_GPIO_NR(1, 4)
#define PIO_LED_SD              IMX_GPIO_NR(1, 12)  
#define PIO_SW_BUTTON	        IMX_GPIO_NR(5, 11)

#define PIO_LED_MP_0		IMX_GPIO_NR(6, 18)
#define PIO_LED_MP_1		IMX_GPIO_NR(6, 12)		
#define PIO_LED_MP_2		IMX_GPIO_NR(6, 17)		
#define PIO_LED_MP_3		IMX_GPIO_NR(6, 16)		
#define PIO_LED_MP_4		IMX_GPIO_NR(6, 15)		

#define PIO_LED_WL_1				(1 << 4)
#define PIO_LED_WL_2				(1 << 5)
#define PIO_LED_WL_3				(1 << 12)
#define PIO_LED_SIM_SEL				(1 << 8)
#define PIO_LED_USER_1				(1 << 9)
#define PIO_LED_USER_2				(1 << 10)
#define PIO_LED_CEL_1				(1 << 13)
#define PIO_LED_CEL_2				(1 << 14)
#define PIO_LED_CEL_3				(1 << 15)

#define	GPIO1_12				44
#define	GPIO1_13				45
#define	GPIO1_14				46
#define	GPIO1_15				47

#define PIO_MPE_RST				88
#define PIO_MPE_W_DIS				89
#define PIO_TPM_FORCE_CLEAR			87
#define PIO_UART_NUMBER				61
#define PIO_TPM_RST				117
#define PIO_TPM_INT				112
#define PIO_PSB_EN				86
#define PIO_PMIC_INT1				115
#define PIO_PMIC_SLEEP				11
#define PIO_BUZZER			        60
#define PIO_BT_EN				31
#define PIO_WLAN_EN				30
#define PIO_WLAN_IRQ				19
#define PIO_USER_LED				20
#define PIO_LED_READY				PIO_USER_LED
#define PIO_DO0                                 (1 << 0)
#define PIO_DO1                                 (1 << 1)
#define PIO_DO2                                 (1 << 2)
#define PIO_DO3                                 (1 << 3)
#define PIO_DI0                                 (1 << 8)
#define PIO_DI1                                 (1 << 9)
#define PIO_DI2                                 (1 << 10)
#define PIO_DI3                                 (1 << 11)

#endif //_MODEL_H

