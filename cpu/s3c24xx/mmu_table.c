/*
 * $Id: mmu_table.c,v 1.3 2008/11/08 02:38:33 scsuh Exp $
 *
 * (C) Copyright 2008 Samsung Electronics
 *       SeungChull, Suh <sc.suh@samsung.com>
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

/*
 * This function can make mmu_table on run_time.
 * It takes only trivial time to make table. Thus using this function
 * is recommended not using static mmu_table in lowlevel_init.S
 * To enable this functionality add definitions of
 * CONFIG_BUILD_MMU_TABLE_RUNTIME and MMU_TABLE_BASE on your config file.
 * refering to include/configs/smdk2450.h will be helpful.
 *
 * By scsuh.
 */


#include <common.h>

#ifdef CONFIG_BUILD_MMU_TABLE_RUNTIME
#define SECTION_ENTRY(base,ap,d,c,b) \
	((base << 20)|(ap<<10)|(d<<5)|(1<<4)|(c<<3)|(b<<2)|(1<<1))

void make_mmu_table (void)
{
	int i;
	uint *addr = (uint*)MMU_TABLE_BASE;

	/* 1:1 mapping */
	for (i=(MMU_IO_AREA_START>>20); i<(MMU_RESERVED1_START>>20); i++) {
		addr[i] = SECTION_ENTRY(i,3,0,0,0);
	}

	/* disabled */
	for (i=(MMU_RESERVED1_START>>20); i<(MMU_MEM_AREA_START>>20); i++) {
		addr[i] = 0x00000000;
	}

	/* mapping system memory to 0xc0000000 */
	for (i=(MMU_MEM_AREA_START>>20); i<(MMU_RESERVED2_START>>20); i++) {
		addr[i] = SECTION_ENTRY((i-(0xc00-0x300)),3,0,1,1);
	}

	/* disabled */
	for (i=(MMU_RESERVED2_START>>20); i<0x1000; i++) {
		addr[i] = 0x00000000;
	}
}

#endif

