
#include <Arduino.h>
#include "WiFi.h"
#include "Logger.h"

void printStatus (WiFi::wl_status_t status);

int aLed = 0x0D;

static WiFi wifiShield;

String ssidHome       = "xxxxxxxxxxxxx";
String ssidUrbanPad   = "yyyyyyyyyyyyy";
String testLocalPing  = "192.168.1.1";
String testRemotePing = "8.8.8.8";

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
    int numberOfNetworks = wifiShield.scanNetworks();

    log(INFO, String("Number of networks detected = ") + numberOfNetworks);
    log(INFO, "Look for either the home or the urban pad SSID");

    String networkSsid;

    for (int networkIndex = 1; networkIndex < numberOfNetworks; networkIndex++)
    {
        if (wifiShield.SSID(networkIndex, networkSsid) == true)
        {
            networkSsid.trim();

            log(INFO, networkSsid + "<--- SSID being compared");

            if (networkSsid.equals(ssidHome))
            {
                log(INFO, "started on home");
                if (wifiShield.begin(ssidHome) == true)
                {
                    log(INFO, "SUCCESS!");
                    break;
                }
            }
            else if (networkSsid.equals(ssidUrbanPad))
            {
                log(INFO, "started on urban pad");
                if (wifiShield.begin(ssidUrbanPad, "12341234") == true)
                {
                    log(INFO, "SUCCESS!");
                    break;
                }
            }
            else
            {
                log(INFO, "skip this network");
            }
        }
    }
} /* End of setup */

void loop(void)
{
    digitalWrite(aLed, HIGH);
    delay(500);
    digitalWrite(aLed, LOW);
    delay(2500);
    printStatus(wifiShield.status());
    wifiShield.ping(testLocalPing);
    wifiShield.ping(testRemotePing);
} /* End of loop */

void printStatus (WiFi::wl_status_t status)
{
    switch (status)
    {
        case WiFi::WL_NO_SHIELD:       Serial.println("connection status WL_NO_SHIELD");       break;
        case WiFi::WL_IDLE_STATUS:     Serial.println("connection status WL_IDLE_STATUS");     break;
        case WiFi::WL_NO_SSID_AVAIL:   Serial.println("connection status WL_NO_SSID_AVAIL");   break;
        case WiFi::WL_SCAN_COMPLETED:  Serial.println("connection status WL_SCAN_COMPLETED");  break;
        case WiFi::WL_CONNECTED:       Serial.println("connection status WL_CONNECTED");       break;
        case WiFi::WL_CONNECT_FAILED:  Serial.println("connection status WL_CONNECT_FAILED");  break;
        case WiFi::WL_CONNECTION_LOST: Serial.println("connection status WL_CONNECTION_LOST"); break;
        case WiFi::WL_DISCONNECTED:    Serial.println("connection status WL_DISCONNECTED");    break;
        default:                       Serial.println("connection status unknown");            break;
     }
}

