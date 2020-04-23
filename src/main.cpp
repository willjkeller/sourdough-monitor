/*
	HTTP over TLS (HTTPS) example sketch

	This example demonstrates how to use
	WiFiClientSecure class to access HTTPS API.
	We fetch and display the status of
	esp8266/Arduino project continuous integration
	build.

	Limitations:
	  only RSA certificates
	  no support of Perfect Forward Secrecy (PFS)
	  TLSv1.2 is supported since version 2.4.0-rc1

	Created by Ivan Grokhotkov, 2015.
	This example is in public domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "WebSocketClient.h"
#include "ArduinoJson.h"
#include <Time.h>
#include <String.h>
#include <string>

# WiFi.mode(WIFI_STA);


#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTPIN D2    
#define DHTTYPE    DHT22 

#ifndef STASSID
#define STASSID "Keller Home 2.4G"
#define STAPSK  "9105997989"
#endif                                                                                                                                               

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG Serial.println
#else
#define DEBUG_MSG(MSG)
#endif

unsigned long heartbeatInterval = 0;
unsigned long lastHeartbeatAck = 0;
unsigned long lastHeartbeatSend = 0;

bool hasWsSession = false;
String websocketSessionId;
bool hasReceivedWSSequence = false;
unsigned long lastWebsocketSequence = 0;

WebSocketClient ws(true);
DynamicJsonDocument doc(2048);
DHT dht(DHTPIN, DHTTYPE);

//String bot_token = "NTY0NjkxODk2MzA0MTQwMjg4.Xp_RdA.Nsc82xW_oHsw3zaUf2tsZ6uZNNQ";
String bot_token   = "NTY0NjkxODk2MzA0MTQwMjg4.Xp_RdA.Nsc82xW_oHsw3zaUf2tsZ6uZNNQ";

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "discordapp.com";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char fingerprint[] PROGMEM = "E1 8A 1D 84 F2 0E DE B1 0C 6F F7 8A 95 94 84 B9 EB 42 53 B8";// 59 74 61 88 13 CA 12 34 15 4D 11 0A C1 7f E6 67 07 69 42 F5"; //5F F1 60 31 09 04 3E F2 90 D2 B0 8A 50 38 04 E8 37 9F BC 76";
  // Use WiFiClientSecure class to create TLS connection




void make_req() {
    WiFiClientSecure client;	
    String url;
    url = "/api/v6/guilds/494533687585538048";///repos/esp8266/Arduino/commits/master/status";

    Serial.print("connecting to ");
    Serial.println(host);
    Serial.printf("Using fingerprint '%s'\n", fingerprint);
    client.setFingerprint(fingerprint);

    if (!client.connect(host, httpsPort)) {
        Serial.println("connection failed");
        return;
    } else {
        Serial.println("REST GOOD");
    }

    Serial.print("requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
			   "Host: " + host + "\r\n" +
			   "Authorization: Bot " + bot_token + "\r\n" +
			   "User-Agent: DiscordBot (freedems.org, 1.0)\r\n" +
			   "Connection: close\r\n\r\n");

    Serial.println(String("GET ") + url + " HTTP/1.1\r\n" +
			   "Host: " + host + "\r\n" +
			   "Authorization: Bot " + bot_token + "\r\n" +
			   "User-Agent: DiscordBot (freedems.org, 1.0)\r\n" +
			   "Connection: close\r\n\r\n");

    Serial.println("request sent");

	while (client.connected() || client.available())
	{
	  if (client.available())
	  {
		String line = client.readStringUntil('\n');
		Serial.println(line);
		break;
	  }
	}
	//while (client.available()) {
	//	client.readStringUntil('\n');
	//}
	client.stop();
	Serial.println("\n[Disconnected]");

    return;
}


void make_req2() {
    WiFiClientSecure client;	
    String url;
    url = "/api/v6/channels/528755453661020182/messages";///repos/esp8266/Arduino/commits/master/status";

    Serial.print("connecting to ");
    Serial.println(host);
    Serial.printf("Using fingerprint '%s'\n", fingerprint);
    client.setFingerprint(fingerprint);

    if (!client.connect(host, httpsPort)) {
        Serial.println("connection failed");
        return;
    } else {
        Serial.println("REST GOOD");
    }

    Serial.print("requesting URL: ");
    Serial.println(url);

    String conten = "RH: " + String(dht.readHumidity()) + " Temp: " + String(dht.readTemperature(true));

    String mess = "{\"content\":\""+ conten + "\"}\r\n\r\n";

    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
			   "Host: " + host + "\r\n" +
			   "Authorization: Bot " + bot_token + "\r\n" +
			   "User-Agent: DiscordBot (freedems.org, 1.0)\r\n" +
               "Content-Type: application/json\r\n"+
               "Content-Length: " + mess.length() + "\r\n"+
			   "Connection: close\r\n\r\n" + 
               mess);

    Serial.print(String("POST ") + url + " HTTP/1.1\r\n" +
			   "Host: " + host + "\r\n" +
			   "Authorization: Bot " + bot_token + "\r\n" +
			   "User-Agent: DiscordBot (freedems.org, 1.0)\r\n" +
               "Content-Type: application/json\r\n"+
               "Content-Length: " + mess.length() + "\r\n"+
			   "Connection: close\r\n\r\n" + 
               mess);

    Serial.println("request sent");

	while (client.connected() || client.available())
	{
	  if (client.available())
	  {
		String line = client.readStringUntil('\n');
		Serial.println(line);
		break;
	  }
	}
	//while (client.available()) {
	//	client.readStringUntil('\n');
	//}
	client.stop();
	Serial.println("\n[Disconnected]");

    return;
}




void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
	delay(500);
	Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

 

//make_req();




}
//		Serial.println("loopstart");
//socket
void loop() {
    //make_req();
      /*float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("\n%  Temperature: "));
  Serial.print(f);
    delay(2000);*/
	if (!ws.isConnected()) {
		Serial.println("connecting ws.........");
		// It technically should fetch url from discordapp.com/api/gateway
		ws.connect("gateway.discord.gg", "https://gateway.discord.gg/", 443);
	} else {
        unsigned long now = millis();
        if(heartbeatInterval > 0)
        {
            if(now > lastHeartbeatSend + heartbeatInterval)
            {
                if(hasReceivedWSSequence)
                {
                    //DEBUG_MSG("Send:: {\"op\":1,\"d\":" + String(lastWebsocketSequence, 10) + "}");
                    ws.send("{\"op\":1,\"d\":" + String(lastWebsocketSequence, 10) + "}");
                }
                else
                {
                    //DEBUG_MSG("Send:: {\"op\":1,\"d\":null}");
                    ws.send("{\"op\":1,\"d\":null}");
                }
                lastHeartbeatSend = now;
            }
            if(lastHeartbeatAck > lastHeartbeatSend + (heartbeatInterval / 2))
            {
                DEBUG_MSG("Heartbeat ack timeout");
                ws.disconnect();
                heartbeatInterval = 0;
            }
        }

        String msg;
        if (ws.getMessage(msg))
        {

            deserializeJson(doc, msg);

            // TODO Should maintain heartbeat
            if(doc["op"] == 0) // Message
            {
                if(doc.containsKey("s"))
                {
                    lastWebsocketSequence = doc["s"];
                    hasReceivedWSSequence = true;
                }

                if(doc["t"] == "READY")
                {
                    websocketSessionId = doc["d"]["session_id"].as<String>();
                    hasWsSession = true;
                }

                if(doc["t"] == "MESSAGE_CREATE")
                {
                    Serial.print("\nMESSAGE: ");
                    Serial.println(doc["d"]["content"].as<String>());

                    if(doc["d"]["content"].as<String>() == "!trig") {
                        Serial.println("TRIGTRIGTRIG");
                        ws.disconnect();
                        make_req();
                    }             
                }

            }
            else if(doc["op"] == 9) // Connection invalid
            {
                ws.disconnect();
                hasWsSession = false;
                heartbeatInterval = 0;
            }
            else if(doc["op"] == 11) // Heartbeat ACK
            {
                lastHeartbeatAck = now;
            }
            else if(doc["op"] == 10) // Start
            {
                heartbeatInterval = doc["d"]["heartbeat_interval"];
                Serial.print("heart int*********************: ");
                Serial.println(heartbeatInterval);
                if(hasWsSession)
                {
                    DEBUG_MSG("Send:: {\"op\":6,\"d\":{\"token\":\"" + bot_token + "\",\"session_id\":\"" + websocketSessionId + "\",\"seq\":\"" + String(lastWebsocketSequence, 10) + "\"}}");
                    ws.send("{\"op\":6,\"d\":{\"token\":\"" + bot_token + "\",\"session_id\":\"" + websocketSessionId + "\",\"seq\":\"" + String(lastWebsocketSequence, 10) + "\"}}");
                }
                else
                {
                    DEBUG_MSG("Send:: {\"op\":2,\"d\":{\"token\":\"" + bot_token + "\",\"properties\":{\"$os\":\"linux\",\"$browser\":\"ESP8266\",\"$device\":\"ESP8266\"},\"compress\":false,\"large_threshold\":250}}");
                    ws.send("{\"op\":2,\"d\":{\"token\":\"" + bot_token + "\",\"properties\":{\"$os\":\"linux\",\"$browser\":\"ESP8266\",\"$device\":\"ESP8266\"},\"compress\":false,\"large_threshold\":250,\"intents\":512}}");
                }

                lastHeartbeatSend = now;
                lastHeartbeatAck = now;
            }
            
            Serial.println("=========================");

        }

    }
}
