idf_component_register(SRCS "wifi.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "driver" "esp_wifi" "esp_wifi" "nvs_flash" "lwip")
