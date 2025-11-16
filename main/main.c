/**
 * Archivo: main.c
 * Descripción: Implementación principal del sistema de monitoreo de temperatura y humedad
 *              utilizando el sensor DHT11 en ESP32-C3 SuperMini con alerta por LED.
 * Autor: migbertweb
 * Fecha: 16/11/2024
 * Repositorio: https://github.com/migbertweb/dht11-esp32c3
 * Licencia: MIT License
 *
 * Uso: Este archivo contiene la lógica principal para leer datos del sensor DHT11,
 *      controlar un LED de alerta basado en umbrales de temperatura, y mostrar
 *      las lecturas a través del puerto serie.
 *
 * Nota: Este proyecto usa Licencia MIT. Se recomienda (no obliga) mantener 
 * derivados como código libre, especialmente para fines educativos.
 */

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <dht.h>
#include <esp_log.h>
#include <driver/gpio.h>

// Configuración de pines para ESP32-C3 SuperMini
#define DHT_GPIO 3      // Pin del sensor DHT11
#define LED_GPIO 1      // Pin del LED de alerta
#define TEMP_UMBRAL 35  // Umbral de temperatura para encender LED

static const char *TAG = "DHT11_ALERTA";

// Declaración de funciones
void configurar_led(void);
void controlar_led_por_temperatura(float temperatura);

void configurar_led(void)
{
    // Configurar el pin del LED como salida
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0); // Apagar LED inicialmente
    ESP_LOGI(TAG, "LED configurado en GPIO %d", LED_GPIO);
}

void controlar_led_por_temperatura(float temperatura)
{
    static bool led_encendido = false;
    
    if (temperatura > TEMP_UMBRAL && !led_encendido) {
        gpio_set_level(LED_GPIO, 1); // Encender LED
        led_encendido = true;
        ESP_LOGW(TAG, "¡ALERTA! Temperatura %.1f°C > %d°C - LED ENCENDIDO", 
                temperatura, TEMP_UMBRAL);
    } 
    else if (temperatura <= TEMP_UMBRAL && led_encendido) {
        gpio_set_level(LED_GPIO, 0); // Apagar LED
        led_encendido = false;
        ESP_LOGI(TAG, "Temperatura normal %.1f°C ≤ %d°C - LED APAGADO", 
                temperatura, TEMP_UMBRAL);
    }
}

void dht11_task(void *pvParameters)
{
    // Configurar hardware
    configurar_led();
    
    ESP_LOGI(TAG, "Iniciando monitor DHT11 en GPIO %d", DHT_GPIO);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    
    while (1) {
        int16_t temperature, humidity;
        esp_err_t result = dht_read_data(DHT_TYPE_DHT11, (gpio_num_t)DHT_GPIO, &humidity, &temperature);
        
        if (result == ESP_OK) {
            float temp_c = temperature / 10.0;
            float hum_p = humidity / 10.0;
            
            // Mostrar lectura
            ESP_LOGI(TAG, "Temperatura: %.1f°C, Humedad: %.1f%%", temp_c, hum_p);
            printf("Datos: Temp=%.1f°C, Hum=%.1f%%, Umbral=%d°C\n", 
                   temp_c, hum_p, TEMP_UMBRAL);
            
            // Controlar LED según temperatura
            controlar_led_por_temperatura(temp_c);
            
        } else {
            ESP_LOGE(TAG, "Error lectura: %s", esp_err_to_name(result));
            
            // Parpadear LED rápido en caso de error
            for (int i = 0; i < 3; i++) {
                gpio_set_level(LED_GPIO, 1);
                vTaskDelay(100 / portTICK_PERIOD_MS);
                gpio_set_level(LED_GPIO, 0);
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
        }
        
        vTaskDelay(2000 / portTICK_PERIOD_MS); // Lectura cada 2 segundos
    }
}

void app_main(void)
{
    printf("=== MONITOR DHT11 CON ALERTA LED ===\n");
    printf("Placa: ESP32-C3 SuperMini\n");
    printf("Sensor DHT11 en GPIO: %d\n", DHT_GPIO);
    printf("LED alerta en GPIO: %d\n", LED_GPIO);
    printf("Umbral temperatura: %d°C\n", TEMP_UMBRAL);
    printf("------------------------------------\n");
    
    // Crear tarea principal
    xTaskCreate(dht11_task, "dht11_task", 4096, NULL, 5, NULL);
    
    ESP_LOGI(TAG, "Sistema iniciado - Esperando lecturas...");
}