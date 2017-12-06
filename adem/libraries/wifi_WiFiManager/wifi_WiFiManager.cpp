/*
 * This file is part of the ADEM project.
 *
 * ADEM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,·
 * (at your option) any later version.
 *
 * ADEM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ADEM.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2016 Dag Wieers, Marco Stolle
 *
 */

#include "wifi_WiFiManager.h"

WiFiManagerWifi::WiFiManagerWifi() : myWiFiManager() {
    //myWiFiManager wifimanager;
    sprintf(SSID, "ADEM-%d", ESP.getChipId());
    _initialized = false;
}

WiFiManagerWifi::~WiFiManagerWifi() {
    //delete &wifimanager;
}

void WiFiManagerWifi::begin() {
    Serial.print("Initializing WiFiManager... ");
    if (!_initialized){
        // Set ConfigPortal timeout as AP
        //wifimanager.setConfigPortalTimeout(300);
        setConfigPortalTimeout(300);
        // Set connect timeout for WiFiManager client is sec
        //wifimanager.setConnectTimeout(15);
        setConnectTimeout(15);
        // Only consider APs with signal over 15%
        //wifimanager.setMinimumSignalQuality(15);
        setMinimumSignalQuality(15);
    }
    Serial.println("OK");
    _initialized = true;
}



void WiFiManagerWifi::end() {
}

void WiFiManagerWifi::read() {
}

void WiFiManagerWifi::write() {
}

void WiFiManagerWifi::process() {
}

void WiFiManagerWifi::sleep() {
    Serial.print("Turning WiFi off... ");

    // Disable WIFI
    //wifi_set_sleep_type(LIGHT_SLEEP_T);
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    //KOV WiFi.forceSleepBegin();
    //KOV delay(1); // needed to go to sleep

    Serial.println("OK");
}

void WiFiManagerWifi::start_ap(char SSID[20]) {
    if (!_initialized) begin();
    // Wake WIFI
    //KOV WiFi.forceSleepWake();
    //KOV delay(1); // needed to wakeup the modem

    // Start ConfigPortal and wait for submit or timeout
    WiFi.begin();
    //if (wifimanager.startConfigPortal(SSID)) {
    if (startConfigPortal(SSID)) {
        connected_once = true;
        // Log that wifi-connection worked
    } else {
        // Log that wifi-connection failed
    }
}

void WiFiManagerWifi::start_client() {
    if (!_initialized) begin();
    // Wake WIFI
    //KOV WiFi.forceSleepWake();
    //KOV delay (1); //needed to wakeup the modem
    // Start ConfigPortal and wait for submit or timeout
    ETS_GPIO_INTR_DISABLE(); // stop interrupt comming in
    Serial.println(" interrupts disabled ");
    //wifimanager.setSleepAfterAutoConnect(true);
    setSleepAfterAutoConnect(true);
    Serial.println(" wifi sleep after autoconnect ");
    //if (wifimanager.autoConnect()) {
    if (autoConnect()) {
        connected = true;
        // Log that wifi-connection worked
    } else {
        // Log that wifi-connection failed
        connected = false;
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
    }
    ETS_GPIO_INTR_ENABLE();
    Serial.println(" interrupts enabled ");
}

String WiFiManagerWifi::report()  {
    return "Nothing to report.";
}