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
static struct pinctrl *g_pinctrl_ptr = NULL;
static struct pinctrl_state *g_ext32k_pin_state_init = NULL;
static struct pinctrl_state *g_ext32k_pin_state_on = NULL;
static struct pinctrl_state *g_ext32k_pin_state_off = NULL;

/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/
int connv3_plt_pinctrl_init_mt6985(struct platform_device *pdev);
int connv3_plt_pinctrl_deinit_mt6985(void);
int connv3_plt_pinctrl_setup_pre_mt6985(void);
int connv3_plt_pinctrl_setup_done_mt6985(void);
int connv3_plt_pinctrl_remove_mt6985(void);
int connv3_plt_pinctrl_ext_32k_ctrl(bool on);

const struct connv3_platform_pinctrl_ops g_connv3_platform_pinctrl_ops_mt6985 = {
	.pinctrl_init = connv3_plt_pinctrl_init_mt6985,
	.pinctrl_deinit = connv3_plt_pinctrl_deinit_mt6985,
	.pinctrl_setup_pre = connv3_plt_pinctrl_setup_pre_mt6985,
	.pinctrl_setup_done = connv3_plt_pinctrl_setup_done_mt6985,
	.pinctrl_remove = connv3_plt_pinctrl_remove_mt6985,
	.pinctrl_ext_32k_ctrl = connv3_plt_pinctrl_ext_32k_ctrl,
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

	if (IS_ERR_OR_NULL(g_pinctrl_ptr))
		pr_notice("[%s] fail to get connv3 pinctrl", __func__);
	else {
		pinctrl_init = pinctrl_lookup_state(
				g_pinctrl_ptr, "connsys_combo_gpio_init");
		if (!IS_ERR_OR_NULL(pinctrl_init)) {
			ret = pinctrl_select_state(g_pinctrl_ptr, pinctrl_init);
			if (ret)
				pr_notice("[%s] pinctrl init fail, %d", __func__, ret);
		} else
			pr_notice("[%s] fail to get \"connsys_combo_gpio_init\"",  __func__);
	}

	_dump_uart_gpio_state("init");
	return 0;
}

int connv3_plt_pinctrl_init_mt6985(struct platform_device *pdev)
{
	int ret;

	g_pinctrl_ptr = devm_pinctrl_get(&pdev->dev);
	if (IS_ERR_OR_NULL(g_pinctrl_ptr))
		pr_notice("[%s] fail to get connv3 pinctrl", __func__);
	else {
		g_ext32k_pin_state_init = pinctrl_lookup_state(
						g_pinctrl_ptr, "connsys_pin_ext32_en_default");
		g_ext32k_pin_state_on = pinctrl_lookup_state(
						g_pinctrl_ptr, "connsys_pin_ext32_en_set");
		g_ext32k_pin_state_off = pinctrl_lookup_state(
						g_pinctrl_ptr, "connsys_pin_ext32_en_clr");
		if (IS_ERR_OR_NULL(g_ext32k_pin_state_init) ||
		    IS_ERR_OR_NULL(g_ext32k_pin_state_on) ||
		    IS_ERR_OR_NULL(g_ext32k_pin_state_off))
			pr_notice("[%s] get ext32k fail: [%p][%p][%p]",
				__func__, g_ext32k_pin_state_init, g_ext32k_pin_state_on, g_ext32k_pin_state_off);
		else {
			ret = pinctrl_select_state(g_pinctrl_ptr, g_ext32k_pin_state_init);
			if (ret)
				pr_notice("[%s] ext32k init fail, %d", __func__, ret);
		}
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

	if (IS_ERR_OR_NULL(g_pinctrl_ptr)) {
		pr_notice("[%s] fail to get connv3 pinctrl", __func__);
	} else {
		pinctrl_pre_on = pinctrl_lookup_state(
				g_pinctrl_ptr, "connsys_combo_gpio_pre_on");
		if (!IS_ERR_OR_NULL(pinctrl_pre_on)) {
			ret = pinctrl_select_state(g_pinctrl_ptr, pinctrl_pre_on);
			if (ret)
				pr_notice("[%s] pinctrl pre on fail, %d", __func__, ret);
		} else
			pr_notice("[%s] fail to get \"connsys_combo_gpio_pre_on\"",  __func__);
	}
	_dump_uart_gpio_state("pre after");

	return 0;
}

int connv3_plt_pinctrl_setup_done_mt6985(void)
{
	struct pinctrl_state *pinctrl_on;
	int ret;

	if (IS_ERR_OR_NULL(g_pinctrl_ptr)) {
		pr_notice("[%s] fail to get connv3 pinctrl", __func__);
	} else {
		pinctrl_on = pinctrl_lookup_state(
				g_pinctrl_ptr, "connsys_combo_gpio_on");
		if (!IS_ERR_OR_NULL(pinctrl_on)) {
			ret = pinctrl_select_state(g_pinctrl_ptr, pinctrl_on);
			if (ret)
				pr_notice("[%s] pinctrl on fail, %d", __func__, ret);
		} else
			pr_notice("[%s] fail to get \"connsys_combo_gpio_on\"",  __func__);
	}
	_dump_uart_gpio_state("setup done");
	return 0;
}

int connv3_plt_pinctrl_remove_mt6985(void)
{
	connv3_plt_pinctrl_initial_state();
	return 0;
}

int connv3_plt_pinctrl_ext_32k_ctrl(bool on)
{
	int ret = 0;

	if (IS_ERR_OR_NULL(g_ext32k_pin_state_on) || IS_ERR_OR_NULL(g_ext32k_pin_state_off))
		ret = -1;
	else {
		if (on)
			ret = pinctrl_select_state(g_pinctrl_ptr, g_ext32k_pin_state_on);
		else
			ret = pinctrl_select_state(g_pinctrl_ptr, g_ext32k_pin_state_off);
	}

	if (ret)
		pr_notice("[%s][%d] ext32k control fail: ret=[%d], pin state=[%p][%p]",
			__func__, on, ret, g_ext32k_pin_state_on, g_ext32k_pin_state_off);

	return ret;
}
