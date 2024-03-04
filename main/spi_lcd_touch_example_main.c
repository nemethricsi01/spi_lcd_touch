/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "driver/i2c.h"
#include "ui.h"
#include <math.h>
#include "potik/poti.h"
#include "lvgl_stuff/lvgl_init.h"
#include "driver/twai.h"
#include "can_task/can_task.h"


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
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = 1;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 48,
        .scl_io_num = 21,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 200*1000,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

void reset_stm32(void)
{

}




static const char *TAG = "example";



int8_t digitPotiVals[41] = 
{
    6,5,4,3,2,1,
    0,-1,-2,-3,-4,-5,
    -6,-7,-8,-9,-10,-11,
    -12,-13,-14,-15,-16,-17,
    -18,-19,-20,-21,-22,-23,
    -24,-25,-26,-27,-29,-31,
    -33,-35,-37,-39,-42
};


SemaphoreHandle_t lvgl_sem;
SemaphoreHandle_t i2c_sem;
SemaphoreHandle_t buttonSemaphore;


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
typedef struct {
    uint8_t MicPotiVal[8];
    uint8_t RefPotiVal[8];
    uint8_t BypassPotiVal[8];
    uint8_t KiPotiVal[8];
    uint16_t micData[8];
    uint16_t refData[8];
    uint16_t kiData[8];
} Stm2EspData;

Stm2EspData stm2EspData;
ChannelState channelState;

CHANNEL_TYPE channelArray[NUM_CHANNEL];


extern void lv_example_bar_3();
extern lv_obj_t * ui_Slider1;
extern lv_obj_t * ui_Slider2;
extern lv_obj_t * ui_Slider3;
extern lv_obj_t * ui_Slider4;
//main screen bars
extern lv_obj_t *ui_Bar1;
extern lv_obj_t *ui_Bar2;
extern lv_obj_t *ui_Bar3;
extern lv_obj_t *ui_Bar4;
extern lv_obj_t *ui_Bar5;
extern lv_obj_t *ui_Bar6;
extern lv_obj_t *ui_Bar7;
extern lv_obj_t *ui_Bar13;
extern lv_obj_t *ui_Bar14;
extern lv_obj_t *ui_Bar9;
extern lv_obj_t *ui_Bar15;
extern lv_obj_t *ui_Bar16;
extern lv_obj_t *ui_Bar8;
extern lv_obj_t *ui_Bar17;
extern lv_obj_t *ui_Bar18;
extern lv_obj_t *ui_Bar10;
extern lv_obj_t *ui_Bar19;
extern lv_obj_t *ui_Bar20;
extern lv_obj_t *ui_Bar11;
extern lv_obj_t *ui_Bar21;
extern lv_obj_t *ui_Bar22;
extern lv_obj_t *ui_Bar12;
extern lv_obj_t *ui_Bar23;
extern lv_obj_t *ui_Bar24;
//main screen bars end


//adjust screen bars
extern lv_obj_t *ui_Bar25;
extern lv_obj_t *ui_Bar26;
extern lv_obj_t *ui_Bar28;
//adjust screen bars end








lv_obj_t *MainScreenBars[24];  // Declare the array globally
void init_Bar_Array(void) 
{
    // Initialize the array
    MainScreenBars[0] = ui_Bar1;
    MainScreenBars[1] = ui_Bar2;
    MainScreenBars[2] = ui_Bar3;
    MainScreenBars[3] = ui_Bar4;
    MainScreenBars[4] = ui_Bar5;
    MainScreenBars[5] = ui_Bar6;
    MainScreenBars[6] = ui_Bar7;
    MainScreenBars[7] = ui_Bar8;
    MainScreenBars[8] = ui_Bar9;
    MainScreenBars[9] = ui_Bar10;
    MainScreenBars[10] = ui_Bar11;
    MainScreenBars[11] = ui_Bar12;
    MainScreenBars[12] = ui_Bar13;
    MainScreenBars[13] = ui_Bar14;
    MainScreenBars[14] = ui_Bar15;
    MainScreenBars[15] = ui_Bar16;
    MainScreenBars[16] = ui_Bar17;
    MainScreenBars[17] = ui_Bar18;
    MainScreenBars[18] = ui_Bar19;
    MainScreenBars[19] = ui_Bar20;
    MainScreenBars[20] = ui_Bar21;
    MainScreenBars[21] = ui_Bar22;
    MainScreenBars[22] = ui_Bar23;
    MainScreenBars[23] = ui_Bar24;
}

void changeToLogger(lv_event_t * e)
{
	_ui_screen_change(&ui_Screen5,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen5_screen_init);
}
void canAddChanged(lv_event_t * e)
{
	// Your code here
}
void backToSettings(lv_event_t * e)
{
	_ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void goToCanSettings(lv_event_t * e)
{
	_ui_screen_change(&ui_Screen4,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen4_screen_init);
}
void Ch1Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_1;
    lv_slider_set_value(ui_Slider2,Channel_getMicVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider1,Channel_getRefVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider3,Channel_getByPassVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider4,Channel_getKiVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch2Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_2;
    lv_slider_set_value(ui_Slider2,Channel_getMicVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider1,Channel_getRefVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider3,Channel_getByPassVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider4,Channel_getKiVal(channelArray[channelState.lastPress]),LV_ANIM_ON);

    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch3Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_3;
    lv_slider_set_value(ui_Slider2,Channel_getMicVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider1,Channel_getRefVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider3,Channel_getByPassVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider4,Channel_getKiVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch4Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_4;
    lv_slider_set_value(ui_Slider2,Channel_getMicVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider1,Channel_getRefVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider3,Channel_getByPassVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider4,Channel_getKiVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch5Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_5;
    lv_slider_set_value(ui_Slider2,Channel_getMicVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider1,Channel_getRefVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider3,Channel_getByPassVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider4,Channel_getKiVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch6Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_6;
    lv_slider_set_value(ui_Slider2,Channel_getMicVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider1,Channel_getRefVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider3,Channel_getByPassVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider4,Channel_getKiVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch7Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_7;
    lv_slider_set_value(ui_Slider2,Channel_getMicVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider1,Channel_getRefVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider3,Channel_getByPassVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider4,Channel_getKiVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    _ui_screen_change(&ui_Screen2,LV_SCR_LOAD_ANIM_NONE,0,0,&ui_Screen2_screen_init);
}
void Ch8Pressed(lv_event_t * e)
{
	// Your code here
    //lv_obj_clean(lv_scr_act());
    channelState.lastPress = CHANNEL_8;
    lv_slider_set_value(ui_Slider2,Channel_getMicVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider1,Channel_getRefVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider3,Channel_getByPassVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
    lv_slider_set_value(ui_Slider4,Channel_getKiVal(channelArray[channelState.lastPress]),LV_ANIM_ON);
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
    uint8_t mappedVal = (uint8_t)((float)val/101.0*41.0);//this maps the values to match the amplitude
    lv_label_set_text_fmt(ui_Label16,"%d\ndB",digitPotiVals[40-mappedVal]);
    Channel_setRefVal(&channelArray[channelState.lastPress],val);
    ESP_LOGI("lvgl:","ref slider val: %d",val);
}

void bypassValueChanged(lv_event_t * e)
{
	lv_obj_t *slider = lv_event_get_target(e);
    uint8_t val = (uint8_t)lv_slider_get_value(slider);
    uint8_t mappedVal = (uint8_t)((float)val/101.0*41.0);//this maps the values to match the amplitude
    lv_label_set_text_fmt(ui_Label18,"%d\ndB",digitPotiVals[40-mappedVal]);
    Channel_setByPassVal(&channelArray[channelState.lastPress],val);
    ESP_LOGI("lvgl:","bypass slider val: %d",val);
}

void outValueChanged(lv_event_t * e)
{
	lv_obj_t *slider = lv_event_get_target(e);
    uint8_t val = (uint8_t)lv_slider_get_value(slider);
    uint8_t mappedVal = (uint8_t)((float)val/101.0*41.0);//this maps the values to match the amplitude
    lv_label_set_text_fmt(ui_Label19,"%d\ndB",digitPotiVals[40-mappedVal]);
    Channel_setKiVal(&channelArray[channelState.lastPress],val);
    ESP_LOGI("lvgl:","out slider val: %d\ndB",val);
}
void micValueChanged(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    uint8_t val = (uint8_t)lv_slider_get_value(slider);
    uint8_t mappedVal = (uint8_t)((float)val/101.0*41.0);//this maps the values to match the amplitude
    lv_label_set_text_fmt(ui_Label14,"%d\ndB",digitPotiVals[40-mappedVal]);
    Channel_setMicVal(&channelArray[channelState.lastPress],val);
    ESP_LOGI("lvgl:","mic slider val: %d, mappedval: %d",val,mappedVal);
}







// static void twai_receive_task(void *arg)
// {
//     xSemaphoreTake(rx_sem, portMAX_DELAY);
//     bool start_cmd = false;
//     bool stop_resp = false;
//     uint32_t iterations = 0;
//     twai_message_t rx_msg;
//     while(1)
//     {
//         twai_receive(&rx_msg, portMAX_DELAY);
//     ESP_LOGI(EXAMPLE_TAG, "Received message");
//     ESP_LOG_BUFFER_HEX(EXAMPLE_TAG, &rx_msg, sizeof(rx_msg));
//     }
// }
void i2c_read_task(void *pvParameter) {
    uint8_t data[16];
    uint8_t data2[16];
    // ESP_ERROR_CHECK(i2c_master_init());
    // ESP_LOGI(TAG, "I2C initialized successfully");
    // i2c_cmd_handle_t i2chan;
    uint16_t micDataArr[8];
    memset(data,0,16);
    
    if (pdTRUE == xSemaphoreTake(i2c_sem, portMAX_DELAY)) 
            {
                for(uint8_t i = 0; i<8;i++)
                {
                    i2c_master_read_from_device(1,i+1,data,16,100 / portTICK_PERIOD_MS);//read values from slaves nvs
                    //save them to the channelArray
                    channelArray[i].potik.micVal = data[12];
                    channelArray[i].potik.refVal = data[13];
                    channelArray[i].potik.kiVal = data[14];
                    channelArray[i].potik.byPassVal = data[15];
                }
                // i2c_master_read_from_device(1,0x1,data,16,100 / portTICK_PERIOD_MS);//read values from slaves nvs
                // //save them to the channelArray
                // channelArray[CH_1].potik.micVal = 100-data[12];
                // channelArray[CH_1].potik.refVal = 100-data[13];
                // channelArray[CH_1].potik.kiVal = 100-data[14];
                // channelArray[CH_1].potik.byPassVal = 100-data[15];
                // i2c_master_read_from_device(1,0x2,data,16,100 / portTICK_PERIOD_MS);//read values from slaves nvs
                // //save them to the channelArray
                // channelArray[CH_2].potik.micVal = 100-data[12];
                // channelArray[CH_2].potik.refVal = 100-data[13];
                // channelArray[CH_2].potik.kiVal = 100-data[14];
                // channelArray[CH_2].potik.byPassVal = 100-data[15];
                xSemaphoreGive(i2c_sem);
                
            }
    while (1) 
    {
         if (pdTRUE == xSemaphoreTake(i2c_sem, portMAX_DELAY)) 
            {
                for(uint8_t i = 0;i<8;i++ )
                {
                i2c_master_read_from_device(1,i+1,data,16,100 / portTICK_PERIOD_MS);
                stm2EspData.MicPotiVal[i] = data[12];
                stm2EspData.RefPotiVal[i] = data[13];
                stm2EspData.KiPotiVal[i] = data[14];
                stm2EspData.BypassPotiVal[i] = data[15];
                stm2EspData.micData[i] = data[0]<<8;
                stm2EspData.micData[i] |= data[1];
                stm2EspData.refData[i] = data[2]<<8;
                stm2EspData.refData[i] |= data[3];
                stm2EspData.kiData[i] = data[4]<<8;
                stm2EspData.kiData[i] |= data[5];
                
                }
                // i2c_master_read_from_device(1,0x1,data,16,100 / portTICK_PERIOD_MS);
                // micData1 = data[0]<<8;
                // micData2 |= data[1];
                // //ESP_LOGI("i2c_rec:","ch1 potik: mic: %d, ref: %d, ki: %d, byp: %d ",100-data[12],100-data[13],100-data[14],100-data[15]);
                // i2c_master_read_from_device(1,0x2,data2,16,100 / portTICK_PERIOD_MS);
                // micData2 = data2[0]<<8;
                // micData2 |= data2[1];
                // //ESP_LOGI("i2c_rec:","ch1 mic: %d  ch2 mic: %d",micData, micData2);
                
                // ESP_LOGI("i2c_rec:","ch1 mic: %d, ref: %d, ki: %d, byp: %d ch2 mic: %d, ref: %d, ki: %d, byp: %d "
                // ,stm2EspData.MicPotiVal[0],stm2EspData.RefPotiVal[0],stm2EspData.KiPotiVal[0],stm2EspData.BypassPotiVal[0],
                // stm2EspData.MicPotiVal[1],stm2EspData.RefPotiVal[1],stm2EspData.KiPotiVal[1],stm2EspData.BypassPotiVal[1]);

                xSemaphoreGive(i2c_sem);
            }
    
    // float scaledValue = ( (float)micData / 32768.0f ) * 170.0f;
    // int mappedValue = (int)scaledValue; // This integer value is in the range [0, 170]
    xSemaphoreTake(lvgl_sem,100);
    for(int i = 0; i < 24; i += 3)
    {
        if(MainScreenBars[i] != NULL)
        {
            lv_bar_set_value(MainScreenBars[i], mapLinearToLogarithmic(stm2EspData.micData[i/3]), LV_ANIM_ON);
            lv_bar_set_value(MainScreenBars[i+1], mapLinearToLogarithmic(stm2EspData.refData[i/3]), LV_ANIM_ON);
            lv_bar_set_value(MainScreenBars[i+2], mapLinearToLogarithmic(stm2EspData.kiData[i/3]), LV_ANIM_ON);
        }
        //ESP_LOGI("ic_rec:","mic raw: %d mic log: %d ",stm2EspData.micData[i/3],mapLinearToLogarithmic(stm2EspData.micData[i/3]));
    }
    if(ui_Bar25 != NULL)//mic
    {
        lv_bar_set_value(ui_Bar25,(100.0/170.0)*mapLinearToLogarithmic(stm2EspData.micData[channelState.lastPress]),LV_ANIM_ON);
    }
    if(ui_Bar26 != NULL)//ref
    {
        lv_bar_set_value(ui_Bar26,(100.0/170.0)*mapLinearToLogarithmic(stm2EspData.refData[channelState.lastPress]),LV_ANIM_ON);
    }
    if(ui_Bar28 != NULL)//ki
    {
        lv_bar_set_value(ui_Bar28,(100.0/170.0)*mapLinearToLogarithmic(stm2EspData.kiData[channelState.lastPress]),LV_ANIM_ON);
    }
    xSemaphoreGive(lvgl_sem);
    vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void i2c_write_task(void *pvParameter) {
    i2c_cmd_handle_t i2chan;
    while (1) 
    {
         if (pdTRUE == xSemaphoreTake(i2c_sem, portMAX_DELAY)) 
            {
                for(uint8_t i = 0;i<8;i++)
                {
                    if(channelArray[i].changed == true)
                    {
                        uint8_t data[10];
                        memset(data,0,10);
                        data[0] = Channel_getMicVal(channelArray[i]);
                        data[1] = Channel_getRefVal(channelArray[i]);
                        data[2] = Channel_getKiVal(channelArray[i]);
                        data[3] = Channel_getByPassVal(channelArray[i]);
                        i2c_master_write_to_device(1,i+1,data,10,pdMS_TO_TICKS(100));//i2c address starts from 1, not 0
                        channelArray[i].changed = false;
                    }
                }
                xSemaphoreGive(i2c_sem);
            }
    vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void gpio_toggle_task(void *pvParameter) {
    
    while (1) 
    {
        gpio_set_level(40,0);
        vTaskDelay(pdMS_TO_TICKS(5));
        gpio_set_level(40,1);
    vTaskDelay(pdMS_TO_TICKS(5));
    }
}
void app_main(void)

{
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = 1ULL<<40;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = 0;
    io_conf.pull_down_en = 0;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);
    gpio_set_drive_capability(40,GPIO_DRIVE_CAP_1);
    gpio_set_level(40,1);

    for(int i = 0;i<NUM_CHANNEL;i++)
    {
        channelArray[i].channel = i;
        channelArray[i].potik.byPassVal = 0;
        channelArray[i].potik.kiVal = 0;
        channelArray[i].potik.micVal = 0;
        channelArray[i].potik.refVal = 0;
        channelArray[i].changed = false;
    }
    channelState.lastPress = CHANNEL_NONE;
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
    lvgl_init();
    //lv_example_bar_3();
    lvgl_sem =  xSemaphoreCreateBinary();
    i2c_sem = xSemaphoreCreateBinary();
    buttonSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(buttonSemaphore);
    xSemaphoreGive(lvgl_sem);
    xSemaphoreGive(i2c_sem);
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");
    
    vTaskDelay(pdMS_TO_TICKS(5000));
    xTaskCreate(&i2c_read_task,"i2c read slave",4096,NULL,5,NULL);
    xTaskCreate(&i2c_write_task,"i2c write slave",4096,NULL,5,NULL);
    xTaskCreate(&gpio_toggle_task,"gpio_toggle",4096,NULL,5,NULL);
    can_task_init();
    ui_init();
    init_Bar_Array();
    size_t free_heap = esp_get_free_heap_size();
    ESP_LOGI("Memory", "Free heap: %zu bytes", free_heap);
    while (1) {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(10));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        xSemaphoreTake(lvgl_sem,100);
        lv_timer_handler();
        xSemaphoreGive(lvgl_sem);
    }
}
