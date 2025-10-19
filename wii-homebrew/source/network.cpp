#include "network.h"
#include <network.h>
#include <ogc/lwp_watchdog.h>
#include <string.h>
#include <ogcsys.h>
#include <gccore.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

static bool networkInitialized = false;
static bool networkConnected = false;
static char responseBuffer[8192]; // Buffer for HTTP responses

bool initNetwork() {
    printf("Initializing network...\n");
    
    s32 result = net_init();
    if (result < 0) {
        printf("Network initialization failed: %d\n", result);
        return false;
    }
    
    networkInitialized = true;
    
    // Wait for network to connect (timeout after 10 seconds)
    int timeout = 100; // 10 seconds
    while (timeout > 0) {
        s32 status = net_get_status();
        if (status == 0) {
            networkConnected = true;
            printf("Network connected successfully!\n");
            return true;
        }
        usleep(100000); // 100ms
        timeout--;
    }
    
    printf("Network connection timeout\n");
    return false;
}

void cleanupNetwork() {
    if (networkInitialized) {
        net_deinit();
        networkInitialized = false;
        networkConnected = false;
    }
}

bool isNetworkConnected() {
    return networkConnected && (net_get_status() == 0);
}

// Simple HTTP GET using raw sockets (no external dependencies)
char* httpGet(const char* url) {
    if (!isNetworkConnected()) {
        printf("Network not connected\n");
        return NULL;
    }
    
    // Parse URL to get host and path
    char host[256] = {0};
    char path[512] = {0};
    int port = 80;
    
    // Simple URL parsing (http://host/path)
    const char* hostStart = strstr(url, "://");
    if (hostStart) {
        hostStart += 3;
    } else {
        hostStart = url;
    }
    
    const char* pathStart = strchr(hostStart, '/');
    if (pathStart) {
        strncpy(host, hostStart, pathStart - hostStart);
        strcpy(path, pathStart);
    } else {
        strcpy(host, hostStart);
        strcpy(path, "/");
    }
    
    printf("Connecting to %s%s\n", host, path);
    
    // Resolve hostname
    struct hostent *server = net_gethostbyname(host);
    if (!server) {
        printf("Failed to resolve host: %s\n", host);
        return NULL;
    }
    
    // Create socket
    int sock = net_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return NULL;
    }
    
    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    net_setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    net_setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    
    // Connect to server
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    
    if (net_connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Failed to connect to %s\n", host);
        net_close(sock);
        return NULL;
    }
    
    // Build HTTP request
    char request[1024];
    snprintf(request, sizeof(request),
        "GET %s HTTP/1.0\r\n"
        "Host: %s\r\n"
        "User-Agent: WiiDashboard/1.0\r\n"
        "Connection: close\r\n"
        "\r\n",
        path, host);
    
    // Send request
    int sent = net_send(sock, request, strlen(request), 0);
    if (sent < 0) {
        printf("Failed to send request\n");
        net_close(sock);
        return NULL;
    }
    
    // Receive response
    memset(responseBuffer, 0, sizeof(responseBuffer));
    int total = 0;
    int received;
    
    while (total < sizeof(responseBuffer) - 1) {
        received = net_recv(sock, responseBuffer + total, sizeof(responseBuffer) - total - 1, 0);
        if (received <= 0) break;
        total += received;
    }
    
    net_close(sock);
    
    if (total == 0) {
        printf("No data received\n");
        return NULL;
    }
    
    // Find start of body (after \r\n\r\n)
    char* body = strstr(responseBuffer, "\r\n\r\n");
    if (body) {
        body += 4;
        return body;
    }
    
    return responseBuffer;
}

bool httpPost(const char* url, const char* data) {
    if (!isNetworkConnected()) {
        return false;
    }
    
    printf("HTTP POST not implemented yet\n");
    return false;
}

// Stock data fetching using Yahoo Finance API
bool fetchStockData(const char* symbol, char* outPrice, char* outChange) {
    if (!isNetworkConnected()) {
        // Return mock data if offline
        sprintf(outPrice, "$%.2f", 150.25f + (rand() % 100) / 10.0f);
        sprintf(outChange, "%+.2f%%", (rand() % 200 - 100) / 10.0f);
        return true;
    }
    
    // Use Yahoo Finance query API
    char url[512];
    snprintf(url, sizeof(url),
        "http://query1.finance.yahoo.com/v8/finance/chart/%s?interval=1d&range=1d", 
        symbol);
    
    char* response = httpGet(url);
    if (!response) {
        printf("Failed to fetch stock data for %s\n", symbol);
        // Return mock data on failure
        sprintf(outPrice, "$%.2f", 150.25f + (rand() % 100) / 10.0f);
        sprintf(outChange, "%+.2f%%", (rand() % 200 - 100) / 10.0f);
        return false;
    }
    
    // Parse JSON response for current price
    // Look for "regularMarketPrice"
    const char* priceKey = "\"regularMarketPrice\":";
    const char* priceStart = strstr(response, priceKey);
    if (priceStart) {
        priceStart += strlen(priceKey);
        float price = atof(priceStart);
        
        // Look for previous close
        const char* prevKey = "\"previousClose\":";
        const char* prevStart = strstr(response, prevKey);
        if (prevStart) {
            prevStart += strlen(prevKey);
            float prevClose = atof(prevStart);
            
            float change = ((price - prevClose) / prevClose) * 100.0f;
            
            sprintf(outPrice, "$%.2f", price);
            sprintf(outChange, "%+.2f%%", change);
            
            printf("Stock %s: %s (%s)\n", symbol, outPrice, outChange);
            return true;
        }
    }
    
    // If parsing fails, return mock data
    sprintf(outPrice, "$%.2f", 150.25f + (rand() % 100) / 10.0f);
    sprintf(outChange, "%+.2f%%", (rand() % 200 - 100) / 10.0f);
    return false;
}

// World time fetching using WorldTimeAPI
bool fetchWorldTime(const char* timezone, char* outTime) {
    if (!isNetworkConnected()) {
        // Return local time if offline
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(outTime, 32, "%H:%M:%S", timeinfo);
        return true;
    }
    
    // Build URL - timezone should be like "America/New_York"
    char url[256];
    snprintf(url, sizeof(url), "http://worldtimeapi.org/api/timezone/%s", timezone);
    
    char* response = httpGet(url);
    if (!response) {
        printf("Failed to fetch time for %s\n", timezone);
        // Return local time on failure
        time_t rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(outTime, 32, "%H:%M:%S", timeinfo);
        return false;
    }
    
    // Parse JSON for datetime field
    // Format: "datetime":"2025-01-08T14:30:45.123456-05:00"
    const char* dtKey = "\"datetime\":\"";
    const char* dtStart = strstr(response, dtKey);
    if (dtStart) {
        dtStart += strlen(dtKey);
        
        // Extract time portion (HH:MM:SS)
        char datetime[64];
        int i = 0;
        while (dtStart[i] && dtStart[i] != '\"' && i < 63) {
            datetime[i] = dtStart[i];
            i++;
        }
        datetime[i] = '\0';
        
        // Find time part (after 'T')
        char* timePart = strchr(datetime, 'T');
        if (timePart) {
            timePart++; // Skip 'T'
            // Copy HH:MM:SS
            strncpy(outTime, timePart, 8);
            outTime[8] = ' ';
            
            printf("Time for %s: %s\n", timezone, outTime);
            return true;
        }
    }
    
    // If parsing fails, return local time
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(outTime, 32, "%H:%M:%S", timeinfo);
    return false;
}

// NTP time sync implementation
bool syncNTPTime() {
    if (!isNetworkConnected()) {
        printf("Network not connected, cannot sync time\n");
        return false;
    }
    
    printf("Syncing time via NTP...\n");
    
    // NTP server
    const char* ntpServer = "pool.ntp.org";
    
    // Resolve NTP server
    struct hostent *server = net_gethostbyname(ntpServer);
    if (!server) {
        printf("Failed to resolve NTP server\n");
        return false;
    }
    
    // Create UDP socket
    int sock = net_socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return false;
    }
    
    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    net_setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    
    // Setup server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(123); // NTP port
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    
    // Build NTP request packet
    unsigned char msg[48] = {0};
    msg[0] = 0x1b; // NTP version 3, client mode
    
    // Send request
    int sent = net_sendto(sock, msg, sizeof(msg), 0, 
                         (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (sent < 0) {
        printf("Failed to send NTP request\n");
        net_close(sock);
        return false;
    }
    
    // Receive response
    socklen_t len = sizeof(serv_addr);
    int recv_len = net_recvfrom(sock, msg, sizeof(msg), 0,
                                (struct sockaddr*)&serv_addr, &len);
    net_close(sock);
    
    if (recv_len < 48) {
        printf("Invalid NTP response\n");
        return false;
    }
    
    // Extract timestamp from bytes 40-43 (transmit timestamp seconds)
    unsigned long timestamp = 
        ((unsigned long)msg[40] << 24) |
        ((unsigned long)msg[41] << 16) |
        ((unsigned long)msg[42] << 8) |
        (unsigned long)msg[43];
    
    // Convert from NTP epoch (1900) to Unix epoch (1970)
    timestamp -= 2208988800UL;
    
    // Set system time using Wii's settime function
    time_t newTime = (time_t)timestamp;
    settime(newTime);
    
    printf("Time synced successfully to: %s", ctime(&newTime));
    return true;
}

// Simple JSON helpers (very basic, no full parser)
const char* jsonGetString(const char* json, const char* key) {
    static char buffer[256];
    char searchKey[128];
    sprintf(searchKey, "\"%s\":\"", key);
    
    const char* start = strstr(json, searchKey);
    if (!start) return NULL;
    
    start += strlen(searchKey);
    const char* end = strchr(start, '"');
    if (!end) return NULL;
    
    int len = end - start;
    if (len >= 256) len = 255;
    
    strncpy(buffer, start, len);
    buffer[len] = '\0';
    
    return buffer;
}

float jsonGetFloat(const char* json, const char* key) {
    char searchKey[128];
    sprintf(searchKey, "\"%s\":", key);
    
    const char* start = strstr(json, searchKey);
    if (!start) return 0.0f;
    
    start += strlen(searchKey);
    return atof(start);
}
