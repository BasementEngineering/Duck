#ifndef WIFI_H
#define WIFI_H

#include "Config.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#define DEBUG

//AP
String ssid;
String password;

String networkSsid;
String networkPassword;

bool stationMode = false;

const char* dnsName = "MiniKenterprise";

bool wifiOnline = false;
unsigned long lastWifiUpdate = 0;
int nextWaitInterval = 1000;
int attemptCounter = 0;

IPAddress local_IP(1,2,3,4);
IPAddress gateway(1,2,3,4);
IPAddress subnet(255,255,255,0);
int myChannel = 1;

// "Public" functions
void Wifi_setupAp();
void startMdns();
void Wifi_update();
bool Wifi_online();
bool Wifi_connected();
int Wifi_getQualityPercentage();

// Improv functions for initially setting up the wifi
//TODO: Implement Improiv functions


//To find a good Wifi Channel
#define CHANNEL_CNT 14
uint8_t ap_count[CHANNEL_CNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int32_t max_rssi[CHANNEL_CNT] = {-100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100};
#define SCAN_DURATION 10000;

void clearData();
void scanWiFi();
void processScanData(int n);
void printWifiNetwork(int i);
void printScanData();
int findEmptyChannel();
int findLowestRssiChannel();
int getBestChannel();

void setApCredentials(String _ssid, String _password);
void setNetworkCredentials(String _ssid, String _password);

void startStation();
void startAp();

void Wifi_setStationMode(bool _stationMode){
  stationMode = _stationMode;
}

void Wifi_setup(){
  scanWiFi();
  myChannel = getBestChannel();
  #ifdef DEBUG
  Serial.print("Best WiFi Cahnnel: ");Serial.println(myChannel);
  #endif
}

void Wifi_start(){
  if( (millis() - lastWifiUpdate) > nextWaitInterval){
    lastWifiUpdate = millis();
    wifi_set_sleep_type(NONE_SLEEP_T);
    if(stationMode){
      startStation();
    }
    else{
      startAp();
    }
    startAp();
  startMdns();
  }
}

bool Wifi_connected(){
  if (!stationMode){
    return WiFi.softAPgetStationNum();
  }
  else{
    return WiFi.isConnected();
  }
}

void startStation(){
  Serial.println("Starting Station");
  Serial.println("Connecting to Network");
  Serial.println(networkSsid);
  
  WiFi.disconnect();
  if(attemptCounter == 0){
    Serial.println("First Attempt");
    WiFi.mode(WIFI_STA);
    const char *c_networkSsid = networkSsid.c_str();
    const char *c_networkPassword = networkPassword.c_str();
    WiFi.begin(c_networkSsid, c_networkPassword);
    attemptCounter++;
  }
  if(attemptCounter > 0){
    Serial.println("Subsequent Attempts");
    if(WiFi.waitForConnectResult() == WL_CONNECTED){
       wifiOnline = true;
    }
   }
}

void Wifi_setApCredentials(String _ssid, String _password){
  ssid = _ssid;
  password = _password;
}

void Wifi_setNetworkCredentials(String _ssid, String _password){
  networkSsid = _ssid;
  networkPassword = _password;
}

void startAp(){
    Serial.println("Starting AP");
    const char *c_ssid = ssid.c_str();
    const char *c_password = password.c_str();
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
    WiFi.softAP(c_ssid, c_password,myChannel,false,MAX_WIFI_CONNECTIONS);
    #ifdef DEBUG
      Serial.print("Access Point \"");
      Serial.print(c_ssid);
      Serial.println("\" started");
      Serial.print("IP address:\t");
      Serial.println(WiFi.softAPIP());
    #endif
    wifiOnline = true;
}

void startMdns(){
  if (!MDNS.begin(dnsName, WiFi.softAPIP())) {
    Serial.println("mDNS setup failed");
  } else {
    Serial.println("mDNS online at http://"+String(dnsName)+".local");
    MDNS.addService("http", "tcp", 80);
  }
}

void Wifi_update(){
  MDNS.update();
}

bool Wifi_online(){
  return wifiOnline;
}

bool Wifi_hasClient(){
  return  WiFi.softAPgetStationNum();
}

int Wifi_getQualityPercentage(){
  //if (WiFi.status() != WL_CONNECTED)
    //return -1;
  int dBm = WiFi.RSSI();
  if (dBm <= -100){
    return 0;
  }
  else if (dBm >= -50){
    return 100;
  }
  else{
    return 2 * (dBm + 100);
  }
}

void clearData(){
  for(int i = 0; i < CHANNEL_CNT; i++){
    ap_count[i] = 0;
    max_rssi[i] = -100;
  }
}

void scanWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  //unsigned long scanStartTime = millis();
  //while( (millis() - scanStartTime) > SCAN_DURATION){
    int n = WiFi.scanNetworks();
    processScanData(n);
    delay(100);
  //}
}

void processScanData(int n){
  
  for (int i = 0; i < n; i++) {
    int32_t channel = WiFi.channel(i);
    int32_t rssi = WiFi.RSSI(i);

    ap_count[channel - 1]++;
    if (rssi > max_rssi[channel - 1]) {
      max_rssi[channel - 1] = rssi;
    }
   }
}

void printWifiNetwork(int i){
   Serial.print(WiFi.SSID(i));
   Serial.print('(');
   Serial.print(WiFi.RSSI(i));
   Serial.print(')');
   if (WiFi.encryptionType(i) == ENC_TYPE_NONE) {
      Serial.print('*');
    }
    Serial.println();
}

void printScanData(){
  Serial.println("Wifi Scan Result:");
  Serial.print("AP Count: [");
  for(int i = 0; i < CHANNEL_CNT; i++){
    Serial.print(ap_count[i]);
    if(i < CHANNEL_CNT-1){
      Serial.print(",");
    }
  }
  Serial.println("]");
  
  Serial.print("RSSI: [");
  for(int i = 0; i < CHANNEL_CNT; i++){
    Serial.print(max_rssi[i]);
    if(i < CHANNEL_CNT-1){
      Serial.print(",");
    }
  }
  Serial.println("]");
}

int findEmptyChannel(){
  for(int i = 0; i < CHANNEL_CNT;i++){
    if(ap_count[i] == 0){
      return i+1;
    }
  }
  return -1;
}

int findLowestRssiChannel(){
  int lowestRssi = 0;
  int channel = 1;

  for(int i = 0; i < CHANNEL_CNT;i++){
    if(max_rssi[i] < lowestRssi){
      lowestRssi = max_rssi[i];
      channel = i+1;
    }
  }
  Serial.print("Found lowest RSSI channel: ");Serial.println(channel);
  Serial.print("RSSI:"); Serial.println(lowestRssi);
  return channel;
}

int getBestChannel(){
  int emptyChannel = findEmptyChannel();
  if( emptyChannel != -1){
    return emptyChannel;
  }
  else{
    return findLowestRssiChannel();
  }
}

#endif
