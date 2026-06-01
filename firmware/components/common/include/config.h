/**
 * @file config.h
 * @brief Configuración global del sistema — constantes de tiempo de compilación.
 *
 * Centraliza todos los parámetros ajustables del sistema en un único lugar.
 * No contiene lógica ni variables — solo constantes mediante #define.
 */

#pragma once

/* Prioridades de tareas FreeRTOS */
#define TAREA_PRIO_CMD      4  /**< Prioridad de la tarea de comandos          */
#define TAREA_PRIO_BATERIA  2  /**< Prioridad de la tarea de monitoreo batería */

/* Tamaños de stack de tareas */
#define TAREA_STACK_SIZE  2048  /**< Stack por defecto para tareas del sistema  */

/* Tamaños de colas */
#define COLA_ENTRADA_TAM   10  /**< Capacidad de la cola de eventos de entrada */
#define COLA_COMANDOS_TAM  10  /**< Capacidad de la cola de comandos           */

/* Configuración de gestión de energía */
#define PM_CPU_FREQ_MAX_MHZ  160  /**< Frecuencia máxima del CPU en MHz        */
#define PM_CPU_FREQ_MIN_MHZ   40  /**< Frecuencia mínima del CPU en MHz        */

/* Configuración de interrupciones */
#define ISR_FLAGS_DEFAULT  0  /**< Sin flags especiales — prioridad estándar   */