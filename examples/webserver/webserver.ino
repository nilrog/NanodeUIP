/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * @file webserver.ino
 *
 * This example shows how to use NanodeUIP to run a web server.
 * It will show the current state of all digital and analog
 * pins, and allow you to change the state of digital pins
 * which are configured as outputs.
 */

//#define ETHERSHIELD // uncomment to run on EtherShield
#include <NanodeUIP.h>
#include <psock.h>
#include "httpd.h"
#include "printf.h"
#ifndef ETHERSHIELD
#include <NanodeUNIO.h>
#endif

// This is the board I test with.  You probably don't have it, so
// how about using these LED pins instead?
#ifndef PINS_MIGHTY_EXPERIMENTER_V2
const int led_red = 6;
const int led_yellow = 5;
const int led_green = 4;
#endif

UIPASSERT(sizeof(struct httpd_state)<=TCP_APP_STATE_SIZE)

enum app_states_e { state_none = 0, state_needip, state_waiting, state_invalid };

static app_states_e app_state = state_needip;

struct app_flags_t
{
  uint8_t have_ip:1;
  uint8_t have_resolv:1;
};

static app_flags_t app_flags;

static void dhcp_status(int s,const uint16_t *dnsaddr)
{
  char buf[20];
  if (s==DHCP_STATUS_OK)
  {
    resolv_conf(dnsaddr);
    uip.get_ip_addr_str(buf);
    printf_P(PSTR("%lu: IP:%s\r\n"),millis(),buf);
    app_flags.have_ip = 1;
  }
}

void setup_pins()
{
  // Turn all ditigal-only pins to INPUTS and set the pullup.  This is for consistency
  // so that the web page always looks right.
  int i = NUM_DIGITAL_PINS - NUM_ANALOG_INPUTS;
  while (i--)
  {
    pinMode(i,INPUT);
    digitalWrite(i,HIGH);
  }

  // led's are outputs
  pinMode(led_red,OUTPUT);
  pinMode(led_yellow,OUTPUT);
  pinMode(led_green,OUTPUT);
}

extern uint16_t* __brkval;

void setup()
{

  // Set pins first, so that later inits() can override
  setup_pins();

#ifdef ETHERSHIELD // EtherShield
  byte macaddr[6] = { 0x2, 0x00, 0x00, 0x1, 0x2, 0x3 };
  uip.init(macaddr,SS);
#else  // Nanode
  byte macaddr[6];
  NanodeUNIO unio(NANODE_MAC_DEVICE);
  unio.read(macaddr,NANODE_MAC_ADDRESS,6);
  uip.init(macaddr);
#endif

  Serial.begin(38400);
  printf_begin();
  printf_P(PSTR(__FILE__"\r\n"));
  printf_P(PSTR("FREE: %u\r\n"),SP-(uint16_t)__brkval);

  char buf[20];
  uip.get_mac_str(buf);
  Serial.println(buf);
  uip.wait_for_link();
  nanode_log_P(PSTR("Link is up"));
  uip.start_dhcp(dhcp_status);
  nanode_log_P(PSTR("setup() done"));
}

void loop()
{
  uip.poll();

  // State is pretty simple in this app, we just sit here and
  // listen for http connections
  switch (app_state)
  {
  case state_needip:
    if ( app_flags.have_ip )
    {
      app_state = state_waiting;
      httpd_init();
      printf_P(PSTR("+READY"));
    }
    break;
  case state_waiting:
  case state_none:
  case state_invalid:
    break;
  }
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
