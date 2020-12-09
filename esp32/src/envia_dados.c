#include "includes.h"
#include "defines.h"

static const char *TAG = "HTTP_CLIENT";

extern const char howsmyssl_com_root_cert_pem_start[] asm("_binary_howsmyssl_com_root_cert_pem_start");
extern const char howsmyssl_com_root_cert_pem_end[] asm("_binary_howsmyssl_com_root_cert_pem_end");

QueueHandle_t bufferTemperatura;
QueueHandle_t bufferUmidade;

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!esp_http_client_is_chunked_response(evt->client))
        {
            // Write out data
            // printf("%.*s", evt->data_len, (char*)evt->data);
        }

        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        int mbedtls_err = 0;
        esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
        if (err != 0)
        {
            ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
            ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
        }
        break;
    }
    return ESP_OK;
}

static void http_rest_with_url(void)
{
    esp_http_client_config_t config = {
        .url = "http://192.168.0.104:3001/",
        .event_handler = _http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    uint16_t temp;
    uint16_t umid;
    char stringTemperatura[10];
    char stringUmidade[10];
    char str_params[70];

    xQueueReceive(bufferTemperatura, &temp, pdMS_TO_TICKS(2000));
    xQueueReceive(bufferUmidade, &umid, pdMS_TO_TICKS(2000));

    if (temp != 0 && umid != 0)
    {
        sprintf(stringTemperatura, "%d", temp);
        sprintf(stringUmidade, "%d", umid);
        sprintf(str_params, "type=1&temperatura=%s&umidade=%s", stringTemperatura, stringUmidade);

        // POST
        const char *post_data = str_params;
        esp_http_client_set_url(client, "http://192.168.0.104:3000/");
        esp_http_client_set_method(client, HTTP_METHOD_POST);
        esp_http_client_set_header(client, "Content-Type", "application/x-www-form-urlencoded");
        esp_http_client_set_post_field(client, post_data, strlen(post_data));
        err = esp_http_client_perform(client);
        if (err == ESP_OK)
        {
            ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %d",
                     esp_http_client_get_status_code(client),
                     esp_http_client_get_content_length(client));
            ESP_LOGI(TAG, "Temperatura: %s / Umidade: %s", stringTemperatura, stringUmidade);
        }
        else
        {
            ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
        }
    }

    esp_http_client_cleanup(client);
}

void task_requests(void *pvParameters)
{
    while (1)
    {
        http_rest_with_url();
        vTaskDelay(30000 / portTICK_PERIOD_MS);
    }
}

void envia_dados(void)
{
    xTaskCreate(&task_requests, "task_requests", 8192, NULL, 5, NULL);
}
