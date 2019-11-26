#ifndef LV_IMGBAR_H
#define LV_IMGBAR_H

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

#if LV_USE_IMGBAR != 0

/*Testing of dependencies*/
#if LV_USE_BAR == 0
#error "lv_imgbar: lv_bar is required. Enable it in lv_conf.h (LV_USE_BAR  1) "
#endif

#include "../lv_core/lv_obj.h"
#include "lv_bar.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
/*Data of imgbarate*/
typedef struct
{
	 lv_bar_ext_t bar; /*Ext. of ancestor*/
	 /*New data for this type */
	 const void *src_bg;
	 const void *src_indic;
} lv_imgbar_ext_t;

/*Styles*/
enum {
	 LV_IMGBAR_STYLE_BG,
	 LV_IMGBAR_STYLE_INDIC,
};
typedef uint8_t lv_imgbar_style_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a imgbarate objects
 * @param par pointer to an object, it will be the parent of the new imgbarate
 * @param copy pointer to a imgbarate object, if not NULL then the new object will be copied from it
 * @return pointer to the created imgbarate
 */
lv_obj_t * lv_imgbar_create(lv_obj_t * par, const lv_obj_t * copy);

/*======================
 * Add/remove functions
 *=====================*/

/*=====================
 * Setter functions
 *====================*/

/**
 * Set image for the background of the image bar
 * @param imgbar pointer to an image bar object
 * @param src pointer to an image source (a C array or path to a file)
 */
void lv_imgbar_set_bg_src(lv_obj_t * imgbar, const void * src);

/**
 * Set image for the indicator of the image bar
 * @param imgbar pointer to an image bar object
 * @param src pointer to an image source (a C array or path to a file)
 */
void lv_imgbar_set_indic_src(lv_obj_t * imgbar, const void * src);

/**
 * Set a new value on the image bar
 * @param imgbar pointer to the image bar object
 * @param value new value
 * @param anim LV_ANIM_ON: set the value with an animation; LV_ANIM_OFF: change the value immediately
 */
static inline void lv_imgbar_set_value(lv_obj_t * imgbar, int16_t value, lv_anim_enable_t anim)
{
	 lv_bar_set_value(imgbar, value, anim);
}

/**
 * Set minimum and the maximum values of a image bar
 * @param imgbar pointer to the image bar object
 * @param min minimum value
 * @param max maximum value
 */
static inline void lv_imgbar_set_range(lv_obj_t * imgbar, int16_t min, int16_t max)
{
	 lv_bar_set_range(imgbar, min, max);
}

/**
 * Set a style of a imgbarate.
 * @param imgbar pointer to imgbarate object
 * @param type which style should be set
 * @param style pointer to a style
 */
void lv_imgbar_set_style(lv_obj_t * imgbar, lv_imgbar_style_t type, const lv_style_t * style);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get the image of the background
 * @param imgslider pointer to an image bar object
 * @return pointer to an image source (a C array or path to a file)
 */
const void * lv_imgbar_get_bg_src(lv_obj_t * imgbar);

/**
 * Get the image of the indicator
 * @param imgslider pointer to an image bar object
 * @return pointer to an image source (a C array or path to a file)
 */
const void * lv_imgbar_get_indic_src(lv_obj_t * imgbar);

/**
 * Get the value of a image bar
 * @param imgbar pointer to a image bar object
 * @return the value of the image bar
 */
static inline int16_t lv_imgbar_get_value(const lv_obj_t * imgbar)
{
	return lv_bar_get_value(imgbar);
}

/**
 * Get the minimum value of a image bar
 * @param imgbar pointer to a image bar object
 * @return the minimum value of the image bar
 */
static inline int16_t lv_imgbar_get_min_value(const lv_obj_t * imgbar)
{
	 return lv_bar_get_min_value(imgbar);
}

/**
 * Get the maximum value of a image bar
 * @param imgbar pointer to a image bar object
 * @return the maximum value of the image bar
 */
static inline int16_t lv_imgbar_get_max_value(const lv_obj_t * imgbar)
{
	 return lv_bar_get_max_value(imgbar);
}

/**
 * Get style of a imgbarate.
 * @param imgbar pointer to imgbarate object
 * @param type which style should be get
 * @return style pointer to the style
 */
lv_style_t * lv_imgbar_get_style(const lv_obj_t * imgbar, lv_imgbar_style_t type);

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

#endif // LV_IMGBAR_H
