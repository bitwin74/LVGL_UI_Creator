/**
 * @file setting.h
 *
 */

#ifndef _SETTING_H_
#define _SETTING_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lvgl.h"
#include "lv_ex_conf.h"
#else
#include "./lvgl/lvgl.h"
#include "./lv_ex_conf.h"
#endif



/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void setting_win_init(lv_obj_t * parent);
void setting_attr_mod(lv_obj_t * obj);
void lb_selected_mod(lv_obj_t * obj);
/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
