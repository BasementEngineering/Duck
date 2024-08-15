#ifndef FRONTEND_SERVER_H
#define FRONTEND_SERVER_H

#include <ESP8266WebServer.h>
#define DEBUG_FRONTEND_SERVER

#include "LittleFS.h"
#include "Config.h"

bool serverOnline = false;
Settings* settingsObject = NULL;

ESP8266WebServer server(80);

#include "website_functions.h"

//Function Prototypes
void FrontendServer_init();
String getContentType(String filename);
bool handleFileRead(String path);
void FrontendServer_update();
void FrontendServer_start();
void FrontendServer_stop();

void FrontendServer_setSettingsObject(Settings* _settingsObject){
  settingsObject = _settingsObject;
}

void FrontendServer_init(){
  setupServerFunctions();

  server.on("/settings/wifi", HTTP_POST, [](){
    String jsonString = server.arg("plain");
    settingsObject->parseWifiSettings(jsonString);
    settingsObject->saveSettings();
    server.send(200, "text/plain", "Settings updated");
  });
  server.on("/settings/wifi",HTTP_GET, [](){
    String response = settingsObject->getWifiSettings();
    server.send(200, "application/json", response);
  });

  server.on("/settings/drivingsystem", HTTP_POST, [](){
    String jsonString = server.arg("plain");
    settingsObject->parseDrivingSystemSettings(jsonString);
    settingsObject->saveSettings();
    server.send(200, "text/plain", "Settings updated");
  });
  server.on("/settings/drivingsystem",HTTP_GET, [](){
    String response = settingsObject->getDrivingSystemSettings();
    server.send(200, "application/json", response);
  });

  server.on("/settings/leds", HTTP_POST, [](){
    String jsonString = server.arg("plain");
    settingsObject->parseLedSettings(jsonString);
    settingsObject->saveSettings();
    server.send(200, "text/plain", "Settings updated");
  });
  server.on("/settings/leds",HTTP_GET, [](){
    String response = settingsObject->getLedSettings();
    settingsObject->saveSettings();
    server.send(200, "application/json", response);
  });

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