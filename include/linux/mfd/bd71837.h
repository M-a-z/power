/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (C) 2018 ROHM Semiconductors */

/*
 * ROHM BD71837MWV header file
 */

#ifndef __LINUX_MFD_BD71837_H__
#define __LINUX_MFD_BD71837_H__

#include <linux/regmap.h>

enum {
	BD71837_BUCK1	=	0,
	BD71837_BUCK2,
	BD71837_BUCK3,
	BD71837_BUCK4,
	BD71837_BUCK5,
	BD71837_BUCK6,
	BD71837_BUCK7,
	BD71837_BUCK8,
	BD71837_LDO1,
	BD71837_LDO2,
	BD71837_LDO3,
	BD71837_LDO4,
	BD71837_LDO5,
	BD71837_LDO6,
	BD71837_LDO7,
	BD71837_REGULATOR_CNT,
};

#define BD71837_BUCK1_VOLTAGE_NUM	0x40
#define BD71837_BUCK2_VOLTAGE_NUM	0x40
#define BD71837_BUCK3_VOLTAGE_NUM	0x40
#define BD71837_BUCK4_VOLTAGE_NUM	0x40

#define BD71837_BUCK5_VOLTAGE_NUM	0x08
#define BD71837_BUCK6_VOLTAGE_NUM	0x04
#define BD71837_BUCK7_VOLTAGE_NUM	0x08
#define BD71837_BUCK8_VOLTAGE_NUM	0x40

#define BD71837_LDO1_VOLTAGE_NUM	0x04
#define BD71837_LDO2_VOLTAGE_NUM	0x02
#define BD71837_LDO3_VOLTAGE_NUM	0x10
#define BD71837_LDO4_VOLTAGE_NUM	0x10
#define BD71837_LDO5_VOLTAGE_NUM	0x10
#define BD71837_LDO6_VOLTAGE_NUM	0x10
#define BD71837_LDO7_VOLTAGE_NUM	0x10

enum {
	BD71837_REG_REV                = 0x00,
	BD71837_REG_SWRESET            = 0x01,
	BD71837_REG_I2C_DEV            = 0x02,
	BD71837_REG_PWRCTRL0           = 0x03,
	BD71837_REG_PWRCTRL1           = 0x04,
	BD71837_REG_BUCK1_CTRL         = 0x05,
	BD71837_REG_BUCK2_CTRL         = 0x06,
	BD71837_REG_BUCK3_CTRL         = 0x07,
	BD71837_REG_BUCK4_CTRL         = 0x08,
	BD71837_REG_BUCK5_CTRL         = 0x09,
	BD71837_REG_BUCK6_CTRL         = 0x0A,
	BD71837_REG_BUCK7_CTRL         = 0x0B,
	BD71837_REG_BUCK8_CTRL         = 0x0C,
	BD71837_REG_BUCK1_VOLT_RUN     = 0x0D,
	BD71837_REG_BUCK1_VOLT_IDLE    = 0x0E,
	BD71837_REG_BUCK1_VOLT_SUSP    = 0x0F,
	BD71837_REG_BUCK2_VOLT_RUN     = 0x10,
	BD71837_REG_BUCK2_VOLT_IDLE    = 0x11,
	BD71837_REG_BUCK3_VOLT_RUN     = 0x12,
	BD71837_REG_BUCK4_VOLT_RUN     = 0x13,
	BD71837_REG_BUCK5_VOLT         = 0x14,
	BD71837_REG_BUCK6_VOLT         = 0x15,
	BD71837_REG_BUCK7_VOLT         = 0x16,
	BD71837_REG_BUCK8_VOLT         = 0x17,
	BD71837_REG_LDO1_VOLT          = 0x18,
	BD71837_REG_LDO2_VOLT          = 0x19,
	BD71837_REG_LDO3_VOLT          = 0x1A,
	BD71837_REG_LDO4_VOLT          = 0x1B,
	BD71837_REG_LDO5_VOLT          = 0x1C,
	BD71837_REG_LDO6_VOLT          = 0x1D,
	BD71837_REG_LDO7_VOLT          = 0x1E,
	BD71837_REG_TRANS_COND0        = 0x1F,
	BD71837_REG_TRANS_COND1        = 0x20,
	BD71837_REG_VRFAULTEN          = 0x21,
	BD71837_REG_MVRFLTMASK0        = 0x22,
	BD71837_REG_MVRFLTMASK1        = 0x23,
	BD71837_REG_MVRFLTMASK2        = 0x24,
	BD71837_REG_RCVCFG             = 0x25,
	BD71837_REG_RCVNUM             = 0x26,
	BD71837_REG_PWRONCONFIG0       = 0x27,
	BD71837_REG_PWRONCONFIG1       = 0x28,
	BD71837_REG_RESETSRC           = 0x29,
	BD71837_REG_MIRQ               = 0x2A,
	BD71837_REG_IRQ                = 0x2B,
	BD71837_REG_IN_MON             = 0x2C,
	BD71837_REG_POW_STATE          = 0x2D,
	BD71837_REG_OUT32K             = 0x2E,
	BD71837_REG_REGLOCK            = 0x2F,
	BD71837_REG_OTPVER             = 0xFF,
	BD71837_MAX_REGISTER           = 0x100,
};

/* register write induced reset settings */

/* even though the bit zero is not SWRESET type we still want to write zero
 * to it when changing type. Biz zero is 'SWRESET' trigger bit and if we
 * write 1 to it we will trigger the action. So always write 0 to it when
 * changning SWRESET action - no matter what we read from it.
 */
#define BD71837_SWRESET_TYPE_MASK 7
#define BD71837_SWRESET_TYPE_DISABLED 0
#define BD71837_SWRESET_TYPE_COLD 4
#define BD71837_SWRESET_TYPE_WARM 6

#define BD71837_SWRESET_RESET_MASK 1
#define BD71837_SWRESET_RESET 1

/* Poweroff state transition conditions */

#define BD718XX_ON_REQ_POWEROFF_MASK 1
#define BD718XX_SWRESET_POWEROFF_MASK 2
#define BD718XX_WDOG_POWEROFF_MASK 4
#define BD718XX_KEY_L_POWEROFF_MASK 8

#define BD718XX_POWOFF_TO_SNVS 0
#define BD718XX_POWOFF_TO_RDY 0xF

#define BD718XX_POWOFF_TIME_MASK 0xF0
enum {
	BD718XX_POWOFF_TIME_5MS = 0,
	BD718XX_POWOFF_TIME_10MS,
	BD718XX_POWOFF_TIME_15MS,
	BD718XX_POWOFF_TIME_20MS,
	BD718XX_POWOFF_TIME_25MS,
	BD718XX_POWOFF_TIME_30MS,
	BD718XX_POWOFF_TIME_35MS,
	BD718XX_POWOFF_TIME_40MS,
	BD718XX_POWOFF_TIME_45MS,
	BD718XX_POWOFF_TIME_50MS,
	BD718XX_POWOFF_TIME_75MS,
	BD718XX_POWOFF_TIME_100MS,
	BD718XX_POWOFF_TIME_250MS,
	BD718XX_POWOFF_TIME_500MS,
	BD718XX_POWOFF_TIME_750MS,
	BD718XX_POWOFF_TIME_1500MS
};

/* Poweron sequence state transition conditions */

#define BD718XX_RDY_TO_SNVS_MASK 0xF
#define BD718XX_SNVS_TO_RUN_MASK 0xF0

#define BD718XX_PWR_TRIG_KEY_L 1
#define BD718XX_PWR_TRIG_KEY_S 2
#define BD718XX_PWR_TRIG_PMIC_ON 4
#define BD718XX_PWR_TRIG_VSYS_UVLO 8
#define BD718XX_RDY_TO_SNVS_SIFT 0
#define BD718XX_SNVS_TO_RUN_SIFT 4

/* Timout value for detecting short press */

#define BD718XX_PWRBTN_SHORT_PRESS_MASK 0xF

enum {
	BD718XX_PWRBTN_SHORT_PRESS_10MS = 0,
	BD718XX_PWRBTN_SHORT_PRESS_500MS,
	BD718XX_PWRBTN_SHORT_PRESS_1000MS,
	BD718XX_PWRBTN_SHORT_PRESS_1500MS,
	BD718XX_PWRBTN_SHORT_PRESS_2000MS,
	BD718XX_PWRBTN_SHORT_PRESS_2500MS,
	BD718XX_PWRBTN_SHORT_PRESS_3000MS,
	BD718XX_PWRBTN_SHORT_PRESS_3500MS,
	BD718XX_PWRBTN_SHORT_PRESS_4000MS,
	BD718XX_PWRBTN_SHORT_PRESS_4500MS,
	BD718XX_PWRBTN_SHORT_PRESS_5000MS,
	BD718XX_PWRBTN_SHORT_PRESS_5500MS,
	BD718XX_PWRBTN_SHORT_PRESS_6000MS,
	BD718XX_PWRBTN_SHORT_PRESS_6500MS,
	BD718XX_PWRBTN_SHORT_PRESS_7000MS,
	BD718XX_PWRBTN_SHORT_PRESS_7500MS
};

#define REGLOCK_PWRSEQ	0x1
#define REGLOCK_VREG	0x10

/* Generic BUCK control masks */
#define BD71837_BUCK_SEL	0x02
#define BD71837_BUCK_EN		0x01
#define BD71837_BUCK_RUN_ON	0x04

/* Generic LDO masks */
#define BD71837_LDO_SEL		0x80
#define BD71837_LDO_EN		0x40

/* BD71837 BUCK ramp rate CTRL reg bits */
#define BUCK_RAMPRATE_MASK	0xC0
#define BUCK_RAMPRATE_10P00MV	0x0
#define BUCK_RAMPRATE_5P00MV	0x1
#define BUCK_RAMPRATE_2P50MV	0x2
#define BUCK_RAMPRATE_1P25MV	0x3

/* BD71837_REG_BUCK1_VOLT_RUN bits */
#define BUCK1_RUN_MASK		0x3F
#define BUCK1_RUN_DEFAULT	0x14

/* BD71837_REG_BUCK1_VOLT_SUSP bits */
#define BUCK1_SUSP_MASK		0x3F
#define BUCK1_SUSP_DEFAULT	0x14

/* BD71837_REG_BUCK1_VOLT_IDLE bits */
#define BUCK1_IDLE_MASK		0x3F
#define BUCK1_IDLE_DEFAULT	0x14

/* BD71837_REG_BUCK2_VOLT_RUN bits */
#define BUCK2_RUN_MASK		0x3F
#define BUCK2_RUN_DEFAULT	0x1E

/* BD71837_REG_BUCK2_VOLT_IDLE bits */
#define BUCK2_IDLE_MASK		0x3F
#define BUCK2_IDLE_DEFAULT	0x14

/* BD71837_REG_BUCK3_VOLT_RUN bits */
#define BUCK3_RUN_MASK		0x3F
#define BUCK3_RUN_DEFAULT	0x1E

/* BD71837_REG_BUCK4_VOLT_RUN bits */
#define BUCK4_RUN_MASK		0x3F
#define BUCK4_RUN_DEFAULT	0x1E

/* BD71837_REG_BUCK5_VOLT bits */
#define BUCK5_MASK		0x07
#define BUCK5_DEFAULT		0x02

/* BD71837_REG_BUCK6_VOLT bits */
#define BUCK6_MASK		0x03
#define BUCK6_DEFAULT		0x03

/* BD71837_REG_BUCK7_VOLT bits */
#define BUCK7_MASK		0x07
#define BUCK7_DEFAULT		0x03

/* BD71837_REG_BUCK8_VOLT bits */
#define BUCK8_MASK		0x3F
#define BUCK8_DEFAULT		0x1E

/* BD71837_REG_IRQ bits */
#define IRQ_SWRST		0x40
#define IRQ_PWRON_S		0x20
#define IRQ_PWRON_L		0x10
#define IRQ_PWRON		0x08
#define IRQ_WDOG		0x04
#define IRQ_ON_REQ		0x02
#define IRQ_STBY_REQ		0x01

/* BD71837_REG_OUT32K bits */
#define BD71837_OUT32K_EN	0x01

/* BD71837 gated clock rate */
#define BD71837_CLK_RATE 32768

/* BD71837 irqs */
enum {
	BD71837_INT_STBY_REQ,
	BD71837_INT_ON_REQ,
	BD71837_INT_WDOG,
	BD71837_INT_PWRBTN,
	BD71837_INT_PWRBTN_L,
	BD71837_INT_PWRBTN_S,
	BD71837_INT_SWRST
};

/* BD71837 interrupt masks */
#define BD71837_INT_SWRST_MASK		0x40
#define BD71837_INT_PWRBTN_S_MASK	0x20
#define BD71837_INT_PWRBTN_L_MASK	0x10
#define BD71837_INT_PWRBTN_MASK		0x8
#define BD71837_INT_WDOG_MASK		0x4
#define BD71837_INT_ON_REQ_MASK		0x2
#define BD71837_INT_STBY_REQ_MASK	0x1

/* BD71837_REG_LDO1_VOLT bits */
#define LDO1_MASK		0x03

/* BD71837_REG_LDO1_VOLT bits */
#define LDO2_MASK		0x20

/* BD71837_REG_LDO3_VOLT bits */
#define LDO3_MASK		0x0F

/* BD71837_REG_LDO4_VOLT bits */
#define LDO4_MASK		0x0F

/* BD71837_REG_LDO5_VOLT bits */
#define LDO5_MASK		0x0F

/* BD71837_REG_LDO6_VOLT bits */
#define LDO6_MASK		0x0F

/* BD71837_REG_LDO7_VOLT bits */
#define LDO7_MASK		0x0F

struct bd71837;
struct bd71837_pmic;
struct bd71837_clk;
struct bd718xx_pwrkey;

/*
 * Board platform data may be used to initialize regulators.
 */

struct bd71837_board {
	struct regulator_init_data *init_data[BD71837_REGULATOR_CNT];
	int	gpio_intr;
	int	irq_base;
};

struct bd71837 {
	struct device *dev;
	struct i2c_client *i2c_client;
	struct regmap *regmap;
	unsigned long int id;

	int chip_irq;
	struct regmap_irq_chip_data *irq_data;

	struct bd71837_pmic *pmic;
	struct bd71837_clk *clk;
	struct bd718xx_pwrkey *pwrkey;

	struct bd71837_board *of_plat_data;
};

/*
 * bd71837 sub-driver chip access routines
 */

static inline int bd71837_reg_read(struct bd71837 *bd71837, u8 reg)
{
	int r, val;

	r = regmap_read(bd71837->regmap, reg, &val);
	if (r < 0)
		return r;
	return val;
}

static inline int bd71837_reg_write(struct bd71837 *bd71837, u8 reg,
				    unsigned int val)
{
	return regmap_write(bd71837->regmap, reg, val);
}

static inline int bd71837_set_bits(struct bd71837 *bd71837, u8 reg, u8 mask)
{
	return regmap_update_bits(bd71837->regmap, reg, mask, mask);
}

static inline int bd71837_clear_bits(struct bd71837 *bd71837, u8 reg,
				     u8 mask)
{
	return regmap_update_bits(bd71837->regmap, reg, mask, 0);
}

static inline int bd71837_update_bits(struct bd71837 *bd71837, u8 reg,
				      u8 mask, u8 val)
{
	return regmap_update_bits(bd71837->regmap, reg, mask, val);
}

#endif /* __LINUX_MFD_BD71837_H__ */
