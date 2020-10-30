/*****************************************************************************\
 *  npu.h - driver for npu plugin
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

#ifndef _NPU_PLUGIN_H
#define _NPU_PLUGIN_H

#include "slurm/slurm.h"
#include "src/common/gres.h"

extern int npu_plugin_init(void);
extern int npu_plugin_fini(void);
extern void npu_g_reconfig(void);
extern List npu_g_get_system_npu_list(node_config_load_t *node_conf);
extern void npu_g_step_hardware_init(bitstr_t *usable_gpus, char *tres_freq);
extern void npu_g_step_hardware_fini(void);
extern char *npu_g_test_cpu_conv(char *cpu_range);

#endif /* !_NPU_PLUGIN_H */
