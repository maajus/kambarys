#include "config.h"

unsigned long debounceLastTime = 0;
unsigned long pirStartTime = 0;
unsigned long ledsOffTime = 0;
#ifdef VONIA
unsigned long pirTimeout = PIR_TIMEOUT;
unsigned long pirDebounce = PIR_DEBOUNCE_TIMEOUT;
#endif



void init_io(){

    pinMode(IN_PINS[0], INPUT_PULLUP);
    attachInterrupt(IN_PINS[0], interrupt1, FALLING);

    pinMode(IN_PINS[1], INPUT_PULLUP);
    attachInterrupt(IN_PINS[1], interrupt2, FALLING);

    pinMode(IN_PINS[2], INPUT_PULLUP);
    attachInterrupt((IN_PINS[2]), interrupt3, FALLING);

#if defined(KORIDORIUS) || defined(VONIA)
    pinMode(IN_PINS[3], INPUT);
    attachInterrupt(digitalPinToInterrupt(IN_PINS[3]), interrupt4, RISING);
#else
    pinMode(IN_PINS[3], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(IN_PINS[3]), interrupt4, FALLING);
#endif

    // pinMode(LED_PIN, OUTPUT);
    // digitalWrite(LED_PIN, HIGH);
    for(int i = 0; i < OUT_PINS_COUNT; i++){
      pinMode(OUT_PINS[i], OUTPUT);
      digitalWrite(OUT_PINS[i], LIGHTS_OFF);
    }


}



int read_output(int pin){

  if(pin >= OUT_PINS_COUNT) return 0;
  int ret = digitalRead(OUT_PINS[pin]);
  if(ret == LIGHTS_ON) return LIGHTS_ON;
  else return LIGHTS_OFF;

}

void write_output(int pin, int value){

  if(pin >= OUT_PINS_COUNT) return;
  digitalWrite(OUT_PINS[pin], value);

}

int toggle_output(int pin){

  if(pin >= OUT_PINS_COUNT) return 1;
  int val = !digitalRead(OUT_PINS[pin]);
  digitalWrite(OUT_PINS[pin], val);
  if(val == LIGHTS_ON) return LIGHTS_ON;
  else return LIGHTS_OFF;

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


ICACHE_RAM_ATTR void interrupt1(void){

  noInterrupts();
  // tcp_write_string("int1\n");
  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if ((interrupt_time - debounceLastTime) > DEBOUNCE_DELAY_MS)
  {
      toggle_output(0);
      int val = read_output(0);
      debounceLastTime = interrupt_time;
#ifdef VONIA
      write_output(1, val); //toggle leds
      write_output(2, val); //toggle fan
      if(val == LIGHTS_OFF){
        write_output(3, LIGHTS_OFF); //turn off mirror
        ledsOffTime = millis(); //save off time for PIR "debounce"
      }
#endif

  }
  interrupts();
}


ICACHE_RAM_ATTR void interrupt2(void){

  noInterrupts();
  // tcp_write_string("int2\n");
  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if ((interrupt_time - debounceLastTime) > DEBOUNCE_DELAY_MS)
  {
      toggle_output(2);
      debounceLastTime = interrupt_time;

  }
  interrupts();
}


ICACHE_RAM_ATTR void interrupt3(void){

  noInterrupts();
  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if ((interrupt_time - debounceLastTime) > DEBOUNCE_DELAY_MS)
  {
    int val = toggle_output(1);
    pirStartTime = 0;
    if(val == LIGHTS_OFF)
    ledsOffTime = millis(); //save off time for PIR "debounce"
    debounceLastTime = interrupt_time;
  }
  interrupts();
}


ICACHE_RAM_ATTR void interrupt4(void){

  noInterrupts();
  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if ((interrupt_time - debounceLastTime) > DEBOUNCE_DELAY_MS)
  {

#if defined(VONIA) || defined(KORIDORIUS)
    if(millis() - ledsOffTime > pirDebounce){
      write_output(1, LIGHTS_ON);
      pirStartTime = millis();
    }
  #else
    int val = toggle_output(3);
#endif


    debounceLastTime = interrupt_time;
  }
  interrupts();
}


#if defined(VONIA) || defined(KORIDORIUS)
void check_io_timeouts(){
  //leds are on & main light is off & timeout is active & timeout occured
  if(read_output(1) == LIGHTS_ON && read_output(0) == LIGHTS_OFF &&
  ((pirStartTime + pirTimeout) < millis())){
      write_output(1, LIGHTS_OFF);
      debounceLastTime = millis();
      ledsOffTime = millis(); //save off time for PIR "debounce"
  }
}

#endif
