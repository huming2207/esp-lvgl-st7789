#pragma once

#include <string>
#include <functional>
#include <map>

#include <esp_http_server.h>

namespace sys
{
    class esp_web_serv
    {
    public:
        esp_web_serv();
        esp_err_t on(const std::string& url, httpd_method_t method,
                     const std::function<esp_err_t(httpd_req_t*)>& cb);
        esp_err_t on_error(httpd_err_code_t err_code, const std::function<esp_err_t(httpd_req_t*)>& cb);
        static esp_err_t send_body(httpd_req_t *req, const std::vector<uint8_t>& buf,
                                    const std::string& status = HTTPD_200, const std::string& type = HTTPD_TYPE_TEXT);
        static esp_err_t send_body(httpd_req_t *req, const std::string& buf,
                                    const std::string& status = HTTPD_200, const std::string& type = HTTPD_TYPE_TEXT);
        static esp_err_t send_chunk(httpd_req_t *req, const std::vector<uint8_t>& chunk);
        static esp_err_t send_chunk(httpd_req_t *req, const std::string& chunk);
        static esp_err_t set_header(httpd_req_t *req, const std::map<std::string, std::string>& headers);
        static esp_err_t set_header(httpd_req_t *req, const std::string& key, const std::string& val);
        static esp_err_t set_status(httpd_req_t *req, const std::string& status);
        static esp_err_t set_type(httpd_req_t *req, const std::string& type);
        httpd_handle_t get_serv_instance();

    private:
        std::map<std::tuple<std::string, httpd_method_t>, std::function<esp_err_t(httpd_req_t*)>> uri_handler;
        std::map<httpd_err_code_t, std::function<esp_err_t(httpd_req_t*)>> err_handler;
        httpd_handle_t server = nullptr;
    };
}