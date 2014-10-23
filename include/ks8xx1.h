/*
 * Micrel KS8xx1 Ethernet PHY
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

/* Micrel PHYSICAL LAYER TRANSCEIVER KS8xx1 */

#define KS8xx1_BCR 	0	/* Basic Control Register */
#define KS8xx1_BSR	1	/* Basic Status Register */
#define KS8xx1_PHYID1	2	/* PHY Idendifier Register 1 */
#define KS8xx1_PHYID2	3	/* PHY Idendifier Register 2 */
#define KS8xx1_ANAR	4	/* Auto_Negotiation Advertisement Register */
#define KS8xx1_ANLPAR	5	/* Auto_N. Link Partner Ability Register */
#define KS8xx1_ANER	6	/* Auto-N. Expansion Register */
#define KS8xx1_ANNPR	7	/* Auto-N. Next Page Register */
#define KS8xx1_LPNPA	8	/* Link Partner Next Page Ability */
#define KS8xx1_RXERCR	21	/* RXER Counter Register */
#define KS8xx1_ICSR	27	/* Interrupt Control/Status Register */
#define KS8xx1_TXPCR	31	/* 100BASE-TX PHY Control Register */

/* --Bit definitions: KS8xx1_BCR */
#define KS8xx1_RESET		(1 << 15)
#define KS8xx1_LOOPBACK		(1 << 14)
#define KS8xx1_SPEED_SELECT	(1 << 13)
#define KS8xx1_AUTONEG		(1 << 12)
#define KS8xx1_POWER_DOWN	(1 << 11)
#define KS8xx1_ISOLATE		(1 << 10)
#define KS8xx1_RESTART_AUTONEG	(1 << 9)
#define KS8xx1_DUPLEX_MODE	(1 << 8)
#define KS8xx1_COLLISION_TEST	(1 << 7)
#define KS8xx1_DIS_TX		(1 << 0)

/*--Bit definitions: KS8xx1_BSR */
#define KS8xx1_100BASE_T4        (1 << 15)
#define KS8xx1_100BASE_TX_FD     (1 << 14)
#define KS8xx1_100BASE_TX_HD     (1 << 13)
#define KS8xx1_10BASE_T_FD       (1 << 12)
#define KS8xx1_10BASE_T_HD       (1 << 11)
#define KS8xx1_MF_PREAMB_SUPPR   (1 << 6)
#define KS8xx1_AUTONEG_COMP      (1 << 5)
#define KS8xx1_REMOTE_FAULT      (1 << 4)
#define KS8xx1_AUTONEG_ABILITY   (1 << 3)
#define KS8xx1_LINK_STATUS       (1 << 2)
#define KS8xx1_JABBER_DETECT     (1 << 1)
#define KS8xx1_EXTEND_CAPAB      (1 << 0)

/*--definitions: KS8xx1_PHYID1 and  KS8xx1_PHYID2*/
#define KS8xx1_OUI		0x0010A1

/*--Bit definitions: KS8xx1_ANAR, KS8xx1_ANLPAR */
#define KS8xx1_NP               (1 << 15)
#define KS8xx1_ACK              (1 << 14)
#define KS8xx1_RF               (1 << 13)
#define KS8xx1_PAUSE_11		(1 << 11)
#define KS8xx1_PAUSE_10		(1 << 10)
#define KS8xx1_T4               (1 << 9)
#define KS8xx1_TX_FDX           (1 << 8)
#define KS8xx1_TX_HDX           (1 << 7)
#define KS8xx1_10_FDX           (1 << 6)
#define KS8xx1_10_HDX           (1 << 5)
#define KS8xx1_AN_IEEE_802_3	0x01

/*--Bit definitions: KS8xx1_ANER */
#define KS8xx1_PDF              (1 << 4)
#define KS8xx1_LP_NP_ABLE       (1 << 3)
#define KS8xx1_NP_ABLE          (1 << 2)
#define KS8xx1_PAGE_RX          (1 << 1)
#define KS8xx1_LP_AN_ABLE       (1 << 0)


/* --Bit definitions: KS8xx1_ICSR */
#define KS8xx1_JAB_MASK		(1 << 15)
#define KS8xx1_RXERR_MASK	(1 << 14)
#define KS8xx1_PAREC_MASK	(1 << 13)
#define KS8xx1_PDF_MASK		(1 << 12)
#define KS8xx1_LKPAK_MASK	(1 << 11)
#define KS8xx1_LKDWN_MASK	(1 << 10)
#define KS8xx1_REFLT_MASK	(1 << 9)
#define KS8xx1_LKUP_MASK	(1 << 8)
#define KS8xx1_JAB_ISTAT	(1 << 7)
#define KS8xx1_RXERR_ISTAT	(1 << 6)
#define KS8xx1_PAREC_ISTAT	(1 << 5)
#define KS8xx1_PDF_ISTAT	(1 << 4)
#define KS8xx1_LKPAK_ISTAT	(1 << 3)
#define KS8xx1_LKDWN_ISTAT	(1 << 2)
#define KS8xx1_REFLT_ISTAT	(1 << 1)
#define KS8xx1_LKUP_ISTAT	(1 << 0)

/******************  function prototypes **********************/
unsigned int  ks8xx1_IsPhyConnected(AT91PS_EMAC p_mac);
unsigned char ks8xx1_GetLinkSpeed(AT91PS_EMAC p_mac);
unsigned char ks8xx1_AutoNegotiate(AT91PS_EMAC p_mac, int *status);
unsigned char ks8xx1_InitPhy(AT91PS_EMAC p_mac);
