/*
 * ParseUtilities.h
 *
 *  Created on: Feb 21, 2014
 *      Author: richard
 */

#ifndef PARSEUTILITIES_H_
#define PARSEUTILITIES_H_

bool     decodeErrorValue(const String errorCode);
char     convertHexToAscii(uint8_t hex1, uint8_t hex2);
uint8_t  convertHexToUint8(uint8_t hex);
uint8_t  convertHexToUint8(uint8_t hex1, uint8_t hex2);
uint16_t convertHexToUint16(uint8_t hex1, uint8_t hex2);
void     convertUint8ToAscii(uint8_t i, char *result);
String   convertUint8ToAscii(uint8_t i);

#endif /* PARSEUTILITIES_H_ */
