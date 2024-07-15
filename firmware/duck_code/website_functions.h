#ifndef WEBSITE_FUNCTIONS_H
#define WEBSITE_FUNCTIONS_H

#include "Arduino.h"

void handle_index_html() { server.send_P(200, "text/html", index_html,index_html_length);}
server.on("index.html", handle_index_html);
void handle_dataBuffer_js() { server.send_P(200, "application/javascript", dataBuffer_js,dataBuffer_js_length);}
server.on("scripts/dataBuffer.js", handle_dataBuffer_js);
void handle_bootstrap_icons_css() { server.send_P(200, "text/plain", bootstrap_icons_css,bootstrap_icons_css_length);}
server.on("icons/bootstrap-icons.css", handle_bootstrap_icons_css);
void handle_bootstrap_icons_json() { server.send_P(200, "text/plain", bootstrap_icons_json,bootstrap_icons_json_length);}
server.on("icons/bootstrap-icons.json", handle_bootstrap_icons_json);
void handle_bootstrap_icons_scss() { server.send_P(200, "text/plain", bootstrap_icons_scss,bootstrap_icons_scss_length);}
server.on("icons/bootstrap-icons.scss", handle_bootstrap_icons_scss);
void handle_icons_woff2() { server.send_P(200, "text/plain", icons_woff2,icons_woff2_length);}
server.on("icons/icons.woff2", handle_icons_woff2);

#endif

