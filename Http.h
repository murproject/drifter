#pragma once
#include <Arduino.h>

#define modem Serial2
class Http
{
public:
	Http();
	~Http();
	void init_http();
	void post(const String &url, const String &body);

private:
	bool waitResp(String & resp, const String &stop, unsigned long timeout);
	void sendCmd(String cmd);
	void reset();
	bool sendAndWait(String cmd, String & resp, const String &stop, unsigned long timeout) {
		sendCmd(cmd);
		bool ret = waitResp(resp, stop, timeout);
		Serial.println(resp);
		return ret;
	}
};

