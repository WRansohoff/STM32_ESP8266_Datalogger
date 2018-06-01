# Overview

Simple firmware for using an STM32 with an ESP8266 to communicate over WiFi.

# Current Status

Sort of working with an STM32F051K8 - it can join a WiFi network and report its connection status in a debugger, but it's messy and very much a work-in-progress.

But it is not reaching out to any websites or APIs yet.

Note that the project will not compile without a 'wifi\_config.h' file which contains `static const char[]` definitions for the `WIFI_SSID` and `WIFI_PW` variables.
