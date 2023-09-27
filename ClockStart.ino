#include <Arduino.h>
#include <IRremote.h>
#include <uRTCLib.h>

/// @brief RTC Interface
/// @note SDA is pin 20, SCL is pin 21 by default on the Mega
uRTCLib rtc(0x68);

/// @brief This is the IR code for 1 on the clock remote
/// @note To retrieve these values, use the IR Receivers and the IR dumper
/// example in the IRremote library and print the NEC codes. The pins are a
/// little bit finnicky so you may need to try using a couple different ones.
constexpr const uint8_t KEY_1 = 0xE;

/// @brief This is the IR code for start on the clock remote
constexpr const uint8_t KEY_START = 0x5E;

/// @brief This is the IR code for clock on the clock remote
constexpr const uint8_t KEY_CLOCK = 0x53;

/// @brief This is the pin the IR LED positive lead is connected to.
constexpr const uint8_t IR_SEND_PIN = 9;

/// @brief Days of the week
constexpr const char DOW[7][10] = {"Monday",   "Tuesday", "Wednesday",
                                   "Thursday", "Friday",  "Saturday",
                                   "Sunday"};

/// @brief Gets the current time as a string
/// @return The current time formatted as "MM/DD/YYYY HH:MM:SS DOW"
/// @note There is probably a better way to do this... :)
String now() {
  String s;
  s.concat(rtc.month());
  s.concat('/');
  s.concat(rtc.day());
  s.concat('/');
  s.concat("20");
  if (rtc.year() < 10) {
    s.concat('0');
  }
  s.concat(rtc.year());
  s.concat(' ');
  s.concat(rtc.hour() > 12 ? rtc.hour() - 12 : rtc.hour());
  s.concat(':');
  if (rtc.minute() < 10) {
    s.concat('0');
  }
  s.concat(rtc.minute());
  s.concat(':');
  if (rtc.second() < 10) {
    s.concat('0');
  }
  s.concat(rtc.second());
  s.concat(' ');
  s.concat(DOW[rtc.dayOfWeek() - 1]);
  return s;
}

void setup() {
  /// @note Uncomment this to set the time (seconds, minutes, hours, day of
  /// week,day of month, month, year)
  // rtc.set(30, 16, 10,URTCLIB_WEEKDAY_MONDAY,25, 9, 23);

  IrSender.begin(IR_SEND_PIN);
  // Send 1 key (3 times to ensure it is received)
  IrSender.sendNEC(0x1, KEY_1, 3);
  delay(1000);

  // Send start key (also 3 times)
  IrSender.sendNEC(0x1, KEY_START, 3);
  delay(1000);

  // Initialize the RTC
  URTCLIB_WIRE.begin();
}

void loop() {
  rtc.refresh();
  // Print current time for debugging purposes
  // Serial.println(now());
  if (rtc.hour() == 7 && rtc.minute() == 50 && rtc.second() == 0) {
    // 7:50 AM
    // Turn on timer
    IrSender.sendNEC(0x1, KEY_1, 3);
    delay(1000);
    IrSender.sendNEC(0x1, KEY_START, 3);
    delay(1000);
  } else if (rtc.hour() == 10 && rtc.minute() == 49 && rtc.second() == 0) {
    // 10:50 AM
    // Turn on timer
    IrSender.sendNEC(0x1, KEY_CLOCK, 3);
    delay(1000);
  } else if (rtc.hour() == 11 && rtc.minute() == 20 && rtc.second() == 0) {
    // 11:20 AM
    // Turn on timer
    IrSender.sendNEC(0x1, KEY_1, 3);
    delay(1000);
    IrSender.sendNEC(0x1, KEY_START, 3);
    delay(1000);
  } else if (rtc.hour() == 14 && rtc.minute() == 20 && rtc.second() == 0) {
    // 2:20 PM
    // Set to default clock mode
    IrSender.sendNEC(0x1, KEY_CLOCK, 3);
  }
  delay(500);
}
