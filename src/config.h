#ifndef _CONFIG_H
#define _CONFIG_H

//#define MIEGAMASIS
//#define VONIA
#define KORIDORIUS


const char* WIFI_SSID = "Stotis";
const char* WIFI_PASSWORD = "turekbabkiu";


#define DHTTYPE DHT22   // DHT 11
#define TCP_PORT 5555
#define UPDATE_INTERVAL 30000
#define DEBOUNCE_DELAY_MS 300

//Button pins
// #define BUTTON0  12 //D6
// #define BUTTON1  13 //D7
// #define BUTTON2  3 //D9
// #define BUTTON3  1 //D10


#ifdef VONIA

#define HOSTNAME "vonia"
#define PIR_TIMEOUT   60000 //10s
#define PIR_DEBOUNCE_TIMEOUT 5000 //3s

// Some SSR are active low...
#define LIGHTS_ON     0
#define LIGHTS_OFF    1

// DHT Sensor
#define DHT_PIN       14 //D5
// LED pin
// #define LED_PIN       14 //D5


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
  12,  // D6 Pirmas mygtukas
  13,  // D7 Vidurinis mygtukas
  3,   // D10 Trecias mygtukas
  1,   // D9
};

#endif


#ifdef MIEGAMASIS

#define HOSTNAME "miegamasis"

// Some SSR are active low...
#define LIGHTS_ON     1
#define LIGHTS_OFF    0

// DHT Sensor
#define DHT_PIN       14 //D5
// LED pin
// #define LED_PIN       14 //D5


#define OUT_PINS_COUNT  4
#define IN_PINS_COUNT  4

// ssr control
int OUT_PINS[OUT_PINS_COUNT] = {
  16, // D0 Lubos
  0,  // D3 Simos puse
  4,  // D2 Justo puse
  5,  // D1 Sudeges ssr
};

// buttons
int IN_PINS[IN_PINS_COUNT] = {
  12,  // D6 Pirmas mygtukas
  13,  // D7 Trecias mygtukas (viduj)
  3,   // D10 (RX pin) antras mygtukas
  1,   // D9 TX pin  PIR
};

#endif




#ifdef KORIDORIUS

#define HOSTNAME "koridorius"
#define PIR_TIMEOUT   60000 //10s
#define PIR_DEBOUNCE_TIMEOUT 5000 //3s

// Some SSR are active low...
#define LIGHTS_ON     0
#define LIGHTS_OFF    1

// DHT Sensor
#define DHT_PIN       14 //D5
// LED pin
// #define LED_PIN       14 //D5


#define OUT_PINS_COUNT  4
#define IN_PINS_COUNT  4

// ssr control
int OUT_PINS[OUT_PINS_COUNT] = {
  16, // D0 Sandeliuks
  5,  // D1 Koridorius
  4,  // D2 Koridorius prie duru
  0,  // N/A
};

// buttons
int IN_PINS[IN_PINS_COUNT] = {
  12,  // D6 Sandelio mygtukas
  13,  // D7 Vidurinis mygtukas
  3,   // D10 Trecias mygtukas
  1,   // D9
};

#endif


#endif
