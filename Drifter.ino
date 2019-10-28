/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
#include "TinyGPS.h"
#include "Http.h"
// the setup function runs once when you press reset or power the board
TinyGPS gps;
Http http;
static unsigned long time_update = 0;
void setup() {
	Serial.begin(115200);
	Serial3.begin(9600);
	http.init_http();
	time_update = millis();
}

// the loop function runs over and over again forever

void printgps(float lat, float longt,  unsigned long age) {
	Serial.print("sat: ");
	Serial.print((int)gps.satellites());
	Serial.print("lat: ");
	Serial.print(lat);
	Serial.print(" long: ");
	Serial.print(longt);
	Serial.print(" age: ");
	Serial.println(age);
}


String toJson(float lat, float lon, float alt, float yaw, float sat, float speed, float hdop) {
	String format = "{\"data\": {\"latitude_gps\": \"%1\", \"longitude_gps\": \"%2\", \"altitude_gps\": \"%3\", \"yaw_gps\": \"%4\",  \"speed_gps\": \"%5\", \"satellites_gps\": \"%6\", \"hdop_gps\": \"%7\"},\"key\": \"around\"}";
	format.replace("%1", String(lat, 7));
	format.replace("%2", String(lon, 7));
	format.replace("%3", String(alt));
	format.replace("%4", String(yaw));
	format.replace("%5", String(sat));
	format.replace("%6", String(speed));
	format.replace("%7", String(hdop));
	//Serial.println(format);
	return format;
}

void loop() {
	while (Serial3.available()) {

		if (gps.encode(Serial3.read())) {
			float lat, longt;
			unsigned long age;
			gps.f_get_position(&lat, &longt, &age);
            String body = toJson(lat, longt, gps.f_altitude(), gps.f_course(), gps.f_speed_kmph(), gps.satellites(), gps.hdop());
            Serial.println(body);
            return;
			Serial.println((millis() - time_update));
			if ((millis() - time_update) > 30000) {
				String body = toJson(lat, longt, gps.f_altitude(), gps.f_course(), gps.f_speed_kmph(), gps.satellites(), gps.hdop());
                Serial.println(body);
				//http.post("aroundisland.ru/api/board/post/",
				//	body);
				time_update = millis();
			}
			return;
		}
	}
}
