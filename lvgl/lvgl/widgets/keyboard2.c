/*
 * keyboard2.c
 *
 *  Created on: Aug 28, 2025
 *      Author: RickyHuo
 */


#include "widgets.h"
#include "lvgl.h"
#include <string.h>

#if LV_USE_KEYBOARD && LV_BUILD_EXAMPLES

static char char_arr[256];
static uint8_t ta_change_status=0;
static lv_obj_t * ta;



static void text_area_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta1 = lv_event_get_target_obj(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        const char * text = lv_textarea_get_text(ta1);
        if (text != NULL) {
            strncpy(char_arr, text, strlen(text)+1);
            char_arr[strlen(text)+1] = '\0';

            if(ta == ta1)ta_change_status=1;
        }
    }
}

void lv_example_keyboard_2(lv_obj_t *parent)
{
    /*Create an AZERTY keyboard map*/
    static const char * kb_map[] = {"A", "Z", "E", "R", "T", "Y", "U", "I", "O", "P", LV_SYMBOL_BACKSPACE, "\n",
                                    "Q", "S", "D", "F", "G", "J", "K", "L", "M",  LV_SYMBOL_NEW_LINE, "\n",
                                    "W", "X", "C", "V", "B", "N", ",", ".", ":", "!", "?", "\n",
                                    LV_SYMBOL_CLOSE, " ",  " ", " ", LV_SYMBOL_OK, NULL
                                   };

    /*Set the relative width of the buttons and other controls*/
    static const lv_buttonmatrix_ctrl_t kb_ctrl[] = {LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_6,
                                                     LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_6,
                                                     LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4, LV_BUTTONMATRIX_CTRL_WIDTH_4,
                                                     LV_BUTTONMATRIX_CTRL_WIDTH_2, (lv_buttonmatrix_ctrl_t)(LV_BUTTONMATRIX_CTRL_HIDDEN | LV_BUTTONMATRIX_CTRL_WIDTH_2), LV_BUTTONMATRIX_CTRL_WIDTH_6, (lv_buttonmatrix_ctrl_t)(LV_BUTTONMATRIX_CTRL_HIDDEN | LV_BUTTONMATRIX_CTRL_WIDTH_2), LV_BUTTONMATRIX_CTRL_WIDTH_2
                                                    };

    /*Create a keyboard and add the new map as USER_1 mode*/
    //lv_obj_t * kb = lv_keyboard_create(lv_screen_active());
    lv_obj_t * kb = lv_keyboard_create(parent);


    lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
    lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_USER_1);

    /*Create a text area. The keyboard will write here*/

    //ta = lv_textarea_create(lv_screen_active());
    ta = lv_textarea_create(parent);
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_size(ta, lv_pct(90), 80);
    lv_obj_add_state(ta, LV_STATE_FOCUSED);


    lv_obj_add_event_cb(ta, text_area_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_keyboard_set_textarea(kb, ta);
}


int get_keyboard2_input(char * arr, int* which_text_area)
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
