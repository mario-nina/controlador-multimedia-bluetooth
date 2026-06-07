# Resultados — Integración completa

## Configuración de prueba

| Parámetro | Valor |
|-----------|-------|
| Fecha | Mayo 2026 |
| Firmware | v0.1.0 |
| Dispositivo objetivo | Ubuntu 24.04 LTS con Bluetooth integrado |
| Aplicación | Spotify |

---

## Resultados por comando

| Comando | Control físico | Resultado |
|---------|---------------|-----------|
| Play/Pause | Encoder — presionar | ✓ Funciona |
| Siguiente pista | Botón 1 | ✓ Funciona |
| Pista anterior | Botón 2 | ✓ Funciona |
| Subir volumen | Encoder — girar derecha | ✓ Funciona |
| Bajar volumen | Encoder — girar izquierda | ✓ Funciona |
| Mute/Unmute | Botón 3 | ✓ Funciona |

---

## Estabilidad

| Métrica | Resultado |
|---------|-----------|
| Duración de sesión continua | ~30 minutos |
| Desconexiones espontáneas | Ninguna |
| Reconexión tras desconexión | Manual desde Configuración → Bluetooth |

---

## Observaciones

- Ubuntu desconecta el dispositivo tras período de inactividad HID — comportamiento normal del sistema operativo.
- La reconexión manual desde Configuración → Bluetooth funciona correctamente sin necesidad de re-emparejar.
