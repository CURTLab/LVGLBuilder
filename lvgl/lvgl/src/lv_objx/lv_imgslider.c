/**
 * @file lv_imgslider.c
 *
 */

/* TODO Remove these instructions
 * Search an replace: image slider -> object normal name with lower case (e.g. button, label etc.)
 *                    imgslider -> object short name with lower case(e.g. btn, label etc)
 *                    IMGSLIDER -> object short name with upper case (e.g. BTN, LABEL etc.)
 *
 * You can remove the defined() clause from the #if statement below. This exists because
 * LV_USE_imgslider is not in lv_conf.h or lv_conf_imgslider.h by default.
 */

/*********************
 *      INCLUDES
 *********************/
#include "../lv_core/lv_debug.h"
#include "lv_imgslider.h"
#include "../lv_themes/lv_theme.h"
#include "../lv_draw/lv_draw.h"

#if defined(LV_USE_IMGSLIDER) && LV_USE_IMGSLIDER != 0

#define LV_IMGSLIDER_NOT_PRESSED INT16_MIN

/*********************
 *      DEFINES
 *********************/
#define LV_OBJX_NAME "lv_imgslider"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool lv_imgslider_design(lv_obj_t * imgslider, const lv_area_t * mask, lv_design_mode_t mode);
static lv_res_t lv_imgslider_signal(lv_obj_t * imgslider, lv_signal_t sign, void * param);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_signal_cb_t ancestor_signal;
static lv_design_cb_t ancestor_design;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a imgsliderate object
 * @param par pointer to an object, it will be the parent of the new image slider
 * @param copy pointer to a image slider object, if not NULL then the new object will be copied from it
 * @return pointer to the created image slider
 */
lv_obj_t * lv_imgslider_create(lv_obj_t * par, const lv_obj_t * copy)
{
	LV_LOG_TRACE("image slider create started");

	/*Create the ancestor of image slider*/
	/*TODO modify it to the ancestor create function */
	lv_obj_t * imgslider = lv_slider_create(par, copy);
	LV_ASSERT_MEM(imgslider);
	if(imgslider == NULL) return NULL;

	/*Allocate the image slider type specific extended data*/
	lv_imgslider_ext_t * ext = lv_obj_allocate_ext_attr(imgslider, sizeof(lv_imgslider_ext_t));
	LV_ASSERT_MEM(ext);
	if(ext == NULL) return NULL;
	if(ancestor_signal == NULL) ancestor_signal = lv_obj_get_signal_cb(imgslider);
	if(ancestor_design == NULL) ancestor_design = lv_obj_get_design_cb(imgslider);

	/*Initialize the allocated 'ext' */
	//memset(ext->src_knob, 0, sizeof(ext->src_knob));
	//memset(ext->src_bar, 0, sizeof(ext->src_bar));
	ext->src_knob = 0;
	ext->src_indic = 0;
	ext->src_bg = 0;
	ext->offset_knob.x = 0;
	ext->offset_knob.y = 0;
	ext->slider.knob_in = 1;

	/*The signal and design functions are not copied so set them here*/
	lv_obj_set_signal_cb(imgslider, lv_imgslider_signal);
	lv_obj_set_design_cb(imgslider, lv_imgslider_design);

	/*Init the new image slider image slider*/
	if(copy == NULL) {
		/*Set the default styles*/
		lv_theme_t * th = lv_theme_get_current();
		if(th) {
			lv_slider_set_style(imgslider, LV_SLIDER_STYLE_BG, th->style.slider.bg);
			lv_slider_set_style(imgslider, LV_SLIDER_STYLE_INDIC, th->style.slider.indic);
			lv_slider_set_style(imgslider, LV_SLIDER_STYLE_KNOB, th->style.slider.knob);
		} else {
			lv_slider_set_style(imgslider, LV_IMGSLIDER_STYLE_KNOB, ext->slider.style_knob);
		}
	}
	/*Copy an existing image slider*/
	else {
		lv_imgslider_ext_t * copy_ext = lv_obj_get_ext_attr(copy);
		memcpy(ext->src_knob, copy_ext->src_knob, sizeof(ext->src_knob));
		memcpy(ext->src_bg, copy_ext->src_bg, sizeof(ext->src_bg));
		memcpy(&ext->offset_knob, &copy_ext->offset_knob, sizeof(ext->offset_knob));
		memcpy(&ext->slider, &copy_ext->slider, sizeof(ext->slider));

		/*Refresh the style with new signal function*/
		lv_obj_refresh_style(imgslider);
	}

	LV_LOG_INFO("image slider created");

	return imgslider;
}

/*======================
 * Add/remove functions
 *=====================*/

/*
 * New object specific "add" or "remove" functions come here
 */

/*=====================
 * Setter functions
 *====================*/

/*
 * New object specific "set" functions come here
 */

/**
 * Set image for the background of the image slider
 * @param imgslider pointer to an image slider object
 * @param src pointer to an image source (a C array or path to a file)
 */
void lv_imgslider_set_bg_src(lv_obj_t *imgslider, const void *src)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);
	ext->src_bg = src;

	lv_img_header_t header;
	lv_res_t info_res = LV_RES_OK;
	if(lv_img_src_get_type(src) == LV_IMG_SRC_SYMBOL) {
		 const lv_style_t * style = lv_obj_get_style(imgslider);
		 header.h = lv_font_get_line_height(style->text.font);
		 header.w = lv_txt_get_width(src, strlen(src), style->text.font, style->text.letter_space, LV_TXT_FLAG_NONE);
		 header.always_zero = 0;
		 header.cf = LV_IMG_CF_ALPHA_1BIT;
	} else {
		 info_res = lv_img_decoder_get_info(src, &header);
	}

	if(info_res == LV_RES_OK) {
		 lv_obj_set_size(imgslider, header.w, header.h);
	}
	lv_obj_invalidate(imgslider);
}

/**
 * Set image for the indicator of the image slider
 * @param imgslider pointer to an image slider object
 * @param src pointer to an image source (a C array or path to a file)
 */
void lv_imgslider_set_indic_src(lv_obj_t *imgslider, const void *src)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);
	ext->src_indic = src;

	lv_img_header_t header;
	lv_res_t info_res = LV_RES_OK;
	if(lv_img_src_get_type(src) == LV_IMG_SRC_SYMBOL) {
		 const lv_style_t * style = lv_obj_get_style(imgslider);
		 header.h = lv_font_get_line_height(style->text.font);
		 header.w = lv_txt_get_width(src, strlen(src), style->text.font, style->text.letter_space, LV_TXT_FLAG_NONE);
		 header.always_zero = 0;
		 header.cf = LV_IMG_CF_ALPHA_1BIT;
	} else {
		 info_res = lv_img_decoder_get_info(src, &header);
	}

	if(info_res == LV_RES_OK) {
		 lv_obj_set_size(imgslider, header.w, header.h);
	}
	lv_obj_invalidate(imgslider);
}

/**
 * Set image for the knob of the image slider
 * @param imgslider pointer to an image slider object
 * @param src pointer to an image source (a C array or path to a file)
 */
void lv_imgslider_set_knob_src(lv_obj_t *imgslider, const void *src)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);
	ext->src_knob = src;

	lv_img_header_t header;
	lv_res_t info_res = LV_RES_OK;
	if(lv_img_src_get_type(src) == LV_IMG_SRC_SYMBOL) {
		 const lv_style_t * style = lv_obj_get_style(imgslider);
		 header.h = lv_font_get_line_height(style->text.font);
		 header.w = lv_txt_get_width(src, strlen(src), style->text.font, style->text.letter_space, LV_TXT_FLAG_NONE);
		 header.always_zero = 0;
		 header.cf = LV_IMG_CF_ALPHA_1BIT;
	} else {
		 info_res = lv_img_decoder_get_info(src, &header);
	}

	if(info_res == LV_RES_OK) {
		if (header.w < lv_obj_get_width(imgslider))
			header.w = lv_obj_get_width(imgslider);
		if (header.h < lv_obj_get_height(imgslider))
			header.h = lv_obj_get_height(imgslider);
		lv_obj_set_size(imgslider, header.w, header.h);
	}
	lv_obj_invalidate(imgslider);
}

/**
 * Set a style of a image slider.
 * @param imgslider pointer to image slider object
 * @param type which style should be set
 * @param style pointer to a style
 */
void lv_imgslider_set_style(lv_obj_t * imgslider, lv_imgslider_style_t type, const lv_style_t * style)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_slider_set_style(imgslider, type, style);
}

/**
 * Set an offset for the knob.
 * @param imgslide pointer to an image slider
 * @param x: the new offset along x axis.
 */
void lv_imgslider_set_offset_x(lv_obj_t *imgslider, lv_coord_t x)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);

	ext->offset_knob.x = x;

	lv_obj_invalidate(imgslider);
}

/**
 * Set an offset for the knob.
 * @param imgslide pointer to an image slider
 * @param y: the new offset along y axis.
 */
void lv_imgslider_set_offset_y(lv_obj_t *imgslider, lv_coord_t y)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);

	ext->offset_knob.y = y;

	lv_obj_invalidate(imgslider);
}

/*=====================
 * Getter functions
 *====================*/

/*
 * New object specific "get" functions come here
 */

/**
 * Get the image of the background
 * @param imgslider pointer to an image slider object
 * @return pointer to an image source (a C array or path to a file)
 */
const void *lv_imgslide_get_bg_src(lv_obj_t *imgslider)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);

	return ext->src_bg;
}

/**
 * Get the image of the indicator
 * @param imgslider pointer to an image slider object
 * @return pointer to an image source (a C array or path to a file)
 */
const void *lv_imgslide_get_indic_src(lv_obj_t *imgslider)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);

	return ext->src_indic;
}

/**
 * Get the image of the knob
 * @param imgslider pointer to an image slider object
 * @return pointer to an image source (a C array or path to a file)
 */
const void *lv_imgslide_get_knob_src(lv_obj_t *imgslider)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);

	return ext->src_knob;
}

/**
 * Get style of a image slider.
 * @param imgslider pointer to image slider object
 * @param type which style should be get
 * @return style pointer to the style
 */
lv_style_t * lv_imgslider_get_style(const lv_obj_t * imgslider, lv_imgslider_style_t type)
{
	 LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	 return lv_slider_get_style(imgslider, type);
}

/**
 * Get the offset.x attribute of the image slider knob.
 * @param imgslide pointer to an image slider
 * @return offset.x value.
 */
lv_coord_t lv_imgslider_get_offset_x(lv_obj_t *imgslider)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);

	return ext->offset_knob.x;
}

/**
 * Get the offset.y attribute of the image slider knob.
 * @param imgslide pointer to an image slider
 * @return offset.y value.
 */
lv_coord_t lv_imgslider_get_offset_y(lv_obj_t *imgslider)
{
	LV_ASSERT_OBJ(imgslider, LV_OBJX_NAME);

	lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);

	return ext->offset_knob.y;
}

/*=====================
 * Other functions
 *====================*/

/*
 * New object specific "other" functions come here
 */

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Handle the drawing related tasks of the imgsliderates
 * @param imgslider pointer to an object
 * @param mask the object will be drawn only in this area
 * @param mode LV_DESIGN_COVER_CHK: only check if the object fully covers the 'mask_p' area
 *                                  (return 'true' if yes)
 *             LV_DESIGN_DRAW: draw the object (always return 'true')
 *             LV_DESIGN_DRAW_POST: drawing after every children are drawn
 * @param return true/false, depends on 'mode'
 */
static bool lv_imgslider_design(lv_obj_t * imgslider, const lv_area_t * mask, lv_design_mode_t mode)
{
	/*Return false if the object is not covers the mask_p area*/
	if(mode == LV_DESIGN_COVER_CHK) {
		return false;
	}
	/*Draw the object*/
	else if(mode == LV_DESIGN_DRAW_MAIN) {
		lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);

		const lv_style_t * style_bg    = lv_slider_get_style(imgslider, LV_IMGSLIDER_STYLE_BG);
		const lv_style_t * style_indic = lv_slider_get_style(imgslider, LV_IMGSLIDER_STYLE_INDIC);
		const lv_style_t * style_knob  = lv_slider_get_style(imgslider, LV_IMGSLIDER_STYLE_KNOB);

		const lv_img_dsc_t *dsc_bg  = ext->src_bg;
		const lv_img_dsc_t *dsc_indic = ext->src_indic;
		const lv_img_dsc_t *dsc_knob = ext->src_knob;

		lv_opa_t opa_scale = lv_obj_get_opa_scale(imgslider);

		lv_coord_t slider_w = lv_area_get_width(&imgslider->coords);
		lv_coord_t slider_h = lv_area_get_height(&imgslider->coords);

		lv_coord_t cur_value = lv_slider_get_value(imgslider);
		lv_coord_t min_value = lv_slider_get_min_value(imgslider);
		lv_coord_t max_value = lv_slider_get_max_value(imgslider);

		lv_coord_t knob_w = dsc_knob ? dsc_knob->header.w : (slider_w > slider_h ? slider_h : slider_w);
		lv_coord_t knob_h = dsc_knob ? dsc_knob->header.h : (slider_w > slider_h ? slider_h : slider_w);

		/*Draw the bar*/
		lv_area_t area_bg;
		lv_area_copy(&area_bg, &imgslider->coords);
		if (style_bg->image.opa > 0)
			lv_draw_img(&area_bg, mask, dsc_bg, style_bg, opa_scale);

		/*Draw the indicator*/
		lv_area_t area_indic;
		lv_area_copy(&area_indic, &area_bg);


		if (style_indic->image.opa > 0) {
			/*If dragged draw to the drag position*/
			if(ext->slider.drag_value != LV_IMGSLIDER_NOT_PRESSED) cur_value = ext->slider.drag_value;
			bool sym = false;
			if(ext->slider.bar.sym && ext->slider.bar.min_value < 0 && ext->slider.bar.max_value > 0) sym = true;

			if(slider_w >= slider_h) {
				lv_coord_t indic_w = lv_area_get_width(&area_indic);
	#if LV_USE_ANIMATION
				if(ext->slider.bar.anim_state != LV_BAR_ANIM_STATE_INV) {
					/*Calculate the coordinates of anim. start and end*/
					lv_coord_t anim_start_x =
							(int32_t)((int32_t)indic_w * (ext->slider.bar.anim_start - min_value)) / (max_value - min_value);
					lv_coord_t anim_end_x =
							(int32_t)((int32_t)indic_w * (ext->slider.bar.anim_end - min_value)) / (max_value - min_value);

					/*Calculate the real position based on `anim_state` (between `anim_start` and
						* `anim_end`)*/
					area_indic.x2 = anim_start_x + (((anim_end_x - anim_start_x) * ext->slider.bar.anim_state) >> 8);
				} else
	#endif
				{
					area_indic.x2 = (int32_t)((int32_t)indic_w * (cur_value - min_value)) / (max_value - min_value);
				}

				area_indic.x2 = area_indic.x1 + area_indic.x2 - 1;
				if(sym) {
					/*Calculate the coordinate of the zero point*/
					lv_coord_t zero;
					zero = area_indic.x1 + (-ext->slider.bar.min_value * slider_w) / (ext->slider.bar.max_value - ext->slider.bar.min_value);
					if(area_indic.x2 > zero)
						area_indic.x1 = zero;
					else {
						area_indic.x1 = area_indic.x2;
						area_indic.x2 = zero;
					}
				}

				/*Draw the indicator but don't draw an ugly 1px wide rectangle on the left on min.
				  * value*/
				if(area_indic.x1 != area_indic.x2)
					lv_draw_img(&area_bg, &area_indic, dsc_indic, style_indic, opa_scale);

			} else {
				lv_coord_t indic_h = lv_area_get_height(&area_indic);
	#if LV_USE_ANIMATION
				if(ext->slider.bar.anim_state != LV_BAR_ANIM_STATE_INV) {
					/*Calculate the coordinates of anim. start and end*/
					lv_coord_t anim_start_y =
							(int32_t)((int32_t)indic_h * (ext->slider.bar.anim_start - min_value)) / (max_value - min_value);
					lv_coord_t anim_end_y =
							(int32_t)((int32_t)indic_h * (ext->slider.bar.anim_end - min_value)) / (max_value - min_value);

					/*Calculate the real position based on `anim_state` (between `anim_start` and
						* `anim_end`)*/
					area_indic.y1 = anim_start_y + (((anim_end_y - anim_start_y) * ext->slider.bar.anim_state) >> 8);
				} else
	#endif
				{
					area_indic.y1 = (int32_t)((int32_t)indic_h * (cur_value - min_value)) / (max_value - min_value);
				}

				area_indic.y1 = area_indic.y2 - area_indic.y1 + 1;

				if(sym) {
					/*Calculate the coordinate of the zero point*/
					lv_coord_t zero;
					zero = area_indic.y2 - (-ext->slider.bar.min_value * slider_h) / (ext->slider.bar.max_value - ext->slider.bar.min_value);
					if(area_indic.y1 < zero)
						area_indic.y2 = zero;
					else {
						area_indic.y2 = area_indic.y1;
						area_indic.y1 = zero;
					}
				}

				/*Draw the indicator but don't draw an ugly 1px height rectangle on the bottom on min.
				  * value*/
				if(area_indic.x1 != area_indic.x2)
					lv_draw_img(&area_bg, &area_indic, dsc_indic, style_indic, opa_scale);
			}
		}

		/*Draw the knob*/
		lv_area_t knob_area;
		lv_area_copy(&knob_area, &imgslider->coords);

		if (style_knob->image.opa > 0) {
			if(slider_w >= slider_h) {
	#if LV_USE_ANIMATION
					if(ext->slider.bar.anim_state != LV_BAR_ANIM_STATE_INV) {
						lv_coord_t w = slider_w - knob_w - 2 * ext->offset_knob.x - 1;
						lv_coord_t anim_start_x =
								(int32_t)((int32_t)w * (ext->slider.bar.anim_start - min_value)) / (max_value - min_value);
						lv_coord_t anim_end_x =
								(int32_t)((int32_t)w * (ext->slider.bar.anim_end - min_value)) / (max_value - min_value);

						/*Calculate the real position based on `anim_state` (between `anim_start` and
							 * `anim_end`)*/
						knob_area.x1 = anim_start_x + (((anim_end_x - anim_start_x) * ext->slider.bar.anim_state) >> 8);
					} else
	#endif
					{
						knob_area.x1 = (int32_t)((int32_t)(slider_w - knob_w - 2 * ext->offset_knob.x - 1) * (cur_value - min_value)) /
								(max_value - min_value);
					}

					knob_area.x1 += imgslider->coords.x1 + ext->offset_knob.x;
					knob_area.x2 = knob_area.x1 + knob_w - 1;

				knob_area.y1 = imgslider->coords.y1 + (slider_h - knob_h) / 2  + ext->offset_knob.y;
				knob_area.y2 = knob_area.y1 + knob_h - 1;
			} else {
	#if LV_USE_ANIMATION
					if(ext->slider.bar.anim_state != LV_BAR_ANIM_STATE_INV) {
						lv_coord_t h = slider_h - knob_h - 2 * ext->offset_knob.y - 1;
						lv_coord_t anim_start_x =
								(int32_t)((int32_t)h * (ext->slider.bar.anim_start - min_value)) / (max_value - min_value);
						lv_coord_t anim_end_x =
								(int32_t)((int32_t)h * (ext->slider.bar.anim_end - min_value)) / (max_value - min_value);

						/*Calculate the real position based on `anim_state` (between `anim_start` and
							 * `anim_end`)*/
						knob_area.y2 = anim_start_x + (((anim_end_x - anim_start_x) * ext->slider.bar.anim_state) >> 8);
					} else
	#endif
					{
						knob_area.y2 = (int32_t)((int32_t)(slider_h - knob_h - 2 * ext->offset_knob.y - 1) * (cur_value - min_value)) /
								(max_value - min_value);
					}

					knob_area.y2 = imgslider->coords.y2 - knob_area.y2 - ext->offset_knob.y;
					knob_area.y1 = knob_area.y2 - knob_h - 1;

				knob_area.x1 = imgslider->coords.x1 + (slider_w - knob_w) / 2  + ext->offset_knob.x;
				knob_area.x2 = knob_area.x1 + knob_w - 1;
			}
			lv_draw_img(&knob_area, mask, dsc_knob, style_knob, opa_scale);
		}
	}

	return true;
}

/**
 * Signal function of the image slider
 * @param imgslider pointer to a image slider object
 * @param sign a signal type from lv_signal_t enum
 * @param param pointer to a signal specific variable
 * @return LV_RES_OK: the object is not deleted in the function; LV_RES_INV: the object is deleted
 */
static lv_res_t lv_imgslider_signal(lv_obj_t * imgslider, lv_signal_t sign, void * param)
{
	 lv_res_t res = LV_RES_OK;

	 /* Include the ancient signal function */
	 res = ancestor_signal(imgslider, sign, param);
	 if(res != LV_RES_OK) return res;
	 if(sign == LV_SIGNAL_GET_TYPE) return lv_obj_handle_get_type_signal(param, LV_OBJX_NAME);

	 if(sign == LV_SIGNAL_CLEANUP) {
		  /*Nothing to cleanup. (No dynamically allocated memory in 'ext')*/
	 }

	 if (sign == LV_SIGNAL_CORD_CHG) {
		 lv_imgslider_ext_t * ext = lv_obj_get_ext_attr(imgslider);
		 const lv_img_dsc_t *dsc_bg  = ext->src_bg;
		 if (dsc_bg)
			lv_obj_set_size(imgslider, dsc_bg->header.w, dsc_bg->header.h);
	 }

	 return res;
}

#endif
