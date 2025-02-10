#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_URL_LENGTH 2048

typedef struct {
    char *workerUrl;
} CloudflareFileStorage;

CloudflareFileStorage* create_cloudflare_storage(const char *workerUrl) {
    CloudflareFileStorage *storage = malloc(sizeof(CloudflareFileStorage));
    storage->workerUrl = strdup(workerUrl);
    if (storage->workerUrl[strlen(storage->workerUrl) - 1] == '/')
        storage->workerUrl[strlen(storage->workerUrl) - 1] = '\0';
    return storage;
}

char* upload_file(CloudflareFileStorage *storage, const char *filePath) {
    FILE *file = fopen(filePath, "rb");
    if (!file) {
        perror("File not found");
        return "Error uploading file";
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        fclose(file);
        return "CURL initialization failed";
    }

    curl_easy_setopt(curl, CURLOPT_URL, strcat(storage->workerUrl, "/upload"));
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_READDATA, file);
    
    CURLcode res = curl_easy_perform(curl);
    fclose(file);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return "Error uploading file";
    }

    return "File uploaded successfully";
}

char* download_file(CloudflareFileStorage *storage, const char *fileName, const char *savePath) {
    CURL *curl = curl_easy_init();
    if (!curl) return "CURL initialization failed";

    char url[MAX_URL_LENGTH];
    snprintf(url, MAX_URL_LENGTH, "%s/download/%s", storage->workerUrl, fileName);

    FILE *file = fopen(savePath, "wb");
    if (!file) return "Error opening file for writing";

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    CURLcode res = curl_easy_perform(curl);
    fclose(file);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return "Error downloading file";
    
    return "File downloaded successfully";
}

char* delete_file(CloudflareFileStorage *storage, const char *fileName) {
    CURL *curl = curl_easy_init();
    if (!curl) return "CURL initialization failed";

    char url[MAX_URL_LENGTH];
    snprintf(url, MAX_URL_LENGTH, "%s/delete/%s", storage->workerUrl, fileName);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return "Error deleting file";

    return "File deleted successfully";
}
