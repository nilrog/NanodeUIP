#include <avr/pgmspace.h>
#include "clock.h"
#include <uip-conf.h>

#if ARDUINO >= 100
  #include <Arduino.h> // Arduino 1.0
#else
  #include <WProgram.h> // Arduino 0022
#endif

#define uip_log printf
#define uip_log_P printf_P

void clock_init(void) {}

clock_time_t clock_time(void) {
  return millis();
}

void nullproc(void) {}
