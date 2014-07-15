/*
 * IpNetwork.h
 *
 *  Created on: Feb 21, 2014
 *      Author: richard
 */

#ifndef IPNETWORK_H_
#define IPNETWORK_H_

#include <IPAddress.h>
#include "ParseUtilities.h"

#define WL_MAC_ADDR_LENGTH 20

class IpNetwork
{
private:
    char mac[WL_MAC_ADDR_LENGTH];
    IPAddress *ipAddress;
    IPAddress *subnetMask;
    IPAddress *gateway;
    IPAddress *dnsServer1;
    IPAddress *dnsServer2;

public:
    IpNetwork();
    virtual ~IpNetwork();

    const char * getMac(String & mac);
    void setMac(const char *mac);
    IPAddress getIpAddress();
    void setIpAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress getSubnetMask();
    void setSubnetMask(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress getGateway();
    void setGateway(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress getDnsServer1();
    void setDnsServer1(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress getDnsServer2();
    void setDnsServer2(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
};

#endif /* IPNETWORK_H_ */
