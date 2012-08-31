#include <avr/pgmspace.h>
const char http_10[9] PROGMEM = 
/* "HTTP/1.0" */
{0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0 };
const char http_11[9] PROGMEM = 
/* "HTTP/1.1" */
{0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x31, 0 };
const char http_200[5] PROGMEM = 
/* "200 " */
{0x32, 0x30, 0x30, 0x20, 0 };
const char http_301[5] PROGMEM = 
/* "301 " */
{0x33, 0x30, 0x31, 0x20, 0 };
const char http_302[5] PROGMEM = 
/* "302 " */
{0x33, 0x30, 0x32, 0x20, 0 };
const char http_404_html[10] PROGMEM = 
/* "/404.html" */
{0x2f, 0x34, 0x30, 0x34, 0x2e, 0x68, 0x74, 0x6d, 0x6c, 0 };
const char http_content_length [17] PROGMEM = 
/* "Content-Length: " */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x4c, 0x65, 0x6e, 0x67, 0x74, 0x68, 0x3a, 0x20, 0 };
const char http_content_type_binary[43] PROGMEM = 
/* "Content-type: application/octet-stream\r\n\r\n" */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x6f, 0x63, 0x74, 0x65, 0x74, 0x2d, 0x73, 0x74, 0x72, 0x65, 0x61, 0x6d, 0xd, 0xa, 0xd, 0xa, 0 };
const char http_content_type[15] PROGMEM = 
/* "content-type: " */
{0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0 };
const char http_content_type_css [27] PROGMEM = 
/* "Content-type: text/css\r\n\r\n" */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2f, 0x63, 0x73, 0x73, 0xd, 0xa, 0xd, 0xa, 0 };
const char http_content_type_gif [28] PROGMEM = 
/* "Content-type: image/gif\r\n\r\n" */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x2f, 0x67, 0x69, 0x66, 0xd, 0xa, 0xd, 0xa, 0 };
const char http_content_type_html[28] PROGMEM = 
/* "Content-type: text/html\r\n\r\n" */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0xd, 0xa, 0xd, 0xa, 0 };
const char http_content_type_jpg [29] PROGMEM = 
/* "Content-type: image/jpeg\r\n\r\n" */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x2f, 0x6a, 0x70, 0x65, 0x67, 0xd, 0xa, 0xd, 0xa, 0 };
const char http_content_type_plain[29] PROGMEM = 
/* "Content-type: text/plain\r\n\r\n" */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2f, 0x70, 0x6c, 0x61, 0x69, 0x6e, 0xd, 0xa, 0xd, 0xa, 0 };
const char http_content_type_png [28] PROGMEM = 
/* "Content-type: image/png\r\n\r\n" */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x69, 0x6d, 0x61, 0x67, 0x65, 0x2f, 0x70, 0x6e, 0x67, 0xd, 0xa, 0xd, 0xa, 0 };
const char http_content_type_text[28] PROGMEM = 
/* "Content-type: text/text\r\n\r\n" */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2f, 0x74, 0x65, 0x78, 0x74, 0xd, 0xa, 0xd, 0xa, 0 };
const char http_content_type_manifest[32] PROGMEM =
/* "Content-type: text/manifest\r\n\r\n" */
{0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2f, 0x6d, 0x61, 0x6e, 0x69, 0x66, 0x65, 0x73, 0x74, 0xd, 0xa, 0xd, 0xa, };
const char http_crnl[3] PROGMEM = 
/* "\r\n" */
{0xd, 0xa, 0 };
const char http_css[5] PROGMEM = 
/* ".css" */
{0x2e, 0x63, 0x73, 0x73, 0 };
const char http_get[5] PROGMEM = 
/* "GET " */
{0x47, 0x45, 0x54, 0x20, 0 };
const char http_gif[5] PROGMEM = 
/* ".gif" */
{0x2e, 0x67, 0x69, 0x66, 0 };
const char http_header_200[54] PROGMEM = 
/* "HTTP/1.0 200 OK\r\nServer: uIP/1.0\r\nConnection: close\r\n" */
{0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0x20, 0x32, 0x30, 0x30, 0x20, 0x4f, 0x4b, 0xd, 0xa, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x3a, 0x20, 0x75, 0x49, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0xd, 0xa, 0x43, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3a, 0x20, 0x63, 0x6c, 0x6f, 0x73, 0x65, 0xd, 0xa, 0 };
const char http_header_404[61] PROGMEM = 
/* "HTTP/1.0 404 Not found\r\nServer: uIP/1.0\r\nConnection: close\r\n" */
{0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0x20, 0x34, 0x30, 0x34, 0x20, 0x4e, 0x6f, 0x74, 0x20, 0x66, 0x6f, 0x75, 0x6e, 0x64, 0xd, 0xa, 0x53, 0x65, 0x72, 0x76, 0x65, 0x72, 0x3a, 0x20, 0x75, 0x49, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0xd, 0xa, 0x43, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3a, 0x20, 0x63, 0x6c, 0x6f, 0x73, 0x65, 0xd, 0xa, 0 };
const char http_host[7] PROGMEM = 
/* "host: " */
{0x68, 0x6f, 0x73, 0x74, 0x3a, 0x20, 0 };
const char http_htm[5] PROGMEM = 
/* ".htm" */
{0x2e, 0x68, 0x74, 0x6d, 0 };
const char http_html[6] PROGMEM = 
/* ".html" */
{0x2e, 0x68, 0x74, 0x6d, 0x6c, 0 };
const char http_http[8] PROGMEM = 
/* "http://" */
{0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0 };
const char http_icy[4] PROGMEM = 
/* "ICY" */
{0x49, 0x43, 0x59, 0 };
const char http_index_html[12] PROGMEM = 
/* "/index.html" */
{0x2f, 0x69, 0x6e, 0x64, 0x65, 0x78, 0x2e, 0x68, 0x74, 0x6d, 0x6c, 0 };
const char http_index_shtml[13] PROGMEM = 
/* "/index.shtml" */
{0x2f, 0x69, 0x6e, 0x64, 0x65, 0x78, 0x2e, 0x73, 0x68, 0x74, 0x6d, 0x6c, 0 };
const char http_jpg[5] PROGMEM = 
/* ".jpg" */
{0x2e, 0x6a, 0x70, 0x67, 0 };
const char http_location[11] PROGMEM = 
/* "location: " */
{0x6c, 0x6f, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x3a, 0x20, 0 };
const char http_manifest[10] PROGMEM =
/* ".manifest" */
{0x2e, 0x6d, 0x61, 0x6e, 0x69, 0x66, 0x65, 0x73, 0x74, };
const char http_png[5] PROGMEM = 
/* ".png" */
{0x2e, 0x70, 0x6e, 0x67, 0 };
const char http_post[6] PROGMEM = 
/* "POST " */
{0x50, 0x4f, 0x53, 0x54, 0x20, 0 };
const char http_put[5] PROGMEM = 
/* "PUT " */
{0x50, 0x55, 0x54, 0x20, 0 };
const char http_referer[9] PROGMEM = 
/* "Referer:" */
{0x52, 0x65, 0x66, 0x65, 0x72, 0x65, 0x72, 0x3a, 0 };
const char http_shtml[7] PROGMEM = 
/* ".shtml" */
{0x2e, 0x73, 0x68, 0x74, 0x6d, 0x6c, 0 };
const char http_texthtml[10] PROGMEM = 
/* "text/html" */
{0x74, 0x65, 0x78, 0x74, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0 };
const char http_text[6] PROGMEM = 
/* ".text" */
{0x2e, 0x74, 0x65, 0x78, 0x74, 0 };
const char http_txt[5] PROGMEM = 
/* ".txt" */
{0x2e, 0x74, 0x78, 0x74, 0 };
const char http_user_agent_fields[43] PROGMEM = 
/* "Connection: close\r\nUser-Agent: uIP/1.0\r\n\r\n" */
{0x43, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3a, 0x20, 0x63, 0x6c, 0x6f, 0x73, 0x65, 0xd, 0xa, 0x55, 0x73, 0x65, 0x72, 0x2d, 0x41, 0x67, 0x65, 0x6e, 0x74, 0x3a, 0x20, 0x75, 0x49, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0xd, 0xa, 0xd, 0xa, 0 };
