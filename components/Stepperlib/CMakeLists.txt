idf_component_register(SRCS "stepper_motor_encoder.c" "stepper_motor_control.c"
                    INCLUDE_DIRS "include" 
                    REQUIRES "driver")
component_compile_options(-Wno-error=format= -Wno-format)
