/*
 * (C) Copyright 2006 FTZ Leipzig <www.ftz-leipzig.de>
 * Mirco Fuchs <fuchs@ftz-leipzig.de>
 *
 * Configuation settings for the eTW-ARM board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* ARM asynchronous clock */
#define AT91C_MAIN_CLOCK	180000000	/* from 12.000 MHz crystal (12000000 * 15) */
#define AT91C_MASTER_CLOCK	45000000	/* (AT91C_MAIN_CLOCK/4)	peripheral clock */

#define AT91_SLOW_CLOCK		32768	/* slow clock */

#define CONFIG_ARM920T		1	/* This is an ARM920T Core	*/
#define CONFIG_AT91RM9200	1	/* It's an Atmel AT91RM9200 SoC	*/
#define CONFIG_ETWARM		1	/* on a eTW-ARM board		*/
#undef  CONFIG_USE_IRQ			/* we don't need IRQ/FIQ stuff	*/
#define USE_920T_MMU		1
 
#define CONFIG_CMDLINE_TAG	1	/* enable passing of ATAGs	*/
#define CONFIG_SETUP_MEMORY_TAGS 1
#define CONFIG_INITRD_TAG	1

#ifndef CONFIG_SKIP_LOWLEVEL_INIT
#define CFG_USE_MAIN_OSCILLATOR		1
/* flash */
#define MC_PUIA_VAL	0x00000000
#define MC_PUP_VAL	0x00000000
#define MC_PUER_VAL	0x00000000
#define MC_ASR_VAL	0x00000000
#define MC_AASR_VAL	0x00000000
#define EBI_CFGR_VAL	0x00000000
#define SMC2_CSR_VAL	0x10003185 /* 1 RWHOLD, 16bit, 1 TDF, 5 WS */

/* clocks */
#define PLLAR_VAL	0x200EBF01 /* 180.000 MHz for PCK */
#define PLLBR_VAL	0x10083F01 /* 48.000 MHz (divider by 2 for USB) */
#define MCKR_VAL	0x00000302 /* PCK/4 = MCK Master Clock = 46.000 MHz from PLLA */

/* sdram */
#define PIOC_ASR_VAL	0xFFFF0000 /* Configure PIOC as peripheral (D16/D31) */
#define PIOC_BSR_VAL	0x00000000
#define PIOC_PDR_VAL	0xFFFF0000
#define EBI_CSA_VAL	0x00000002 /* CS1=SDRAM */
#define SDRC_CR_VAL	0x21914159 /* set up the SDRAM */
#define SDRAM		0x20000000 /* address of the SDRAM */
#define SDRAM1		0x20000080 /* address of the SDRAM + 128 Bytes, probably for CFG_GBL_DATA_SIZE*/
#define SDRAM_VAL	0x00000000 /* value written to SDRAM */
#define SDRC_MR_VAL	0x00000002 /* Precharge All */
#define SDRC_MR_VAL1	0x00000004 /* refresh */
#define SDRC_MR_VAL2	0x00000003 /* Load Mode Register */
#define SDRC_MR_VAL3	0x00000000 /* Normal Mode */
#define SDRC_TR_VAL	0x000002E0 /* Write refresh rate */
#endif	/* CONFIG_SKIP_LOWLEVEL_INIT */
/*
 * Size of malloc() pool
 */
#define CFG_MALLOC_LEN	(CFG_ENV_SIZE + 128*1024) /* 128 KiB reserved for environment data */
#define CFG_GBL_DATA_SIZE	128	/* size in bytes reserved for initial data */

#define CONFIG_BAUDRATE 115200

/* hardcode so no __divsi3 : AT91C_MASTER_CLOCK / baudrate / 16 */
#define CFG_AT91C_BRGR_DIVISOR	75

/*
 * Hardware drivers
 */

/* define one of these to choose the DBGU, USART0  or USART1 as console */
#define CONFIG_DBGU
#undef CONFIG_USART0
#undef CONFIG_USART1

#undef	CONFIG_HWFLOW			/* don't include RTS/CTS flow control support	*/

#undef	CONFIG_MODEM_SUPPORT		/* disable modem initialization stuff */

#define CONFIG_BOOTDELAY      3
/* #define CONFIG_ENV_OVERWRITE	1 */

#define CONFIG_COMMANDS		\
		       ((CONFIG_CMD_DFL | \
			CFG_CMD_JFFS2 | \
			CFG_CMD_DHCP | \
			CFG_CMD_PING ) & \
		      ~(CFG_CMD_BDI | \
			CFG_CMD_IMI | \
			CFG_CMD_AUTOSCRIPT | \
			CFG_CMD_FPGA | \
			CFG_CMD_MISC | \
			CFG_CMD_LOADS ))

/* this must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>

#define AT91_SMART_MEDIA_ALE (1 << 22)	/* our ALE is AD22 */
#define AT91_SMART_MEDIA_CLE (1 << 21)	/* our CLE is AD21 */

#define CONFIG_NR_DRAM_BANKS 1
#define PHYS_SDRAM		0x20000000
#define PHYS_SDRAM_SIZE		0x4000000  /* 64 megs */

#define CFG_MEMTEST_START	PHYS_SDRAM
#define CFG_MEMTEST_END			CFG_MEMTEST_START + PHYS_SDRAM_SIZE - 512*1024 - 4
#define CFG_ALT_MEMTEST			1
#define CFG_MEMTEST_SCRATCH		CFG_MEMTEST_START + PHYS_SDRAM_SIZE - 4

#define CONFIG_DRIVER_ETHER
#define CONFIG_NET_RETRY_COUNT		20
#undef CONFIG_AT91C_USE_RMII

#undef CONFIG_HAS_DATAFLASH
#define CFG_SPI_WRITE_TOUT		(5*CFG_HZ)
#define CFG_MAX_DATAFLASH_BANKS 	0
#define CFG_MAX_DATAFLASH_PAGES 	16384
#define CFG_DATAFLASH_LOGIC_ADDR_CS0	0xC0000000	/* Logical adress for CS0 */
#define CFG_DATAFLASH_LOGIC_ADDR_CS3	0xD0000000	/* Logical adress for CS3 */

/* #define CONFIG_ETHADDR
#define CONFIG_IPADDR
#define	CONFIG_NETMASK
#define CONFIG_SERVERIP
#define CFG_TFTP_LOADADDR */

/*
 * Ethernet PHY interface configuration
 */
/* definition of ethernet interface (Phy/Switch) - '1' means active*/
#define CFG_ETH_KS8xx1 0 /* Micrel Phy */
#define CFG_ETH_KS8893 1 /* Micrel Switch */
#define CFG_ETH_KS8993 0 /* Micrel Switch */ 



/* address of the phy on MII interface, normally (but not necessarily) 0 
 * When using the switch (KS8893), the address has to be 4 (see smi mode as
 * refernce), because the switch does not support addresses in smi mode,
 * nevertheless the third address bit has to be set
 */
#define CONFIG_PHY_ADDR	0

/* used to reset phy/switch with PC15 after initialization*/
#define BOARD_LATE_INIT 1

/*
 * FLASH Device configuration
 */
//#define DEBUG 3
#define PHYS_FLASH_1			0x10000000
#define PHYS_FLASH_SIZE			0x01000000  /* 16 megs main flash */
#define CFG_FLASH_BASE			PHYS_FLASH_1
#define CFG_FLASH_CFI		1	/* flash is CFI conformant	*/
#define CFG_FLASH_CFI_DRIVER	1	/* use common cfi driver	*/
#define CFG_FLASH_EMPTY_INFO
//#define CFG_FLASH_USE_BUFFER_WRITE 1	/* use buffered writes (20x faster) */
#define CFG_MAX_FLASH_BANKS	1	/* max # of memory banks	*/
#define CFG_FLASH_INCREMENT	0	/* there is only one bank	*/
#define CFG_FLASH_CFI_WIDTH FLASH_CFI_16BIT

//#define CFG_FLASH_PROTECTION	1	/* hardware flash protection	*/
/*normally 128, but cfi_flash() function on line 1150 is buggy*/
#define CFG_MAX_FLASH_SECT		256

#define CFG_JFFS2_FIRST_BANK	0
#define CFG_JFFS2_FIRST_SECTOR	3
#define CFG_JFFS2_NUM_BANKS	1

#undef	CFG_ENV_IS_IN_DATAFLASH

#ifdef CFG_ENV_IS_IN_DATAFLASH
#define CFG_ENV_OFFSET			0x20000
#define CFG_ENV_ADDR			(CFG_DATAFLASH_LOGIC_ADDR_CS0 + CFG_ENV_OFFSET)
#define CFG_ENV_SIZE			0x2000  /* 0x8000 */
#else
#define CFG_ENV_IS_IN_FLASH		1
#define CFG_ENV_ADDR			(PHYS_FLASH_1 + 0x20000)  /* after u-boot.bin */
#define CFG_ENV_SIZE			0x20000 /* sectors are 128K here */
#endif	/* CFG_ENV_IS_IN_DATAFLASH */


#define CFG_LOAD_ADDR		0x21000000  /* default load address */

#define CFG_BAUDRATE_TABLE	{115200, 57600, 38400, 19200, 9600 }

#define CFG_PROMPT		"U-Boot> "	/* Monitor Command Prompt */
#define CFG_CBSIZE		256		/* Console I/O Buffer Size */
#define CFG_MAXARGS		16		/* max number of command args */
#define CFG_PBSIZE		(CFG_CBSIZE+sizeof(CFG_PROMPT)+16) /* Print Buffer Size */

#ifndef __ASSEMBLY__
/*-----------------------------------------------------------------------
 * Board specific extension for bd_info
 *
 * This structure is embedded in the global bd_info (bd_t) structure
 * and can be used by the board specific code (eg board/...)
 */

struct bd_info_ext {
	/* helper variable for board environment handling
	 *
	 * env_crc_valid == 0    =>   uninitialised
	 * env_crc_valid  > 0    =>   environment crc in flash is valid
	 * env_crc_valid  < 0    =>   environment crc in flash is invalid
	 */
	int env_crc_valid;
};
#endif

#define CFG_HZ 1000
#define CFG_HZ_CLOCK AT91C_MASTER_CLOCK/2	/* AT91C_TC0_CMR is implicitly set to */
					/* AT91C_TC_TIMER_DIV1_CLOCK */

#define CONFIG_STACKSIZE	(32*1024)	/* regular stack */

#ifdef CONFIG_USE_IRQ
#error CONFIG_USE_IRQ not supported
#endif

#endif
