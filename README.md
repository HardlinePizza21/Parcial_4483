# Parcial de Teoría de la Conmutación

En este repositorio se encuentra un proyecto creado con **PlatformIO** que usa el framework de **Arduino** y la placa **ESP32 DevKit V1**.  
Además, incluye:

- Un archivo de configuración (`mosquitto.conf`) para el broker **Mosquitto** (versión 2.0.21).
- Un archivo `flows.json` que representa el flujo diseñado en **Node-RED**.

---

## 🔌 Broker (Mosquitto)

El broker MQTT está instalado de forma local usando:

```bash
sudo apt install mosquitto
```

Por defecto se ejecuta en un puerto local, por lo que se debe indicar el archivo de configuración personalizado para que funcione en la red Wi-Fi.  
Usá el siguiente comando para iniciarlo:

```bash
mosquitto -c <ubicación-del-archivo>/mosquitto.conf
```

Luego, necesitás conocer la **IP local** del computador donde está corriendo el broker para que los clientes MQTT puedan conectarse.

---

## 🌐 Node-RED

Node-RED actúa como cliente MQTT. Una vez tengas la interfaz funcionando:

1. Importá el archivo `flows.json`.
2. Configurá los nodos MQTT con un servidor que apunte a la **IP del broker**.
3. El nodo que **publica** debe usar el **topic**: `led`.
4. El nodo que **se suscribe** debe escuchar el **topic**: `temperatura`.

---

## 📟 ESP32

El ESP32 también actúa como cliente MQTT. El montaje junto con el circuito es el siguiente:

![WhatsApp Image 2025-04-30 at 4 23 42 PM](https://github.com/user-attachments/assets/5212cbbe-2187-45d8-a7cc-0d9ff5e0b8eb)

### Funcionalidad:

- Lee la temperatura desde un sensor **LM35**.
- Publica esa temperatura al broker en el topic `temperatura`, para que Node-RED pueda visualizarla.
- Se suscribe al topic `led`, y si recibe:
  - `"encender"` → enciende un LED.
  - `"apagar"` → apaga el LED.

> **Importante:** Recordá definir la IP del broker tanto en Node-RED como en el código `main.cpp` del ESP32.  
> En el código, se define en la variable:
```cpp
const char* mqtt_server = "192.168.X.X";
```

---

