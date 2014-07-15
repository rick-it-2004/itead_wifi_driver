
#include <Arduino.h>
#include "WiFi.h"
#include "HttpClient.h"
#include "Logger.h"
#include "ParseUtilities.h"

void printStatus (WiFi::wl_status_t status);

int aLed = 0x0D;

static WiFi wifiShield;
static HttpClient client;

String ssidHome       = "fred";
String ssidUrbanPad   = "freds other";
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
    boolean connected = false;

    for (int networkIndex = 0; networkIndex < numberOfNetworks; networkIndex++)
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
                    connected = true;
                    break;
                }
            }
            else if (networkSsid.equals(ssidUrbanPad))
            {
                log(INFO, "started on urban pad");
                if (wifiShield.begin(ssidUrbanPad, "34563456") == true)
                {
                    log(INFO, "SUCCESS!");
                    connected = true;
                    break;
                }
            }
            else
            {
                log(INFO, "skip this network");
            }
        }
    }

    if (connected == true)
    {
        log(DEBUG,"system initialised");
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

    IPAddress host = wifiShield.lookup("www.bbc.co.uk");
    String destination = convertUint8ToAscii(host[0]) + "." +
                         convertUint8ToAscii(host[1]) + "." +
                         convertUint8ToAscii(host[2]) + "." +
                         convertUint8ToAscii(host[3]);
    log(INFO, client.httpGet(destination));

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

