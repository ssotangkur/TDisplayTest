#include <stdio.h>
#include <stdbool.h>
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"

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

  // PSRAM test
  printf("Total heap: %lu bytes\n", esp_get_free_heap_size());
  printf("PSRAM heap: %u bytes\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

  // Allocate 1MB from PSRAM
  uint8_t *psram_buf = heap_caps_malloc(1024 * 1024, MALLOC_CAP_SPIRAM);
  if (psram_buf == NULL)
  {
    printf("Failed to allocate from PSRAM\n");
  }
  else
  {
    printf("Allocated 1MB from PSRAM successfully\n");
    // Fill with pattern
    for (int i = 0; i < 1024 * 1024; i++)
    {
      psram_buf[i] = (uint8_t)(i % 256);
    }
    // Verify
    bool ok = true;
    for (int i = 0; i < 1024 * 1024; i++)
    {
      if (psram_buf[i] != (uint8_t)(i % 256))
      {
        ok = false;
        break;
      }
    }
    if (ok)
    {
      printf("PSRAM test passed\n");
    }
    else
    {
      printf("PSRAM test failed\n");
    }
    free(psram_buf);
  }

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
