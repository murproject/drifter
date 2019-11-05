
#include "TinyGPS.h"
#include "Http.h"

TinyGPS gps;
Http http;


/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
/*!!!Установить const int ID в значение 1, 2 или 3 для каждого дрифтера значение должно быть уникально!!!!*/
/*!!!!!! т.е. для первого дрифтра установить значение в 1, второго в 2 и для третьего в 3!!!!!!!*/
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
const int ID = 1;
/* Отображение ID в цвета */
/* ID = 1 - красный */
/* ID = 2 - зеленый */
/* ID = 3 - синий */
/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
//Выбор оператора SIM
Http::Mobile oper = Http::Mobile::yota;


static unsigned long time_update = 0;
// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);
    if (ID < 1 || ID > 3) {
        while (true) {
            Serial.println("/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/");
            Serial.println("/*!!!!!!!!!!!! WRONG ID !!!!!!!!!!*/");
            Serial.println("/*!!!!! See line 13 in code! !!!!!*/");
            Serial.println("/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/");
            delay(1000);
        }
    }

    Serial3.begin(9600);
    http.init_http(oper);
    time_update = millis();
}



void printgps(float lat, float longt, unsigned long age) {
    Serial.print("sat: ");
    Serial.print((int)gps.satellites());
    Serial.print("lat: ");
    Serial.print(lat, 7);
    Serial.print(" long: ");
    Serial.print(longt, 7);
    Serial.print(" age: ");
    Serial.println(age);
}


String toJson(int id, float speed, float lat, float lng) {
    String format = "{\"id\": \"%1\", \"speed\": \"%2\", \"lat\": \"%3\", \"long\": \"%4\"}";
    format.replace("%1", String(id));
    format.replace("%2", String(speed));
    format.replace("%3", String(lat, 7));
    format.replace("%4", String(lng, 7));
    return format;
}
// the loop function runs over and over again forever
void loop() {
    while (Serial3.available()) {
        if (gps.encode(Serial3.read())) {
            float lat, longt;
            unsigned long age;
            gps.f_get_position(&lat, &longt, &age);
            if ((millis() - time_update) > 6000) {
                String body = toJson(ID, gps.f_speed_kmph(), lat, longt);
                http.post("https://robocenter.net/murtracker/point/create/",
                    body);

                time_update = millis();
            }
        }
    }
}
