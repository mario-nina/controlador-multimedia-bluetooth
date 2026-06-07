# Asignación de pines — ESP32 WROOM-32 DevKit 30 pines

Documento de referencia para el proyecto **Controlador Multimedia Bluetooth**.
Registra los pines utilizados, su justificación técnica, restricciones del
microcontrolador, y criterios para selección de pines alternativos.

Última actualización: junio 2026

---

## Tabla de contenidos

- [Pines utilizados](#pines-utilizados)
- [Restricciones del ESP32 WROOM-32](#restricciones-del-esp32-wroom-32)
- [Pines disponibles para expansión](#pines-disponibles-para-expansión)
- [Criterios de selección](#criterios-de-selección)
- [Referencia de periféricos internos](#referencia-de-periféricos-internos)

---

## Pines utilizados

### Indicadores visuales

| GPIO | Pin DevKit | Función | Periférico | Dirección | Pull | Estado |
|------|------------|---------|------------|-----------|------|--------|
| 32 | D32 | LED azul — estado Bluetooth | GPIO | Salida | — | ✓ Validado |
| 33 | D33 | LED rojo — estado batería | GPIO | Salida | — | ✓ Validado |

**Notas:**
- Resistencia de 1kΩ en serie con cada LED hacia GND.
- GPIO32 y GPIO33 pertenecen a ADC1 — compatibles con BLE activo.
- Pines reasignados desde GPIO26/GPIO27 para facilitar ruteo del PCB.
- Ambos pines validados físicamente.

**Pines alternativos aceptables para LEDs:** GPIO4, GPIO5\*, GPIO13, GPIO14, GPIO15\*, GPIO16, GPIO17.

---

### Botones

| GPIO | Pin DevKit | Función | Periférico | Dirección | Pull | Estado |
|------|------------|---------|------------|-----------|------|--------|
| 22 | D22 | Botón Next | GPIO + ISR | Entrada | Pull-up interno | ✓ Validado |
| 23 | D23 | Botón Previous | GPIO + ISR | Entrada | Pull-up interno | ✓ Validado |
| 25 | D25 | Botón Mute/Unmute | GPIO + ISR | Entrada | Pull-up interno | ✓ Validado |

**Notas:**
- Configurados con `GPIO_INTR_NEGEDGE` — interrupción en flanco de bajada.
- Pull-up interno del ESP32 (~45kΩ) es suficiente para pulsadores táctiles. No se requieren resistencias externas.
- Debouncing implementado por software mediante timestamp con umbral de 150ms.
- Los tres pines validados físicamente sin rebotes.

**Pines alternativos aceptables para botones:** cualquier GPIO de propósito general con soporte de interrupción: GPIO4, GPIO13, GPIO14, GPIO16, GPIO17. Evitar GPIO34-39 para botones porque son solo entrada y algunos no tienen pull-up interno.

---

### Encoder rotativo EC11

| GPIO | Pin DevKit | Función | Periférico | Dirección | Pull | Estado |
|------|------------|---------|------------|-----------|------|--------|
| 18 | D18 | Encoder CLK (canal A) | PCNT canal A edge | Entrada | Pull-up manual | ✓ Validado |
| 19 | D19 | Encoder DT (canal B) | PCNT canal A level / canal B edge | Entrada | Pull-up manual | ✓ Validado |
| 21 | D21 | Encoder SW (pulsador) | GPIO + ISR | Entrada | Pull-up interno | ✓ Validado |

**Notas:**
- GPIO18 y GPIO19 son los pines recomendados para PCNT porque soportan todos los periféricos de alta velocidad.
- El periférico PCNT no configura pull-ups automáticamente — se aplican manualmente con `gpio_set_pull_mode()`.
- Filtro de glitches del PCNT configurado en 10µs (`max_glitch_ns = 10000`) para suprimir ruido mecánico del EC11.
- Watch points en ±4 — genera un evento por cada detente físico completo del EC11 (4 pulsos de cuadratura).
- Dirección CW → `EVT_VOL_SUBIR`, CCW → `EVT_VOL_BAJAR`. Validado físicamente.
- Debouncing de dirección en callback PCNT: cambios de dirección en menos de 50ms se descartan.

**Pines alternativos aceptables para PCNT:** GPIO18/GPIO19 son la primera opción. Alternativas: GPIO4/GPIO13. Evitar GPIO34-39 para CLK/DT porque son solo entrada y el PCNT necesita pines bidireccionales para algunos modos.

---

### Sistema de alimentación

| GPIO | Pin DevKit | Función | Periférico | Dirección | Pull | Estado |
|------|------------|---------|------------|-----------|------|--------|
| 34 | D34 | ADC batería (divisor resistivo) | ADC1 canal 6 | Entrada analógica | — | ✓ Validado |

**Notas:**
- GPIO34 es entrada exclusiva — no puede configurarse como salida.
- Usar obligatoriamente ADC1 (GPIO32-GPIO39) para lectura de batería. ADC2 es incompatible con BLE activo.
- Divisor resistivo: 100kΩ desde VBAT+ → nodo → 100kΩ → GND. El nodo central conecta a GPIO34.
- Atenuación: `ADC_ATTEN_DB_12` para rango 0–3.3V.

**Pines alternativos aceptables para ADC batería:** GPIO35, GPIO36, GPIO39 — todos pertenecen a ADC1 y son compatibles con BLE activo. Son solo entrada, lo cual es adecuado para ADC.

---

## Restricciones del ESP32 WROOM-32

### Pines reservados — nunca usar

| GPIO | Razón |
|------|-------|
| GPIO6 | Bus SPI flash interno (CLK) |
| GPIO7 | Bus SPI flash interno (SD0/MISO) |
| GPIO8 | Bus SPI flash interno (SD1/MOSI) |
| GPIO9 | Bus SPI flash interno (SD2) |
| GPIO10 | Bus SPI flash interno (SD3) |
| GPIO11 | Bus SPI flash interno (CMD) |

Usar cualquiera de estos pines causa comportamiento indefinido o fallo de arranque del chip.

---

### Pines de strapping — precaución en boot

| GPIO | Función de strapping | Estado seguro en operación |
|------|---------------------|---------------------------|
| GPIO0 | Boot mode (LOW = flash, HIGH = normal) | Libre tras arranque, evitar cargas que lo mantengan en LOW |
| GPIO2 | Debe estar en LOW o flotante durante flash | Libre tras arranque |
| GPIO5 | SDIO timing | Libre tras arranque, tiene pull-up interno |
| GPIO12 | VDD_SDIO voltage (HIGH = 1.8V, LOW = 3.3V) | Mantener en LOW o flotante — crítico para alimentación |
| GPIO15 | MTDO / silencia mensajes de boot en LOW | Libre tras arranque |

**Regla práctica:** estos pines pueden usarse en operación normal, pero no conectar resistencias de pull-down en GPIO12 ni elementos que fuercen GPIO0 a LOW permanentemente.

---

### ADC2 — incompatible con BLE/WiFi

Los siguientes pines pertenecen a ADC2 y **no pueden usarse para lectura analógica mientras BLE esté activo:**

GPIO0, GPIO2, GPIO4, GPIO12, GPIO13, GPIO14, GPIO15, GPIO25, GPIO26, GPIO27.

Para lectura analógica con BLE activo, usar exclusivamente ADC1: GPIO32-GPIO39.

---

### GPIO34-GPIO39 — solo entrada

| GPIO | Nota |
|------|------|
| GPIO34 | Solo entrada, sin pull-up/pull-down interno |
| GPIO35 | Solo entrada, sin pull-up/pull-down interno |
| GPIO36 (VP) | Solo entrada, sin pull-up/pull-down interno |
| GPIO39 (VN) | Solo entrada, sin pull-up/pull-down interno |

No pueden configurarse como salida. Adecuados para ADC1 y señales de entrada que no requieran pull-up interno.

---

## Pines disponibles para expansión

Pines no utilizados en el diseño actual, libres para funciones futuras:

| GPIO | Observaciones |
|------|---------------|
| GPIO4 | Propósito general, soporta ADC2 (no usar con BLE), touch |
| GPIO13 | Propósito general, soporta ADC2 (no usar con BLE), touch |
| GPIO14 | Propósito general, soporta ADC2 (no usar con BLE), touch |
| GPIO16 | Propósito general, sin restricciones relevantes |
| GPIO17 | Propósito general, sin restricciones relevantes |
| GPIO26 | Propósito general, ADC2 (no usar con BLE) |
| GPIO27 | Propósito general, ADC2 (no usar con BLE) |
| GPIO35 | Solo entrada, ADC1 canal 7 |
| GPIO36 (VP) | Solo entrada, ADC1 canal 0 |
| GPIO39 (VN) | Solo entrada, ADC1 canal 3 |

GPIO1 (TX0) y GPIO3 (RX0) están en uso por la UART de consola/monitor serie durante desarrollo. No usar para funciones de aplicación.

---

## Criterios de selección

Los pines del proyecto se seleccionaron siguiendo estos criterios en orden de prioridad:

1. **Compatibilidad con BLE activo** — evitar ADC2 para señales analógicas; verificar que los pines no interfieren con el stack Bluetooth.

2. **Periférico hardware dedicado** — preferir pines que permiten usar periféricos de hardware (PCNT para encoder, ADC1 para batería) en lugar de implementaciones por software, para mayor precisión y menor carga del CPU.

3. **Ausencia de funciones de strapping** — evitar pines que afectan el comportamiento de arranque del chip para simplificar el diseño del PCB.

4. **Facilidad de ruteo en PCB** — agrupar pines por subsistema para minimizar cruces de pistas y simplificar el layout.

5. **Validación física incremental** — cada pin se confirmó funcionando en hardware físico antes de comprometerse en el esquemático KiCad.

---

## Referencia de periféricos internos

| Periférico | Usado para | Pines asignados |
|------------|------------|-----------------|
| GPIO + ISR | Botones y pulsador encoder | GPIO21, GPIO22, GPIO23, GPIO25 |
| GPIO salida | LEDs indicadores | GPIO32, GPIO33 |
| PCNT unidad 0 | Decodificación cuadratura encoder | GPIO18 (CLK), GPIO19 (DT) |
| ADC1 canal 6 | Monitoreo voltaje batería | GPIO34 |
| UART0 | Monitor serie (desarrollo) | GPIO1 (TX), GPIO3 (RX) |

---

*Proyecto: Controlador Multimedia Bluetooth — Sistemas Embebidos II*
*Universidad Católica Boliviana — Ingeniería Mecatrónica*
