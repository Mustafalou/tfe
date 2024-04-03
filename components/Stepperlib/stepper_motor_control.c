#include "stepper_motor_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
esp_err_t rmt_write_items(rmt_channel_handle_t channel, const void *rmt_item, int item_num, bool wait_tx_done);
typedef struct {
    uint16_t duration0 : 15;
    uint16_t level0 : 1;
    uint16_t duration1 : 15;
    uint16_t level1 : 1;
} my_rmt_item32_t;

void stepper_motor_init(void) {
    // Vous pouvez initialiser votre moteur pas à pas ici, si nécessaire
}
uint32_t pps_to_delay_us(int32_t pps) {
    return 1000000 / pps; // Convertir les pas par seconde en microsecondes de délai entre les pas
}

void move_to_position(rmt_channel_handle_t motor_chan, int32_t target_position, int32_t current_position, uint32_t step_delay_us) {
    int32_t steps_to_move = target_position - current_position;

    if (steps_to_move == 0) {
        return; // Pas de mouvement nécessaire
    }

    // Configurez la direction du moteur
    gpio_set_level(4, steps_to_move > 0 ? 0 : !0);

    // Calculez le nombre absolu de pas à parcourir
    uint32_t steps_to_move_abs = abs(steps_to_move);

    // Définir la vitesse ici si nécessaire, en utilisant la variable 'speed'

    // Envoyer les impulsions pour faire tourner le moteur
    my_rmt_item32_t  step_pulse = {
        .level0 = 1,
        .duration0 = step_delay_us,
        .level1 = 0,
        .duration1 = step_delay_us
    };

      for (uint32_t i = 0; i < steps_to_move_abs; ++i) {
        rmt_write_items(motor_chan, &step_pulse, 1, false);
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}