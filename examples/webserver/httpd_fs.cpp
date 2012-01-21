/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <NanodeUIP.h>
#include "uip.h"
#include "httpd_fs.h"

static const char index_shtml_name[] PROGMEM = "/index.shtml";
static const char index_shtml[] PROGMEM = 
"%!: /header.html\r\n"
"<h2>Home</h2>\r\n"
"<p><a href=\"digital.shtml\">View Digital Pins</a>\r\n"
"<p><a href=\"analog.shtml\">View Analog Pins</a>\r\n"
"%!: /footer.html\r\n"
;

static const char header_html_name[] PROGMEM = "/header.html";
static const char header_html[] PROGMEM = 
"<html>\r\n"
"<head>\r\n"
"<script type=\"text/javascript\">\r\n"
"function toggle_pin(pin)\r\n"
"{\r\n"
"\txmlhttp=new XMLHttpRequest();\r\n"
"\txmlhttp.onreadystatechange=function()\r\n"
"\t{\r\n"
"\t\tif (xmlhttp.readyState==4 && xmlhttp.status==200)\r\n"
"\t\t{\r\n"
"\t\t\tdocument.getElementById(\"p\"+pin).innerHTML=xmlhttp.responseText;\r\n"
"\t\t}\r\n"
"\t}\r\n"
"\txmlhttp.open(\"GET\",\"/toggle_pin.shtml?pin=\"+pin,true);\r\n"
"\txmlhttp.send();\r\n"
"}\r\n"
"</script>\r\n"
"</head>\r\n" 
"<body>\r\n"
"<h1>NanodeUIP</h1>\r\n"
"<p>[ <a href=\"/index.shtml\">Home</a> | <a href=\"/digital.shtml\">Digital</a> | <a href=\"analog.shtml\">Analog</a> ]</p>\r\n"
;

static const char footer_html_name[] PROGMEM = "/footer.html";
static const char footer_html[] PROGMEM = 
"</body>\r\n"
"</html>\r\n"
;

static const char digital_shtml_name[] PROGMEM = "/digital.shtml";
static const char digital_shtml[] PROGMEM =
"%!: /header.html\r\n"
"<h2>Digital Pins</h2>\r\n"
"<table><tr><th>Pin#</th><th>Pin Mode</th><th>State</th><th></th></tr>\r\n"
"%! digital-pins\r\n"
"</table>\r\n"
"%!: /footer.html\r\n"
;

static const char analog_shtml_name[] PROGMEM = "/analog.shtml";
static const char analog_shtml[] PROGMEM =
"%!: /header.html\r\n"
"<h2>Analog Pins</h2>\r\n"
"<table><tr><th>Input#</th><th>Value</th></tr>\r\n"
"%! analog-pins\r\n"
"</table>\r\n"
"%!: /footer.html\r\n"
;

static const char ok_txt_name[] PROGMEM = "/ok.txt";
static const char ok_txt[] PROGMEM =
"+OK\r\n"
;

static const char toggle_pin_shtml_name[] PROGMEM = "/toggle_pin.shtml";
static const char toggle_pin_shtml[] PROGMEM =
"%! toggle_pin\r\n"
;

int strncmp_PP(const char *ch1, const char *ch2, size_t len)
{
  char c1=0,c2=0;
  while (len--)
  {
    c1 = pgm_read_byte(ch1++);
    c2 = pgm_read_byte(ch2++);
    if ( c1 != c2 )
      break;
    if(!c1 && !c2)
      return (0);
    if(!c1 || !c2)
      break;
  } 
  return (c1 -c2);
}

struct entry_t
{
  const char* filename;
  const char* data;
  size_t len;
};
struct entry
{
  char* filename;
  char* data;
  size_t len;
  entry(const void* data)
  {
    memcpy_P(this,data,sizeof(entry_t));
  }
  bool matches(const char* name) const
  {
    return ( name && filename && !strncmp_P(name,filename,strlen_P(filename)) );
  }
  bool matches_P(const char* name) const
  {
    return ( name && filename && !strncmp_PP(name,filename,strlen_P(filename)) );
  }
  void result(struct httpd_fs_file *file) const
  {
    file->data = data;
    file->len = len;
  }
  bool isvalid(void) const
  {
    return filename != 0;
  }
};

static const entry_t dir[] PROGMEM =
{
  { index_shtml_name, index_shtml, sizeof(index_shtml) },
  { header_html_name, header_html, sizeof(header_html) },
  { footer_html_name, footer_html, sizeof(footer_html) },
  { analog_shtml_name, analog_shtml, sizeof(analog_shtml) },
  { digital_shtml_name, digital_shtml, sizeof(digital_shtml) },
  { ok_txt_name, ok_txt, sizeof(ok_txt) },
  { toggle_pin_shtml_name, toggle_pin_shtml, sizeof(toggle_pin_shtml) },
  { 0,0,0 }
};

// Note that 'filename' might not be zero-terminated!
int httpd_fs_open(const char *filename, struct httpd_fs_file *file)
{
  nanode_log_P(PSTR("http: file open"));
  nanode_log((char*)filename);
  
  int result = 0;
  const entry_t* cur = dir;
  bool done = false;
  while (!done)
  {
    entry e(cur);
    if (e.matches(filename))
    {
      e.result(file);
      done = 1;
      result = 1;
    }
    if (!e.isvalid())
      done = 1;
    ++cur;
  }

  return result; 
}
int httpd_fs_open_P(const char *filename, struct httpd_fs_file *file)
{
  nanode_log_P(PSTR("http: file open _P"));
  nanode_log_P((char*)filename);
  
  int result = 0;
  const entry_t* cur = dir;
  bool done = false;
  while (!done)
  {
    entry e(cur);
    if (e.matches_P(filename))
    {
      e.result(file);
      done = 1;
      result = 1;
    }
    if (!e.isvalid())
      done = 1;
    ++cur;
  }

  return result; 
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
