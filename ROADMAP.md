# Roadmap

Plan de desarrollo del controlador multimedia inalámbrico Bluetooth basado en ESP32.
Refleja el estado actual del proyecto y las tareas pendientes por área de trabajo.

---

## Estado general

| Área | Estado |
|------|--------|
| Firmware | ✓ Completo |
| Hardware — esquemático | ✓ Completo |
| Hardware — PCB | En curso |
| Hardware — diseño 3D | En curso |
| Pruebas | Pendiente |
| Documentación | En curso |

---

## Firmware

| Tarea | Estado |
|-------|--------|
| Proyecto base ESP-IDF y estructura de componentes | ✓ |
| Driver de botones con debouncing por timestamp | ✓ |
| Driver de encoder rotativo EC11 mediante PCNT | ✓ |
| Control de LEDs con patrones via esp_timer | ✓ |
| Stack NimBLE con advertising y callbacks de conexión | ✓ |
| Perfil HID Consumer Control — 6 comandos multimedia | ✓ |
| Máquina de estados del sistema con mutex FreeRTOS | ✓ |
| Gestión de energía — ADC batería con promediado y calibración | ✓ |
| Gestión de energía — modem sleep via esp_pm | ✓ |
| Arquitectura de componentes refactorizada — config.h, buenas prácticas | ✓ |
| Validación funcional con fuente de voltaje | ✓ |
| Validación funcional con batería real | Pendiente |

---

## Hardware

### Esquemático

| Tarea | Estado |
|-------|--------|
| Cadena de potencia — batería → TP4056 → MT3608 → AMS1117-3.3V | ✓ |
| Interfaz de usuario — encoder EC11 y botones | ✓ |
| Indicadores visuales — LEDs con resistencias | ✓ |
| Divisor resistivo para monitoreo de batería | ✓ |
| ERC sin errores | ✓ |
| Huellas asignadas a todos los componentes | ✓ |
| Modelos 3D asignados a todos los componentes | ✓ |
| Exportación PDF del esquemático | ✓ |

### PCB

| Tarea | Estado |
|-------|--------|
| Importar netlist desde esquemático | ✓ |
| Placement de componentes | ✓ |
| Ruteo de señales | En curso |
| Plano de GND | ✓ |
| Keepout de antena ESP32 WROOM-32 | — |
| Silkscreen con etiquetas y versión | ✓ |
| DRC sin errores | Pendiente |
| Exportación de Gerbers | ✓ |

### Diseño 3D

| Tarea | Estado |
|-------|--------|
| Modelo inicial de carcasa | En curso |
| Verificación de medidas contra PCB | Pendiente |
| Verificación de acceso a conectores USB y botones | Pendiente |
| Exportación STEP | Pendiente |

---

## Pruebas

> Las pruebas formales están por definirse. Esta sección se completará
> cuando se establezca el protocolo de pruebas definitivo.

| Prueba | Estado |
|--------|--------|
| Integración completa — 6 comandos end-to-end con Spotify | ✓ |
| Pruebas formales del sistema | Pendiente |

---

## Documentación

| Tarea | Estado |
|-------|--------|
| README con descripción, requisitos e instrucciones | ✓ |
| Asignación de pines con justificación técnica | ✓ |
| Decisiones de diseño PCB | ✓ |
| Memoria técnica del proyecto (entrega académica) | ✓ |
| Documentación técnica del sistema | Pendiente |
| Lista de materiales con especificaciones y costos | Pendiente |
| Manual de usuario | Pendiente |

---

## Hitos

| Hito | Descripción | Estado |
|------|-------------|--------|
| MVP firmware | 6 comandos funcionando end-to-end desde hardware hasta Ubuntu via BLE HID | ✓ Logrado |
| Esquemático completo | ERC sin errores, huellas y modelos 3D asignados | ✓ Logrado |
| PCB fabricable | DRC sin errores, Gerbers exportados | En curso |
| Sistema validado | Pruebas formales documentadas con batería real | Pendiente |
| Entrega final | PCB, documentación técnica y manual de usuario completos | Pendiente |

---

*Proyecto académico — Sistemas Embebidos II · Ingeniería Mecatrónica · UCB*
