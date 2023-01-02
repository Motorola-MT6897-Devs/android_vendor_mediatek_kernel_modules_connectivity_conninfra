// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 MediaTek Inc.
 */
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/regmap.h>
#include <linux/timer.h>

#include "osal.h"
#include "consys_hw.h"
#include "consys_reg_util.h"
#include "pmic_mng.h"
#include "mt6985_pmic.h"
#include "mt6985_consys_reg_offset.h"
#include "mt6985_pos.h"


static int consys_plt_pmic_get_from_dts_mt6985(struct platform_device*, struct conninfra_dev_cb*);
static int consys_plt_pmic_gps_power_ctrl_mt6985(unsigned int);
static int consys_plt_pmic_event_notifier_mt6985(unsigned int, unsigned int);


const struct consys_platform_pmic_ops g_consys_platform_pmic_ops_mt6985 = {
	.consys_pmic_get_from_dts = consys_plt_pmic_get_from_dts_mt6985,
	.consys_pmic_gps_power_ctrl = consys_plt_pmic_gps_power_ctrl_mt6985,
	.consys_pmic_event_notifier = consys_plt_pmic_event_notifier_mt6985,
};

int consys_plt_pmic_get_from_dts_mt6985(struct platform_device* dev, struct conninfra_dev_cb* cb)
{
	return 0;
}

int consys_plt_pmic_gps_power_ctrl_mt6985(unsigned int enable)
{
	return 0;
}

int consys_plt_pmic_event_notifier_mt6985(unsigned int id, unsigned int event)
{
	return 0;
}
