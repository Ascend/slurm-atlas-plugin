/*****************************************************************************\
 *  npu.c - driver for npu plugin
 *****************************************************************************
 *
 *  Copyright (C) 2020 Computer Center, Peking University
 *  *  Produced at Computer Center, Peking University.
 *  *  Written by Yinping Ma <mayinping@pku.edu.cn>, Chun Fan <fanchun@pku.edu.cn>.
 *
 *  This is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Slurm; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
 *
\*****************************************************************************/

#include <dlfcn.h>

#include "src/common/npu.h"
#include "src/common/plugin.h"

/* Gres symbols provided by the plugin */
typedef struct slurm_ops {
	void    (*reconfig)		(void);
	List	(*get_system_npu_list) 	(node_config_load_t *node_conf);
	void	(*step_hardware_init)	(bitstr_t *usable_npus,
					 char *tres_freq);
	void	(*step_hardware_fini)	(void);
	char   *(*test_cpu_conv)	(char *cpu_range);
} slurm_ops_t;

/*
 * These strings must be kept in the same order as the fields
 * declared for slurm_ops_t.
 */
static const char *syms[] = {
	"npu_p_reconfig",
	"npu_p_get_system_npu_list",
	"npu_p_step_hardware_init",
	"npu_p_step_hardware_fini",
	"npu_p_test_cpu_conv",
};

/* Local variables */
static slurm_ops_t ops;
static plugin_context_t *g_context = NULL;
static pthread_mutex_t g_context_lock =	PTHREAD_MUTEX_INITIALIZER;
static bool init_run = false;

/*
 *  Common function to dlopen() the appropriate npu libraries, and
 *   report back type needed.
 */
static char *_get_npu_type(void)
{
	/*
	 *  Here we are dlopening the npu .so to verify it exists on this node.
	 */
	uint32_t autodetect_types = gres_get_autodetect_types();

	if (autodetect_types & GRES_AUTODETECT_NPU) {
#ifdef HAVE_NPU
		if (!dlopen("libdrvdsmi_host.so", RTLD_NOW | RTLD_GLOBAL))
			fatal("We were configured with NPU functionality, but that lib wasn't found on the system.");
		else
			return "npu/generic";
#else
		fatal("We were configured to autodetect NPU functionality, but we weren't able to find that lib when Slurm was configured.");
#endif
	} 
	return "npu/generic";
}

/*
 * Initialize the GRES plugins.
 *
 * Returns a Slurm errno.
 */
extern int npu_plugin_init(void)
{
	int retval = SLURM_SUCCESS;
	char *plugin_type = "npu";
	char *type = NULL;

	if (init_run && g_context)
		return retval;

	slurm_mutex_lock(&g_context_lock);

	if (g_context)
		goto done;

	type = _get_npu_type();

	g_context = plugin_context_create(
		plugin_type, type, (void **)&ops, syms, sizeof(syms));

	if (!g_context) {
		error("cannot create %s context for %s", plugin_type, type);
		retval = SLURM_ERROR;
		goto done;
	}
	init_run = true;

done:
	slurm_mutex_unlock(&g_context_lock);

	return retval;
}

extern int npu_plugin_fini(void)
{
	int rc;

	if (!g_context)
		return SLURM_SUCCESS;

	slurm_mutex_lock(&g_context_lock);
	init_run = false;
	rc = plugin_context_destroy(g_context);
	g_context = NULL;
	slurm_mutex_unlock(&g_context_lock);

	return rc;
}

extern void npu_g_reconfig(void)
{
	if (npu_plugin_init() < 0)
		return;
	(*(ops.reconfig))();
}

extern List npu_g_get_system_npu_list(node_config_load_t *node_conf)
{
	if (npu_plugin_init() < 0)
		return NULL;

	return (*(ops.get_system_npu_list))(node_conf);
}

extern void npu_g_step_hardware_init(bitstr_t *usable_npus, char *tres_freq)
{
	if (npu_plugin_init() < 0)
		return;
	(*(ops.step_hardware_init))(usable_npus, tres_freq);
}

extern void npu_g_step_hardware_fini(void)
{
	if (npu_plugin_init() < 0)
		return;
	(*(ops.step_hardware_fini))();
}

extern char *npu_g_test_cpu_conv(char *cpu_range)
{
	if (npu_plugin_init() < 0)
		return NULL;
	return (*(ops.test_cpu_conv))(cpu_range);

}
