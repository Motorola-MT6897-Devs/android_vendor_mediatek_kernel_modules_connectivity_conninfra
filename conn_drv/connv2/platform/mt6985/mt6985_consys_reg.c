// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 MediaTek Inc.
 */

#include <linux/memblock.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>

#include "consys_reg_mng.h"
#include "consys_reg_util.h"
#include "osal.h"
#include "mt6985_consys_reg.h"
#include "mt6985_consys_reg_offset.h"
#include "mt6985_pos.h"
#include "mt6985_pos_gen.h"

static int consys_reg_init(struct platform_device *pdev);
static int consys_reg_deinit(void);
#if 0
static int consys_check_reg_readable(void);
static int consys_check_reg_readable_for_coredump(void);
static int __consys_check_reg_readable(int check_type);
static int consys_is_consys_reg(unsigned int addr);
static int consys_is_bus_hang(void);
static void consys_print_platform_debug(void);
#endif

struct consys_base_addr g_conn_reg_mt6985;

struct consys_reg_mng_ops g_dev_consys_reg_ops_mt6985 = {
	.consys_reg_mng_init = consys_reg_init,
	.consys_reg_mng_deinit = consys_reg_deinit,
#if 0
	.consys_reg_mng_check_reable = consys_check_reg_readable,
	.consys_reg_mng_check_reable_for_coredump = consys_check_reg_readable_for_coredump,
	.consys_reg_mng_is_bus_hang = consys_is_bus_hang,
	.consys_reg_mng_is_consys_reg = consys_is_consys_reg,
#endif
};

static const char* consys_base_addr_index_to_str[CONSYS_BASE_ADDR_MAX] = {
	"conn_cfg",
	"conn_host_csr_top",
	"conn_infra_sysram",
	"conn_infra_bus_cr",
	"conn_infra_rgu_on",
	"conn_wt_slp_ctl_reg",
	"conn_infra_cfg_on",
	"conn_infra_bus_cr_on",
	"conn_infra_off_debug_ctrl_ao",
	"conn_infra_clkgen_top",
	"conn_rf_spi_mst_reg",
	"ifrbus_ao_reg",
	"spm",
	"conn_semaphore",
	"conn_afe_ctl",
	"SRCLKENRC",
};

int consys_reg_init(struct platform_device *pdev)
{
	int ret = -1;
	struct device_node *node = NULL;
	struct consys_reg_base_addr *base_addr = NULL;
	struct resource res;
	int flag, i = 0;

	node = pdev->dev.of_node;
	pr_info("[%s] node=[%p]\n", __func__, node);
	if (node) {
		for (i = 0; i < CONSYS_BASE_ADDR_MAX; i++) {
			base_addr = &g_conn_reg_mt6985.reg_base_addr[i];

			ret = of_address_to_resource(node, i, &res);
			if (ret) {
				pr_err("Get Reg Index(%d-%s) failed",
						i, consys_base_addr_index_to_str[i]);
				continue;
			}

			base_addr->phy_addr = res.start;
			base_addr->vir_addr =
				(unsigned long) of_iomap(node, i);
			of_get_address(node, i, &(base_addr->size), &flag);

			pr_info("Get Index(%d-%s) phy(0x%zx) baseAddr=(0x%zx) size=(0x%zx)",
				i, consys_base_addr_index_to_str[i], base_addr->phy_addr,
				base_addr->vir_addr, base_addr->size);
		}

	} else {
		pr_err("[%s] can't find CONSYS compatible node\n", __func__);
		return ret;
	}

	return 0;
}

static int consys_reg_deinit(void)
{
	int i = 0;

	for (i = 0; i < CONSYS_BASE_ADDR_MAX; i++) {
		if (g_conn_reg_mt6985.reg_base_addr[i].vir_addr) {
			pr_info("[%d] Unmap %s (0x%zx)",
				i, consys_base_addr_index_to_str[i],
				g_conn_reg_mt6985.reg_base_addr[i].vir_addr);
			iounmap((void __iomem*)g_conn_reg_mt6985.reg_base_addr[i].vir_addr);
			g_conn_reg_mt6985.reg_base_addr[i].vir_addr = 0;
		}
	}

	return 0;
}
