#pragma once

// Forward to native HTTP client implementation
#include "medusa_native_http_client.hpp"

using MedusaHttp::CURL;
using MedusaHttp::CURLcode;
using MedusaHttp::CURLINFO;
using MedusaHttp::curl_easy_init;
using MedusaHttp::curl_easy_cleanup;
using MedusaHttp::curl_easy_setopt;
using MedusaHttp::curl_easy_perform;
using MedusaHttp::curl_easy_getinfo;
using MedusaHttp::curl_easy_strerror;
using MedusaHttp::curl_slist;
using MedusaHttp::curl_slist_append;
using MedusaHttp::curl_slist_free_all;
using MedusaHttp::curl_global_init;
using MedusaHttp::curl_global_cleanup;

// Re-export constants as macros for compatibility
#define CURLE_OK MedusaHttp::CURLE_OK
#define CURLE_FAILED_INIT MedusaHttp::CURLE_FAILED_INIT
#define CURLE_COULDNT_CONNECT MedusaHttp::CURLE_COULDNT_CONNECT
#define CURLOPT_URL MedusaHttp::CURLOPT_URL
#define CURLOPT_WRITEFUNCTION MedusaHttp::CURLOPT_WRITEFUNCTION
#define CURLOPT_WRITEDATA MedusaHttp::CURLOPT_WRITEDATA
#define CURLOPT_POSTFIELDS MedusaHttp::CURLOPT_POSTFIELDS
#define CURLOPT_HTTPHEADER MedusaHttp::CURLOPT_HTTPHEADER
#define CURLOPT_CUSTOMREQUEST MedusaHttp::CURLOPT_CUSTOMREQUEST
#define CURLOPT_HEADERFUNCTION MedusaHttp::CURLOPT_HEADERFUNCTION
#define CURLOPT_HEADERDATA MedusaHttp::CURLOPT_HEADERDATA
#define CURLOPT_FOLLOWLOCATION MedusaHttp::CURLOPT_FOLLOWLOCATION
#define CURLOPT_SSL_VERIFYPEER MedusaHttp::CURLOPT_SSL_VERIFYPEER
#define CURLOPT_SSL_VERIFYHOST MedusaHttp::CURLOPT_SSL_VERIFYHOST
#define CURLOPT_POST MedusaHttp::CURLOPT_POST
#define CURLOPT_SSL_CIPHER_LIST MedusaHttp::CURLOPT_SSL_CIPHER_LIST
#define CURL_GLOBAL_DEFAULT MedusaHttp::CURL_GLOBAL_DEFAULT
#define CURLINFO_RESPONSE_CODE MedusaHttp::CURLINFO_RESPONSE_CODE
