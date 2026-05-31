# Controlador Multimedia Bluetooth

![CI](https://github.com/mario-nina/controlador-multimedia-bluetooth/actions/workflows/ci.yml/badge.svg)
![Estado](https://img.shields.io/badge/estado-en%20desarrollo-yellow)
![Versión](https://img.shields.io/badge/versión-v0.1.0-blue)
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
- Operación autónoma mediante batería LiPo recargable
- Indicadores visuales de estado de conexión y batería
- Gestión energética con modos de bajo consumo

---

## Requisitos

### Hardware

| Componente | Especificación |
|------------|----------------|
| Microcontrolador | ESP32 WROOM-32 (DevKit 30 pines) |
| Batería | LiPo 3.7V 500mAh |
| Módulo de carga | TP4056 con protección |
| Regulador | AMS1117-3.3V |
| Interfaz de usuario | Encoder rotativo + 3 botones |
| Indicadores | LED azul (Bluetooth), LED rojo (batería) |

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
| IDE | Espressif IDE 4.2.0 |
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
4. Una vez conectado (LED azul fijo), usar los controles:

| Control | Función |
|---------|---------|
| Encoder — girar derecha | Subir volumen |
| Encoder — girar izquierda | Bajar volumen |
| Encoder — presionar | Play/Pause |
| Botón 1 | Siguiente pista |
| Botón 2 | Pista anterior |
| Botón 3 | Mute/Unmute |

---

## Estructura del proyecto

```
controlador-multimedia-bluetooth/
├── firmware/                     # Código fuente del firmware (ESP-IDF)
│   ├── main/                     # Punto de entrada de la aplicación
│   └── components/               # Módulos independientes
│       ├── comunicacion_bt/      # Stack NimBLE + perfil HID Consumer Control
│       ├── control_leds/         # Patrones de LEDs via esp_timer
│       ├── driver_entrada/       # Botones (ISR) + encoder rotativo (PCNT)
│       └── gestion_energia/      # Gestión de energía — pendiente Fase 10
├── hardware/                     # Archivos de diseño de hardware
│   ├── controlador_multimedia/   # Proyecto KiCad (esquemático + PCB)
│   ├── esquematico/              # Exportaciones del esquemático (PDF/SVG)
│   ├── pcb/                      # Exportaciones Gerber — pendiente Fase 12
│   ├── diseno_3d/                # Exportaciones STEP — pendiente Fase 12
│   └── pines.md                  # Asignación de pines con justificación técnica
├── docs/                         # Documentación técnica
│   ├── memoria_tecnica/          # Memoria técnica del proyecto
│   ├── manual_usuario/           # Manual de usuario — pendiente Fase 12
│   └── lista_materiales/         # Lista de materiales — pendiente Fase 12
├── test/                         # Pruebas y validación
│   └── resultados/               # Resultados de pruebas físicas por fase
├── CHANGELOG.md
├── CONVENTIONS.md
├── CONTRIBUTING.md
├── LICENSE
├── ROADMAP.md
└── README.md
```

---

## Hardware

El sistema se compone de cuatro subsistemas principales:

- **Interfaz de usuario** — encoder rotativo y tres botones para generación de comandos
- **Microcontrolador** — ESP32 WROOM-32 como unidad central de procesamiento
- **Sistema de alimentación** — batería LiPo con módulo TP4056 y regulador AMS1117-3.3V
- **Indicadores de estado** — LED azul para Bluetooth y LED rojo para batería

Los archivos de esquemático y PCB se encuentran en la carpeta `hardware/`.

---

## Firmware

Desarrollado en C mediante ESP-IDF v6.0.1 con arquitectura basada en FreeRTOS.

La arquitectura implementa separación de responsabilidades en tareas independientes:

- Gestión de entrada de usuario
- Procesamiento de comandos multimedia
- Comunicación Bluetooth HID
- Gestión de energía

---

## Documentación

La documentación técnica completa se encuentra en la carpeta `docs/`:

- `memoria_tecnica/` — memoria técnica del proyecto en formato LaTeX
- `manual_usuario/` — instrucciones de operación y emparejamiento
- `lista_materiales/` — componentes con especificaciones técnicas y costos

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
