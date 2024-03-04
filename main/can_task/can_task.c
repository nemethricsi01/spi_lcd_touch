#include "can_task/can_task.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/twai.h"
#include "esp_err.h"
#include "canbus.h"
#include "ui.h"
#include "lvgl.h"

static const char *TAG = "can_task";

static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
//static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_10KBITS();
static const twai_timing_config_t t_config = {
    .clk_src = TWAI_CLK_SRC_DEFAULT,
    .brp = 584,//Baud_Rate = Clock_Frequency / (BRP * (1 + TSEG1 + TSEG2))
    .tseg_1 = 7, .tseg_2 = 6, .sjw = 1,
    .triple_sampling = false
};
//Set TX queue length to 0 due to listen only mode
static const twai_general_config_t g_config = {.mode = TWAI_MODE_LISTEN_ONLY,
                                              .tx_io = TX_GPIO_NUM, .rx_io = RX_GPIO_NUM,
                                              .clkout_io = TWAI_IO_UNUSED, .bus_off_io = TWAI_IO_UNUSED,
                                              .tx_queue_len = 0, .rx_queue_len = 200,
                                              .alerts_enabled = TWAI_ALERT_NONE,
                                              .clkout_divider = 0};
twai_status_info_t canStatus;

CanMessageCircBuff canMessageCircBuff;
CanMessage canMessage;
extern lv_obj_t * ui_TextArea6;
extern SemaphoreHandle_t lvgl_sem;
extern SemaphoreHandle_t buttonSemaphore;

volatile char canMessageText[5000];


void handle_led_layer(char* buffer, int sequenceNumber, long hours, long minutes, long seconds, uint8_t* data, long timestamp, char* ledLabel) {
    char led2bit[4] = {'0','Z','P','S'};
    int offset = sprintf(buffer, "%d. %02ld:%02ld:%02ld  %s:", sequenceNumber, hours, minutes, seconds, ledLabel);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            offset += sprintf(buffer + offset, "%c", led2bit[(data[i] >> (j * 2)) & 0x3]);
        }
        if (i != 2) {
            offset += sprintf(buffer + offset, "_");
        }
    }
    sprintf(buffer + offset, "  %02ld.%03ld\n",
            
            seconds, timestamp % 1000000 / 1000);

    // char led2bit[4] = {'0','Z','P','S'};
            // int offset = sprintf(buffer, "%d. %02ld:%02ld:%02ld  LEDL1-12:", message->sequenceNumber, hours, minutes, seconds);

            // for (int i = 0; i < 3; i++) {
            //     for (int j = 0; j < 4; j++) {
            //         offset += sprintf(buffer + offset, "%c", led2bit[(message->data[i] >> (j * 2)) & 0x3]);
            //     }
            //     if (i != 2) {
            //         offset += sprintf(buffer + offset, "_");
            //     }
            // }
            // sprintf(buffer + offset, "  0x%x_%x_%x_%x  %02ld.%03ld\n",
            //         message->data[0], message->data[1], message->data[2], message->data[3],
            //         seconds, message->timestamp % 1000000 / 1000);
}
void handle_remote_led_layer(char* buffer, int sequenceNumber, long hours, long minutes, long seconds, uint8_t* data, long timestamp, char* ledLabel) {
    char led2bit[4] = {'0','Z','P','S'};
    int offset = sprintf(buffer, "%d. %02ld:%02ld:%02ld  %s: %d", sequenceNumber, hours, minutes, seconds, ledLabel, data[0]);

    for (int i = 1; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            offset += sprintf(buffer + offset, "%c", led2bit[(data[i] >> (j * 2)) & 0x3]);
        }
        if (i != 2) {
            offset += sprintf(buffer + offset, "_");
        }
    }
    sprintf(buffer + offset, " %02ld.%03ld\n",
            
            seconds, timestamp % 1000000 / 1000);

    // char led2bit[4] = {'0','Z','P','S'};
            // int offset = sprintf(buffer, "%d. %02ld:%02ld:%02ld  LEDL1-12:", message->sequenceNumber, hours, minutes, seconds);

            // for (int i = 0; i < 3; i++) {
            //     for (int j = 0; j < 4; j++) {
            //         offset += sprintf(buffer + offset, "%c", led2bit[(message->data[i] >> (j * 2)) & 0x3]);
            //     }
            //     if (i != 2) {
            //         offset += sprintf(buffer + offset, "_");
            //     }
            // }
            // sprintf(buffer + offset, "  0x%x_%x_%x_%x  %02ld.%03ld\n",
            //         message->data[0], message->data[1], message->data[2], message->data[3],
            //         seconds, message->timestamp % 1000000 / 1000);
}

void format_can_message(const CanMessage* message, char* buffer, const char* action) {
    long int hours, minutes, seconds;

    // Convert microseconds to seconds
    seconds = message->timestamp / 1000000;

    // Calculate hours, minutes, and seconds
    hours = seconds / 3600;
    seconds %= 3600;
    minutes = seconds / 60;
    seconds %= 60;

    sprintf(buffer, "%d. %02ld:%02ld:%02ld       %d %s %d       0x%x_%x_%x      %02ld.%03ld\n",
        message->sequenceNumber,
        hours, minutes, seconds,
        message->data[0]+1,
        action,
        message->data[1]+1,
        message->data[0],
        message->data[1],
        message->data[2], 
        seconds,
        message->timestamp % 1000000 / 1000);
}
void format_can_message4byte(const CanMessage* message, char* buffer,int16_t command)
{
    long int hours, minutes, seconds;

    // Convert microseconds to seconds
    seconds = message->timestamp / 1000000;

    // Calculate hours, minutes, and seconds
    hours = seconds / 3600;
    seconds %= 3600;
    minutes = seconds / 60;
    seconds %= 60;
    switch(command)
    {
        case NARV_COMM_ERROR:
        {
            sprintf(buffer, "%d. %02ld:%02ld:%02ld COMM ERROR  0x%x_%x_%x_%x  %ld\n",
            message->sequenceNumber,
            hours, minutes, seconds,
            message->data[0],
            message->data[1],
            message->data[2], 
            message->data[3],
            message->timestamp/1000);
        break;
        }
        case NARV_COMM4_CLOCKDATE:
        {
            sprintf(buffer, "%d. %02ld:%02ld:%02ld     clk%d%s: %02d:%02d:%02d     0x%x_%x_%x_%x     %02ld.%03ld\n",
            message->sequenceNumber,
            hours, minutes, seconds,
            (message->data[0]&0xe0)>>5,
            ((message->data[1]&0xc0)>>6 == 0)? "":((message->data[1]&0xc0)>>6 == 1)?"dwn":((message->data[1]&0xc0)>>6 == 2)?"up":"stp",
            message->data[0]&0x1f,
            message->data[1]&0x3f,
            message->data[2]&0x3f,
            message->data[0],
            message->data[1],
            message->data[2],
            message->data[3],
            seconds,
            message->timestamp % 1000000 / 1000);
        break;
        }
        case NARV_COMM4_DATE:
        {
            sprintf(buffer, "%d. %02ld:%02ld:%02ld    date: 20%02d.%02d.%02d     0x%x_%x_%x_%x     %02ld.%03ld\n",
            message->sequenceNumber,
            hours, minutes, seconds,
            message->data[0],
            message->data[1],
            message->data[2],
            message->data[0],
            message->data[1],
            message->data[2],
            message->data[3],
            seconds,
            message->timestamp % 1000000 / 1000);
        break;
        }
        case NARV_COMM4_CLOCK_DOTINFO:
        {
            sprintf(buffer, "%d. %02ld:%02ld:%02ld    dot:0x%x_%x_%x      0x%x_%x_%x_%x     %02ld.%03ld\n",
            message->sequenceNumber,
            hours, minutes, seconds,
            message->data[0],
            message->data[1],
            message->data[2],
            message->data[0],
            message->data[1],
            message->data[2],
            message->data[3],
            seconds,
            message->timestamp % 1000000 / 1000);
        break;
        }
        case NARV_COMM4_MP3:
        {
            switch (message->data[2])
            {
                    case NARV_MP3_START:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d mp3 Start: track:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[1]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_MP3_STOP_ACT:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d mp3 Stop act   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_MP3_STOP_ALL:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d mp3 Stop all   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    }
                    break;
                    case NARV_MP3_PAUSE:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d mp3 pause  0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_MP3_REMOVEFROM_PLAYLIST:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld   %d mp3 del all track  0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    default:
                    break;
            }
        break;
        }
        case NARV_COMM4_INTERURB_BUSY:
        {
            switch (message->data[2])
            {
                    case NARV_INTERURB_INTERURB_BUSY:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d inturb inturb bsy:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[1]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_INTERURB_MATRIX_BUSY:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d inturb mtx bsy:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[1]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    default:
                    break;
            }
        break;
        }
        case NARV_COMM4_PHONE:
        {
            switch (message->data[1])
            {
                    case NARV_PHONE_CALLEND_TOMODEM:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d callend toMod   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_PHONE_PICKUP_TOMODEM:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d pickup toMod   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_PHONE_DTMF_TOMODEM:
                    {
                        char dtmf[11] = {'0','1','2','3','4','5','6','7','8','9','*','#'};
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d dtmf2mod:%c,l:%dms   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        dtmf[message->data[2]&0xf],
                        (message->data[2]>>4)*25,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_PHONE_RING_TOMODEM:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d ring toMod   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_PHONE_RING_STOP_TOMODEM:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d ring stop toMod   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_PHONE_CALLEND_TOPHONE:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d callend toPho   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_PHONE_PICKUP_TOPHONE:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d pickup toPho   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_PHONE_DTMF_TOPHONE:
                    {
                        char dtmf[11] = {'0','1','2','3','4','5','6','7','8','9','*','#'};
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d dtmf2pho:%c,l:%dms   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        dtmf[message->data[2]&0xf],
                        (message->data[2]>>4)*25,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_PHONE_RING_TOPHONE:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d ring toPho   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_PHONE_RING_STOP_TOPHONE:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d ring stop toPho   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    default:
                    break;
            }
        break;
        }
        case NARV_COMM4_LEDLAYER1:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL1-12");
        break;
        }
        case NARV_COMM4_LEDLAYER2:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL13-24");
        break;
        }
        case NARV_COMM4_LEDLAYER3:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL25-36");
        break;
        }
        case NARV_COMM4_LEDLAYER4:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL37-49");
        break;
        }
        case NARV_COMM4_LEDLAYER5:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL50-61");
        break;
        }
        case NARV_COMM4_LEDLAYER6:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL62-73");
        break;
        }
        case NARV_COMM4_LEDLAYER7:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL74-85");

        break;
        }
        case NARV_COMM4_LEDLAYER8:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL86-97");
        break;
        }
        case NARV_COMM4_LEDLAYER10:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL98-109");
        break;
        }
        case NARV_COMM4_LEDLAYER11:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL110-121");
        break;
        }
        case NARV_COMM4_LEDLAYER12:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL122-133");

        break;
        }
        case NARV_COMM4_LEDLAYER13:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL134-145");
        break;
        }
        case NARV_COMM4_LEDLAYER14:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL146-157");
        break;
        }
        case NARV_COMM4_LEDLAYER15:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL158-169");
        break;
        }
        case NARV_COMM4_LEDLAYER16:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "LEDL170-181");
        break;
        }
        case NARV_COMM4_BUTTONPRESS:
        {
            switch (message->data[1])
            {
                    case NARV_HH_BUTTONPRESS:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d hhgomble:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[2]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_HH_GROUPLEARN:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d hhtanit:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[2]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_HH_LED_QUERY:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d ledker%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[2]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    case NARV_HH_BUTTONRELEASE:
                    {
                        sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d csend/gombfel:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                        message->sequenceNumber,
                        hours, minutes, seconds,
                        message->data[0]+1,
                        message->data[2]+1,
                        message->data[0],
                        message->data[1],
                        message->data[2],
                        message->data[3],
                        seconds,
                        message->timestamp % 1000000 / 1000);
                    break;
                    }
                    default:
                    break;
            }
        break;
        }
        case NARV_COMM4_REMOTECTRL:
        {
            if(message->data[1] == NARV_REMCTRL_QUERY)
            {
                switch (message->data[2])
                {
                        case NARV_REMCTRL_VOLUME:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemVolKer   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_CHANNEL:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemChKer   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_BASS:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemBassKer   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_TREBLE:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemTrebKer   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_HDMI:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemHdmiKer   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_ONOFF:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemOnOffKer   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_BRIGHT:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemBrightKer   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        default:
                        break;
                }
            }
            else
            {
                switch (message->data[1])
                {
                        case NARV_REMCTRL_VOLUME:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemVol:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[2],
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_CHANNEL:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemCh:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[2],
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_BASS:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemBass:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[2],
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_TREBLE:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemTreb:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[2],
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_HDMI:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemHdmi:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[2],
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_ONOFF:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemOnOff:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[2],
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_BRIGHT:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cRemBright:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'k':'t',//közp,tav
                            message->data[2],
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        case NARV_REMCTRL_MUVIND:
                        {
                            sprintf(buffer, "%d. %02ld:%02ld:%02ld    %d %cMuvInd:%d   0x%x_%x_%x_%x     %02ld.%03ld\n",
                            message->sequenceNumber,
                            hours, minutes, seconds,
                            (message->data[0]&0x7f)+1,
                            (message->data[0]&0x80)?'S':'M',//Master = 0,SLV = 1
                            message->data[2],
                            message->data[0],
                            message->data[1],
                            message->data[2],
                            message->data[3],
                            seconds,
                            message->timestamp % 1000000 / 1000);
                        break;
                        }
                        default:
                        break;
                }
            }
            
        break;
        }
        case NARV_COMM4_LEDLAYER1_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL1-12");
            break;
        }
        case NARV_COMM4_LEDLAYER2_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL13-24");
            break;
        }
        case NARV_COMM4_LEDLAYER3_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL25-36");
            break;
        }
        case NARV_COMM4_LEDLAYER4_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL37-49");
            break;
        }
        case NARV_COMM4_LEDLAYER5_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL50-61");
            break;
        }
        case NARV_COMM4_LEDLAYER6_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL62-73");
            break;
        }
        case NARV_COMM4_LEDLAYER7_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL74-85");
            break;
        }
        case NARV_COMM4_LEDLAYER8_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL86-97");
            break;
        }
        case NARV_COMM4_LEDLAYER10_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL98-109");
            break;
        }
        case NARV_COMM4_LEDLAYER11_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL110-121");
            break;
        }
        case NARV_COMM4_LEDLAYER12_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL122-133");
            break;
        }
        case NARV_COMM4_LEDLAYER13_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL134-145");
            break;
        }
        case NARV_COMM4_LEDLAYER14_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL146-157");
            break;
        }
        case NARV_COMM4_LEDLAYER15_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL158-169");
            break;
        }
        case NARV_COMM4_LEDLAYER16_REMOTE:
        {
            handle_led_layer(buffer, message->sequenceNumber, hours, minutes, seconds, message->data, message->timestamp, "REMLEDL170-181");
            break;
        }

    
    
    }
    
}

void can_task_init()
{
    //Install and start TWAI driver
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(TAG, "Driver installed");
    ESP_ERROR_CHECK(twai_start());
    ESP_LOGI(TAG, "Driver started");
    xTaskCreate(&can_receive_task,"can receive task",4096,NULL,5,NULL);
}


void can_receive_task(void *pvParameter)
{
    twai_message_t rx_msg;
    CanMessageCircBuff_init(&canMessageCircBuff);
    
    ESP_LOGI(TAG, "can_task started");
    while(1)
    {
        if(twai_receive(&rx_msg, pdMS_TO_TICKS(100)) == ESP_OK)
        {
            ESP_LOGI(TAG, "Received message");
            FillCanMessage(rx_msg.data, rx_msg.data_length_code, &canMessage);
            CanMessageCircBuff_push(&canMessageCircBuff, &canMessage);
            ESP_LOGI(TAG, "msg len: %d, msg0: %x,msg1: %x,msg2: %x,msg3: %x", 
            rx_msg.data_length_code, rx_msg.data[0], rx_msg.data[1], rx_msg.data[2], rx_msg.data[3]);
            twai_get_status_info(&canStatus);
            if(canStatus.msgs_to_rx == 0)
            {
                
            
                xSemaphoreTake(lvgl_sem,100);

                if(ui_TextArea6 != NULL)
                {
                    
                    if(canMessageCircBuff.overflowed == 0)
                    {
                        memset(canMessageText, 0, sizeof(canMessageText));

                        for(int i = canMessageCircBuff.writeIndex-1;i>=0;i--)
                        {
                            char line[100];  // Buffer for one line
                            static uint32_t seconds, minutes, hours;
                            seconds = canMessageCircBuff.messages[i].timestamp/1000000;
                            hours = seconds/3600;
                            seconds %= 3600;
                            minutes = seconds/60;
                            seconds %= 60;
                            switch(canMessageCircBuff.messages[i].length)
                            {
                                case 3:
                                switch(canMessageCircBuff.messages[i].data[2])
                                {
                                    case NARV_COMM_ERROR:
                                        sprintf(line, "%d. %02ld:%02ld:%02ld COMM ERROR  0x%x_%x_%x  %ld\n",
                                        
                                        canMessageCircBuff.messages[i].sequenceNumber,
                                        hours, minutes, seconds,
                                        canMessageCircBuff.messages[i].data[0],
                                        canMessageCircBuff.messages[i].data[1],
                                        canMessageCircBuff.messages[i].data[2], 
                                        canMessageCircBuff.messages[i].timestamp/1000);
                                        break;
                                        case NARV_COMM_CALL_START:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "hivja");
                                        break;
                                        case NARV_COMM_CALL_END:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "bontja");
                                        break;
                                        case NARV_COMM_RING_START:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "csong start");
                                        break;
                                        case NARV_COMM_RING_END:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "csong end");
                                        break;
                                        case NARV_COMM_CALLB_REQ:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "visszahiv ker");
                                        break;
                                        case NARV_COMM_CALLB_TERM:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "visszh. bont");
                                        break;
                                }
                                break;
                                case 4:
                                    switch(canMessageCircBuff.messages[i].data[3])
                                        {
                                            case NARV_COMM4_ERROR:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM_ERROR);
                                                break;
                                                case NARV_COMM4_CLOCKDATE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_CLOCKDATE);
                                                break;
                                                case NARV_COMM4_CLOCK_DOTINFO:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_CLOCK_DOTINFO);
                                                break;
                                                case NARV_COMM4_DATE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_DATE);
                                                break;
                                                case NARV_COMM4_MP3:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_MP3);
                                                break;
                                                case NARV_COMM4_PHONE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_PHONE);
                                                break;
                                                case NARV_COMM4_LEDLAYER1:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER1);
                                                break;
                                                case NARV_COMM4_LEDLAYER2:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER2);
                                                break;
                                                case NARV_COMM4_LEDLAYER3:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER3);
                                                break;
                                                case NARV_COMM4_LEDLAYER4:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER4);
                                                break;
                                                case NARV_COMM4_LEDLAYER5:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER5);
                                                break;
                                                case NARV_COMM4_LEDLAYER6:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER6);
                                                break;
                                                case NARV_COMM4_LEDLAYER7:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER7);
                                                break;
                                                case NARV_COMM4_LEDLAYER8:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER8);
                                                break;
                                                case NARV_COMM4_LEDLAYER10:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER10);
                                                break;
                                                case NARV_COMM4_LEDLAYER11:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER11);
                                                break;
                                                case NARV_COMM4_LEDLAYER12:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER12);
                                                break;
                                                case NARV_COMM4_LEDLAYER13:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER13);
                                                break;
                                                case NARV_COMM4_LEDLAYER14:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER14);
                                                break;
                                                case NARV_COMM4_LEDLAYER15:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER15);
                                                break;
                                                case NARV_COMM4_LEDLAYER16:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER16);
                                                break;
                                                case NARV_COMM4_BUTTONPRESS:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_BUTTONPRESS);
                                                break;
                                                case NARV_COMM4_REMOTECTRL:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_REMOTECTRL);
                                                break;
                                                case NARV_COMM4_LEDLAYER1_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER1_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER2_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER2_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER3_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER3_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER4_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER4_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER5_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER5_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER6_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER6_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER7_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER7_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER8_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER8_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER10_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER10_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER11_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER11_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER12_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER12_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER13_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER13_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER14_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER14_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER15_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER15_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER16_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER16_REMOTE);
                                                break;
                                                case NARV_COMM4_RESET:
                                                {
                                                    sprintf(line, "%d. %02ld:%02ld:%02ld RESET: %d  0x%x_%x_%x_%x  %ld\n",
                                                    canMessageCircBuff.messages[i].sequenceNumber,
                                                    hours, minutes, seconds,
                                                    canMessageCircBuff.messages[i].data[3],
                                                    canMessageCircBuff.messages[i].data[0],
                                                    canMessageCircBuff.messages[i].data[1],
                                                    canMessageCircBuff.messages[i].data[2], 
                                                    canMessageCircBuff.messages[i].data[3],
                                                    canMessageCircBuff.messages[i].timestamp/1000);
                                                    break;
                                                }
                                        }
                                    break;
                                default:
                                    sprintf(line, "valami baj lett :)\n");
                                    break;
                            }
                            strncat(canMessageText, line, sizeof(canMessageText) - strlen(canMessageText) - 1);
                            
                        }
                        canMessageText[sizeof(canMessageText) - 1] = '\0';  // Ensure null-termination
                        lv_textarea_set_text(ui_TextArea6,canMessageText);
                        lv_obj_scroll_to_y(ui_TextArea6,0,LV_ANIM_OFF);
                    }
                    else
                    {
                        memset(canMessageText, 0, sizeof(canMessageText));
                        ESP_LOGI(TAG, "Buffer overflowed");
                        int i = canMessageCircBuff.writeIndex;
                        do
                        {
                            i = (i - 1 + 100) % 100;  // Go to the previous index, wrapping around if necessary

                             char line[100];  // Buffer for one line
                            static uint32_t seconds, minutes, hours;
                            seconds = canMessageCircBuff.messages[i].timestamp/1000000;
                            hours = seconds/3600;
                            seconds %= 3600;
                            minutes = seconds/60;
                            seconds %= 60;
                            switch(canMessageCircBuff.messages[i].length)
                            {
                                case 3:
                                switch(canMessageCircBuff.messages[i].data[2])
                                {
                                    case NARV_COMM_ERROR:
                                        sprintf(line, "%d. %02ld:%02ld:%02ld COMM ERROR  0x%x_%x_%x  %ld\n",
                                        
                                        canMessageCircBuff.messages[i].sequenceNumber,
                                        hours, minutes, seconds,
                                        canMessageCircBuff.messages[i].data[0],
                                        canMessageCircBuff.messages[i].data[1],
                                        canMessageCircBuff.messages[i].data[2], 
                                        canMessageCircBuff.messages[i].timestamp/1000);
                                        break;
                                        case NARV_COMM_CALL_START:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "hivja");
                                        break;
                                        case NARV_COMM_CALL_END:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "bontja");
                                        break;
                                        case NARV_COMM_RING_START:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "csong start");
                                        break;
                                        case NARV_COMM_RING_END:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "csong end");
                                        break;
                                        case NARV_COMM_CALLB_REQ:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "visszahiv ker");
                                        break;
                                        case NARV_COMM_CALLB_TERM:
                                        format_can_message(&canMessageCircBuff.messages[i], line, "visszh. bont");
                                        break;
                                }
                                break;
                                case 4:
                                    switch(canMessageCircBuff.messages[i].data[3])
                                        {
                                            case NARV_COMM4_ERROR:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM_ERROR);
                                                break;
                                                case NARV_COMM4_CLOCKDATE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_CLOCKDATE);
                                                break;
                                                case NARV_COMM4_CLOCK_DOTINFO:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_CLOCK_DOTINFO);
                                                break;
                                                case NARV_COMM4_DATE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_DATE);
                                                break;
                                                case NARV_COMM4_MP3:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_MP3);
                                                break;
                                                case NARV_COMM4_PHONE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_PHONE);
                                                break;
                                                case NARV_COMM4_LEDLAYER1:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER1);
                                                break;
                                                case NARV_COMM4_LEDLAYER2:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER2);
                                                break;
                                                case NARV_COMM4_LEDLAYER3:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER3);
                                                break;
                                                case NARV_COMM4_LEDLAYER4:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER4);
                                                break;
                                                case NARV_COMM4_LEDLAYER5:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER5);
                                                break;
                                                case NARV_COMM4_LEDLAYER6:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER6);
                                                break;
                                                case NARV_COMM4_LEDLAYER7:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER7);
                                                break;
                                                case NARV_COMM4_LEDLAYER8:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER8);
                                                break;
                                                case NARV_COMM4_LEDLAYER10:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER10);
                                                break;
                                                case NARV_COMM4_LEDLAYER11:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER11);
                                                break;
                                                case NARV_COMM4_LEDLAYER12:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER12);
                                                break;
                                                case NARV_COMM4_LEDLAYER13:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER13);
                                                break;
                                                case NARV_COMM4_LEDLAYER14:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER14);
                                                break;
                                                case NARV_COMM4_LEDLAYER15:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER15);
                                                break;
                                                case NARV_COMM4_LEDLAYER16:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER16);
                                                break;
                                                case NARV_COMM4_BUTTONPRESS:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_BUTTONPRESS);
                                                break;
                                                case NARV_COMM4_REMOTECTRL:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_REMOTECTRL);
                                                break;
                                                case NARV_COMM4_LEDLAYER1_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER1_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER2_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER2_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER3_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER3_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER4_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER4_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER5_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER5_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER6_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER6_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER7_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER7_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER8_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER8_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER10_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER10_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER11_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER11_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER12_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER12_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER13_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER13_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER14_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER14_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER15_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER15_REMOTE);
                                                break;
                                                case NARV_COMM4_LEDLAYER16_REMOTE:
                                                format_can_message4byte(&canMessageCircBuff.messages[i], line, NARV_COMM4_LEDLAYER16_REMOTE);
                                                break;
                                                case NARV_COMM4_RESET:
                                                {
                                                    sprintf(line, "%d. %02ld:%02ld:%02ld RESET: %d  0x%x_%x_%x_%x  %ld\n",
                                                    canMessageCircBuff.messages[i].sequenceNumber,
                                                    hours, minutes, seconds,
                                                    canMessageCircBuff.messages[i].data[3],
                                                    canMessageCircBuff.messages[i].data[0],
                                                    canMessageCircBuff.messages[i].data[1],
                                                    canMessageCircBuff.messages[i].data[2], 
                                                    canMessageCircBuff.messages[i].data[3],
                                                    canMessageCircBuff.messages[i].timestamp/1000);
                                                    break;
                                                }
                                        }
                                    break;
                                default:
                                    sprintf(line, "valami baj lett :)\n");
                                    break;
                            }
                            strncat(canMessageText, line, sizeof(canMessageText) - strlen(canMessageText) - 1);
                        }
                        while (i != canMessageCircBuff.writeIndex);
                        canMessageText[sizeof(canMessageText) - 1] = '\0';  // Ensure null-termination
                        lv_textarea_set_text(ui_TextArea6,canMessageText);
                        lv_obj_scroll_to_y(ui_TextArea6,0,LV_ANIM_OFF);
                    }
                    
                
                }
                xSemaphoreGive(lvgl_sem);
            }
        }
        
        // ESP_LOGI(TAG, "can_task running");
        // vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}