/*
 * (C) Copyright 2001-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, d.mueller@elsoft.ch
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

/* This code should work for both the S3C2400 and the S3C2410
 * as they seem to have the same PLL and clock machinery inside.
 * The different address mapping is handled by the s3c24xx.h files below.
 */

#include <common.h>
#include <s5p6440.h>

#define APLL 0
#define MPLL 1
#define EPLL 2

/* ------------------------------------------------------------------------- */
/* NOTE: This describes the proper use of this file.
 *
 * CONFIG_SYS_CLK_FREQ should be defined as the input frequency of the PLL.
 *
 * get_FCLK(), get_HCLK(), get_PCLK() and get_UCLK() return the clock of
 * the specified bus in HZ.
 */
/* ------------------------------------------------------------------------- */

static ulong get_PLLCLK(int pllreg)
{
	ulong r, m, p, s;

	if (pllreg == APLL)
		r = APLL_CON_REG;
	else if (pllreg == MPLL)
		r = MPLL_CON_REG;
	else if (pllreg == EPLL)
		r = EPLL_CON0_REG;
	else
		hang();

	m = (r>>16) & 0x3ff;
	p = (r>>8) & 0x3f;
	s = r & 0x7;

	return (m * (CONFIG_SYS_CLK_FREQ / (p * (1 << s))));
}

/* return ARMCORE frequency */
ulong get_ARMCLK(void)
{
	ulong div;

	div = CLK_DIV0_REG;

	return (get_PLLCLK(APLL) / ((div & 0xf) + 1));
}

/* return FCLK frequency */
ulong get_FCLK(void)
{
	return (get_PLLCLK(APLL));
}

/* return HCLK frequency */
ulong get_HCLK(void)
{
	ulong arm_clk;
	uint hclk_div = ((CLK_DIV0_REG>>8) & 0xf) + 1;

	arm_clk = get_ARMCLK();		// SYNC Mode

	return arm_clk/hclk_div;
}

/* return PCLK frequency */
ulong get_PCLK(void)
{
	ulong hclk;
	uint pre_div = ((CLK_DIV0_REG>>12) & 0xf) + 1;

	hclk = get_HCLK();

	return hclk/pre_div;
}

/* return UCLK frequency */
ulong get_UCLK(void)
{
	return (get_PLLCLK(EPLL));
}

/* return MPLL frequency */
ulong get_MPLL_CLK(void)
{
	return (get_PLLCLK(MPLL));
}

int print_cpuinfo(void)
{
	printf("\nCPU:     S5P6440@%dMHz\n", get_ARMCLK()/1000000);
	printf("         Fclk = %dMHz, Hclk = %dMHz, Pclk = %dMHz",
			get_FCLK()/1000000, get_HCLK()/1000000, get_PCLK()/1000000);

/**************
* Display Serial SRC
***************/

#if defined(CONFIG_CLKSRC_CLKUART)
	puts(", Serial = CLKUART \n");
#else
	puts(", Serial = PCLK \n");
#endif

	return 0;
}
