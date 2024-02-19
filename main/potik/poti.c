#include "poti.h"
#include <stdbool.h>

CHANNEL_TYPE Channel_Init(POTI_TYPE potiVals)
{
    CHANNEL_TYPE channel;
    channel.potik = potiVals;
    channel.changed = false;
    return channel;

}
void Channel_setMicVal(CHANNEL_TYPE *channel, uint8_t micVal)
{
    channel->potik.micVal = micVal;
    channel->changed = true;
}
void Channel_setRefVal(CHANNEL_TYPE *channel, uint8_t refVal)
{
    channel->potik.refVal = refVal;
    channel->changed = true;
}
void Channel_setKiVal(CHANNEL_TYPE *channel, uint8_t kiVal)
{
    channel->potik.kiVal = kiVal;
    channel->changed = true;
}
void Channel_setByPassVal(CHANNEL_TYPE *channel, uint8_t byPassVal)
{
    channel->potik.byPassVal = byPassVal;
    channel->changed = true;
}

uint8_t Channel_getMicVal(CHANNEL_TYPE channel)
{
    return channel.potik.micVal;
}
uint8_t Channel_getRefVal(CHANNEL_TYPE channel)
{
    return channel.potik.refVal;
}
uint8_t Channel_getKiVal(CHANNEL_TYPE channel)
{
    return channel.potik.kiVal;
}
uint8_t Channel_getByPassVal(CHANNEL_TYPE channel)
{
    return channel.potik.byPassVal;
}
