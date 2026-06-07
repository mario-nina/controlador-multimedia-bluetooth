# Protocolo de pruebas formales — Controlador Multimedia Bluetooth v1.0

Define las pruebas a realizar para validación funcional del sistema completo.

---

## Configuración base

| Parámetro | Valor |
|-----------|-------|
| Firmware | v1.0.0 |
| Dispositivo objetivo | Ubuntu 24.04 LTS con Bluetooth integrado |
| Batería | Li-ion 18650 3.7V 2600mAh cargada completamente |

---

## Prueba 1 — Alcance BLE

**Objetivo:** determinar el rango operativo del enlace Bluetooth en condiciones reales.

**Procedimiento:**
1. Conectar el dispositivo y establecer conexión BLE.
2. Ejecutar un comando de prueba (Play/Pause) en cada rango.
3. Registrar si el comando se ejecuta correctamente.
4. Repetir con y sin obstáculos entre el dispositivo y la computadora.

**Rangos evaluados:**

| Rango | Condición | Resultado |
|-------|-----------|-----------|
| Corto (< 5m) | Sin obstáculos | — |
| Medio (5–10m) | Sin obstáculos | — |
| Largo (> 10m) | Sin obstáculos | — |
| Corto (< 5m) | Con obstáculos (paredes/puertas) | — |
| Medio (5–10m) | Con obstáculos (paredes/puertas) | — |

**Métrica:** rango máximo operativo con y sin obstáculos.

---

## Prueba 2 — Latencia

**Objetivo:** evaluar el tiempo de respuesta entre activación del control y
ejecución de la acción en Ubuntu.

**Procedimiento:**
1. Conectar el monitor serie del ESP32.
2. Ejecutar cada comando 10 veces.
3. Registrar el timestamp del evento en el monitor serie.
4. Evaluar subjetivamente si el retardo es perceptible durante el uso normal.

**Comandos evaluados:**

| Comando | Evaluación subjetiva | Timestamp promedio (ms) |
|---------|----------------------|------------------------|
| Play/Pause | — | — |
| Siguiente pista | — | — |
| Pista anterior | — | — |
| Subir volumen | — | — |
| Bajar volumen | — | — |
| Mute/Unmute | — | — |

**Métrica:** timestamp del firmware + evaluación cualitativa (perceptible / no perceptible).

---

## Prueba 3 — Autonomía de batería

**Objetivo:** medir el tiempo de operación continua con batería completamente cargada.

**Procedimiento:**
1. Cargar la batería completamente.
2. Encender el dispositivo y establecer conexión BLE.
3. Ejecutar comandos aproximadamente cada 5 minutos simulando uso normal.
4. Registrar hora de inicio y hora en que el dispositivo deja de funcionar.

**Registro:**

| Parámetro | Valor |
|-----------|-------|
| Hora de inicio | — |
| Hora de fin | — |
| Duración total | — |
| Nivel de batería al inicio | 100% |

**Métrica:** horas de operación continua en uso normal.

---

## Prueba 4 — Confiabilidad

**Objetivo:** verificar que los comandos se ejecutan correctamente de manera
consistente bajo uso repetido.

**Procedimiento:**
1. Conectar el dispositivo y establecer conexión BLE.
2. Ejecutar cada comando 50 veces consecutivas.
3. Registrar cuántos se ejecutaron correctamente en Ubuntu.

**Resultados:**

| Comando | Ejecutados | Correctos | Fallidos | Tasa de éxito |
|---------|------------|-----------|----------|---------------|
| Play/Pause | 50 | — | — | — |
| Siguiente pista | 50 | — | — | — |
| Pista anterior | 50 | — | — | — |
| Subir volumen | 50 | — | — | — |
| Bajar volumen | 50 | — | — | — |
| Mute/Unmute | 50 | — | — | — |
| **Total** | **300** | — | — | — |

**Métrica:** porcentaje de éxito por comando y global.

---

## Prueba 5 — Compatibilidad

**Objetivo:** verificar funcionamiento con distintas aplicaciones de reproducción.

**Procedimiento:**
1. Conectar el dispositivo y establecer conexión BLE.
2. Probar los 6 comandos con cada aplicación.
3. Registrar cuáles comandos responden correctamente.

**Resultados:**

| Comando | Spotify | VLC | — |
|---------|---------|-----|---|
| Play/Pause | — | — | — |
| Siguiente pista | — | — | — |
| Pista anterior | — | — | — |
| Subir volumen | — | — | — |
| Bajar volumen | — | — | — |
| Mute/Unmute | — | — | — |

> Completar la tercera columna con la aplicación adicional elegida al momento
> de ejecutar las pruebas.

**Métrica:** tabla de compatibilidad por aplicación.

---

*Proyecto: Controlador Multimedia Bluetooth — Sistemas Embebidos II*
*Universidad Católica Boliviana — Ingeniería Mecatrónica*
