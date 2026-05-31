idf_component_register(
    SRCS "driver_entrada.c" "botones.c" "encoder.c"
    INCLUDE_DIRS "include" "."
    REQUIRES esp_driver_gpio esp_driver_pcnt esp_timer freertos log common
)