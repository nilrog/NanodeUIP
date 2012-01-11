#include <NanodeUNIO.h>
#include <NanodeUIP.h>
#include <psock.h>
#include "webclient.h"
#include "printf.h"

#undef PSTR
#define PSTR(s) (__extension__({static const char __c[] __attribute__ (( section (".progmem") )) = (s); &__c[0];}))

enum app_states_e { state_none = 0, state_needip, state_needresolv, 
  state_noconnection, state_connecting, state_done, state_invalid };

static app_states_e app_state;

struct app_flags_t
{
  uint8_t have_ip:1;
  uint8_t have_resolv:1;
};

static app_flags_t app_flags;

static const char pachube_api_key[] __attribute__ (( section (".progmem") )) = "X-PachubeApiKey: 8pvNK_06BCBDXtRwq96si4ikFtKZn4rtDjmFoejHOG2iTDQpdXnu3jjMoDSk_E5_CRVMtjql79Jbz-4CT9HMR1Bs3LpqsV_sHKzmjuAM00Y574bHA3zGlarGhrmj9cFS";

static void dhcp_status(int s,const uint16_t *dnsaddr) {
  char buf[20];
  if (s==DHCP_STATUS_OK) {
    resolv_conf(dnsaddr);
    uip.get_ip_addr_str(buf);
    printf_P(PSTR("%lu: IP: %s\r\n"),millis(),buf);
    app_flags.have_ip = 1;
  }
}

static void resolv_found(char *name,uint16_t *addr) {
  char buf[20];
  uip.format_ipaddr(buf,addr);
  printf_P(PSTR("%lu: DNS: %s has address %s\r\n"),millis(),name,buf);
  app_flags.have_resolv = 1;
}

extern uint16_t* __brkval;

void setup() {
  char buf[20];
  byte macaddr[6];
  NanodeUNIO unio(NANODE_MAC_DEVICE);

  Serial.begin(38400);
  printf_begin();
  printf_P(PSTR(__FILE__"\r\n"));
  printf_P(PSTR("FREE: %u\r\n"),SP-(uint16_t)__brkval);
  
  unio.read(macaddr,NANODE_MAC_ADDRESS,6);
  uip.init(macaddr);
  uip.get_mac_str(buf);
  printf_P(PSTR("MAC: %s\r\n"),buf);
  uip.wait_for_link();
  nanode_log_P(PSTR("Link is up"));
  printf_P(PSTR("+READY\r\n"));
  uip.start_dhcp(dhcp_status);
  uip.init_resolv(resolv_found);
  app_state = state_needip;
}

void loop() {
  uip.poll();

  switch (app_state)
  {
    case state_needip:
      if ( app_flags.have_ip )
      {
	// launch resolver
	uip.query_name("api.pachube.com");
	app_state = state_needresolv;
      }
      break;
    case state_needresolv:
      if ( app_flags.have_resolv )
      {
	app_state = state_noconnection;
      }
      break;
    case state_noconnection:
      {
	// Try to connect
	nanode_log_P(PSTR("Starting pachube put..."));
	webclient_init();

	// Log the current app time and free memory
	char put_values[25];
	snprintf_P(put_values,sizeof(put_values),PSTR("1,%lu\r\n2,%u\r\n"),millis(),SP-(uint16_t)__brkval); 
	webclient_put_P(PSTR("api.pachube.com"), 80, PSTR("/v2/feeds/33735.csv"), pachube_api_key, put_values);
	app_state = state_connecting;
      }
      break;
    case state_done:
      printf_P(PSTR("+OK\r\n"));
      app_state = state_none;
      break;
    case state_connecting:
    case state_none:
    case state_invalid:
      break;
  }
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

  if ( len )
  {
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
#else  
  Serial.print('.');
#endif
  } 

  // If data is NULL, we are done.  Print the stats
  if (!data && size_received)
  {
    Serial.println();
    printf_P(PSTR("%lu: DONE. Received %lu bytes in %lu msec.\r\n"),millis(),size_received,millis()-started_at);
    size_received = 0;
    started_at = 0;
    app_state = state_done;
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
  uip_log_P(PSTR("webclient_timedout\r\n"));
  app_state = state_noconnection;
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
  uip_log_P(PSTR("webclient_aborted\r\n"));
  app_state = state_noconnection;
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
  uip_log_P(PSTR("webclient_closed\r\n"));
  app_state = state_noconnection;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
// vim:cin:ai:sts=2 sw=2 ft=cpp
