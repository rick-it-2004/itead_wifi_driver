/*
 * IpNetwork.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: richard
 */

#include <Arduino.h>
#include "IpNetwork.h"
#include <ParseUtilities.h>

IpNetwork::IpNetwork()
{
    memset(mac, '\0', MAC_STR_LENGTH);
    memset(ipAddress, '\0', IP_STR_LENGTH);
    memset(gateway, '\0', IP_STR_LENGTH);
    memset(dnsServer1, '\0', IP_STR_LENGTH);
    memset(dnsServer2, '\0', IP_STR_LENGTH);
}

void IpNetwork::getMac(char *mac)
{
    strcpy(mac, this->mac);
}

void IpNetwork::setMac(char *mac)
{
    strncpy(this->mac, mac, MAC_STR_LENGTH);
}

void IpNetwork::getIpAddress(char *ipAddress)
{
    strcpy(ipAddress, this->ipAddress);
}

void IpNetwork::setIpAddress(char *ipAddress)
{
    strncpy(this->ipAddress, ipAddress, IP_STR_LENGTH);
}

void IpNetwork::getGateway(char *gateway)
{
    strcpy(gateway, this->gateway);
}

void IpNetwork::setGateway(char *gateway)
{
    strncpy(this->gateway, gateway, IP_STR_LENGTH);
}

void IpNetwork::getDnsServer1(char *dnsServer)
{
    strcpy(dnsServer, this->dnsServer1);
}

void IpNetwork::setDnsServer1(char *dnsServer)
{
    strncpy(this->dnsServer1, dnsServer, IP_STR_LENGTH);
}

void IpNetwork::getDnsServer2(char *dnsServer)
{
    strcpy(dnsServer, this->dnsServer2);
}

void IpNetwork::setDnsServer2(char *dnsServer)
{
    strncpy(this->dnsServer2, dnsServer, IP_STR_LENGTH);
}

IpNetwork::~IpNetwork()
{
    // TODO Auto-gntenerated destructor stub
}

