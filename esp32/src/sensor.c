
#include "includes.h"
#include "defines.h"

static const dht_sensor_type_t sensor_type = DHT_TYPE_DHT11;
static const gpio_num_t dht_gpio = DHT_GPIO;

static const char *TAG = "Sensor";
QueueHandle_t bufferTemperatura;
QueueHandle_t bufferUmidade;
QueueHandle_t bufferDistancia;

int16_t temperatura;
int16_t umidade;
uint32_t distancia;


void task_dht(void *pvParamters)
{

    gpio_set_pull_mode(dht_gpio, GPIO_PULLUP_ONLY);
    while (1)
    {
        if (dht_read_data(sensor_type, dht_gpio, &umidade, &temperatura) == ESP_OK)
        {
            umidade = umidade / 10;         //Quem tem sensor
            temperatura = temperatura / 10; //Quem tem sensor
            xQueueSend(bufferTemperatura, &temperatura, pdMS_TO_TICKS(0));
            xQueueSend(bufferUmidade, &umidade, pdMS_TO_TICKS(0));
        }
        else
        {
            ESP_LOGE(TAG, "Não foi possível ler o sensor");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void task_ultrasonic(void *pvParamters)
{
    ultrasonic_sensor_t sensor = {
        .trigger_pin = TRIGGER_GPIO,
        .echo_pin = ECHO_GPIO
    };

    ultrasonic_init(&sensor);

    while (true)
    {
        esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distancia);
        if (res != ESP_OK)
        {
            /*switch (res)
            {
                case ESP_ERR_ULTRASONIC_PING:
                    printf("Cannot ping (device is in invalid state)\n");
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    printf("Ping timeout (no device found)\n");
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    printf("Echo timeout (i.e. distance too big)\n");
                    break;
                default:
                    printf("%d\n", res);
            }*/
        }
        else
            xQueueSend(bufferDistancia, &distancia, pdMS_TO_TICKS(0));

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


void inicia_sensor()
{
    bufferTemperatura = xQueueCreate(5, sizeof(uint16_t));
    bufferUmidade = xQueueCreate(5, sizeof(uint16_t));
    bufferDistancia = xQueueCreate(5, sizeof(uint32_t));
    xTaskCreate(task_dht, "task_dht", 2048, NULL, 1, NULL);
    xTaskCreate(task_ultrasonic, "task_ultrasonic", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}