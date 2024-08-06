#include <ArduinoJson.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiAP.h>

IPAddress AP_LOCAL_IP(192, 168, 0, 1);
IPAddress AP_GATEWAY_IP(192, 168, 0, 0);
IPAddress AP_NETWORK_MASK(255, 255, 255, 0);

const char *TEXT_CONTENT_TYPE = "text/plain";
const char *JSON_CONTENT_TYPE = "application/json";
const int STATUS_OK = 200;

class MyAPServer {
   private:
    WebServer _server;

   public:
    void config(const String &ssid, const String &password) {
        WiFi.softAPConfig(AP_LOCAL_IP, AP_GATEWAY_IP, AP_NETWORK_MASK);

        if (!WiFi.softAP(ssid, password, 1, 0, 1)) {
            log_e("Soft AP creation failed.");
            while (1) {
            }
        }
    }

    void start() {
        _server.begin();
        for (;;) {
            _server.handleClient();
        }
    }

    void get(const Uri &uri, JsonDocument (*callback)()) {
        _server.on(uri, HTTP_GET, [this, callback]() {
            JsonDocument response = callback();
            String buffer;
            serializeJson(response, buffer);
            _server.send(STATUS_OK, JSON_CONTENT_TYPE, buffer);
        });
    }

    void post(const Uri &uri, void (*callback)(JsonDocument), void (*afterSendAction)()) {
        _server.on(uri, HTTP_POST, [this, callback, afterSendAction]() {
            String content =
                _server.hasArg("plain") ? _server.arg("plain") : "";
            JsonDocument json;
            deserializeJson(json, content);
            callback(json);
            _server.send(STATUS_OK, TEXT_CONTENT_TYPE, "");
            afterSendAction();
        });
    }

    void post(const Uri &uri, void (*callback)(JsonDocument)) {
        post(uri, callback, []() {});
    }
};