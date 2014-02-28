
#include <Arduino.h>
#include "WiFi.h"
#include "Logger.h"

int aLed = 0x0D;

static WiFi wifiShield;

char ssid[] = "george family";     // the name of your network

void readUart0();

void setup(void)
{
    setLogLevel(INFO);
    pinMode(aLed, OUTPUT);
    digitalWrite(aLed, LOW);

    Serial.begin(9600);
    Serial.flush();
    Serial.println("reboot wifi shield");
    wifiShield.waitForAvailability();
    Serial.print(wifiShield.firmwareVersion());
    log(INFO, "WiFi board running");
    int numberNetworks = wifiShield.scanNetworks();
    Serial.print("Number of WiFi networks is ");
    Serial.println(numberNetworks);
} /* End of setup */

void loop(void)
{
    digitalWrite(aLed, HIGH);
    delay(500);
    digitalWrite(aLed, LOW);
    delay(500);

} /* End of loop */


void readUart0() // read from port 0, send to port 0:
{
    String str = "";
    str = Serial.readString();

    if (str.startsWith("1")) str = "at+scan=0\r\n";
    else if (str.startsWith("2")) str = "at+get_scan=11\r\n";
    else str = "rubbish \r\n";

    Serial.print("sending " + str);
}






