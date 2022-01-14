#include "Arduino.h"
#include "secrets.h"

#include <WiFi.h>


constexpr auto LED = 4;


void init_wifi() {
	WiFi.mode(WIFI_STA);
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.print("Connecting to WiFi ..");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print('.');
		delay(1000);
	}
	Serial.println(WiFi.localIP());
}

void setup()
{
	// initialize LED digital pin as an output.
	// pinMode(LED, OUTPUT);
	// Serial.begin(115200);
}

void loop()
{
	// if (WiFi.status() != WL_CONNECTED) {
	// 	delay(5000);
	// 	init_wifi();
	// }

	// Serial.println("Going to sleep");
	delay(5000);
	esp_deep_sleep_start();

	// turn the LED on (HIGH is the voltage level)
	digitalWrite(LED, HIGH);

	// wait for a second
	delay(1000);

	// turn the LED off by making the voltage LOW
	digitalWrite(LED, LOW);

	// wait for a second
	delay(1000);

	Serial.println("Loop done");
}
