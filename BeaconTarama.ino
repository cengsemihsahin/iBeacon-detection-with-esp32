//#include <HTTPClient.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>

const char * ssid = "Wi-Fi_SSID_adi";
const char * parola = "Wi-Fi_sifre";
int taramaZamani = 7;
int cihazSayisi = 0;
int rssi;
int i;
String deviceName;
BLEScan * pBeaconTarama;
BLEAdvertisedDevice cihaz;

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");
  pBeaconTarama = BLEDevice::getScan(); // yeni tarama yarat
  pBeaconTarama -> setActiveScan(true); // suanki taramaya guc ver
  pBeaconTarama -> setInterval(100);
  pBeaconTarama -> setWindow(99);  // en az setInterval degeri
}

void loop() {
  
  Serial.printf("\n--------------------\n\nTARANIYOR...\n");
  BLEScanResults bulunanCihazlar = pBeaconTarama -> start(taramaZamani, false);
  cihazSayisi = bulunanCihazlar.getCount();

  int cihazRssi[cihazSayisi];
  String deviceAddress[cihazSayisi];
  String cihazName[cihazSayisi];
  
  for (i = 0; i < cihazSayisi; i++) {
    cihaz = bulunanCihazlar.getDevice(i);
    rssi = cihaz.getRSSI();
    deviceName = cihaz.getName().c_str();
    Serial.printf("Cihaz Adi: %s\nCihaz Adres: %s\nCihaz RSSI: %d\n\n", deviceName, cihaz.getAddress().toString().c_str(), rssi);
    cihazName[i] = deviceName;
    cihazRssi[i] = rssi;
    deviceAddress[i] = cihaz.getAddress().toString().c_str();
  }
  Serial.printf("Toplam bulunan: %d\nTARAMA BITTI!\n--------------------\n", bulunanCihazlar.getCount());
  pBeaconTarama -> clearResults();   // arabellekten verileri sil
  delayMicroseconds(10);

  WiFi.begin(ssid, parola);
  while (true) {
    delay(250);
    Serial.println("Wifi baglantisi bekleniyor...");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi BAGLANTISI AKTIF");
      break;
    }
  }

  delay(10000);

  WiFi.disconnect();
  Serial.println("WiFi BAGLANTISI KAPATILDI..!");
}
