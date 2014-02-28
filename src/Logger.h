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
    NONE,
    ERROR,
    WARNING,
    DEBUG,
    INFO
};

void setLogLevel(DebugLevel level);
void log(DebugLevel level, String message);

#endif /* LOGGER_H_ */
