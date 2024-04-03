#include "driver/rmt_encoder.h"
#include "driver/gpio.h"

#ifndef _STEPPER_MOTOR_CONTROL_H_
#define _STEPPER_MOTOR_CONTROL_H_


void stepper_motor_init(void);
void move_to_position(rmt_channel_handle_t motor_chan, int32_t target_position, int32_t current_position, uint32_t step_delay_us);

#endif //_STEPPER_MOTOR_CONTROL_H_