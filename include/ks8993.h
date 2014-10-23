/*
 * Micrel KS8993 Ethernet Switch
 *
 * (C) Copyright 2006
 * Authors :
 *	Udo Jakobza (jakobza@ftz-leipzig.de)
 *	Mirco Fuchs (fuchs@ftz-leipzig.de)
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
 
#ifndef __KS8993_H_
#define __KS8993_H_
 
/* Micrel Ethernet Switch KS8993 Global Registers (Extended via SMI) */
#define KS8993_CPID0_REG	0 	/* Chip ID0 register */
#define KS8993_CPID1_REG	1	/* Chip ID1 / Start Switch register */
#define KS8993_GLB_CTL4_REG	6

/* Micrel Ethernet Switch KS8993 Port1 Registers (Extended via SMI) */
#define KS8993_PORT1_CTL0_REG	16
#define KS8993_PORT1_CTL1_REG	17
#define KS8993_PORT1_CTL2_REG	18
#define KS8993_PORT1_CTL3_REG	19
#define KS8993_PORT1_CTL4_REG	20
#define KS8993_PORT1_CTL5_REG	21
#define KS8993_PORT1_CTL6_REG	22
#define KS8993_PORT1_CTL7_REG	23
#define KS8993_PORT1_CTL8_REG	24
#define KS8993_PORT1_CTL9_REG	25
#define KS8993_PORT1_CTL10_REG	26
#define KS8993_PORT1_CTL11_REG	27
#define KS8993_PORT1_CTL12_REG	28
#define KS8993_PORT1_CTL13_REG	29
#define KS8993_PORT1_STAT0_REG	30
#define KS8993_PORT1_STAT1_REG	31

/* Micrel Ethernet Switch KS8993 Port2 Registers (Extended via SMI) */
#define KS8993_PORT2_CTL0_REG	32
#define KS8993_PORT2_CTL1_REG	33
#define KS8993_PORT2_CTL2_REG	34
#define KS8993_PORT2_CTL3_REG	35
#define KS8993_PORT2_CTL4_REG	36
#define KS8993_PORT2_CTL5_REG	37
#define KS8993_PORT2_CTL6_REG	38
#define KS8993_PORT2_CTL7_REG	39
#define KS8993_PORT2_CTL8_REG	40
#define KS8993_PORT2_CTL9_REG	41
#define KS8993_PORT2_CTL10_REG	42
#define KS8993_PORT2_CTL11_REG	43
#define KS8993_PORT2_CTL12_REG	44
#define KS8993_PORT2_CTL13_REG	45
#define KS8993_PORT2_STAT0_REG	46
#define KS8993_PORT2_STAT1_REG	47

/* Micrel Ethernet Switch KS8993 Port2 Registers (Extended via SMI) */
#define KS8993_PORT3_CTL0_REG	48
#define KS8993_PORT3_CTL1_REG	49
#define KS8993_PORT3_CTL2_REG	50
#define KS8993_PORT3_CTL3_REG	51
#define KS8993_PORT3_CTL4_REG	52
#define KS8993_PORT3_CTL5_REG	53
#define KS8993_PORT3_CTL6_REG	54
#define KS8993_PORT3_CTL7_REG	55
#define KS8993_PORT3_CTL8_REG	56
#define KS8993_PORT3_CTL9_REG	57
#define KS8993_PORT3_CTL10_REG	58
#define KS8993_PORT3_CTL11_REG	59
#define KS8993_PORT3_STAT1_REG	63

/*--definitions: KS8993_CPID0_REG */
#define KS8993_CHIPID0	0x93	/* bits 0-1 from Chip Id */

/*--definitions: KS8993_CPID0_REG */
#define KS8993_CHIPID1	0x0 << 4 /* bits 2-3 from Chip Id */
#define KS8993_REVID	0x0 << 3 /* not defined */
#define KS8993_START_SW	0x1 << 0 /* start chip */

/*--definitions: KS8993_GLB_CTL4_REG */
#define KS8993_MII_HD		1 << 6 /* MII interface is HD when set */
#define KS8993_MII_FLWCTL_EN	1 << 5 /* MII i. flow ctl. enable when set */
#define KS8993_MII_10BT		1 << 4 /* MII i. is in 10 Mbps mode when set */
#define KS8993_NULL_VID		1 << 3 /* replace NULL VID with port VID */
#define KS8993_BCAST_SP_10_8	0      /* bit 10:8 of broadcast strom prot. */ 

/*--definitions: KS8993_PORT_CTL12_REG */
#define KS8993_ANEG_EN		1 << 7 /* if set, auto-negotiation is on */
#define KS8993_FCE_SPEED	1 << 6 /* select speed if AN is disabled */
#define KS8993_FCE_DUPLEX	1 << 5 /* select duplex if AN is disabled */
#define KS8993_ADV_FLWCTL	1 << 4 /* flow control capability (for AN) */
#define KS8993_ADV_100_FD	1 << 3 /* 100 MBit full duplex cap. (for AN) */
#define KS8993_ADV_100_HD	1 << 2 /* 100 MBit half duplex cap. (for AN) */
#define KS8993_ADV_10_FD	1 << 1 /* 10 MBit full duplex cap. (for AN) */
#define KS8993_ADV_10_HD	1 << 0 /* 100 MBit full duplex cap. (for AN) */

/*--definitions: KS8993_PORT_CTL13_REG */
#define KS8993_LED_OFF		1 << 7 /* turns off all port's LEDs */
#define KS8993_TX_DIS		1 << 6 /* disables transmitter when set */
#define KS8993_RESTART_AN	1 << 5 /* restart auto-negotiation */
#define KS8993_FAR_END_FLT_DIS	1 << 4 /* disables far end fault when set */
#define KS8993_PWR_DOWN		1 << 3 /* power down when set */
#define KS8993_AUTO_MDIX_DIS	1 << 2 /* disables auto MDI-X function */
#define KS8993_FCE_MDIX		1 << 1 /* force MDI or MDI-X*/
#define KS8993_LOOPBACK		1 << 0 /* enable loopback */

/*--definitions: KS8993_PORT_STAT0_REG */
#define KS8993_MDIX		1 << 7 /* MDI-X is enabled when set */
#define KS8993_AN_RDY 		1 << 6 /* Auto-negotiation ready */
#define KS8993_LNK_UP		1 << 5 /* Link-Up */
#define KS8993_PART_FLWCTL	1 << 4 /* partner flow control cabability */
#define KS8993_PART_100_FD	1 << 3 /* partner 100 MBit full duplex cap. */ 
#define KS8993_PART_100_HD	1 << 2 /* partner 100 MBit half duplex cap. */ 
#define KS8993_PART_10_FD	1 << 1 /* partner 10 MBit full duplex cap. */ 
#define KS8993_PART_10_HD	1 << 0 /* partner 10 MBit half duplex cap. */ 

/*--definitions: KS8993_PORT_STAT1_REG */
#define KS8993_RX_FLWCTL_EN	1 << 4 /* receive flow control */
#define KS8993_TX_FLWCTL_EN	1 << 3 /* transmit flow control */
#define KS8993_LNK_SPEED	1 << 2 /* operation speed (1 = 100 MBit) */
#define KS8993_LNK_DUPLEX	1 << 1 /* operation duplex (1 = full duplex) */
#define KS8993_FAR_END_FAULT	1 << 0 /* far end fault status (only port 1) */

/******************  function prototypes **********************/
unsigned int  ks8993_IsPhyConnected(AT91PS_EMAC p_mac);
unsigned char ks8993_GetLinkSpeed(AT91PS_EMAC p_mac);
unsigned char ks8993_AutoNegotiate(AT91PS_EMAC p_mac, int *status);
unsigned char ks8993_InitPhy(AT91PS_EMAC p_mac);

#endif /* __KS8993_H_ */ 
