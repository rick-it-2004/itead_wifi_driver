/*
 * IteadWifiShieldDriver.h
 */

#ifndef _ITEAD_WIFI_SHIELD_DRIVER_H_
#define _ITEAD_WIFI_SHIELD_DRIVER_H_

#include <Arduino.h>

class IteadWifiShieldDriver
{
public:
    IteadWifiShieldDriver();
    virtual ~IteadWifiShieldDriver();

    /*
     * blocks until the WiFi board is available
     */

    void waitForAvailability();

    /*
     * Function used to send an AT command to the WiFi board and get response
     * param command : AT command terminated with "\r\n"
     * param response : pointer to the String to receive the response from the WiFi board
     * return: true if return is "OK", and false of "ERROR"
     */
    bool sendAtCommand(String command, char *response, unsigned int responseLength);
};

#endif /* _ITEAD_WIFI_SHIELD_DRIVER_H_ */
