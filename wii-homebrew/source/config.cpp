#include "config.h"
#include <stdio.h>
#include <string.h>

static AppConfig config;
static const char* CONFIG_PATH = "sd:/apps/wii-dashboard/config.dat";

void initConfig() {
    // Default settings
    config.networkEnabled = true;
    config.ntpEnabled = true;
    config.stockUpdateInterval = 300; // 5 minutes
    config.customApiKey[0] = '\0';
}

bool loadConfig() {
    FILE* file = fopen(CONFIG_PATH, "rb");
    if (!file) {
        printf("No config file found, using defaults\n");
        initConfig();
        return false;
    }
    
    size_t read = fread(&config, sizeof(AppConfig), 1, file);
    fclose(file);
    
    if (read == 1) {
        printf("Config loaded successfully\n");
        return true;
    }
    
    printf("Failed to read config, using defaults\n");
    initConfig();
    return false;
}

bool saveConfig() {
    FILE* file = fopen(CONFIG_PATH, "wb");
    if (!file) {
        printf("Failed to open config file for writing\n");
        return false;
    }
    
    size_t written = fwrite(&config, sizeof(AppConfig), 1, file);
    fclose(file);
    
    if (written == 1) {
        printf("Config saved successfully\n");
        return true;
    }
    
    printf("Failed to write config\n");
    return false;
}

AppConfig* getConfig() {
    return &config;
}
