/* This file has been generated by encodeInArduino.py */
/* The callbacks send encoded content that can be found in the website_content.h file.*/
#ifndef WEBSITE_FUNCTIONS_H
#define WEBSITE_FUNCTIONS_H

#include "Arduino.h"

#include "website_content.h"

// content callbacks
void handle_index_html() { server.send_P(200, "text/html", index_html,index_html_length);}
void handle_index_BjLVT99__js() { server.send_P(200, "application/javascript", index_BjLVT99__js,index_BjLVT99__js_length);}
void handle_index_DeMo6euZ_css() { server.send_P(200, "text/plain", index_DeMo6euZ_css,index_DeMo6euZ_css_length);}

void setupServerFunctions() {
server.on("/", handle_index_html);
server.on("/assets/index-BjLVT99-.js", handle_index_BjLVT99__js);
server.on("/assets/index-DeMo6euZ.css", handle_index_DeMo6euZ_css);
}

#endif

