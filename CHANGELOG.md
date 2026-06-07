# Registro de cambios

Todos los cambios notables de este proyecto se documentan en este archivo.
Formato basado en [Keep a Changelog](https://keepachangelog.com/es/1.0.0/).
Versionado basado en [SemVer](https://semver.org/lang/es/).

---

## [Sin publicar]

### Firmware
- Validación funcional con batería real pendiente

### Hardware
- PCB layout en curso — ruteo de señales pendiente
- DRC pendiente

### Documentación
- Documentación técnica del sistema pendiente
- Lista de materiales pendiente
- Manual de usuario pendiente

### Pruebas
- Protocolo de pruebas formales por definir

---

## [0.2.0] — 2026-06-07

### Firmware
- Gestión de energía: lectura ADC batería con promediado de 16 muestras y calibración line fitting
- Gestión de energía: modem sleep via `esp_pm` con escalado dinámico de frecuencia CPU (40–160MHz)
- Indicadores LED integrados con estado BLE y nivel de batería
- Refactorización de arquitectura: centralización de configuración en `config.h`
- Renombrado `EVT_SILENCIAR` → `EVT_MUTE_UNMUTE` para reflejar función toggle
- Eliminación de `input_task` — `command_task` lee directamente de la cola de entrada
- Constantes de comportamiento movidas a headers públicos según principio de responsabilidad
- Watchpoints del encoder ajustados de ±2 a ±4 — un evento por detente físico completo
- Debouncing de botones ajustado a 150ms

### Hardware
- Reasignación de LEDs: GPIO26/GPIO27 → GPIO32/GPIO33 para facilitar ruteo PCB
- Esquemático actualizado con nueva asignación de pines
- Diseño 3D de carcasa completado
- Exportación de Gerbers

### Documentación
- Reestructuración de carpetas `hardware/`: `kicad/` y `exportaciones/`
- Decisiones de diseño PCB documentadas en `docs/diseno_pcb.md`
- Roadmap reestructurado por áreas de trabajo
- `hardware/pines.md` actualizado con nueva asignación de LEDs y correcciones técnicas

---

## [0.1.0] — 2026-05-31

Primera versión funcional del proyecto. MVP validado en hardware físico.
Los 6 comandos multimedia operan end-to-end desde el hardware hasta Ubuntu via BLE HID.

### Firmware
- Integración completa validada: Play/Pause, Siguiente, Anterior, Volumen +/−, Mute/Unmute
- Perfil HID Consumer Control sobre BLE (NimBLE) con reportes GATT
- Device Information Service con PnP ID (Espressif)
- Stack NimBLE con advertising y reconexión automática ante desconexión
- Máquina de estados: `SYS_ADVERTISING`, `SYS_CONNECTED`
- Acceso thread-safe al estado del sistema mediante mutex FreeRTOS
- Driver de botones: ISR con debouncing por timestamp en GPIO22, GPIO23, GPIO25, GPIO21
- Driver de encoder EC11: periférico PCNT con filtro de glitches (10µs), watch points en ±4
- Control de LEDs con cuatro patrones via `esp_timer`: apagado, encendido, parpadeo rápido (200ms), parpadeo lento (1000ms)
- NVS Flash inicializado en `app_main` para persistencia del stack Bluetooth
- Abstracción HID encapsulada en el módulo `comunicacion_bt`

### Hardware
- Esquemático KiCad completo con cadena de potencia: batería → TP4056 → MT3608 → AMS1117-3.3V → ESP32
- ERC sin errores
- Huellas asignadas a todos los componentes
- Modelos 3D asignados a todos los componentes

### Documentación
- README con descripción, requisitos e instrucciones
- Asignación de pines con justificación técnica y restricciones del ESP32
- Resultados de prueba de integración completa — sesión continua de 30+ minutos sin desconexiones

---

*Formato basado en [Keep a Changelog](https://keepachangelog.com/es/1.0.0/)*
*Versionado basado en [SemVer](https://semver.org/lang/es/)*
