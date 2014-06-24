/*
 * WiFiNetwork.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: richard
 */

#include <Arduino.h>
#include "WiFiNetwork.h"
#include "Logger.h"
#include <ParseUtilities.h>

WiFiNetwork::WiFiNetwork()
{
    strcpy(ssid, "Empty");
    strcpy(bssid, "Empty");
    channel = 0;
    rssi = -100;
    security = 0x00;
}

WiFiNetwork::WiFiNetwork(char * description)
{
    memset(ssid, 0x00, 34);

    for(int i = 3, j = 0 ; i < 69; i+=2, j++)
    {
        ssid[j] = convertHexToAscii((uint8_t)description[i], (uint8_t)description[i + 1]);
    }

    memset(bssid, 0x00, 7);
    for(int i = 69, j = 0 ; i < 83; i+=2, j++)
    {
        bssid[j] = convertHexToAscii((uint8_t)description[i], (uint8_t)description[i + 1]);
    }

    channel = convertHexToUint8((uint8_t)description[83], (uint8_t)description[84]);
    rssi = convertHexToUint8((uint8_t)description[85], (uint8_t)description[86]);
    security = convertHexToUint8((uint8_t)description[87], (uint8_t)description[88]);

    log(INFO, ssid);
}

void WiFiNetwork::getSsid(String & ssid)
{
    ssid = this->ssid;
}

void WiFiNetwork::getBssid(String & bssid)
{
    bssid = this->bssid;
}

uint8_t WiFiNetwork::getChannel()
{
    return this->channel;
}

uint8_t WiFiNetwork::getRssi()
{
    return this->rssi;
}

WiFiNetwork::~WiFiNetwork()
{
    // TODO Auto-gntenerated destructor stub
}

