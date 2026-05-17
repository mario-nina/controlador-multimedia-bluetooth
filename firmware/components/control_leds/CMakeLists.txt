idf_component_register(
    SRCS "control_leds.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_driver_gpio esp_timer log
)