#include "config.h"

unsigned long debounceLastTime = 0;
unsigned long pirStartTime = 0;
unsigned long pirTimeout = PIR_TIMEOUT;
unsigned long ledsOffTime = 0;


void init_io(){

    pinMode(IN_PINS[0], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(IN_PINS[0]), interrupt, FALLING);

    pinMode(IN_PINS[1], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(IN_PINS[1]), interrupt, FALLING);

    pinMode(IN_PINS[2], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(IN_PINS[2]), interrupt, FALLING);

#if defined(KORIDORIUS) || defined(VONIA)
    pinMode(IN_PINS[3], INPUT);
    attachInterrupt(digitalPinToInterrupt(IN_PINS[3]), interrupt, RISING);
#else
    pinMode(IN_PINS[3], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(IN_PINS[3]), interrupt, FALLING);
#endif

    // pinMode(LED_PIN, OUTPUT);
    // digitalWrite(LED_PIN, HIGH);
    for(int i = 0; i < OUT_PINS_COUNT; i++){
      pinMode(OUT_PINS[i], OUTPUT);
      digitalWrite(OUT_PINS[i], LIGHTS_OFF);
    }


}

void write_output(int pin, int value){

  if(pin >= OUT_PINS_COUNT) return;
  digitalWrite(OUT_PINS[pin], value);

}

int toggle_output(int pin){

  if(pin >= OUT_PINS_COUNT) return 1;
  int val = !digitalRead(OUT_PINS[pin]);
  digitalWrite(OUT_PINS[pin], val);
  return val;

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


void ICACHE_RAM_ATTR interrupt(void){

  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - debounceLastTime > DEBOUNCE_DELAY_MS)
  {
    if(digitalRead(IN_PINS[0]) == LOW){
      int val = toggle_output(0);
#ifdef VONIA
      write_output(1, val); //toggle leds
      write_output(2, val); //toggle fan
      if(val == LIGHTS_OFF){
        write_output(3, LIGHTS_OFF); //turn off mirror
        ledsOffTime = millis(); //save off time for PIR "debounce"
      }
#endif
    }
    if(digitalRead(IN_PINS[1]) == LOW){
      toggle_output(2);
    }
    if(digitalRead(IN_PINS[2]) == LOW){
      int val = toggle_output(1);
      pirStartTime = 0;
      if(val == LIGHTS_OFF)
        ledsOffTime = millis(); //save off time for PIR "debounce"

    }
    else if(digitalRead(IN_PINS[3]) == HIGH){
      if(millis() - ledsOffTime > PIR_DEBOUNCE_TIMEOUT){
        write_output(1, LIGHTS_ON);
        pirStartTime = millis();
      }
    }
  }
  debounceLastTime = interrupt_time;
}

void check_io_timeouts(){
  //leds are on & main light is off & timeout is active & timeout occured
  if(digitalRead(OUT_PINS[1]) && !digitalRead(OUT_PINS[0]) &&
  ((millis() - pirStartTime) > pirTimeout) && pirStartTime > 0){
      write_output(1, LIGHTS_OFF);
      ledsOffTime = millis(); //save off time for PIR "debounce"
  }
}

//
// void ICACHE_RAM_ATTR interrupt3(void){
//
//   noInterrupts();
//   unsigned long interrupt_time = millis();
//
//   // If interrupts come faster than 200ms, assume it's a bounce and ignore
//   if (interrupt_time - DebounceLastTime[2] > DEBOUNCE_DELAY_MS)
//   {
//     #ifdef VONIA
//     //PIR interrupt
//     bool pir_value = digitalRead(IN_PINS[3]);
//     bool main_lights_value = digitalRead(OUT_PINS[0]);
//     if(main_lights_value == LIGHTS_OFF) //i
//       write_output(1, pir_value);
//     #endif
//   }
//   DebounceLastTime[2] = interrupt_time;
//   interrupts();
// }
