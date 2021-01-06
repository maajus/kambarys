
#include "config.h"





#ifdef VONIA

unsigned long ledsOffTime = 0;
unsigned long pirTimeout = PIR_TIMEOUT;
unsigned long pirDebounce = PIR_DEBOUNCE_TIMEOUT;
unsigned long pirStartTime = 0;

void function0() {

  toggle_output(0);
  int val = read_output(0);
  write_output(1, val); //toggle leds
  write_output(2, val); //toggle fan
  if(val == LIGHTS_OFF){
    write_output(3, LIGHTS_OFF); //turn off mirror
    ledsOffTime = millis(); //save off time for PIR "debounce"
  }
}

void function1() {
  toggle_output(2);
}

void function2() {
  int val = toggle_output(1);
  pirStartTime = 0;
  if(val == LIGHTS_OFF)
  ledsOffTime = millis(); //save off time for PIR "debounce"
}

void function3() {

  if(millis() - ledsOffTime > pirDebounce){
    write_output(1, LIGHTS_ON);
    pirStartTime = millis();
  }

}

void check_io_timeouts(){
  //leds are on & main light is off & timeout is active & timeout occured
  if(read_output(1) == LIGHTS_ON && read_output(0) == LIGHTS_OFF &&
  !digitalRead(IN_PINS[3]) &&
  ((pirStartTime + pirTimeout) < millis()) && pirStartTime > 0){
      write_output(1, LIGHTS_OFF);
      ledsOffTime = millis(); //save off time for PIR "debounce"
  }
}
#endif

#ifdef KORIDORIUS

unsigned long pirTimeout = PIR_TIMEOUT;
unsigned long pirDebounce = PIR_DEBOUNCE_TIMEOUT;
unsigned long pirStartTime = 0;

void function0() {
  toggle_output(0);
}

void function1() {
  toggle_output(1);
}

void function2() {
  toggle_output(2);
}

void function3() {
  write_output(0, LIGHTS_ON);
  pirStartTime = millis();
}

void check_io_timeouts(){
  //leds are on & main light is off & timeout is active & timeout occured
  if(read_output(0) == LIGHTS_ON  && digitalRead(IN_PINS[3]) &&
  ((pirStartTime + pirTimeout) < millis()) && pirStartTime > 0){
      write_output(0, LIGHTS_OFF);
      pirStartTime = 0;
      ledsOffTime = millis(); //save off time for PIR "debounce"
  }
}
#endif



#ifdef MIEGAMASIS

void function0() {
  toggle_output(0);
}

void function1() {
  toggle_output(2);
}

void function2() {
  toggle_output(1);
}

void function3() {
  toggle_output(3);
}

#endif
