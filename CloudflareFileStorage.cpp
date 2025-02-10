#include <iostream>
#include <fstream>
#include <sstream>
#include <curl/curl.h>

class CloudflareFileStorage {
private:
    std::string workerUrl;

public:
    CloudflareFileStorage(const std::string &url) {
        workerUrl = url.back() == '/' ? url : url + "/";
    }

    std::string uploadFile(const std::string &filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            throw std::runtime_error("File not found: " + filePath);
        }

        CURL *curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("CURL initialization failed");
        }

        curl_mime *mime = curl_mime_init(curl);
        curl_mimepart *part = curl_mime_addpart(mime);
        curl_mime_filedata(part, filePath.c_str());
        curl_mime_type(part, "application/octet-stream");

        curl_easy_setopt(curl, CURLOPT_URL, (workerUrl + "upload").c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        CURLcode res = curl_easy_perform(curl);
        curl_mime_free(mime);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("CURL request failed");
        }

        return "File uploaded successfully";
    }

    std::string downloadFile(const std::string &fileName, const std::string &savePath) {
        CURL *curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("CURL initialization failed");
        }

        curl_easy_setopt(curl, CURLOPT_URL, (workerUrl + "download/" + fileName).c_str());

        std::ofstream outFile(savePath + "/" + fileName, std::ios::binary);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("CURL request failed");
        }

        return "File downloaded successfully";
    }

    std::string deleteFile(const std::string &fileName) {
        CURL *curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("CURL initialization failed");
        }

        curl_easy_setopt(curl, CURLOPT_URL, (workerUrl + "delete/" + fileName).c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("CURL request failed");
        }

        return "File deleted successfully";
    }
};
