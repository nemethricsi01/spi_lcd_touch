#include "nvs/nvs.h"
esp_err_t nvs_init()
{
    esp_err_t ret = ESP_OK;
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    return ret;

}
esp_err_t nvs_write_uint8(const char* key, uint8_t value)
{
    esp_err_t ret = ESP_OK;
    nvs_handle_t my_handle;
    ret = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (ret != ESP_OK)
    {
        return ret;
    }
    ret = nvs_set_u8(my_handle, key, value);
    if (ret != ESP_OK)
    {
        return ret;
    }
    ret = nvs_commit(my_handle);
    if (ret != ESP_OK)
    {
        return ret;
    }
    ESP_LOGI("nvs_write_uint8", "Done");
    nvs_close(my_handle);
    return ret;
}
esp_err_t nvs_read(const char* key, uint8_t *value)
{
    esp_err_t ret = ESP_OK;
    nvs_handle_t my_handle;
    ret = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (ret != ESP_OK)
    {
        return ret;
    }
    ret = nvs_get_u8(my_handle, key, value);
    if (ret != ESP_OK)
    {
        return ret;
    }
    ESP_LOGI("nvs_read", "Done");
    nvs_close(my_handle);
    return ret;

}
esp_err_t nvs_erase(const char* key)
{
    esp_err_t ret = ESP_OK;
    nvs_handle_t my_handle;
    ret = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (ret != ESP_OK)
    {
        return ret;
    }
    ret = nvs_erase_key(my_handle, key);
    if (ret != ESP_OK)
    {
        return ret;
    }
    ret = nvs_commit(my_handle);
    if (ret != ESP_OK)
    {
        return ret;
    }
    ESP_LOGI("nvs_erase", "Done");
    nvs_close(my_handle);
    return ret;
}