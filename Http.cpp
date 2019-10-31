#include "Http.h"



Http::Http()
{
}


Http::~Http()
{
}

void Http::init_http()
{
	//reset();
	modem.begin(9600);
	delay(10000);

	bool inited = false;
	while (!inited) {
		{
			String resp;
			if (!sendAndWait("AT+CREG?", resp, "0,1", 600)) continue;
		}
		{
			String resp;
			if (!sendAndWait("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", resp, "OK", 600)) continue;
		}
		{
			String resp;
			if (!sendAndWait("AT+SAPBR=3,1,\"APN\",\"internet.tele2.ru\"", resp, "OK", 600)) continue;
		}

        {
            String resp;
            if (!sendAndWait("AT+SAPBR=3,1,\"USER\",\"tele2\"", resp, "OK", 600)) continue;
        }
        {
            String resp;
            if (!sendAndWait("AT+SAPBR=3,1,\"PWD\",\"tele2\"", resp, "OK", 600)) continue;
        }

		{
			String resp;
			if (!sendAndWait("AT+SAPBR=1,1", resp, "OK", 3200000)) continue;
		}
		inited = true;
		delay(1000);
	}
}

void Http::post(const String & url, const String & body)
{
	bool sended = false;
	{
		String resp;
		if (!sendAndWait("AT+HTTPTERM", resp, "OK", 120000)) {
			
		}
	}
	while(!sended){
		{
			String resp;
			if (!sendAndWait("AT+HTTPINIT", resp, "OK", 5000)) {
				sendAndWait("AT+HTTPTERM", resp, "OK", 10000);
				break;
			}
		}
		{
			String resp;
			if (!sendAndWait("AT+HTTPPARA=\"CID\",1", resp, "OK", 5000)) {
				sendAndWait("AT+HTTPTERM", resp, "OK", 10000);
				break;
			}
		}
		{
			String resp;
			if (!sendAndWait("AT+HTTPPARA=\"URL\",\""+ url + "\"", resp, "OK", 5000)) {
				sendAndWait("AT+HTTPTERM", resp, "OK", 10000);
				break;
			}
		}
		{
			String resp;
			if (!sendAndWait("AT+HTTPPARA=\"CONTENT\",\"application/json\"", resp, "OK", 5000)) {
				sendAndWait("AT+HTTPTERM", resp, "OK", 10000);
				break;
			}
		}
		{
			String resp;
			if (!sendAndWait("AT+HTTPDATA=" + String(body.length()) + ",20000", resp, "DOWNLOAD", 20000)) {
				sendAndWait("AT+HTTPTERM", resp, "OK", 10000);
				break;
			}
			sendAndWait(body, resp, "OK", 10000);

		}
		{	
			String resp;
			if (!sendAndWait("AT+HTTPSSL=1", resp, "OK", 5000)) {
				sendAndWait("AT+HTTPTERM", resp, "OK", 10000);
				break;
			}
		}
		{

			String resp;
			if (!sendAndWait("AT+HTTPACTION=1", resp, "+HTTPACTION:", 120000)) {
				sendAndWait("AT+HTTPTERM", resp, "OK", 10000);
				break;
			}
			sendAndWait("", resp, "asdasd", 1000);
		}
		{
			String resp;
			if (!sendAndWait("AT+HTTPREAD", resp, "OK", 120000)) {
				sendAndWait("AT+HTTPTERM", resp, "OK", 10000);
				break;
			}
		}
		{
			String resp;
			if (!sendAndWait("AT+HTTPTERM", resp, "OK", 120000)) {
				break;
			}
		}
		sended = true;
	}
}

bool Http::waitResp(String & resp, const String &stop, unsigned long timeout)
{
	unsigned long start = millis();
	unsigned long cur = 0;
	String recv;
	while (cur < timeout) {
		if (modem.available()) {
			recv += (char)modem.read();
		}
		if (recv.indexOf(stop) != -1) {
			resp = recv;
			return true;
		}
		if (recv.indexOf("ERROR") != -1) {
			resp = recv;
			return false;
		}
		cur = millis() - start;
	}
	
	return false;
}

void Http::sendCmd(String cmd)
{
	cmd += "\r\n";
	modem.write(cmd.c_str());
	Serial.write(cmd.c_str());
}

void Http::reset()
{
	int resetPin = 4;
	pinMode(resetPin, OUTPUT);
	Serial.println("SIM800L RESET...");
	digitalWrite(resetPin, HIGH);
	delay(200);
	digitalWrite(resetPin, LOW);
	delay(2000);
	digitalWrite(resetPin, HIGH);
	delay(3000);
	Serial.println("SIM800L RESET END");
}
