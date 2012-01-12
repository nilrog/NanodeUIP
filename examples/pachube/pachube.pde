#include <NanodeUNIO.h>
#include <NanodeUIP.h>
#include <psock.h>
#include "webclient.h"
#include "printf.h"

#undef PSTR
#define PSTR(s) (__extension__({static const char __c[] __attribute__ (( section (".progmem") )) = (s); &__c[0];}))

enum app_states_e { state_none = 0, state_needip, state_needresolv, 
  state_noconnection, state_connecting, state_waiting, state_done, state_invalid };

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

// Returns a CSV file, suitable for posting to pachube.
// In this example, the 'reading' is the current system time adn the free
// memory.
static void take_reading(char* buf, size_t len)
{
  snprintf_P(buf,len,PSTR("1,%lu\r\n2,%u\r\n"),millis(),SP-(uint16_t)__brkval); 
}
	
uint16_t num_samples_remaining;
struct timer send_sample_timer;

void setup() {
  Serial.begin(38400);
  printf_begin();
  printf_P(PSTR(__FILE__"\r\n"));
  printf_P(PSTR("FREE: %u\r\n"),SP-(uint16_t)__brkval);

#if 1  // Nanode
  byte macaddr[6];
  NanodeUNIO unio(NANODE_MAC_DEVICE);
  unio.read(macaddr,NANODE_MAC_ADDRESS,6);
  uip.init(macaddr);
#endif
#if 0 // EtherShield
  byte macaddr[6] = { 0x2, 0x00, 0x00, 0x1, 0x2, 0x3 }; 
  uip.init(macaddr,10);
#endif

  char buf[20];
  uip.get_mac_str(buf);
  printf_P(PSTR("MAC: %s\r\n"),buf);
  uip.wait_for_link();
  nanode_log_P(PSTR("Link is up"));
  uip.start_dhcp(dhcp_status);
  uip.init_resolv(resolv_found);
  app_state = state_needip;
  
  // We'll send a sample every so often
  timer_set(&send_sample_timer, CLOCK_SECOND * 2);
  
  printf_P(PSTR("+READY\r\n"));
  printf_P(PSTR("How many readings to send, or '0' to send lots?\r\n"));
  while ( !Serial.available() );
  char c = Serial.read();
  if ( isdigit(c) && c > '0' )
    num_samples_remaining = c - '0';
  else
    num_samples_remaining = -1;
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
	timer_restart(&send_sample_timer);
      }
      break;
    case state_noconnection:
      {
	// Try to connect
	nanode_log_P(PSTR("Starting pachube put..."));
	webclient_init();

	// Send a 'reading' 
	char reading_buffer[25];
	take_reading(reading_buffer,sizeof(reading_buffer));
	webclient_put_P(PSTR("api.pachube.com"), 80, PSTR("/v2/feeds/33735.csv"), pachube_api_key, reading_buffer);
	app_state = state_connecting;
      }
      break;
    case state_done:
      printf_P(PSTR("+OK\r\n"));
      app_state = state_none;
      break;
    case state_waiting:
      if ( timer_expired(&send_sample_timer) )
      {
	app_state = state_noconnection;
	timer_reset(&send_sample_timer);
      }
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
void webclient_datahandler(char *data, u16_t len)
{
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
    if ( --num_samples_remaining )
      app_state = state_waiting;
    else
      app_state = state_done;
  }
}

/****************************************************************************/
void webclient_connected(void)
{
  uip_log_P(PSTR("webclient_connected"));
}

/****************************************************************************/
void webclient_timedout(void)
{
  uip_log_P(PSTR("webclient_timedout\r\n"));
  app_state = state_noconnection;
}

/****************************************************************************/
void webclient_aborted(void)
{
  uip_log_P(PSTR("webclient_aborted\r\n"));
  app_state = state_noconnection;
}

/****************************************************************************/
void webclient_closed(void)
{
  uip_log_P(PSTR("webclient_closed\r\n"));
  app_state = state_noconnection;
}

// vim:cin:ai:sts=2 sw=2 ft=cpp
// vim:cin:ai:sts=2 sw=2 ft=cpp
