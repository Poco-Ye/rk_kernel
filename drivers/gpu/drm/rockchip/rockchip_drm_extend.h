#include "../../../video/rockchip/rk_drm_fb.h"
#include "../../../video/rockchip/hdmi/rockchip-hdmi.h"
#define WINDOWS_NR	4
#define MAX_HDMI_WIDTH   1920
#define MAX_HDMI_HEIGHT  1080

#define get_extend_context(dev)	platform_get_drvdata(to_platform_device(dev))

struct extend_win_data {
	unsigned int		offset_x;
	unsigned int		offset_y;
	unsigned int		ovl_width;
	unsigned int		ovl_height;
	unsigned int		fb_width;
	unsigned int		fb_height;
	unsigned int		bpp;
	dma_addr_t		dma_addr[MAX_FB_BUFFER];
	unsigned int		buf_offsize;
	unsigned int		line_size;	/* bytes */
	bool			enabled;
	bool			resume;
};

struct extend_context {
	struct rockchip_drm_subdrv	subdrv;
	int				vblank_en;
	struct drm_crtc			*crtc;
	struct rk_drm_display		*drm_disp;
	struct extend_win_data		win_data[WINDOWS_NR];
	unsigned int			default_win;
	bool				suspended;
	struct mutex			lock;
	wait_queue_head_t		wait_vsync_queue;
	atomic_t			wait_vsync_event;

	struct rockchip_drm_panel_info *panel;
};
