# Decisiones de diseño PCB — Controlador Multimedia Bluetooth v1.0

Registro de decisiones técnicas tomadas durante el diseño del PCB con su
justificación. Sirve como referencia para entender el diseño actual y como
base para revisiones futuras.

---

## Anchos de pista

Base de cálculo: norma IPC-2221 con elevación de temperatura máxima de 10°C
y espesor de cobre de 35µm (1oz).

| Red | Corriente | Teórico IPC-2221 | Valor final |
|-----|-----------|------------------|-------------|
| Señales GPIO | 10mA | 0.02mm | 0.8mm |
| VCC 3.3V | 260mA | 0.08mm | 0.8mm |
| VBAT, BAT_P, OUT_P, BOOST_P, PWR_SW | 1A | 0.35mm | 1.0mm |
| VBAT_ADC | <1mA | 0.02mm | 0.8mm |
| GND | — | — | Plano completo |

`VBAT_ADC` se clasificó como señal (0.8mm) a pesar del nombre — la corriente
es de microamperios al ser solo lectura ADC.

---

## Clearance

Valor definido: **0.5mm** en todas las clases de red.

---

## Clases de red

| Clase | Ancho | Clearance | Redes |
|-------|-------|-----------|-------|
| Default | 0.8mm | 0.5mm | Señales GPIO, LEDs, encoder |
| Power | 1.0mm | 0.5mm | VBAT, BAT_P, OUT_P, BOOST_P, PWR_SW, VCC |

---

## Plano de GND

Se implementó plano de cobre GND en F.Cu en lugar de pistas individuales.

Configuración:
- Fill type: Solid fill
- Clearance: 0.5mm
- Minimum width: 0.5mm
- Pad connections: Thermal reliefs
- Remove islands: Always

El pad C del encoder SW4 usa Solid en lugar de Thermal reliefs por falta de
espacio físico para los spokes.

---

## Capacitores de desacople

**Decisión: no se agregan.**

Justificación:
- Los módulos TP4056, MT3608 y AMS1117 incluyen capacitores en su propia PCB
- El ESP32 WROOM-32 tiene capacitores internos en el módulo
- El circuito opera a baja frecuencia — botones, encoder, LEDs — sin cargas
  con cambios abruptos de corriente

Consideración para revisiones futuras: agregar capacitores de desacople en
la línea VCC si se presentan resets esporádicos.

---

## Diodo TVS

**Decisión: no se agrega en esta revisión.**

Justificación: el componente correcto para protección de la línea USB de 5V
no estaba disponible en proveedores locales al momento del diseño. El P6KE27A
disponible tiene voltaje de ruptura de 25.7V — incompatible con 5V.

Mejora pendiente para revisiones futuras.

---

## Keepout zone de antena ESP32

**Decisión: no se define keepout formal.**

Justificación: el ESP32 WROOM-32 se representó mediante headers J1/J2 en lugar
de footprint de módulo completo, lo que impide ubicar con precisión la zona de
antena en el layout.

Consideración de montaje: orientar el ESP32 con la antena hacia el borde de
la placa para minimizar impacto en alcance BLE.

---

## Distribución de componentes

Criterio principal: ergonomía de uso vertical con una mano.

| Zona | Componentes |
|------|-------------|
| Superior | Encoder y botones |
| Media | ESP32 con headers J1/J2 |
| Inferior | Cadena de potencia: TP4056 → MT3608 → AMS1117, conector batería |

---

## Teardrops

Activados con valores por defecto de KiCad en todos los pads THT y SMD para
reducir el riesgo de corte en la unión entre pista y pad durante el grabado.

---

*Proyecto: Controlador Multimedia Bluetooth*
*Universidad Católica Boliviana — Ingeniería Mecatrónica*
