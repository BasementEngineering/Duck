#ifndef CONFIG_H
#define CONFIG_H

/*** WiFi Config ***/
// Set the Mini Kenterprise up in AP Mode to use it standalone without a WiFi network
#define AP_MODE

#define MAX_WIFI_CONNECTIONS 2

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

  String steeringType;
  String motor1_driver;
  String motor2_driver;

  bool station_mode;
  String ap_ssid;
  String ap_password;
  String sta_ssid;
  String sta_password;

  int led_pin;
  int led_count;

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

  void printAllSettings(){
    Serial.println("Settings:");
    Serial.println("Motor 1 Pin A: " + String(motor1_pinA));
    Serial.println("Motor 1 Pin B: " + String(motor1_pinB));
    Serial.println("Motor 1 Pin E: " + String(motor1_pinE));
    Serial.println("Motor 2 Pin A: " + String(motor2_pinA));
    Serial.println("Motor 2 Pin B: " + String(motor2_pinB));
    Serial.println("Motor 2 Pin E: " + String(motor2_pinE));
    Serial.println("Motor 1 Center: " + String(motor1_center));
    Serial.println("Motor 1 Max: " + String(motor1_max));
    Serial.println("Motor 1 Min: " + String(motor1_min));
    Serial.println("Motor 2 Center: " + String(motor2_center));
    Serial.println("Motor 2 Max: " + String(motor2_max));
    Serial.println("Motor 2 Min: " + String(motor2_min));
    Serial.println("Steering Type: " + steeringType);
    Serial.println("Motor 1 Driver: " + motor1_driver);
    Serial.println("Motor 2 Driver: " + motor2_driver);
    Serial.println("Station Mode: " + String(station_mode));
    Serial.println("AP SSID: " + ap_ssid);
    Serial.println("AP Password: " + ap_password);
    Serial.println("STA SSID: " + sta_ssid);
    Serial.println("STA Password: " + sta_password);
    Serial.println("LED Pin: " + String(led_pin));
    Serial.println("LED Count: " + String(led_count));
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
    motor1_max = preferences.getInt("motor1_max", 100);
    motor1_min = preferences.getInt("motor1_min", 0);
    motor2_center = preferences.getInt("motor2_center", 50);
    motor2_max = preferences.getInt("motor2_max", 100);
    motor2_min = preferences.getInt("motor2_min", 0);

    steeringType = preferences.getString("steeringType", "Differential Thrust");
    motor2_driver = preferences.getString("motor2_driver", "H-Bridge");
    motor1_driver = preferences.getString("motor1_driver", "H-Bridge");

    station_mode = preferences.getBool("station_mode", false);
    ap_ssid = preferences.getString("ap_ssid", "MiniKenterprise");
    ap_password = preferences.getString("ap_password", "IAmTheCaptainNow");
    sta_ssid = preferences.getString("sta_ssid", "NetworkName");
    sta_password = preferences.getString("sta_password", "Password");

    led_pin = preferences.getInt("led_pin", 4);
    led_count = preferences.getInt("led_count", 8);
  }

  void saveSettings()
  {
    Serial.println("Saving Settings");
    preferences.begin("settings", false);
    preferences.putBool("station_mode", station_mode);
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

    preferences.putInt("motor1_center", motor1_center);
    preferences.putInt("motor1_max", motor1_max);
    preferences.putInt("motor1_min", motor1_min);
    preferences.putInt("motor2_center", motor2_center);
    preferences.putInt("motor2_max", motor2_max);
    preferences.putInt("motor2_min", motor2_min);

    preferences.putString("steeringType", steeringType);
    preferences.putString("motor2_driver", motor2_driver);
    preferences.putString("motor1_driver", motor1_driver);

    preferences.putInt("led_pin", led_pin);
    preferences.putInt("led_count", led_count);

    //preferences.end();
  }

  String getDrivingSystemSettings()
  {
    String response = "{\"motor1_pinA\":" + String(motor1_pinA) + "," +
                      "\"motor1_pinB\":" + String(motor1_pinB) + "," +
                      "\"motor1_pinE\":" + String(motor1_pinE) + "," +
                      "\"motor1_pinA\":" + String(motor2_pinA) + "," +
                      "\"motor2_pinB\":" + String(motor2_pinB) + "," +
                      "\"motor2_pinE\":" + String(motor2_pinE) + "," +
                      "\"motor1_center\":" + String(motor1_center) + "," +
                      "\"motor1_max\":" + String(motor1_max) + "," +
                      "\"motor1_min\":" + String(motor1_min) + "," +
                      "\"motor2_center\":" + String(motor2_center) + "," +
                      "\"motor2_max\":" + String(motor2_max) + "," +
                      "\"motor2_min\":" + String(motor2_min) + "," +
                      "\"steeringType\":\"" + steeringType + "\"," +
                      "\"motor1_driver\":\"" + motor1_driver + "\"," +
                      "\"motor2_driver\":\"" + motor2_driver + "\"" +
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

    motor1_center = doc["motor1_center"].as<int>();
    motor1_max = doc["motor1_max"].as<int>();
    motor1_min = doc["motor1_min"].as<int>();
    motor2_center = doc["motor2_center"].as<int>();
    motor2_max = doc["motor2_max"].as<int>();
    motor2_min = doc["motor2_min"].as<int>();

    steeringType = doc["steeringType"].as<String>();
    motor1_driver = doc["motor1_driver"].as<String>();
    motor2_driver = doc["motor2_driver"].as<String>();

    Serial.println("Motor 1 Pin A: " + motor1_pinA);
    Serial.println("Motor 1 Pin B: " + motor1_pinB);
  }

  String getWifiSettings()
  {
  String response = "{\"station_mode\": " + String(station_mode) + "," +
                      "\"ap_ssid\": \"" +  ap_ssid     + "\"" + "," +
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

    station_mode = doc["station_mode"].as<bool>();
    ap_ssid = doc["ap_ssid"].as<String>();
    ap_password = doc["ap_password"].as<String>();
    sta_ssid = doc["sta_ssid"].as<String>();
    sta_password = doc["sta_password"].as<String>();

    Serial.println("AP SSID: " + ap_ssid);
    Serial.println("AP Password: " + ap_password);
  }

  String getLedSettings()
  {
    String response = "{\"led_pin\": " + String(led_pin) + "," +
                      "\"led_count\": " + String(led_count) + "}";
    return response;
  }

  void parseLedSettings(String jsonString)
  {
    Serial.println("Parsing led settings");
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

    led_pin = doc["led_pin"].as<int>();
    led_count = doc["led_count"].as<int>();

    Serial.println("LED Pin: " + led_pin);
    Serial.println("LED Count: " + led_count);
  }
};

#endif
