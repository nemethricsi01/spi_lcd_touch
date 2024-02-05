#ifndef POTI_H
#define POTI_H
#include <stdint.h>
#include <stdbool.h>
typedef enum 
{
    CH_1 = 0,
    CH_2,
    CH_3,
    CH_4,
    CH_5,
    CH_6,
    CH_7,
    CH_8
} CHANNEL_NUM_ENUM;
typedef struct 
{
    uint8_t micVal;
    uint8_t refVal;
    uint8_t kiVal;
    uint8_t byPassVal;
} POTI_TYPE;

typedef struct 
{
    CHANNEL_NUM_ENUM channel;
    POTI_TYPE potik;
    bool changed;
} CHANNEL_TYPE;

#define NUM_CHANNEL 8

CHANNEL_TYPE Channel_Init(POTI_TYPE potiVals);

void Channel_setMicVal(CHANNEL_TYPE *channel, uint8_t micVal);
void Channel_setRefVal(CHANNEL_TYPE *channel, uint8_t refVal);
void Channel_setKiVal(CHANNEL_TYPE *channel, uint8_t kiVal);
void Channel_setByPassVal(CHANNEL_TYPE *channel, uint8_t byPassVal);

uint8_t Channel_getMicVal(CHANNEL_TYPE channel);
uint8_t Channel_getRefVal(CHANNEL_TYPE channel);
uint8_t Channel_getKiVal(CHANNEL_TYPE channel);
uint8_t Channel_getByPassVal(CHANNEL_TYPE channel);




#endif // POTI_H
