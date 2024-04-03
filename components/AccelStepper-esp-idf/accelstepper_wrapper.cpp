#include "accelstepper_wrapper.h"
#include "AccelStepper.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/*-----------*/
#include "freertos/semphr.h"


AccelStepperWrapper accelstepper_create(int motorInterfaceType, int stepPin, int dirPin) {
    return new AccelStepper(motorInterfaceType, static_cast<gpio_num_t>(stepPin), static_cast<gpio_num_t>(dirPin));
}

extern "C" void accelstepper_destroy(AccelStepperWrapper stepper) {
    static_cast<AccelStepper*>(stepper)->destroy();
}

void accelstepper_move_to(AccelStepperWrapper stepper, int32_t absolute) {
    static_cast<AccelStepper*>(stepper)->moveTo(absolute);
}

void accelstepper_set_speed(AccelStepperWrapper stepper, float speed) {
    static_cast<AccelStepper*>(stepper)->setSpeed(speed);
}
/*
void accelstepper_move(AccelStepperWrapper *stepper, int32_t relative_steps) {
    if (stepper && stepper->accelstepper) {
        stepper->accelstepper->move(relative_steps);
    }
}*/
void accelstepper_move(AccelStepperWrapper stepper, int32_t relative_steps) {
    static_cast<AccelStepper*>(stepper)->move(relative_steps);
}

void accelstepper_run_speed(AccelStepperWrapper stepper) {
    static_cast<AccelStepper*>(stepper)->runSpeed();
}
void accelstepper_set_max_speed(AccelStepperWrapper stepper, float max_speed) {
    ((AccelStepper *)stepper)->setMaxSpeed(max_speed);
}

void accelstepper_set_acceleration(AccelStepperWrapper stepper, float acceleration) {
    ((AccelStepper *)stepper)->setAcceleration(acceleration);
}

long accelstepper_distance_to_go(AccelStepperWrapper stepper) {
    return ((AccelStepper *)stepper)->distanceToGo();
}

void accelstepper_run(AccelStepperWrapper stepper) {
    ((AccelStepper *)stepper)->run();
}
void accelstepper_run_to_new_position(AccelStepperWrapper stepper, int32_t position) {
    if (stepper) {
        static_cast<AccelStepper*>(stepper)->runToNewPosition(position);
    }
}
/*--------------*/
void    accelstepper_runToPosition(AccelStepperWrapper stepper){
static_cast<AccelStepper*>(stepper)->runToPosition();
}
/*------------------------------*/
void accelstepper_stop(AccelStepperWrapper stepper) {
    static_cast<AccelStepper*>(stepper)->stop();
}
/*
long accelstepper_current_position(AccelStepperWrapper stepper) {
    return ((AccelStepper *)stepper)->currentPosition();
}*/
extern "C" {
    SemaphoreHandle_t get_accel_stepper_mutex();
}

int32_t accelstepper_current_position(AccelStepperWrapper stepper) {
    xSemaphoreTake(get_accel_stepper_mutex(), portMAX_DELAY);
    int32_t position = static_cast<AccelStepper*>(stepper)->currentPosition();
    xSemaphoreGive(get_accel_stepper_mutex());
    return position;
}

    float accelstepper_max_speed(AccelStepperWrapper stepper) {
        return ((AccelStepper *)stepper)->maxSpeed();
    }



#include "MultiStepper.h"

MultiStepperWrapper multistepper_create() {
    return new MultiStepper();
}

void multistepper_destroy(MultiStepperWrapper multi_stepper) {
    delete static_cast<MultiStepper*>(multi_stepper);
}

int multistepper_add_stepper(MultiStepperWrapper multi_stepper, AccelStepperWrapper stepper) {
    return static_cast<MultiStepper*>(multi_stepper)->addStepper(*static_cast<AccelStepper*>(stepper));
}

//int multistepper_set_positions(MultiStepperWrapper multi_stepper, long* positions, int num_positions) {
    //return static_cast<MultiStepper*>(multi_stepper)->setPositions(positions, num_positions);
//}

int multistepper_run(MultiStepperWrapper multi_stepper) {
    return static_cast<MultiStepper*>(multi_stepper)->run();
}
