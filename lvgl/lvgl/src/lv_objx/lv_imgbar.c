/**
 * @file lv_imgbar.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "../lv_core/lv_debug.h"
#include "lv_imgbar.h"
#include "../lv_themes/lv_theme.h"

#if defined(LV_USE_IMGBAR) && LV_USE_IMGBAR != 0

/*********************
 *      DEFINES
 *********************/
#define LV_OBJX_NAME "lv_imgbar"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static bool lv_imgbar_design(lv_obj_t * imgbar, const lv_area_t * mask, lv_design_mode_t mode);
static lv_res_t lv_imgbar_signal(lv_obj_t * imgbar, lv_signal_t sign, void * param);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_design_cb_t ancestor_design;
static lv_signal_cb_t ancestor_signal;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a imgbarate object
 * @param par pointer to an object, it will be the parent of the new imgbarate
 * @param copy pointer to a imgbarate object, if not NULL then the new object will be copied from it
 * @return pointer to the created imgbarate
 */
lv_obj_t * lv_imgbar_create(lv_obj_t * par, const lv_obj_t * copy)
{
	 LV_LOG_TRACE("imgbarate create started");

	 /*Create the ancestor of imgbarate*/
	 /*TODO modify it to the ancestor create function */
	 lv_obj_t * new_imgbar = lv_bar_create(par, copy);
	 LV_ASSERT_MEM(new_imgbar);
	 if(new_imgbar == NULL) return NULL;

	 /*Allocate the imgbarate type specific extended data*/
	 lv_imgbar_ext_t * ext = lv_obj_allocate_ext_attr(new_imgbar, sizeof(lv_imgbar_ext_t));
	 LV_ASSERT_MEM(ext);
	 if(ext == NULL) return NULL;
	 if(ancestor_signal == NULL) ancestor_signal = lv_obj_get_signal_cb(new_imgbar);
	 if(ancestor_design == NULL) ancestor_design = lv_obj_get_design_cb(new_imgbar);

	 ext->src_bg = 0;
	 ext->src_indic = 0;

	 /*Initialize the allocated 'ext' */
	 //ext->xyz = 0;

	 /*The signal and design functions are not copied so set them here*/
	 lv_obj_set_signal_cb(new_imgbar, lv_imgbar_signal);
	 lv_obj_set_design_cb(new_imgbar, lv_imgbar_design);

	 /*Init the new imgbarate imgbarate*/
	 if(copy == NULL) {
		 lv_theme_t * th = lv_theme_get_current();
		 if(th) {
			  lv_bar_set_style(new_imgbar, LV_BAR_STYLE_BG, th->style.bar.bg);
			  lv_bar_set_style(new_imgbar, LV_BAR_STYLE_INDIC, th->style.bar.indic);
		 } else {
			  lv_obj_set_style(new_imgbar, &lv_style_pretty);
		 }
	 }
	 /*Copy an existing imgbarate*/
	 else {
		  lv_imgbar_ext_t * copy_ext = lv_obj_get_ext_attr(copy);
		  memcpy(&ext->bar, &copy_ext->bar, sizeof(ext->bar));
		  lv_obj_refresh_style(new_imgbar);

		  lv_bar_set_value(new_imgbar, ext->bar.cur_value, false);
	 }

	 LV_LOG_INFO("imgbarate created");

	 return new_imgbar;
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
 * Set image for the background of the image bar
 * @param imgbar pointer to an image bar object
 * @param src pointer to an image source (a C array or path to a file)
 */
void lv_imgbar_set_bg_src(lv_obj_t * imgbar, const void * src)
{
	LV_ASSERT_OBJ(imgbar, LV_OBJX_NAME);

	lv_imgbar_ext_t * ext = lv_obj_get_ext_attr(imgbar);
	ext->src_bg = src;

	lv_img_header_t header;

	lv_res_t info_res = LV_RES_OK;
	if(lv_img_src_get_type(src) == LV_IMG_SRC_SYMBOL) {
		 const lv_style_t * style = lv_imgbar_get_style(imgbar, LV_IMGBAR_STYLE_BG);
		 header.h = lv_font_get_line_height(style->text.font);
		 header.w = lv_txt_get_width(src, strlen(src), style->text.font, style->text.letter_space, LV_TXT_FLAG_NONE);
		 header.always_zero = 0;
		 header.cf = LV_IMG_CF_ALPHA_1BIT;
	} else {
		 info_res = lv_img_decoder_get_info(src, &header);
	}

	if(info_res == LV_RES_OK) {
		 lv_obj_set_size(imgbar, header.w, header.h);
	}

	lv_obj_invalidate(imgbar);
}

/**
 * Set image for the indicator of the image bar
 * @param imgbar pointer to an image bar object
 * @param src pointer to an image source (a C array or path to a file)
 */
void lv_imgbar_set_indic_src(lv_obj_t *imgbar, const void *src)
{
	LV_ASSERT_OBJ(imgbar, LV_OBJX_NAME);

	lv_imgbar_ext_t * ext = lv_obj_get_ext_attr(imgbar);
	ext->src_indic = src;

	lv_img_header_t header;

	lv_res_t info_res = LV_RES_OK;
	if(lv_img_src_get_type(src) == LV_IMG_SRC_SYMBOL) {
		 const lv_style_t * style = lv_imgbar_get_style(imgbar, LV_IMGBAR_STYLE_BG);
		 header.h = lv_font_get_line_height(style->text.font);
		 header.w = lv_txt_get_width(src, strlen(src), style->text.font, style->text.letter_space, LV_TXT_FLAG_NONE);
		 header.always_zero = 0;
		 header.cf = LV_IMG_CF_ALPHA_1BIT;
	} else {
		 info_res = lv_img_decoder_get_info(src, &header);
	}

	if(info_res == LV_RES_OK) {
		 lv_obj_set_height(imgbar, header.h);
	}

	lv_obj_invalidate(imgbar);
}

/**
 * Set a style of a imgbarate.
 * @param imgbar pointer to imgbarate object
 * @param type which style should be set
 * @param style pointer to a style
 */
void lv_imgbar_set_style(lv_obj_t * imgbar, lv_imgbar_style_t type, const lv_style_t * style)
{
	 LV_ASSERT_OBJ(imgbar, LV_OBJX_NAME);

	 lv_bar_set_style(imgbar, type, style);
}

/*=====================
 * Getter functions
 *====================*/

/*
 * New object specific "get" functions come here
 */

/**
 * Get the image of the background
 * @param imgslider pointer to an image bar object
 * @return pointer to an image source (a C array or path to a file)
 */
const void *lv_imgbar_get_bg_src(lv_obj_t *imgbar)
{
	LV_ASSERT_OBJ(imgbar, LV_OBJX_NAME);

	lv_imgbar_ext_t * ext = lv_obj_get_ext_attr(imgbar);

	return ext->src_bg;
}

/**
 * Get the image of the indicator
 * @param imgslider pointer to an image bar object
 * @return pointer to an image source (a C array or path to a file)
 */
const void *lv_imgbar_get_indic_src(lv_obj_t *imgbar)
{
	LV_ASSERT_OBJ(imgbar, LV_OBJX_NAME);

	lv_imgbar_ext_t * ext = lv_obj_get_ext_attr(imgbar);

	return ext->src_indic;
}

/**
 * Get style of a imgbarate.
 * @param imgbar pointer to imgbarate object
 * @param type which style should be get
 * @return style pointer to the style
 */
lv_style_t * lv_imgbar_get_style(const lv_obj_t * imgbar, lv_imgbar_style_t type)
{
	 LV_ASSERT_OBJ(imgbar, LV_OBJX_NAME);

	 return lv_bar_get_style(imgbar, type);
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
 * Handle the drawing related tasks of the imgbarates
 * @param imgbar pointer to an object
 * @param mask the object will be drawn only in this area
 * @param mode LV_DESIGN_COVER_CHK: only check if the object fully covers the 'mask_p' area
 *                                  (return 'true' if yes)
 *             LV_DESIGN_DRAW: draw the object (always return 'true')
 *             LV_DESIGN_DRAW_POST: drawing after every children are drawn
 * @param return true/false, depends on 'mode'
 */
static bool lv_imgbar_design(lv_obj_t * imgbar, const lv_area_t * mask, lv_design_mode_t mode)
{
	 /*Return false if the object is not covers the mask_p area*/
	 if(mode == LV_DESIGN_COVER_CHK) {
		  return false;
	 }
	 /*Draw the object*/
	 else if(mode == LV_DESIGN_DRAW_MAIN) {
		 lv_opa_t opa_scale = lv_obj_get_opa_scale(imgbar);
		 lv_imgbar_ext_t * ext = lv_obj_get_ext_attr(imgbar);

		 const lv_img_dsc_t *dsc_bg  = ext->src_bg;
		 const lv_img_dsc_t *dsc_indic = ext->src_indic;

		 const lv_style_t * style_bg = lv_bar_get_style(imgbar, LV_BAR_STYLE_BG);
		 const lv_style_t * style_indic = lv_bar_get_style(imgbar, LV_BAR_STYLE_INDIC);
		 lv_area_t bg_area;
		 lv_area_copy(&bg_area, &imgbar->coords);

		 lv_coord_t w = lv_area_get_width(&imgbar->coords);
		 if (dsc_indic)
			 w = dsc_indic->header.w;
		 else if (dsc_bg)
			 w = dsc_bg->header.w;

		 if (dsc_bg) {
			bg_area.x2 = bg_area.x1 + dsc_bg->header.w - 1;
			bg_area.y2 = bg_area.y1 + dsc_bg->header.h - 1;
			lv_draw_img(&bg_area, mask, dsc_bg, style_bg, opa_scale);
		 }

		 lv_area_t indic_area;
		 lv_area_copy(&indic_area, &imgbar->coords);

		 if (dsc_indic) {
			 int32_t width = (int32_t)((int32_t)w * (ext->bar.cur_value - ext->bar.min_value)) / (ext->bar.max_value - ext->bar.min_value);
			 indic_area.x2 = indic_area.x1 + width - 1;
			 indic_area.y2 = indic_area.y1 + dsc_bg->header.h - 1;
			 lv_draw_img(&bg_area, &indic_area, dsc_indic, style_bg, opa_scale);
		 }
	 }
	 /*Post draw when the children are drawn*/
	 else if(mode == LV_DESIGN_DRAW_POST) {
	 }

	 return true;
}

/**
 * Signal function of the imgbarate
 * @param imgbar pointer to a imgbarate object
 * @param sign a signal type from lv_signal_t enum
 * @param param pointer to a signal specific variable
 * @return LV_RES_OK: the object is not deleted in the function; LV_RES_INV: the object is deleted
 */
static lv_res_t lv_imgbar_signal(lv_obj_t * imgbar, lv_signal_t sign, void * param)
{
	 lv_res_t res;

	 /* Include the ancient signal function */
	 res = ancestor_signal(imgbar, sign, param);
	 if(res != LV_RES_OK) return res;
	 if(sign == LV_SIGNAL_GET_TYPE) return lv_obj_handle_get_type_signal(param, LV_OBJX_NAME);

	 if(sign == LV_SIGNAL_CLEANUP) {
		  /*Nothing to cleanup. (No dynamically allocated memory in 'ext')*/
	 }

	 if (sign == LV_SIGNAL_CORD_CHG) {
		 lv_imgbar_ext_t * ext = lv_obj_get_ext_attr(imgbar);
		 const lv_img_dsc_t *dsc_bg  = ext->src_bg;
		 if (dsc_bg)
			lv_obj_set_size(imgbar, dsc_bg->header.w, dsc_bg->header.h);
	 }

	 return res;
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
