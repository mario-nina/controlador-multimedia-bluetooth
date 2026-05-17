# Asignación de pines — ESP32 DevKit 30 pines

Tabla de asignación de pines del proyecto. El estado indica si el pin fue
validado físicamente mediante pruebas de firmware.

## Estados

| Estado | Descripción |
|--------|-------------|
| Pendiente | Asignado pero no validado físicamente aún |
| Validado | Confirmado mediante prueba física de firmware |
| Reservado | Uso interno del chip — nunca usar |
| Precaución | Strapping pin — usar con cuidado |

---

## Pines utilizados

| Pin DevKit | GPIO | Función | Componente | Estado |
|------------|------|---------|------------|--------|
| D26 | GPIO26 | LED azul — estado Bluetooth | control_leds | Validado |
| D27 | GPIO27 | LED rojo — estado batería | control_leds | Validado |
| D22 | GPIO22 | Botón Next | driver_entrada | Validado |
| D23 | GPIO23 | Botón Previous | driver_entrada | Validado |
| D25 | GPIO25 | Botón Mute | driver_entrada | Validado |
| D18 | GPIO18 | Encoder CLK | driver_entrada | Pendiente |
| D19 | GPIO19 | Encoder DT | driver_entrada | Pendiente |
| D21 | GPIO21 | Encoder SW (Play/Pause) | driver_entrada | Pendiente |
| D34 | GPIO34 | Lectura ADC batería | gestion_energia | Pendiente |

## Pines de alimentación

| Pin DevKit | Función |
|------------|---------|
| 3V3 | Salida 3.3V — alimentación del circuito |
| GND | Tierra |
| VIN | Entrada 5V desde USB (no usado en diseño final) |

## Pines reservados — nunca usar

| GPIO | Razón |
|------|-------|
| GPIO6 | Flash interno SPI |
| GPIO7 | Flash interno SPI |
| GPIO8 | Flash interno SPI |
| GPIO9 | Flash interno SPI |
| GPIO10 | Flash interno SPI |
| GPIO11 | Flash interno SPI |

## Pines con precaución

| GPIO | Razón |
|------|-------|
| GPIO0 | Strapping pin — pull-up interno, afecta modo de boot |
| GPIO2 | Strapping pin — pull-down interno |
| GPIO5 | Strapping pin — pull-up interno |
| GPIO12 | Strapping pin — pull-down interno, afecta voltaje flash |
| GPIO15 | Strapping pin — pull-up interno |

---

*Última actualización: Fase 4 — driver de botones*
