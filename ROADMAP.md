# Roadmap

Plan de desarrollo del controlador multimedia inalámbrico Bluetooth basado en ESP32.
Refleja el estado actual del proyecto y las tareas pendientes por área de trabajo.

---

## Estado general

| Área | Estado |
|------|--------|
| Firmware | ✓ Completo |
| Hardware — esquemático | ✓ Completo |
| Hardware — PCB | ✓ Completo |
| Hardware — diseño 3D | ✓ Completo |
| Pruebas | En curso |
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
| Validación funcional con batería real | ✓ |

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
| Ruteo de señales | ✓ |
| Plano de GND | ✓ |
| Silkscreen con etiquetas y versión | ✓ |
| DRC sin errores | ✓ |
| Exportación de Gerbers | ✓ |
| Fabricación física | ✓ |

### Diseño 3D

| Tarea | Estado |
|-------|--------|
| Modelo de carcasa | ✓ |
| Verificación de medidas contra PCB | ✓ |
| Verificación de acceso a conectores USB y botones | ✓ |
| Exportación STEP | ✓ |
| Impresión 3D | Pendiente |

---

## Pruebas

| Prueba | Estado |
|--------|--------|
| Integración completa — 6 comandos end-to-end con Spotify | ✓ |
| Protocolo de pruebas formales definido | ✓ |
| Prueba 1 — Alcance BLE | Pendiente |
| Prueba 2 — Latencia | Pendiente |
| Prueba 3 — Autonomía de batería | Pendiente |
| Prueba 4 — Confiabilidad | Pendiente |
| Prueba 5 — Compatibilidad | Pendiente |

---

## Documentación

| Tarea | Estado |
|-------|--------|
| README con descripción, requisitos e instrucciones | ✓ |
| Asignación de pines con justificación técnica | ✓ |
| Decisiones de diseño PCB | ✓ |
| Memoria técnica del proyecto (entrega académica) | ✓ |
| Lista de materiales con especificaciones y costos | ✓ |
| Manual de usuario | ✓ |
| Documentación técnica del sistema | Pendiente |

---

## Hitos

| Hito | Descripción | Estado |
|------|-------------|--------|
| MVP firmware | 6 comandos funcionando end-to-end desde hardware hasta Ubuntu via BLE HID | ✓ Logrado |
| Esquemático completo | ERC sin errores, huellas y modelos 3D asignados | ✓ Logrado |
| PCB fabricada | DRC sin errores, Gerbers exportados, PCB física lista | ✓ Logrado |
| Sistema validado | Pruebas formales documentadas con batería real | Pendiente |
| Entrega final | Pruebas formales y documentación técnica completa | Pendiente |

---

*Proyecto académico — Sistemas Embebidos II · Ingeniería Mecatrónica · UCB*
