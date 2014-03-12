#ifndef WIFI_H_
#define WIFI_H_

#include <Arduino.h>
#include "WiFiNetwork.h"
#include "IpNetwork.h"

/*
#include <inttypes.h>

extern "C" {
	#include "utility/wl_definitions.h"
	#include "utility/wl_types.h"
}

#include "IPAddress.h"
#include "WiFiClient.h"
#include "WiFiServer.h"
*/

class WiFi
{
private:

public:

//    static int16_t 	_state[MAX_SOCK_NUM];
//    static uint16_t _server_port[MAX_SOCK_NUM];

    WiFi();
    virtual ~WiFi();

    /*
     * blocks until the WiFi board is available
     */

    void waitForAvailability();

    /*
     * Get firmware version from the WiFi board
     */

    String firmwareVersion();

    /*
     * Start scan WiFi networks available
     * return: Number of discovered networks
     */

    int8_t scanNetworks();

    /*
     * function to get the network at the given index
     * param networkIndex: specify from which network item want to get the information (0 to networkCount-1)
     * return: the WiFI network at that index
     */

    WiFiNetwork * getWiFiNetwork(uint8_t networkIndex);

    /*
     * gets the SSID discovered during the network scan.
     * param networkIndex: specify from which network item want to get the information (0 to networkCount-1)
     * param ssid: pointer to the memory into which the SSID will be copies (at least 34 char long)
     * return: bool, true if successful, false if the networkIndex is not valid
     */

    bool SSID(uint8_t networkIndex, char * ssid);

    /*
     * gets the BSSID discovered during the network scan.
     * param networkIndex: specify from which network item want to get the information (0 to networkCount-1)
     * param bssid: pointer to the memory into which the BSSID will be copies (at least 7 char long)
     * return: bool, true if successful, false if the networkIndex is not valid
     */

    bool BSSID(uint8_t networkIndex, char * bssid);

    /*
     * gets the channel discovered during the network scan.
     * param networkIndex: specify from which network item want to get the information (0 to networkCount-1)
     * return: uint8_t, the channel 1-11 if successful, 0 if the networkIndex is not valid
     */

    uint8_t channel(uint8_t networkIndex);

    /*
     * gets the RSSI discovered during the network scan.
     * param networkIndex: specify from which network item want to get the information (0 to networkCount-1)
     * return: uint8_t, the RSSI if successful, 0 if the networkIndex is not valid. returned as a positive
     *                  number, but should be understood to be negative
     */

    uint8_t RSSI(uint8_t networkIndex);

    /*
     * Return the encryption type of the networks discovered during the scanNetworks
     * param networkIndex: specify from which network item want to get the information(0 to NetworkCount-1)
     * return: encryption type (enum wl_enc_type) of the specified item on the networks scanned list
     */

    uint8_t encryptionType(uint8_t networkIdex);

    // Encryption type is a bitmap
    // bit7      bit6     bit5    bit4        bit3    bit2    bit1     bit0
    // WPA2      WPA      WEP     802.1X      PSK     WEP     TKIP     CCMP

    #define  WPA2               0x80
    #define  WPA                0x40
    #define  WEP                0x20
    #define  KEY_DIST_8021X     0x10
    #define  KEY_DIST_PSK       0x08
    #define  ENC_PROTOCOL_WEP   0x04
    #define  ENC_PROTOCOL_TKIP  0x02
    #define  ENC_PROTOCOL_CCMP  0x01

    /*
     * Start Wifi connection for OPEN networks
     * param ssid: Pointer to the SSID string.
     */

    void begin(char* ssid);

    /*
     * Start Wifi connection with passphrase. The most secure supported mode will be automatically selected
     * param ssid: Pointer to the SSID string.
     * param passphrase: Passphrase. Valid characters in a passphrase, must be between ASCII 32-126 (decimal).
     */

    void begin(char* ssid, const char *passphrase);

    /*
     * Return Connection status.
     * return: one of the value defined in wl_status_t
     */

    typedef enum
    {
        WL_NO_SHIELD       = 255,
        WL_IDLE_STATUS     = 0,
        WL_NO_SSID_AVAIL,
        WL_SCAN_COMPLETED,
        WL_CONNECTED,
        WL_CONNECT_FAILED,
        WL_CONNECTION_LOST,
        WL_DISCONNECTED
    } wl_status_t;

    wl_status_t status();

    /*
     * Disconnect from the network
     * return: one value of wl_status_t enum
     */

    wl_status_t disconnect();

    /*
     * function to get the IP network
     * return: the IP network
     */

    IpNetwork * getIpNetwork();

    /*
     * Get the interface MAC address.
     * return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
     */

    uint8_t* macAddress(uint8_t* mac);

    /*
     * Get the interface IP address.
     * return: Ip address value
     */

    IPAddress localIP();

    /*
     * Get the interface subnet mask address.
     * return: subnet mask address value
     */

    IPAddress subnetMask();

    /*
     * Get the gateway ip address.
     * return: gateway ip address value
     */

   IPAddress gatewayIP();

   /*
    * Get the DNS Server 1 ip address.
    * return: gateway ip address value
    */

  IPAddress dns1IP();
  /*
   * Get the DNS Server 2 ip address.
   * return: gateway ip address value
   */

 IPAddress dns2IP();




    /*
     * Get the first socket available
     */
//    static uint8_t getSocket();



    /* Start Wifi connection with WEP encryption.
     * Configure a key into the device. The key type (WEP-40, WEP-104)
     * is determined by the size of the key (5 bytes for WEP-40, 13 bytes for WEP-104).
     *
     * param ssid: Pointer to the SSID string.
     * param key_idx: The key index to set. Valid values are 0-3.
     * param key: Key input buffer.
     */
 //   int begin(char* ssid, uint8_t key_idx, const char* key);


    /* Change Ip configuration settings disabling the dhcp client
        *
        * param local_ip: 	Static ip configuration
        */
 //   void config(IPAddress local_ip);

    /* Change Ip configuration settings disabling the dhcp client
        *
        * param local_ip: 	Static ip configuration
	* param dns_server:     IP configuration for DNS server 1
        */
 //   void config(IPAddress local_ip, IPAddress dns_server);

    /* Change Ip configuration settings disabling the dhcp client
        *
        * param local_ip: 	Static ip configuration
	* param dns_server:     IP configuration for DNS server 1
        * param gateway : 	Static gateway configuration
        */
 //   void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway);

    /* Change Ip configuration settings disabling the dhcp client
        *
        * param local_ip: 	Static ip configuration
	* param dns_server:     IP configuration for DNS server 1
        * param gateway: 	Static gateway configuration
        * param subnet:		Static Subnet mask
        */
 //   void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet);

    /* Change DNS Ip configuration
     *
     * param dns_server1: ip configuration for DNS server 1
     */
 //   void setDNS(IPAddress dns_server1);

    /* Change DNS Ip configuration
     *
     * param dns_server1: ip configuration for DNS server 1
     * param dns_server2: ip configuration for DNS server 2
     *
     */
 //   void setDNS(IPAddress dns_server1, IPAddress dns_server2);




    /*
     * Resolve the given hostname to an IP address.
     * param aHostname: Name to be resolved
     * param aResult: IPAddress structure to store the returned IP address
     * result: 1 if aIPAddrString was successfully converted to an IP address,
     *          else error code
     */
 //   int hostByName(const char* aHostname, IPAddress& aResult);

//    friend class WiFiClient;
 //   friend class WiFiServer;
};

#endif
