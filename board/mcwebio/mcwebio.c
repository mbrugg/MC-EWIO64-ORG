/*
 * (C) Copyright 2009 MC Technology <www.mc-technology.net>
 * Karl-Heinz Weber <KWeber@metz-connect.com>
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




#include <common.h>
#include <asm/arch/AT91RM9200.h>
#include <at91rm9200_net.h>


#if (CFG_ETH_KS8993)
#include <ks8993.h>
#elif (CFG_ETH_KS8893)
#include <ks8893.h>
#else /* use CFG_ETH_KS8xx1 */
#include <ks8xx1.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#define AT91C_PIO_PC8 ((unsigned int) 1 << 8)

/* ------------------------------------------------------------------------- */
/*
 * Miscelaneous platform dependent initialisations
 */

int board_init (void)
{
	/* Enable Ctrlc */
	console_init_f ();

	/* memory and cpu-speed are setup before relocation */
	/* so we do _nothing_ here */

	/* arch number of MC-WEBIO-ARM-Board */
	gd->bd->bi_arch_number = MACH_TYPE_MCWEBIO;
	/* adress of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;
	
	return 0;
}

#define AT91C_PIO_PC15 ((unsigned int) 1 << 15)



int board_late_init (void)
{
	/* Enables PIO control of the pin */
	AT91C_BASE_PIOC->PIO_PER = AT91C_PIO_PC15;
	/* Set Output Data Register of the pin */
	AT91C_BASE_PIOC->PIO_SODR = AT91C_PIO_PC15;
	/* Set Output Enable of the pin */
	AT91C_BASE_PIOC->PIO_OER = AT91C_PIO_PC15;
	/* Clear Output Data Register of the pin */
	AT91C_BASE_PIOC->PIO_CODR = AT91C_PIO_PC15;
	/* wait 2ms until signal low */
	udelay(3000);
	/* Set Output Data Register of the pin */
	AT91C_BASE_PIOC->PIO_SODR = AT91C_PIO_PC15;
	AT91C_BASE_PIOC->PIO_ODR = AT91C_PIO_PC15;
	/* wait 150ms until signal high */
	udelay(150000);

}



int dram_init (void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_SIZE;
	return 0;
}

#ifdef CONFIG_DRIVER_ETHER
#if (CONFIG_COMMANDS & CFG_CMD_NET)

/*
 * Name:
 *	at91rm9200_GetPhyInterface
 * Description:
 *	Initialise the interface functions to the PHY
 * Arguments:
 *	None
 * Return value:
 *	None
 */
void at91rm9200_GetPhyInterface(AT91PS_PhyOps p_phyops)
{
#if (CFG_ETH_KS8993)
	p_phyops->Init		 = ks8993_InitPhy;
	p_phyops->IsPhyConnected = ks8993_IsPhyConnected;
	p_phyops->GetLinkSpeed	 = ks8993_GetLinkSpeed;
	p_phyops->AutoNegotiate	 = NULL;
#elif (CFG_ETH_KS8893)
	p_phyops->Init		 = ks8893_InitPhy;
	p_phyops->IsPhyConnected = ks8893_IsPhyConnected;
	p_phyops->GetLinkSpeed	 = ks8893_GetLinkSpeed;
	p_phyops->AutoNegotiate	 = NULL;
#else /* use CFG_ETH_KS8xx1 */
	p_phyops->Init		 = ks8xx1_InitPhy;
	p_phyops->IsPhyConnected = ks8xx1_IsPhyConnected;
	p_phyops->GetLinkSpeed	 = ks8xx1_GetLinkSpeed;
	p_phyops->AutoNegotiate	 = ks8xx1_AutoNegotiate;
#endif
}

#endif	/* CONFIG_COMMANDS & CFG_CMD_NET */
#endif	/* CONFIG_DRIVER_ETHER */
