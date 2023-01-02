// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 MediaTek Inc.
 */

#define pr_fmt(fmt) KBUILD_MODNAME "@(%s:%d) " fmt, __func__, __LINE__

#include <linux/gpio/consumer.h>
#include <linux/of_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/pinctrl/consumer.h>
#include <linux/platform_device.h>

#include "connv3_hw.h"
#include "connv3_pinctrl_mng.h"

#include "consys_reg_util.h"

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/
enum uart_gpio_type {
	GPIO_COEX_UTXD,
	GPIO_COEX_URXD,
	GPIO_SCP_WB_UTXD,
	GPIO_SCP_WB_URXD,
	GPIO_UART_MAX,
};

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/
/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/
static struct pinctrl *pinctrl_ptr = NULL;

/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/
int connv3_plt_pinctrl_init_mt6985(struct platform_device *pdev);
int connv3_plt_pinctrl_deinit_mt6985(void);
int connv3_plt_pinctrl_setup_pre_mt6985(void);
int connv3_plt_pinctrl_setup_done_mt6985(void);
int connv3_plt_pinctrl_remove_mt6985(void);

const struct connv3_platform_pinctrl_ops g_connv3_platform_pinctrl_ops_mt6985 = {
	.pinctrl_init = connv3_plt_pinctrl_init_mt6985,
	.pinctrl_deinit = connv3_plt_pinctrl_deinit_mt6985,
	.pinctrl_setup_pre = connv3_plt_pinctrl_setup_pre_mt6985,
	.pinctrl_setup_done = connv3_plt_pinctrl_setup_done_mt6985,
	.pinctrl_remove = connv3_plt_pinctrl_remove_mt6985,
	.pinctrl_ext_32k_ctrl = NULL,
};

#if 0
static int _drv_map(unsigned int drv)
{
	const int drv_map[] = {2, 4, 6, 8, 10, 12, 14, 16};

	if (drv >= 8)
		return 0;
	return drv_map[drv];
}
#endif

static void _dump_uart_gpio_state(char* tag)
{

}

static int connv3_plt_pinctrl_initial_state(void)
{
	struct pinctrl_state *pinctrl_init;
	int ret;

	if (IS_ERR(pinctrl_ptr)) {
		pr_err("[%s] fail to get connv3 pinctrl", __func__);
	} else {
		pinctrl_init = pinctrl_lookup_state(
				pinctrl_ptr, "connsys_combo_gpio_init");
		if (!IS_ERR(pinctrl_init)) {
			ret = pinctrl_select_state(pinctrl_ptr, pinctrl_init);
			if (ret)
				pr_err("[%s] pinctrl init fail, %d", __func__, ret);
		} else {
			pr_err("[%s] fail to get \"connsys_combo_gpio_init\"",  __func__);
		}
	}

	_dump_uart_gpio_state("init");
	return 0;
}

int connv3_plt_pinctrl_init_mt6985(struct platform_device *pdev)
{
	pinctrl_ptr = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR(pinctrl_ptr)) {
		pr_err("[%s] fail to get connv3 pinctrl", __func__);
	}

	connv3_plt_pinctrl_initial_state();

	return 0;
}

int connv3_plt_pinctrl_deinit_mt6985(void)
{
	return 0;
}

int connv3_plt_pinctrl_setup_pre_mt6985(void)
{
	struct pinctrl_state *pinctrl_pre_on;
	int ret;

	_dump_uart_gpio_state("pre before");

	if (IS_ERR(pinctrl_ptr)) {
		pr_err("[%s] fail to get connv3 pinctrl", __func__);
	} else {
		pinctrl_pre_on = pinctrl_lookup_state(
				pinctrl_ptr, "connsys_combo_gpio_pre_on");
		if (!IS_ERR(pinctrl_pre_on)) {
			ret = pinctrl_select_state(pinctrl_ptr, pinctrl_pre_on);
			if (ret)
				pr_err("[%s] pinctrl pre on fail, %d", __func__, ret);
		} else {
			pr_err("[%s] fail to get \"connsys_combo_gpio_pre_on\"",  __func__);
		}
	}
	_dump_uart_gpio_state("pre after");

	return 0;
}

int connv3_plt_pinctrl_setup_done_mt6985(void)
{
	struct pinctrl_state *pinctrl_on;
	int ret;

	if (IS_ERR(pinctrl_ptr)) {
		pr_err("[%s] fail to get connv3 pinctrl", __func__);
	} else {
		pinctrl_on = pinctrl_lookup_state(
				pinctrl_ptr, "connsys_combo_gpio_on");
		if (!IS_ERR(pinctrl_on)) {
			ret = pinctrl_select_state(pinctrl_ptr, pinctrl_on);
			if (ret)
				pr_err("[%s] pinctrl on fail, %d", __func__, ret);
		} else {
			pr_err("[%s] fail to get \"connsys_combo_gpio_on\"",  __func__);
		}
	}
	_dump_uart_gpio_state("setup done");
	return 0;
}

int connv3_plt_pinctrl_remove_mt6985(void)
{
	connv3_plt_pinctrl_initial_state();
	return 0;
}

