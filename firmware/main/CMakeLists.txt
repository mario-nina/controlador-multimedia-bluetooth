idf_component_register(
    SRCS "firmware.c"
    INCLUDE_DIRS "."
    REQUIRES control_leds driver_entrada comunicacion_bt gestion_energia log freertos common esp_pm
)