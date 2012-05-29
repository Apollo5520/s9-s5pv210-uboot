/*
 *  U-Boot command for OneNAND support
 *
 *  Copyright (C) 2005-2008 Samsung Electronics
 *  Kyungmin Park <kyungmin.park@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <command.h>
#include <malloc.h>

#include <linux/mtd/compat.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/onenand.h>
#include <s5pc110.h>				// for timer

#include <asm/io.h>

#if !defined(CONFIG_SYS_64BIT_VSPRINTF)
#warning Please define CONFIG_SYS_64BIT_VSPRINTF for correct output!
#endif

extern struct mtd_info onenand_mtd;
static struct mtd_info *mtd;

#if 0	// incomplete - a firm BBM scheme is required.
static loff_t next_ofs;
static loff_t skip_ofs;
#endif

static inline int str2long(char *p, ulong *num)
{
	char *endptr;

	*num = simple_strtoul(p, &endptr, 16);
	return (*p != '\0' && *endptr == '\0') ? 1 : 0;
}

static int arg_off_size(int argc, char *argv[], ulong *off, size_t *size)
{
	if (argc >= 1) {
		if (!(str2long(argv[0], off))) {
			printf("'%s' is not a number\n", argv[0]);
			return -1;
		}
	} else {
		*off = 0;
	}

	if (argc >= 2) {
		if (!(str2long(argv[1], (ulong *)size))) {
			printf("'%s' is not a number\n", argv[1]);
			return -1;
		}
	} else {
		*size = mtd->size - *off;
	}

	if ((*off + *size) > mtd->size) {
		printf("total chip size (0x%llx) exceeded!\n", mtd->size);
		return -1;
	}

	if (*size == mtd->size)
		puts("whole chip\n");
	else
		printf("offset 0x%lx, size 0x%x\n", *off, *size);

	return 0;
}

extern int onenand_block_read(loff_t from, size_t len,
			      size_t *retlen, u_char *buf, int oob)
{
	struct onenand_chip *this = mtd->priv;
	int blocks = (int) len >> this->erase_shift;
	int blocksize = (1 << this->erase_shift);
	loff_t ofs = from;
	struct mtd_oob_ops ops = {
		.retlen		= 0,
	};
	int ret;

	if (ofs & (mtd->erasesize - 1)) {
		printk(" ERROR: starting address %x is not a block start address\n",
				(unsigned int) ofs);
		return 1;
	}

	if (len & (mtd->erasesize - 1)) {
		printk(" ERROR: %x length is not block aligned\n", (unsigned int) len);
		return 1;
	}

	printk("Main area read (%d blocks):\n", blocks);

	if (oob)
		ops.ooblen = blocksize;
	else
		ops.len = blocksize;

	while (blocks) {
		ret = mtd->block_isbad(mtd, ofs);
		if (ret) {
			printk("Bad blocks %d at 0x%x\n",
			       (u32)(ofs >> this->erase_shift), (u32)ofs);
			ofs += blocksize;
			continue;
		}

		if (oob)
			ops.oobbuf = buf;
		else
			ops.datbuf = buf;

		ops.retlen = 0;
		ret = mtd->read_oob(mtd, ofs, &ops);
		if (ret) {
			printk("Read failed 0x%x, %d\n", (u32)ofs, ret);
			ofs += blocksize;
			continue;
		}
		ofs += blocksize;
		buf += blocksize;
		blocks--;
		*retlen += ops.retlen;
	}

	return 0;
}

extern int onenand_block_write(loff_t to, size_t len,
			       size_t *retlen, const u_char * buf)
{
	struct onenand_chip *this = mtd->priv;
	int blocks = len >> this->erase_shift;
	int blocksize = (1 << this->erase_shift);
	loff_t ofs;
	size_t _retlen = 0;
	int ret;
#if defined(CONFIG_S5PC110) && defined(CONFIG_EVT1) && !defined(CONFIG_FUSED)
	int i;
	ulong checksum;
	uint8_t *ptr;
#endif

#if 0	// incomplete code
	if (to == next_ofs) {
		next_ofs = to + len;
		to += skip_ofs;
	} else {
		next_ofs = to + len;
		skip_ofs = 0;
	}
#endif
	ofs = to;

	if (ofs & (mtd->erasesize - 1)) {
		printk(" ERROR: starting address %x is not a block start address\n",
				(unsigned int) ofs);
		return 1;
	}

	if (len & (mtd->erasesize - 1)) {
		printk(" ERROR: %x length is not block aligned\n", (unsigned int) len);
		return 1;
	}

#if defined(CONFIG_S5PC110) && defined(CONFIG_EVT1) && !defined(CONFIG_FUSED)
	if (to == 0) {
		ptr = buf + 16;
		for(i = 16, checksum = 0; i < 8192; i++) {
			checksum += *ptr;
			ptr++;
		}
		*((volatile u32 *)(buf + 0x8)) = checksum;
	}
#endif

	printk("Main area write (%d blocks):\n", blocks);

	while (blocks) {
		ret = mtd->block_isbad(mtd, ofs);
		if (ret) {
			printk("Bad blocks %d at 0x%x is skipped.\n",
			       (u32)(ofs >> this->erase_shift), (u32)ofs);
			//skip_ofs += blocksize;
			goto next;
		}

		ret = mtd->write(mtd, ofs, blocksize, &_retlen, buf);
		if (ret) {
			printk("Write failed 0x%x, %d", (u32)ofs, ret);
			//skip_ofs += blocksize;
			goto next;
		}

		buf += blocksize;
		blocks--;
		*retlen += _retlen;
next:
		ofs += blocksize;
	}

	return 0;
}

/*
 * Assumption: A variable, to, is a block-aligned address.
 */
extern int onenand_write_yaffs2(loff_t to, size_t len,
			       size_t *retlen, const u_char * buf)
{
	struct onenand_chip *this = mtd->priv;
	loff_t ofs = to;
	struct mtd_oob_ops ops = {
		.retlen		= 0,
	};
	size_t _retlen = 0;
	size_t written = 0;
	int ret;

	if (ofs & (mtd->erasesize - 1)) {
		printk(" ERROR starting address %x is not a block start address\n",
				(unsigned int) ofs);
		return 1;
	}

	printk("Yaffs2 write:\n");

	ops.mode = MTD_OOB_AUTO;
	ops.len = mtd->writesize;
	ops.ooblen = mtd->oobsize;
	
	while (written < len) {
		//if (ofs & (mtd->erasesize - 1)) {
		if ((ofs & (mtd->erasesize - 1)) == 0) {
			ret = mtd->block_isbad(mtd, ofs);
			if (ret) {
				printk("Bad blocks %d at 0x%x is skipped.\n",
				       (u32)(ofs >> this->erase_shift), (u32)ofs);

				ofs += mtd->erasesize;
				continue;
			}
			printk("\rWriting data -- %3d%%.", ((written*4)/(len/25))+1);
		}

		ops.datbuf = buf;
		ops.oobbuf = buf + mtd->writesize;
		
		ret = mtd->write_oob(mtd, ofs, &ops);
		if (ret) {
			printk("ERROR: Write failed 0x%x, %d", (u32)ofs, ret);
			return 1;
		}

		buf += mtd->writesize + mtd->oobsize;
		written += mtd->writesize + mtd->oobsize;
		_retlen += ops.retlen;
		ofs += mtd->writesize;
	}

	printk("\n");
	*retlen = _retlen;

	return 0;
}

extern int onenand_block_erase(u32 start, u32 size, int force)
{

	struct onenand_chip *this;

	mtd = &onenand_mtd;
	this = mtd->priv;
	
	struct erase_info instr = {
		.callback	= NULL,
	};

	loff_t ofs;
	int ret;
	int blocksize = 1 << this->erase_shift;


	for (ofs = start; ofs < (start + size); ofs += blocksize) {
		ret = mtd->block_isbad(mtd, ofs);
		if (ret && !force) {
			printf("Skip erase bad block %d at 0x%x\n",
			       (u32)(ofs >> this->erase_shift), (u32)ofs);
			continue;
		}

		instr.addr = ofs;
		instr.len = blocksize;
		instr.priv = force;
		instr.mtd = mtd;
		ret = mtd->erase(mtd, &instr);
		if (ret) {
			printf("erase failed block %d at 0x%x\n",
			       (u32)(ofs >> this->erase_shift), (u32)ofs);
			continue;
		}
	}

	return 0;
}

static int onenand_block_test(u32 start, u32 size)
{
	struct onenand_chip *this = mtd->priv;
	struct erase_info instr = {
		.callback	= NULL,
		.priv		= 0,
	};

	int blocks;
	loff_t ofs;
	int blocksize = 1 << this->erase_shift;
	int start_block, end_block;
	size_t retlen;
	u_char *buf;
	u_char *verify_buf;
	int ret;

	buf = malloc(blocksize);
	if (!buf) {
		printf("Not enough malloc space available!\n");
		return -1;
	}

	verify_buf = malloc(blocksize);
	if (!verify_buf) {
		printf("Not enough malloc space available!\n");
		return -1;
	}

	start_block = start >> this->erase_shift;
	end_block = (start + size) >> this->erase_shift;

	/* Protect boot-loader from badblock testing */
	if (start_block < 2)
		start_block = 2;

	if (end_block > (mtd->size >> this->erase_shift))
		end_block = mtd->size >> this->erase_shift;

	blocks = start_block;
	ofs = start;
	while (blocks < end_block) {
		printf("\rTesting block %d at 0x%x", (u32)(ofs >> this->erase_shift), (u32)ofs);

		ret = mtd->block_isbad(mtd, ofs);
		if (ret) {
			printf("Skip erase bad block %d at 0x%x\n",
			       (u32)(ofs >> this->erase_shift), (u32)ofs);
			goto next;
		}

		instr.addr = ofs;
		instr.len = blocksize;
		ret = mtd->erase(mtd, &instr);
		if (ret) {
			printk("Erase failed 0x%x, %d\n", (u32)ofs, ret);
			goto next;
		}

		ret = mtd->write(mtd, ofs, blocksize, &retlen, buf);
		if (ret) {
			printk("Write failed 0x%x, %d\n", (u32)ofs, ret);
			goto next;
		}

		ret = mtd->read(mtd, ofs, blocksize, &retlen, verify_buf);
		if (ret) {
			printk("Read failed 0x%x, %d\n", (u32)ofs, ret);
			goto next;
		}

		if (memcmp(buf, verify_buf, blocksize))
			printk("\nRead/Write test failed at 0x%x\n", (u32)ofs);

next:
		ofs += blocksize;
		blocks++;
	}
	printf("...Done\n");

	free(buf);
	free(verify_buf);

	return 0;
}

static int onenand_dump(struct mtd_info *mtd, ulong off, int only_oob)
{
	int i;
	u_char *datbuf, *oobbuf, *p;
	struct mtd_oob_ops ops;
	loff_t addr;

	datbuf = malloc(mtd->writesize + mtd->oobsize);
	oobbuf = malloc(mtd->oobsize);
	if (!datbuf || !oobbuf) {
		puts("No memory for page buffer\n");
		return 1;
	}
	off &= ~(mtd->writesize - 1);
	addr = (loff_t) off;
	memset(&ops, 0, sizeof(ops));
	ops.datbuf = datbuf;
	ops.oobbuf = oobbuf;
	ops.len = mtd->writesize;
	ops.ooblen = mtd->oobsize;
	ops.retlen = 0;
	i = mtd->read_oob(mtd, addr, &ops);
	if (i < 0) {
		printf("Error (%d) reading page %08lx\n", i, off);
		free(datbuf);
		free(oobbuf);
		return 1;
	}
	printf("Page %08lx dump:\n", off);
	i = mtd->writesize >> 4;
	p = datbuf;
	if (!only_oob) {
		while (i--) {
			printf("\t%02x %02x %02x %02x %02x %02x %02x %02x"
			       "  %02x %02x %02x %02x %02x %02x %02x %02x\n",
			       p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
			       p[8], p[9], p[10], p[11], p[12], p[13], p[14],
			       p[15]);
			p += 16;
		}
	}
	
	puts("OOB:\n");
	i = mtd->oobsize >> 3;
	p= oobbuf;
	while (i--) {
		printf("\t%02x %02x %02x %02x %02x %02x %02x %02x\n",
		       p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
		p += 8;
	}
	
	free(datbuf);
	free(oobbuf);

	return 0;
}

int do_onenand(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	struct onenand_chip *this;
	int blocksize;
	ulong addr, ofs;
	size_t len, retlen = 0;
	int ret = 0;
	char *cmd, *s;

	mtd = &onenand_mtd;
	this = mtd->priv;
	blocksize = (1 << this->erase_shift);

	cmd = argv[1];

	switch (argc) {
	case 0:
	case 1:
		goto usage;

	case 2:
		if (strcmp(cmd, "info") == 0) {
			printf("Device %d: ", 0);
			onenand_print_device_info(this->device_id, this->version_id);
			
			return 0;
		}

		if (strcmp(cmd, "bad") == 0) {
			/* Currently only one OneNAND device is supported */
			printf("\nDevice %d bad blocks:\n", 0);
			for (ofs = 0; ofs < mtd->size; ofs += mtd->erasesize) {
				if (mtd->block_isbad(mtd, ofs))
					printf("  %04d at 0x%08x\n", (u32)ofs >> this->erase_shift, (u32)ofs);
			}

			return 0;
		}

	default:
		/* At least 4 args */

		/*
		 * Syntax is:
		 *   0       1     2       3    4
		 *   onenand erase [force] [off size]
		 */
		if ((strcmp(cmd, "erase") == 0) || (strcmp(cmd, "test") == 0)) {
			int force = argc > 2 && !strcmp("force", argv[2]);
			int o = force ? 3 : 2;
			int erase;

			erase = strcmp(cmd, "erase") == 0; /* 1 = erase, 0 = test */
			printf("\nOneNAND %s: ", erase ? "erase" : "test");

			/* skip first two or three arguments, look for offset and size */
			if (arg_off_size(argc - o, argv + o, &ofs, &len) != 0)
				return 1;

			if (erase)
				ret = onenand_block_erase(ofs, len, force);
			else
				ret = onenand_block_test(ofs, len);

			printf("%s\n", ret ? "ERROR" : "OK");

			return ret == 0 ? 0 : 1;
		}

		if (strncmp(cmd, "read", 4) == 0 || strncmp(cmd, "write", 5) == 0) {
			int read;
			int oob = 0;
			int yaffs_write = 0;

			if (argc < 4)
				goto usage;

			addr = (ulong)simple_strtoul(argv[2], NULL, 16);

			read = strncmp(cmd, "read", 4) == 0; /* 1 = read, 0 = write */
			printf("\nOneNAND %s: ", read ? "read" : "write");
			if (arg_off_size(argc - 3, argv + 3, &ofs, &len) != 0)
				return 1;

			s = strchr(cmd, '.');
			if (s != NULL) {
				if ((strcmp(s, ".yaffs2") == 0) || (strcmp(s, ".yaffs") == 0))
					yaffs_write = 1;
				if (strcmp(s, ".oob") == 0)
					oob = 1;
			}

			if (read) {
#ifdef CONFIG_SMDKC110_SPEEDCHECK
				u32 time_start, time_end;
				time_start= get_timer(0);
#endif
				ret = onenand_block_read(ofs, len, &retlen, (u8 *)addr, oob);
#ifdef CONFIG_SMDKC110_SPEEDCHECK
				time_end= get_timer(0);

				printf("0x%08x - 0x%08x: %d\n", time_start, time_end, (time_end - time_start));
#endif
			} else {
				if (yaffs_write == 0) {
					ret = onenand_block_write(ofs, len, &retlen, (u8 *)addr);
				} else {
					ret = onenand_write_yaffs2(ofs, len, &retlen, (u8 *)addr);
				}
			}

			printf(" %d bytes %s: %s\n", retlen,
			       read ? "read" : "written", ret ? "ERROR" : "OK");

			return ret == 0 ? 0 : 1;
		}

		if (strcmp(cmd, "markbad") == 0) {
			argc -= 2;
			argv += 2;

			if (argc <= 0)
				goto usage;

			while (argc > 0) {
				addr = simple_strtoul(*argv, NULL, 16);

				if (mtd->block_markbad(mtd, addr)) {
				//if (onenand_block_markbad(mtd, addr)) {
					printf("block 0x%08lx NOT marked "
						"as bad! ERROR %d\n",
						addr, ret);
					ret = 1;
				} else {
					printf("block 0x%08lx successfully "
						"marked as bad\n",
						addr);
				}
				--argc;
				++argv;
			}
			return ret;
		}

		if (strncmp(cmd, "dump", 4) == 0) {
			if (argc < 3)
				goto usage;

			s = strchr(cmd, '.');
			ofs = (int)simple_strtoul(argv[2], NULL, 16);

			if (s != NULL && strcmp(s, ".oob") == 0)
				ret = onenand_dump(mtd, ofs, 1);
			else
				ret = onenand_dump(mtd, ofs, 0);

			return ret == 0 ? 1 : 0;
		}

		break;
	}

	return 0;

usage:
	printf("Usage:\n%s\n", cmdtp->usage);	
	return 1;
}

U_BOOT_CMD(
	onenand,	6, 1, do_onenand,
	"OneNAND sub-system",
	"info - show available OneNAND devices\n"
	"onenand bad - show bad blocks\n"
	"onenand read[.oob] addr off size\n"
	"onenand write[.oob] addr off size\n"
	"    read/write 'size' bytes starting at offset 'off'\n"
	"    to/from memory address 'addr', skipping bad blocks.\n"
	"onenand write.yaffs addr off size - read/write `size' bytes starting\n"
	"    at offset `off' to/from memory address `addr'\n"
	"onenand erase [force] [off size] - erase 'size' bytes from\n"
	"onenand test [off size] - test 'size' bytes from\n"
	"    offset 'off' (entire device if not specified)\n"
	"onenand dump[.oob] off - dump page\n"
	"onenand markbad off [...] - mark bad block(s) at offset (UNSAFE)"
);

