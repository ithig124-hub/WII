# Network Implementation Guide

## Overview

This guide explains how to implement real network functionality for the Wii Dashboard.
The current implementation uses mock data - follow these steps to add real API calls.

## Prerequisites

- libcurl installed (`dkp-pacman -S ppc-curl`)
- mbedTLS for HTTPS (`dkp-pacman -S ppc-mbedtls`)
- WiFi configured on Wii

## 1. Stock Data API

### Option A: Alpha Vantage (Free)

**Get API Key:**
1. Visit https://www.alphavantage.co/support/#api-key
2. Get free API key

**Implementation in `network.cpp`:**

```cpp
#include <curl/curl.h>

#define ALPHA_VANTAGE_KEY "YOUR_API_KEY_HERE"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool fetchStockData(const char* symbol, char* outPrice, char* outChange) {
    if (!isNetworkConnected()) return false;
    
    CURL *curl = curl_easy_init();
    if (!curl) return false;
    
    char url[512];
    snprintf(url, sizeof(url), 
        "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=%s&apikey=%s",
        symbol, ALPHA_VANTAGE_KEY);
    
    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) return false;
    
    // Parse JSON response
    const char* price = jsonGetString(response.c_str(), "05. price");
    const char* change = jsonGetString(response.c_str(), "10. change percent");
    
    if (price) strcpy(outPrice, price);
    if (change) strcpy(outChange, change);
    
    return true;
}
```

### Option B: Yahoo Finance (Free, No Key)

```cpp
bool fetchStockData(const char* symbol, char* outPrice, char* outChange) {
    char url[512];
    snprintf(url, sizeof(url),
        "https://query1.finance.yahoo.com/v8/finance/chart/%s", symbol);
    
    // Use curl to fetch and parse JSON
    // Similar to Alpha Vantage example above
}
```

## 2. World Time API

**Implementation:**

```cpp
bool fetchWorldTime(const char* timezone, char* outTime) {
    if (!isNetworkConnected()) return false;
    
    CURL *curl = curl_easy_init();
    if (!curl) return false;
    
    char url[256];
    snprintf(url, sizeof(url), "http://worldtimeapi.org/api/timezone/%s", timezone);
    
    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) return false;
    
    // Parse datetime from JSON
    const char* datetime = jsonGetString(response.c_str(), "datetime");
    if (datetime) {
        strcpy(outTime, datetime);
        return true;
    }
    
    return false;
}
```

**Timezone Strings:**
```
America/New_York
Europe/London
Asia/Tokyo
Australia/Sydney
Asia/Dubai
America/Los_Angeles
```

## 3. NTP Time Sync

**Implementation:**

```cpp
#include <sys/time.h>

bool syncNTPTime() {
    if (!isNetworkConnected()) return false;
    
    // Simple NTP implementation
    int sock = net_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) return false;
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(123); // NTP port
    
    // Resolve pool.ntp.org
    struct hostent *server = net_gethostbyname("pool.ntp.org");
    if (!server) {
        net_close(sock);
        return false;
    }
    
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    
    // Send NTP request
    unsigned char msg[48] = {0x1b}; // NTP v3, client
    net_sendto(sock, msg, sizeof(msg), 0, 
               (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    // Receive response
    socklen_t len = sizeof(serv_addr);
    int recv_len = net_recvfrom(sock, msg, sizeof(msg), 0,
                                 (struct sockaddr*)&serv_addr, &len);
    net_close(sock);
    
    if (recv_len < 48) return false;
    
    // Extract timestamp (simplified)
    unsigned long timestamp = 
        ((unsigned long)msg[40] << 24) |
        ((unsigned long)msg[41] << 16) |
        ((unsigned long)msg[42] << 8) |
        (unsigned long)msg[43];
    
    // Convert to Unix time (NTP epoch is 1900, Unix is 1970)
    timestamp -= 2208988800UL;
    
    // Set system time
    struct timeval tv;
    tv.tv_sec = timestamp;
    tv.tv_usec = 0;
    settimeofday(&tv, NULL);
    
    return true;
}
```

## 4. JSON Parsing

For more robust JSON parsing, use a library:

**Option A: cJSON (Recommended)**

```bash
# Install
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON
make CC=powerpc-eabi-gcc AR=powerpc-eabi-ar
cp cJSON.h $DEVKITPRO/portlibs/ppc/include/
cp libcjson.a $DEVKITPRO/portlibs/ppc/lib/
```

**Usage:**

```cpp
#include <cJSON.h>

float parseStockPrice(const char* json) {
    cJSON *root = cJSON_Parse(json);
    if (!root) return 0.0f;
    
    cJSON *globalQuote = cJSON_GetObjectItem(root, "Global Quote");
    cJSON *price = cJSON_GetObjectItem(globalQuote, "05. price");
    
    float result = price ? price->valuedouble : 0.0f;
    
    cJSON_Delete(root);
    return result;
}
```

## 5. Error Handling

**Add retry logic:**

```cpp
bool fetchWithRetry(const char* url, std::string& response, int retries = 3) {
    for (int i = 0; i < retries; i++) {
        CURL *curl = curl_easy_init();
        if (!curl) continue;
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if (res == CURLE_OK) return true;
        
        // Wait before retry
        usleep(1000000); // 1 second
    }
    return false;
}
```

## 6. Async Network Requests

**Use threads to prevent blocking:**

```cpp
#include <pthread.h>

static pthread_t networkThread;
static bool isNetworkBusy = false;

void* fetchStocksThread(void* arg) {
    isNetworkBusy = true;
    
    for (int i = 0; i < MAX_STOCKS; i++) {
        fetchStockData(stocks[i].symbol, stocks[i].price, stocks[i].change);
    }
    
    isNetworkBusy = false;
    return NULL;
}

void startStockUpdate() {
    if (isNetworkBusy) return;
    pthread_create(&networkThread, NULL, fetchStocksThread, NULL);
}
```

## 7. Rate Limiting

**Prevent API spam:**

```cpp
static time_t lastFetchTime = 0;
#define MIN_FETCH_INTERVAL 60 // seconds

bool canFetch() {
    time_t now = time(NULL);
    if (now - lastFetchTime < MIN_FETCH_INTERVAL) {
        return false;
    }
    lastFetchTime = now;
    return true;
}
```

## 8. Caching

**Save fetched data:**

```cpp
void cacheStockData() {
    FILE* file = fopen("sd:/apps/wii-dashboard/stocks.cache", "wb");
    if (file) {
        fwrite(stocks, sizeof(StockInfo), MAX_STOCKS, file);
        fclose(file);
    }
}

void loadCachedStockData() {
    FILE* file = fopen("sd:/apps/wii-dashboard/stocks.cache", "rb");
    if (file) {
        fread(stocks, sizeof(StockInfo), MAX_STOCKS, file);
        fclose(file);
    }
}
```

## 9. Testing

**Test network on Wii:**

```cpp
void testNetwork() {
    printf("Testing network connection...\n");
    
    if (!isNetworkConnected()) {
        printf("ERROR: Network not connected\n");
        return;
    }
    
    printf("Network connected!\n");
    
    // Test HTTP request
    char* response = httpGet("http://example.com");
    if (response) {
        printf("HTTP test successful\n");
        free(response);
    } else {
        printf("HTTP test failed\n");
    }
}
```

## 10. Configuration

**Store API keys in config file:**

```cpp
#define CONFIG_PATH "sd:/apps/wii-dashboard/config.ini"

bool loadConfig() {
    FILE* file = fopen(CONFIG_PATH, "r");
    if (!file) return false;
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "ALPHA_VANTAGE_KEY=", 18) == 0) {
            strcpy(apiKey, line + 18);
            // Remove newline
            apiKey[strcspn(apiKey, "\n")] = 0;
        }
    }
    
    fclose(file);
    return true;
}
```

**Example config.ini:**
```ini
ALPHA_VANTAGE_KEY=YOUR_KEY_HERE
UPDATE_INTERVAL=300
ENABLE_NTP=true
```

## Summary

1. Install libcurl and mbedTLS
2. Choose APIs (Alpha Vantage, Yahoo Finance, WorldTimeAPI)
3. Implement HTTP requests with curl
4. Add JSON parsing (cJSON recommended)
5. Implement error handling and retries
6. Use threads for async requests
7. Add caching for offline mode
8. Test thoroughly on real Wii hardware

## API Limits

**Alpha Vantage:**
- Free: 5 requests/minute, 500/day
- Premium: Higher limits

**Yahoo Finance:**
- No official API, may change
- Rate limiting recommended

**WorldTimeAPI:**
- No authentication required
- Fair use policy

---

**For production use, implement these features gradually and test each one thoroughly!**
