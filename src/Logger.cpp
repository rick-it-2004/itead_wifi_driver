/*
 * Logger.cpp
 *
 *  Created on: Feb 23, 2014
 *      Author: richard
 */

#include <Arduino.h>
#include <Logger.h>

static DebugLevel debugLevel = NONE;

void setLogLevel(DebugLevel level)
{
    debugLevel = level;
}

void log(DebugLevel level, String message)
{
    if (debugLevel == level) Serial.println(message);
}
