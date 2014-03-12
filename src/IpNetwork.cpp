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
}

void IpNetwork::getMac(char *mac)
{
    strcpy(mac, this->mac);
}

void IpNetwork::setMac(char *mac)
{
    strncpy(this->mac, mac, MAC_STR_LENGTH);
}

IPAddress *IpNetwork::getIpAddress()
{
    return this->ipAddress;
}

void IpNetwork::setIpAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    this->ipAddress = new IPAddress(first_octet, second_octet, third_octet, fourth_octet);
}

IPAddress *IpNetwork::getGateway()
{
    return this->gateway;
}

void IpNetwork::setGateway(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    this->gateway = new IPAddress(first_octet, second_octet, third_octet, fourth_octet);
}

IPAddress *IpNetwork::getDnsServer1()
{
    return this->dnsServer1;
}

void IpNetwork::setDnsServer1(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    this->dnsServer1 = new IPAddress(first_octet, second_octet, third_octet, fourth_octet);
}

IPAddress *IpNetwork::getDnsServer2()
{
    return this->dnsServer2;
}

void IpNetwork::setDnsServer2(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    this->dnsServer2 = new IPAddress(first_octet, second_octet, third_octet, fourth_octet);
}

IpNetwork::~IpNetwork()
{
    // TODO Auto-gntenerated destructor stub
}

