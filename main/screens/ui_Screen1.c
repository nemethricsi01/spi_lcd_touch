// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.1
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_flex_flow(ui_Screen1, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Screen1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Screen1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Screen1, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Screen1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Screen1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel1 = lv_obj_create(ui_Screen1);
    lv_obj_set_height(ui_Panel1, 240);
    lv_obj_set_flex_grow(ui_Panel1, 1);
    lv_obj_set_x(ui_Panel1, 0);
    lv_obj_set_y(ui_Panel1, 23);
    lv_obj_set_align(ui_Panel1, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel1, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel1, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_clear_flag(ui_Panel1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel1, lv_color_hex(0xAAAAAA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel1, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImgButton3 = lv_imgbtn_create(ui_Panel1);
    lv_imgbtn_set_src(ui_ImgButton3, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_6875758_png, NULL);
    lv_imgbtn_set_src(ui_ImgButton3, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_6875758_png, NULL);
    lv_obj_set_height(ui_ImgButton3, 25);
    lv_obj_set_width(ui_ImgButton3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_ImgButton3, 0);
    lv_obj_set_y(ui_ImgButton3, 9);
    lv_obj_set_align(ui_ImgButton3, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_ImgButton3, LV_OBJ_FLAG_FLOATING);     /// Flags

    ui_Bar1 = lv_bar_create(ui_Panel1);
    lv_bar_set_range(ui_Bar1, 0, 170);
    lv_bar_set_value(ui_Bar1, 20, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar1, 5);
    lv_obj_set_height(ui_Bar1, 170);
    lv_obj_set_x(ui_Bar1, -19);
    lv_obj_set_y(ui_Bar1, -20);
    lv_obj_set_align(ui_Bar1, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_style_bg_color(ui_Bar1, lv_color_hex(0xDDDDDD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Bar1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_Bar1, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Bar1, lv_color_hex(0xFF0000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Bar1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_Bar1, lv_color_hex(0x007C00), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_Bar1, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_Bar1, 100, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_Bar1, LV_GRAD_DIR_VER, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    ui_Bar2 = lv_bar_create(ui_Panel1);
    lv_bar_set_range(ui_Bar2, 0, 170);
    lv_bar_set_value(ui_Bar2, 25, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar2, 5);
    lv_obj_set_height(ui_Bar2, 170);
    lv_obj_set_align(ui_Bar2, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar3 = lv_bar_create(ui_Panel1);
    lv_bar_set_range(ui_Bar3, 0, 170);
    lv_bar_set_value(ui_Bar3, 25, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar3, 5);
    lv_obj_set_height(ui_Bar3, 170);
    lv_obj_set_align(ui_Bar3, LV_ALIGN_BOTTOM_LEFT);

    ui_Label8 = lv_label_create(ui_Panel1);
    lv_obj_set_width(ui_Label8, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label8, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label8, 1);
    lv_obj_set_y(ui_Label8, -21);
    lv_obj_set_align(ui_Label8, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label8, "1");
    lv_obj_add_flag(ui_Label8, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_font(ui_Label8, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel2 = lv_obj_create(ui_Screen1);
    lv_obj_set_height(ui_Panel2, 240);
    lv_obj_set_flex_grow(ui_Panel2, 1);
    lv_obj_set_align(ui_Panel2, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel2, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel2, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel2, lv_color_hex(0xAAAAAA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel2, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel2, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel2, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel2, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImgButton2 = lv_imgbtn_create(ui_Panel2);
    lv_imgbtn_set_src(ui_ImgButton2, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_6875758_png, NULL);
    lv_imgbtn_set_src(ui_ImgButton2, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_6875758_png, NULL);
    lv_obj_set_height(ui_ImgButton2, 25);
    lv_obj_set_width(ui_ImgButton2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_ImgButton2, 0);
    lv_obj_set_y(ui_ImgButton2, 9);
    lv_obj_set_align(ui_ImgButton2, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_ImgButton2, LV_OBJ_FLAG_FLOATING);     /// Flags

    ui_Bar4 = lv_bar_create(ui_Panel2);
    lv_bar_set_range(ui_Bar4, 0, 170);
    lv_bar_set_value(ui_Bar4, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar4, 5);
    lv_obj_set_height(ui_Bar4, 170);
    lv_obj_set_x(ui_Bar4, -19);
    lv_obj_set_y(ui_Bar4, -20);
    lv_obj_set_align(ui_Bar4, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar5 = lv_bar_create(ui_Panel2);
    lv_bar_set_range(ui_Bar5, 0, 170);
    lv_bar_set_value(ui_Bar5, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar5, 5);
    lv_obj_set_height(ui_Bar5, 170);
    lv_obj_set_x(ui_Bar5, -19);
    lv_obj_set_y(ui_Bar5, -20);
    lv_obj_set_align(ui_Bar5, LV_ALIGN_LEFT_MID);

    ui_Bar6 = lv_bar_create(ui_Panel2);
    lv_bar_set_range(ui_Bar6, 0, 170);
    lv_bar_set_value(ui_Bar6, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar6, 5);
    lv_obj_set_height(ui_Bar6, 170);
    lv_obj_set_x(ui_Bar6, -19);
    lv_obj_set_y(ui_Bar6, -20);
    lv_obj_set_align(ui_Bar6, LV_ALIGN_BOTTOM_LEFT);

    ui_Label1 = lv_label_create(ui_Panel2);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label1, 0);
    lv_obj_set_y(ui_Label1, -20);
    lv_obj_set_align(ui_Label1, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label1, "2");
    lv_obj_add_flag(ui_Label1, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_font(ui_Label1, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel4 = lv_obj_create(ui_Screen1);
    lv_obj_set_height(ui_Panel4, 240);
    lv_obj_set_flex_grow(ui_Panel4, 1);
    lv_obj_set_align(ui_Panel4, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel4, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel4, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_clear_flag(ui_Panel4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel4, lv_color_hex(0xAAAAAA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel4, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel4, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel4, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel4, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImgButton4 = lv_imgbtn_create(ui_Panel4);
    lv_imgbtn_set_src(ui_ImgButton4, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_6875758_png, NULL);
    lv_imgbtn_set_src(ui_ImgButton4, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_6875758_png, NULL);
    lv_obj_set_height(ui_ImgButton4, 25);
    lv_obj_set_width(ui_ImgButton4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_ImgButton4, 0);
    lv_obj_set_y(ui_ImgButton4, 9);
    lv_obj_set_align(ui_ImgButton4, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_ImgButton4, LV_OBJ_FLAG_FLOATING);     /// Flags

    ui_Bar7 = lv_bar_create(ui_Panel4);
    lv_bar_set_range(ui_Bar7, 0, 170);
    lv_bar_set_value(ui_Bar7, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar7, 5);
    lv_obj_set_height(ui_Bar7, 170);
    lv_obj_set_x(ui_Bar7, -19);
    lv_obj_set_y(ui_Bar7, -20);
    lv_obj_set_align(ui_Bar7, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar13 = lv_bar_create(ui_Panel4);
    lv_bar_set_range(ui_Bar13, 0, 170);
    lv_bar_set_value(ui_Bar13, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar13, 5);
    lv_obj_set_height(ui_Bar13, 170);
    lv_obj_set_x(ui_Bar13, -19);
    lv_obj_set_y(ui_Bar13, -20);
    lv_obj_set_align(ui_Bar13, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar14 = lv_bar_create(ui_Panel4);
    lv_bar_set_range(ui_Bar14, 0, 170);
    lv_bar_set_value(ui_Bar14, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar14, 5);
    lv_obj_set_height(ui_Bar14, 170);
    lv_obj_set_x(ui_Bar14, -19);
    lv_obj_set_y(ui_Bar14, -20);
    lv_obj_set_align(ui_Bar14, LV_ALIGN_BOTTOM_LEFT);

    ui_Label2 = lv_label_create(ui_Panel4);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label2, 0);
    lv_obj_set_y(ui_Label2, -20);
    lv_obj_set_align(ui_Label2, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label2, "3\n");
    lv_obj_add_flag(ui_Label2, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_font(ui_Label2, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel5 = lv_obj_create(ui_Screen1);
    lv_obj_set_height(ui_Panel5, 240);
    lv_obj_set_flex_grow(ui_Panel5, 1);
    lv_obj_set_align(ui_Panel5, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel5, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel5, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_clear_flag(ui_Panel5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel5, lv_color_hex(0xAAAAAA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel5, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel5, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel5, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel5, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImgButton5 = lv_imgbtn_create(ui_Panel5);
    lv_imgbtn_set_src(ui_ImgButton5, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_6875758_png, NULL);
    lv_imgbtn_set_src(ui_ImgButton5, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_6875758_png, NULL);
    lv_obj_set_height(ui_ImgButton5, 25);
    lv_obj_set_width(ui_ImgButton5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_ImgButton5, 0);
    lv_obj_set_y(ui_ImgButton5, 9);
    lv_obj_set_align(ui_ImgButton5, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_ImgButton5, LV_OBJ_FLAG_FLOATING);     /// Flags

    ui_Bar9 = lv_bar_create(ui_Panel5);
    lv_bar_set_range(ui_Bar9, 0, 170);
    lv_bar_set_value(ui_Bar9, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar9, 5);
    lv_obj_set_height(ui_Bar9, 170);
    lv_obj_set_x(ui_Bar9, -19);
    lv_obj_set_y(ui_Bar9, -20);
    lv_obj_set_align(ui_Bar9, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar15 = lv_bar_create(ui_Panel5);
    lv_bar_set_range(ui_Bar15, 0, 170);
    lv_bar_set_value(ui_Bar15, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar15, 5);
    lv_obj_set_height(ui_Bar15, 170);
    lv_obj_set_x(ui_Bar15, -19);
    lv_obj_set_y(ui_Bar15, -20);
    lv_obj_set_align(ui_Bar15, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar16 = lv_bar_create(ui_Panel5);
    lv_bar_set_range(ui_Bar16, 0, 170);
    lv_bar_set_value(ui_Bar16, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar16, 5);
    lv_obj_set_height(ui_Bar16, 170);
    lv_obj_set_x(ui_Bar16, -19);
    lv_obj_set_y(ui_Bar16, -20);
    lv_obj_set_align(ui_Bar16, LV_ALIGN_BOTTOM_LEFT);

    ui_Label7 = lv_label_create(ui_Panel5);
    lv_obj_set_width(ui_Label7, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label7, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label7, 0);
    lv_obj_set_y(ui_Label7, -20);
    lv_obj_set_align(ui_Label7, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label7, "4");
    lv_obj_add_flag(ui_Label7, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_font(ui_Label7, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel6 = lv_obj_create(ui_Screen1);
    lv_obj_set_height(ui_Panel6, 240);
    lv_obj_set_flex_grow(ui_Panel6, 1);
    lv_obj_set_align(ui_Panel6, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel6, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel6, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_clear_flag(ui_Panel6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel6, lv_color_hex(0xAAAAAA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel6, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel6, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel6, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel6, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImgButton6 = lv_imgbtn_create(ui_Panel6);
    lv_imgbtn_set_src(ui_ImgButton6, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_6875758_png, NULL);
    lv_imgbtn_set_src(ui_ImgButton6, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_6875758_png, NULL);
    lv_obj_set_height(ui_ImgButton6, 25);
    lv_obj_set_width(ui_ImgButton6, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_ImgButton6, 0);
    lv_obj_set_y(ui_ImgButton6, 9);
    lv_obj_set_align(ui_ImgButton6, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_ImgButton6, LV_OBJ_FLAG_FLOATING);     /// Flags

    ui_Bar8 = lv_bar_create(ui_Panel6);
    lv_bar_set_range(ui_Bar8, 0, 170);
    lv_bar_set_value(ui_Bar8, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar8, 5);
    lv_obj_set_height(ui_Bar8, 170);
    lv_obj_set_x(ui_Bar8, -19);
    lv_obj_set_y(ui_Bar8, -20);
    lv_obj_set_align(ui_Bar8, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar17 = lv_bar_create(ui_Panel6);
    lv_bar_set_range(ui_Bar17, 0, 170);
    lv_bar_set_value(ui_Bar17, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar17, 5);
    lv_obj_set_height(ui_Bar17, 170);
    lv_obj_set_x(ui_Bar17, -19);
    lv_obj_set_y(ui_Bar17, -20);
    lv_obj_set_align(ui_Bar17, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar18 = lv_bar_create(ui_Panel6);
    lv_bar_set_range(ui_Bar18, 0, 170);
    lv_bar_set_value(ui_Bar18, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar18, 5);
    lv_obj_set_height(ui_Bar18, 170);
    lv_obj_set_x(ui_Bar18, -19);
    lv_obj_set_y(ui_Bar18, -20);
    lv_obj_set_align(ui_Bar18, LV_ALIGN_BOTTOM_LEFT);

    ui_Label6 = lv_label_create(ui_Panel6);
    lv_obj_set_width(ui_Label6, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label6, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label6, 0);
    lv_obj_set_y(ui_Label6, -20);
    lv_obj_set_align(ui_Label6, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label6, "5");
    lv_obj_add_flag(ui_Label6, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_font(ui_Label6, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel8 = lv_obj_create(ui_Screen1);
    lv_obj_set_height(ui_Panel8, 240);
    lv_obj_set_flex_grow(ui_Panel8, 1);
    lv_obj_set_align(ui_Panel8, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel8, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel8, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_clear_flag(ui_Panel8, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel8, lv_color_hex(0xAAAAAA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel8, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel8, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel8, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel8, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel8, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImgButton7 = lv_imgbtn_create(ui_Panel8);
    lv_imgbtn_set_src(ui_ImgButton7, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_6875758_png, NULL);
    lv_imgbtn_set_src(ui_ImgButton7, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_6875758_png, NULL);
    lv_obj_set_height(ui_ImgButton7, 25);
    lv_obj_set_width(ui_ImgButton7, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_ImgButton7, 0);
    lv_obj_set_y(ui_ImgButton7, 9);
    lv_obj_set_align(ui_ImgButton7, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_ImgButton7, LV_OBJ_FLAG_FLOATING);     /// Flags

    ui_Bar10 = lv_bar_create(ui_Panel8);
    lv_bar_set_range(ui_Bar10, 0, 170);
    lv_bar_set_value(ui_Bar10, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar10, 5);
    lv_obj_set_height(ui_Bar10, 170);
    lv_obj_set_x(ui_Bar10, -19);
    lv_obj_set_y(ui_Bar10, -20);
    lv_obj_set_align(ui_Bar10, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar19 = lv_bar_create(ui_Panel8);
    lv_bar_set_range(ui_Bar19, 0, 170);
    lv_bar_set_value(ui_Bar19, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar19, 5);
    lv_obj_set_height(ui_Bar19, 170);
    lv_obj_set_x(ui_Bar19, -19);
    lv_obj_set_y(ui_Bar19, -20);
    lv_obj_set_align(ui_Bar19, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar20 = lv_bar_create(ui_Panel8);
    lv_bar_set_range(ui_Bar20, 0, 170);
    lv_bar_set_value(ui_Bar20, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar20, 5);
    lv_obj_set_height(ui_Bar20, 170);
    lv_obj_set_x(ui_Bar20, -19);
    lv_obj_set_y(ui_Bar20, -20);
    lv_obj_set_align(ui_Bar20, LV_ALIGN_BOTTOM_LEFT);

    ui_Label5 = lv_label_create(ui_Panel8);
    lv_obj_set_width(ui_Label5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label5, 0);
    lv_obj_set_y(ui_Label5, -20);
    lv_obj_set_align(ui_Label5, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label5, "6");
    lv_obj_add_flag(ui_Label5, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_font(ui_Label5, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel3 = lv_obj_create(ui_Screen1);
    lv_obj_set_height(ui_Panel3, 240);
    lv_obj_set_flex_grow(ui_Panel3, 1);
    lv_obj_set_align(ui_Panel3, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel3, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel3, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel3, lv_color_hex(0xAAAAAA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel3, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel3, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel3, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel3, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImgButton8 = lv_imgbtn_create(ui_Panel3);
    lv_imgbtn_set_src(ui_ImgButton8, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_6875758_png, NULL);
    lv_imgbtn_set_src(ui_ImgButton8, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_6875758_png, NULL);
    lv_obj_set_height(ui_ImgButton8, 25);
    lv_obj_set_width(ui_ImgButton8, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_ImgButton8, 0);
    lv_obj_set_y(ui_ImgButton8, 9);
    lv_obj_set_align(ui_ImgButton8, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_ImgButton8, LV_OBJ_FLAG_FLOATING);     /// Flags

    ui_Bar11 = lv_bar_create(ui_Panel3);
    lv_bar_set_range(ui_Bar11, 0, 170);
    lv_bar_set_value(ui_Bar11, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar11, 5);
    lv_obj_set_height(ui_Bar11, 170);
    lv_obj_set_x(ui_Bar11, -19);
    lv_obj_set_y(ui_Bar11, -20);
    lv_obj_set_align(ui_Bar11, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar21 = lv_bar_create(ui_Panel3);
    lv_bar_set_range(ui_Bar21, 0, 170);
    lv_bar_set_value(ui_Bar21, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar21, 5);
    lv_obj_set_height(ui_Bar21, 170);
    lv_obj_set_x(ui_Bar21, -19);
    lv_obj_set_y(ui_Bar21, -20);
    lv_obj_set_align(ui_Bar21, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar22 = lv_bar_create(ui_Panel3);
    lv_bar_set_range(ui_Bar22, 0, 170);
    lv_bar_set_value(ui_Bar22, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar22, 5);
    lv_obj_set_height(ui_Bar22, 170);
    lv_obj_set_x(ui_Bar22, -19);
    lv_obj_set_y(ui_Bar22, -20);
    lv_obj_set_align(ui_Bar22, LV_ALIGN_BOTTOM_LEFT);

    ui_Label4 = lv_label_create(ui_Panel3);
    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label4, 0);
    lv_obj_set_y(ui_Label4, -20);
    lv_obj_set_align(ui_Label4, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label4, "7");
    lv_obj_add_flag(ui_Label4, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_font(ui_Label4, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel7 = lv_obj_create(ui_Screen1);
    lv_obj_set_height(ui_Panel7, 240);
    lv_obj_set_flex_grow(ui_Panel7, 1);
    lv_obj_set_align(ui_Panel7, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel7, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel7, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_clear_flag(ui_Panel7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel7, lv_color_hex(0xAAAAAA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel7, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel7, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel7, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel7, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImgButton9 = lv_imgbtn_create(ui_Panel7);
    lv_imgbtn_set_src(ui_ImgButton9, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_6875758_png, NULL);
    lv_imgbtn_set_src(ui_ImgButton9, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_6875758_png, NULL);
    lv_obj_set_height(ui_ImgButton9, 25);
    lv_obj_set_width(ui_ImgButton9, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_ImgButton9, 0);
    lv_obj_set_y(ui_ImgButton9, 9);
    lv_obj_set_align(ui_ImgButton9, LV_ALIGN_TOP_MID);
    lv_obj_add_flag(ui_ImgButton9, LV_OBJ_FLAG_FLOATING);     /// Flags

    ui_Bar12 = lv_bar_create(ui_Panel7);
    lv_bar_set_range(ui_Bar12, 0, 170);
    lv_bar_set_value(ui_Bar12, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar12, 5);
    lv_obj_set_height(ui_Bar12, 170);
    lv_obj_set_x(ui_Bar12, -19);
    lv_obj_set_y(ui_Bar12, -20);
    lv_obj_set_align(ui_Bar12, LV_ALIGN_BOTTOM_MID);

    ui_Bar23 = lv_bar_create(ui_Panel7);
    lv_bar_set_range(ui_Bar23, 0, 170);
    lv_bar_set_value(ui_Bar23, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar23, 5);
    lv_obj_set_height(ui_Bar23, 170);
    lv_obj_set_x(ui_Bar23, -19);
    lv_obj_set_y(ui_Bar23, -20);
    lv_obj_set_align(ui_Bar23, LV_ALIGN_BOTTOM_LEFT);

    ui_Bar24 = lv_bar_create(ui_Panel7);
    lv_bar_set_range(ui_Bar24, 0, 170);
    lv_bar_set_value(ui_Bar24, 50, LV_ANIM_OFF);
    lv_obj_set_width(ui_Bar24, 5);
    lv_obj_set_height(ui_Bar24, 170);
    lv_obj_set_x(ui_Bar24, -19);
    lv_obj_set_y(ui_Bar24, -20);
    lv_obj_set_align(ui_Bar24, LV_ALIGN_BOTTOM_LEFT);

    ui_Label3 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label3, 0);
    lv_obj_set_y(ui_Label3, -20);
    lv_obj_set_align(ui_Label3, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label3, "8");
    lv_obj_add_flag(ui_Label3, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_font(ui_Label3, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ImgButton3, ui_event_ImgButton3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Panel1, ui_event_Panel1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ImgButton2, ui_event_ImgButton2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ImgButton4, ui_event_ImgButton4, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ImgButton5, ui_event_ImgButton5, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ImgButton6, ui_event_ImgButton6, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ImgButton7, ui_event_ImgButton7, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ImgButton8, ui_event_ImgButton8, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ImgButton9, ui_event_ImgButton9, LV_EVENT_ALL, NULL);

}
