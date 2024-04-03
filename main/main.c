#include <stdio.h> //for basic printf commands
#include <string.h> //for handling strings
#include "freertos/FreeRTOS.h" //for delay,mutexs,semphrs rtos operations
#include "freertos/task.h"
#include "esp_system.h" //esp_init funtions esp_err_t 
#include "esp_wifi.h" //esp_wifi_init functions and wifi operations
#include "esp_log.h" //for showing logs
#include "esp_event.h" //for wifi event
#include "nvs_flash.h" //non volatile storage
#include "lwip/err.h" //light weight ip packets error handling
#include "lwip/sys.h" //system applications for light weight ip apps

#include "mqtt_client.h"


const char *ssid = "Orange-c0f9d";
const char *pass = "v7cVdy4j";
int retry_num=0;

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data){
    if(event_id == WIFI_EVENT_STA_START){
        printf("WIFI CONNECTING....\n");
    }
    else if (event_id == WIFI_EVENT_STA_CONNECTED){
        printf("WiFi CONNECTED\n");
    }
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED){
        printf("WiFi lost connection\n");
        if(retry_num<5){
            esp_wifi_connect();
            retry_num++;
            printf("Retrying to Connect...\n");
        }
    }
    else if (event_id == IP_EVENT_STA_GOT_IP){
        printf("Wifi got IP...\n\n");
    }
}

void wifi_connection(){
    esp_netif_init(); //network interdace initialization
    esp_event_loop_create_default(); //responsible for handling and dispatching events
    esp_netif_create_default_wifi_sta(); //sets up necessary data structs for wifi station interface
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();//sets up wifi wifi_init_config struct with default values
    esp_wifi_init(&wifi_initiation); //wifi initialised with dafault wifi_initiation
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);//creating event handler register for wifi
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);//creating event handler register for ip event
    wifi_config_t wifi_configuration ={ //struct wifi_config_t var wifi_configuration
        .sta= {
            .ssid = "",
            .password= "", /*we are sending a const char of ssid and password which we will strcpy in following line so leaving it blank*/ 
        }//also this part is used if you donot want to use Kconfig.projbuild
    };
    strcpy((char*)wifi_configuration.sta.ssid,ssid); // copy chars from hardcoded configs to struct
    strcpy((char*)wifi_configuration.sta.password,pass);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);//setting up configs when event ESP_IF_WIFI_STA
    esp_wifi_start();//start connection with configurations provided in funtion
    esp_wifi_set_mode(WIFI_MODE_STA);//station mode selected
    esp_wifi_connect(); //connect with saved ssid and pass
    printf( "wifi_init_softap finished. SSID:%s  password:%s",ssid,pass);
}

static void mqtt_event_handler(esp_mqtt_event_handle_t event){ //here esp_mqtt_event_handle_t is a struct which receieves struct event from mqtt app start funtion
    esp_mqtt_client_handle_t client = event->client ;//making obj client of struct esp_mqtt_client_handle_t and assigning it the receieved event client
    if(event->event_id == MQTT_EVENT_CONNECTED){
        printf("MQTT_EVENT_CONNECTED");
        esp_mqtt_client_subscribe(client,"your topic",0); //in mqtt we require a topic to subscribe and client is from event client and 0 is quality of service it can be 1 or 2
        printf("sent subscribe successful" );
    }
    else if(event->event_id == MQTT_EVENT_DISCONNECTED)
    {
        printf("MQTT_EVENT_DISCONNECTED"); //if disconnected
    }
    else if(event->event_id == MQTT_EVENT_SUBSCRIBED)
    {
        printf("MQTT_EVENT_SUBSCRIBED");
    }
    else if(event->event_id == MQTT_EVENT_UNSUBSCRIBED) //when subscribed
    {
        printf("MQTT_EVENT_UNSUBSCRIBED");
    }
    else if(event->event_id == MQTT_EVENT_DATA)//when unsubscribed
    {
        printf("MQTT_EVENT_DATA");
    }
    else if(event->event_id == MQTT_EVENT_ERROR)//when any error
    {
        printf("MQTT_EVENT_ERROR");
    }
}

static void mqtt_initialize(void)
{/*Depending on your website or cloud there could be more parameters in mqtt_cfg.*/
    const esp_mqtt_client_config_t mqtt_cfg={
        .broker.address.uri="mqtt://localhost:1883", //Uniform Resource Identifier includes path,protocol

    };
    esp_mqtt_client_handle_t client=esp_mqtt_client_init(&mqtt_cfg); //sending struct as a parameter in init client function
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client); //starting the process
}



void app_main(void){
    nvs_flash_init(); // this is important in wifi case to store configurations , code will not work if this is not added
    wifi_connection();
    vTaskDelay(10000/portTICK_PERIOD_MS);
    mqtt_initialize();
}