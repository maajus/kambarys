#include "config.h"

unsigned long intStartTime = 0;



void init_io(){

    pinMode(IN_PINS[0], INPUT_PULLUP);
    attachInterrupt(IN_PINS[0], interrupt0, FALLING);

#ifndef DEBUG
    pinMode(IN_PINS[1], INPUT_PULLUP);
    attachInterrupt(IN_PINS[1], interrupt0, FALLING);

    pinMode(IN_PINS[2], INPUT_PULLUP);
    attachInterrupt((IN_PINS[2]), interrupt0, FALLING);

#ifdef VONIA
    pinMode(IN_PINS[3], INPUT);
    attachInterrupt(digitalPinToInterrupt(IN_PINS[3]), interrupt0, RISING);
#endif
#ifdef KORIDORIUS
    pinMode(IN_PINS[3], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(IN_PINS[3]), interrupt0, FALLING);
#endif
#ifdef MIEGAMASIS
    pinMode(IN_PINS[3], INPUT_PULLUP);
    attachInterrupt(IN_PINS[3], interrupt0, FALLING);
#endif

 #endif
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






ICACHE_RAM_ATTR void interrupt0(void){

    noInterrupts();
    unsigned long now = millis();
    int32_t delta = now - intStartTime;
    if(delta < 0) delta += 0xFFFFFFFF;
    if(delta > CLICK_MIN_DURATION){

        //falling edge
        if(!digitalRead(IN_PINS[0])){
            function0();
        }
        if(!digitalRead(IN_PINS[1])){
          function1();
        }
        if(!digitalRead(IN_PINS[2])){
          function2();
        }
#ifdef VONIA
        if(digitalRead(IN_PINS[3])){
          function3();
        }
#else
        if(!digitalRead(IN_PINS[3])){
          function3();
        }
#endif

    }
    intStartTime = now;
    interrupts();

}






/*ICACHE_RAM_ATTR void interrupt0(void){*/

  /*//falling edge*/
  /*if(!digitalRead(IN_PINS[0])){*/
    /*intStartTime0 = millis();*/
  /*}*/
  /*else{ //rising edge*/
    /*if(!intStartTime0) return;*/
    /*unsigned long now = millis();*/
    /*int32_t delta = now - intStartTime0;*/
    /*if(delta < 0) delta += 0xFFFFFFFF;*/

    /*if(delta > CLICK_MIN_DURATION && delta < CLICK_MAX_DURATION){*/
      /*function0();*/
    /*}*/
    /*intStartTime0 = 0;*/
  /*}*/

/*}*/




/*ICACHE_RAM_ATTR void interrupt1(void){*/

  /*//falling edge*/
  /*if(!digitalRead(IN_PINS[1])){*/
    /*intStartTime1 = millis();*/
  /*}*/
  /*//rising edge*/
  /*else{*/
    /*if(!intStartTime1) return;*/
    /*unsigned long now = millis();*/
    /*int32_t delta = now - intStartTime1;*/
    /*if(delta < 0) delta += 0xFFFFFFFF;*/

    /*if(delta > CLICK_MIN_DURATION && delta < CLICK_MAX_DURATION){*/
      /*function1();*/
    /*}*/
    /*intStartTime1 = 0;*/
  /*}*/

/*}*/



/*ICACHE_RAM_ATTR void interrupt2(void){*/

  /*//falling edge*/
  /*if(!digitalRead(IN_PINS[2])){*/
    /*intStartTime2 = millis();*/
  /*}*/
  /*//rising edge*/
  /*else{*/
    /*if(!intStartTime2) return;*/
    /*unsigned long now = millis();*/
    /*int32_t delta = now - intStartTime2;*/
    /*if(delta < 0) delta += 0xFFFFFFFF;*/

    /*if(delta > CLICK_MIN_DURATION && delta < CLICK_MAX_DURATION){*/
      /*function2();*/
    /*}*/
    /*intStartTime2 = 0;*/
  /*}*/

/*}*/


/*ICACHE_RAM_ATTR void interrupt3(void){*/

      /*function3();*/
/*}*/
