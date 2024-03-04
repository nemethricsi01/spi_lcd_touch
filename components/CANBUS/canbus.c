#include <stdio.h>
#include "canbus.h"
#include "esp_timer.h"

void CanMessageCircBuff_init(CanMessageCircBuff *buff)
{
    for (int i = 0; i < 100; i++)
    {
        buff->messages[i].length = 0;
        buff->messages[i].timestamp = 0;
        memset(buff->messages[i].data, 0, 4);
    }
    buff->writeIndex = 0;
    buff->overflowed = 0;
    buff->readIndex = 0;
}

esp_err_t FillCanMessage(uint8_t *buff, uint8_t messageLength, CanMessage *message)
{
    esp_err_t ret = ESP_OK;
    if (messageLength > 4 || messageLength < 3)
    {
        ret = ESP_ERR_INVALID_SIZE;
    }
    if(buff == NULL)
    {
        ret = ESP_ERR_INVALID_ARG;
    }
    if(message == NULL)
    {
        ret = ESP_ERR_INVALID_ARG;
    }
    message->length = messageLength;
    message->timestamp = esp_timer_get_time();
    ESP_LOGI("FillCanMessage", "timestamp: %ld", message->timestamp);
    memcpy(message->data, buff, messageLength);
    return ret;
}
esp_err_t CanMessageCircBuff_push(CanMessageCircBuff *buff, CanMessage *message)
{
    esp_err_t ret = ESP_OK;
    if(buff == NULL)
    {
        ret = ESP_ERR_INVALID_ARG;
    }
    if(message == NULL)
    {
        ret = ESP_ERR_INVALID_ARG;
    }
    if(buff->writeIndex == 100)
    {
        buff->writeIndex = 0;
        ESP_LOGI("CanMessageCircBuff_push", "buffer overflow");
        buff->overflowed = 1;
    }
    message->sequenceNumber = buff->writeIndex;
    memcpy(&buff->messages[buff->writeIndex], message, sizeof(CanMessage));
    ESP_LOGI("CanMessageCircBuff_push", "timestamp: %ld wrindex: %d", buff->messages[buff->writeIndex].timestamp, buff->writeIndex);
    buff->writeIndex++;
    return ret;
}