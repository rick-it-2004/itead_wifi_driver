/*
 * WiFiDriver.h
 */

#ifndef WIFIDRIVER_H_
#define WIFIDRIVER_H_

#include <Arduino.h>

class WiFiDriver
{
public:
    WiFiDriver();
    virtual ~WiFiDriver();

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
    bool sendAtCommand(String command, char *response);
};

#endif /* WIFIDRIVER_H_ */
