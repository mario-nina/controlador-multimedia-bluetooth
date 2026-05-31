# Convenciones del proyecto

## Idioma

- Documentación, comentarios y nombres de variables/funciones: **español**
- Palabras técnicas sin traducción directa (handle, callback, stack, payload, flag): se mantienen en inglés

---

## Estructura de ramas

| Rama | Propósito |
|------|-----------|
| `main` | Código estable y validado en hardware. Solo recibe merges desde `develop`. |
| `develop` | Integración activa de desarrollo. Base para nuevas ramas de feature. |
| `feature/<nombre>` | Desarrollo de una funcionalidad nueva. Se fusiona en `develop` al completarse. |

**Regla:** nunca hacer push directo a `main`. Todo cambio entra por `develop`.

---

## Mensajes de commit

Formato: `<tipo>: <descripción en imperativo y minúsculas>`

### Tipos válidos

| Tipo | Cuándo usarlo |
|------|---------------|
| `feat` | Nueva funcionalidad implementada |
| `fix` | Corrección de un error |
| `docs` | Cambios exclusivamente en documentación |
| `test` | Registro de pruebas físicas o de validación |
| `hardware` | Cambios en esquemático, PCB o archivos KiCad |
| `chore` | Configuración, dependencias, `.gitignore`, CMakeLists |
| `refactor` | Reestructuración de código sin cambio de comportamiento |

### Ejemplos correctos

```
feat: implementar perfil HID Consumer Control con reportes BLE
fix: corregir debouncing del encoder en cambios rápidos de dirección
docs: actualizar roadmap con estado real de las fases
test: confirmar GPIO22 GPIO23 GPIO25 sin rebotes en hardware físico
hardware: completar esquemático con cadena de potencia y ERC sin errores
chore: agregar sdkconfig.defaults con configuración NimBLE
refactor: mover nvs_flash_init a app_main
```

---

## Código C

### Nombres

| Elemento | Convención | Ejemplo |
|----------|------------|---------|
| Funciones | `snake_case` con prefijo del módulo | `driver_entrada_init`, `control_leds_set` |
| Variables locales | `snake_case` | `cola_eventos`, `ultimo_tiempo` |
| Variables estáticas de módulo | `snake_case` | `static uint16_t conn_handle` |
| Constantes y macros | `SNAKE_CASE_MAYUSCULAS` | `DEBOUNCE_TIEMPO_US`, `LED_PERIODO_RAPIDO_US` |
| Tipos (`typedef enum/struct`) | `snake_case_t` | `evento_entrada_t`, `sys_state_t` |
| Tags de log ESP-IDF | nombre del módulo en minúsculas | `"driver_entrada"`, `"hid"` |

### Archivos

- Implementación: `<modulo>.c`
- Cabecera pública (API externa): `include/<modulo>.h`
- Cabecera interna (uso solo dentro del componente): `<nombre>.h` en la raíz del componente

### Comentarios

- Solo cuando el **por qué** no es obvio: una restricción oculta, un invariante sutil, un workaround para un bug específico del hardware
- Usar `@brief` en funciones públicas de los headers
- No comentar **qué** hace el código — los nombres de funciones y variables deben ser suficientes
- Los bloques `TODO` se marcan con `/* TODO Fase N: descripción */`

---

## Estructura de componentes ESP-IDF

```
components/<modulo>/
├── CMakeLists.txt
├── <modulo>.c              # Implementación principal
├── <submodulo>.c           # Submódulo interno (si aplica)
├── <submodulo>.h           # Header interno del submódulo
└── include/
    └── <modulo>.h          # API pública del componente
```

---

## Estructura del repositorio

```
controlador-multimedia-bluetooth/
├── firmware/               # Código fuente del firmware (ESP-IDF)
│   ├── main/               # Punto de entrada de la aplicación
│   └── components/         # Módulos independientes
├── hardware/               # Archivos de diseño de hardware
│   ├── controlador_multimedia/   # Proyecto KiCad (fuente)
│   ├── esquematico/              # Exportaciones del esquemático (PDF/SVG)
│   ├── pcb/                      # Exportaciones Gerber
│   ├── diseno_3d/                # Exportaciones STEP
│   └── pines.md                  # Asignación de pines con justificación técnica
├── docs/                   # Documentación técnica
│   ├── memoria_tecnica/    # Memoria técnica del proyecto
│   ├── manual_usuario/     # Manual de usuario
│   └── lista_materiales/   # Lista de materiales con costos
├── test/
│   └── resultados/         # Resultados de pruebas físicas por fase
├── CHANGELOG.md
├── CONVENTIONS.md
├── CONTRIBUTING.md
├── LICENSE
├── README.md
└── ROADMAP.md
```

---

## Flujo de trabajo

1. Crear rama `feature/<nombre>` desde `develop`
2. Desarrollar y validar en hardware físico
3. Actualizar `CHANGELOG.md` y `ROADMAP.md` al completar la fase
4. Crear Pull Request hacia `develop` usando el template de PR
5. Fusionar en `develop`
6. Fusionar `develop` → `main` solo cuando la fase esté completamente validada
