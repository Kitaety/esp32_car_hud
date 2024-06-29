#include <WiFi.h>
#include <WebServer.h>
#include <WiFiAP.h>

IPAddress AP_GATEWAY_IP(192, 168, 0, 0);
IPAddress AP_NETWORK_MASK(255, 255, 255, 0);

const char* TEXT_CONTENT_TYPE = "text/plain";
const int STATUS_OK = 200;

class MyAPServer {
private:
  WebServer _server;
public:
  void config(const String &ssid, const String &password, IPAddress local_ip) {
    WiFi.softAPConfig(local_ip, AP_GATEWAY_IP, AP_NETWORK_MASK);

    if (!WiFi.softAP(ssid, password, 1, 0, 1)) {
      log_e("Soft AP creation failed.");
      while (1) {}
    }
  }
  void start() {
    _server.begin();
    for (;;) {
      _server.handleClient();
    }
  }

  void get(const Uri &uri, const char* (*callback)()) {
    _server.on(uri, HTTP_GET, [this, callback]() {
      _server.send(STATUS_OK, TEXT_CONTENT_TYPE, callback());
    });
  };
  void get(const Uri &uri, void (*callback)()) {
    _server.on(uri, HTTP_GET, [this, callback]() {
      callback();
      _server.send(STATUS_OK, TEXT_CONTENT_TYPE, "");
    });
  };
};