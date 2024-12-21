/******************************************************************************************
 *                               Archer Clock 2025E Firmware                              *
 *----------------------------------------------------------------------------------------*
 * Project:   Clock with hypnotic motion and time synchronization via Wi-Fi and NTP.      *
 * Author:    Archer                                                                      *
 * Date:      25/09/2024                                                                  *
 *----------------------------------------------------------------------------------------*
 * Description:                                                                           *
 * - This clock is inspired by the light effects of the "Corpus Clock" located on the     *
 *   campus of the University of Cambridge.                                               *
 *                                                                                        *
 * Features:                                                                              *
 * -  Hypnotic LED movement of the hands.                                                 *
 * -  Time synchronization via Wi-Fi to NTP server.                                       *
 *                                                                                        *
 ******************************************************************************************/
#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>

#include "config.hpp"


CRGB leds[NUM_LEDS];

uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;
uint8_t syncCountdown = SYNC_TIME;

unsigned long previousMillis = 0;


void draw(CRGB leds[], uint8_t s, uint8_t m, uint8_t h) {
  leds[0] = ZERO_MARKER_COLOR;  // Initial marker

  // Draw second markers
  #ifdef SHOW_SECONDS_MARCKERS
    for (int c = 5; c <= 55; c += 5) {
      leds[c] = SECONDS_MARCKERS_COLOR;
    }
  #endif

  // Draw minute markers
  #ifdef SHOW_MINUTES_MARCKERS
    for (int c = 0; c <= 55; c += 5) {
      leds[60 + c] = MINUTES_MARCKERS_COLOR;
    }
  #endif

  // Draw hour markers
  #ifdef SHOW_HOURS_MARCKERS
    for (int c = 0; c < 12; ++c) {
      leds[120 + c] = HOURS_MARCKERS_COLOR;
    }
  #endif

  // Draw hands
  leds[s] = SECONDS_HAND_COLOR;
  leds[60 + m] = MINUTES_HAND_COLOR;
  leds[120 + h] = HOURS_HAND_COLOR;

  // Update LEDs
  FastLED.show();
}


void syncronize() {
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);

  leds[0] = CRGB::Orange;
  FastLED.show();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  leds[0] = CRGB::Green;
  FastLED.show();

  // Configuration for synchronization with the NTP server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;
  
  hours = timeinfo.tm_hour;
  minutes = timeinfo.tm_min;
  seconds = timeinfo.tm_sec;
  if (hours>12) hours-=12;
  syncCountdown = SYNC_TIME;

  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}


void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  syncronize();
 
  previousMillis = millis();
}


void loop() {
  unsigned long currentMillis = millis();
  while (currentMillis - previousMillis < 1000) {
    currentMillis = millis();
  }
  previousMillis = currentMillis;


  bool move_minutes = false;
  bool move_hours = false;
  uint8_t steps_hour = -1;
  if (seconds == 59) {
    move_minutes = true;
    if (minutes == 59) {
      move_hours = true;
      syncCountdown--;
    }
  }

  uint8_t acceleration = 20;
  for (int i = 0; i<=60; i++) {

    seconds++;
    if (move_minutes) minutes++;
    if (seconds>59) seconds = 0;
    if (minutes>59) minutes = 0;

    if (move_hours) {
      steps_hour++;
      if (i==60) hours++;
    }
    if (steps_hour == 5) {
      hours++;
      steps_hour = 0;
    }
    if (hours>11) hours -= 12;

    unsigned long start_timer = millis();
    draw(leds, seconds, minutes, hours);
    unsigned long stop_timer = millis();
    unsigned long total_time = stop_timer - start_timer;
    Serial.println(total_time);

    delay(acceleration);
    if (acceleration > 3) acceleration--;
    leds[seconds] = CRGB::Black;
    leds[60+minutes] = CRGB::Black;
    leds[120+hours] = CRGB::Black;
  }
  
  if (syncCountdown == 0) syncronize();
}



