/*
 * Driver for Micrel's KS8xx1 ethernet phy interface
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
#include <ks8xx1.h>

#ifdef CONFIG_DRIVER_ETHER

#if (CONFIG_COMMANDS & CFG_CMD_NET)

#ifndef CONFIG_PHY_ADDR
#define CONFIG_PHY_ADDR 0
#endif

/*
 * Name:
 *	ks8xx1_IsPhyConnected
 * Description:
 *	Reads the 2 PHY ID registers and calculates the OUI
 * Arguments:
 *	p_mac - pointer to AT91S_EMAC struct
 * Return value:
 *	TRUE - if id read successfully
 *	FALSE- if error
 */
unsigned int ks8xx1_IsPhyConnected (AT91PS_EMAC p_mac)
{
	unsigned short Id1, Id2;
	unsigned int oui = 0;
	unsigned int tmp_id = 0;
	unsigned int mask = 1;
	unsigned char i;
	
	at91rm9200_EmacEnableMDIO (p_mac);
	at91rm9200_EmacReadPhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_PHYID1, &Id1);
	at91rm9200_EmacReadPhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_PHYID2, &Id2);
	at91rm9200_EmacDisableMDIO (p_mac);

	/* the storage format of micrel's OUI is really strange*/
	tmp_id = Id1 << 6;
	tmp_id |= (Id2 >> 10);
	
	/* reverse bit order*/
	for (i = 24; i > 0; i--) {
		oui = oui << 1;
		if (tmp_id & mask) {
			oui |= 1;
		}
		mask = mask << 1;
	}
	
	/* arrange the nibbles to get OUI*/
	tmp_id = oui;
	oui  = ((tmp_id >> 16)  & 0x00000f );
	oui |= ((tmp_id >> 16)  & 0x0000f0 );
	oui |= ((tmp_id >>  0)  & 0x000f00 );
	oui |= ((tmp_id >>  0)  & 0x00f000 );
	oui |= ((tmp_id << 16)  & 0x0f0000 );
	oui |= ((tmp_id << 16)  & 0xf00000 );
	
	if (oui == KS8xx1_OUI) {
		return TRUE;
	}
	return FALSE;
}

/*
 * Name:
 *	ks8xx1_GetLinkSpeed
 * Description:
 *	Link parallel detection status of MAC is checked and set in the
 *	MAC configuration registers
 * Arguments:
 *	p_mac - pointer to MAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
UCHAR ks8xx1_GetLinkSpeed (AT91PS_EMAC p_mac)
{
	unsigned short stat0, stat1, stat2;
	
	if (!at91rm9200_EmacReadPhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_BCR,
		&stat0)) {
		return FALSE;
	}
	
	if (!at91rm9200_EmacReadPhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_BSR,
		&stat1)) {
		return FALSE;
	}
	
	/* link status up? */
	if (!(stat1 & KS8xx1_LINK_STATUS)) {
		printf ("No Link\n");
		return FALSE;
	}
	
	if (!at91rm9200_EmacReadPhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_ANLPAR,
		&stat2)) {
		return FALSE;
	}
	
	if (!(stat0 & KS8xx1_AUTONEG)) {
		debug ("Auto-negotiation has to be enabled");
		return FALSE;
	}
	
	if (!(stat1 & KS8xx1_AUTONEG_COMP)) {
		debug ("Auto-negotiation still in progress");
		return FALSE;
	}
	
	if ((stat1 & KS8xx1_100BASE_TX_FD) && (stat2 & KS8xx1_TX_FDX)) {
		/*set Emac for 100BaseTX and Full Duplex  */
		debug ("100 MBit, Full Duplex\n");
		p_mac->EMAC_CFG |= AT91C_EMAC_SPD | AT91C_EMAC_FD;
		return TRUE;
	}
	if ((stat1 & KS8xx1_10BASE_T_FD) && (stat2 & KS8xx1_10_FDX)) {
		/*set MII for 10BaseT and Full Duplex */
		debug ("10 MBit, Full Duplex\n");
		p_mac->EMAC_CFG = (p_mac->EMAC_CFG & ~(AT91C_EMAC_SPD |
			AT91C_EMAC_FD)) | AT91C_EMAC_FD;
		return TRUE;
	}
	if ((stat1 & KS8xx1_100BASE_TX_HD) && (stat2 & KS8xx1_TX_HDX)) {
		/* set MII for 100BaseTX and Half Duplex */
		debug ("100 MBit, Half Duplex\n");
		p_mac->EMAC_CFG = (p_mac->EMAC_CFG & ~(AT91C_EMAC_SPD |
			AT91C_EMAC_FD)) | AT91C_EMAC_SPD;
		return TRUE;
	}
	if ((stat1 & KS8xx1_10BASE_T_FD) && (stat2 & KS8xx1_10_HDX)) {
		/* set MII for 10BaseT and Half Duplex */
		debug ("10 MBit, Half Duplex\n");
		p_mac->EMAC_CFG &= ~(AT91C_EMAC_SPD | AT91C_EMAC_FD);
		return TRUE;
	}
	return FALSE;
}

/*
}
 * Name:
 *	ks8xx1_InitPhy
 * Description:
 *	MAC starts checking its link by using parallel detection and
 *	Autonegotiation and the same is set in the MAC configuration registers
 * Arguments:
 *	p_mac - pointer to struct AT91S_EMAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
UCHAR ks8xx1_InitPhy (AT91PS_EMAC p_mac)
{
	UCHAR ret = TRUE;
	unsigned short IntValue = 0;
	
	at91rm9200_EmacEnableMDIO (p_mac);
	
	if (!ks8xx1_GetLinkSpeed (p_mac)) {
		/* Try another time */
		ret = ks8xx1_GetLinkSpeed (p_mac);
	}
	/* Disable PHY Interrupts */
	IntValue &= ~(KS8xx1_JAB_MASK | KS8xx1_RXERR_MASK | KS8xx1_PAREC_MASK |
		KS8xx1_PDF_MASK | KS8xx1_LKPAK_MASK | KS8xx1_LKDWN_MASK |
		KS8xx1_REFLT_MASK | KS8xx1_LKUP_MASK);
	at91rm9200_EmacWritePhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_ICSR,
		&IntValue);
	at91rm9200_EmacDisableMDIO (p_mac);
	return (ret);
}

/*
 * Name:
 *	ks8xx1_AutoNegotiate
 * Description:
 *	MAC Autonegotiates with the partner status of same is set in the
 *	MAC configuration registers
 * Arguments:
 *	dev - pointer to struct net_device
 * Return value:
 *	TRUE - if link status set successfully
 *	FALSE - if link status not set
 */
UCHAR ks8xx1_AutoNegotiate (AT91PS_EMAC p_mac, int *status)
{
	unsigned short value;
	unsigned short PhyAnar;
	unsigned short PhyAnalpar;
	
	/* Set ks8xx1 control register */
	if (!at91rm9200_EmacReadPhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_BCR,
		&value)) {
		return FALSE;
	}
	
	/* disable auto-negotiation process */
	value &= ~KS8xx1_AUTONEG;
	
	/* Electrically isolate PHY */
	value |= KS8xx1_ISOLATE;
	
	if (!at91rm9200_EmacWritePhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_BCR,
		&value)) {
		return FALSE;
	}
	
	/* Set the Auto_negotiation Advertisement Register
	 * MII advertising for Next page, 100BaseTxFD and HD, 10BaseTFD and HD,
	 * IEEE 802.3
	 */
	PhyAnar = KS8xx1_NP | KS8xx1_TX_FDX | KS8xx1_TX_HDX | KS8xx1_10_FDX |
		KS8xx1_10_HDX | KS8xx1_AN_IEEE_802_3;
	
	if (!at91rm9200_EmacWritePhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_ANAR,
		&PhyAnar)) {
		return FALSE;
	}
	
	/* Read the Control Register */
	if (!at91rm9200_EmacReadPhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_BCR,
		&value)) {
		return FALSE;
	}
	
	value |= KS8xx1_SPEED_SELECT | KS8xx1_AUTONEG | KS8xx1_DUPLEX_MODE;
	
	if (!at91rm9200_EmacWritePhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_BCR,
		&value)) {
		return FALSE;
	}
	
	/* Restart Auto_negotiation */
	value |= KS8xx1_RESTART_AUTONEG;
	value &= ~KS8xx1_ISOLATE;
	
	if (!at91rm9200_EmacWritePhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_BCR,
		&value)) {
		return FALSE;
	}
	/* check AutoNegotiate complete */
	udelay (10000);
	
	at91rm9200_EmacReadPhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_BSR, &value);
	if (!(value & KS8xx1_AUTONEG_COMP)) {
		return FALSE;
	}
	
	/* Get the AutoNeg Link partner base page */
	if (!at91rm9200_EmacReadPhy (p_mac, CONFIG_PHY_ADDR, KS8xx1_ANLPAR,
		&PhyAnalpar)) {
		return FALSE;
	}
	if ((PhyAnar & KS8xx1_TX_FDX) && (PhyAnalpar & KS8xx1_TX_FDX)) {
		/* set MII for 100BaseTX and Full Duplex */
		p_mac->EMAC_CFG |= AT91C_EMAC_SPD | AT91C_EMAC_FD;
		return TRUE;
	}
	if ((PhyAnar & KS8xx1_10_FDX) && (PhyAnalpar & KS8xx1_10_FDX)) {
		/* set MII for 10BaseT and Full Duplex */
		p_mac->EMAC_CFG = (p_mac->EMAC_CFG & ~(AT91C_EMAC_SPD |
			AT91C_EMAC_FD)) | AT91C_EMAC_FD;
		return TRUE;
	}
	return FALSE;
}

#endif	/* CONFIG_COMMANDS & CFG_CMD_NET */
#endif	/* CONFIG_DRIVER_ETHER */
