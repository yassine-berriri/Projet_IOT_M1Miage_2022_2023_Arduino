/*********
    Based on Rui Santos work : https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
    File mqtt_full/mqtt_full.ino
    Modified by GM
*********/
#include <WiFi.h>
#include <PubSubClient.h>
#include "classic_setup.h"
#include "DHT.h"
#include <ArduinoJson.h>


double haversine(double lat1, double lon1, double lat2, double lon2) 
{
    // Distance between latitudes and longitudes
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    // Convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // Apply formula
    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}


StaticJsonDocument<1000> init_loc(float lat, float lon) {
  StaticJsonDocument<1000> loc_dict;
  loc_dict["lat"] = lat;
  loc_dict["lon"] = lon;
  return loc_dict;
}
// Déclaration de la fonction init_json
StaticJsonDocument<1000> init_json(const char* name, const char* descr,float temp , float lat, float lon,String stat,String presence,String last_ip) {
  // Crée un document JSON vide
  StaticJsonDocument<1000> piscine_dict;

  // Ajoute les champs au document JSON
  piscine_dict["status"]["temperature"] = temp;
  piscine_dict["status"]["light"] = 345;
  piscine_dict["status"]["heat"] = 1;
  piscine_dict["status"]["cold"] = 0;
  piscine_dict["status"]["running"] = 1;
  piscine_dict["status"]["fire"] = 0;

  piscine_dict["info"]["ident"] = "P_22211388";
  piscine_dict["info"]["user"] = "YB";
  piscine_dict["info"]["description"] = descr;
  piscine_dict["info"]["uptime"] = 123456;
  piscine_dict["info"]["ssid"] = "123455666";
  piscine_dict["info"]["ip"] = last_ip;

  // Utilise la méthode init_loc pour ajouter les coordonnées de localisation
  StaticJsonDocument<512> loc_doc = init_loc(lat, lon);
  piscine_dict["info"]["loc"] = loc_doc.as<JsonObject>();

  piscine_dict["regul"]["sh"] = 24.5;
  piscine_dict["regul"]["sb"] = 23.0;

  piscine_dict["reporthost"]["target_ip"] = "192.168.1.101";
  piscine_dict["reporthost"]["target_port"] = 80;
  piscine_dict["reporthost"]["sp"] = 60;

  piscine_dict["piscine"]["led"] = stat;
  piscine_dict["piscine"]["presence"] = presence;

  // Retourne le document JSON
  return piscine_dict;
}


StaticJsonDocument<1000> create_jsonDemandes(String id_user, String id_piscine, String Nom_user,  String Date_d_entree,  String Date_d_sortie, float distance,  float lan_piscine, float lat_piscine, float temp_piscine
) {
  StaticJsonDocument<1000> doc;

  doc["id_user"] = id_user;
  doc["id_piscine"] = id_piscine;
  doc["Nom_user"] = Nom_user;
  doc["Date_d_entree"] = Date_d_entree;
  doc["Date_d_sortie"] = Date_d_sortie;
  doc["distance"] = distance;
  doc["lan_piscine"] = lan_piscine;
  doc["lat_piscine"] = lat_piscine;
  doc["temp_piscine"] = temp_piscine;

  return doc;
}

StaticJsonDocument<1000> create_jsonPostion(String id, String tid, float lat, float lon, String couleur) {
  StaticJsonDocument<1000> doc;

  doc["id"] = id;
  doc["tid"] = tid;
  doc["lat"] = lat;
  doc["lon"] = lon;
  doc["couleur"] = couleur;

  return doc;
}
