#include "includes.h"
#include "defines.h"

u8g2_t u8g2;
QueueHandle_t bufferTemperatura;
QueueHandle_t bufferUmidade;

void task_oLED(void *pvParameters)
{
    // initialize the u8g2 hal
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_esp32_hal.sda = PIN_SDA;
    u8g2_esp32_hal.scl = PIN_SCL;
    u8g2_esp32_hal_init(u8g2_esp32_hal);

    // initialize the u8g2 library
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
        &u8g2,
        U8G2_R0,
        u8g2_esp32_i2c_byte_cb,
        u8g2_esp32_gpio_and_delay_cb);

    // set the display address
    u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);

    // initialize the display
    u8g2_InitDisplay(&u8g2);

    // wake up the display
    u8g2_SetPowerSave(&u8g2, 0);

    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawFrame(&u8g2, 0, 0, 128, 64);
    u8g2_SetFont(&u8g2, u8g2_font_6x10_mf);
    u8g2_DrawUTF8(&u8g2, 15, 15, "IoT Aplicada");
    u8g2_SendBuffer(&u8g2);
    uint16_t temp;
    uint16_t umid;
    char stringTemperatura[10];
    char stringUmidade[10];
    while (1)
    {
        xQueueReceive(bufferTemperatura, &temp, pdMS_TO_TICKS(2000));
        xQueueReceive(bufferUmidade, &umid, pdMS_TO_TICKS(2000));

        u8g2_DrawUTF8(&u8g2, 15, 30, "Temp.: ");
        sprintf(stringTemperatura, "%d", temp);
        u8g2_DrawUTF8(&u8g2, 55, 30, stringTemperatura);
        u8g2_DrawUTF8(&u8g2, 70, 30, "°C");

        u8g2_DrawUTF8(&u8g2, 15, 40, "Umidade: ");
        sprintf(stringUmidade, "%d", umid);
        u8g2_DrawUTF8(&u8g2, 65, 40, stringUmidade);
        u8g2_DrawUTF8(&u8g2, 80, 40, "%");

        u8g2_SendBuffer(&u8g2);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void inicia_oled()
{
    xTaskCreate(task_oLED, "task_oLED", 2048, NULL, 2, NULL);
}