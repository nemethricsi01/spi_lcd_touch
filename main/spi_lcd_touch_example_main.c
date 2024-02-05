/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "driver/i2c.h"
#include "ui.h"
#include <math.h>
#include "potik/poti.h"
#define CONFIG_ESP_LCD_TOUCH_MAX_BUTTONS 1



#include "esp_lcd_ili9341.h"
#include "esp_lcd_touch_tt21100.h"

static const char *TAG = "example";

// Using SPI2 in the example
#define LCD_HOST  SPI2_HOST

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     (60 * 1000 * 1000)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_SCLK           18
#define EXAMPLE_PIN_NUM_MOSI           47
#define EXAMPLE_PIN_NUM_MISO           21
#define EXAMPLE_PIN_NUM_LCD_DC         5
#define EXAMPLE_PIN_NUM_LCD_RST        3
#define EXAMPLE_PIN_NUM_LCD_CS         4
#define EXAMPLE_PIN_NUM_BK_LIGHT       2

// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES              320
#define EXAMPLE_LCD_V_RES              240

// Bit number used to represent command and parameter
#define EXAMPLE_LCD_CMD_BITS           8
#define EXAMPLE_LCD_PARAM_BITS         8

#define EXAMPLE_LVGL_TICK_PERIOD_MS    2

esp_lcd_touch_handle_t tp = NULL;
esp_lcd_touch_handle_t button = NULL;




SemaphoreHandle_t lvgl_sem;
SemaphoreHandle_t i2c_sem;


typedef enum {
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_5,
    CHANNEL_6,
    CHANNEL_7,
    CHANNEL_8,
    CHANNEL_NONE
} LastChannelPress;

typedef struct {
    LastChannelPress lastPress;
} ChannelState;

ChannelState channelState;

CHANNEL_TYPE channelArray[NUM_CHANNEL];


extern void lv_example_bar_3();
void Ch1Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_1;
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch2Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_2;
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch3Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_3;
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch4Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_4;
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch5Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_5;
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch6Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_6;
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch7Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_7;
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch8Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_8;
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}





void backButtonPressed(lv_event_t *e)
{
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_NONE;
    _ui_screen_change(&ui_Screen1,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen1_screen_init);
}


void applyToAllChanPressed(lv_event_t *e)
{

}
void refValueChanged(lv_event_t * e)
{
	lv_obj_t *slider = lv_event_get_target(e);
    uint8_t val = (uint8_t)lv_slider_get_value(slider);
    Channel_setRefVal(&channelArray[CH_1],val);
    ESP_LOGI("lvgl:","ref slider val: %d",val);
}

void bypassValueChanged(lv_event_t * e)
{
	lv_obj_t *slider = lv_event_get_target(e);
    uint8_t val = (uint8_t)lv_slider_get_value(slider);
    Channel_setByPassVal(&channelArray[CH_1],val);
    ESP_LOGI("lvgl:","bypass slider val: %d",val);
}

void outValueChanged(lv_event_t * e)
{
	lv_obj_t *slider = lv_event_get_target(e);
    uint8_t val = (uint8_t)lv_slider_get_value(slider);
    Channel_setKiVal(&channelArray[CH_1],val);
    ESP_LOGI("lvgl:","out slider val: %d",val);
}
void micValueChanged(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t val = (uint8_t)lv_slider_get_value(slider);
    Channel_setMicVal(&channelArray[CH_1],val);
    ESP_LOGI("lvgl:","mic slider val: %d",val);
}

static bool example_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
    lv_disp_flush_ready(disp_driver);
    return false;
}

static void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    // copy a buffer's content to a specific area of the display
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
}

/* Rotate display and touch, when rotated screen in LVGL. Called when driver parameters are updated. */
static void example_lvgl_port_update_callback(lv_disp_drv_t *drv)
{
    
}


static void example_lvgl_touch_cb(lv_indev_drv_t * drv, lv_indev_data_t * data)
{
    uint16_t touchpad_x[1] = {0};
    uint16_t touchpad_y[1] = {0};
    uint8_t touchpad_cnt = 0;
    uint8_t points = 0;
    uint8_t butt;

    /* Read touch controller data */
    ESP_ERROR_CHECK(esp_lcd_touch_read_data(drv->user_data));
    
    

    /* Get coordinates */
    bool touchpad_pressed = esp_lcd_touch_get_coordinates(drv->user_data, touchpad_x, touchpad_y, NULL, &touchpad_cnt, 1);
    esp_lcd_touch_get_button_state(drv->user_data,points,&butt);
    //ESP_LOGI("ricsi","pressed,x:%d,y:%d points: %d",touchpad_x[0],touchpad_y[0],butt);

    if (touchpad_pressed && touchpad_cnt > 0) {
        data->point.x = touchpad_x[0];
        data->point.y = touchpad_y[0];
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = 1;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 20,
        .scl_io_num = 21,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 200*1000,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}
uint8_t mapLinearToLogarithmic(uint16_t input) {
    if (input == 0) {
        return 0;  // Avoid log(0), output 0
    }
    
    double log_output = log(input + 1) * (170.0 / log(32769));
    if(log_output < 85)
    {
        log_output = 0;
    }
    if( log_output >85)
    {
        log_output = (log_output - 85)*2;
    }
    return (uint8_t)log_output;
}
void i2c_read_task(void *pvParameter) {
    uint8_t data[32];
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");
    i2c_cmd_handle_t i2chan;
    uint16_t micData = 0;
    memset(data,0,32);
    while (1) 
    {
         if (pdTRUE == xSemaphoreTake(i2c_sem, portMAX_DELAY)) 
            {
            i2c_master_read_from_device(1,0x55,data,16,100 / portTICK_PERIOD_MS);
            xSemaphoreGive(i2c_sem);
            }
        micData = data[0]<<8;
        micData |= data[1];
    //ESP_LOG_BUFFER_HEX("buff: ", data,32);
    
    float scaledValue = ( (float)micData / 32768.0f ) * 170.0f;
    int mappedValue = (int)scaledValue; // This integer value is in the range [0, 170]
    xSemaphoreTake(lvgl_sem,100);
    //lv_bar_set_value(ui_Bar1,mappedValue,LV_ANIM_ON);
    ESP_LOGI("i2c_receive:","mic raw: %d mic log: %d mic lin: %d",micData,mapLinearToLogarithmic(micData),mappedValue);
    //lv_bar_set_value(ui_Bar2,mapLinearToLogarithmic(micData),LV_ANIM_ON);
    xSemaphoreGive(lvgl_sem);
    vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void i2c_write_task(void *pvParameter) {
    i2c_cmd_handle_t i2chan;
    while (1) 
    {
         if (pdTRUE == xSemaphoreTake(i2c_sem, portMAX_DELAY)) 
            {
                if(channelArray[CH_1].changed == true)
                {
                    uint8_t data[10];
                    memset(data,0,10);
                    data[0] = Channel_getMicVal(channelArray[CH_1]);
                    data[1] = Channel_getRefVal(channelArray[CH_1]);
                    data[2] = Channel_getKiVal(channelArray[CH_1]);
                    data[3] = Channel_getByPassVal(channelArray[CH_1]);

                    i2c_master_write_to_device(1,0x55,data,10,100 / portTICK_PERIOD_MS);
                    channelArray[CH_1].changed = false;
                }
                xSemaphoreGive(i2c_sem);
            }
    vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void app_main(void)
{

    for(int i = 0;i<NUM_CHANNEL;i++)
    {
        channelArray[i].channel = i;
        channelArray[i].potik.byPassVal = 0;
        channelArray[i].potik.kiVal = 0;
        channelArray[i].potik.micVal = 0;
        channelArray[i].potik.refVal = 0;
        channelArray[i].changed = false;
    }
    static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
    static lv_disp_drv_t disp_drv;      // contains callback functions

    ESP_LOGI(TAG, "Turn off LCD backlight");
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << EXAMPLE_PIN_NUM_BK_LIGHT
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    ESP_LOGI(TAG, "Initialize SPI bus");
    spi_bus_config_t buscfg = {
        .sclk_io_num = EXAMPLE_PIN_NUM_SCLK,
        .mosi_io_num = EXAMPLE_PIN_NUM_MOSI,
        .miso_io_num = EXAMPLE_PIN_NUM_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = EXAMPLE_LCD_H_RES * 80 * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = EXAMPLE_PIN_NUM_LCD_DC,
        .cs_gpio_num = EXAMPLE_PIN_NUM_LCD_CS,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
        .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .on_color_trans_done = example_notify_lvgl_flush_ready,
        .user_ctx = &disp_drv,
    };
    // Attach the LCD to the SPI bus
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_PIN_NUM_LCD_RST,
        .rgb_endian = LCD_RGB_ENDIAN_BGR,
        .bits_per_pixel = 16,
    };
    ESP_LOGI(TAG, "Install ILI9341 panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    // user can flush pre-defined pattern to the screen before we turn on the screen or backlight
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
        // Attach the TOUCH to the I2C bus
    const i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 14,
        .scl_io_num = 13,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };
    /* Initialize I2C */
    ESP_ERROR_CHECK(i2c_param_config(0, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(0, i2c_conf.mode, 0, 0, 0));
    
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_H_RES,
        .y_max = EXAMPLE_LCD_V_RES,
        .rst_gpio_num = -1,
        .int_gpio_num = -1,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 1,
            .mirror_y = 0,
        },
    };
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_TT21100_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)0, &tp_io_config, &tp_io_handle));
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_tt21100(tp_io_handle, &tp_cfg, &tp));
    

    

    ESP_LOGI(TAG, "Turn on LCD backlight");
    gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);

    ESP_LOGI(TAG, "Initialize LVGL library");
    lv_init();
    // alloc draw buffers used by LVGL
    // it's recommended to choose the size of the draw buffer(s) to be at least 1/10 screen sized
    lv_color_t *buf1 = heap_caps_malloc(EXAMPLE_LCD_H_RES * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1);
    lv_color_t *buf2 = heap_caps_malloc(EXAMPLE_LCD_H_RES * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2);
    // initialize LVGL draw buffers
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, EXAMPLE_LCD_H_RES * 20);

    ESP_LOGI(TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = EXAMPLE_LCD_H_RES;
    disp_drv.ver_res = EXAMPLE_LCD_V_RES;
    disp_drv.flush_cb = example_lvgl_flush_cb;
    disp_drv.drv_update_cb = example_lvgl_port_update_callback;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    ESP_LOGI(TAG, "Install LVGL tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &example_increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));


    static lv_indev_drv_t indev_drv;    // Input device driver (Touch)
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.disp = disp;
    indev_drv.read_cb = example_lvgl_touch_cb;
    indev_drv.user_data = tp;

    lv_indev_drv_register(&indev_drv);

    ESP_LOGI(TAG, "Display LVGL Meter Widget");
    esp_lcd_panel_swap_xy(panel_handle, false);
    
    esp_lcd_panel_mirror(panel_handle, true, true);
    //lv_example_bar_3();
    lvgl_sem =  xSemaphoreCreateBinary();
    i2c_sem = xSemaphoreCreateBinary();
    xSemaphoreGive(lvgl_sem);
    xSemaphoreGive(i2c_sem);
    //xTaskCreate(&i2c_read_task,"i2c read slave",4096,NULL,5,NULL);
    xTaskCreate(&i2c_write_task,"i2c write slave",4096,NULL,5,NULL);
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");
    ui_init();
    while (1) {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(10));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        xSemaphoreTake(lvgl_sem,100);
        lv_timer_handler();
        xSemaphoreGive(lvgl_sem);
    }
}
