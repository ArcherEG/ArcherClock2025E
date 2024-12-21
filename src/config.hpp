#define FIRM_VERSION 0.8

// Wi-Fi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASWD";

#define LED_PIN   16
#define NUM_LEDS  132

// Unremark for show
#define SHOW_SECONDS_MARCKERS
//#define SHOW_MINUTES_MARCKERS
#define SHOW_HOURS_MARCKERS

// Marckers color
#define ZERO_MARKER_COLOR CRGB::OrangeRed
#define SECONDS_MARCKERS_COLOR CRGB::SeaGreen
#define MINUTES_MARCKERS_COLOR CRGB::LimeGreen
#define HOURS_MARCKERS_COLOR CRGB::LimeGreen

// Hands color
#define SECONDS_HAND_COLOR CRGB::Blue
#define MINUTES_HAND_COLOR CRGB::Blue
#define HOURS_HAND_COLOR CRGB::Blue

// Time zone and NTP server
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600; // Time offset in seconds (GMT+1)
const int daylightOffset_sec = 3600; // Daylight Saving Time (in seconds, optional)

// Synchronization time in hours
#define SYNC_TIME 48
