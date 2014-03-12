/*
 * WiFi.cpp
 */

#include "WiFi.h"
#include "WiFiDriver.h"
#include "Logger.h"

#include <ParseUtilities.h>

#define MAX_NUMBER_OF_NETWORKS 9

static WiFiDriver   wifiDriver;
static WiFiNetwork *networkList[MAX_NUMBER_OF_NETWORKS];
static IpNetwork   *ipNetwork;
static uint8_t      networkCount;

WiFi::WiFi()
{
    networkCount = 0;
}

WiFi::~WiFi()
{

}

void WiFi::waitForAvailability()
{
    wifiDriver.waitForAvailability();
}

String WiFi::firmwareVersion ()
{
    String version;
    char response[100];

    log(DEBUG, "read firmware version for WiFi shield");

    if (wifiDriver.sendAtCommand("at+version\r\n", response))
    {
        int i = 3;
        version  = "Host version ";
        for( ; !((response[i] == 0x30) && (response[i+1] == 0x30)); i+=2)
        {
            version = version + convertHexToAscii((uint8_t)response[i], (uint8_t)response[i + 1]);
        }
        version.concat("\r\n");
        version.concat("WLAN version ");
        i+=2;
        for( ; !((response[i] == 0x30) && (response[i+1] == 0x30)); i+=2)
        {
            version = version + convertHexToAscii((uint8_t)response[i], (uint8_t)response[i + 1]);
        }
        version.concat("\r\n");
    }
    else
    {
        char * errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            log(ERROR, String(strcat("unknown error ", errorCode)));
        }
        version ="Error\r\n";
    }
    return version;
}

/*
 * Start scan WiFi networks available
 * return: Number of discovered networks
 */

int8_t WiFi::scanNetworks()
{
    networkCount = 0;
    String number;
    char response[100];
    log(DEBUG, "scan all channels 1-11");

    if (wifiDriver.sendAtCommand("at+scan=0\r\n", response))
    {
        networkCount = convertHexToUint8((uint8_t)response[3], (uint8_t)response[4]);
        if (networkCount > MAX_NUMBER_OF_NETWORKS) networkCount = MAX_NUMBER_OF_NETWORKS;

        char getScanString[25];
        strcpy(getScanString, "at+get_scan=0\r\n"); // note, index can be 0-9 only

        // loop around getting the details for each network found
        for (int i = 1; i < networkCount+ 1; i++)
        {
            getScanString[12] = 0x30 + i;
            if (wifiDriver.sendAtCommand(getScanString, response))
            {
                networkList[i] = new WiFiNetwork(response);
            }
        }
    }
    else
    {
        char * errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (strcmp("01", errorCode)) { Serial.println("cannot find available SSID"); return 0; }
            if (strcmp("02", errorCode)) { Serial.println("cannot find specified SSID"); return 0; }
            log(ERROR, String(strcat("unknown error ", errorCode)));
        }
    }
    return networkCount;
}

/*
 * function to get the network at the given index
 * param networkIndex: specify from which network item want to get the information (0 to networkCount-1)
 * return: the WiFI network at that index
 */

WiFiNetwork * getWiFiNetwork(uint8_t networkIndex)
{
    if (networkIndex >= networkCount) return NULL;
    return networkList[networkIndex];
}

/*
 * gets the SSID discovered during the network scan.
 * param networkIndex: specify from which network item want to get the information (0 - networkCount -1)
 * param ssid: pointer to the memory into which the SSID will be copied (at least 34 char long)
 * return: bool, true if successful, false if the networkIndex is not valid
 */

bool WiFi::SSID(uint8_t networkIndex, char * ssid)
{
    if (networkIndex >= networkCount) return false;
    networkList[networkIndex]->getSsid(ssid);
    return true;
}

/*
 * gets the BSSID discovered during the network scan.
 * param networkIndex: specify from which network item want to get the information (0 - networkCount -1)
 * param ssid: pointer to the memory into which the BSSID will be copied (at least 7 char long)
 * return: bool, true if successful, false if the networkIndex is not valid
 */

bool WiFi::BSSID(uint8_t networkIndex, char * bssid)
{
    if (networkIndex >= networkCount) return false;
    networkList[networkIndex]->getBssid(bssid);
    return true;
}

/*
 * gets the channel discovered during the network scan.
 * param networkIndex: specify from which network item want to get the information (0 - networkCount -1)
 * return: uint8_t, the channel 1-11 if successful, 0 if the networkIndex is not valid
 */

uint8_t WiFi::channel(uint8_t networkIndex)
{
    if (networkIndex >= networkCount) return 0;
    return networkList[networkIndex]->getChannel();
}

/*
 * gets the RSSI discovered during the network scan.
 * param networkIndex: specify from which network item want to get the information (0 - networkCount -1)
 * return: uint8_t, the RSSI if successful, 0 if the networkIndex is not valid. returned as a positive
 *                  number, but should be understood to be negative
 */

uint8_t WiFi::RSSI(uint8_t networkIndex)
{
    if (networkIndex >= networkCount) return 0;
    return networkList[networkIndex]->getRssi();
}

/*
 * Start Wifi connection for OPEN networks
 * param ssid: Pointer to the SSID string.
 */

void WiFi::begin(char* ssid)
{
    char command[100];
    char response[100];
    Serial.println("connect");

    strcpy(command, "at+connect=");
    strcat(command, ssid);
    strcat(command, "\r\n");

    if (wifiDriver.sendAtCommand(command, response))
    {
        log(DEBUG, "connected");
    }
    else
    {
        char * errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (strcmp("01", errorCode)) { Serial.println("No SSID is found"); return; }
            if (strcmp("02", errorCode)) { Serial.println("connection failed"); return; }
            log(ERROR, strcat("unknown error ", errorCode));
        }
    }
}

/*
 * Start Wifi connection with passphrase. The most secure supported mode will be automatically selected
 * param ssid: Pointer to the SSID string.
 * param passphrase: Passphrase. Valid characters in a passphrase, must be between ASCII 32-126 (decimal).
 */

void WiFi::begin(char* ssid, const char *passphrase)
{
    char command[100];
    char response[100];

    log(DEBUG, "securely connect");

    strcpy(command, "at+psk=");
    strcat(command, passphrase);
    strcat(command, "\r\n");

    if (wifiDriver.sendAtCommand(command, response))
    {
        log(DEBUG, "password set");

        strcpy(command, "at+connect=");
        strcat(command, ssid);
        strcat(command, "\r\n");

        if (wifiDriver.sendAtCommand(command, response))
        {
            log(DEBUG, "connected");
        }
        else
        {
            char * errorCode = response + 5;
            if (!decodeErrorValue(errorCode))
            {
                if (strcmp("01", errorCode)) { Serial.println("No SSID is found"); return; }
                if (strcmp("02", errorCode)) { Serial.println("connection failed"); return; }
                log(ERROR, strcat("unknown error ", errorCode));
            }
        }
    }
    else
    {
        char * errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            log(ERROR, strcat("unknown error ", errorCode));
        }
    }
}


/*
 * Return Connection status.
 * return: one of the value defined in wl_status_t
 */

WiFi::wl_status_t WiFi::status()
{
    char response[100];

    log(DEBUG, "get wifi status");

    if (wifiDriver.sendAtCommand("at+con_status\r\n", response))
    {
        log(DEBUG, "status OK");
        return WL_CONNECTED;
    }
    else
    {
        char * errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (strcmp("01", errorCode)) { Serial.println("No network connection"); return WL_DISCONNECTED; }
            log(ERROR, strcat("unknown error ", errorCode));
        }

        return WL_DISCONNECTED;
    }
}

/*
 * Disconnect from the network
 * return: one value of wl_status_t enum
 */

WiFi::wl_status_t WiFi::disconnect(void)
{
    char response[100];

    log(DEBUG, "disconnect from wifi");

    if (wifiDriver.sendAtCommand("at+disc\r\n", response))
    {
        log(DEBUG, "disconnected OK");
        return WL_DISCONNECTED;
    }
    else
    {
        char * errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (strcmp("01", errorCode)) { Serial.println("No network connection"); return WL_DISCONNECTED; }
            log(ERROR, strcat("unknown error ", errorCode));
        }

        return WL_DISCONNECTED;
    }
}

/*
 * function to get the IP network
 * return: the IP network
 */

IpNetwork WiFi::getIpNetwork()
{
    char response[100];
    log(DEBUG, "get the IP Network details");

    if (wifiDriver.sendAtCommand("at+ipconfig\r\n", response))
    {
        char tmpStr[WL_MAC_ADDR_LENGTH];

        convertUint8ToAscii(convertHexToUint8(response[3]), &(tmpStr[0]));
        tmpStr[2]  = ':';
        convertUint8ToAscii(convertHexToUint8(response[4]), &(tmpStr[3]));
        tmpStr[5]  = ':';
        convertUint8ToAscii(convertHexToUint8(response[5]), &(tmpStr[6]));
        tmpStr[8]  = ':';
        convertUint8ToAscii(convertHexToUint8(response[6]), &(tmpStr[9]));
        tmpStr[11]  = ':';
        convertUint8ToAscii(convertHexToUint8(response[7]), &(tmpStr[12]));
        tmpStr[14]  = ':';
        convertUint8ToAscii(convertHexToUint8(response[8]), &(tmpStr[15]));
        ipNetwork->setMac(tmpStr);

        ipNetwork->setIpAddress( convertHexToUint8(response[9]),
                                 convertHexToUint8(response[10]),
                                 convertHexToUint8(response[11]),
                                 convertHexToUint8(response[12]) );

        ipNetwork->setGateway( convertHexToUint8(response[13]),
                               convertHexToUint8(response[14]),
                               convertHexToUint8(response[15]),
                               convertHexToUint8(response[16]) );

        ipNetwork->setDnsServer1( convertHexToUint8(response[17]),
                                  convertHexToUint8(response[18]),
                                  convertHexToUint8(response[19]),
                                  convertHexToUint8(response[20]) );

        ipNetwork->setDnsServer2( convertHexToUint8(response[21]),
                                  convertHexToUint8(response[22]),
                                  convertHexToUint8(response[23]),
                                  convertHexToUint8(response[24]) );
    }
    else
    {
        ipNetwork->setMac("00:00:00:00:00:00");
        ipNetwork->setIpAddress(0, 0, 0, 0);
        ipNetwork->setGateway(0, 0, 0, 0);
        ipNetwork->setDnsServer1(0, 0, 0, 0);
        ipNetwork->setDnsServer2(0, 0, 0, 0);

        char * errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (strcmp("01", errorCode)) { Serial.println("failed to get IP address"); return *ipNetwork; }
            log(ERROR, String(strcat("unknown error ", errorCode)));
        }
    }

    return *ipNetwork;
}

/*
 * Get the interface MAC address.
 * return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
 */

char * WiFi::macAddress(char * mac)
{
    return ipNetwork->getMac(mac);
}

/*
 * Get the interface IP address.
 * return: Ip address value
 */

IPAddress WiFi::localIP()
{
    return ipNetwork->getIpAddress();
}

/*
 * Get the interface subnet mask address.
 * return: subnet mask address value
 */

IPAddress WiFi::subnetMask()
{
    return ipNetwork->getSubnetMask();
}

/*
 * Get the gateway ip address.
 * return: gateway ip address value
 */

IPAddress WiFi::gatewayIP()
{
    return ipNetwork->getIpAddress();
}

/*
* Get the DNS Server 1 ip address.
* return: gateway ip address value
*/

IPAddress WiFi::dns1IP()
{
    return ipNetwork->getIpAddress();
}
/*
* Get the DNS Server 2 ip address.
* return: gateway ip address value
*/

IPAddress WiFi::dns2IP()
{
    return ipNetwork->getIpAddress();
}





