#ifndef NVS_H
#define NVS_H
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "esp_err.h"

// Include any necessary headers here
esp_err_t nvs_init();
esp_err_t nvs_write_uint8(const char* key, uint8_t value);
esp_err_t nvs_read(const char* key, uint8_t *value);
esp_err_t nvs_erase(const char* key);

// Define any constants or macros here

// Declare any global variables or data structures here

// Declare function prototypes here

#endif // NVS_H
