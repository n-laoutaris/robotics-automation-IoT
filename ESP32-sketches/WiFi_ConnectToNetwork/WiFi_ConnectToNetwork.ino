#include <WiFi.h>

const char* ssid = "Mia_Skatoula";
const char* password = "6972129095Chatzi!";

void setup() {

  Serial.begin(115200);

  scanForNetworks();
  connectToNetwork();

  Serial.print("ESP32 MAC address:  ");  Serial.println(WiFi.macAddress());
  Serial.print("Local IP:  ");           Serial.println(WiFi.localIP());

  WiFi.disconnect(true);
  Serial.println(WiFi.localIP());
  Serial.println("Successfully disconnected.");

}

void loop() {
}

void scanForNetworks() {

  int numberOfNetworks = WiFi.scanNetworks();

  Serial.print("Αριθμος δικτύων που βρέθηκαν: ");
  Serial.println(numberOfNetworks);

  for (int i = 0; i < numberOfNetworks; i++) {

    Serial.print("Network name: ");
    Serial.println(WiFi.SSID(i));

    Serial.print("Signal strength: ");
    Serial.println(WiFi.RSSI(i));

    Serial.print("MAC address: ");
    Serial.println(WiFi.BSSIDstr(i));

    Serial.print("Encryption type: ");
    String encryptionTypeDescription = translateEncryptionType(WiFi.encryptionType(i));
    Serial.println(encryptionTypeDescription);
    Serial.println("-------------------------------");

  }
}

void connectToNetwork() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Establishing connection to WiFi...");
  }

  Serial.print("Connected to network ");
  Serial.print(ssid); Serial.println("!");

}

String translateEncryptionType(wifi_auth_mode_t encryptionType) {

  switch (encryptionType) {
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
  }
}
