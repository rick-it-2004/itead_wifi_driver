/*
 * ParseUtilities.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: richard
 */

#include <Arduino.h>
#include <ParseUtilities.h>
#include <Logger.h>


bool decodeErrorValue(const char * errorCode)
{
    if (strcmp("FF", errorCode)) { Serial.println("error - Input parameter error"); return true; }
    if (strcmp("FE", errorCode)) { Serial.println("error - Bad command error"); return true; }
    if (strcmp("F6", errorCode)) { Serial.println("error - System error"); return true; }
    if (strcmp("F5", errorCode)) { Serial.println("error - Fatal error"); return true; }
    return false;
}

static const char hexConvertTable[16][16] =
{
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },//0F
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },//1F
    { ' ', '!', '"', '#', '$', '%', '&', ' ', '(', ')', '*', '+', ',', '-', '.', ' ' },//2F
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?' },//3F
    { '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O' },//4F
    { 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ', ' ', ' ', ' ', '_' },//5F
    { ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o' },//6F
    { 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', ' ', ' ' },//7F
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },//8F
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },//9F
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },//AF
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },//BF
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },//CF
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },//DF
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },//EF
    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' } //FF
};

char convertHexToAscii(uint8_t hex1, uint8_t hex2)
{
    uint8_t i = convertHexToUint8(hex1);
    uint8_t j = convertHexToUint8(hex2);
    return hexConvertTable[i][j];
}

char convertHexToAsciiNumber(uint8_t hex1)
{
    uint8_t j = convertHexToUint8(hex1);

    if (j > 9)
    {
        log(ERROR, "convertHexToAsciiNumber: bigger than 9");
        j = 9;
    }
    return hexConvertTable[3][j];
}

uint8_t convertHexToUint8(uint8_t hex)
{
    uint8_t i = hex - 48;
    if (i > 9) i = i - 7;
    return i;
}

uint8_t convertHexToUint8(uint8_t hex1, uint8_t hex2)
{
    int i = hex1 - 48;
    if (i > 9) i = i - 7;
    int j = hex2 - 48;
    if (j > 9) j = j - 7;
    return (i * 16) + j;
}

void convertUint8ToAscii(uint8_t i, char *result)
{
    char hun = i / 100;
    char ten = (i % 100) / 10;
    char one = i % 10;

    if (hun != 0)
    {
        result[0] = hun + 48;
        result[1] = ten + 48;
        result[2] = one + 48;
        result[3] = '\0';
        return;
    }

    if (ten > 0)
    {
        result[0] = ten + 48;
        result[1] = one + 48;
        result[2] = '\0';
        return;
    }

    result[0] = one + 48;
    result[1] = '\0';
    return;
}
