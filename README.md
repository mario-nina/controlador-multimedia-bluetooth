# Controlador Multimedia Bluetooth

![CI](https://github.com/mario-nina/controlador-multimedia-bluetooth/actions/workflows/ci.yml/badge.svg)
![Estado](https://img.shields.io/badge/estado-en%20desarrollo-yellow)
![Versión](https://img.shields.io/badge/versión-v0.2.0-blue)
![Licencia](https://img.shields.io/badge/licencia-MIT-green)

Controlador multimedia inalámbrico portátil basado en ESP32 que permite gestionar la reproducción de audio en una computadora mediante comunicación Bluetooth.

---

## Tabla de contenidos

- [Descripción](#descripción)
- [Características](#características)
- [Requisitos](#requisitos)
- [Instalación](#instalación)
- [Uso](#uso)
- [Estructura del proyecto](#estructura-del-proyecto)
- [Hardware](#hardware)
- [Firmware](#firmware)
- [Documentación](#documentación)
- [Versionado](#versionado)
- [Licencia](#licencia)
- [Autor](#autor)

---

## Descripción

El sistema permite controlar remotamente la reproducción multimedia de una laptop sin requerir interacción física directa con la misma. Comunica mediante protocolo Bluetooth HID, lo que garantiza compatibilidad nativa con el sistema operativo sin necesidad de software adicional.

Desarrollado como proyecto de la materia Sistemas Embebidos II de la carrera de Ingeniería Mecatrónica en la Universidad Católica Boliviana.

---

## Características

- Control de reproducción: Play/Pause
- Navegación entre pistas: Siguiente y Anterior
- Control de volumen: Incremento y Decremento
- Silenciamiento: Mute/Unmute
- Comunicación inalámbrica mediante Bluetooth HID
- Operación autónoma mediante batería Li-ion recargable
- Indicadores visuales de estado de conexión y batería
- Gestión energética con modem sleep y escalado dinámico de frecuencia CPU

---

## Requisitos

### Hardware

| Componente | Especificación |
|------------|----------------|
| Microcontrolador | ESP32 WROOM-32 (DevKit 30 pines) |
| Batería | Li-ion 18650 3.7V 2600mAh |
| Módulo de carga | TP4056 USB-C con protección |
| Boost converter | MT3608 |
| Regulador | AMS1117-3.3V |
| Encoder rotativo | Alps EC11 |
| Botones | 3 pulsadores táctiles |
| Indicadores | LED azul (Bluetooth — GPIO32), LED rojo (batería — GPIO33) |

### Software

| Herramienta | Versión |
|-------------|---------|
| ESP-IDF | v6.0.1 |
| KiCad | 8.0 |
| Sistema operativo | Ubuntu 24.04 LTS |
| Compilador | xtensa-esp-elf-gcc 15.2.0 |
| Python | 3.12.3 |
| CMake | 3.28.3 |
| Git | 2.43.0 |

---

## Instalación

### Configuración del entorno

1. Instalar ESP-IDF v6.0.1 siguiendo la [documentación oficial](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html).

2. Clonar el repositorio:
```bash
git clone git@github.com:mario-nina/controlador-multimedia-bluetooth.git
cd controlador-multimedia-bluetooth
```

3. Activar el entorno ESP-IDF:
```bash
get_idf
```

4. Configurar el chip objetivo:
```bash
cd firmware
idf.py set-target esp32
```

5. Compilar el firmware:
```bash
idf.py build
```

6. Flashear el ESP32:
```bash
idf.py flash monitor
```

---

## Uso

1. Encender el dispositivo.
2. En la laptop, activar Bluetooth y buscar dispositivos disponibles.
3. Emparejar con el dispositivo **"Controlador Multimedia"**.
4. Una vez conectado, usar los controles:

| Control | Función |
|---------|---------|
| Encoder — girar derecha | Subir volumen |
| Encoder — girar izquierda | Bajar volumen |
| Encoder — presionar | Play/Pause |
| Botón 1 | Siguiente pista |
| Botón 2 | Pista anterior |
| Botón 3 | Mute/Unmute |

### Indicadores visuales

| LED | Estado | Significado |
|-----|--------|-------------|
| Azul | Parpadeo rápido | Sin conexión BLE — advertising |
| Azul | Apagado | Conexión BLE activa |
| Rojo | Apagado | Batería OK (>= 3400mV) |
| Rojo | Parpadeo lento | Batería baja (< 3400mV) |
| Rojo | Parpadeo rápido | Batería crítica (< 3200mV) |

---

## Estructura del proyecto
controlador-multimedia-bluetooth/
├── firmware/                          # Código fuente del firmware (ESP-IDF)
│   ├── main/                          # Punto de entrada de la aplicación
│   │   └── firmware.c
│   └── components/                    # Módulos independientes
│       ├── common/                    # Configuración global del sistema
│       │   └── include/
│       │       ├── config.h           # Constantes de configuración del sistema
│       │       └── pines.h            # Mapa de asignación de pines GPIO
│       ├── comunicacion_bt/           # Stack NimBLE + perfil HID Consumer Control
│       ├── control_leds/              # Patrones de LEDs via esp_timer
│       ├── driver_entrada/            # Botones (ISR) + encoder rotativo (PCNT)
│       └── gestion_energia/           # Monitoreo ADC batería + modem sleep
├── hardware/                          # Archivos de diseño de hardware
│   ├── kicad/                         # Proyecto KiCad (esquemático + PCB)
│   ├── exportaciones/                 # Archivos exportados desde KiCad
│   │   ├── esquematico/               # PDF y SVG del esquemático
│   │   ├── gerbers/                   # Archivos Gerber para fabricación
│   │   └── step/                      # Modelos 3D de la carcasa
│   └── pines.md                       # Asignación de pines con justificación técnica
├── docs/                              # Documentación técnica
│   ├── memoria_tecnica/               # Memoria técnica del proyecto (entrega académica)
│   ├── diseno_pcb.md                  # Decisiones de diseño PCB con justificación
│   ├── manual_usuario/                # Manual de usuario
│   └── lista_materiales/             # Lista de materiales con especificaciones y costos
├── test/                              # Pruebas y validación
│   └── resultados/                    # Resultados de pruebas documentadas
├── CHANGELOG.md
├── CONTRIBUTING.md
├── CONVENTIONS.md
├── LICENSE
├── ROADMAP.md
└── README.md

---

## Hardware

El sistema se compone de cuatro subsistemas principales:

- **Interfaz de usuario** — encoder rotativo Alps EC11 y tres botones táctiles para generación de comandos
- **Microcontrolador** — ESP32 WROOM-32 como unidad central de procesamiento
- **Sistema de alimentación** — batería Li-ion 18650 → TP4056 (carga USB-C) → MT3608 (boost) → AMS1117-3.3V
- **Indicadores de estado** — LED azul (GPIO32) para Bluetooth y LED rojo (GPIO33) para batería

Los archivos de diseño se encuentran en `hardware/kicad/` y las exportaciones en `hardware/exportaciones/`.

---

## Firmware

Desarrollado en C mediante ESP-IDF v6.0.1 con arquitectura basada en FreeRTOS.

Separación de responsabilidades en componentes independientes:

| Componente | Responsabilidad |
|------------|-----------------|
| `common` | Configuración global — `config.h` y `pines.h` |
| `driver_entrada` | Lectura de botones (ISR + debounce) y encoder (PCNT) |
| `comunicacion_bt` | Stack NimBLE, advertising, perfil HID Consumer Control |
| `control_leds` | Patrones de iluminación via esp_timer |
| `gestion_energia` | Monitoreo ADC batería y modem sleep via esp_pm |

---

## Documentación

| Documento | Ubicación | Descripción |
|-----------|-----------|-------------|
| Asignación de pines | `hardware/pines.md` | Pines GPIO con justificación técnica y restricciones del ESP32 |
| Decisiones de diseño PCB | `docs/diseno_pcb.md` | Criterios técnicos aplicados en el diseño del PCB |
| Memoria técnica | `docs/memoria_tecnica/` | Documento académico de descripción del proyecto |
| Historial de cambios | `CHANGELOG.md` | Registro de cambios por versión |
| Roadmap | `ROADMAP.md` | Estado actual y tareas pendientes por área |

---

## Versionado

Este proyecto usa [SemVer](https://semver.org/lang/es/) para el versionado.
El historial de cambios se documenta en [CHANGELOG.md](CHANGELOG.md).

---

## Licencia

Este proyecto está bajo la licencia MIT. Ver [LICENSE](LICENSE) para más detalles.

---

## Autor

**Mario Nina**
Ingeniería Mecatrónica — Universidad Católica Boliviana
GitHub: [@mario-nina](https://github.com/mario-nina)
