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
 * Copyright 2016 Dag Wieers, Lieven Blancke
 *
 */

#include "gps_SwSerial.h"

#ifdef DEBUG_GPS
#define __LOG(msg) Serial.print(msg)
#define __LOGLN(msg) Serial.println(msg)
#else
#define __LOG(msg)
#define __LOGLN(msg)
#endif

SwSerialGPS::SwSerialGPS(int rx, int tx, int bd) {
    swserial = new SoftwareSerial(rx, tx, false, 512);
    tinygps = new TinyGPSPlus();
    baud = bd;
    measuredData.ID = GPS_SWSERIAL;
}

SwSerialGPS::SwSerialGPS() {
    SwSerialGPS(4, 0, 9600);
}

SwSerialGPS::~SwSerialGPS() {
    delete tinygps;
    delete swserial;
}

void SwSerialGPS::begin(void) {
    Serial.print("Initializing GPS... ");
    swserial->begin(baud);
    Serial.println("OK");
}

void SwSerialGPS::end() {
}

void SwSerialGPS::write() {
}

void SwSerialGPS::process() {
}

void SwSerialGPS::read() {

    unsigned long start = millis();
    while (swserial->available() > 0 and millis() - start < 150) {
        char c = swserial->read();
        __LOG(c);
        tinygps->encode(c);
    }

    measuredData.date = tinygps->date;
    measuredData.time = tinygps->time;
    measuredData.location = tinygps->location;
    measuredData.satellites = tinygps->satellites;
    measuredData.altitude = tinygps->altitude;
    measuredData.speed = tinygps->speed;
	_measured = true;
}

String SwSerialGPS::FormatDateTime(TinyGPSDate date, TinyGPSTime time) {
    char gpsDateTime[26] = "";
    sprintf(gpsDateTime, "%04d-%02d-%02dT%02d:%02d:%02d.%02d0Z", date.year(), date.month(), date.day(), time.hour(), time.minute(), time.second(), time.centisecond());
    return gpsDateTime;
}

String SwSerialGPS::report()  {
    if (!_measured)
        read();
    _measured = false;
    return buildReport( &measuredData);
}

String SwSerialGPS::buildReport(sensorData *sData)  {

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    char response[200];
    SwSerialGPSData *gpsData = reinterpret_cast<SwSerialGPSData *>(sData);
    
    root["Sensor"] = "GPS";

    if (gpsData->date.isValid() && gpsData->time.isValid()) {
        root["Time"] = FormatDateTime(gpsData->date,gpsData->time);
    }

    if (gpsData->location.isValid()) {
        root["Latitude"].set(gpsData->location.lat(), 5);
        root["Longitude"].set(gpsData->location.lng(), 5);
        ready = true;
    } else {
        ready = false;
    }

    if (gpsData->altitude.isValid()) {
        root["Altitude"] = gpsData->altitude.meters();
    }

    if (gpsData->speed.isValid()) {
        root["Speed"] = gpsData->speed.kmph();
    }

    if (gpsData->satellites.isValid()) {
        root["Satellites"] = gpsData->satellites.value();
    }

    root.printTo(response, sizeof(response));
    return response;
}
