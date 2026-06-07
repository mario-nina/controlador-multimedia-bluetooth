# Manual de usuario — Controlador Multimedia Bluetooth v1.0

Guía de operación del controlador multimedia inalámbrico basado en ESP32.

---

## Descripción del dispositivo

El controlador multimedia permite gestionar la reproducción de audio en una
computadora mediante comunicación Bluetooth, sin requerir interacción física
directa con la misma.

---

## Disposición de controles

De izquierda a derecha:

| Posición | Elemento | Descripción |
|----------|----------|-------------|
| 1 | ENC | Encoder rotativo — control de volumen y Play/Pause |
| 2 | PREV | Botón pista anterior |
| 3 | NEXT | Botón siguiente pista |
| 4 | MUTE | Botón Mute/Unmute |
| 5 | 🔵 🔴 | LEDs de estado — Bluetooth y batería |
| 6 | SW | Interruptor de encendido/apagado |
| — | USB | Puerto de carga USB-C — parte inferior derecha |
---

## Encendido y apagado

1. Deslizar el interruptor **SW** hacia la posición de encendido.
2. El LED azul comenzará a parpadear rápidamente — el dispositivo está
   buscando conexión Bluetooth.
3. Para apagar, deslizar el interruptor **SW** hacia la posición de apagado.

---

## Emparejamiento con la computadora

El emparejamiento solo es necesario la primera vez o al cambiar de computadora.

1. Encender el dispositivo — el LED azul parpadeará rápidamente.
2. En la computadora, abrir **Configuración → Bluetooth**.
3. Activar Bluetooth si no está activo.
4. Buscar dispositivos disponibles.
5. Seleccionar **"Controlador Multimedia"** de la lista.
6. Confirmar el emparejamiento cuando se solicite.
7. El LED azul se apagará — la conexión está establecida.

> Para reconectar en sesiones posteriores, encender el dispositivo y activar
> el Bluetooth de la computadora. La reconexión es automática.

---

## Controles de reproducción

| Control | Acción | Función |
|---------|--------|---------|
| Encoder — girar derecha | Subir volumen | Incrementa el volumen del sistema |
| Encoder — girar izquierda | Bajar volumen | Decrementa el volumen del sistema |
| Encoder — presionar | Play/Pause | Reproduce o pausa la reproducción |
| Botón PREV | Presionar | Retrocede a la pista anterior |
| Botón NEXT | Presionar | Avanza a la siguiente pista |
| Botón MUTE | Presionar | Silencia o activa el audio |

---

## Indicadores visuales

### LED azul — estado Bluetooth

| Estado del LED | Significado |
|----------------|-------------|
| Parpadeo rápido | Sin conexión — buscando dispositivo |
| Apagado | Conexión Bluetooth activa |

### LED rojo — estado de batería

| Estado del LED | Significado |
|----------------|-------------|
| Apagado | Batería OK — nivel suficiente |
| Parpadeo lento | Batería baja — recargar pronto |
| Parpadeo rápido | Batería crítica — recargar inmediatamente |

---

## Carga de batería

1. Conectar un cable USB-C al puerto **USB** ubicado en la parte inferior
   derecha del dispositivo.
2. Conectar el otro extremo a un cargador USB estándar o puerto USB de
   computadora.
3. El dispositivo puede seguir en uso durante la carga.
4. Desconectar el cable cuando la batería esté cargada.

> El módulo TP4056 incluye protección contra sobrecarga — no hay riesgo de
> dañar la batería si se deja conectado.

---

## Solución de problemas

| Problema | Posible causa | Solución |
|----------|---------------|----------|
| LED azul no enciende | Dispositivo apagado o batería descargada | Verificar interruptor SW y cargar batería |
| No aparece en la lista de Bluetooth | Dispositivo fuera de rango o apagado | Acercar el dispositivo y verificar que está encendido |
| Comandos no responden | Sin conexión Bluetooth | Verificar que el LED azul está apagado — si parpadea, reconectar |
| LED rojo parpadea rápido | Batería crítica | Cargar el dispositivo inmediatamente |
| Volumen cambia muy rápido | Normal — girar el encoder más despacio | Girar el encoder lentamente para ajuste fino |

---

*Proyecto: Controlador Multimedia Bluetooth — Sistemas Embebidos II*
*Universidad Católica Boliviana — Ingeniería Mecatrónica*
