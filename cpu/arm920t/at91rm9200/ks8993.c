/*
 * Driver for Micrel's KS8993 ethernet switch. The configuration of the switch
 * uses the Serial Management Interface (SMI), modification of the
 * MII-Management Interface.
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
 
#include <at91rm9200_net.h>
#include <net.h>
#include <ks8993.h>

#ifdef CONFIG_DRIVER_ETHER

#if (CONFIG_COMMANDS & CFG_CMD_NET)

/*
 * Name:
 *	ks8993_GetPhyAddresVal
 * Description:
 *	calculates the value for the pyh address lines to access smi-regs
 * Arguments:
 *	p_mac - pointer to AT91S_EMAC struct
 * Return value:
 *	TRUE - if id read successfully
 *	FALSE- if error
 */
unsigned char ks8993_GetPhyAddressVal (unsigned char smi_reg)
{
	unsigned short reg;
	
	reg = smi_reg;
	
	reg = reg >> 2;
	reg &= 0x0018;
	reg |= 0x0004;
	return reg;
}
 
/*
 * Name:
 *	ks8993_GetRegAddressVal
 * Description:
 *	
 * Arguments:
 *	p_mac - pointer to AT91S_EMAC struct
 * Return value:
 *	TRUE - if id read successfully
 *	FALSE- if error
 */
unsigned char ks8993_GetRegAddressVal (unsigned char smi_reg)
{
	return (smi_reg & 0x1f);
}

/*
 * Name:
 *	ks8993_IsPhyConnected
 * Description:
 *	Reads the 2 PHY ID registers and calculates the OUI
 * Arguments:
 *	p_mac - pointer to AT91S_EMAC struct
 * Return value:
 *	TRUE - if id read successfully
 *	FALSE- if error
 */
unsigned int ks8993_IsPhyConnected (AT91PS_EMAC p_mac)
{
	unsigned short id;
	
	at91rm9200_EmacEnableMDIO (p_mac);
	at91rm9200_EmacReadPhy (p_mac,
		ks8993_GetPhyAddressVal(KS8993_CPID0_REG),
		ks8993_GetRegAddressVal(KS8993_CPID0_REG),
		&id);
	at91rm9200_EmacDisableMDIO (p_mac);
	
	if (id == KS8993_CHIPID0) {
		return TRUE;
	}
	return FALSE;
}

/*
 * Name:
 *	ks8993_GetLinkSpeed
 * Description:
 *	Link parallel detection status of MAC is checked for both switch
 *	ports 
 * Arguments:
 *	p_mac - pointer to MAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
UCHAR ks8993_GetLinkSpeed (AT91PS_EMAC p_mac)
{
	unsigned short p1stat0 = 0x00aa;
	unsigned short p1stat1 = 0x00aa;
	unsigned short p2stat0  = 0x00aa; 
	unsigned short p2stat1  = 0x00aa;
	unsigned short tmp;

	/* check if there is a link on port 1 or port 2*/
	if (!at91rm9200_EmacReadPhy (p_mac,
		ks8993_GetPhyAddressVal(KS8993_PORT1_STAT0_REG),
		ks8993_GetRegAddressVal(KS8993_PORT1_STAT0_REG),
		&p1stat0)) {
		return FALSE;
	}
	
	if (!at91rm9200_EmacReadPhy (p_mac,
		ks8993_GetPhyAddressVal(KS8993_PORT2_STAT0_REG),
		ks8993_GetRegAddressVal(KS8993_PORT2_STAT0_REG),
		&p2stat0)) {
		return FALSE;
	}
	
	debug ("Port1: STAT0 is 0x%x\n", p1stat0);
	debug ("Port2: STAT0 is 0x%x\n", p2stat0);
	
	/* if there is no link on both ports */
	if (!(p1stat0 & KS8993_LNK_UP) && !(p2stat0 & KS8993_LNK_UP)) {
		debug ("No Link on Port 1 and Port 2\n");
		return FALSE;
	}
	
	/* check operation speed on both ports */
	if (!at91rm9200_EmacReadPhy (p_mac,
		ks8993_GetPhyAddressVal(KS8993_PORT1_STAT1_REG),
		ks8993_GetRegAddressVal(KS8993_PORT1_STAT1_REG),
		&p1stat1)) {
		return FALSE;
	}
	
	if (!at91rm9200_EmacReadPhy (p_mac,
		ks8993_GetPhyAddressVal(KS8993_PORT2_STAT1_REG),
		ks8993_GetRegAddressVal(KS8993_PORT2_STAT1_REG),
		&p2stat1)) {
		return FALSE;
	}
	
	if ((p1stat0 & KS8993_LNK_UP)) {
		if ((p1stat1 & KS8993_LNK_SPEED) &&
			(p1stat1 & KS8993_LNK_DUPLEX)) {
			debug ("KS8993 Port 1: 100 MBit, Full Duplex\n");
		} else if ((p1stat1 & KS8993_LNK_SPEED) && 
			!(p1stat1 & KS8993_LNK_DUPLEX)) {
			debug ("KS8993 Port 1: 100 MBit, Half Duplex\n");
		} else if (!(p1stat1 & KS8993_LNK_SPEED) && 
			(p1stat1 & KS8993_LNK_DUPLEX)) {
			debug ("KS8993 Port 1: 10 MBit, Full Duplex\n");
		} else {
			debug ("KS8993 Port 1: 10 MBit, Half Duplex\n");
		}
	} else {
		debug ("KS8993 Port 1: No Link\n");
	}

	if ((p2stat0 & KS8993_LNK_UP)) {
		if ((p2stat1 & KS8993_LNK_SPEED) &&
			(p2stat1 & KS8993_LNK_DUPLEX)) {
			debug ("KS8993 Port 2: 100 MBit, Full Duplex\n");
		} else if ((p2stat1 & KS8993_LNK_SPEED) && 
			!(p2stat1 & KS8993_LNK_DUPLEX)) {
			debug ("KS8993 Port 2: 100 MBit, Half Duplex\n");
		} else if (!(p2stat1 & KS8993_LNK_SPEED) && 
			(p2stat1 & KS8993_LNK_DUPLEX)) {
			debug ("KS8993 Port 2: 10 MBit, Full Duplex\n");
		} else {
			debug ("KS8993 Port 2: 10 MBit, Half Duplex\n");
		}
	} else {
		debug ("KS8993 Port 2: No Link\n");
	}
		
	/* check the speed of the mii-interface to the switch (3-port) 
	 * Speed on port 3 only depends on power up settings, not on link
	 * partner
	 */
	if (!at91rm9200_EmacReadPhy (p_mac,
		ks8993_GetPhyAddressVal(KS8993_GLB_CTL4_REG),
		ks8993_GetRegAddressVal(KS8993_GLB_CTL4_REG),
		&tmp)) {
		return FALSE;
	}
	
	if (!(tmp & KS8993_MII_10BT) && !(tmp & KS8993_MII_HD)) {
		/*set Emac for 100BaseTX and Full Duplex  */
		debug ("Port 3: 100 MBit, Full Duplex\n");
		p_mac->EMAC_CFG |= AT91C_EMAC_SPD | AT91C_EMAC_FD;
		return TRUE;
	}
	if (!(tmp & KS8993_MII_10BT) && (tmp & KS8993_MII_HD)) {
		/* set MII for 100BaseTX and Half Duplex */
		debug ("Port 3:100 MBit, Half Duplex\n");
		p_mac->EMAC_CFG = (p_mac->EMAC_CFG & ~(AT91C_EMAC_SPD |
			AT91C_EMAC_FD)) | AT91C_EMAC_SPD;
		return TRUE;
	}
	if ((tmp & KS8993_MII_10BT) && !(tmp & KS8993_MII_HD)) {
		/* set MII for 10BaseT and Full Duplex */
		debug ("Port 3:10 MBit, Full Duplex\n");
		p_mac->EMAC_CFG = (p_mac->EMAC_CFG & ~(AT91C_EMAC_SPD |
			AT91C_EMAC_FD)) | AT91C_EMAC_FD;
		return TRUE;
	}
	if ((tmp & KS8993_MII_10BT) && (tmp & KS8993_MII_HD)) {
		/* set MII for 10BaseT and Half Duplex */
		debug ("Port 3:10 MBit, Half Duplex\n");
		p_mac->EMAC_CFG &= ~(AT91C_EMAC_SPD | AT91C_EMAC_FD);
		return TRUE;
	}
	return FALSE;
}

/*
 * Name:
 *	ks8993_InitPhy
 * Description:
 *	MAC starts checking its link by using parallel detection and
 *	Autonegotiation
 * Arguments:
 *	p_mac - pointer to struct AT91S_EMAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
UCHAR ks8993_InitPhy (AT91PS_EMAC p_mac)
{
	UCHAR ret = TRUE;
	unsigned short tmp;
	
	at91rm9200_EmacEnableMDIO (p_mac);
	
	if (!at91rm9200_EmacReadPhy (p_mac,
		ks8993_GetPhyAddressVal(KS8993_CPID1_REG),
		ks8993_GetRegAddressVal(KS8993_CPID1_REG),
		&tmp)) {
		return FALSE;
	}
	/* turn on switch */ 
	tmp |= KS8993_START_SW;
	if (!at91rm9200_EmacWritePhy (p_mac,
		ks8993_GetPhyAddressVal(KS8993_CPID1_REG),
		ks8993_GetRegAddressVal(KS8993_CPID1_REG),
		&tmp)) {
		return FALSE;
	}
	
	if (!ks8993_GetLinkSpeed (p_mac)) {
		/* Try another time */
		ret = ks8993_GetLinkSpeed (p_mac);
	}
	at91rm9200_EmacDisableMDIO (p_mac);
	return (ret);
}

#endif	/* CONFIG_COMMANDS & CFG_CMD_NET */
#endif	/* CONFIG_DRIVER_ETHER */
