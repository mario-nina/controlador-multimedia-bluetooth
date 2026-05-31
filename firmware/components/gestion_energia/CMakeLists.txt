idf_component_register(
    SRCS "gestion_energia.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_adc freertos log common
)