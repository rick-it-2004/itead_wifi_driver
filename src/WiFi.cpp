/*
 * IteadWifiShieldDriver.cpp
 */

#include "WiFi.h"
#include "IteadWifiShieldDriver.h"
#include "Logger.h"

#include <ParseUtilities.h>

#define MAX_NUMBER_OF_NETWORKS 9

static IteadWifiShieldDriver wifiDriver;
static WiFiNetwork          *networkList[MAX_NUMBER_OF_NETWORKS];
static IpNetwork             ipNetwork;
static uint8_t               networkCount;

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

    if (wifiDriver.sendAtCommand("at+version\r\n", response, 15))
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
        const String errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            log(ERROR, String("unknown error ") +  errorCode);
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
    char response[500]; // bug in the shield firmware, each get_scan gets bigger and uses up more memory
    log(DEBUG, "scan all channels 1-11");

    if (wifiDriver.sendAtCommand("at+scan=0\r\n", response, 5))
    {
        networkCount = convertHexToUint8((uint8_t)response[3], (uint8_t)response[4]);
        if (networkCount > MAX_NUMBER_OF_NETWORKS) networkCount = MAX_NUMBER_OF_NETWORKS;

        char getScanString[100];
        strcpy(getScanString, "at+get_scan=0\r\n"); // note, index can be 0-9 only

        // loop around getting the details for each network found
        for (int i = 1; i < networkCount; i++)
        {
            getScanString[12] = 0x30 + i;
            if (wifiDriver.sendAtCommand(getScanString, response, 46))
            {
                networkList[i] = new WiFiNetwork(response);
            }
        }
    }
    else
    {
        const String errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (errorCode.equals("01")) { Serial.println("Error 01 - cannot find available SSID"); return 0; }
            if (errorCode.equals("02")) { Serial.println("Error 02 - cannot find specified SSID"); return 0; }
            log(ERROR, String("unknown error ") + errorCode);
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

bool WiFi::SSID(uint8_t networkIndex, String & ssid)
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

bool WiFi::BSSID(uint8_t networkIndex, String & bssid)
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

boolean WiFi::begin(const String & ssid)
{
    char command[100];
    char response[100];
    log(INFO, "connect");

    strcpy(command, "at+connect=");
    strcat(command, ssid.c_str());
    strcat(command, "\r\n");

    if (wifiDriver.sendAtCommand(command, response, 4))
    {
        log(DEBUG, "connected");
        setDhcpClient(); // this is needed to get the DHCP servers set up correctly
        getIpNetwork();
        return true;
    }
    else
    {
        const String errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (errorCode.equals("01")) { Serial.println("Error 01 - No SSID is found"); return false; }
            if (errorCode.equals("02")) { Serial.println("Error 02 - connection failed"); return false; }
            log(ERROR, String("unknown error ") + errorCode);
        }
    }
    return false;
}

/*
 * Start Wifi connection with passphrase. The most secure supported mode will be automatically selected
 * param ssid: Pointer to the SSID string.
 * param passphrase: Passphrase. Valid characters in a passphrase, must be between ASCII 32-126 (decimal).
 */

boolean WiFi::begin(const String & ssid, const String & passphrase)
{
    char command[100];
    char response[100];

    log(DEBUG, "securely connect");

    strcpy(command, "at+psk=");
    strcat(command, passphrase.c_str());
    strcat(command, "\r\n");

    if (wifiDriver.sendAtCommand(command, response, 4))
    {
        log(DEBUG, "password set");

        strcpy(command, "at+connect=");
        strcat(command, ssid.c_str());
        strcat(command, "\r\n");

        if (wifiDriver.sendAtCommand(command, response, 4))
        {
            log(DEBUG, "connected");
            setDhcpClient(); // this is needed to get the DHCP servers set up correctly
            getIpNetwork();
            return true;
        }
        else
        {
            const String errorCode = response + 5;
            if (!decodeErrorValue(errorCode))
            {
                if (errorCode.equals("01")) { Serial.println("Error 01 - No SSID is found"); return false; }
                if (errorCode.equals("02")) { Serial.println("Error 02 - connection failed"); return false; }
                log(ERROR, String("unknown error ") + errorCode);
            }
        }
    }
    else
    {
        const String errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            log(ERROR, String("unknown error ") + errorCode);
        }
    }
    return false;
}


/*
 * Return Connection status.
 * return: one of the value defined in wl_status_t
 */

WiFi::wl_status_t WiFi::status()
{
    char response[100];

    log(DEBUG, "get wifi status");

    if (wifiDriver.sendAtCommand("at+con_status\r\n", response, 4))
    {
        log(DEBUG, "status OK");
        return WL_CONNECTED;
    }
    else
    {
        const String errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (errorCode.equals("01")) { Serial.println("Error 01 - No network connection"); return WL_DISCONNECTED; }
            log(ERROR, String("unknown error ") + errorCode);
        }

        return WL_DISCONNECTED;
    }
}


/*
 * return true if connected
 */

boolean isConnected()
{
    char response[100];

    log(DEBUG, "get wifi status");

    if (wifiDriver.sendAtCommand("at+con_status\r\n", response, 4))
    {
        log(DEBUG, "status OK");
        return true;
    }
    else
    {
        const String errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (errorCode.equals("01")) { Serial.println("Error 01 - No network connection"); return false; }
            log(ERROR, String("unknown error ") + errorCode);
        }
    }

    return false;
}


/*
 * Disconnect from the network
 * return: one value of wl_status_t enum
 */

WiFi::wl_status_t WiFi::disconnect(void)
{
    char response[100];

    log(DEBUG, "disconnect from wifi");

    if (wifiDriver.sendAtCommand("at+disc\r\n", response, 4))
    {
        log(DEBUG, "disconnected OK");
        return WL_DISCONNECTED;
    }
    else
    {
        const String errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (errorCode.equals("01")) { Serial.println("Error 01 - No network connection"); return WL_DISCONNECTED; }
            log(ERROR, String("unknown error ") + errorCode);
        }

        return WL_DISCONNECTED;
    }
}


/*
 * Get the interface MAC address.
 * return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
 */

const char * WiFi::macAddress(String & mac)
{
    return ipNetwork.getMac(mac);
}

/*
 * Get the interface IP address.
 * return: Ip address value
 */

IPAddress WiFi::localIP()
{
    return ipNetwork.getIpAddress();
}

/*
 * Get the interface subnet mask address.
 * return: subnet mask address value
 */

IPAddress WiFi::subnetMask()
{
    return ipNetwork.getSubnetMask();
}

/*
 * Get the gateway ip address.
 * return: gateway ip address value
 */

IPAddress WiFi::gatewayIP()
{
    return ipNetwork.getIpAddress();
}

/*
* Get the DNS Server 1 ip address.
* return: gateway ip address value
*/

IPAddress WiFi::dns1IP()
{
    return ipNetwork.getIpAddress();
}
/*
* Get the DNS Server 2 ip address.
* return: gateway ip address value
*/

IPAddress WiFi::dns2IP()
{
    return ipNetwork.getIpAddress();
}

/*
 * look up the IP address for the given FQDN.
 * return:  ip address value
 */

IPAddress WiFi::lookup(const String & FQDN)
{
    char command[100];
    char response[20];

    strcpy(command, "at+dns=");
    strcat(command, FQDN.c_str());
    strcat(command, "\r\n");

    if (wifiDriver.sendAtCommand(command, response, 11))
    {
        return IPAddress( convertHexToUint8(response[3], response[4]),
                          convertHexToUint8(response[5], response[6]),
                          convertHexToUint8(response[7], response[8]),
                          convertHexToUint8(response[9], response[10]) );
    }
    else
    {
        const String errorCode = response + 5;
        if (!decodeErrorValue(errorCode))
        {
            if (errorCode.equals("FD")) { Serial.println("Error FD - DNS receive error"); return IPAddress(0, 0, 0, 0); }
            if (errorCode.equals("FC")) { Serial.println("Error FC - DNS request failed"); return IPAddress(0, 0, 0, 0); }
            log(ERROR, String("unknown error ") + errorCode);
        }
    }
    return IPAddress(0, 0, 0, 0);
}

/*
   * pings the host once
   * return true if the ping worked
   */

  boolean WiFi::ping(const String & host)
  {
      char command[100];
      char response[100];

      log(DEBUG, "ping host");

      strcpy(command, "at+ping=");
      strcat(command, host.c_str());
      strcat(command, "\r\n");

      if (wifiDriver.sendAtCommand(command, response, 4))
      {
          log(DEBUG, "ping OK");
          return true;
      }
      else
      {
          const String errorCode = response + 5;
          if (!decodeErrorValue(errorCode))
          {
              if (errorCode.equals("01")) { Serial.println("Error 01 - Unable to access destination host"); return false; }
              log(ERROR, String("unknown error ") + errorCode);
          }

          return false;
      }
  }

  void WiFi::setDhcpClient()
  {

      char response[60];
      log(DEBUG, "set DHCP client");

      if (wifiDriver.sendAtCommand("at+ipdhcp=0\r\n", response, 30))
      {
          ipNetwork.setIpAddress( convertHexToUint8(response[3], response[4]),
                                  convertHexToUint8(response[5], response[6]),
                                  convertHexToUint8(response[7], response[8]),
                                  convertHexToUint8(response[9], response[10]) );

          ipNetwork.setSubnetMask( convertHexToUint8(response[11], response[12]),
                                   convertHexToUint8(response[13], response[14]),
                                   convertHexToUint8(response[15], response[16]),
                                   convertHexToUint8(response[17], response[18]) );

          ipNetwork.setGateway( convertHexToUint8(response[19], response[20]),
                                convertHexToUint8(response[21], response[22]),
                                convertHexToUint8(response[23], response[24]),
                                convertHexToUint8(response[25], response[26]) );

          ipNetwork.setDnsServer1( convertHexToUint8(response[27], response[28]),
                                   convertHexToUint8(response[29], response[30]),
                                   convertHexToUint8(response[31], response[32]),
                                   convertHexToUint8(response[33], response[34]) );

          ipNetwork.setDnsServer2( convertHexToUint8(response[35], response[36]),
                                   convertHexToUint8(response[37], response[38]),
                                   convertHexToUint8(response[39], response[40]),
                                   convertHexToUint8(response[41], response[42]) );
      }
      else
      {
          ipNetwork.setMac("00:00:00:00:00:00");
          ipNetwork.setIpAddress(0, 0, 0, 0);
          ipNetwork.setGateway(0, 0, 0, 0);
          ipNetwork.setDnsServer1(0, 0, 0, 0);
          ipNetwork.setDnsServer2(0, 0, 0, 0);

          const String errorCode = response + 5;
          if (!decodeErrorValue(errorCode))
          {
              if (errorCode.equals("01")) { Serial.println("Error 01 - failed to get IP address"); }
              log(ERROR, String("unknown error ") + errorCode);
          }
      }
  }

  void WiFi::getIpNetwork()
  {
      char response[60];
      log(DEBUG, "get the IP Network details");

      if (wifiDriver.sendAtCommand("at+ipconfig\r\n", response, 30))
      {
          char tmpStr[20];
          memset(tmpStr, '\0', 20);

          tmpStr[0]  = response[3];  tmpStr[1]  = response[4];  tmpStr[2]   = ':';
          tmpStr[3]  = response[5];  tmpStr[4]  = response[6];  tmpStr[5]   = ':';
          tmpStr[6]  = response[7];  tmpStr[7]  = response[8];  tmpStr[8]   = ':';
          tmpStr[9]  = response[9];  tmpStr[10] = response[10]; tmpStr[11]  = ':';
          tmpStr[12] = response[11]; tmpStr[13] = response[12]; tmpStr[14]  = ':';
          tmpStr[15] = response[13]; tmpStr[16] = response[14];
          ipNetwork.setMac(tmpStr);

          ipNetwork.setIpAddress( convertHexToUint8(response[15], response[16]),
                                  convertHexToUint8(response[17], response[18]),
                                  convertHexToUint8(response[19], response[20]),
                                  convertHexToUint8(response[21], response[22]) );

          ipNetwork.setSubnetMask( convertHexToUint8(response[23], response[24]),
                                   convertHexToUint8(response[25], response[26]),
                                   convertHexToUint8(response[27], response[28]),
                                   convertHexToUint8(response[29], response[30]) );

          ipNetwork.setGateway( convertHexToUint8(response[31], response[32]),
                                convertHexToUint8(response[33], response[34]),
                                convertHexToUint8(response[35], response[36]),
                                convertHexToUint8(response[37], response[38]) );

          ipNetwork.setDnsServer1( convertHexToUint8(response[39], response[40]),
                                   convertHexToUint8(response[41], response[42]),
                                   convertHexToUint8(response[43], response[44]),
                                   convertHexToUint8(response[45], response[46]) );

          ipNetwork.setDnsServer2( convertHexToUint8(response[47], response[48]),
                                   convertHexToUint8(response[49], response[50]),
                                   convertHexToUint8(response[51], response[52]),
                                   convertHexToUint8(response[53], response[54]) );
      }
      else
      {
          ipNetwork.setMac("00:00:00:00:00:00");
          ipNetwork.setIpAddress(0, 0, 0, 0);
          ipNetwork.setGateway(0, 0, 0, 0);
          ipNetwork.setDnsServer1(0, 0, 0, 0);
          ipNetwork.setDnsServer2(0, 0, 0, 0);

          const String errorCode = response + 5;
          if (!decodeErrorValue(errorCode))
          {
              if (errorCode.equals("01")) { Serial.println("Error 01 - failed to get IP address"); }
              log(ERROR, String("unknown error ") + errorCode);
          }
      }
  }
