#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

// Configuration structure
typedef struct {
    bool networkEnabled;
    bool ntpEnabled;
    int stockUpdateInterval; // seconds
    char customApiKey[128];
} AppConfig;

// Config functions
bool loadConfig();
bool saveConfig();
AppConfig* getConfig();

// Initialize with defaults
void initConfig();

#endif // CONFIG_H
