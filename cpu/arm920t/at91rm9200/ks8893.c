/*
 * Driver for Micrel's KS8893 ethernet switch. The configuration of the switch
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
#include <ks8893.h>

#ifdef CONFIG_DRIVER_ETHER

#if (CONFIG_COMMANDS & CFG_CMD_NET)

/*
 * Name:
 *	ks8893_SmiReadPhy
 * Description:ETHER_MD_CA_ADDR_MSK
 *	Reads data from the PHY register
 * Arguments:
 *	dev - pointer to struct net_device
 *	RegisterAddress - unsigned char
 * 	pInput - pointer to value read from register
 * Return value:
 *	TRUE - if data read successfully
 */
UCHAR ks8893_SmiReadPhy (AT91PS_EMAC p_mac,
			unsigned char regadd,
			unsigned short *pInput)
{
	p_mac->EMAC_MAN = (AT91C_EMAC_HIGH & ~AT91C_EMAC_LOW) |
		(!AT91C_EMAC_RW) |
		(((((regadd >> 5) & 0x7) | 0x10) << 23) & AT91C_EMAC_PHYA) |
		((regadd << 18) & AT91C_EMAC_REGA) |
		(AT91C_EMAC_CODE_802_3) | 
		0xFFFF;
	udelay (10000);
	*pInput = (unsigned short) p_mac->EMAC_MAN;
	return TRUE;
}


/*
 * Name:
 *	ks8893_SmiWritePhy
 * Description:
 *	Writes data to the PHY register
 * Arguments:
 *	dev - pointer to struct net_device
 *	RegisterAddress - unsigned char
 * 	pOutput - pointer to value to be written in the register
 * Return value:
 *	TRUE - if data read successfully
 */
UCHAR ks8893_SmiWritePhy (AT91PS_EMAC p_mac,
			unsigned char regadd,
			unsigned short *pOutput)
{
	p_mac->EMAC_MAN = (AT91C_EMAC_HIGH & ~AT91C_EMAC_LOW) |
			(!AT91C_EMAC_RW) |
			((((regadd >> 5) & 0x7) << 23) & AT91C_EMAC_PHYA) |
			((regadd << 18) & AT91C_EMAC_REGA) |
			(AT91C_EMAC_CODE_802_3) | 
			*pOutput;
	udelay (10000);
	return TRUE;
}

/*
 * Name:
 *	ks8893_IsPhyConnected
 * Description:
 *	Reads the 2 PHY ID registers and calculates the OUI
 * Arguments:
 *	p_mac - pointer to AT91S_EMAC struct
 * Return value:
 *	TRUE - if id read successfully
 *	FALSE- if error
 */
unsigned int ks8893_IsPhyConnected (AT91PS_EMAC p_mac)
{
	unsigned short id;
	
	at91rm9200_EmacEnableMDIO (p_mac);
	ks8893_SmiReadPhy (p_mac, KS8893_CPID0_REG, &id);
	at91rm9200_EmacDisableMDIO (p_mac);
	
	if (id == KS8893_CHIPID0) {
		return TRUE;
	}
	return FALSE;
}

/*
 * Name:
 *	ks8893_GetLinkSpeed
 * Description:
 *	Link parallel detection status of MAC is checked for both switch
 *	ports 
 * Arguments:
 *	p_mac - pointer to MAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
UCHAR ks8893_GetLinkSpeed (AT91PS_EMAC p_mac)
{
	unsigned short p1stat0 = 0x00aa;
	unsigned short p1stat1 = 0x00aa;
	unsigned short p2stat0  = 0x00aa; 
	unsigned short p2stat1  = 0x00aa;
	unsigned short tmp;

	/* check if there is a link on port 1 or port 2*/
	if (!ks8893_SmiReadPhy (p_mac, KS8893_PORT1_STAT0_REG, &p1stat0)) {
		return FALSE;
	}
	
	if (!ks8893_SmiReadPhy (p_mac, KS8893_PORT2_STAT0_REG, &p2stat0)) {
		return FALSE;
	}
	
	debug ("Port1: STAT0 is 0x%x\n", p1stat0);
	debug ("Port2: STAT0 is 0x%x\n", p2stat0);
	
	/* if there is no link on both ports */
	if (!(p1stat0 & KS8893_LNK_UP) && !(p2stat0 & KS8893_LNK_UP)) {
		printf ("No Link on Port 1 and Port 2\n");
		return FALSE;
	}
	
	/* check operation speed on both ports */ 
	if (!ks8893_SmiReadPhy (p_mac, KS8893_PORT1_STAT1_REG, &p1stat1)) {
		return FALSE;
	}
	
	if (!ks8893_SmiReadPhy (p_mac, KS8893_PORT2_STAT1_REG, &p2stat1)) {
		return FALSE;
	}
	
	if ((p1stat0 & KS8893_LNK_UP)) {
		if ((p1stat1 & KS8893_LNK_SPEED) &&
			(p1stat1 & KS8893_LNK_DUPLEX)) {
			debug ("KS8893 Port 1: 100 MBit, Full Duplex\n");
		} else if ((p1stat1 & KS8893_LNK_SPEED) && 
			!(p1stat1 & KS8893_LNK_DUPLEX)) {
			debug ("KS8893 Port 1: 100 MBit, Half Duplex\n");
		} else if (!(p1stat1 & KS8893_LNK_SPEED) && 
			(p1stat1 & KS8893_LNK_DUPLEX)) {
			debug ("KS8893 Port 1: 10 MBit, Full Duplex\n");
		} else {
			debug ("KS8893 Port 1: 10 MBit, Half Duplex\n");
		}
	} else {
		debug ("KS8893 Port 1: No Link\n");
	}

	if ((p2stat0 & KS8893_LNK_UP)) {
		if ((p2stat1 & KS8893_LNK_SPEED) &&
			(p2stat1 & KS8893_LNK_DUPLEX)) {
			debug ("KS8893 Port 2: 100 MBit, Full Duplex\n");
		} else if ((p2stat1 & KS8893_LNK_SPEED) && 
			!(p2stat1 & KS8893_LNK_DUPLEX)) {
			debug ("KS8893 Port 2: 100 MBit, Half Duplex\n");
		} else if (!(p2stat1 & KS8893_LNK_SPEED) && 
			(p2stat1 & KS8893_LNK_DUPLEX)) {
			debug ("KS8893 Port 2: 10 MBit, Full Duplex\n");
		} else {
			debug ("KS8893 Port 2: 10 MBit, Half Duplex\n");
		}
	} else {
		debug ("KS8893 Port 2: No Link\n");
	}
		
	/* check the speed of the mii-interface to the switch (3-port) 
	 * Speed on port 3 only depends on power up settings, not on link
	 * partner
	 */
	if (!ks8893_SmiReadPhy (p_mac, KS8893_GLB_CTL4_REG, &tmp)) {
		return FALSE;
	}
	
	if (!(tmp & KS8893_MII_10BT) && !(tmp & KS8893_MII_HD)) {
		/*set Emac for 100BaseTX and Full Duplex  */
		debug ("Port 3: 100 MBit, Full Duplex\n");
		p_mac->EMAC_CFG |= AT91C_EMAC_SPD | AT91C_EMAC_FD;
		return TRUE;
	}
	if (!(tmp & KS8893_MII_10BT) && (tmp & KS8893_MII_HD)) {
		/* set MII for 100BaseTX and Half Duplex */
		debug ("Port 3: 100 MBit, Half Duplex\n");
		p_mac->EMAC_CFG = (p_mac->EMAC_CFG & ~(AT91C_EMAC_SPD |
			AT91C_EMAC_FD)) | AT91C_EMAC_SPD;
		return TRUE;
	}
	if ((tmp & KS8893_MII_10BT) && !(tmp & KS8893_MII_HD)) {
		/* set MII for 10BaseT and Full Duplex */
		debug ("Port 3: 10 MBit, Full Duplex\n");
		p_mac->EMAC_CFG = (p_mac->EMAC_CFG & ~(AT91C_EMAC_SPD |
			AT91C_EMAC_FD)) | AT91C_EMAC_FD;
		return TRUE;
	}
	if ((tmp & KS8893_MII_10BT) && (tmp & KS8893_MII_HD)) {
		/* set MII for 10BaseT and Half Duplex */
		debug ("Port 3: 10 MBit, Half Duplex\n");
		p_mac->EMAC_CFG &= ~(AT91C_EMAC_SPD | AT91C_EMAC_FD);
		return TRUE;
	}
	return FALSE;
}

/*
 * Name:
 *	ks8893_InitPhy
 * Description:
 *	MAC starts checking its link by using parallel detection and
 *	Autonegotiation
 * Arguments:
 *	p_mac - pointer to struct AT91S_EMAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
UCHAR ks8893_InitPhy (AT91PS_EMAC p_mac)
{
	UCHAR ret = TRUE;
	unsigned short tmp;
	
	at91rm9200_EmacEnableMDIO (p_mac);
	
	if (!ks8893_SmiReadPhy (p_mac, KS8893_CPID1_REG, &tmp)) {
		return FALSE;
	}
	/* turn on switch */ 
	tmp |= KS8893_START_SW;
	if (!ks8893_SmiWritePhy (p_mac, KS8893_CPID1_REG, &tmp)) {
		return FALSE;
	}
	
	if (!ks8893_GetLinkSpeed (p_mac)) {
		/* Try another time */
		ret = ks8893_GetLinkSpeed (p_mac);
	}
	at91rm9200_EmacDisableMDIO (p_mac);
	return (ret);
}

#endif	/* CONFIG_COMMANDS & CFG_CMD_NET */
#endif	/* CONFIG_DRIVER_ETHER */
