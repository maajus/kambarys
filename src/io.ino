#include "config.h"

unsigned long DebounceLastTime[IN_PINS_COUNT] = {0};
bool io_inited = false;

//
// --buttons
// B0 = 6
// B1 = 7
// B2 = 9 -- TX pin
// B3 = 10 -- RX pin
//
// -- DHT senor data pin
// dht_pin = 4
//
// -- LED pin
// LED = 5
//


void init_io(){

  if(io_inited) return;
    pinMode(IN_PINS[0], INPUT_PULLUP);
    attachInterrupt(IN_PINS[0], interrupt0, FALLING);

    pinMode(IN_PINS[1], INPUT_PULLUP);
    attachInterrupt(IN_PINS[1], interrupt1, FALLING);

    pinMode(IN_PINS[2], INPUT_PULLUP);
    attachInterrupt(IN_PINS[2], interrupt2, CHANGE);

#if defined(KORIDORIUS) || defined(VONIA)
    pinMode(IN_PINS[3], INPUT);
    attachInterrupt(IN_PINS[3], interrupt3, RISING);
#else
    pinMode(IN_PINS[3], INPUT_PULLUP);
    attachInterrupt(IN_PINS[3], interrupt3, FALLING);
#endif

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    for(int i = 0; i < OUT_PINS_COUNT; i++){
      pinMode(OUT_PINS[i], OUTPUT);
      digitalWrite(OUT_PINS[i], LIGHTS_OFF);
    }

    io_inited = true;

}

void write_output(int pin, int value){

  if(pin >= OUT_PINS_COUNT) return;
  digitalWrite(OUT_PINS[pin], value);

}

int toggle_output(int pin){

  if(pin >= OUT_PINS_COUNT) return 1;
  int val = digitalRead(OUT_PINS[pin]);
  digitalWrite(OUT_PINS[pin], !val);
  return !val;

}


int write_all(int val){

  for(int i = 0; i < OUT_PINS_COUNT; i++){
    digitalWrite(OUT_PINS[i], val);
  }
  return val;
}



int toggle_all(int val){

  int some_out_high = 0;

  for(int i = 0; i < OUT_PINS_COUNT; i++){
    if(digitalRead(OUT_PINS[i]))
      some_out_high = 1;
  }

  for(int i = 0; i < OUT_PINS_COUNT; i++){
    digitalWrite(OUT_PINS[i], !some_out_high);
  }

  return some_out_high;
}


void ICACHE_RAM_ATTR interrupt0(void){

  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - DebounceLastTime[0] > DEBOUNCE_DELAY_MS)
  {
    int val = toggle_output(0);
    #ifdef VONIA
    write_output(1, val); //toggle leds
    write_output(2, val); //toggle fan
    if(val == LIGHTS_OFF)
      write_output(3, LIGHTS_OFF); //turn off mirror
    #endif
  }
  DebounceLastTime[0] = interrupt_time;
}

void ICACHE_RAM_ATTR interrupt1(void){

  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - DebounceLastTime[1] > DEBOUNCE_DELAY_MS)
  {
    #ifdef VONIA
    //toggle only if lights are on or to turn off only
    if(digitalRead(OUT_PINS[0]) || digitalRead(OUT_PINS[3]) )
    toggle_output(3);
    #endif
  }
  DebounceLastTime[1] = interrupt_time;
}

void ICACHE_RAM_ATTR interrupt2(void){

  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - DebounceLastTime[2] > DEBOUNCE_DELAY_MS)
  {
    #ifdef VONIA
    //PIR interrupt
    bool pir_value = digitalRead(IN_PINS[2]);
    bool main_lights_value = digitalRead(OUT_PINS[0]);
    if(!main_lights_value) //i
      write_output(OUT_PINS[1], pir_value);
    #endif
  }
  DebounceLastTime[2] = interrupt_time;
}

void ICACHE_RAM_ATTR interrupt3(void){

  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - DebounceLastTime[3] > DEBOUNCE_DELAY_MS)
  {
    toggle_output(1);
  }
  DebounceLastTime[3] = interrupt_time;
}
