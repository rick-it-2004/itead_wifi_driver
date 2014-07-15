/*
 * HttpClient.cpp
 *
 *  Created on: May 11, 2014
 *      Author: Richard
 */

#include <HttpClient.h>

#include "IteadWifiShieldDriver.h"
#include "Logger.h"

#include <ParseUtilities.h>

static IteadWifiShieldDriver wifiDriver;

HttpClient::HttpClient()
{
    // TODO Auto-generated constructor stub

}

String HttpClient::httpGet(const String & url)
{
    char command[100];
    char response[1002];

    strcpy(command, "at+http_get=");
    strcat(command, url.c_str());
    strcat(command, "\r\n");

    if (wifiDriver.sendAtCommand(command, response, 20))
    {
        uint16_t status = convertHexToUint16(response[3], response[4]);
        uint16_t length = convertHexToUint16(response[5], response[6]);

        log(INFO, String("status = ") + status);
        log(INFO, String("length = ") + length);
        return response;
    }
    else
    {
        const String errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            log(ERROR, String("unknown error ") +  errorCode);
        }
    }
    return "Error\r\n";
}

HttpClient::~HttpClient()
{
    // TODO Auto-generated destructor stub
}

