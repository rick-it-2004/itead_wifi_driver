/*
 * HttpClient.h
 *
 *  Created on: May 11, 2014
 *      Author: Richard
 */

#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include <Arduino.h>

class HttpClient
{
public:
    HttpClient();

    String httpGet(const String & url);

    virtual ~HttpClient();
};

#endif /* HTTPCLIENT_H_ */
