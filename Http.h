#pragma once
#include <Arduino.h>

#define modem Serial2
class Http
{
public:
    enum class Mobile {
        yota,
        mts,
        tele2
    };
	Http();
	~Http();
	void init_http(Mobile oper);
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

    bool networkAuth(Mobile oper);

    bool yotaAuth();
    bool tele2Auth();

};

