/*
**************************************************************************
 * Rockchip driver for CIF ISP 1.1
 * (Based on Intel driver for sofiaxxx)
 *
 * Copyright (C) 2015 Intel Mobile Communications GmbH
 * Copyright (C) 2016 Fuzhou Rockchip Electronics Co., Ltd.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
**************************************************************************
 */

#ifndef PLATFORM_CAMERA_MODULE_H
#define PLATFORM_CAMERA_MODULE_H
#include <linux/videodev2.h>

#define PLTFRM_CAMERA_MODULE_REG_TYPE_DATA 0
#define PLTFRM_CAMERA_MODULE_REG_TYPE_TIMEOUT 1
#define PLTFRM_CAMERA_MODULE_REG_TYPE_DATA_SINGLE 2

#define PLTFRM_CAMERA_MODULE_MIRROR_BIT 0
#define PLTFRM_CAMERA_MODULE_FLIP_BIT 1
#define PLTFRM_CAMERA_MODULE_IS_MIRROR(a) \
	(a & (1 << PLTFRM_CAMERA_MODULE_MIRROR_BIT))
#define PLTFRM_CAMERA_MODULE_IS_FLIP(a) \
	(a & (1 << PLTFRM_CAMERA_MODULE_FLIP_BIT))

extern const char *PLTFRM_CAMERA_MODULE_PIN_PD;
extern const char *PLTFRM_CAMERA_MODULE_PIN_PWR;
extern const char *PLTFRM_CAMERA_MODULE_PIN_FLASH;
extern const char *PLTFRM_CAMERA_MODULE_PIN_TORCH;
extern const char *PLTFRM_CAMERA_MODULE_PIN_RESET;
extern const char *PLTFRM_CAMERA_MODULE_PIN_VSYNC;

enum pltfrm_camera_module_pin_state {
	PLTFRM_CAMERA_MODULE_PIN_STATE_INACTIVE = 0,
	PLTFRM_CAMERA_MODULE_PIN_STATE_ACTIVE = 1
};

struct pltfrm_camera_module_reg {
	u32 flag;
	u16 reg;
	u16 val;
};

struct pltfrm_camera_module_reg_table {
	u32 reg_table_num_entries;
	struct pltfrm_camera_module_reg *reg_table;
};

int pltfrm_camera_module_set_pm_state(
	struct v4l2_subdev *sd,
	int on);

int pltfrm_camera_module_set_pin_state(
	struct v4l2_subdev *sd,
	const char *pin,
	enum pltfrm_camera_module_pin_state state);

int pltfrm_camera_module_get_pin_state(
	struct v4l2_subdev *sd,
	const char *pin);

int pltfrm_camera_module_s_power(
	struct v4l2_subdev *sd,
	int on);

int pltfrm_camera_module_patch_config(
	struct v4l2_subdev *sd,
	struct v4l2_mbus_framefmt *frm_fmt,
	struct v4l2_subdev_frame_interval *frm_intrvl);

struct v4l2_subdev *pltfrm_camera_module_get_af_ctrl(
	struct v4l2_subdev *sd);

struct v4l2_subdev *pltfrm_camera_module_get_ircut_ctrl(
	struct v4l2_subdev *sd);

struct v4l2_subdev *pltfrm_camera_module_get_fl_ctrl(
	struct v4l2_subdev *sd);

char *pltfrm_camera_module_get_flash_driver_name(
	struct v4l2_subdev *sd);

int pltfrm_camera_module_init(
	struct v4l2_subdev *sd,
	void **pldata);

void pltfrm_camera_module_release(
	struct v4l2_subdev *sd);

int pltfrm_camera_module_read_reg(struct v4l2_subdev *sd,
	u16 data_length,
	u16 reg,
	u32 *val);

int pltfrm_superpix_camera_module_read_reg(struct v4l2_subdev *sd,
	u16 data_length,
	u8 reg,
	u8 *val);


int pltfrm_camera_module_write_reg(struct v4l2_subdev *sd,
	u16 reg, u8 val);

int pltfrm_camera_module_write_reglist(
	struct v4l2_subdev *sd,
	const struct pltfrm_camera_module_reg reglist[],
	int len);

long pltfrm_camera_module_ioctl(struct v4l2_subdev *sd,
	unsigned int cmd,
	void *arg);

const char *pltfrm_dev_string(struct v4l2_subdev *sd);

int pltfrm_camera_module_get_flip_mirror(
	struct v4l2_subdev *sd);

#define pltfrm_camera_module_pr_debug(dev, fmt, arg...) \
	pr_debug("%s.%s: " fmt, \
		pltfrm_dev_string(dev), __func__, ## arg)
#define pltfrm_camera_module_pr_info(dev, fmt, arg...) \
	pr_info("%s.%s: " fmt, \
		pltfrm_dev_string(dev), __func__, ## arg)
#define pltfrm_camera_module_pr_warn(dev, fmt, arg...) \
	pr_warn("%s.%s WARN: " fmt, \
		pltfrm_dev_string(dev), __func__, ## arg)
#define pltfrm_camera_module_pr_err(dev, fmt, arg...) \
	pr_err("%s.%s(%d) ERR: " fmt, \
		pltfrm_dev_string(dev), __func__, __LINE__, \
		## arg)

#endif
