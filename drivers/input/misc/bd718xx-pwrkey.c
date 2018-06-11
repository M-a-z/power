// SPDX-License-Identifier: GPL-2.0
// Copyright (C) 2018 ROHM Semiconductors
// bd718xx-pwrbtn.c -- ROHM BD71837MWV and BD71847 power button driver

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mfd/bd71837.h>


struct bd718xx_pwrkey {
	struct input_dev *idev;
	struct bd71837 *mfd;
	int irq;
};

static irqreturn_t button_irq(int irq, void *_priv)
{
	struct input_dev *idev = (struct input_dev *)_priv;
	input_report_key(idev, KEY_POWER, 1);
	input_sync(idev);
	input_report_key(idev, KEY_POWER, 0);
	input_sync(idev);

	return IRQ_HANDLED;
}

static struct regmap *pwr_regmap = NULL;

static void bd718xx_power_off(void)
{

	struct config_reg {
		uint8_t reg;
		uint8_t mask;
		uint8_t val;
	};
	int i;
	struct config_reg pwroff_seq[] = {
		{
			/* Go to ready state after poweroff by register write */
			.reg = BD71837_REG_TRANS_COND1,
			.mask = BD718XX_SWRESET_POWEROFF_MASK,
			.val = BD718XX_POWOFF_TO_RDY
		}, {
			/* Select PWRON_B Short Push and Long Push as
			 * READY ==> SNVS
			 */
			.reg = BD71837_REG_TRANS_COND0,
			.mask = BD718XX_RDY_TO_SNVS_MASK,
			.val = BD718XX_PWR_TRIG_KEY_L <<
			       BD718XX_RDY_TO_SNVS_SIFT |
			       BD718XX_PWR_TRIG_KEY_S <<
			       BD718XX_RDY_TO_SNVS_SIFT
		}, {
			/* Use plain VSYS_UVLO as
			 * READY ==> SNVS
			.reg = BD71837_REG_TRANS_COND0,
			.mask = BD718XX_SNVS_TO_RUN_MASK,
			.val = BD718XX_PWR_TRIG_VSYS_UVLO <<
			       BD718XX_SNVS_TO_RUN_SIFT
			 */
		}, {
			.reg = BD71837_REG_SWRESET,
			.mask = BD71837_SWRESET_TYPE_MASK,
			.val = BD71837_SWRESET_TYPE_COLD
		}, {
			.reg = BD71837_REG_SWRESET,
			.mask = BD71837_SWRESET_RESET_MASK,
			.val = BD71837_SWRESET_RESET
		}
	};

	pr_info("poweroff handler called\n");
	pr_info("poweroff handler called\n");
	pr_info("poweroff handler called\n");
	pr_info("poweroff handler called\n");
	pr_info("poweroff handler called\n");

	for (i=0; i<ARRAY_SIZE(pwroff_seq); i++)
		regmap_update_bits(pwr_regmap, pwroff_seq[i].reg,
				   pwroff_seq[i].mask, pwroff_seq[i].val);
}
static void (*old_poff)(void);

static void set_power_off(struct bd718xx_pwrkey *pk)
{
	old_poff = pm_power_off;
	pm_power_off = bd718xx_power_off;
	pwr_regmap = pk->mfd->regmap;
	pr_info("YaY! I have the POWER - BWAHAHAHAHahahahahaaa...\n");
}
static int bd718xx_pwr_btn_remove(struct platform_device *pdev)
{
	if(pwr_regmap)
		pm_power_off = old_poff;
	return 0;
}

static int bd718xx_pwr_btn_probe(struct platform_device *pdev)
{
	int err = -ENOMEM;
	struct bd718xx_pwrkey *pk;

	pr_info("%s\n",__func__);

	pk = devm_kzalloc(&pdev->dev, sizeof(*pk), GFP_KERNEL);
	if (!pk)
		goto err_out;

	pk->mfd = dev_get_drvdata(pdev->dev.parent);

	pk->idev = devm_input_allocate_device(&pdev->dev);
	if (!pk->idev)
		goto err_out;

	pk->idev->name = "bd718xx-pwrkey";
	pk->idev->phys = "bd718xx-pwrkey/input0";
	pk->idev->dev.parent = &pdev->dev;

	input_set_capability(pk->idev, EV_KEY, KEY_POWER);

	err = platform_get_irq_byname(pdev, "pwr-btn-s");
	if (err<0)
		goto err_out;

	pk->irq = err;
	err = devm_request_threaded_irq(&pdev->dev, pk->irq, NULL, &button_irq,
					0, "bd718xx-pwrkey", pk);
	if (err)
		goto err_out;

	platform_set_drvdata(pdev, pk);
	err = regmap_update_bits(pk->mfd->regmap,
				 BD71837_REG_PWRONCONFIG0,
				 BD718XX_PWRBTN_SHORT_PRESS_MASK,
				 BD718XX_PWRBTN_SHORT_PRESS_10MS);
	if (err)
		goto err_out;

	err = input_register_device(pk->idev);
	pr_info("My node :%p\n",pdev->dev.of_node);
	if(pdev->dev.of_node)
		pr_info("node name :%s\n",(pdev->dev.of_node->name)?pdev->dev.of_node->name:"unknown");

	if (of_device_is_system_power_controller(pdev->dev.of_node))
		set_power_off(pk);

err_out:

	return err;
}

#ifdef CONFIG_OF
static const struct of_device_id bd718xx_pwr_key_match[] = {
	{ .compatible = "rohm,bd718xx-pwrkey" },
	{},
};
MODULE_DEVICE_TABLE(of, bd718xx_pwr_key_match);
#endif


static struct platform_driver bd718xx_pwr_btn_driver = {
	.probe	= bd718xx_pwr_btn_probe,
	.remove = bd718xx_pwr_btn_remove,
	.driver = {
		.name	= "bd718xx-pwrkey",
		.of_match_table = of_match_ptr(bd718xx_pwr_key_match),
	},
};
module_platform_driver(bd718xx_pwr_btn_driver);
MODULE_DESCRIPTION("Power button driver for buttons connected to ROHM bd71837/bd71847 PMIC");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matti Vaittinen <matti.vaittinen@fi.rohmeurope.com>");

