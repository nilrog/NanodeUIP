#include "files.h"

const file_entry_t dir[] PROGMEM = {
{ name_header_html , header_html , & header_html_len },
{ name_footer_html , footer_html , & footer_html_len },
{ name_buttons_list_shtml , buttons_list_shtml , & buttons_list_shtml_len },
{ name_buttons_shtml , buttons_shtml , & buttons_shtml_len },
{ name_credits_shtml , credits_shtml , & credits_shtml_len },
{ name_sensors_list_shtml , sensors_list_shtml , & sensors_list_shtml_len },
{ name_index_shtml , index_shtml , & index_shtml_len },
{ name_set_pin_shtml , set_pin_shtml , & set_pin_shtml_len },
{ name_sensors_shtml , sensors_shtml , & sensors_shtml_len },
{ name_lights_shtml , lights_shtml , & lights_shtml_len },
{ name_ok_txt , ok_txt , & ok_txt_len },
{ name_index_js , index_js , & index_js_len },
{0,0,0},
};
