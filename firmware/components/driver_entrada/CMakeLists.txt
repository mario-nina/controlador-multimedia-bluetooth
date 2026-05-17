idf_component_register(
    SRCS "driver_entrada.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_driver_gpio esp_timer freertos log
)