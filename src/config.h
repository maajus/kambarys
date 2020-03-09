#ifndef _CONFIG_H
#define _CONFIG_H

#define VONIA


const char* WIFI_SSID = "Stotis";
const char* WIFI_PASSWORD = "turekbabkiu";


#define DHTTYPE DHT22   // DHT 11
#define TCP_PORT 5555
#define UPDATE_INTERVAL 30000
#define DEBOUNCE_DELAY_MS 250

//Button pins
// #define BUTTON0  12 //D6
// #define BUTTON1  13 //D7
// #define BUTTON2  3 //D9
// #define BUTTON3  1 //D10


#ifdef VONIA

#define HOSTNAME "vonia"

// Some SSR are active low...
#define LIGHTS_ON     1
#define LIGHTS_OFF    0

// DHT Sensor
#define DHT_PIN       2 //D4
// LED pin
#define LED_PIN       14 //D5


#define OUT_PINS_COUNT  4
#define IN_PINS_COUNT  4

// ssr control
int OUT_PINS[OUT_PINS_COUNT] = {
  5,  // D1 lempos
  16, // D0 ledai
  4,  // D2 ventiliatorius
  0,  // D3 veidrodis
};

// buttons
int IN_PINS[IN_PINS_COUNT] = {
  12,  // D6
  13,  // D7
  1,   // D9 TX pin
  3,   // D10 RX pin
};

#endif




#endif
