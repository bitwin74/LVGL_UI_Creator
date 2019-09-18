/**
 * @setting.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "./lvgl/lvgl.h"
#include <stdlib.h>
#include <stdio.h>
#include "setting.h"
#include "dataset.h"
#include "custom_widget.h"
#include "gencode.h"
#include "loadproj.h"
#include "saveproj.h"

#if LV_EX_KEYBOARD || LV_EX_MOUSEWHEEL
#include "lv_drv_conf.h"
#endif

#if LV_EX_KEYBOARD
#include "lv_drivers/indev/keyboard.h"
#endif

#if LV_EX_MOUSEWHEEL
#include "lv_drivers/indev/mousewheel.h"
#endif


/*********************
 *      DEFINES
 *********************/


/**********************
 *      EXTERN
 **********************/
extern lv_obj_t * setting_win;
extern lv_obj_t * tft_win;
/**********************
 *      TYPEDEFS
 **********************/
typedef struct
{
    lv_obj_t * obj_selected;
    lv_obj_t * pos_x;
    lv_obj_t * pos_y;
    lv_obj_t * size_h;
    lv_obj_t * size_w;
    lv_obj_t * drag;
    lv_obj_t * click;
    
}setting_attr_panel_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void setting_indev_init(lv_group_t * g);
static void saveproj_cb(lv_obj_t * obj, lv_event_t ev);
static void loadproj_cb(lv_obj_t * obj, lv_event_t ev);
/**********************
 *  STATIC VARIABLES
 **********************/
static setting_attr_panel_t base_attr;
/**********************
 *      MACROS
 **********************/


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void setting_win_init(lv_obj_t * parent)
{
    setting_win = lv_win_create(parent, NULL);
    lv_win_set_title(setting_win, "Setting");
    lv_obj_set_size(setting_win, lv_obj_get_width_fit(parent) / 4.5 , lv_obj_get_height(parent));
    lv_obj_align(setting_win, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);
    lv_page_set_scrl_fit2(lv_win_get_content(setting_win), LV_FIT_FLOOD, LV_FIT_TIGHT);
    lv_page_set_scrl_layout(lv_win_get_content(setting_win), LV_LAYOUT_PRETTY);

    lv_obj_t * win_btn = lv_win_add_btn(setting_win, LV_SYMBOL_DOWNLOAD);
    lv_obj_set_protect(win_btn, LV_PROTECT_CLICK_FOCUS);
    lv_obj_set_event_cb(win_btn, saveproj_cb);

    win_btn = lv_win_add_btn(setting_win, LV_SYMBOL_UPLOAD);
    lv_obj_set_protect(win_btn, LV_PROTECT_CLICK_FOCUS);
    lv_obj_set_event_cb(win_btn, loadproj_cb);    

    lv_group_t * g = lv_group_create();
    setting_indev_init(g);

    //ID


    //POSITION
    lv_obj_t * title = lv_label_create(setting_win, NULL);
    lv_label_set_text(title, "\n\n\n[Postion]");
    lv_obj_t * cont_x = (lv_obj_t *)tbox_create(setting_win, "X: ");
    lv_obj_t * cont_y = (lv_obj_t *)tbox_create(setting_win, "Y: ");
    // base_attr.pos_x = tbox_get_ta(cont_x);
    // base_attr.pos_y = tbox_get_ta(cont_y);
    // lv_group_add_obj(g, base_attr.pos_x);
    // lv_group_add_obj(g, base_attr.pos_y);


    //SIZE
    title = lv_label_create(setting_win, NULL);
    lv_label_set_text(title, "[Size]");
    lv_obj_t * cont_h = tbox_create(setting_win, "Height:");
    lv_obj_t * cont_w = tbox_create(setting_win, "Width:");
    base_attr.size_h = tbox_get_ta(cont_h);
    base_attr.size_w = tbox_get_ta(cont_w);
    
    //DRAG && CLICK
    lv_obj_t * cb_drag = lv_cb_create(setting_win, NULL);
    lv_cb_set_text(cb_drag, "Drag     ");

    lv_obj_t * cb_click = lv_cb_create(setting_win, cb_drag);
    lv_cb_set_text(cb_click, "Click    ");
    lv_obj_align(cb_click, cb_drag, LV_ALIGN_OUT_RIGHT_MID, 20, 0); 
    
    //SElECTED
    base_attr.obj_selected = lv_label_create(setting_win, NULL);

    //Layer View
    layerview_init(setting_win);
    // lv_obj_t * lav1 = layerview_create(setting_win, "Screen1");
    // lv_obj_t * b = layerview_create(lav1, "Button");
    // layerview_create(b, "Label");
    // layerview_create(lav1, "Bar"LV_SYMBOL_OK);  

}

void setting_attr_mod(lv_obj_t * obj)
{
    lv_coord_t x = lv_obj_get_x(obj);
    char x_str[8];
    snprintf(x_str, 8, "%d", x);
    lv_ta_set_text(base_attr.pos_x, x_str);
    lv_coord_t y = lv_obj_get_y(obj);
    char y_str[8];
    snprintf(y_str, 8, "%d", y);    
    lv_ta_set_text(base_attr.pos_y, y_str);
}

void lb_selected_mod(lv_obj_t * obj)
{
    widget_info_t * info = (widget_info_t * )lv_obj_get_user_data(obj);
    char str[30];
    snprintf(str, 29, "(%s)%x", widget_get_type_name(info->type), obj);
    lv_label_set_text(base_attr.obj_selected, str);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void setting_indev_init(lv_group_t * g)
{
#if LV_EX_KEYBOARD
    lv_indev_drv_t real_kb_drv;
    lv_indev_drv_init(&real_kb_drv);
    real_kb_drv.type = LV_INDEV_TYPE_KEYPAD;
    real_kb_drv.read_cb = keyboard_read;
    lv_indev_t * real_kb_indev = lv_indev_drv_register(&real_kb_drv);
    lv_indev_set_group(real_kb_indev, g);
#endif

#if LV_EX_MOUSEWHEEL
    lv_indev_drv_t enc_drv;
    lv_indev_drv_init(&enc_drv);
    enc_drv.type = LV_INDEV_TYPE_ENCODER;
    enc_drv.read_cb = mousewheel_read;
    lv_indev_t * enc_indev = lv_indev_drv_register(&enc_drv);
    lv_indev_set_group(enc_indev, g);
#endif
}


static void saveproj_cb(lv_obj_t * obj, lv_event_t ev)
{
    if(ev == LV_EVENT_CLICKED)
    {
        // save_project(tft_win);
    }
}

static void loadproj_cb(lv_obj_t * obj, lv_event_t ev)
{
    if(ev == LV_EVENT_CLICKED)
    {
        load_project(tft_win);
    }
}