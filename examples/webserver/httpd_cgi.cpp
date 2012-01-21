/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/*
 * Copyright (c) 2001-2006, Adam Dunkels.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "uip.h"
#include "psock.h"
#include "httpd.h"
#include "httpd_cgi.h"
#include "httpd_fs.h"

#include <stdio.h>
#include <string.h>

#include <NanodeUIP.h>
#include <Arduino.h>

HTTPD_CGI_CALL(digital, "digital-pins", digital_pins);
HTTPD_CGI_CALL(analog, "analog-pins", analog_pins);
HTTPD_CGI_CALL(cgi_toggle_pin, "toggle_pin", toggle_pin);

static const struct httpd_cgi_call *calls[] = { &digital, &analog, &cgi_toggle_pin, NULL };

/*---------------------------------------------------------------------------*/
static
PT_THREAD(nullfunction(struct httpd_state *s, char *))
{
  PSOCK_BEGIN(&s->sout);
  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
httpd_cgifunction
httpd_cgi(char *name)
{
  printf_P(PSTR("CGI: %S\r\n"),name);

  const struct httpd_cgi_call **f;

  /* Find the matching name in the table, return the function. */
  for(f = calls; *f != NULL; ++f) {
    if(strncmp_P((*f)->name, name, strlen((*f)->name)) == 0) {
      return (*f)->function;
    }
  }
  return nullfunction;
}

/*---------------------------------------------------------------------------*/
int getPinMode(uint8_t pin)
{
	int result = -1;

	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);

	if (port != NOT_A_PIN)
	{
		volatile uint8_t *reg = portModeRegister(port);

		if ( *reg & bit )
			result = OUTPUT;
		else
			result = INPUT; 
	}
	return result;
}

/*---------------------------------------------------------------------------*/

static const char str_high[] PROGMEM = "HIGH";
static const char str_low[] PROGMEM = "LOW";
static const char str_input[] PROGMEM = "INPUT";
static const char str_output[] PROGMEM = "OUTPUT";
static const char str_empty[] PROGMEM = "";
static const char str_notfound[] PROGMEM = "Not found";

static unsigned short
generate_digital_pins(void *arg)
{
  struct httpd_state *s = (struct httpd_state *)arg;
   
  char* buf = (char*)uip_appdata;
  size_t len = UIP_APPDATA_SIZE;
  size_t written;

  written = snprintf_P(buf,len,
		 PSTR("<tr><td>%u</td><td>%S</td><td id=\"p%u\">%S</td><td>"),
		 s->count,
		 getPinMode(s->count)?str_output:str_input,
		 s->count,
		 digitalRead(s->count)?str_high:str_low
  );
  buf += written;
  len -= written;
 
  if ( getPinMode(s->count) )
  { 
  written = snprintf_P(buf,len,
		 PSTR("<button type=\"button\" onclick=\"toggle_pin(%u)\">Toggle</button>"),
		 s->count
  );
  buf += written;
  len -= written;
  }

  written = snprintf_P(buf,len,
		 PSTR("</td></tr>\r\n")
  );
  buf += written;
  len -= written;
  
  return UIP_APPDATA_SIZE - len;
}
static
PT_THREAD(digital_pins(struct httpd_state *s, char *))
{
  
  PSOCK_BEGIN(&s->sout);

  for(s->count = 0; s->count < NUM_DIGITAL_PINS; ++s->count) {
    PSOCK_GENERATOR_SEND(&s->sout, generate_digital_pins, s);
  }

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static unsigned short
generate_analog_pins(void *arg)
{
  struct httpd_state *s = (struct httpd_state *)arg;
    
  return snprintf_P((char *)uip_appdata, UIP_APPDATA_SIZE,
		 PSTR("<tr><td>%u</td><td>%u</td></tr>\r\n"),
		 s->count,
		 analogRead(s->count)
  );
}
static
PT_THREAD(analog_pins(struct httpd_state *s, char *))
{
  
  PSOCK_BEGIN(&s->sout);

  for(s->count = 0; s->count < NUM_ANALOG_INPUTS; ++s->count) {
    PSOCK_GENERATOR_SEND(&s->sout, generate_analog_pins, s);
  }

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
static
PT_THREAD(toggle_pin(struct httpd_state *s, char *))
{
  char* pin_at;
  const char* response = str_notfound;

  PSOCK_BEGIN(&s->sout);
  // Take action

  nanode_log(s->filename);
  pin_at = strstr(s->filename,"pin=");
  if ( pin_at )
  {
    uint8_t pin = atoi(pin_at+4);
    digitalWrite(pin,digitalRead(pin)^HIGH);
    
    response = digitalRead(pin)?str_high:str_low;
  }
  
    // Send response
  PSOCK_SEND_P(&s->sout,(uint8_t*)response,strlen_P(response));

  PSOCK_END(&s->sout);
}
/*---------------------------------------------------------------------------*/
/** @} */
// vim:cin:ai:sts=2 sw=2 ft=cpp
