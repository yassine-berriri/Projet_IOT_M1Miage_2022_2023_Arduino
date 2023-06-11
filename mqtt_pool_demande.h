  #include <ArduinoJson.h>
double haversine(double lat1, double lon1, double lat2, double lon2);

StaticJsonDocument<1000> init_loc(float lat, float lon);
StaticJsonDocument<1000> init_json(const char* name, const char* descr,float temp , float lat, float lon,String stat,String presence,String last_ip);
StaticJsonDocument<1000> create_jsonPostion(String id, String tid, float lat, float lon, String couleur);

StaticJsonDocument<1000> create_jsonDemandes(
  String id_user,
  String id_piscine,
  String Nom_user,
  String Date_d_entree,
  String Date_d_sortie,
  float distance,
  float lan_piscine,
  float lat_piscine,
  float temp_piscine
);
