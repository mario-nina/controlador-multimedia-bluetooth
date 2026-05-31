# Roadmap del proyecto

Plan de desarrollo del controlador multimedia inalámbrico Bluetooth basado en ESP32.
Cada fase valida el hardware físicamente antes de comprometerse en el esquemático.

---

## Estado de fases

| Fase | Descripción | Estado |
|------|-------------|--------|
| 1 | Proyecto base + primer test físico (blink LED) | ✓ Completada |
| 2 | Estructura de 4 componentes ESP-IDF | ✓ Completada |
| 3 | Control de LEDs validado + indicadores en esquemático | ✓ Completada |
| 4 | Driver de botones validado + botones en esquemático | ✓ Completada |
| 5 | Driver de encoder validado + encoder en esquemático | ✓ Completada |
| 6 | Pines confirmados + tareas FreeRTOS + FSM principal | ✓ Completada |
| 7 | Stack NimBLE + advertising + callbacks de conexión | ✓ Completada |
| 8 | Perfil HID Consumer Control con reportes BLE | ✓ Completada |
| 9 | Integración completa — MVP validado en hardware | ✓ Completada |
| 10a | Cadena de potencia en KiCad + huellas + modelos 3D | ✓ Completada |
| 10b | Firmware de gestión de energía (ADC batería + light sleep) | En curso |
| 11 | Pruebas formales del sistema completo + esquemático final | Pendiente |
| 12 | PCB layout + Gerbers + documentación final | Pendiente |

---

## Hitos principales

| Hito | Fase | Descripción | Estado |
|------|------|-------------|--------|
| MVP | 9 | Los 6 comandos funcionan end-to-end desde hardware hasta Ubuntu via BLE HID | ✓ Logrado |
| Entrega final | 12 | PCB, Gerbers, memoria técnica y manual de usuario completos | Pendiente |

---

## Próximos pasos

**Fase 10b — Gestión de energía (firmware)**
- Lectura de voltaje de batería via ADC1 canal 6 (GPIO34)
- Tarea FreeRTOS de monitoreo periódico
- Indicación de batería baja mediante LED rojo
- Modo light sleep cuando BLE esté inactivo
- Activación del estado `SYS_SLEEPING` en la FSM

**Fase 11 — Pruebas formales**
- Suite de pruebas documentadas para todos los módulos
- Validación de esquemático final con ERC
- Verificación de huellas contra datasheet de componentes

**Fase 12 — PCB y documentación**
- Diseño de PCB layout en KiCad
- Generación de Gerbers
- Memoria técnica completa
- Manual de usuario

---

*Proyecto académico — Sistemas Embebidos II · Ingeniería Mecatrónica · UCB*
