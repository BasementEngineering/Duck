#ifndef CONFIG_H
#define CONFIG_H

/*** WiFi Config ***/
// Set the Mini Kenterprise up in AP Mode to use it standalone without a WiFi network
#define AP_MODE

// Access Point Settings
#define APSSID "Ente"
#define APPSK "RowYourBoat"
#define MAX_WIFI_CONNECTIONS 2

// Station Settings
#define NETWORK_SSID "**********"
#define NETWORK_PSK "**********"

// Motor Settings
#define MIN_PWM_L 1000
#define MIN_PWM_R 1000
#define MAX_PWM_L 2000
#define MAX_PWM_R 2000

#define TRIM 0.0F

#define DEBUG

// #define RUDDER_STEERING
#define ESC_PROPULSION

#define VERSION1_PINS
// #define VERSION3_PINS
// #define CUSTOM_PINS
// #define LEGOBOAT_PINS
/*** Your Pin Configuration ***/
/*** Version 1 ***/
#ifdef VERSION1_PINS
#define MOTOR_EN 15
#define MOTOR_IN1 13
#define MOTOR_IN2 12
#define MOTOR_IN3 14
#define MOTOR_IN4 0

#define LED_PIN 2
#define LED_COUNT 8
#endif

/*** Version 1 ***/
#ifdef LEGOBOAT_PINS
#define MOTOR_EN 12
#define MOTOR_IN1 15
#define MOTOR_IN2 13
#define MOTOR_IN3 16 // Rudder

#define LED_PIN 2
#define LED_COUNT 1
#endif

/*** Version 3 ***/
#ifdef VERSION3_PINS
#define MOTOR_EN 15  // D8
#define MOTOR_IN1 13 // D7
#define MOTOR_IN2 12 // D6
#define MOTOR_IN3 14 // D5
#define MOTOR_IN4 16 // D0

#define LED_PIN 2 // D4
#define LED_COUNT 5
#endif

/*! The Pin Markings on the WEMOS D1 Mini Board don't match the GPIO numbers !
 * Do not use GPIO 0 aka. D3 as it is used for flashing programs.
 *
 * Pinout Table:
 * Wifi Antenna = Top
 * | Left Side        | Right Side    |
 * ------------------------------------
 * | RST      | RST   | TX    | GPIO1 |
 * | ADC0     | A0    | RX    | GPIO3 |
 * | GPIO16   | D0    | D1    | GPIO5 |
 * | GPIO14   | D5    | D2    | GPIO4 |
 * | GPIO12   | D6    | D3    | GPIO0 |
 * | GPIO13   | D7    | D4    | GPIO2 |
 * | GPIO15   | D8    | G     | GND   |
 * | 3.3V Out | 3V3   | 5V    | 5V In |
 * USB Port = Bottom
 */

/*** New Settings ***/
// This new class is supposed to make the static config dynamic
#include <Preferences.h>
#include <Arduino.h>
#include <ArduinoJson.h>

class Settings
{
private:
public:
  uint8_t motor1_pinA;
  uint8_t motor1_pinB;
  uint8_t motor1_pinE;
  uint8_t motor2_pinA;
  uint8_t motor2_pinB;
  uint8_t motor2_pinE;

  int motor1_center = 50;
  int motor1_max = 100;
  int motor1_min = 0;
  int motor2_center = 50;

  int motor2_max = 100;
  int motor2_min = 0;

  String steeringType = "Differential Thrust";
  String motor1_driver = "ESC";
  String motor2_driver = "ESC";

  String ap_ssid;
  String ap_password;
  String sta_ssid;
  String sta_password;

  Preferences preferences;

  Settings() : preferences()
  {
  }

  void begin()
  {
    preferences.begin("settings", false);
    getSettings();
    preferences.end();
  }

  void getSettings()
  {
    preferences.begin("settings", false);
    motor1_pinA = preferences.getUInt("motor1_pinA", 15);
    motor1_pinB = preferences.getUInt("motor1_pinB", 13);
    motor1_pinE = preferences.getUInt("motor1_pinE", 15);
    motor2_pinA = preferences.getUInt("motor2_pinA", 14);
    motor2_pinB = preferences.getUInt("motor2_pinB", 16);
    motor2_pinE = preferences.getUInt("motor2_pinE", 15);

    motor1_center = preferences.getInt("motor1_center", 50);
    motor1_driver = preferences.getString("motor1_driver", "ESC");
    motor1_max = preferences.getInt("motor1_max", 100);
    motor1_min = preferences.getInt("motor1_min", 0);
    motor2_center = preferences.getInt("motor2_center", 50);
    motor2_driver = preferences.getString("motor2_driver", "ESC");
    motor2_max = preferences.getInt("motor2_max", 100);
    motor2_min = preferences.getInt("motor2_min", 0);
    steeringType = preferences.getString("steeringType", "Differential Thrust");

    ap_ssid = preferences.getString("ap_ssid", "MiniKenterprise");
    ap_password = preferences.getString("ap_password", "IAmTheCaptainNow");
    sta_ssid = preferences.getString("sta_ssid", "NetworkName");
    sta_password = preferences.getString("sta_password", "Password");
    preferences.end();
  }

  void saveSettings()
  {
    Serial.println("Saving Settings");
    preferences.begin("settings", false);
    preferences.putString("ap_ssid", ap_ssid);
    preferences.putString("ap_password", ap_password);
    preferences.putString("sta_ssid", sta_ssid);
    preferences.putString("sta_password", sta_password);
    preferences.putUInt("motor1_pinA", motor1_pinA);
    preferences.putUInt("motor1_pinB", motor1_pinB);
    preferences.putUInt("motor1_pinE", motor1_pinE);
    preferences.putUInt("motor2_pinA", motor2_pinA);
    preferences.putUInt("motor2_pinB", motor2_pinB);
    preferences.putUInt("motor2_pinE", motor2_pinE);
    preferences.end();
  }

  String getDrivingSystemSettings()
  {
    String response = "{\"motor1_pinA\":" + String(motor1_pinA) + "," +
                      "\"motor1_pinB\":" + String(motor1_pinB) + "," +
                      "\"motor1_pinE\":" + String(motor1_pinE) + "," +
                      "\"motor1_pinA\":" + String(motor2_pinA) + "," +
                      "\"motor2_pinB\":" + String(motor2_pinB) + "," +
                      "\"motor2_pinE\":" + String(motor2_pinE) +
                      "}";

    return response;
  }

  void parseDrivingSystemSettings(String jsonString)
  {
    Serial.println("Parsing driving system settings");
    StaticJsonDocument<200> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, jsonString);

    // Test if parsing succeeds.
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    motor1_pinA = doc["motor1_pinA"].as<uint8_t>();
    motor1_pinB = doc["motor1_pinB"].as<uint8_t>();
    motor1_pinE = doc["motor1_pinE"].as<uint8_t>();
    motor2_pinA = doc["motor2_pinA"].as<uint8_t>();
    motor2_pinB = doc["motor2_pinB"].as<uint8_t>();
    motor2_pinE = doc["motor2_pinE"].as<uint8_t>();

    Serial.println("Motor 1 Pin A: " + motor1_pinA);
    Serial.println("Motor 1 Pin B: " + motor1_pinB);
  }

  String getWifiSettings()
  {
    String response = "{\"ap_ssid\": \"" +  ap_ssid     + "\"" + "," +
                      "\"ap_password\": \"" + ap_password  + "\"" + "," +
                      "\"sta_ssid\": \"" + sta_ssid     + "\"" + "," +
                      "\"sta_password\": \"" + sta_password   + "\"}";
    
    Serial.println(response);
    return response;
  }

  void parseWifiSettings(String jsonString)
  {
    Serial.println("Parsing wifi settings");
    Serial.println(jsonString);
    StaticJsonDocument<200> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, jsonString);

    // Test if parsing succeeds.
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    ap_ssid = doc["ap_ssid"].as<String>();
    ap_password = doc["ap_password"].as<String>();
    sta_ssid = doc["sta_ssid"].as<String>();
    sta_password = doc["sta_password"].as<String>();

    Serial.println("AP SSID: " + ap_ssid);
    Serial.println("AP Password: " + ap_password);
  }
};

#endif
