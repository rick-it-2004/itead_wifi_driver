/*
 * IpNetwork.h
 *
 *  Created on: Feb 21, 2014
 *      Author: richard
 */

#ifndef IPNETWORK_H_
#define IPNETWORK_H_

#define MAC_STR_LENGTH 18
#define IP_STR_LENGTH  16
class IpNetwork
{
    char mac[MAC_STR_LENGTH];
    char ipAddress[IP_STR_LENGTH];
    char gateway[IP_STR_LENGTH];
    char dnsServer1[IP_STR_LENGTH];
    char dnsServer2[IP_STR_LENGTH];

public:
    IpNetwork();
    virtual ~IpNetwork();

    void getMac(char *mac);
    void setMac(char *mac);
    void getIpAddress(char *ipAddress);
    void setIpAddress(char *ipAddress);
    void getGateway(char *gateway);
    void setGateway(char *gateway);
    void getDnsServer1(char *dnsServer);
    void setDnsServer1(char *dnsServer);
    void getDnsServer2(char *dnsServer);
    void setDnsServer2(char *dnsServer);
};

#endif /* IPNETWORK_H_ */
