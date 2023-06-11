#include <WiFi.h>


#include <WiFiMulti.h>

WiFiMulti wifiMulti; // Creates an instance of the WiFiMulti class

/*------------------------*/
String translateEncryptionType(byte encryptionType) {
   // cf https://www.arduino.cc/en/Reference/WiFiEncryptionType 
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
/*------------------------*/
void print_network_status(){ // Utilisation de String !
  String s = "";
  s += "\tIP address : " + WiFi.localIP().toString() + "\n"; // bizarre IPAddress
  s += "\tMAC address : " + String(WiFi.macAddress()) + "\n";
  s += "\tWifi SSID : " + String(WiFi.SSID()) + "\n";
  s += "\tWifi Signal Strength : " + String(WiFi.RSSI()) + "\n";
  s += "\tWifi BSSID : " + String(WiFi.BSSIDstr()) + "\n";
  s += "\tWifi Encryption type : " + translateEncryptionType(WiFi.encryptionType(0))+ "\n";
  // a mon avis bug ! => manque WiFi.encryptionType() !
  Serial.print(s);
}
/*------------------------*/
void connect_wifi(){
 //  Set WiFi to station mode 
 WiFi.mode(WIFI_STA);
 // and disconnect from an AP if
 // it was previously connected
 WiFi.disconnect();
 delay(100); // ms

 Serial.println(String("\nAttempting to connect to SSIDs : "));
 //wifiMulti.addAP("iPhone-Arman", "ilovesql");
// wifiMulti.addAP("AndroidAPFE44", "12091999");
 wifiMulti.addAP("S21ultra", "s21ultra");
 wifiMulti.addAP("HUAWEI-553A", "QTM06RTT");
 //wifiMulti.addAP("GMAP", "vijx47050");
 //wifiMulti.addAP("Livebox-B870","MYCNcZqnvsWsiy7s52");
 
 while(wifiMulti.run() != WL_CONNECTED) {
   delay(1000);
   Serial.print(".");
 }

 if(wifiMulti.run() == WL_CONNECTED) {
   Serial.print("\nWiFi connected : \n");
  }
}
