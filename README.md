# Sistema de Monitoreo DHT11 con ESP32-C3 SuperMini

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Este proyecto implementa un sistema de monitoreo de temperatura y humedad utilizando un sensor DHT11 conectado a una placa ESP32-C3 SuperMini. El sistema incluye un LED indicador que se activa cuando la temperatura supera un umbral predefinido.

## Características

- Lectura de temperatura y humedad del sensor DHT11
- Visualización de datos por puerto serie
- LED indicador que se activa al superar el umbral de temperatura
- Fácil configuración de pines GPIO
- Compatible con ESP-IDF

## Hardware Requerido

- Placa ESP32-C3 SuperMini
- Sensor DHT11
- LED (para alerta visual)
- Resistencias de 10KΩ (pull-up para DHT11) y 220Ω (para el LED)
- Protoboard y cables de conexión

## Diagrama de Conexiones

```
DHT11    ESP32-C3
------------------
VCC  ->  3.3V
DATA ->  GPIO3 (D3)
GND  ->  GND

LED   ->  GPIO1 (D1) -> Resistencia 220Ω -> GND
```

## Configuración

1. Clona este repositorio:
   ```bash
   git clone https://github.com/migbertweb/dht11-esp32c3.git
   cd dht11-esp32c3
   ```

2. Configura el entorno de desarrollo ESP-IDF (asegúrate de tenerlo instalado previamente).

3. Configura el proyecto:
   ```bash
   idf.py set-target esp32c3
   idf.py menuconfig
   ```
   (Opcional) Ajusta la configuración según sea necesario.

4. Compila y flashea el proyecto:
   ```bash
   idf.py build
   idf.py -p /dev/ttyUSB0 flash monitor
   ```
   Reemplaza `/dev/ttyUSB0` con el puerto serie correspondiente.

## Uso

1. Conecta el hardware según el diagrama proporcionado.
2. Enciende la placa ESP32-C3.
3. Abre el monitor serie para ver las lecturas de temperatura y humedad.
4. El LED se encenderá cuando la temperatura supere el umbral configurado (35°C por defecto).

## Estructura del Proyecto

```
├── CMakeLists.txt          # Configuración principal de CMake
├── main/
│   ├── CMakeLists.txt     # Configuración del componente principal
│   └── main.c             # Código fuente principal
├── components/            # Componentes personalizados
├── LICENSE               # Licencia MIT
└── README.md             # Este archivo
```

## Personalización

Puedes modificar los siguientes parámetros en `main.c`:

- `DHT_GPIO`: Pin GPIO para el sensor DHT11 (por defecto: GPIO3)
- `LED_GPIO`: Pin GPIO para el LED (por defecto: GPIO1)
- `TEMP_UMBRAL`: Umbral de temperatura para activar la alerta (por defecto: 35°C)

## Licencia

Este proyecto está bajo la licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.

## Autor

Migbertweb - [GitHub](https://github.com/migbertweb)

## Agradecimientos

- Basado en el ejemplo de DHT de Espressif
- Utiliza la biblioteca DHT de ESP-IDF

---

**Nota sobre uso educativo:** Se recomienda encarecidamente, aunque no es obligatorio, que las obras derivadas mantengan este mismo espíritu de código libre y abierto, especialmente cuando se utilicen con fines educativos o de investigación.
