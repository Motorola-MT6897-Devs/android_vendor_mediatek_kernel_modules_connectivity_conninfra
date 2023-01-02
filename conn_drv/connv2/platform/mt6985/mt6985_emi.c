// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 MediaTek Inc.
 */

#include "emi_mng.h"
#include "mt6985.h"
#include "mt6985_pos.h"

struct consys_platform_emi_ops g_consys_platform_emi_ops_mt6985 = {
	.consys_ic_emi_mpu_set_region_protection = NULL,
	.consys_ic_emi_set_remapping_reg = consys_emi_set_remapping_reg_mt6985,
	.consys_ic_emi_get_md_shared_emi = NULL,
};
