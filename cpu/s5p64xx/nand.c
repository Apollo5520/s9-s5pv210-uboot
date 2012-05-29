/*
 * (C) Copyright 2006 DENX Software Engineering
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
 *
 * original code is from mtd part in U-Boot.
 * 1st modification for S3C is done by js.yang.
 * 2nd modification for S3C is done by sc.suh.
 *                      (because of new version of nand controller)
 */

#include <common.h>

#if defined(CONFIG_CMD_NAND)
#include <nand.h>
#include <regs.h>

#include <asm/io.h>
#include <asm/errno.h>

/* When NAND is used as boot device, below is set to 1. */
int boot_nand = 0;

/* Nand flash definition values by jsgood */
#define S3C_NAND_TYPE_UNKNOWN	0x0
#define S3C_NAND_TYPE_SLC	0x1
#define S3C_NAND_TYPE_MLC	0x2
#undef	S3C_NAND_DEBUG

/* Nand flash global values by jsgood */
int cur_ecc_mode = 0;
int nand_type = S3C_NAND_TYPE_UNKNOWN;

/* Nand flash oob definition for SLC 512b page size by jsgood */
static struct nand_ecclayout s3c_nand_oob_16 = {
	.useecc = MTD_NANDECC_AUTOPLACE,	/* Only for U-Boot */
	.eccbytes = 4,
	.eccpos = {1, 2, 3, 4},
	.oobfree = {
		{.offset = 6,
		 . length = 10}}
};

#if 1
/* Nand flash oob definition for SLC 2k page size by jsgood */
static struct nand_ecclayout s3c_nand_oob_64 = {
	.useecc = MTD_NANDECC_AUTOPLACE,	/* Only for U-Boot */
	.eccbytes = 16,
	.eccpos = {40, 41, 42, 43, 44, 45, 46, 47,
		   48, 49, 50, 51, 52, 53, 54, 55},
	.oobfree = {
		{.offset = 2,
		 .length = 38}}
};
#else
/* Nand flash oob definition for SLC 2k page size by jsgood */
static struct nand_ecclayout s3c_nand_oob_64 = {
	.useecc = MTD_NANDECC_AUTOPLACE,	/* Only for U-Boot */
	.eccbytes = 4,
	.eccpos = {56, 57, 58, 59},
	.oobfree	= {
		{2, 6}, {13, 3}, {18, 6}, {29, 3},
		{34, 6}, {45, 3}, {50, 6}, {61, 3}}
};
#endif

/* Nand flash oob definition for MLC 2k page size by jsgood */
static struct nand_ecclayout s3c_nand_oob_mlc_64 = {
	.useecc = MTD_NANDECC_AUTOPLACE,	/* Only for U-Boot */
	.eccbytes = 32,
	.eccpos = {
		   32, 33, 34, 35, 36, 37, 38, 39,
		   40, 41, 42, 43, 44, 45, 46, 47,
 		   48, 49, 50, 51, 52, 53, 54, 55,
   		   56, 57, 58, 59, 60, 61, 62, 63},
	.oobfree = {
		{.offset = 2,
		 .length = 28}}
};

#if defined(S3C_NAND_DEBUG)
/*
 * Function to print out oob buffer for debugging
 * Written by jsgood
 */
static void print_oob(const char *header, struct mtd_info *mtd)
{
	int i;
	struct nand_chip *chip = mtd->priv;

	printk("%s:\t", header);

	for(i = 0; i < 64; i++)
		printk("%02x ", chip->oob_poi[i]);

	printk("\n");
}
#endif

/*
 * Hardware specific access to control-lines function
 * Written by jsgood
 */
static void s3c_nand_hwcontrol(struct mtd_info *mtd, int dat, unsigned int ctrl)
{
	unsigned int cur;

	if (ctrl & NAND_CTRL_CHANGE) {
		if (ctrl & NAND_NCE) {
			if (dat != NAND_CMD_NONE) {
				cur = readl(NFCONT);

				if (boot_nand)
					cur &= ~NFCONT_CS;
				else
					cur &= ~NFCONT_CS_ALT;

				writel(cur, NFCONT);
			}
		} else {
			cur = readl(NFCONT);

			if (boot_nand)
				cur |= NFCONT_CS;
			else
				cur |= NFCONT_CS_ALT;

			writel(cur, NFCONT);
		}
	}

	if (dat != NAND_CMD_NONE) {
		if (ctrl & NAND_CLE)
			writeb(dat, NFCMMD);
		else if (ctrl & NAND_ALE)
			writeb(dat, NFADDR);
	}
}

/*
 * Function for checking device ready pin
 * Written by jsgood
 */
static int s3c_nand_device_ready(struct mtd_info *mtdinfo)
{
	while (!(readl(NFSTAT) & NFSTAT_RnB)) {}
	return 1;
}

/*
 * We don't use bad block table
 */
static int s3c_nand_scan_bbt(struct mtd_info *mtdinfo)
{
	return nand_default_bbt(mtdinfo);
}

#if defined(CFG_NAND_HWECC)
#if 0
/*
 * S3C Nand flash chip enable function
 * Written by jsgood
 */
static void s3c_nand_ce_on(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;

	chip->cmd_ctrl(mtd, 0x0, NAND_NCE | NAND_CTRL_CHANGE);
	nand_wait_ready(mtd);
}

/*
 * S3C Nand flash chip disable function
 * Written by jsgood
 */
static void s3c_nand_ce_off(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;

	chip->cmd_ctrl(mtd, NAND_CMD_NONE, NAND_CTRL_CHANGE);
	nand_wait_ready(mtd);
}
#endif

/*
 * Function for checking ECCEncDone in NFSTAT
 * Written by jsgood
 */
static void s3c_nand_wait_enc(void)
{
	while (!(readl(NFSTAT) & NFSTAT_ECCENCDONE)) {}
}

/*
 * Function for checking ECCDecDone in NFSTAT
 * Written by jsgood
 */
static void s3c_nand_wait_dec(void)
{
	while (!(readl(NFSTAT) & NFSTAT_ECCDECDONE)) {}
}

/*
 * Function for checking ECC Busy
 * Written by jsgood
 */
static void s3c_nand_wait_ecc_busy(void)
{
	while (readl(NFESTAT0) & NFESTAT0_ECCBUSY) {}
}

/*
 * This function is called before encoding ecc codes to ready ecc engine.
 * Written by jsgood
 */
static void s3c_nand_enable_hwecc(struct mtd_info *mtd, int mode)
{
	u_long nfcont, nfconf;

	cur_ecc_mode = mode;

	nfconf = readl(NFCONF);

#if defined(CONFIG_S3C6410)
	nfconf &= ~(0x3 << 23);

	if (nand_type == S3C_NAND_TYPE_SLC)
		nfconf |= NFCONF_ECC_1BIT;
	else
		nfconf |= NFCONF_ECC_4BIT;
#else
	if (nand_type == S3C_NAND_TYPE_SLC)
		nfconf &= ~NFCONF_ECC_MLC;	/* SLC */
	else
		nfconf |= NFCONF_ECC_MLC;	/* MLC */
#endif

	writel(nfconf, NFCONF);

	/* Initialize & unlock */
	nfcont = readl(NFCONT);
	nfcont |= NFCONT_INITMECC;
	nfcont &= ~NFCONT_MECCLOCK;

	if (nand_type == S3C_NAND_TYPE_MLC) {
		if (mode == NAND_ECC_WRITE)
			nfcont |= NFCONT_ECC_ENC;
		else if (mode == NAND_ECC_READ)
			nfcont &= ~NFCONT_ECC_ENC;
	}

	writel(nfcont, NFCONT);
}

/*
 * This function is called immediately after encoding ecc codes.
 * This function returns encoded ecc codes.
 * Written by jsgood
 */
static int s3c_nand_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
	u_long nfcont, nfmecc0, nfmecc1;

	/* Lock */
	nfcont = readl(NFCONT);
	nfcont |= NFCONT_MECCLOCK;
	writel(nfcont, NFCONT);

	if (nand_type == S3C_NAND_TYPE_SLC) {
		nfmecc0 = readl(NFMECC0);

		ecc_code[0] = nfmecc0 & 0xff;
		ecc_code[1] = (nfmecc0 >> 8) & 0xff;
		ecc_code[2] = (nfmecc0 >> 16) & 0xff;
		ecc_code[3] = (nfmecc0 >> 24) & 0xff;
	} else {
		if (cur_ecc_mode == NAND_ECC_READ)
			s3c_nand_wait_dec();
		else {
			s3c_nand_wait_enc();

			nfmecc0 = readl(NFMECC0);
			nfmecc1 = readl(NFMECC1);

			ecc_code[0] = nfmecc0 & 0xff;
			ecc_code[1] = (nfmecc0 >> 8) & 0xff;
			ecc_code[2] = (nfmecc0 >> 16) & 0xff;
			ecc_code[3] = (nfmecc0 >> 24) & 0xff;
			ecc_code[4] = nfmecc1 & 0xff;
			ecc_code[5] = (nfmecc1 >> 8) & 0xff;
			ecc_code[6] = (nfmecc1 >> 16) & 0xff;
			ecc_code[7] = (nfmecc1 >> 24) & 0xff;
		}
	}

	return 0;
}

/*
 * This function determines whether read data is good or not.
 * If SLC, must write ecc codes to controller before reading status bit.
 * If MLC, status bit is already set, so only reading is needed.
 * If status bit is good, return 0.
 * If correctable errors occured, do that.
 * If uncorrectable errors occured, return -1.
 * Written by jsgood
 */
static int s3c_nand_correct_data(struct mtd_info *mtd, u_char *dat, u_char *read_ecc, u_char *calc_ecc)
{
	int ret = -1;
	u_long nfestat0, nfestat1, nfmeccdata0, nfmeccdata1, nfmlcbitpt;
	u_char err_type;

	if (nand_type == S3C_NAND_TYPE_SLC) {
		/* SLC: Write ecc to compare */
		nfmeccdata0 = (read_ecc[1] << 16) | read_ecc[0];
		nfmeccdata1 = (read_ecc[3] << 16) | read_ecc[2];
		writel(nfmeccdata0, NFMECCDATA0);
		writel(nfmeccdata1, NFMECCDATA1);

		/* Read ecc status */
		nfestat0 = readl(NFESTAT0);
		err_type = nfestat0 & 0x3;

		switch (err_type) {
		case 0: /* No error */
			ret = 0;
			break;

		case 1: /* 1 bit error (Correctable)
			   (nfestat0 >> 7) & 0x7ff	:error byte number
			   (nfestat0 >> 4) & 0x7	:error bit number */
			printk("s3c-nand: 1 bit error detected at byte %ld, correcting from "
					"0x%02x ", (nfestat0 >> 7) & 0x7ff, dat[(nfestat0 >> 7) & 0x7ff]);
			dat[(nfestat0 >> 7) & 0x7ff] ^= (1 << ((nfestat0 >> 4) & 0x7));
			printk("to 0x%02x...OK\n", dat[(nfestat0 >> 7) & 0x7ff]);
			ret = 1;
			break;

		case 2: /* Multiple error */
		case 3: /* ECC area error */
			printk("s3c-nand: ECC uncorrectable error detected\n");
			ret = -1;
			break;
		}
	} else {
		/* MLC: */
		s3c_nand_wait_ecc_busy();

		nfestat0 = readl(NFESTAT0);
		nfestat1 = readl(NFESTAT1);
		nfmlcbitpt = readl(NFMLCBITPT);

		err_type = (nfestat0 >> 26) & 0x7;

		/* No error, If free page (all 0xff) */
		if ((nfestat0 >> 29) & 0x1) {
			err_type = 0;
		} else {
			/* No error, If all 0xff from 17th byte in oob (in case of JFFS2 format) */
			if (dat) {
				if (dat[17] == 0xff && dat[26] == 0xff && dat[35] == 0xff && dat[44] == 0xff && dat[54] == 0xff)
					err_type = 0;
			}
		}

		switch (err_type) {
		case 5: /* Uncorrectable */
			printk("s3c-nand: ECC uncorrectable error detected\n");
			ret = -1;
			break;

		case 4: /* 4 bit error (Correctable) */
			dat[(nfestat1 >> 16) & 0x3ff] ^= ((nfmlcbitpt >> 24) & 0xff);

		case 3: /* 3 bit error (Correctable) */
			dat[nfestat1 & 0x3ff] ^= ((nfmlcbitpt >> 16) & 0xff);

		case 2: /* 2 bit error (Correctable) */
			dat[(nfestat0 >> 16) & 0x3ff] ^= ((nfmlcbitpt >> 8) & 0xff);

		case 1: /* 1 bit error (Correctable) */
			printk("s3c-nand: %d bit(s) error detected, corrected successfully\n", err_type);
			dat[nfestat0 & 0x3ff] ^= (nfmlcbitpt & 0xff);
			ret = err_type;
			break;

		case 0: /* No error */
			ret = 0;
			break;
		}
	}

	return ret;
}

#if defined(CONFIG_NAND_BL1_8BIT_ECC) && (defined(CONFIG_S5P6440))
/***************************************************************
 * jsgood: Temporary 8 Bit H/W ECC supports for BL1 (6440 only)
 ***************************************************************/
static void s3c_nand_wait_dec_8bit(void)
{
	uint i = 0;
	for (i=0; i<0x100000; i++) {
		if (readl(NFECCSTAT) & NFECCSTAT_DEC_DONE)
			return;
	}
	printf("ecc dec timeout\n");
}

static void s3c_nand_wait_enc_8bit(void)
{
	uint i = 0;
	for (i=0; i<0x100000; i++) {
		if (readl(NFECCSTAT) & NFECCSTAT_ENC_DONE)
			return;
	}
	printf("ecc enc timeout\n");
}

static void s3c_nand_wait_ecc_busy_8bit(void)
{
	uint i = 0;
	for (i=0; i<0x100000; i++) {
		if (readl(NFECCSTAT) & NFECCSTAT_BUSY)
			return;
	}
	printf("wait ecc timeout\n");
}

/*
 * mode: NAND_ECC_WRITE/NAND_ECC_READ
 */
void s3c_nand_enable_hwecc_8bit(struct mtd_info *mtd, int mode)
{
	uint32_t nfeccconf, nfecccont, nfcont;

	cur_ecc_mode = mode;

	/* 8bit/512byte selection */
	nfeccconf = NFECCCONF_ECCTYPE_8_512 | NFECCCONF_MSGLENGTH(511);
	writel(nfeccconf, NFECCCONF);

	/* Initialize & unlock */
	nfecccont = readl(NFECCCONT);
	nfecccont |= NFECCCONT_INITECC;

	if (mode == NAND_ECC_WRITE)
		nfecccont |= NFECCCONT_ECCENC;
	else if (mode == NAND_ECC_READ)
		nfecccont &= ~NFECCCONT_ECCENC;

	writel(nfecccont, NFECCCONT);

	/* Initialize & unlock */
	nfcont = readl(NFCONT);
	nfcont &= ~NFCONT_MECCLOCK;
	writel(nfcont, NFCONT);

	writel(0xffffffff, NFECCSTAT);
//	printf("readl(NFECCSTAT) = %08x\n", readl(NFECCSTAT));
}

int s3c_nand_calculate_ecc_8bit(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
	u_long nfcont, nfm8ecc0, nfm8ecc1, nfm8ecc2, nfm8ecc3;

	/* Lock */
	nfcont = readl(NFCONT);
	nfcont |= NFCONT_MECCLOCK;
	writel(nfcont, NFCONT);

	if (cur_ecc_mode == NAND_ECC_READ) {
		s3c_nand_wait_dec_8bit();
	}
	else {
		s3c_nand_wait_enc_8bit();

		nfm8ecc0 = readl(NFECCPRGECC0);
		nfm8ecc1 = readl(NFECCPRGECC1);
		nfm8ecc2 = readl(NFECCPRGECC2);
		nfm8ecc3 = readl(NFECCPRGECC3);

		ecc_code[0] = nfm8ecc0 & 0xff;
		ecc_code[1] = (nfm8ecc0 >> 8) & 0xff;
		ecc_code[2] = (nfm8ecc0 >> 16) & 0xff;
		ecc_code[3] = (nfm8ecc0 >> 24) & 0xff;
		ecc_code[4] = nfm8ecc1 & 0xff;
		ecc_code[5] = (nfm8ecc1 >> 8) & 0xff;
		ecc_code[6] = (nfm8ecc1 >> 16) & 0xff;
		ecc_code[7] = (nfm8ecc1 >> 24) & 0xff;
		ecc_code[8] = nfm8ecc2 & 0xff;
		ecc_code[9] = (nfm8ecc2 >> 8) & 0xff;
		ecc_code[10] = (nfm8ecc2 >> 16) & 0xff;
		ecc_code[11] = (nfm8ecc2 >> 24) & 0xff;
		ecc_code[12] = nfm8ecc3 & 0xff;
	}

	return 0;
}

int s3c_nand_correct_data_8bit(struct mtd_info *mtd, u_char *dat, u_char *read_ecc, u_char *calc_ecc)
{
	int ret = -1;
	uint err_type;
	ulong nfeccerl0, nfeccerl1, nfeccerl2, nfeccerl3, nfeccerp0, nfeccerp1;
	ulong nfeccstat;

	s3c_nand_wait_ecc_busy_8bit();

	printf("s3c_nand_correct_data_8bit()\n");

	nfeccstat = readl(NFECCSTAT);
	nfeccerl0 = readl(NFMLCECCERL0);
	nfeccerl1 = readl(NFMLCECCERL1);
	nfeccerl2 = readl(NFMLCECCERL2);
	nfeccerl3 = readl(NFMLCECCERL3);

	nfeccerp0 = readl(NFMLCECCERP0);
	nfeccerp1 = readl(NFMLCECCERP1);

	/* No error, If free page (all 0xff) */
	if (nfeccstat & NFECCSTAT_FREE_PAGE)
		err_type = 0;
	else
		err_type = readl(NFECCSECSTAT0) & 0x1f;

	/* no errors are found. */
	if (err_type == 0)
		return 0;

	/* unrecoverable errors are found */
	if (err_type > 8) {
		printf("s3c-nand: ECC uncorrectable error: %d\n", err_type);
		return -1;
	}

	switch (err_type) {
	case 8: /* 8 bit error (Correctable) */
		dat[(nfeccerl3 >> 16) & 0x3ff] ^= ((nfeccerp1 >> 24) & 0xff);

	case 7: /* 7 bit error (Correctable) */
		dat[nfeccerl3 & 0x3ff] ^= ((nfeccerp1 >> 16) & 0xff);

	case 6: /* 6 bit error (Correctable) */
		dat[(nfeccerl2 >> 16) & 0x3ff] ^= ((nfeccerp1 >> 8) & 0xff);

	case 5: /* 5 bit error (Correctable) */
		dat[nfeccerl2 & 0x3ff] ^= (nfeccerp1 & 0xff);

	case 4: /* 4 bit error (Correctable) */
		dat[(nfeccerl1 >> 16) & 0x3ff] ^= ((nfeccerp0 >> 24) & 0xff);

	case 3: /* 3 bit error (Correctable) */
		dat[nfeccerl1 & 0x3ff] ^= ((nfeccerp0 >> 16) & 0xff);

	case 2: /* 2 bit error (Correctable) */
		dat[(nfeccerl0 >> 16) & 0x3ff] ^= ((nfeccerp0 >> 8) & 0xff);

	case 1: /* 1 bit error (Correctable) */
		printf("s3c-nand: %d bit(s) error is recoverd.\n", err_type);
		dat[nfeccerl0 & 0x3ff] ^= (nfeccerp0 & 0xff);
		ret = err_type;
		break;

	default:
		printf("err_type is not recognized: %d\n", err_type);
		ret = -1;
	}

	return ret;
}

void s3c_nand_write_page_8bit(struct mtd_info *mtd, struct nand_chip *chip,
				  const uint8_t *buf)
{
	int i, eccsize = 512;
	int eccbytes = 13;
	int eccsteps = mtd->writesize / eccsize;
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	uint8_t *p = (uint8_t *) buf;

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		s3c_nand_enable_hwecc_8bit(mtd, NAND_ECC_WRITE);
		chip->write_buf(mtd, p, eccsize);
		s3c_nand_calculate_ecc_8bit(mtd, p, &ecc_calc[i]);
	}

	for (i = 0; i < eccbytes * (mtd->writesize / eccsize); i++)
		chip->oob_poi[i] = ecc_calc[i];

	chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);
}

void s3c_nand_write_page_8bit_bug(struct mtd_info *mtd, struct nand_chip *chip,
				  const uint8_t *buf)
{
	int i, eccsize = 512;
	int eccbytes = 13;
	int eccsteps = mtd->writesize / eccsize;
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	uint8_t *p = (uint8_t *) buf;
	uint col;

	/* Step1: read whole oob */
	col = mtd->writesize;
	chip->cmdfunc(mtd, NAND_CMD_RNDOUT, col, -1);
	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		s3c_nand_enable_hwecc_8bit(mtd, NAND_ECC_WRITE);
		chip->write_buf(mtd, p, eccsize);
		s3c_nand_calculate_ecc_8bit(mtd, p, &ecc_calc[i]);
	}

	chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);
}

int s3c_nand_read_page_8bit(struct mtd_info *mtd, struct nand_chip *chip,
				uint8_t *buf)
{
	int i, stat, eccsize = 512;
	int eccbytes = 13;
	int eccsteps = mtd->writesize / eccsize;
	int col = 0;
	uint8_t *p = buf;

	/* Step1: read whole oob */
	col = mtd->writesize;
	chip->cmdfunc(mtd, NAND_CMD_RNDOUT, col, -1);
	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	col = 0;
	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		chip->cmdfunc(mtd, NAND_CMD_RNDOUT, col, -1);
		s3c_nand_enable_hwecc_8bit(mtd, NAND_ECC_READ);
		chip->read_buf(mtd, p, eccsize);
		chip->write_buf(mtd, chip->oob_poi + (((mtd->writesize / eccsize) - eccsteps) * eccbytes), eccbytes);
		s3c_nand_calculate_ecc_8bit(mtd, 0, 0);

		if (mtd->writesize > 512) {
			if ((readl(NFECCSECSTAT0) & 0x1f) != 0) {
				printf("NFECCSECSTAT0: %08x\n", readl(NFECCSECSTAT0));
				stat = s3c_nand_correct_data_8bit(mtd, p, 0, 0);
				if (stat == -1)
					mtd->ecc_stats.failed++;
			}
		}

		col = eccsize * ((mtd->writesize / eccsize) + 1 - eccsteps);
	}

	return 0;
}

/********************************************************/
#endif

static int s3c_nand_write_oob_1bit(struct mtd_info *mtd, struct nand_chip *chip,
			      int page)
{
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	int status = 0;
	int eccbytes = chip->ecc.bytes;
	int secc_start = mtd->oobsize - eccbytes;
	int i;

	chip->cmdfunc(mtd, NAND_CMD_SEQIN, mtd->writesize, page);

	/* spare area */
	chip->ecc.hwctl(mtd, NAND_ECC_WRITE);
	chip->write_buf(mtd, chip->oob_poi, secc_start);
	chip->ecc.calculate(mtd, 0, &ecc_calc[chip->ecc.total]);

	for (i = 0; i < eccbytes; i++)
		chip->oob_poi[secc_start + i] = ecc_calc[chip->ecc.total + i];

	chip->write_buf(mtd, chip->oob_poi + secc_start, eccbytes);

	/* Send command to program the OOB data */
	chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);

	status = chip->waitfunc(mtd, chip);

	return status & NAND_STATUS_FAIL ? -EIO : 0;
}

static int s3c_nand_read_oob_1bit(struct mtd_info *mtd, struct nand_chip *chip,
			     int page, int sndcmd)
{
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	int eccbytes = chip->ecc.bytes;
	int secc_start = mtd->oobsize - eccbytes;

	if (sndcmd) {
		chip->cmdfunc(mtd, NAND_CMD_READOOB, 0, page);
		sndcmd = 0;
	}

	chip->ecc.hwctl(mtd, NAND_ECC_READ);
	chip->read_buf(mtd, chip->oob_poi, secc_start);
	chip->ecc.calculate(mtd, 0, &ecc_calc[chip->ecc.total]);
	chip->read_buf(mtd, chip->oob_poi + secc_start, eccbytes);

	/* jffs2 special case */
	if (!(chip->oob_poi[2] == 0x85 && chip->oob_poi[3] == 0x19))
		chip->ecc.correct(mtd, chip->oob_poi, chip->oob_poi + secc_start, 0);

	return sndcmd;
}

static void s3c_nand_write_page_1bit(struct mtd_info *mtd, struct nand_chip *chip,
				  const uint8_t *buf)
{
	int i, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	int secc_start = mtd->oobsize - eccbytes;
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	const uint8_t *p = buf;

	uint32_t *eccpos = chip->ecc.layout->eccpos;

	/* main area */
	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		chip->ecc.hwctl(mtd, NAND_ECC_WRITE);
		chip->write_buf(mtd, p, eccsize);
		chip->ecc.calculate(mtd, p, &ecc_calc[i]);
	}

	for (i = 0; i < chip->ecc.total; i++)
		chip->oob_poi[eccpos[i]] = ecc_calc[i];

	/* spare area */
	chip->ecc.hwctl(mtd, NAND_ECC_WRITE);
	chip->write_buf(mtd, chip->oob_poi, secc_start);
	chip->ecc.calculate(mtd, p, &ecc_calc[chip->ecc.total]);

	for (i = 0; i < eccbytes; i++)
		chip->oob_poi[secc_start + i] = ecc_calc[chip->ecc.total + i];

	chip->write_buf(mtd, chip->oob_poi + secc_start, eccbytes);
}

static int s3c_nand_read_page_1bit(struct mtd_info *mtd, struct nand_chip *chip,
				uint8_t *buf)
{
	int i, stat, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	int secc_start = mtd->oobsize - eccbytes;
	int col = 0;
	uint8_t *p = buf;
	uint32_t *mecc_pos = chip->ecc.layout->eccpos;
	uint8_t *ecc_calc = chip->buffers->ecccalc;

	col = mtd->writesize;
	chip->cmdfunc(mtd, NAND_CMD_RNDOUT, col, -1);

	/* spare area */
	chip->ecc.hwctl(mtd, NAND_ECC_READ);
	chip->read_buf(mtd, chip->oob_poi, secc_start);
	chip->ecc.calculate(mtd, p, &ecc_calc[chip->ecc.total]);
	chip->read_buf(mtd, chip->oob_poi + secc_start, eccbytes);

	/* jffs2 special case */
	if (!(chip->oob_poi[2] == 0x85 && chip->oob_poi[3] == 0x19))
		chip->ecc.correct(mtd, chip->oob_poi, chip->oob_poi + secc_start, 0);

	col = 0;

	/* main area */
	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		chip->cmdfunc(mtd, NAND_CMD_RNDOUT, col, -1);
		chip->ecc.hwctl(mtd, NAND_ECC_READ);
		chip->read_buf(mtd, p, eccsize);
		chip->ecc.calculate(mtd, p, &ecc_calc[i]);

		stat = chip->ecc.correct(mtd, p, chip->oob_poi + mecc_pos[0] + ((chip->ecc.steps - eccsteps) * eccbytes), 0);
		if (stat == -1)
			mtd->ecc_stats.failed++;

		col = eccsize * (chip->ecc.steps + 1 - eccsteps);
	}

	return 0;
}

/*
 * Hardware specific page read function for MLC.
 * Written by jsgood
 */
static int s3c_nand_read_page_4bit(struct mtd_info *mtd, struct nand_chip *chip,
				uint8_t *buf)
{
	int i, stat, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	int col = 0;
	uint8_t *p = buf;
	uint32_t *mecc_pos = chip->ecc.layout->eccpos;

	/* Step1: read whole oob */
	col = mtd->writesize;
	chip->cmdfunc(mtd, NAND_CMD_RNDOUT, col, -1);
	chip->read_buf(mtd, chip->oob_poi, mtd->oobsize);

	col = 0;
	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		chip->cmdfunc(mtd, NAND_CMD_RNDOUT, col, -1);
		chip->ecc.hwctl(mtd, NAND_ECC_READ);
		chip->read_buf(mtd, p, eccsize);
		chip->write_buf(mtd, chip->oob_poi + mecc_pos[0] + ((chip->ecc.steps - eccsteps) * eccbytes), eccbytes);
		chip->ecc.calculate(mtd, 0, 0);
		stat = chip->ecc.correct(mtd, p, 0, 0);

		if (stat == -1)
			mtd->ecc_stats.failed++;

		col = eccsize * (chip->ecc.steps + 1 - eccsteps);
	}

	return 0;
}

/*
 * Hardware specific page write function for MLC.
 * Written by jsgood
 */
static void s3c_nand_write_page_4bit(struct mtd_info *mtd, struct nand_chip *chip,
				  const uint8_t *buf)
{
	int i, eccsize = chip->ecc.size;
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	const uint8_t *p = buf;
	uint8_t *ecc_calc = chip->buffers->ecccalc;
	uint32_t *mecc_pos = chip->ecc.layout->eccpos;

	/* Step1: write main data and encode mecc */
	for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize) {
		chip->ecc.hwctl(mtd, NAND_ECC_WRITE);
		chip->write_buf(mtd, p, eccsize);
		chip->ecc.calculate(mtd, p, &ecc_calc[i]);
	}

	/* Step2: save encoded mecc */
	for (i = 0; i < chip->ecc.total; i++)
		chip->oob_poi[mecc_pos[i]] = ecc_calc[i];

	chip->write_buf(mtd, chip->oob_poi, mtd->oobsize);
}
#endif

/*
 * Board-specific NAND initialization. The following members of the
 * argument are board-specific (per include/linux/mtd/nand.h):
 * - IO_ADDR_R?: address to read the 8 I/O lines of the flash device
 * - IO_ADDR_W?: address to write the 8 I/O lines of the flash device
 * - hwcontrol: hardwarespecific function for accesing control-lines
 * - dev_ready: hardwarespecific function for  accesing device ready/busy line
 * - enable_hwecc?: function to enable (reset)  hardware ecc generator. Must
 *   only be provided if a hardware ECC is available
 * - eccmode: mode of ecc, see defines
 * - chip_delay: chip dependent delay for transfering data from array to
 *   read regs (tR)
 * - options: various chip options. They can partly be set to inform
 *   nand_scan about special functionality. See the defines for further
 *   explanation
 * Members with a "?" were not set in the merged testing-NAND branch,
 * so they are not set here either.
 */
void board_nand_init(struct nand_chip *nand)
{
#if defined(CFG_NAND_HWECC)
	int i;
	u_char tmp;
	struct nand_flash_dev *type = NULL;
#endif

	if (INF_REG3_REG >= 2 && INF_REG3_REG <= 6)
		boot_nand = 1;
	else
		boot_nand = 0;

	NFCONT_REG 		&= ~NFCONT_WP;
	nand->IO_ADDR_R		= (void __iomem *)(NFDATA);
	nand->IO_ADDR_W		= (void __iomem *)(NFDATA);
	nand->cmd_ctrl		= s3c_nand_hwcontrol;
	nand->dev_ready		= s3c_nand_device_ready;
	nand->scan_bbt		= s3c_nand_scan_bbt;
	nand->options		= 0;

#if defined(CFG_NAND_FLASH_BBT)
		nand->options 		|= NAND_USE_FLASH_BBT;
#else
		nand->options		|= NAND_SKIP_BBTSCAN;
#endif

#if defined(CFG_NAND_HWECC)
	nand->ecc.mode		= NAND_ECC_HW;
	nand->ecc.hwctl		= s3c_nand_enable_hwecc;
	nand->ecc.calculate	= s3c_nand_calculate_ecc;
	nand->ecc.correct	= s3c_nand_correct_data;

	s3c_nand_hwcontrol(0, NAND_CMD_READID, NAND_NCE | NAND_CLE | NAND_CTRL_CHANGE);
	s3c_nand_hwcontrol(0, 0x00, NAND_CTRL_CHANGE | NAND_NCE | NAND_ALE);
	s3c_nand_hwcontrol(0, 0x00, NAND_NCE | NAND_ALE);
	s3c_nand_hwcontrol(0, NAND_CMD_NONE, NAND_NCE | NAND_CTRL_CHANGE);
	s3c_nand_device_ready(0);

	tmp = readb(nand->IO_ADDR_R); /* Maf. ID */
	tmp = readb(nand->IO_ADDR_R); /* Device ID */

	for (i = 0; nand_flash_ids[i].name != NULL; i++) {
		if (tmp == nand_flash_ids[i].id) {
			type = &nand_flash_ids[i];
			break;
		}
	}

	nand->cellinfo = readb(nand->IO_ADDR_R);	/* 3rd byte */
	tmp = readb(nand->IO_ADDR_R);			/* 4th byte */

	if (!type->pagesize) {
		if (((nand->cellinfo >> 2) & 0x3) == 0) {
			nand_type = S3C_NAND_TYPE_SLC;
			nand->ecc.size = 512;
			nand->ecc.bytes	= 4;

			if ((1024 << (tmp & 0x3)) > 512) {
				nand->ecc.read_page = s3c_nand_read_page_1bit;
				nand->ecc.write_page = s3c_nand_write_page_1bit;
				nand->ecc.read_oob = s3c_nand_read_oob_1bit;
				nand->ecc.write_oob = s3c_nand_write_oob_1bit;
				nand->ecc.layout = &s3c_nand_oob_64;
			} else {
				nand->ecc.layout = &s3c_nand_oob_16;
			}
		} else {
			nand_type = S3C_NAND_TYPE_MLC;
			nand->options |= NAND_NO_SUBPAGE_WRITE;	/* NOP = 1 if MLC */
			nand->ecc.read_page = s3c_nand_read_page_4bit;
			nand->ecc.write_page = s3c_nand_write_page_4bit;
			nand->ecc.size = 512;
			nand->ecc.bytes = 8;	/* really 7 bytes */
			nand->ecc.layout = &s3c_nand_oob_mlc_64;
		}
	} else {
		nand_type = S3C_NAND_TYPE_SLC;
		nand->ecc.size = 512;
		nand->cellinfo = 0;
		nand->ecc.bytes = 4;
		nand->ecc.layout = &s3c_nand_oob_16;
	}
#else
	nand->ecc.mode = NAND_ECC_SOFT;
#endif
}
#endif /* (CONFIG_CMD_NAND) */
