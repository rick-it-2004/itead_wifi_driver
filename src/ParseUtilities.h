/*
 * ParseUtilities.h
 *
 *  Created on: Feb 21, 2014
 *      Author: richard
 */

#ifndef PARSEUTILITIES_H_
#define PARSEUTILITIES_H_

bool    decodeErrorValue(const char * errorCode);
char    convertHexToAscii(uint8_t hex1, uint8_t hex2);
uint8_t convertHexToUint8(uint8_t hex);
uint8_t convertHexToUint8(uint8_t hex1, uint8_t hex2);
void    convertUint8ToAscii(uint8_t i, char *result);

#endif /* PARSEUTILITIES_H_ */
