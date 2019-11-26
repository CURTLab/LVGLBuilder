#ifndef LV_IMGSLIDER_H
#define LV_IMGSLIDER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_conf.h"
#else
#include "../../../lv_conf.h"
#endif

#if LV_USE_IMGSLIDER != 0

/*Testing of dependencies*/
#if LV_USE_SLIDER == 0
#error "lv_imgslider: lv_slider is required. Enable it in lv_conf.h (LV_USE_SLIDER  1) "
#endif

#include "../lv_core/lv_obj.h"
#include "lv_slider.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
/*Data of image slider*/
typedef struct
{
	lv_slider_ext_t slider; /*Ext. of ancestor*/
	/*New data for this type */
	const void *src_bg;
	const void *src_indic;
	const void *src_knob;
	lv_point_t offset_knob;
} lv_imgslider_ext_t;

/*Styles*/
enum {
	 LV_IMGSLIDER_STYLE_BG, /** Slider background style. */
	 LV_IMGSLIDER_STYLE_INDIC, /** Slider indicator (filled area) style. */
	 LV_IMGSLIDER_STYLE_KNOB, /** Slider knob style. */
};
typedef uint8_t lv_imgslider_style_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a image slider objects
 * @param par pointer to an object, it will be the parent of the new image slider
 * @param copy pointer to a image slider object, if not NULL then the new object will be copied from it
 * @return pointer to the created image slider
 */
lv_obj_t * lv_imgslider_create(lv_obj_t * par, const lv_obj_t * copy);

/*======================
 * Add/remove functions
 *=====================*/

/*=====================
 * Setter functions
 *====================*/

/**
 * Set image for the background of the image slider
 * @param imgslider pointer to an image slider object
 * @param src pointer to an image source (a C array or path to a file)
 */
void lv_imgslider_set_bg_src(lv_obj_t * imgslider, const void * src);

/**
 * Set image for the indicator of the image slider
 * @param imgslider pointer to an image slider object
 * @param src pointer to an image source (a C array or path to a file)
 */
void lv_imgslider_set_indic_src(lv_obj_t * imgslider, const void * src);

/**
 * Set image for the knob of the image slider
 * @param imgslider pointer to an image slider object
 * @param src pointer to an image source (a C array or path to a file)
 */
void lv_imgslider_set_knob_src(lv_obj_t * imgslider, const void * src);

/**
 * Set a new value on the image slider
 * @param slider pointer to a imgslider object
 * @param value new value
 * @param anim LV_ANIM_ON: set the value with an animation; LV_ANIM_OFF: change the value immediately
 */
static inline void lv_imgslider_set_value(lv_obj_t * slider, int16_t value, lv_anim_enable_t anim)
{
	 lv_bar_set_value(slider, value, anim);
}

/**
 * Set minimum and the maximum values of a image slider
 * @param slider pointer to the imgslider object
 * @param min minimum value
 * @param max maximum value
 */
static inline void lv_imgslider_set_range(lv_obj_t * slider, int16_t min, int16_t max)
{
	 lv_bar_set_range(slider, min, max);
}

/**
 * Set an offset for the knob.
 * @param imgslide pointer to an image slider
 * @param x: the new offset along x axis.
 */
void lv_imgslider_set_offset_x(lv_obj_t * img, lv_coord_t x);

/**
 * Set an offset for the knob.
 * @param imgslide pointer to an image slider
 * @param y: the new offset along y axis.
 */
void lv_imgslider_set_offset_y(lv_obj_t * img, lv_coord_t y);

/**
 * Set a style of a image slider.
 * @param templ pointer to image slider object
 * @param type which style should be set
 * @param style pointer to a style
 */
void lv_imgslider_set_style(lv_obj_t * templ, lv_imgslider_style_t type, const lv_style_t * style);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the image of the background
 * @param imgslider pointer to an image slider object
 * @return pointer to an image source (a C array or path to a file)
 */
const void * lv_imgslide_get_bg_src(lv_obj_t * imgslider);

/**
 * Get the image of the indicator
 * @param imgslider pointer to an image slider object
 * @return pointer to an image source (a C array or path to a file)
 */
const void * lv_imgslide_get_indic_src(lv_obj_t * imgslider);

/**
 * Get the image of the knob
 * @param imgslider pointer to an image slider object
 * @return pointer to an image source (a C array or path to a file)
 */
const void * lv_imgslide_get_knob_src(lv_obj_t * imgslider);

/**
 * Get the value of a slider
 * @param slider pointer to a slider object
 * @return the value of the slider
 */
static inline int16_t lv_imgslider_get_value(const lv_obj_t * slider)
{
	return lv_slider_get_value(slider);
}

/**
 * Get the minimum value of a slider
 * @param slider pointer to a slider object
 * @return the minimum value of the slider
 */
static inline int16_t lv_imgslider_get_min_value(const lv_obj_t * slider)
{
	 return lv_bar_get_min_value(slider);
}

/**
 * Get the maximum value of a slider
 * @param slider pointer to a slider object
 * @return the maximum value of the slider
 */
static inline int16_t lv_imgslider_get_max_value(const lv_obj_t * slider)
{
	 return lv_bar_get_max_value(slider);
}

/**
 * Get the offset.x attribute of the image slider knob.
 * @param imgslide pointer to an image slider
 * @return offset.x value.
 */
lv_coord_t lv_imgslider_get_offset_x(lv_obj_t * imgslider);

/**
 * Get the offset.y attribute of the image slider knob.
 * @param imgslide pointer to an image slider
 * @return offset.y value.
 */
lv_coord_t lv_imgslider_get_offset_y(lv_obj_t * imgslider);

/**
 * Get style of a image slider.
 * @param templ pointer to image slider object
 * @param type which style should be get
 * @return style pointer to the style
 */
lv_style_t * lv_imgslider_get_style(const lv_obj_t * templ, lv_imgslider_style_t type);

/*=====================
 * Other functions
 *====================*/

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_IMGBAR*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LV_IMGSLIDER_H
