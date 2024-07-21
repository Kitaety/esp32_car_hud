#include <Preferences.h>
#include <ArduinoJson.h>

const char *WIFI_CONFIG_NAMESPACE = "wifi";
const char *WIFI_SSID_KEY = "ssid";
const char *WIFI_PASSWORD_KEY = "password";

const char *DEFAULT_WIFI_SSID = "CAR HUD";
const char *DEFAULT_WIFI_PASSWORD = "12345678";

const char *HUD_CONFIG_NAMESPACE = "wifi";

class PreferencesManager {
private:
  Preferences prefs;

public:
  void reboot() {
    ESP.restart();
  }

  void reset() {
    updateWifiConfig(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);
  }

  void updateWifiConfig(String ssid, String password) {
    prefs.begin(WIFI_CONFIG_NAMESPACE);
    prefs.putString(WIFI_SSID_KEY, ssid);
    prefs.putString(WIFI_PASSWORD_KEY, password);
  }

  JsonDocument getWifiConfig() {
    prefs.begin(WIFI_CONFIG_NAMESPACE);
    String ssid = prefs.getString(WIFI_SSID_KEY);
    String password = prefs.getString(WIFI_PASSWORD_KEY);

    JsonDocument result;
    result["ssid"] = ssid.length() > 0 ? ssid : DEFAULT_WIFI_SSID;
    result["password"] = password.length() > 0 ? password : DEFAULT_WIFI_PASSWORD;

    return result;
  }
};