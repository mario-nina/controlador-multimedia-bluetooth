# Registro de cambios

Todos los cambios notables de este proyecto se documentan en este archivo.
Formato basado en [Keep a Changelog](https://keepachangelog.com/es/1.0.0/).
Versionado basado en [SemVer](https://semver.org/lang/es/).

---

## [Sin publicar]

### En desarrollo
- Firmware de gestión de energía: lectura ADC batería y modos de bajo consumo (Fase 10b)

### Pendiente
- Pruebas formales del sistema completo (Fase 11)
- Diseño PCB y generación de Gerbers (Fase 12)
- Memoria técnica completa (Fase 12)
- Manual de usuario (Fase 12)

---

## [0.1.0] — 2026-05-31

Primera versión funcional del proyecto. MVP validado en hardware físico.
Los 6 comandos multimedia operan end-to-end desde el hardware hasta Ubuntu via BLE HID.

### Firmware

- Integración completa validada: Play/Pause, Siguiente, Anterior, Volumen +/−, Mute
- Perfil HID Consumer Control sobre BLE (NimBLE) con reportes GATT
- Device Information Service con PnP ID (Espressif)
- Stack NimBLE con advertising, conexión y reconexión automática ante desconexión
- Máquina de estados del sistema con tres estados: `SYS_ADVERTISING`, `SYS_CONNECTED`, `SYS_SLEEPING`
- Acceso thread-safe al estado del sistema mediante mutex FreeRTOS
- Driver de entrada separado en dos módulos independientes:
  - Botones: ISR con debouncing por timestamp (50 ms) en GPIO22, GPIO23, GPIO25, GPIO21
  - Encoder EC11: periférico PCNT con filtro de glitches (10 µs), watch points en ±2
- Control de LEDs con cuatro patrones via `esp_timer`: apagado, encendido, parpadeo rápido (200 ms), parpadeo lento (1000 ms)
- Comunicación entre tareas mediante colas FreeRTOS (`cola_eventos`, `cola_comandos`)
- NVS Flash inicializado en `app_main` para persistencia del stack Bluetooth
- Abstracción HID encapsulada en el módulo `comunicacion_bt` — la capa de aplicación no accede directamente al stack GATT

### Hardware

- Esquemático KiCad completo con cadena de potencia: batería LiPo → TP4056 → AMS1117-3.3V → ESP32
- ERC sin errores
- Huellas asignadas a todos los componentes
- Modelos 3D asignados a todos los componentes

### Documentación

- README con descripción del proyecto, requisitos de hardware y software, instrucciones de instalación y uso
- Asignación de pines con justificación técnica, restricciones del ESP32 y criterios de selección
- Resultados de pruebas de integración — Fase 9 (sesión continua de 30+ minutos sin desconexiones)

---

## Historial de fases

| Versión | Fase completada |
|---------|-----------------|
| 0.1.0 | Fases 1–9 + Fase 10a (hardware) |
