#ifndef ACCELSTEPPER_WRAPPER_H
#define ACCELSTEPPER_WRAPPER_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void* AccelStepperWrapper;

AccelStepperWrapper accelstepper_create(int motorInterfaceType, int stepPin, int dirPin);
void accelstepper_destroy(AccelStepperWrapper stepper);
void accelstepper_move_to(AccelStepperWrapper stepper, int32_t absolute);
void accelstepper_set_speed(AccelStepperWrapper stepper, float speed);
void accelstepper_run_speed(AccelStepperWrapper stepper);
void accelstepper_move(AccelStepperWrapper *stepper, int32_t relative_steps);
void accelstepper_set_max_speed(AccelStepperWrapper stepper, float max_speed);
void accelstepper_set_acceleration(AccelStepperWrapper stepper, float acceleration);
long accelstepper_distance_to_go(AccelStepperWrapper stepper);
void accelstepper_run(AccelStepperWrapper stepper);
void accelstepper_run_to_new_position(AccelStepperWrapper stepper, int32_t position);
void accelstepper_runToPosition(AccelStepperWrapper stepper);
/*-----------*/
void accelstepper_stop(AccelStepperWrapper stepper);
/*------------*/
int32_t accelstepper_current_position(AccelStepperWrapper stepper);

float accelstepper_max_speed(AccelStepperWrapper stepper);

typedef void* MultiStepperWrapper;

MultiStepperWrapper multistepper_create();
void multistepper_destroy(MultiStepperWrapper multi_stepper);
int multistepper_add_stepper(MultiStepperWrapper multi_stepper, AccelStepperWrapper stepper);
int multistepper_set_positions(MultiStepperWrapper multi_stepper, long* positions, int num_positions);
int multistepper_run(MultiStepperWrapper multi_stepper);

#ifdef __cplusplus
}
#endif

#endif // ACCELSTEPPER_WRAPPER_H