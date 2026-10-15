#include <stdbool.h>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#define ETAPA 1

#define PIN_SENSOR GPIO_NUM_23
#define PIN_LED GPIO_NUM_18
#define INTERVALO_POLLING_MS 500
#define DEBOUNCE_US 50000

static const char *TAG = "AULA10";

typedef struct {
    int level;
    int64_t instante_us;
} evento_gpio_t;

static volatile uint32_t total_interrupcoes;
static QueueHandle_t fila_eventos;

static void configurar_gpio(void)
{
    const gpio_config_t entrada = {
        .pin_bit_mask = 1ULL << PIN_SENSOR,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&entrada));

    const gpio_config_t saida = {
        .pin_bit_mask = 1ULL << PIN_LED,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&saida));
    ESP_ERROR_CHECK(gpio_set_level(PIN_LED, 0));
}

static void tarefa_led(void *arg)
{
    int level = 0;

    while (true) {
        level = !level;
        gpio_set_level(PIN_LED, level);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void IRAM_ATTR isr_contador(void *arg)
{
    total_interrupcoes++;
}

static void IRAM_ATTR isr_fila(void *arg)
{
    const evento_gpio_t evento = {
        .level = gpio_get_level(PIN_SENSOR),
        .instante_us = esp_timer_get_time(),
    };

    BaseType_t tarefa_acordada = pdFALSE;
    xQueueSendFromISR(fila_eventos, &evento, &tarefa_acordada);

    if (tarefa_acordada == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

static void tarefa_contagem(void *arg)
{
    evento_gpio_t evento;
    int64_t ultimo_evento_us = -DEBOUNCE_US;
    uint32_t contador = 0;

    while (true) {
        if (xQueueReceive(fila_eventos, &evento, portMAX_DELAY) == pdTRUE) {
            if ((evento.instante_us - ultimo_evento_us) < DEBOUNCE_US) {
                continue;
            }

            ultimo_evento_us = evento.instante_us;
            contador++;
            ESP_LOGI(TAG, "Evento válido %" PRIu32 " em %" PRId64 " us",
                     contador, evento.instante_us);
        }
    }
}

static void tarefa_duracao(void *arg)
{
    evento_gpio_t evento;
    int64_t inicio_us = -1;

    while (true) {
        if (xQueueReceive(fila_eventos, &evento, portMAX_DELAY) == pdTRUE) {
            if (evento.level == 0) {
                inicio_us = evento.instante_us;
            } else if (inicio_us >= 0) {
                const int64_t duracao_us = evento.instante_us - inicio_us;
                ESP_LOGI(TAG, "Sensor ativo por %" PRId64 " us (%" PRId64 " ms)",
                         duracao_us, duracao_us / 1000);
                inicio_us = -1;
            }
        }
    }
}

void app_main(void)
{
    configurar_gpio();

#if ETAPA == 1
    ESP_LOGI(TAG, "ETAPA 1: polling a cada %d ms", INTERVALO_POLLING_MS);
    int led = 0;
    uint32_t contador = 0;

    while (true) {
        led = !led;
        gpio_set_level(PIN_LED, led);

        if (gpio_get_level(PIN_SENSOR) == 0) {
            contador++;
            ESP_LOGI(TAG, "Polling detectou %" PRIu32, contador);
        }

        vTaskDelay(pdMS_TO_TICKS(INTERVALO_POLLING_MS));
    }

#elif ETAPA == 2
    ESP_LOGI(TAG, "ETAPA 2: interrupção e contador");
    ESP_ERROR_CHECK(gpio_set_intr_type(PIN_SENSOR, GPIO_INTR_NEGEDGE));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_SENSOR, isr_contador, NULL));

    int led = 0;
    uint32_t ultimo_total = 0;

    while (true) {
        led = !led;
        gpio_set_level(PIN_LED, led);

        if (total_interrupcoes != ultimo_total) {
            ultimo_total = total_interrupcoes;
            ESP_LOGI(TAG, "Interrupções detectadas: %" PRIu32, ultimo_total);
        }

        vTaskDelay(pdMS_TO_TICKS(INTERVALO_POLLING_MS));
    }

#elif ETAPA == 3
    ESP_LOGI(TAG, "ETAPA 3: interrupção, fila e debounce");
    fila_eventos = xQueueCreate(20, sizeof(evento_gpio_t));
    assert(fila_eventos != NULL);

    ESP_ERROR_CHECK(gpio_set_intr_type(PIN_SENSOR, GPIO_INTR_NEGEDGE));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_SENSOR, isr_fila, NULL));

    xTaskCreate(tarefa_led, "led", 2048, NULL, 1, NULL);
    xTaskCreate(tarefa_contagem, "eventos", 3072, NULL, 2, NULL);

#elif ETAPA == 4
    ESP_LOGI(TAG, "ETAPA 4: medição entre as bordas");
    fila_eventos = xQueueCreate(20, sizeof(evento_gpio_t));
    assert(fila_eventos != NULL);

    ESP_ERROR_CHECK(gpio_set_intr_type(PIN_SENSOR, GPIO_INTR_ANYEDGE));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_SENSOR, isr_fila, NULL));

    xTaskCreate(tarefa_led, "led", 2048, NULL, 1, NULL);
    xTaskCreate(tarefa_duracao, "duracao", 3072, NULL, 2, NULL);

#else
#error "ETAPA deve estar entre 1 e 4"
#endif
}

