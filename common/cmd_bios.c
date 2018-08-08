/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.
*/

#include <common.h>
#include <command.h>
#include <memalign.h>
#include <net.h>
#include <usb.h>
#include <gps.h>
#include <i2c.h>
#include <tpm.h>
#include <spi.h>
#include <rf.h>
#include <asm/gpio.h>
#include <environment.h>
#include <pca953x.h>
#include <model.h>
#include <cli.h>
#include "moxa_bios/moxa_console.h"
#include "moxa_bios/moxa_lib.h"
#include "moxa_bios/moxa_boot.h"
#include "moxa_bios/moxa_upgrade.h"
#include "moxa_bios/sys_info.h"
#include "cmd_bios.h"
#include "mmc.h"
#include "fs.h"
#include "fat.h"
#include "cmd_bios.h"
#include "predefine_table.h"
#include "types.h"
#include "watchdog.h"

DECLARE_GLOBAL_DATA_PTR;

static char ReturnUboot = 0;

extern int diag_do_check_gps(void);
char CharBuf[256];
char ShortBuf[64];
char CmdMsg[64];
const char RUNBAR[4][8] = {"\\\r","| \r","/ \r","- \r"};
const char AbortString[20] = {"Abort this test!\r\n"};
const char ESCString[48] = {"Please press 'ESC' to Quit this testing ...\r\n"};
static int WDT_MODE = 0;
extern char usb_start_port;
extern char usb_stop_port;
data_pattern DataPattern; // That pattern that we use in all MOXA BIOS
unsigned char test_pattern_byte[6] = {0x00, 0x55, 0x5A, 0xAA, 0xA5, 0xFF};
unsigned short test_pattern_short[6] = {0x0000, 0x5555, 0x5A5A, 0xAAAA, 0xA5A5, 0xFFFF};
unsigned long test_pattern_long[6] = {0x00000000, 0x55555555, 0x5A5A5A5A, 0xAAAAAAAA, 0xA5A5A5A5, 0xFFFFFFFF};
int usb_init_state = 0;
extern unsigned  int fw_tftp_size;
extern int usb_signal_init(struct usb_device *dev, struct _rf_info *rf_info, int init_state);
void diag_do_nothing(void){printf("\r\nNot Support!\r\n");return;}
board_infos sys_info;

#define BIOS_ITEM_FOR_BASIC_FUNC	1
#define BIOS_ITEM_FOR_FULL_FUNC		1
#define MMC_MAX_DEVICE			3

#if 1
struct _rf_info rf_info[32]={
// vendor,   product,   at_out_bulk, at_in_bulk, gps_out_bulk, gps_in_bulk,   product_Name
	{0x1199,    0x9011,         0x3,       0x5,        0x1,           0x1,        "MC8305"},
	{0x1199,    0x68C0,         0x4,       0x5,        0x1,           0x1,        "MC8090"},
	{0x1199,    0x9011,         0x3,       0x4,        0x2,           0x2,        "MC9090"},
	{0x1199,    0x68C0,         0x3,       0x4,        0x2,           0x2,        "MC7304"},
	{0x1199,    0x68C0,         0x3,       0x4,        0x2,           0x2,        "MC7354"},
	{0x1546,    0x1141,         0x2,       0x3,        0x5,           0x7,        "MODEM-LTE"},
	{0x0bc3,    0x0001,         0x1,       0x1,        0x1,           0x1,        "IPWireless Modem"},
	{0x0,       0x0,            0x0,       0x0,        0x0,           0x0,        "Unknown"}
};
#endif



void diag_do_show_ip(void);
void diag_do_change_ip(void);

void diag_set_os_cmdline(void)
{
        int ret = 0;
        char buf [256] = "\0";
        char *s;

        s = getenv("bootargs_set");

        printf("\nCurrent OS cmdline : ");

        if(s == NULL)
                printf("\nconsole=ttymxc0,115200 root=/dev/mmcblk0dp2 rootwait rw\n");
        else
                printf("\n%s\n", s);

        printf("Set OS cmdline:\n");
        

        if ((ret = con_get_string (CharBuf, sizeof (CharBuf))) <= 0) 
        {
		printf("Set OS cmdline Fail [%d] \r\n", ret);
		return;
        }

        if(ret > sizeof (CharBuf))
        {
		printf("Set OS cmdline Fail [%d] \r\n", ret);
		return;
        }


        sprintf(buf, "setenv bootargs_set %s", CharBuf);

	if ((ret = run_command(buf, 0)) != 0){
		printf("Set OS cmdline Fail [%d] \r\n", ret);
		return;
	}

	if ((ret = run_command("saveenv", 0)) != 0){
		printf("Set OS cmdline Fail [%d] \r\n", ret);
		return;
	}

}

void diag_set_os_cmdline_to_default (void)
{
        int ret = 0;
        char *s;

        s = getenv("bootargs_set");

        printf("\nCurrent OS cmdline : ");
        if(s == NULL)
                printf("console=ttymxc0,115200 root=/dev/mmcblk0dp2 rootwait rw\n");
        else
                printf("%s\n", s);

        printf("set OS cmdline to default.\n");
        

        sprintf(CmdMsg, "env delete bootargs_set");

	if ((ret = run_command(CmdMsg, 0)) != 0){
		printf("Set OS cmdline to default Fail [%d] \r\n", ret);
		return;
	}

	if ((ret = run_command("saveenv", 0)) != 0){
		printf("Set OS cmdline to default Fail [%d] \r\n", ret);
		return;
	}
}

burnin_cmd set_OS_cmdline_cmd_value[] = {
	{0, '0', "set OS cmdline", diag_set_os_cmdline, 1},
	{1, '1', "set to default", diag_set_os_cmdline_to_default, 1}, //R42A
	{-1,'*', "",0,0}
};

void diag_do_set_OS_cmdline(void)
{
	burnin_cmd *burnin_temp = set_OS_cmdline_cmd_value;
	bios_manual_testing (burnin_temp);
}

burnin_cmd BIOS_config_cmd_value [] = {
        {1, 1, "TPM2 Setting", diag_do_tpm2_config, 1},
	{1, 1, "Update Firmware from Tftp", diag_do_tftp_download_firmware, 1},
        {1, 1, "Set OS cmdline", diag_do_set_OS_cmdline, 1},
	{1, 1, "Go To OS", diag_do_run_mmc_func, BIOS_ITEM_FOR_BASIC_FUNC},
 	{99, 'q', "UBoot Command Line", diag_do_uboot, BIOS_ITEM_FOR_BASIC_FUNC},	
	{-1, '*', "", 0, 0}
};


void bios_print_item_msg(burnin_cmd *cmd_list)
{

	int i = 0, j = 0, k = 0;
	char *s;

	burnin_cmd *burnin_temp = cmd_list;

	printf("\r\n----------------------------------------------------------------------------");
	
	s = getenv("modelname");
	printf("\r\n  Model: %s",s);

	printf("\r\n  Boot Loader Version: ");
	s = getenv ("biosver");
	printf("%s         ", s);

	//Check_CPU_Type(1);
	
	printf ("\r\n  Build date: " __DATE__ " - " __TIME__);	

	s = getenv("serialnumber");
	printf("   Serial Number: %s", s);

	s = getenv("ethaddr");
	printf("\r\n  LAN1 MAC: %s", s);

	s = getenv ("eth1addr");

	printf("          LAN2 MAC: %s", s);
	printf("\r\n----------------------------------------------------------------------------\r\n");

	while (burnin_temp->index >= 0) {
		if (burnin_temp->exist) {
			if ((burnin_temp->index == 99) || (burnin_temp->index == 98)) {
				printf("(%c)",burnin_temp->symbol);
			} else {
				if (k == 26)
					k++;

				if (k <= 9) {
					printf ("(%c)",k+'0');
					burnin_temp->symbol = k+'0';
				} else {
					printf ("(%c)",k-10+'a');
					burnin_temp->symbol = k-10+'a';
				}
			}

			printf(" %s",burnin_temp->string);
			
			for(j = strlen((char *)burnin_temp->string); j < 31; j++){
				printf(" ");
			}

			k++;
			i++;

			if((i % 2) == 0) {
				printf("\r\n");
			}
		}

		burnin_temp++;
	}

	if((i % 2) != 0) {
		printf("\r\n");
	}
}

int bios_parse_cmd (char* buffer)
{
	char seps[]  = " ";
	char *token;
	char cmd_buf[256];
	unsigned int i;

	token = strtok (buffer, seps);
	
	if (token == NULL)
		return (-1);

	strcpy (cmd_buf, token);
	
	if ((strcmp (cmd_buf, "q") == 0) ||(strcmp (cmd_buf, "Q") == 0))
		return (-2);

	i = atoi_simple (cmd_buf);

	return i;
}

char bios_parse_cmd_char(char* buffer)
{
	char seps[]  = " ";
	char *token;
	char cmd_buf[256];

	token = strtok(buffer, seps);

	if (token == NULL)
		return (-1);

	strcpy(cmd_buf, token);
	
	if (cmd_buf[0] >= 'A' && cmd_buf[0] <= 'Z')
		cmd_buf[0] += (0x20);

	printf("\r\n");

	return cmd_buf[0];
}

/**	\brief
 *
 *	Get input value from user and convert to the specfic format.
	mode:
		 DIAG_GET_VALUE_HEX_MODE
		 DIAG_GET_VALUE_DEC_MODE
		 DIAG_GET_VALUE_NO_ABORT
 *
*/
int diag_get_value(char *title, unsigned int *val, unsigned int min, unsigned int max, int mode)
{
	while(1) {

		printf("\r\n%s (", title);

		if (mode == DIAG_GET_VALUE_HEX_MODE)
			printf("0x%lX-0x%lX", (unsigned long)min, (unsigned long)max);
		else
			printf("%lu-%lu", (unsigned long)min, (unsigned long)max);

		if (mode & DIAG_GET_VALUE_NO_ABORT)
			printf("): ");
		else
			printf(",enter for abort): ");

		if (con_get_string(CharBuf,sizeof(CharBuf)) == -1){
			printf(AbortString);
			return DIAG_ESC;
		}

		if (CharBuf[0] == 0)
			return DIAG_FAIL;

		if ((bios_parse_num(CharBuf, val, 0, 0) || *val < min || *val > max ) == DIAG_FAIL) {
			printf("\r\n%d: Invalid number !", *val);
			continue;
		}

		break;
	}

	printf("\r\n");
	
	return DIAG_OK;

}

/**	\brief
 *
 *	BIOS Init function. 
 *
 */
void bios_init_global_value(void)
{
	//extern rtc_time_block RtcTime;
	// 1. DataPattern
	DataPattern.pat0 = DATA_PATTERN0;
	DataPattern.pat1 = DATA_PATTERN1;
	DataPattern.pat2 = DATA_PATTERN2;
	DataPattern.pat3 = DATA_PATTERN3;
	DataPattern.pat4 = DATA_PATTERN4;
	DataPattern.pat5 = DATA_PATTERN5;
	/*
	// 2. RTC Time block
	RtcTime.t_year = IXP_RTC_BASE_YEAR;
	RtcTime.t_mon = 1;
	RtcTime.t_mday = 1;
	*/
	return;
}

void diag_do_uboot(void)
{
	printf("\r\n\r\n");
	ReturnUboot = 1;
}

/**	\brief
 *
 *	Show the BIOS menu.
 *
 */
int bios_manual_testing(burnin_cmd *cmd_list)
{
	unsigned long ret = 0;

	while(1){
		burnin_cmd *burnin_temp = cmd_list;
		bios_print_item_msg(burnin_temp);
		printf("---------------------------------------------------------------------------\r\n");
		printf("Command>>");

		if(con_get_string(CmdMsg,sizeof(CmdMsg)) == -1){
			printf("\r\n");
			break;
		}

		ret = bios_parse_cmd_char(CmdMsg);
	
		if (ret == (-1)) {
			continue;
		} else if(ret == 'q' || ret == 'Q'){
			printf("\r\n");
			break;
		}

		while (burnin_temp->index != (-1)) {
			if ((ret == burnin_temp->symbol ) && (burnin_temp->exist)) {
				(*burnin_temp->burnin_routine)();
				break;
			}

			burnin_temp++;
		}

		if (ReturnUboot)
			break;
	}
	return ret;
}

int bios_manual_config(burnin_cmd *cmd_list)
{
	unsigned long ret = 0;

	while(1) {
		burnin_cmd *burnin_temp = cmd_list;
		bios_print_item_msg(burnin_temp);
		printf("---------------------------------------------------------------------------\r\n");
		printf("Command>>");

		if (con_get_string(CmdMsg,sizeof(CmdMsg)) == -1) {
			printf("\r\n");
			break;
		}

		ret = bios_parse_cmd_char(CmdMsg);

		if (ret == (-1)) {
			continue;
		} else if (ret == 'q' || ret == 'Q') {
			printf("\r\n");
			break;
		}

		while(burnin_temp->index != (-1)) {
			if ((ret == burnin_temp->symbol) && (burnin_temp->exist)) {
				(*burnin_temp->burnin_routine)();
				break;
			}

			burnin_temp++;
		}

		if (ReturnUboot)
			break;
	}

	return ret;
}

int do_tftp_download_firmware (void)
{
	char *fw_name = "firmware.img";
	int ret = 0;
	char buf[MAX_SIZE_128BYTE] = {0};
	unsigned int set_or_clean = 0;

	printf("\r\n Current IP Address \n");
	
	//ret = show_ip();

	if (ret)
		goto EXIT;

	printf("\nDo you want to change the ip address?\n");

	diag_get_value("0 - No, 1 - Yes", &set_or_clean, 0, 1, DIAG_GET_VALUE_DEC_MODE);

	if (set_or_clean == 1){
		ret = change_ip();
	
		if(ret)
			goto EXIT;
	}
                
	printf ("\r\nFirmware File Name (%s): ", fw_name);

	if ((ret = con_get_string (buf, sizeof (buf))) == -1) {
		printf ("\r\n");
		goto EXIT;
	} else if (ret == 0) {
		sprintf (buf, "%s", fw_name);
	}

	printf("download firmware name:%s\n", buf);

	printf ("\n");

	ret = tftp_download_firmware(buf);

EXIT:

        return ret;

}

void diag_do_tftp_download_firmware (void)
{
	int ret = 0;

	ret = do_tftp_download_firmware();


	printf("**************************************************\n");
	printf("*                                                *\n");
	
	if(ret)
		printf("*          TFTP FIRMWARE file transfer fail.     *\n");
        else
		printf("*          TFTP FIRMWARE file transfer success   *\n");
	
	printf("*                                                *\n");
	printf("**************************************************\n");

	return;
}

int do_get_hw_verison(char *s) {

	i2c_set_bus_num(0);

	return eeprom_read(EEPROM_I2C_ADDR, HW_VERSION_BASE_ADDR, (unsigned char *) s, MAX_HW_VERSION_SIZE);
}

int sys_board_init(void) {

	int ret = 0;

	ret = board_info_init(&sys_info);
	
	if (ret)
		printf("Board init Fail\n");

	return ret;
}

void diag_do_tpm2_enable_func(int enable) 
{

	char *s;
	char str[128] = {0};
	int state = 0;

	s = getenv("en_tpm2");

	if (NULL != s) {
		state = (int)simple_strtol(s, NULL, 10);
	} else {
		diag_set_env_conf_u32("en_tpm2", 1);	
		state = 1;	
	}

	if (enable)
		printf("Current State: Enable\n");
	else
		printf("Current State: Disable\n");

	if (enable != state) {
		sprintf(str, "setenv en_tpm2 %d", enable);
		run_command(str, 0);
		run_command("saveenv", 0);
	}

	printf("\r\n### Please reboot the system to complete the operation ###\n");
	
	while(1);

	return;
}

void diag_do_set_tpm2_enabled_func(void)
{
	diag_do_tpm2_enable_func(1);
}

void diag_do_set_tpm2_disabled_func(void)
{
	diag_do_tpm2_enable_func(0);
}

burnin_cmd tpm2_config_cmd_value[] = {
	{ 0,'0',"Enable TPM2", diag_do_set_tpm2_enabled_func, 1},
	{ 1,'1',"Disable TPM2", diag_do_set_tpm2_disabled_func, 1},
	{ -1,'*',"",0,0}
};

void diag_do_tpm2_config(void)
{

	char *s;
	int valid = 0;

	s = getenv("tpm2");

	if (NULL != s) {
		valid = (int)simple_strtol(s, NULL, 10);
	} else {
		diag_set_env_conf_u32("tpm2", 0);
		valid = 0;
	}

	if(valid == 1){
		burnin_cmd *burnin_temp = tpm2_config_cmd_value;
		bios_manual_config(burnin_temp);
	} else {
		printf("\r\n***This module does not support TPM2 features***\n");
	}
}

void diag_do_run_mmc_func(void)
{
	printf ("Boot to MMC1: eMMC...\n");
	do_run_mmc_boot(&sys_info);
}


int do_jump_bios_config(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

	burnin_cmd *burnin_temp = BIOS_config_cmd_value;

	ReturnUboot = 0;

	bios_init_global_value();

	while (1) {
		if (ReturnUboot)
			break;

		bios_manual_testing(burnin_temp);
	}

	return 1;
}

U_BOOT_CMD(
	bios,     1,     0,      do_jump_bios_config,
	"bios      - Jump into BIOS config menu.\n",
	"Type \"bios\" to jump into BIOS config menu. \n"
);

