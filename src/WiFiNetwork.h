/*
 * WiFiNetwork.h
 *
 *  Created on: Feb 21, 2014
 *      Author: richard
 */

#ifndef WIFINETWORK_H_
#define WIFINETWORK_H_

class WiFiNetwork
{
    char    ssid[34];
    char    bssid[7];
    uint8_t channel;
    uint8_t rssi;
    uint8_t security;

public:
    WiFiNetwork();
    WiFiNetwork(char *description);
    virtual ~WiFiNetwork();

    void    getSsid(char *ssid);
    void    getBssid(char *bssid);
    uint8_t getChannel();
    uint8_t getRssi();
};

#endif /* WIFINETWORK_H_ */
