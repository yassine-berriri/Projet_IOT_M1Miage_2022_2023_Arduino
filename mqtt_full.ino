
#include <WiFi.h>
#include <PubSubClient.h>
#include "classic_setup.h"
#include "mqtt_pool_demande.h"
#include "DHT.h"
#include <ArduinoJson.h>
#include "ESPAsyncWebServer.h"


/*===== MQTT broker/server ========*/
//const char* mqtt_server = "192.168.1.101"; 
//const char* mqtt_server = "public.cloud.shiftr.io"; // Failed in 2021
// need login and passwd (public,public) mqtt://public:public@public.cloud.shiftr.io
//const char* mqtt_server = "broker.hivemq.com"; // anynomous Ok in 2021 
const char* mqtt_server = "test.mosquitto.org"; // anynomous Ok in 2021*
//const char* mqtt_server = "mqtt.eclipseprojects.io"; // anynomous Ok in 2021
const char* mqtt_server2 = "mqtt.eclipseprojects.io"; // anynomous Ok in 2021

/*===== MQTT TOPICS ===============*/
#define TOPIC "uca/iot/piscine"

#define USE_SERIAL Serial

#define TOPIC_DEMANDE  "uca/waterbnb/#"
#define TOPIC_Postion  "uca/waterbnb/000000/test"


#define DHTPIN 4     // specify the pin connected to DHT11
#define DHTTYPE DHT11 // specify the type of DHT sensor

/*============= GPIO ==============*/
const int greenLed = 19; // LED Pin
const int redLed = 21; // LED Pin
const int blue_led= 2;

 DHT dht(DHTPIN, DHTTYPE);
const int LightPin = A5; // Read analog input on ADC1_CHANNEL_5 (GPIO 33)

/*===== ESP is MQTT Client =======*/
WiFiClient espClient;           // Wifi 
PubSubClient client(espClient); // MQTT client


/*===== ESP is MQTT Client =======*/
WiFiClient espClient2;           // Wifi 
PubSubClient client2(espClient2); // MQTT client
/*============= lES LOCALISATION ==============*/
struct PersonneData { //defintion des pcines recup dans la
  String ident;
  String tid;
  float lat;
  float lon;
};

PersonneData personne[31];
int personneCount = 0;
int cpt = 0;
/*============= Picine ==============*/
struct PoolData { //defintion des pcines recup dans la
  String ident;
  float lat;
  float lon;
  float temp;
};
PoolData pools[31]; // For up to 10 other pools
int poolCount = 0;

// Structure pour definir  les demandes 
struct Demandes {
  String id_user;
  String id_piscine;
  String Nom_user;
  String Date_d_sortie;
  String Date_d_entree;
  float distance;
  float lan_piscine;
  float lat_piscine;
  float temp_piscine;
};

Demandes demandes[31];
int demandesCount = 0; 

/*Declaration des Postions*/

const int MAX_POSITIONS = 20;  // Maximun number of positions you want to store
int positionsCount = 0;  // The current number of positions
StaticJsonDocument<1000> positions[MAX_POSITIONS]; 


/* ---- TEMP ---- */  

float temperature = 0;
float light = 0;
/*
 * 
 */

 // Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const String user = "Yassine Berriri";

float latitude = 43.566669   ;
float longitude = 7.1;
String last_ip= ""; 
String last_mac= "";
bool presence =false;
String last_light;
short int Light_threshold = 250;


void greenLedon(int v){
 digitalWrite(greenLed, v);
}
void redLedon(int v){
 digitalWrite(redLed, v);
}

float get_Temperature(){
   float temp = dht.readTemperature();   // Obtenir la température
    return temp;
 
}
float get_light(int pin){
    int light_value = analogRead(pin);
    return float(light_value);
}

PersonneData* findpersonne(String idu) {
  for (int i = 0; i < personneCount; i++) {
    if (personne[i].ident == idu) {
      Serial.println("j'ai trouver Personne");
      return &personne[i];
    }
  }
  return NULL; // Aucune correspondance trouvée
}

PoolData* findpool(String idu) {
  for (int i = 0; i < poolCount; i++) {
    if (pools[i].ident == idu) 
    {
      Serial.println("j'ai trouver pool");
      return &pools[i];
    }
  }
  return NULL; // Aucune correspondance trouvée
}

double Distance100metre(PoolData* pool, PersonneData personne) {
  // Vérification si les deux points ont été trouvés
  if (pool != NULL && personne.ident != "") {
    // Calcul de la distance entre les points géographiques
    Serial.println("Distance 100 metre passe");
    double distance = haversine(pool->lat, pool->lon, personne.lat, personne.lon);
    
    return distance;
  } else {
          Serial.println("Distance 100 metre passe pas");

    // Au moins l'un des points n'a pas été trouvé, retournez une valeur d'erreur ou effectuez un autre traitement selon vos besoins
    return -1.0;
  }
}

/*===== Arduino IDE paradigm : setup+loop =====*/
void setup() {
  
  int poolCount = 0; // Initialisation à 0
  int personneCount = 0;
  dht.begin();
  Serial.begin(9600);
  while (!Serial); // wait for a serial connection. Needed for native USB port only
   
  connect_wifi(); // Connexion Wifi  
  print_network_status();
  
  // Initialize the output variables as outputs
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
    pinMode(blue_led, OUTPUT);
  digitalWrite(redLed, LOW);// Set outputs to LOW
  digitalWrite(greenLed, LOW);// Set outputs to LOW
  digitalWrite(blue_led, LOW);// Set outputs to LOW

  setup_http_Get(&server); // Appel de la fonction pour configurer les routes GET
  server.begin(); // Démarrez le serveur

  // Init temperature sensor 
  //tempSensor.begin();
  
  // Init temperature sensor 
   dht.begin();   // <-- Ajoutez cette ligne ici
   //Recuper les informations de  mon ip et mac
    if (WiFi.status() == WL_CONNECTED){
    USE_SERIAL.print("\nWiFi connected : yes ! \n"); 
    last_ip = WiFi.localIP().toString();
    last_mac = String(WiFi.macAddress());
  } 
  else {
    USE_SERIAL.print("\nWiFi connected : no ! \n"); 
  }


  // set server of our client
  client.setServer(mqtt_server, 1883);
  client2.setServer(mqtt_server2, 1883);

  // set callback when publishes arrive for the subscribed topic
  client.setCallback(mqtt_pubcallback); 
  client2.setCallback(mqtt_pubcallbackclient2); 
  // Subscribe to the topic
  mqtt_mysubscribe((char*)TOPIC);
  mqtt_mysubscribeclient2((char*)TOPIC_DEMANDE);
  
}

/*============= TO COMPLETE ===================*/

/*============== CALLBACK ===================*/
void mqtt_pubcallback(char* topic, 
                      byte* message, 
                      unsigned int length) {
  /* 
   * Callback if a message is published on this topic.
   */
     StaticJsonDocument<1000> doc;
   
  Serial.print("Message arrived on topic : ");
  Serial.println(topic);
  Serial.print("=> ");

  // Byte list to String and print to Serial
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]); 
    messageTemp += (char)message[i];
  }
  Serial.println();
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, messageTemp);
  if (error) {
    Serial.print("deserializeJson() failed with code ");
    Serial.println(error.c_str());
    return;
  }

  if (String(topic) == TOPIC) {


      // Check the identifier
      String ident = doc["info"]["ident"];
   /*
      if (ident == "P_22204970") {
        Serial.println("Ignoring my own pool data");
        return;
      }
    */
      // Retrieve the coordinates and temperature
      float lat = doc["info"]["loc"]["lat"];
      float lon = doc["info"]["loc"]["lon"];
      float temp = doc["status"]["temperature"];
    
      // Store the data
       if (haversine(latitude, longitude, lat,lon)<=50.0) {
              // Check if the pool already exists in the array
              bool poolExists = false;
              for (int i = 0; i < poolCount; i++) {
                if (pools[i].ident == ident) {
                  poolExists = true;
                  break;
                }
              }
          
              // If the pool doesn't exist in the array, add it
              if (!poolExists && poolCount < 30) { // make sure not to overflow the array
                PoolData pd = {ident, lat, lon, temp};
                pools[poolCount] = pd;
                poolCount++;
              }
      
       }
  }
  

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic,
  // you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == TOPIC_DEMANDE) {
    Serial.print("TOPIC DE DEMANDE EST LA  ");
  }
}

/*============= SUBSCRIBE =====================*/
void  mqtt_mysubscribe(char *topic) {
  /*
   * Subscribe to a MQTT topic 
   */
  while (!client.connected()) { // Loop until we're reconnected

    Serial.print("Attempting MQTT connection...");
    // Attempt to connect => https://pubsubclient.knolleary.net/api
    if (client.connect("ibrahimkrimi", /* Client Id when connecting to the server */
		       NULL,    /* No credential */ 
		       NULL)) {
      Serial.println("connected");
      // then Subscribe topic
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      
      Serial.println(" try again in 5 seconds");
      delay(5000); // Wait 5 seconds before retrying
    }
  }
}



/*============== CALLBACK ===================*/
void mqtt_pubcallbackclient2(char* topic, 
                      byte* message, 
                      unsigned int length) {
  /* 
   * Callback if a message is published on this topic.
   */
     StaticJsonDocument<1000> doc;
   
  Serial.print("Message arrived on topic 2 : ");
  Serial.println(topic);
  Serial.print("=> ");

  // Byte list to String and print to Serial
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]); 
    messageTemp += (char)message[i];
  }
  Serial.println();
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, messageTemp);
  if (error) {
    Serial.print("deserializeJson() failed with code ");
    Serial.println(error.c_str());
    return;
  }

  // Extract the third element from topic as ident
  String ident = "";
  int slashCount = 0;
  for (int i = 0; i < strlen(topic); i++) {
    if (topic[i] == '/') {
      slashCount++;
      if (slashCount == 2) {
        while (topic[i + 1] != '/' && i + 1 < strlen(topic)) {
          ident += topic[i + 1];
          i++;
        }
        break;
      }
    }
  }
      String tid=doc["tid"];
      float lat = doc["lat"];
      float lon = doc["lon"];
  if (!ident.isEmpty() && !tid.isEmpty() && !isnan(lat) && !isnan(lon)) {
  // Check if ident already exists in personne array
  bool identExists = false;
  for (int i = 0; i < personneCount; i++) {
    if (personne[i].ident == ident) {
      identExists = true;
      break;
    }
  }

  if (!identExists && ident!="000000" ) {
    // Store the data
    if (personneCount < 30) { // make sure not to overflow the array
      PersonneData ps = { ident, tid, lat, lon };
      personne[personneCount] = ps;
      personneCount++;
      Serial.println("Creation of a new person.");
    }
  } else {
    // Person with the same ident already exists
    Serial.println("Person with the same ident already exists. Skipping creation of a new person.");
  }
} else {
  // Some required fields are empty, skip creating a new person
  Serial.println("Some fields are empty in the received message. Skipping creation of a new person.");
}
      
  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic,
  // you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == TOPIC_DEMANDE) {
    Serial.print("TOPIC DE DEMANDE EST LA  ");
  }
}

/*============= SUBSCRIBE =====================*/
void  mqtt_mysubscribeclient2(char *topic) {
  /*
   * Subscribe to a MQTT topic 
   */
  while (!client2.connected()) { // Loop until we're reconnected

    Serial.print("Attempting MQTT connection...");
    // Attempt to connect => https://pubsubclient.knolleary.net/api
    if (client2.connect("ibrahimkrimi2", /* Client Id when connecting to the server */
           NULL,    /* No credential */ 
           NULL)) {
      Serial.println("connected to client 2");
      // then Subscribe topic
      client2.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      
      Serial.println(" try again in 5 seconds");
      delay(5000); // Wait 5 seconds before retrying
    }
  }
}


void setup_http_Get(AsyncWebServer* server) {
server->on("/open", HTTP_GET, [](AsyncWebServerRequest *request){
  if (request->hasParam("idswp") && request->hasParam("idu")) {
    String idswp = request->getParam("idswp")->value();
    String idu = request->getParam("idu")->value();
    String Date = request->getParam("datetime")->value();
    
    // Utilisez les valeurs récupérées idswp et idu comme requis
    // Par exemple, vous pouvez les afficher sur la console série :
    Serial.println("idswp: ");
    Serial.println(idswp); //id piscine
    Serial.print("idu: ");
    Serial.println(idu); // id user   
    Serial.print("Date: ");
    Serial.println(Date); // DateTime
    // Recherche de la personne et de la piscine correspondantes
      PersonneData* personneTrouvee = findpersonne(idu);  
      PoolData* poolTrouvee = findpool(idswp);
    // Vérification si les deux correspondances ont été trouvées
      if (personneTrouvee != NULL && poolTrouvee != NULL) {
        // Les deux correspondances ont été trouvées
         double distance = Distance100metre(poolTrouvee, *personneTrouvee);
         Serial.println("La distance est egale");
         Serial.println(distance);
         if (distance <= 10000.0) {
         // Vérification si la demande existe déjà
          bool demandeExiste = false;
          for (int i = 0; i < demandesCount; i++) {
            if (demandes[i].id_user == personneTrouvee->ident && demandes[i].id_piscine == poolTrouvee->ident && demandes[i].Date_d_sortie == "00/00/0000" ) {
              // La demande existe déjà, remplissez la date de sortie avec la date actuelle
              demandes[i].Date_d_sortie = Date;
              demandeExiste = true;
               digitalWrite(blue_led, HIGH);// Set outputs to LOW
              //delay(30000)-delayinterdit enasynchrone
              //digitalWrite(blue_led, LOW);// Set outputs to LOW
                char jsonStringPost[1000];
                StaticJsonDocument<1000> JsonDemandes=create_jsonDemandes(    demandes[i].id_user,   demandes[i].id_piscine,   demandes[i].Nom_user,   demandes[i].Date_d_entree, demandes[i].Date_d_sortie ,demandes[i].distance, demandes[i].lan_piscine, demandes[i].lat_piscine, demandes[i].temp_piscine);
 
               serializeJson(JsonDemandes, jsonStringPost);
                String message = jsonStringPost;
                // Serial info
                Serial.print("Demande To Json:"); 
                Serial.println( message.c_str());
               // Send the JSON string to your server
              String response = httpPOSTRequest(jsonStringPost);
              demandes[i] = demandes[demandesCount - 1]; // Remplacer l'élément à supprimer par le dernier élément

              
              demandesCount--; // Réduire la taille du tableau de 1
              break;
            }
          }
             if (!demandeExiste) {
              // La demande n'existe pas encore, remplissez la date d'entrée avec la date actuelle
              // et la date de sortie avec "00/00/0000"
              demandes[demandesCount].id_user = personneTrouvee->ident;
              demandes[demandesCount].id_piscine = poolTrouvee->ident;
              demandes[demandesCount].Nom_user = personneTrouvee->tid;
              demandes[demandesCount].Date_d_sortie = "00/00/0000";
              demandes[demandesCount].Date_d_entree = Date;
              demandes[demandesCount].distance = distance;
              demandes[demandesCount].lan_piscine = poolTrouvee->lat;
              demandes[demandesCount].lat_piscine = poolTrouvee->lon;
              demandes[demandesCount].temp_piscine = poolTrouvee->temp;
              demandesCount++; 
              Serial.println("Demande Stocker dans le tableau");
              char jsonStringPost[1000];
              StaticJsonDocument<1000> JsonDemandes=create_jsonDemandes(   personneTrouvee->ident,   poolTrouvee->ident,   personneTrouvee->tid,   demandes[demandesCount-1].Date_d_entree, demandes[demandesCount-1].Date_d_sortie ,distance, poolTrouvee->lon,poolTrouvee->lat, poolTrouvee->temp);
                serializeJson(JsonDemandes, jsonStringPost);
                String message = jsonStringPost;
                // Serial info
                Serial.print("Demande To Json:"); 
                Serial.println( message.c_str());
               // Send the JSON string to your server
              String response = httpPOSTRequest(jsonStringPost);
              // Print the server's response
              Serial.println("la reponse de Post : ");
              Serial.println(response);
              Serial.println("fin de reponse");
              digitalWrite(blue_led, HIGH);// Set outputs to LOW
                          // Ajout d'une position au tableau
            if (positionsCount < MAX_POSITIONS) {
              positions[positionsCount] = create_jsonPostion(personneTrouvee->ident, personneTrouvee->tid, personneTrouvee->lat, personneTrouvee->lon, "Yellow");
              positionsCount++;
            } else {
              Serial.println("Erreur: Le tableau de positions est plein !");
            }
            

            /*--- Publish information postion Demandes */
 
              //delay(30000); 
            }
            else{
             Serial.println("Demande existe dans le tableau Date de Sortie faite");
              }
            
            } else {
          Serial.println("La distance est supérieure ou égale à 100 Kilometre");
        }
        
      } else {
        // Au moins l'une des correspondances n'a pas été trouvée, affichez un message d'erreur
        Serial.println("Erreur : Personne ou piscine introuvable");
      }
  }
  else {
      // Un ou les deux paramètres manquent, affichez un message d'erreur
      Serial.println("Erreur : Paramètres manquants");
    }
  request->send(200); // Réponse OK à la requête GET
  });
}

void afficherDemandes() {
  Serial.println("Liste des demandes :");
  for (int i = 0; i < demandesCount; i++) {
    Serial.println("------------------------");
    Serial.print("Demande ");
    Serial.print(i + 1);
    Serial.print(": id_user = ");
    Serial.print(demandes[i].id_user);
    Serial.print(", id_piscine = ");
    Serial.print(demandes[i].id_piscine);
    Serial.print(", Nom_user = ");
    Serial.print(demandes[i].Nom_user);
    Serial.print(", Date_d_sortie = ");
    Serial.print(demandes[i].Date_d_sortie);
    Serial.print(", Date_d_entree = ");
    Serial.print(demandes[i].Date_d_entree);
    Serial.print(", Distance = ");
    Serial.print(demandes[i].distance);
    Serial.print(", Latitude de la piscine = ");
    Serial.print(demandes[i].lan_piscine);
    Serial.print(", Longitude de la piscine = ");
    Serial.print(demandes[i].lat_piscine);
    Serial.print(", Temperature de la piscine = ");
    Serial.println(demandes[i].temp_piscine);
  }
}
void afficherPersonnes() {
  //String datajson=httpGETRequest();
    //Serial.println(datajson);
  Serial.println("Liste des personnes :");
  for (int i = 0; i < personneCount; i++) {
    Serial.println("------------------------");
    Serial.print("Personne ");
    Serial.print(i + 1);
    Serial.print(": Ident = ");
    Serial.print(personne[i].ident);
    Serial.print(": tid = ");
    Serial.print(personne[i].tid);
    Serial.print(", Latitude = ");
    Serial.print(personne[i].lat);
    Serial.print(", Longitude = ");
    Serial.println(personne[i].lon);
  }
}
void afficherPools() {
  Serial.println("Liste des piscines :");
  for (int i = 0; i < poolCount; i++) {
    Serial.println("------------------------");
    Serial.print("Piscine ");
    Serial.print(i + 1);
    Serial.print(": Ident = ");
    Serial.print(pools[i].ident);
    Serial.print(", Latitude = ");
    Serial.print(pools[i].lat);
    Serial.print(", Longitude = ");
    Serial.print(pools[i].lon);
    Serial.print(", Temperature = ");
    Serial.println(pools[i].temp);
  }
}
/*================= LOOP ======================*/
void loop() {
  int32_t period = 7000; // 5 sec

  if (digitalRead(blue_led)) {
    cpt = cpt + 1;

    if ( cpt >= 3 ) {
        digitalWrite(blue_led, LOW);// Set outputs to LOW
        cpt = 0;
     }
  }
   if (!client2.connected()) { 
    mqtt_mysubscribeclient2((char *)(TOPIC_DEMANDE));
  }
  /*--- subscribe to TOPIC_LED if not yet ! */
  if (!client.connected()) { 
    mqtt_mysubscribe((char *)(TOPIC));
  }
    

temperature = get_Temperature();
bool red = false;
float maximum = pools[0].temp;
String sta ="Green" ;
  for (int i = 1; i < poolCount; i++) {
     if( pools[i].temp >=maximum) {
      maximum=pools[i].temp;
    }
  }

  if( temperature >=maximum) {
      red = true;
      sta ="Red";
      greenLedon(LOW);
      redLedon(HIGH);
     }else{
      redLedon(LOW);
      greenLedon(HIGH);
          }
String presence = "non" ;
          float myLight = get_light(LightPin);
          Serial.print("presence :"); 
  Serial.println(get_light(LightPin));
          if (myLight > Light_threshold){
            presence = "oui";
          }
  /*--- Publish Temperature periodically   */
 StaticJsonDocument<1000> piscine_data = init_json("Piscine Yassine","j'aime la pizza",temperature, latitude, longitude,sta,presence,last_ip);
  char jsonString[1000];
  afficherPersonnes();
  afficherPools();
  afficherDemandes();
  serializeJson(piscine_data, jsonString);
  String message = jsonString;
  // Serial info
  Serial.print("Published Informations :"); 
  Serial.println( message.c_str());
  // MQTT Publish
  client.publish(TOPIC, message.c_str());
 
for (int i = 0; i < positionsCount; i++) {
    char jsonStringPostion[1000];
    serializeJson(positions[i], jsonStringPostion);
    String MessagePostion = jsonStringPostion;
    Serial.print("Postion envoyer  :"); 
    Serial.println(MessagePostion.c_str());
    // MQTT Publish
    client2.publish(TOPIC_Postion, MessagePostion.c_str());
  
    // Remplacer l'élément actuel par le dernier élément
    positions[i] = positions[positionsCount - 1];
    // Réduire la taille du tableau de 1
    positionsCount--;
}

  /*--- Publish information postion Demandes */
  /*
   * 
  StaticJsonDocument<1000> Postion= create_jsonPostion("00","IB", 43.621887,7.046368,"Yellow");
              char jsonStringPostion[1000];
              serializeJson(Postion, jsonStringPostion);
              String MessagePostion = jsonStringPostion;
              // Serial info
              Serial.print("Postion envoyer  :"); 
              Serial.println( MessagePostion.c_str());
              // MQTT Publish
              client2.publish(TOPIC_Postion, MessagePostion.c_str());


 */
  client.loop();
  client2.loop();
  delay(period);  

 }
 
