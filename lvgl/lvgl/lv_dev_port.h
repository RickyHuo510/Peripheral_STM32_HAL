/*
 * lv_dev_port.h
 *
 *  Created on: Jul 27, 2025
 *      Author: RickyHuo
 */

#ifndef LV_DEV_PORT_H_
#define LV_DEV_PORT_H_

#include "main.h"
#include "lvgl.h"

void lv_port_display_registration(void);
void lv_port_indev_registration(void);

void lv_example_get_started_1(void);//FROM OFFCIAL WEBSITE
void lvgl_first_demo_start(void);//COPIED FROM CSDN
void lv_example_get_started_4(void);

#define Screen_X 240
#define Screen_Y 320

extern lv_display_t* display1;
extern lv_indev_t * indev1;
extern lv_obj_t * cursor_obj;

#endif /* LV_DEV_PORT_H_ */
