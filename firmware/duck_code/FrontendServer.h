#ifndef FRONTEND_SERVER_H
#define FRONTEND_SERVER_H

#include <ESP8266WebServer.h>
#define DEBUG_FRONTEND_SERVER

#include "LittleFS.h"

bool serverOnline = false;

ESP8266WebServer server(80);

#include "website_content.h"

//Function Prototypes
void FrontendServer_init();
String getContentType(String filename);
bool handleFileRead(String path);
void FrontendServer_update();
void FrontendServer_start();
void FrontendServer_stop();

void handle_index_html() { server.send_P(200, "text/html", index_html,index_html_length);}
void handle_dataBuffer_js() { server.send_P(200, "application/javascript", dataBuffer_js,dataBuffer_js_length);}
void handle_bootstrap_icons_css() { server.send_P(200, "text/plain", bootstrap_icons_css,bootstrap_icons_css_length);}
void handle_bootstrap_icons_json() { server.send_P(200, "text/plain", bootstrap_icons_json,bootstrap_icons_json_length);}
void handle_bootstrap_icons_scss() { server.send_P(200, "text/plain", bootstrap_icons_scss,bootstrap_icons_scss_length);}
void handle_icons_woff2() { server.send_P(200, "text/plain", icons_woff2,icons_woff2_length);}

void FrontendServer_init(){
  server.on("/", handle_index_html);
server.on("/scripts/dataBuffer.js", handle_dataBuffer_js);
server.on("/icons/bootstrap-icons.css", handle_bootstrap_icons_css);
server.on("/icons/bootstrap-icons.json", handle_bootstrap_icons_json);
server.on("/icons/bootstrap-icons.scss", handle_bootstrap_icons_scss);
server.on("/icons/icons.woff2", handle_icons_woff2);

  //ToDo

  FrontendServer_start();
}

void FrontendServer_update(){
  server.handleClient();
}

void FrontendServer_start(){
  server.begin();
  serverOnline = true;
}

void FrontendServer_stop(){
  server.stop();
  serverOnline = false;
}

/*** SERVING FILES (index and stuff) ***/
String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  #ifdef DEBUG_FRONTEND_SERVER
  Serial.println("handleFileRead: " + path);
  #endif
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (LittleFS.exists(path)) {                            // If the file exists
    File file = LittleFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  return false;                                         // If the file doesn't exist, return false
}
#endif