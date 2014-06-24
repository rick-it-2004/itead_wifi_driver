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
    memset(mac, '\0', WL_MAC_ADDR_LENGTH);
    ipAddress  = NULL;
    gateway    = NULL;
    subnetMask = NULL;;
    dnsServer1 = NULL;
    dnsServer2 = NULL;
}

const char * IpNetwork::getMac(String & mac)
{
    mac = this->mac;
    return mac.c_str();
}

void IpNetwork::setMac(const char *mac)
{
    strncpy(this->mac, mac, WL_MAC_ADDR_LENGTH);
}

IPAddress IpNetwork::getIpAddress()
{
    return *(this->ipAddress);
}

void IpNetwork::setIpAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    this->ipAddress = new IPAddress(first_octet, second_octet, third_octet, fourth_octet);
}

IPAddress IpNetwork::getSubnetMask()
{
    return *(this->subnetMask);
}

void IpNetwork::setSubnetMask(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    this->subnetMask = new IPAddress(first_octet, second_octet, third_octet, fourth_octet);
}

IPAddress IpNetwork::getGateway()
{
    return *(this->gateway);
}

void IpNetwork::setGateway(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    this->gateway = new IPAddress(first_octet, second_octet, third_octet, fourth_octet);
}

IPAddress IpNetwork::getDnsServer1()
{
    return *(this->dnsServer1);
}

void IpNetwork::setDnsServer1(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    this->dnsServer1 = new IPAddress(first_octet, second_octet, third_octet, fourth_octet);
}

IPAddress IpNetwork::getDnsServer2()
{
    return *(this->dnsServer2);
}

void IpNetwork::setDnsServer2(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    this->dnsServer2 = new IPAddress(first_octet, second_octet, third_octet, fourth_octet);
}

IpNetwork::~IpNetwork()
{
    // TODO Auto-gntenerated destructor stub
}

