#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "startup_tests.h"

static const char *TAG = "HELLO_WORLD";

void app_main(void)
{
  // Add delay to allow serial monitor to connect and catch early output
  vTaskDelay(2000 / portTICK_PERIOD_MS); // 2 second delay

  // // Initialize NVS (required for WiFi, but good practice to include)
  // esp_err_t ret = nvs_flash_init();
  // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  // {
  //   ESP_ERROR_CHECK(nvs_flash_erase());
  //   ret = nvs_flash_init();
  // }
  // ESP_ERROR_CHECK(ret);

  // Run startup diagnostic tests
  run_startup_tests();

  // Keep the app running
  while (1)
  {
    // Print "Hello World" to serial monitor repeatedly
    ESP_LOGI(TAG, "Hello World!");

    // You can also use printf (requires UART configuration)
    printf("Hello World from printf!\n");

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
