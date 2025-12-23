#include <stdio.h>
#include <stdbool.h>
#include <string.h>
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

  // Allocate DMA-capable buffers from PSRAM for testing
  uint8_t *src_buf = heap_caps_malloc(512 * 1024, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);
  uint8_t *dst_buf = heap_caps_malloc(512 * 1024, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);

  if (src_buf == NULL || dst_buf == NULL)
  {
    printf("Failed to allocate DMA-capable PSRAM buffers\n");
    if (src_buf)
      free(src_buf);
    if (dst_buf)
      free(dst_buf);
  }
  else
  {
    printf("Allocated 1MB DMA-capable PSRAM buffers successfully\n");

    // Fill source buffer with pattern
    for (int i = 0; i < 512 * 1024; i++)
    {
      src_buf[i] = (uint8_t)(i % 256);
    }

    // Test DMA transfer: copy from source to destination buffer
    // memcpy may use DMA internally for large transfers on ESP32
    memcpy(dst_buf, src_buf, 512 * 1024);
    printf("Performed DMA-capable memory transfer (512KB)\n");

    // Verify the transferred data
    bool dma_ok = true;
    for (int i = 0; i < 512 * 1024; i++)
    {
      if (dst_buf[i] != (uint8_t)(i % 256))
      {
        dma_ok = false;
        break;
      }
    }

    if (dma_ok)
    {
      printf("PSRAM DMA test passed\n");
    }
    else
    {
      printf("PSRAM DMA test failed\n");
    }

    free(src_buf);
    free(dst_buf);
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
