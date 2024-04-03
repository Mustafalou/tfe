#include <stdio.h>
#include "wifi.h"

// Wifi event handler
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
  switch (event->event_id) {

    case SYSTEM_EVENT_STA_START:
      esp_wifi_connect();
      break;

    case SYSTEM_EVENT_STA_GOT_IP:
      xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
      break;

    case SYSTEM_EVENT_STA_DISCONNECTED:
      xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
      break;

    default:
      break;
  }

  return ESP_OK;
}

// Main task
void main_task(void *pvParameter)
{
  tcpip_adapter_ip_info_t ip_info;
  // wait for connection
  printf("Waiting for connection to the wifi network... \n");
  xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
  printf("Connected!\n");

  // print the local IP address
  ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));
  printf("IP Address:  %s\n", ip4addr_ntoa(&ip_info.ip));
  printf("Subnet mask: %s\n", ip4addr_ntoa(&ip_info.netmask));
  printf("Gateway:     %s\n", ip4addr_ntoa(&ip_info.gw));
  printf("You can connect now to any webserver online! :-)\n");

  while (1) {
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}