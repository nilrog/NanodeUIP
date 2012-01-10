#include <NanodeUNIO.h>
#include <NanodeUIP.h>
#include <psock.h>
#include "webclient.h"
#include "printf.h"

#undef PSTR
#define PSTR(s) (__extension__({static const char __c[] __attribute__ (( section (".progmem") )) = (s); &__c[0];}))

void dhcp_status(int s,const uint16_t *) {
  char buf[20]="IP:";
  if (s==DHCP_STATUS_OK) {
    uip.get_ip_addr_str(buf+3);
    Serial.println(buf);

    nanode_log_P(PSTR("Starting web download..."));
    webclient_init();
    webclient_get_P(PSTR("98.136.240.40"), 80, PSTR("/7159/6645514331_38eb2bdeaa_s.jpg"));
  }
}

extern uint16_t* __brkval;

void setup() {
  char buf[20];
  byte macaddr[6];
  NanodeUNIO unio(NANODE_MAC_DEVICE);

  Serial.begin(38400);
  printf_begin();
  printf_P(PSTR(__FILE__"\r\n"));
  printf_P(PSTR("FREE=%u\r\n"),SP-(uint16_t)__brkval);
  
  unio.read(macaddr,NANODE_MAC_ADDRESS,6);
  uip.init(macaddr);
  uip.get_mac_str(buf);
  Serial.println(buf);
  uip.wait_for_link();
  nanode_log_P(PSTR("Link is up"));
  uip.start_dhcp(dhcp_status);
  nanode_log_P(PSTR("setup() done"));
}

void loop() {
  uip.poll();
}

// Stats
uint32_t size_received = 0;
uint32_t started_at = 0;

/****************************************************************************/
/**
 * Callback function that is called from the webclient code when HTTP
 * data has been received.
 *
 * This function must be implemented by the module that uses the
 * webclient code. The function is called from the webclient module
 * when HTTP data has been received. The function is not called when
 * HTTP headers are received, only for the actual data.
 *
 * \note This function is called many times, repetedly, when data is
 * being received, and not once when all data has been received.
 *
 * \param data A pointer to the data that has been received.
 * \param len The length of the data that has been received.
 */
void webclient_datahandler(char *data, u16_t len)
{
  //printf_P(PSTR("%lu: webclient_datahandler data=%p len=%u\r\n"),millis(),data,len);
  Serial.print('.');

  if ( ! started_at )
    started_at = millis();

  size_received += len;
#if 0
  // Dump out the text
  while(len--)
  {
    char c = *data++;
    if ( c == '\n' )
      Serial.print('\r');
    Serial.print(c);
  }
  Serial.println();
#endif

  // If data is NULL, we are done.  Print the stats
  if (!data)
  {
    Serial.println();
    printf_P(PSTR("%lu: DONE. Received %lu bytes in %lu msec.\r\n"),millis(),size_received,millis()-started_at);
  }
}

/****************************************************************************/
/**
 * Callback function that is called from the webclient code when the
 * HTTP connection has been connected to the web server.
 *
 * This function must be implemented by the module that uses the
 * webclient code.
 */
void webclient_connected(void)
{
  uip_log_P(PSTR("webclient_connected"));
}

/****************************************************************************/
/**
 * Callback function that is called from the webclient code if the
 * HTTP connection to the web server has timed out.
 *
 * This function must be implemented by the module that uses the
 * webclient code.
 */
void webclient_timedout(void)
{
  printf_P(PSTR("webclient_timedout\r\n"));
}

/****************************************************************************/
/**
 * Callback function that is called from the webclient code if the
 * HTTP connection to the web server has been aborted by the web
 * server.
 *
 * This function must be implemented by the module that uses the
 * webclient code.
 */
void webclient_aborted(void)
{
  printf_P(PSTR("webclient_aborted\r\n"));
}

/****************************************************************************/
/**
 * Callback function that is called from the webclient code when the
 * HTTP connection to the web server has been closed.
 *
 * This function must be implemented by the module that uses the
 * webclient code.
 */
void webclient_closed(void)
{
  printf_P(PSTR("webclient_closed\r\n"));
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
// vim:cin:ai:sts=2 sw=2 ft=cpp
