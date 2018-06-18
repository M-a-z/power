// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2018 ROHM Semiconductors
// bd718xx-reset.c -- ROHM BD71837MWV and BD71847 reset driver

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mfd/bd71837.h>
#include <linux/reboot.h>
#include <linux/device.h>

struct bd718xx_reset {
	struct bd71837 *mfd;
	struct notifier_block reset_handler;
	uint8_t reset_type;
};

static int bd718xx_reset_call(struct notifier_block *rhandle,
			 unsigned long mode, void *cmd)
{

	struct config_reg {
		uint8_t reg;
		uint8_t mask;
		uint8_t val;
	};
	int i;
	struct bd718xx_reset *r = container_of(rhandle, struct bd718xx_reset,
					       reset_handler);
	struct config_reg reset_seq[] = {
		{
			/* Go to ready state after poweroff by register write */
			.reg = BD71837_REG_TRANS_COND1,
			.mask = BD718XX_SWRESET_POWEROFF_MASK,
			.val = BD718XX_POWOFF_TO_RDY
		}, {
			.reg = BD71837_REG_SWRESET,
			.mask = BD71837_SWRESET_TYPE_MASK,
			.val = r->reset_type,
		}, {
			.reg = BD71837_REG_SWRESET,
			.mask = BD71837_SWRESET_RESET_MASK,
			.val = BD71837_SWRESET_RESET
		}
	};

	for (i=0; i<ARRAY_SIZE(reset_seq); i++)
		regmap_update_bits(r->mfd->regmap, reset_seq[i].reg,
				   reset_seq[i].mask, reset_seq[i].val);
	return NOTIFY_DONE;
}

struct delay_slot {
	unsigned int delay;
	uint8_t reg;
};

static int set_delay(struct bd718xx_reset *r, unsigned int delay)
{
	int rval = -EINVAL;
	int i=0;
	const struct delay_slot delay_limit[] = {
		{ .delay = 5, .reg = BD718XX_POWOFF_TIME_5MS },
		{ .delay = 10, .reg = BD718XX_POWOFF_TIME_10MS },
		{ .delay = 15, .reg = BD718XX_POWOFF_TIME_15MS },
		{ .delay = 10, .reg = BD718XX_POWOFF_TIME_20MS },
		{ .delay = 25, .reg = BD718XX_POWOFF_TIME_25MS },
		{ .delay = 30, .reg = BD718XX_POWOFF_TIME_30MS },
		{ .delay = 35, .reg = BD718XX_POWOFF_TIME_35MS },
		{ .delay = 40, .reg = BD718XX_POWOFF_TIME_40MS },
		{ .delay = 45, .reg = BD718XX_POWOFF_TIME_45MS },
		{ .delay = 50, .reg = BD718XX_POWOFF_TIME_50MS },
		{ .delay = 75, .reg = BD718XX_POWOFF_TIME_75MS },
		{ .delay = 100, .reg = BD718XX_POWOFF_TIME_100MS },
		{ .delay = 250, .reg = BD718XX_POWOFF_TIME_250MS },
		{ .delay = 500, .reg = BD718XX_POWOFF_TIME_500MS },
		{ .delay = 750, .reg = BD718XX_POWOFF_TIME_750MS },
		{ .delay = 1500, .reg = BD718XX_POWOFF_TIME_1500MS }
	};
	int amnt = ARRAY_SIZE(delay_limit);
	uint8_t reg = BD718XX_POWOFF_TIME_5MS;

/* Possible regval - duration :
 * 0001 = 10 ms
 * 0010 = 15 ms
 * 0011 = 20 ms
 * 0100 = 25 ms
 * 0101 = 30 ms
 * 0110 = 35 ms
 * 0111 = 40 ms
 * 1000 = 45 ms
 * 1001 = 50 ms
 * 1010 = 75 ms
 * 1011 = 100 ms
 * 1100 = 250 ms
 * 1101 = 500 ms
 * 1110 = 750 ms
 * 1111 = 1500 ms
 */
	if (r && delay <= 1500) {
		for (i = 0; i < amnt && delay_limit[i].delay <= delay; i++)
			reg = delay_limit[i].reg;
		rval = regmap_update_bits(r->mfd->regmap,
					  BD71837_REG_TRANS_COND1,
					  BD718XX_POWOFF_TIME_MASK, reg);
	}
	return rval;
}

static int bd718xx_reset_probe(struct platform_device *pdev)
{
	unsigned int delay = 0;
	int rval;
	bool soft_reset;
	struct bd718xx_reset *r;

	r = devm_kzalloc(&pdev->dev, sizeof(*r), GFP_KERNEL);
	if(!r)
		return -ENOMEM;

	r->mfd = dev_get_drvdata(pdev->dev.parent);
	r->reset_handler.notifier_call = &bd718xx_reset_call;
	r->reset_handler.priority = 129;
	r->reset_type = BD71837_SWRESET_TYPE_COLD;
	platform_set_drvdata(pdev, r);

	rval = of_property_read_u32(pdev->dev.parent->of_node, "rohm,reset-delay",
				    &delay);
	if (!rval)
		rval = set_delay(r, delay);

	if (!rval) {
		soft_reset = of_property_read_bool(pdev->dev.parent->of_node, "rohm,warm-reset");

		if (soft_reset) {
			if (delay)
				dev_warn(&pdev->dev, "can't set delay for warm reset\n");
			r->reset_type = BD71837_SWRESET_TYPE_WARM;
		}

		rval = register_restart_handler(&r->reset_handler);
	}

	return rval;
}

static int bd718xx_reset_remove(struct platform_device *pdev)
{
	struct bd718xx_reset *r = platform_get_drvdata(pdev);
	unregister_restart_handler(&r->reset_handler);
	return 0;
}

static struct platform_driver bd718xx_reset_driver = {
	.probe	= bd718xx_reset_probe,
	.remove = bd718xx_reset_remove,
	.driver = {
		.name	= "bd718xx-reset",
	},
};
module_platform_driver(bd718xx_reset_driver);
MODULE_DESCRIPTION("Power button driver for buttons connected to ROHM bd71837/bd71847 PMIC");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matti Vaittinen <matti.vaittinen@fi.rohmeurope.com>");


