/**
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * MEDUSA NATIVE HTTP CLIENT
 * =========================
 * 
 * Native C++ HTTP/HTTPS client implementation
 * Ground-up implementation - NO STUBS - Yorkshire Champion Standards
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

// OpenSSL includes - NO forward declarations
#include <openssl/ssl.h>
#include <openssl/ossl_typ.h>

namespace MedusaHttp {

struct HttpRequest {
    std::string method;
    std::string url;
    std::map<std::string, std::string> headers;
    std::string body;
    bool verify_ssl = true;
    std::string cipher_list = "TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384";
};

struct HttpResponse {
    int status_code = 0;
    std::map<std::string, std::string> headers;
    std::string body;
    std::string error_message;
    bool success = false;
};

class NativeHttpClient {
private:
    SSL_CTX* ssl_ctx_;
    bool initialized_;
    std::string ssl_cipher_list_;
    
    bool parse_url(const std::string& url, std::string& protocol, std::string& host, 
                   int& port, std::string& path);
    int create_socket(const std::string& host, int port);
    bool ssl_connect(int socket_fd, SSL*& ssl, const std::string& host);
    std::string build_request(const HttpRequest& req, const std::string& host, 
                             const std::string& path);
    HttpResponse parse_response(const std::string& response_data);
    bool send_http_request(int socket_fd, SSL* ssl, const std::string& request, 
                          std::string& response);
    
public:
    NativeHttpClient();
    ~NativeHttpClient();
    
    bool initialize();
    void cleanup();
    
    void set_ssl_cipher_list(const std::string& cipher_list);
    std::string get_ssl_cipher_list() const { return ssl_cipher_list_; }
    
    HttpResponse perform_request(const HttpRequest& request);
    HttpResponse get(const std::string& url, 
                    const std::map<std::string, std::string>& headers = {});
    HttpResponse post(const std::string& url, const std::string& body,
                     const std::map<std::string, std::string>& headers = {});
};

// CURL-compatible interface
struct CURL_State {
    std::string url;
    std::string method;
    std::map<std::string, std::string> headers;
    std::string post_data;
    void* write_data;
    size_t (*write_callback)(void*, size_t, size_t, void*);
    void* header_data;
    size_t (*header_callback)(void*, size_t, size_t, void*);
    bool verify_ssl;
    std::string cipher_list;
    long response_code;
    
    std::unique_ptr<NativeHttpClient> client;
    
    CURL_State() : write_data(nullptr), write_callback(nullptr),
                   header_data(nullptr), header_callback(nullptr),
                   verify_ssl(true), response_code(0) {
        client = std::make_unique<NativeHttpClient>();
        client->initialize();
    }
};

typedef CURL_State CURL;
typedef int CURLcode;
typedef int CURLINFO;

constexpr int CURLE_OK = 0;
constexpr int CURLE_FAILED_INIT = 1;
constexpr int CURLE_COULDNT_CONNECT = 7;
constexpr int CURLOPT_URL = 10002;
constexpr int CURLOPT_WRITEFUNCTION = 20011;
constexpr int CURLOPT_WRITEDATA = 10001;
constexpr int CURLOPT_POSTFIELDS = 10015;
constexpr int CURLOPT_HTTPHEADER = 10023;
constexpr int CURLOPT_CUSTOMREQUEST = 10036;
constexpr int CURLOPT_HEADERFUNCTION = 20079;
constexpr int CURLOPT_HEADERDATA = 10029;
constexpr int CURLOPT_FOLLOWLOCATION = 52;
constexpr int CURLOPT_SSL_VERIFYPEER = 64;
constexpr int CURLOPT_SSL_VERIFYHOST = 81;
constexpr int CURLOPT_POST = 47;
constexpr int CURLOPT_SSL_CIPHER_LIST = 10083;

constexpr long CURL_GLOBAL_DEFAULT = 0;
constexpr int CURLINFO_RESPONSE_CODE = 2097154;

struct curl_slist {
    char* data;
    struct curl_slist* next;
};

// CURL API functions
CURL* curl_easy_init();
void curl_easy_cleanup(CURL* curl);
CURLcode curl_easy_setopt(CURL* curl, int option, ...);
CURLcode curl_easy_perform(CURL* curl);
CURLcode curl_easy_getinfo(CURL* curl, CURLINFO info, void* param);
const char* curl_easy_strerror(CURLcode code);
struct curl_slist* curl_slist_append(struct curl_slist* list, const char* str);
void curl_slist_free_all(struct curl_slist* list);
void curl_global_init(long flags);
void curl_global_cleanup();

} // namespace MedusaHttp

