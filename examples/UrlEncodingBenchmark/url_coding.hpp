#pragma once

/*
From https://github.com/zenmanenergy/ESP8266-Arduino-Examples/blob/master/helloWorld_urlencoded/urlencode.ino

From https://github.com/TwilioDevEd/twilio_esp8266_arduino_example

Modified by Brian T. Park 2020 to compare the runtime of these routines to the
AceUtils UrlEncoding utilities which use the PrintStr utility classes instead
of String to avoid heap fragmentation.
*/

#include <Arduino.h>

unsigned char h2int(char c);
String urldecode_yield(String str);
String urlencode_yield(String str);
String urldecode_no_yield(String str);
String urlencode_no_yield(String str);
