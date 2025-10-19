#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"

// Network initialization
bool initNetwork();
void cleanupNetwork();
bool isNetworkConnected();

// HTTP requests
char* httpGet(const char* url);
bool httpPost(const char* url, const char* data);

// API functions
bool fetchStockData(const char* symbol, char* outPrice, char* outChange);
bool fetchWorldTime(const char* timezone, char* outTime);
bool syncNTPTime();

// JSON helpers
const char* jsonGetString(const char* json, const char* key);
float jsonGetFloat(const char* json, const char* key);

#endif // NETWORK_H
