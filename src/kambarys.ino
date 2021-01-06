#include <EEPROM.h>
#include <ESP8266NetBIOS.h>
#include <ESP8266WiFi.h>          //builtin library for ESP8266 Arduino Core
#include "FS.h"                   //builtin library for ESP8266 Arduino Core
#include "config.h"               //package builtin configuration file
#include <PubSubClient.h>
#include <time.h>
//#include <Time.h>
#include "DHTesp.h"
#include <ArduinoOTA.h>


DHTesp dht;
WiFiServer server(TCP_PORT);
WiFiClient client;
#ifdef MQQT
PubSubClient mqqt_client(client);
#endif
// set up a new serial port
/*SoftwareSerial my_serial =  SoftwareSerial(SW_SER_RX, SW_SER_TX);*/



uint32_t update_timer_time = 0;
uint32_t publish_time = 0;
float humidity = -1;
float temp = -1;
uint8_t Data = 0;


int wifi_reconnect_tries = 0;
long wifi_reconnect_time = 0L;
uint32_t wifi_check_time = 15000L;
uint8_t DataChanged = false;

#ifdef STATIC_IP
// Set your Static IP address
IPAddress local_IP(192, 168, 1, IP_END);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
#endif

/*******************************
  Connect to WiFi network
 *******************************/

void connectWiFi()
{

    WiFi.hostname(HOSTNAME);
    ++wifi_reconnect_tries;
    boolean networkScan = false;
    int n = WiFi.scanNetworks();
    delay(300);
    for (int i = 0; i < n; ++i) {

        if (WiFi.SSID(i) == WIFI_SSID) {
#ifdef DEBUG
            String this_print = String(WIFI_SSID) + " is available";
            Serial.println(this_print);
#endif
            networkScan = true;
            break;
        }
    }
    if(networkScan) {
#ifdef DEBUG
        String this_print = "Connecting to " + String(WIFI_SSID);
        Serial.println(this_print);
#endif
        long wifi_initiate = millis();
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        while (WiFi.status() != WL_CONNECTED) {
            /*Serial.print(".");*/
            if (WiFi.status() == WL_CONNECTED) {
                break;
            }
            if ((millis() - wifi_initiate) > 15000L) {
                break;
            }
            delay(500);
        }
        if (WiFi.status() == WL_CONNECTED) {
#ifdef DEBUG
            Serial.print(" Connected!!");
            Serial.println("");
            Serial.println(WiFi.localIP());
#endif
            wifi_reconnect_tries = 0;
        } else if ((WiFi.status() != WL_CONNECTED) && (wifi_reconnect_tries > 3)) {
#ifdef DEBUG
            String this_print = " Failed to connect to " + String(WIFI_SSID) + " Rebooting...";
            Serial.println(this_print);
#endif
        }
    } else {
        if (wifi_reconnect_tries > 3) {
            wifi_check_time = 300000L;
            wifi_reconnect_tries = 0;
#ifdef DEBUG
            Serial.println("System will try again after 5 minutes");
#endif
        }
    }
}



/***************************
  Keep Aliving Loop
 ***************************/

 void keeplive()
 {
   //client.loop();
   if((WiFi.status() != WL_CONNECTED) && ((millis() - wifi_reconnect_time) > wifi_check_time)) {
     wifi_check_time = 15000L;
     wifi_reconnect_time = millis();
     connectWiFi();
   }

#ifdef MQQT
   if (WiFi.status() == WL_CONNECTED && !mqqt_client.connected()) {
     MQQT_connect();
   }
#endif
 }


/*******************************
 Update sensors, time
 *******************************/


void update_info(){
    /*time_t now = time(nullptr);*/
    /*setTime(now);*/
    humidity = dht.getHumidity();
    temp = dht.getTemperature();

    // Check if any reads failed and exit early (to try again).
    if (dht.getStatus()) {
        humidity = -1;
        temp = -1;
    }


#ifdef DEBUG
    Serial.print(dht.getStatusString());
    Serial.print("\n");
    Serial.print(humidity, 3);
    Serial.print("\n");
    Serial.print(temp, 3);
    Serial.print("\n");
#endif

}

/*******************************
  Setup Function
 *******************************/

 void setup() {

#ifdef DEBUG
   Serial.begin(9600);
#endif
   init_io();

   WiFi.mode(WIFI_STA);
#ifdef STATIC_IP
   // Configures static IP address
   WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
#endif
   connectWiFi();
   //start tcp server
   server.begin();

#ifdef MQQT
   mqqt_client.setServer(MQQT_SERVER_IP, 1883);
   mqqt_client.setCallback(MQQT_callback);
#endif

   OTA_init();

   dht.setup(DHT_PIN,DHTesp::DHT22); // data pin 2
   update_timer_time = millis();

   update_info();
 }

/*******************************
  Loop Function
 *******************************/
void loop()
{
    //
    keeplive();   //necessary to call keep alive for proper functioning
    tcp_listen();
    mqqt_client.loop();
#if defined(VONIA) || defined(KORIDORIUS)
    check_io_timeouts();
#endif


    //update info every UPDATE_INTERVAL
    if(millis()-update_timer_time >= UPDATE_INTERVAL){
        update_info();
        update_timer_time = millis();
    }


#ifdef MQQT
    //update info every UPDATE_INTERVAL
    if(millis()-publish_time >= PUBLISH_INTERVAL){
      MQQT_publish();
      publish_time = millis();
    }
#endif

    ArduinoOTA.handle();
    delay(100);

}


/*******************************
  End of the file
 *******************************/
