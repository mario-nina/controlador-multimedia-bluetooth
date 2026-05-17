idf_component_register(
    SRCS "comunicacion_bt.c"
    INCLUDE_DIRS "include"
    REQUIRES nvs_flash bt log
)
