#ifndef _CANBUS_H
#define _CANBUS_H
#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "esp_log.h"
#include "string.h"















typedef enum {
    NARV_COMM_ERROR                 = 0,
    NARV_COMM_CALL_START            = 1,
    NARV_COMM_CALL_END              = 2,
    NARV_COMM_RING_START            = 4,
    NARV_COMM_RING_END              = 8,
    NARV_COMM_CALLB_REQ             = 16,
    NARV_COMM_CALLB_TERM            = 32,
} NARVAL_3BYTE_COMMANDS_ENUM;

typedef enum {
    NARV_COMM4_ERROR                = 0x00,

    NARV_COMM4_CLOCKDATE            = 0x01,
    NARV_COMM4_CLOCK_DOTINFO        = 0x03,
    NARV_COMM4_DATE                 = 0x02,

    NARV_COMM4_MP3                  = 0x04,

    NARV_COMM4_INTERURB_BUSY        = 0x08,

    NARV_COMM4_PHONE                = 0x28,

    NARV_COMM4_LEDLAYER1            = 0x10,
    NARV_COMM4_LEDLAYER2            = 0x11,
    NARV_COMM4_LEDLAYER3            = 0x12,
    NARV_COMM4_LEDLAYER4            = 0x13,
    NARV_COMM4_LEDLAYER5            = 0x14,
    NARV_COMM4_LEDLAYER6            = 0x15,
    NARV_COMM4_LEDLAYER7            = 0x16,
    NARV_COMM4_LEDLAYER8            = 0x17,
    NARV_COMM4_LEDLAYER10           = 0x19,
    NARV_COMM4_LEDLAYER11           = 0x1A,
    NARV_COMM4_LEDLAYER12           = 0x1B,
    NARV_COMM4_LEDLAYER13           = 0x1C,
    NARV_COMM4_LEDLAYER14           = 0x1D,
    NARV_COMM4_LEDLAYER15           = 0x1E,
    NARV_COMM4_LEDLAYER16           = 0x1F,
    NARV_COMM4_BUTTONPRESS          = 0x18,
    NARV_COMM4_REMOTECTRL           = 0x20,

    NARV_COMM4_LEDLAYER1_REMOTE     = 0x30,
    NARV_COMM4_LEDLAYER2_REMOTE     = 0x31,
    NARV_COMM4_LEDLAYER3_REMOTE     = 0x32,
    NARV_COMM4_LEDLAYER4_REMOTE     = 0x33,
    NARV_COMM4_LEDLAYER5_REMOTE     = 0x34,
    NARV_COMM4_LEDLAYER6_REMOTE     = 0x35,
    NARV_COMM4_LEDLAYER7_REMOTE     = 0x36,
    NARV_COMM4_LEDLAYER8_REMOTE     = 0x37,
    NARV_COMM4_LEDLAYER10_REMOTE    = 0x39,
    NARV_COMM4_LEDLAYER11_REMOTE    = 0x3A,
    NARV_COMM4_LEDLAYER12_REMOTE    = 0x3B,
    NARV_COMM4_LEDLAYER13_REMOTE    = 0x3C,
    NARV_COMM4_LEDLAYER14_REMOTE    = 0x3D,
    NARV_COMM4_LEDLAYER15_REMOTE    = 0x3E,
    NARV_COMM4_LEDLAYER16_REMOTE    = 0x3F,

    NARV_COMM4_RESET                = 0xFF
} NARVAL_4BYTE_COMMAND_TYPES_ENUM;

typedef enum {
    NARV_INTERURB_INTERURB_BUSY     = 0x00,
    NARV_INTERURB_MATRIX_BUSY       = 0x01

} NARVAL_4BYTE_COMMAND_INTERURB_ENUM;
typedef enum {
    NARV_MP3_START                  = 0x01,
    NARV_MP3_STOP_ALL               = 0x02,
    NARV_MP3_STOP_ACT               = 0x03,
    NARV_MP3_REMOVEFROM_PLAYLIST    = 0x04,
    NARV_MP3_PAUSE                  = 0x05

} NARVAL_4BYTE_COMMAND_MP3_ENUM;
typedef enum {
    NARV_PHONE_CALLEND_TOMODEM      = 0x00,
    NARV_PHONE_PICKUP_TOMODEM       = 0x01,
    NARV_PHONE_DTMF_TOMODEM         = 0x02,
    NARV_PHONE_RING_TOMODEM         = 0x03,
    NARV_PHONE_RING_STOP_TOMODEM    = 0x04,
    NARV_PHONE_CALLEND_TOPHONE      = 0x08,
    NARV_PHONE_PICKUP_TOPHONE       = 0x09,
    NARV_PHONE_DTMF_TOPHONE         = 0x0A,
    NARV_PHONE_RING_TOPHONE         = 0x0B,
    NARV_PHONE_RING_STOP_TOPHONE    = 0x0C

} NARVAL_4BYTE_COMMAND_PHONE_ENUM;

typedef enum {
    NARV_HH_BUTTONPRESS             = 0x01,
    NARV_HH_GROUPLEARN              = 0x03,
    NARV_HH_LED_QUERY               = 0x04,
    NARV_HH_BUTTONRELEASE           = 0x08,
} NARVAL_4BYTE_COMMAND_HH_ENUM;

typedef enum {
    NARV_REMCTRL_QUERY              = 0x00,
    NARV_REMCTRL_VOLUME             = 0x01,
    NARV_REMCTRL_CHANNEL            = 0x02,
    NARV_REMCTRL_BASS               = 0x05,
    NARV_REMCTRL_TREBLE             = 0x06,
    NARV_REMCTRL_HDMI               = 0x07,
    NARV_REMCTRL_ONOFF              = 0x0F,
    NARV_REMCTRL_BRIGHT             = 0x09,
    NARV_REMCTRL_MUVIND             = 0x03,
} NARVAL_4BYTE_COMMAND_REMOTECTRL_ENUM;


typedef struct {
    uint8_t length;     // Length of the message (3 or 4)
    uint8_t sequenceNumber;// Sequence number
    uint32_t timestamp; // Timestamp
    uint8_t data[4];    // Message data
} CanMessage;

typedef struct {
    uint8_t writeIndex;
    uint8_t overflowed;
    int16_t readIndex;
    CanMessage messages[100];
} CanMessageCircBuff;

void CanMessageCircBuff_init(CanMessageCircBuff *buff);
esp_err_t FillCanMessage(uint8_t *buff, uint8_t messageLength, CanMessage *message);
esp_err_t CanMessageCircBuff_push(CanMessageCircBuff *buff, CanMessage *message);

#endif//_CANBUS_H
