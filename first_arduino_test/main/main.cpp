#include "Arduino.h"
#include "secrets.h"

#include <WiFi.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_pm.h"
#include "nvs_flash.h"


constexpr auto LED = 4;
constexpr auto *TAG = "power_save";


static void event_handler(void* arg, esp_event_base_t event_base,
								int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		esp_wifi_connect();
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(TAG, "got ip: " IPSTR, IP2STR(&event->ip_info.ip));
	}
}

/*init wifi as sta and set power save mode*/
static void wifi_power_save(void)
{
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
	assert(sta_netif);

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, NULL));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, NULL));

	wifi_config_t wifi_config = {
		.sta = {
			{.ssid = WIFI_SSID},
			{.password = WIFI_PASSWORD},
			// .listen_interval = DEFAULT_LISTEN_INTERVAL,
		},
	};
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGI(TAG, "esp_wifi_set_ps().");
	esp_wifi_set_ps(WIFI_PS_MIN_MODEM);
}


// void init_wifi() {
// 	WiFi.mode(WIFI_STA);
// 	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
// 	Serial.print("Connecting to WiFi ..");
// 	while (WiFi.status() != WL_CONNECTED) {
// 		Serial.print('.');
// 		delay(1000);
// 	}
// 	Serial.println(WiFi.localIP());
// }

void setup()
{
	// initialize LED digital pin as an output.
	// pinMode(LED, OUTPUT);
	// Serial.begin(115200);

	// constexpr auto DEFAULT_WAKEUP_PIN = 0;
	// constexpr auto DEFAULT_WAKEUP_LEVEL = ESP_GPIO_WAKEUP_GPIO_LOW;

	// const gpio_config_t config = {
	// 	.pin_bit_mask = BIT(DEFAULT_WAKEUP_PIN),
	// 	.mode = GPIO_MODE_INPUT,
	// };
	// ESP_ERROR_CHECK(gpio_config(&config));
	// ESP_ERROR_CHECK(esp_deep_sleep_enable_gpio_wakeup(BIT(DEFAULT_WAKEUP_PIN), DEFAULT_WAKEUP_LEVEL));

	// Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

#if CONFIG_PM_ENABLE
    // Configure dynamic frequency scaling:
    // maximum and minimum frequencies are set in sdkconfig,
    // automatic light sleep is enabled if tickless idle support is enabled.
    esp_pm_config_esp32c3_t pm_config = {
            .max_freq_mhz = 80,
            .min_freq_mhz = 10,
#if CONFIG_FREERTOS_USE_TICKLESS_IDLE
            .light_sleep_enable = true
#endif
    };
    ESP_ERROR_CHECK( esp_pm_configure(&pm_config) );
#endif // CONFIG_PM_ENABLE

    wifi_power_save();
}

void loop()
{
	// if (WiFi.status() != WL_CONNECTED) {
	// 	delay(5000);
	// 	init_wifi();
	// }

	// Serial.println("Going to sleep");
	// delay(5000);

	// esp_deep_sleep_start();

	// // turn the LED on (HIGH is the voltage level)
	// digitalWrite(LED, HIGH);

	// // wait for a second
	// delay(1000);

	// // turn the LED off by making the voltage LOW
	// digitalWrite(LED, LOW);

	// // wait for a second
	delay(30000);

	// Serial.println("Loop done");
}
