/*
 * Logger.h
 *
 *  Created on: Feb 23, 2014
 *      Author: richard
 */

#ifndef LOGGER_H_
#define LOGGER_H_

enum DebugLevel
{
    NONE = 1,
    ERROR = 2,
    WARNING = 3,
    DEBUG = 4,
    INFO = 5
};

void setLogLevel(DebugLevel level);
void log(DebugLevel level, const String & message);

#endif /* LOGGER_H_ */
