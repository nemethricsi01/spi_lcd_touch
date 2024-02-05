/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

// This demo UI is adapted from LVGL official example: https://docs.lvgl.io/master/widgets/extra/meter.html#simple-meter

#include "lvgl.h"
#include "esp_log.h"


static lv_obj_t *meter;
static lv_obj_t * btn;
static lv_disp_rot_t rotation = LV_DISP_ROT_NONE;
static const char * btnm_map[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9","\n",
                                  "10", "11", "12", "13", "14", "\n",
                                  "Action1", "Action2", ""
                                 };

static void set_temp(void * bar, int32_t temp)
{
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

/**
 * A temperature meter example
 */
void lv_example_bar_3(void)
{


    // lv_obj_t *bars [23];
    // uint16_t x = 10;


    // int xs[24];  // Declare an array with 24 elements

    // int value = 0;  // Initialize the starting value

    // for (int i = 0; i < 24; i++) {
    //     xs[i] = value;  // Store the current value in the array

    //     // Check if every 3 steps have been completed, and increment accordingly
    //     if ((i + 1) % 3 == 0) {
    //         value += 19;  // Increment by 15
    //     } else {
    //         value += 10;   // Increment by 8
    //     }
    // }

    // lv_obj_t **instances = (lv_obj_t **)malloc(24 * sizeof(lv_obj_t*));


    // for(int i = 0; i<23;i++)
    // {
    //     instances[i] = lv_bar_create(lv_scr_act());
    //     lv_obj_set_size(instances[i], 5, 200);
    //     lv_obj_align(instances[i],LV_ALIGN_BOTTOM_LEFT,xs[i+1],0);
    //     lv_bar_set_range(instances[i], 0, 255);
    //     lv_bar_set_value(instances[i],128,LV_ANIM_OFF);
    //     x = x +10;
        
    // }

    // lv_obj_t * bar1 = lv_bar_create(lv_scr_act());
    // lv_obj_set_size(bar1, 10, 200);
    // lv_obj_align(bar1,LV_ALIGN_BOTTOM_LEFT,300,0);
    // lv_bar_set_range(bar1, 0, 255);
    // lv_bar_set_value(bar1,128,LV_ANIM_OFF);

    // lv_obj_t * bar2 = lv_bar_create(lv_scr_act());
    // lv_obj_set_size(bar2, 10, 200);
    // lv_obj_align(bar2,LV_ALIGN_BOTTOM_LEFT,30,0);
    // lv_bar_set_range(bar2, 0, 255);
    // lv_bar_set_value(bar2,128,LV_ANIM_OFF);

    // lv_obj_t * bar3 = lv_bar_create(lv_scr_act());
    // lv_obj_set_size(bar3, 10, 200);
    // lv_obj_align(bar3,LV_ALIGN_BOTTOM_LEFT,45,0);
    // lv_bar_set_range(bar3, 0, 255);
    // lv_bar_set_value(bar3,128,LV_ANIM_OFF);

    // lv_obj_t * bar4 = lv_bar_create(lv_scr_act());
    // lv_obj_set_size(bar4, 10, 200);
    // lv_obj_align(bar4,LV_ALIGN_BOTTOM_LEFT,60,0);
    // lv_bar_set_range(bar4, 0, 255);
    // lv_bar_set_value(bar4,128,LV_ANIM_OFF);

    // lv_obj_t * bar5 = lv_bar_create(lv_scr_act());
    // lv_obj_set_size(bar5, 10, 200);
    // lv_obj_align(bar5,LV_ALIGN_BOTTOM_LEFT,75,0);
    // lv_bar_set_range(bar5, 0, 255);
    // lv_bar_set_value(bar5,128,LV_ANIM_OFF);



    lv_obj_t * btnm1 = lv_btnmatrix_create(lv_scr_act());
    lv_btnmatrix_set_map(btnm1, btnm_map);
    lv_btnmatrix_set_btn_width(btnm1, 15, 2);        /*Make "Action1" twice as wide as "Action2"*/
    lv_btnmatrix_set_btn_ctrl(btnm1, 15, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_btn_ctrl(btnm1, 16, LV_BTNMATRIX_CTRL_CHECKED);
    lv_obj_align(btnm1, LV_ALIGN_CENTER, 0, 0);













    static lv_style_t style_indic;
    

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    lv_obj_t * bar = lv_bar_create(lv_scr_act());
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 5, 200);
    lv_obj_set_align(bar,LV_ALIGN_BOTTOM_LEFT);






    //lv_obj_center(bar);
    lv_bar_set_range(bar, 0, 255);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_temp);
    lv_anim_set_time(&a, 500);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_set_var(&a, bar);
    lv_anim_set_values(&a, 0, 255);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}
