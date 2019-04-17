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

#include <linux/kernel.h>
#include <linux/i2c.h>
#include <media/v4l2-subdev.h>
#include <media/v4l2-common.h>
#include <media/v4l2-ioctl.h>
#include "cif_isp11.h"
#include <linux/platform_data/rk_isp11_platform.h>
#include <media/v4l2-controls_rockchip.h>
#include <linux/slab.h>
/* ===================== */
/* Image Source */
/* ===================== */
void *cif_isp11_img_src_v4l2_i2c_subdev_to_img_src(
	struct device *dev,
	struct pltfrm_soc_cfg *soc_cfg)
{
	int ret = 0;
	struct i2c_client *client;
	struct v4l2_subdev *subdev;

	client = i2c_verify_client(dev);
	if (IS_ERR_OR_NULL(client)) {
		cif_isp11_pltfrm_pr_err(dev,
			"not an I2C device\n");
		ret = -EINVAL;
		goto err;
	}

	subdev = i2c_get_clientdata(client);
	if (IS_ERR_OR_NULL(subdev))
		return subdev;

	ret = v4l2_subdev_call(subdev,
		core,
		ioctl,
		PLTFRM_CIFCAM_ATTACH,
		(void *)soc_cfg);
	if (ret != 0)
		goto err;

	return (void *)subdev;
err:
	cif_isp11_pltfrm_pr_err(NULL, "failed with error %d\n", ret);
	return ERR_PTR(ret);
}

static enum cif_isp11_pix_fmt img_src_v4l2_subdev_pix_fmt2cif_isp11_pix_fmt(
	int img_src_pix_fmt)
{
	switch (img_src_pix_fmt) {
	case V4L2_MBUS_FMT_Y8_1X8:
		#if (CIF_ISP11_PIX_FMT_Y_AS_BAYER)
		return CIF_BAYER_SBGGR8;
		#else
		return CIF_YUV400;
		#endif
	case V4L2_MBUS_FMT_Y10_1X10:
		#if (CIF_ISP11_PIX_FMT_Y_AS_BAYER)
		return CIF_BAYER_SBGGR10;
		#else
		return CIF_Y10;
		#endif
	case V4L2_MBUS_FMT_Y12_1X12:
		#if (CIF_ISP11_PIX_FMT_Y_AS_BAYER)
		return CIF_BAYER_SBGGR12;
		#else
		return CIF_Y12;
		#endif

	case V4L2_MBUS_FMT_YUYV8_1_5X8:
	case V4L2_MBUS_FMT_YUYV8_2X8:
	case V4L2_MBUS_FMT_YUYV10_2X10:
	case V4L2_MBUS_FMT_YUYV8_1X16:
	case V4L2_MBUS_FMT_YUYV10_1X20:
		return CIF_YUV422I;
	case V4L2_MBUS_FMT_UYVY8_1_5X8:
	case V4L2_MBUS_FMT_UYVY8_2X8:
	case V4L2_MBUS_FMT_UYVY8_1X16:
		return CIF_UYV422I;
	case V4L2_MBUS_FMT_RGB565_2X8_BE:
	case V4L2_MBUS_FMT_RGB565_2X8_LE:
		return CIF_RGB565;
	case V4L2_MBUS_FMT_RGB666_1X18:
		return CIF_RGB666;
	case V4L2_MBUS_FMT_RGB888_1X24:
	case V4L2_MBUS_FMT_RGB888_2X12_BE:
	case V4L2_MBUS_FMT_RGB888_2X12_LE:
		return CIF_RGB888;
	case V4L2_MBUS_FMT_SBGGR8_1X8:
		return CIF_BAYER_SBGGR8;
	case V4L2_MBUS_FMT_SGBRG8_1X8:
		return CIF_BAYER_SGBRG8;
	case V4L2_MBUS_FMT_SGRBG8_1X8:
		return CIF_BAYER_SGRBG8;
	case V4L2_MBUS_FMT_SRGGB8_1X8:
		return CIF_BAYER_SRGGB8;
	case V4L2_MBUS_FMT_SBGGR10_ALAW8_1X8:
	case V4L2_MBUS_FMT_SBGGR10_DPCM8_1X8:
	case V4L2_MBUS_FMT_SBGGR10_2X8_PADHI_BE:
	case V4L2_MBUS_FMT_SBGGR10_2X8_PADHI_LE:
	case V4L2_MBUS_FMT_SBGGR10_2X8_PADLO_BE:
	case V4L2_MBUS_FMT_SBGGR10_2X8_PADLO_LE:
	case V4L2_MBUS_FMT_SBGGR10_1X10:
		return CIF_BAYER_SBGGR10;
	case V4L2_MBUS_FMT_SGBRG10_ALAW8_1X8:
	case V4L2_MBUS_FMT_SGBRG10_DPCM8_1X8:
	case V4L2_MBUS_FMT_SGBRG10_1X10:
		return CIF_BAYER_SGBRG10;
	case V4L2_MBUS_FMT_SGRBG10_ALAW8_1X8:
	case V4L2_MBUS_FMT_SGRBG10_DPCM8_1X8:
	case V4L2_MBUS_FMT_SGRBG10_1X10:
		return CIF_BAYER_SGRBG10;
	case V4L2_MBUS_FMT_SRGGB10_ALAW8_1X8:
	case V4L2_MBUS_FMT_SRGGB10_DPCM8_1X8:
	case V4L2_MBUS_FMT_SRGGB10_1X10:
		return CIF_BAYER_SRGGB10;
	case V4L2_MBUS_FMT_SBGGR12_1X12:
	    return CIF_BAYER_SBGGR12;
	case V4L2_MBUS_FMT_SGBRG12_1X12:
		return CIF_BAYER_SGBRG12;
	case V4L2_MBUS_FMT_SGRBG12_1X12:
		return CIF_BAYER_SGRBG12;
	case V4L2_MBUS_FMT_SRGGB12_1X12:
		return CIF_BAYER_SRGGB12;
	case V4L2_MBUS_FMT_JPEG_1X8:
		return CIF_JPEG;
	default:
		return CIF_UNKNOWN_FORMAT;
	}
}

static int cif_isp11_pix_fmt2img_src_v4l2_subdev_pix_fmt(
	enum cif_isp11_pix_fmt cif_isp11_pix_fmt)
{
	switch (cif_isp11_pix_fmt) {
	case CIF_Y10:
		return V4L2_MBUS_FMT_Y10_1X10;
	case CIF_Y12:
		return V4L2_MBUS_FMT_Y12_1X12;
	case CIF_YUV400:
		return V4L2_MBUS_FMT_Y8_1X8;
	case CIF_YUV422I:
		return V4L2_MBUS_FMT_YUYV8_2X8;
	case CIF_UYV422I:
		return V4L2_MBUS_FMT_UYVY8_2X8;
	case CIF_RGB565:
		return V4L2_MBUS_FMT_RGB565_2X8_LE;
	case CIF_RGB666:
		return V4L2_MBUS_FMT_RGB666_1X18;
	case CIF_RGB888:
		return V4L2_MBUS_FMT_RGB888_1X24;
	case CIF_BAYER_SBGGR8:
		#if (CIF_ISP11_PIX_FMT_Y_AS_BAYER)
		return V4L2_MBUS_FMT_Y8_1X8;
		#else
		return V4L2_MBUS_FMT_SBGGR8_1X8;
		#endif
	case CIF_BAYER_SGBRG8:
		return V4L2_MBUS_FMT_SGBRG8_1X8;
	case CIF_BAYER_SGRBG8:
		return V4L2_MBUS_FMT_SGRBG8_1X8;
	case CIF_BAYER_SRGGB8:
		return V4L2_MBUS_FMT_SRGGB8_1X8;
	case CIF_BAYER_SBGGR10:
		#if (CIF_ISP11_PIX_FMT_Y_AS_BAYER)
		return V4L2_MBUS_FMT_Y10_1X10;
		#else
		return V4L2_MBUS_FMT_SBGGR10_1X10;
		#endif
	case CIF_BAYER_SGBRG10:
		return V4L2_MBUS_FMT_SGBRG10_1X10;
	case CIF_BAYER_SGRBG10:
		return V4L2_MBUS_FMT_SGRBG10_1X10;
	case CIF_BAYER_SRGGB10:
		return V4L2_MBUS_FMT_SRGGB10_1X10;
	case CIF_BAYER_SBGGR12:
		#if (CIF_ISP11_PIX_FMT_Y_AS_BAYER)
		return V4L2_MBUS_FMT_Y12_1X12;
		#else
		return V4L2_MBUS_FMT_SBGGR12_1X12;
		#endif
	case CIF_BAYER_SGBRG12:
		return V4L2_MBUS_FMT_SGBRG12_1X12;
	case CIF_BAYER_SGRBG12:
		return V4L2_MBUS_FMT_SGRBG12_1X12;
	case CIF_BAYER_SRGGB12:
		return V4L2_MBUS_FMT_SRGGB12_1X12;
	case CIF_JPEG:
		return V4L2_MBUS_FMT_JPEG_1X8;
	default:
		return -EINVAL;
	}
}

static int cif_isp11_v4l2_cid2v4l2_cid(u32 cif_isp11_cid)
{
	switch (cif_isp11_cid) {
	case CIF_ISP11_CID_FLASH_MODE:
		return V4L2_CID_FLASH_LED_MODE;
	case CIF_ISP11_CID_AUTO_GAIN:
		return V4L2_CID_AUTOGAIN;
	case CIF_ISP11_CID_AUTO_EXPOSURE:
		return V4L2_EXPOSURE_AUTO;
	case CIF_ISP11_CID_AUTO_WHITE_BALANCE:
		return V4L2_CID_AUTO_WHITE_BALANCE;
	case CIF_ISP11_CID_BLACK_LEVEL:
		return V4L2_CID_BLACK_LEVEL;
	case CIF_ISP11_CID_WB_TEMPERATURE:
		return V4L2_CID_WHITE_BALANCE_TEMPERATURE;
	case CIF_ISP11_CID_EXPOSURE_TIME:
		return V4L2_CID_EXPOSURE;
	case CIF_ISP11_CID_ANALOG_GAIN:
		return V4L2_CID_GAIN;
	case CIF_ISP11_CID_FOCUS_ABSOLUTE:
		return V4L2_CID_FOCUS_ABSOLUTE;
	case CIF_ISP11_CID_AUTO_N_PRESET_WHITE_BALANCE:
		return V4L2_CID_AUTO_N_PRESET_WHITE_BALANCE;
	case CIF_ISP11_CID_SCENE_MODE:
		return V4L2_CID_SCENE_MODE;
	case CIF_ISP11_CID_ISO_SENSITIVITY:
		return V4L2_CID_ISO_SENSITIVITY;
	case CIF_ISP11_CID_AUTO_FPS:
		return RK_V4L2_CID_AUTO_FPS;
	case CIF_ISP11_CID_VBLANKING:
		return RK_V4L2_CID_VBLANKING;
	case CIF_ISP11_CID_HFLIP:
		return V4L2_CID_HFLIP;
	case CIF_ISP11_CID_VFLIP:
		return V4L2_CID_VFLIP;
	case CIF_ISP11_CID_BAND_STOP_FILTER:
		return V4L2_CID_BAND_STOP_FILTER;
	default:
		cif_isp11_pltfrm_pr_err(NULL,
			"unknown/unsupported CIF ISP20 ID %d\n",
			cif_isp11_cid);
		break;
	}
	return -EINVAL;
}

int cif_isp11_img_src_v4l2_subdev_s_streaming(
	void *img_src,
	bool enable)
{
	struct v4l2_subdev *subdev = img_src;

	if (enable)
		return v4l2_subdev_call(subdev, video, s_stream, 1);
	else
		return v4l2_subdev_call(subdev, video, s_stream, 0);
}

int cif_isp11_img_src_v4l2_subdev_s_power(
	void *img_src,
	bool on)
{
	struct v4l2_subdev *subdev = img_src;

	if (on)
		return v4l2_subdev_call(subdev, core, s_power, 1);
	else
		return v4l2_subdev_call(subdev, core, s_power, 0);
}

int cif_isp11_img_src_v4l2_subdev_enum_strm_fmts(
	void *img_src,
	u32 index,
	struct cif_isp11_strm_fmt_desc *strm_fmt_desc)
{
	int ret;
	struct v4l2_subdev *subdev = img_src;
	struct v4l2_frmivalenum fie = {.index = index};
	struct pltfrm_cam_defrect defrect;
	v4l2_std_id std;

	ret = v4l2_subdev_call(subdev, video, querystd, &std);
	if (!IS_ERR_VALUE(ret))
		strm_fmt_desc->std_id = std;
	else
		strm_fmt_desc->std_id = 0;

	ret = v4l2_subdev_call(subdev, video,
		enum_frameintervals, &fie);
	if (!IS_ERR_VALUE(ret)) {
		strm_fmt_desc->discrete_intrvl = true;
		strm_fmt_desc->min_intrvl.numerator =
			fie.discrete.numerator;
		strm_fmt_desc->min_intrvl.denominator =
			fie.discrete.denominator;
		strm_fmt_desc->discrete_frmsize = true;
		strm_fmt_desc->min_frmsize.width = fie.width;
		strm_fmt_desc->min_frmsize.height = fie.height;
		strm_fmt_desc->pix_fmt =
			img_src_v4l2_subdev_pix_fmt2cif_isp11_pix_fmt(
				fie.pixel_format);

		defrect.width = fie.width;
		defrect.height = fie.height;
		memset(&defrect.defrect, 0x00, sizeof(struct v4l2_rect));
		v4l2_subdev_call(subdev,
			core,
			ioctl,
			PLTFRM_CIFCAM_G_DEFRECT,
			(void *)&defrect);
		if ((defrect.defrect.width == 0) ||
			(defrect.defrect.height == 0)) {
			strm_fmt_desc->defrect.left = 0;
			strm_fmt_desc->defrect.top = 0;
			strm_fmt_desc->defrect.width = fie.width;
			strm_fmt_desc->defrect.height = fie.height;
		} else
			strm_fmt_desc->defrect = defrect.defrect;

	}

	return ret;
}

int cif_isp11_img_src_v4l2_subdev_s_strm_fmt(
	void *img_src,
	struct cif_isp11_strm_fmt *strm_fmt)
{
	int ret = 0;
	struct v4l2_subdev *subdev = img_src;
	struct v4l2_mbus_framefmt format;

	format.code = cif_isp11_pix_fmt2img_src_v4l2_subdev_pix_fmt(
		strm_fmt->frm_fmt.pix_fmt);
	format.width = strm_fmt->frm_fmt.width;
	format.height = strm_fmt->frm_fmt.height;
	ret = v4l2_subdev_call(subdev, video, s_mbus_fmt, &format);
	if (IS_ERR_VALUE(ret))
		goto err;

	return 0;
err:
	pr_err("img_src.%s ERR: failed with error %d\n", __func__, ret);
	return ret;
}

int cif_isp11_img_src_v4l2_subdev_s_frame_interval(
	void *img_src,
	struct cif_isp11_frm_intrvl *frm_intrvl)
{
	int ret = 0;
	struct v4l2_subdev *subdev = img_src;
	struct v4l2_subdev_frame_interval interval;

	interval.interval.numerator = frm_intrvl->numerator;
	interval.interval.denominator = frm_intrvl->denominator;

	ret = v4l2_subdev_call(subdev, video, s_frame_interval, &interval);
	if (IS_ERR_VALUE(ret))
		goto err;

	return 0;
err:
	pr_err("img_src.%s ERR: failed with error %d\n", __func__, ret);
	return ret;
}

int cif_isp11_img_src_v4l2_subdev_g_frame_interval(
	void *img_src,
	struct cif_isp11_frm_intrvl *frm_intrvl)
{
	int ret = 0;
	struct v4l2_subdev *subdev = img_src;
	struct v4l2_subdev_frame_interval interval;

	interval.interval.numerator = 0;
	interval.interval.denominator = 0;

	ret = v4l2_subdev_call(subdev, video, g_frame_interval, &interval);
	if (IS_ERR_VALUE(ret))
		goto err;

	frm_intrvl->denominator = interval.interval.denominator;
	frm_intrvl->numerator = interval.interval.numerator;

	return 0;
err:
	pr_err("img_src.%s ERR: failed with error %d\n", __func__, ret);
	return ret;
}

int cif_isp11_img_src_v4l2_subdev_g_ctrl(
	void *img_src,
	int id,
	int *val)
{
	struct v4l2_control ctrl;
	int ret;
	struct v4l2_subdev *subdev = img_src;

	ctrl.id = cif_isp11_v4l2_cid2v4l2_cid(id);

	if (IS_ERR_VALUE(ctrl.id))
		return (int)ctrl.id;

	ret = v4l2_subdev_call(subdev, core, g_ctrl, &ctrl);
	if (!IS_ERR_VALUE(ret)) {
		if (id == CIF_ISP11_CID_FLASH_MODE) {
			if (ctrl.value == V4L2_FLASH_LED_MODE_NONE)
				ctrl.value = CIF_ISP11_FLASH_MODE_OFF;
			else if (ctrl.value == V4L2_FLASH_LED_MODE_FLASH)
				ctrl.value = CIF_ISP11_FLASH_MODE_FLASH;
			else if (ctrl.value == V4L2_FLASH_LED_MODE_TORCH)
				ctrl.value = CIF_ISP11_FLASH_MODE_TORCH;
			else {
				cif_isp11_pltfrm_pr_err(NULL,
					"unknown/unsupported value %d for control ID 0x%x\n",
					ctrl.value, id);
				return -EINVAL;
			}
		}
		*val = ctrl.value;
	}
	return ret;
}

int cif_isp11_img_src_v4l2_subdev_s_ctrl(
	void *img_src,
	int id,
	int val)
{
	struct v4l2_control ctrl;
	struct v4l2_subdev *subdev = img_src;

	ctrl.value = val;
	ctrl.id = cif_isp11_v4l2_cid2v4l2_cid(id);

	if (IS_ERR_VALUE(ctrl.id))
		return (int)ctrl.id;
	else if (id == CIF_ISP11_CID_FLASH_MODE) {
		if (val == CIF_ISP11_FLASH_MODE_OFF)
			ctrl.value = V4L2_FLASH_LED_MODE_NONE;
		else if (val == CIF_ISP11_FLASH_MODE_FLASH)
			ctrl.value = V4L2_FLASH_LED_MODE_FLASH;
		else if (val == CIF_ISP11_FLASH_MODE_TORCH)
			ctrl.value = V4L2_FLASH_LED_MODE_TORCH;
		else {
			cif_isp11_pltfrm_pr_err(NULL,
				"unknown/unsupported value %d for control ID %d\n",
				val, id);
			return -EINVAL;
		}
	}
	return v4l2_subdev_call(subdev, core, s_ctrl, &ctrl);
}

const char *cif_isp11_img_src_v4l2_subdev_g_name(
	void *img_src)
{
	struct v4l2_subdev *subdev = img_src;

	return (char *)subdev->name;
}

int cif_isp11_img_src_v4l2_subdev_s_ext_ctrls(
	void *img_src,
	struct cif_isp11_img_src_ext_ctrl *ctrl)
{
	struct v4l2_ext_controls ctrls;
	struct v4l2_ext_control *controls;
	int i;
	int ret;
	struct v4l2_subdev *subdev = img_src;

	if (ctrl->cnt == 0)
		return -EINVAL;

	controls = kmalloc(ctrl->cnt * sizeof(struct v4l2_ext_control),
		GFP_KERNEL);

	if (!controls)
		return -ENOMEM;

	for (i = 0; i < ctrl->cnt; i++) {
		controls[i].id = ctrl->ctrls[i].id;
		controls[i].value = ctrl->ctrls[i].val;
	}

	ctrls.count = ctrl->cnt;
	ctrls.controls = controls;
	ctrls.ctrl_class = ctrl->class;
	ctrls.reserved[0] = 0;
	ctrls.reserved[1] = 0;

	ret = v4l2_subdev_call(subdev,
		core, s_ext_ctrls, &ctrls);

	kfree(controls);

	return ret;
}

long cif_isp11_img_src_v4l2_subdev_ioctl(
	void *img_src,
	unsigned int cmd,
	void *arg)
{
	struct v4l2_subdev *subdev = img_src;
	long ret = -EINVAL;

	switch (cmd) {
	case RK_VIDIOC_SENSOR_MODE_DATA:
	case RK_VIDIOC_CAMERA_MODULEINFO:

	case PLTFRM_CIFCAM_G_ITF_CFG:
	case PLTFRM_CIFCAM_G_DEFRECT:
	case PLTFRM_CIFCAM_ATTACH:
	case PLTFRM_CIFCAM_R_LIGHTSENSOR:
		ret = v4l2_subdev_call(subdev,
			core,
			ioctl,
			cmd,
			arg);

		break;
	default:
		break;
	}

	if (IS_ERR_VALUE(ret))
		pr_err("img_src.%s subdev call(cmd: 0x%x) failed with error %ld\n",
		__func__, cmd, ret);

	return ret;
}