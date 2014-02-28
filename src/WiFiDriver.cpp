/*
 * WiFiDriver.cpp
 */

#include <WiFiDriver.h>
#include <Arduino.h>
#include <string.h>

#define WIFI_TX_PIN  5
#define WIFI_RX_PIN  6
#define UART3_TX    14
#define UART3_RX    15

WiFiDriver::WiFiDriver()
{
    pinMode(WIFI_TX_PIN, INPUT_PULLUP);
    pinMode(WIFI_RX_PIN, INPUT_PULLUP);
    pinMode(UART3_TX, OUTPUT);
    pinMode(UART3_RX, INPUT);
    Serial3.begin(115200);
    Serial3.flush();
}

WiFiDriver::~WiFiDriver()
{
    // TODO Auto-generated destructor stub
}

void WiFiDriver::waitForAvailability()
{
    do
    {
        delay(500);
    } while (Serial3.available()== 0);

    String str = Serial3.readString();
    Serial.print(str);
}


bool WiFiDriver::sendAtCommand(String command, char *response)
{
    char tmpStr[5];

    Serial.println("send " + command);

    Serial3.print(command);
    String str = Serial3.readString();

    // FOR DEBUG PURPOSES
    for(int i = 0; !((str.charAt(i) == '\r') && (str.charAt(i+1) == '\n')); i++)
    {
        sprintf(tmpStr, "%02X ", (str.charAt(i) & 0x00FF));
        Serial.print(tmpStr);
    }
    Serial.print("\r\n");
    // END DEBUG

    if (str.startsWith("OK") > 0)
    {
        strcpy(response, "OK ");
        unsigned int length = 2;

        while (str.length() > length)
        {
            sprintf(tmpStr, "%02X", (str.charAt(length) & 0x00FF));
            strcat(response, tmpStr);
            length++;
        }
        Serial.println(response);
        return true;
    }
    else
    {
        strcpy(response, "ERROR ");
        sprintf(tmpStr, "%02X", (str.charAt(5) & 0x00FF));
        strcat(response, tmpStr);
        Serial.println(response);
        return false;
    }
}

