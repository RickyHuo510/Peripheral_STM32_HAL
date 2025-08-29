/*
 * keyboard1.c
 *
 *  Created on: Aug 28, 2025
 *      Author: RickyHuo
 */


#include "widgets.h"
#include "lvgl.h"
#include <string.h>
#if LV_USE_KEYBOARD && LV_BUILD_EXAMPLES

static char char_arr[128];
static uint8_t ta_change_status=0;

static lv_obj_t * ta1;
static lv_obj_t * ta2;

static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target_obj(e);
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }

    if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
    if(code == LV_EVENT_VALUE_CHANGED)
    {
    	const char * text = lv_textarea_get_text(ta);
    	if(text!=NULL)
    	{
    		if(ta == ta1)ta_change_status=1;
    		else if(ta == ta2)ta_change_status=2;
    		strncpy(char_arr, text, strlen(text)+1);
    		char_arr[strlen(text)+1]='\0';
    	}
    }
}

void lv_example_keyboard_1(void)
{
    /*Create a keyboard to use it with an of the text areas*/
    lv_obj_t * kb = lv_keyboard_create(lv_screen_active());

    /*Create a text area. The keyboard will write here*/

    ta1 = lv_textarea_create(lv_screen_active());
    lv_obj_align(ta1, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(ta1, ta_event_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_placeholder_text(ta1, "Hello");
    lv_obj_set_size(ta1, 140, 80);


    ta2 = lv_textarea_create(lv_screen_active());
    lv_obj_align(ta2, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(ta2, ta_event_cb, LV_EVENT_ALL, kb);
    lv_obj_set_size(ta2, 140, 80);

    lv_keyboard_set_textarea(kb, ta1);

    /*The keyboard will show Arabic characters if they are enabled */
#if LV_USE_ARABIC_PERSIAN_CHARS && LV_FONT_DEJAVU_16_PERSIAN_HEBREW
    lv_obj_set_style_text_font(kb, &lv_font_dejavu_16_persian_hebrew, 0);
    lv_obj_set_style_text_font(ta1, &lv_font_dejavu_16_persian_hebrew, 0);
    lv_obj_set_style_text_font(ta2, &lv_font_dejavu_16_persian_hebrew, 0);
#endif
}

int get_keyboard1_input(char* arr, int* which_text_area)
{
	if(ta_change_status)
	{
		strncpy(arr, char_arr, strlen(char_arr));
		*which_text_area=(int)ta_change_status;
		ta_change_status=0;
		return 1;
	}
	else
	{
		arr[0]='\0';
		return 0;
	}
	return 0;
}
#endif
