

//Tech Trends Shameer
//Control LED Using Blynk 2.0/Blynk IOT

#define BLYNK_TEMPLATE_ID "TMPLyC9JpcLL"
#define BLYNK_DEVICE_NAME "led test"
#define BLYNK_AUTH_TOKEN "7k2nMHQ5FNPnzgsAwnjRfm1I4ZiwR-HY"
#define BLYNK_PRINT Serial
#define CSE_IP "192.168.247.189"                        //
#define CSE_PORT 5089                                
#define OM2M_ORGIN "admin:admin"
#define OM2M_MN "/~/in-cse/in-name/"
#define OM2M_AE1 "DHT11"
#define OM2M_AE2 "LDR"
#define OM2M_DATA_CONT "Node-1/Data"
#define INTERVAL 2000L
#define DHTTYPE    DHT11
#define DHTPIN D2

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>  
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <time.h>
#include <JSONVar.h>
#include <JSON.h>
#include <Arduino_JSON.h>
#include <SoftwareSerial.h>

const char* host = "api.thingspeak.com";
const char* APIKey = "BIJTD90P8JA23PQ2";
const char * ntpServer = "pool.ntp.org";

long randNumber;
long int prev_millis = 0;
unsigned long epochTime;

HTTPClient http;
WiFiClient client;



DHT dht(DHTPIN, DHTTYPE);



char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "ABCD";  // Enter your Wifi Username
char pass[] = "12345678";
int ledpin = D1;

int LOCAL_PORT = 9999;
char* HTTP_CREATED = "HTTP/1.1 201 Created";
char* HTTP_OK    = "HTTP/1.1 200 OK\r\n";
int REQUEST_TIME_OUT = 5000; //ms


#define DEBUG
///////////////////////////////////////////
// Global variables
WiFiServer server(LOCAL_PORT);    // HTTP Server (over WiFi). Binded to listen on LOCAL_PORT contant

void setup()
{     
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);    
  pinMode(ledpin,OUTPUT);
  pinMode(D0,INPUT);
  dht.begin();
  
  

}
int value = 0;
int value1= 0;
int value2= 0;
void loop()
{
  Blynk.run();
  int state = digitalRead(D0);
  Serial.print(state);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  delay(2000);
    value = t;
    value1 =h;
    value2=state;

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/update";
    url += "?api_key=";
    url += APIKey;
    url += "&field1=";
    url += value;
       
    url += "?api_key=";
    url += APIKey;
    url += "&field2=";
    url += value1;  
      
    url += "?api_key=";
    url += APIKey;
    url += "&field3=";
    url += value2;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");

     
  if (millis() - prev_millis >= INTERVAL) {
    String data;

    String server = "http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String() + OM2M_MN;

    http.begin(client,server + String() + OM2M_AE1 + "/" + OM2M_DATA_CONT + "/");

    http.addHeader("X-M2M-Origin", OM2M_ORGIN);
    http.addHeader("Content-Type", "application/json;ty=4");
    http.addHeader("Content-Length", "100");

    data = "[" + String(t) + ", " + String(h) +   + "]"; 
    String req_data = String() + "{\"m2m:cin\": {"

      +
      "\"con\": \"" + data + "\","

      +
      "\"lbl\": \"" + "V1.0.0" + "\","

      //+ "\"rn\": \"" + "cin_"+String(i++) + "\","

      +
      "\"cnf\": \"text\""

      +
      "}}";
    int code = http.POST(req_data);
    http.end();
    Serial.println(code);

    http.begin(client, server + String() + OM2M_AE2 + "/" + OM2M_DATA_CONT + "/");

    http.addHeader("X-M2M-Origin", OM2M_ORGIN);
    http.addHeader("Content-Type", "application/json;ty=4");
    http.addHeader("Content-Length", "100");

    data = "[" + String(state) +   + "]"; 
    req_data = String() + "{\"m2m:cin\": {"

      +
      "\"con\": \"" + data + "\","

      +
      "\"lbl\": \"" + "V1.0.0" + "\","

      //+ "\"rn\": \"" + "cin_"+String(i++) + "\","

      +
      "\"cnf\": \"text\""

      +
      "}}";

      
    int code1 = http.POST(req_data);
    http.end();
    Serial.println(code1);
  }
}
