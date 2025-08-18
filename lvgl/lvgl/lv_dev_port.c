/*
 * lv_dev_port.c
 *
 *  Created on: Jul 27, 2025
 *      Author: RickyHuo
 */

#include "lv_dev_port.h"
#include "tftlcd.h"
#include "main.h"
#include "lvgl.h"
#include "touch.h"
/*
 * 1. MUST TIMELY FEED lv_tick_inc(1) in an interrupt with HIGH PRIORITY
 *
 * 2. Before calling lv_ functions, must call lv_init() first. Or a HARDFAULT ERROR will be RAISED
 *
 * 3. NEVER FORGET TO CALL HAL_TIM_Base_START_IT() to start up the timer
 *
 * 4. PUT lv_task_handler() in while(1)
 */

/*****************************BUFFER DEFINES*******************************************/
#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565))
static uint8_t buf1[320 * 240 / 10 * BYTES_PER_PIXEL];


/*******************DEV DRIVERS***************************************************/
static void my_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    /* The most simple case (also the slowest) to send all rendered pixels to the
     * screen one-by-one.  `put_px` is just an example.  It needs to be implemented by you. */
    uint16_t * buf16 = (uint16_t *)px_map; /* Let's say it's a 16 bit (RGB565) display */
    int32_t x, y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
        	LCD_Fast_DrawPoint(x,y, *buf16);
            buf16++;
        }
    }

    /* IMPORTANT!!!
     * Inform LVGL that flushing is complete so buffer can be modified again. */
    lv_display_flush_ready(display);
}

static void my_input_read(lv_indev_t * indev, lv_indev_data_t * data)
{
	tp_dev.scan(0);
    if(tp_dev.sta&TP_PRES_DOWN) {
        data->point.x = tp_dev.x[0];
        data->point.y = tp_dev.y[0];
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

/*****************************************************************************************/
lv_display_t* display1;
void lv_port_display_registration(void)
{
	display1= lv_display_create(Screen_X, Screen_Y);
	lv_display_set_buffers(display1, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
	lv_display_set_flush_cb(display1, my_flush_cb);
}

/***************************************************************************************/
lv_indev_t * indev1;
lv_obj_t * cursor_obj;
void lv_port_indev_registration(void)
{
	  /* Create and set up at least one display before you register any input devices. */
	  indev1 = lv_indev_create();        /* Create input device connected to Default Display. */
	  lv_indev_set_type(indev1, LV_INDEV_TYPE_POINTER);   /* Touch pad is a pointer-like device. */
	  lv_indev_set_read_cb(indev1, my_input_read);    /* Set driver function. */

	  LV_IMAGE_DECLARE(mouse_cursor_icon);                          /* Declare the image source. */
	  lv_obj_t * cursor_obj = lv_image_create(lv_screen_active());  /* Create image Widget for cursor. */
	  lv_image_set_src(cursor_obj, &mouse_cursor_icon);             /* Set image source. */
	  lv_indev_set_cursor(indev1, cursor_obj);                 /* Connect image to Input Device. */
}


/************************************************************************************************/
/***************************************EXAMPLE DEMOS*****************************************/

/*
 * Word display
 */
void lv_example_get_started_1(void)
{
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57), LV_PART_MAIN);

    /*Create a white label, set its text and align it to the center*/
    lv_obj_t * label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "Hello world");
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}




/*
 * Button self-increment test
 * 按钮自增 演示
 */
static void btn_event_cb(lv_event_t * event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_event_code_t code = lv_event_get_code(event);//获得事件最初瞄准的对象。即使事件是冒泡的，也是一样的。
    if(code == LV_EVENT_CLICKED)
    {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, NULL);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}
void lvgl_first_demo_start(void)
{
    lv_obj_t * btn = lv_btn_create(lv_scr_act());           /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, (lv_event_cb_t)btn_event_cb, LV_EVENT_CLICKED, NULL);/*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);                /*Add a label to the button*/
    lv_label_set_text(label, "Yeah");                       /*Set the labels text*/


    lv_obj_t * label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, "Hello world!");
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align_to(btn, label1, LV_ALIGN_OUT_TOP_MID, 0, -10);
}


/**
 * Create a slider and write its value on a label.
 * 滑动进度条 演示
 */
static lv_obj_t * label;
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target_obj(e);

    /*Refresh the text*/
    lv_label_set_text_fmt(label, "%" LV_PRId32, lv_slider_get_value(slider));
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/
}
void lv_example_get_started_4(void)
{
    /*Create a slider in the center of the display*/
    lv_obj_t * slider = lv_slider_create(lv_screen_active());
    lv_obj_set_width(slider, 200);                          /*Set the width*/
    lv_obj_center(slider);                                  /*Align to the center of the parent (screen)*/
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);     /*Assign an event function*/

    /*Create a label above the slider*/
    label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "0");
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);    /*Align top of the slider*/
}




//#if LV_USE_KEYBOARD && LV_BUILD_EXAMPLES
//
//static void ta_event_cb(lv_event_t * e)
//{
//    lv_event_code_t code = lv_event_get_code(e);
//    lv_obj_t * ta = lv_event_get_target_obj(e);
//    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);
//    if(code == LV_EVENT_FOCUSED) {
//        lv_keyboard_set_textarea(kb, ta);
//        lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
//    }
//
//    if(code == LV_EVENT_DEFOCUSED) {
//        lv_keyboard_set_textarea(kb, NULL);
//        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
//    }
//}
//
//void lv_example_keyboard_1(void)
//{
//    /*Create a keyboard to use it with an of the text areas*/
//    lv_obj_t * kb = lv_keyboard_create(lv_screen_active());
//
//    /*Create a text area. The keyboard will write here*/
//    lv_obj_t * ta1;
//    ta1 = lv_textarea_create(lv_screen_active());
//    lv_obj_align(ta1, LV_ALIGN_TOP_LEFT, 10, 10);
//    lv_obj_add_event_cb(ta1, ta_event_cb, LV_EVENT_ALL, kb);
//    lv_textarea_set_placeholder_text(ta1, "Hello");
//    lv_obj_set_size(ta1, 140, 80);
//
//    lv_obj_t * ta2;
//    ta2 = lv_textarea_create(lv_screen_active());
//    lv_obj_align(ta2, LV_ALIGN_TOP_RIGHT, -10, 10);
//    lv_obj_add_event_cb(ta2, ta_event_cb, LV_EVENT_ALL, kb);
//    lv_obj_set_size(ta2, 140, 80);
//
//    lv_keyboard_set_textarea(kb, ta1);
//
//    /*The keyboard will show Arabic characters if they are enabled */
//#if LV_USE_ARABIC_PERSIAN_CHARS && LV_FONT_DEJAVU_16_PERSIAN_HEBREW
//    lv_obj_set_style_text_font(kb, &lv_font_dejavu_16_persian_hebrew, 0);
//    lv_obj_set_style_text_font(ta1, &lv_font_dejavu_16_persian_hebrew, 0);
//    lv_obj_set_style_text_font(ta2, &lv_font_dejavu_16_persian_hebrew, 0);
//#endif
//}
//#endif
