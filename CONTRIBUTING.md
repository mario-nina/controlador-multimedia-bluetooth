# Contribuciones

## Configuración del entorno

1. Instalar ESP-IDF v6.0.1 siguiendo la [documentación oficial](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html).

2. Clonar el repositorio:
```bash
git clone git@github.com:mario-nina/controlador-multimedia-bluetooth.git
cd controlador-multimedia-bluetooth
```

3. Activar el entorno ESP-IDF:
```bash
get_idf
```

4. Verificar que el proyecto compila:
```bash
cd firmware
idf.py set-target esp32
idf.py build
```

---

## Flujo de trabajo

1. Crear una rama desde `develop`:
```bash
git checkout develop
git checkout -b feature/<nombre-descriptivo>
```

2. Desarrollar los cambios y validarlos en hardware físico cuando corresponda.

3. Actualizar `CHANGELOG.md` con los cambios realizados y `ROADMAP.md` si se completa una fase.

4. Hacer commits siguiendo las convenciones definidas en `CONVENTIONS.md`.

5. Abrir un Pull Request hacia `develop` usando el template de PR.

---

## Convenciones

Ver [CONVENTIONS.md](CONVENTIONS.md) para las convenciones de código, commits y estructura de ramas.

---

## Validación antes de un PR

- El proyecto compila sin warnings con `idf.py build`
- Los cambios de hardware han sido validados físicamente si corresponde
- `CHANGELOG.md` refleja los cambios de esta rama
- Los mensajes de commit siguen el formato `<tipo>: <descripción>`
