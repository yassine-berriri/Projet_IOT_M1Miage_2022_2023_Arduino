#include "ESPAsyncWebServer.h"
#include "routes.h"
#include <HTTPClient.h>
#include <esp_task_wdt.h>

String proccesus(const String& var){

  return "Hallo "; 
}




String httpPOSTRequest(String httpRequestData) {
  /*
     This method sends data via HTTP POST requests to host/url
     and return the response/payload of the request to UrlServer
  */
  
      String UrlServer="https://iot-27a4.onrender.com/insert";
       String payload = "{}";
        HTTPClient http; // http protocol entity =>  client
      
        USE_SERIAL.printf("Requesting URL : %s\n", UrlServer);
      
        // Configure traged server and url
        http.begin(UrlServer);

        // Header
          http.addHeader("Content-Type", "application/json");
        // POST
        int httpResponseCode = http.POST(httpRequestData);

        // Get the response => will fill payload String
        if (httpResponseCode > 0) {
          USE_SERIAL.printf("HTTP Response code : %d", httpResponseCode);
          payload = http.getString();
        }
        else {
          USE_SERIAL.printf("Error code on HTTP GET Request : %d", httpResponseCode);
        }
        // End connection and Free resources
        http.end();
      
        return payload;

}


String httpGETRequest() {
  String UrlServer = "https://iot-27a4.onrender.com/get";
  String payload = "{}";
  
  HTTPClient http; // Entité cliente pour les requêtes HTTP
  
  USE_SERIAL.printf("Requesting URL: %s\n", UrlServer);
  
  // Configuration du serveur et de l'URL
  http.begin(UrlServer);
  
  // Envoi de la requête GET
  int httpResponseCode = http.GET();
  
  // Récupération de la réponse dans la variable payload
  if (httpResponseCode > 0) {
    USE_SERIAL.printf("HTTP Response code: %d\n", httpResponseCode);
    payload = http.getString();
  } else {
    USE_SERIAL.printf("Error code on HTTP GET Request: %d\n", httpResponseCode);
  }
  
  // Fermeture de la connexion et libération des ressources
  http.end();
  
  return payload;
}
