set(srcs
    "AccelStepper.cpp"
    "MultiStepper.cpp"
    "accelstepper_wrapper.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include" "."
                    REQUIRES "driver" "esp_timer" "freertos")

target_link_libraries(${COMPONENT_LIB} INTERFACE "-Wl,--undefined=uxTopUsedPriority")