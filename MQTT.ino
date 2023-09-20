/*********
  Wagner Becker
*********/

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// Replace the next variables with your SSID/Password combination
const char* ssid = "iPhone";
const char* password = "eae200111";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "7179422ab76b4c6d8ecaa4a5dcfc530b.s2.eu.hivemq.cloud";
const char* mqtt_username = "Embarcado"; // replace with your Credential
const char* mqtt_password = "Embarcado2023";
const int mqtt_port = 8883;

WiFiClientSecure espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";


#define DHTPIN 14 
#define DHTTYPE DHT22 
#define CONNECTION_TIMEOUT 10
#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     -10800
#define UTC_OFFSET_DST 0

DHT dht(DHTPIN, DHTTYPE);
const int oneWireBus = 13;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);


void setup() {
  Serial.begin(115200);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  
  setup_wifi();
  while (!Serial) delay(1);
  espClient.setCACert(root_ca);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  dht.begin();
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

void setup_wifi() {
  WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");
    int timeout_counter = 0;

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(200);
        timeout_counter++;
        if(timeout_counter >= CONNECTION_TIMEOUT*5){
        ESP.restart();
        }
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
  }

  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Wag-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      //SUBSCRIBE TO TOPIC HERE

     client.subscribe("esp32/output");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.print("Connection Err");
    return;
  }
  int hora = timeinfo.tm_hour;
  int min = timeinfo.tm_min;
  int seg = timeinfo.tm_sec;
  int dia = timeinfo.tm_mday;
  int mes = timeinfo.tm_mon + 1;
  int ano = timeinfo.tm_year + 1900;
  char dataHora[20];
  sprintf(dataHora, "%02d/%02d/%04d %02d:%02d:%02d", dia, mes, ano, hora, min, seg);

  char* stime = "/dataHora";
  String macAddress = WiFi.macAddress();

  /// -------------------------------------
  // |Prepara o tópico do Tempo| 
  /// -------------------------------------
  // Determine o tamanho necessário para a string resultante
  int stimeSize = macAddress.length() + strlen(stime) + 1; // +1 para o caractere nulo
  // Aloque memória para a string resultante
  char* time = new char[stimeSize];
  // Copie o endereço MAC e a parte "/sensorTemperaturaAr" para a string ta
  strcpy(time, macAddress.c_str());
  strcat(time, stime);
  client.publish(time, dataHora);

  Serial.print(&timeinfo, "%d/%m/%Y  ");
  Serial.println(&timeinfo, "%H:%M:%S");
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  struct tm timecalc;
  if (!getLocalTime(&timecalc)) {
    Serial.print("Not Connected to NTP");
    return;
  }
  int min = timecalc.tm_min;
  static bool executedThisMinute = false;

  if (min % 2 == 0 && !executedThisMinute) {
    executedThisMinute = true;

    /*long now = millis();
    if (now - lastMsg > 1000) {
      lastMsg = now;*/
    
    /// ------------------------------------------------------
    // |Lê o sensor DHT22(Temp./Hum. do Ar) e prepara os valores| 
    /// ------------------------------------------------------
    float ah = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float at = dht.readTemperature();

    char tempArString[8];
    dtostrf(at, 1, 2, tempArString);
    char humArString[8];
    dtostrf(ah, 1, 2, humArString);

    /// -------------------------------------------------------
    // |Lê o sensor BS18B20(Temp. do Solo) e prepara os valores| 
    /// -------------------------------------------------------
    sensors.requestTemperatures(); 
    float st = sensors.getTempCByIndex(0);

    char tempSoloString[8];
    dtostrf(st, 1, 2, tempSoloString);

    ///---------------------------------------
    char* sta = "/sensorTemperaturaAr";
    char* sha = "/sensorHumidadeAr";
    char* sts = "/sensorTemperaturaSolo";
    String macAddress = WiFi.macAddress();

    /// -------------------------------------
    // |Prepara o tópico da TEMPERATURA DO AR| 
    /// -------------------------------------
    // Determine o tamanho necessário para a string resultante
    int taSize = macAddress.length() + strlen(sta) + 1; // +1 para o caractere nulo
    // Aloque memória para a string resultante
    char* ta = new char[taSize];
    // Copie o endereço MAC e a parte "/sensorTemperaturaAr" para a string ta
    strcpy(ta, macAddress.c_str());
    strcat(ta, sta);
    
    /// -------------------------------------
    // |Prepara o tópico da HUMIDADE DO AR| 
    /// -------------------------------------
    // Determine o tamanho necessário para a string resultante
    int haSize = macAddress.length() + strlen(sha) + 1; // +1 para o caractere nulo
    // Aloque memória para a string resultante
    char* ha = new char[haSize];
    // Copie o endereço MAC e a parte "/sensorHumidadeAr" para a string ha
    strcpy(ha, macAddress.c_str());
    strcat(ha, sha);

    /// ---------------------------------------
    // |Prepara o tópico da Temperatura do Solo| 
    /// ---------------------------------------
    // Determine o tamanho necessário para a string resultante
    int tsSize = macAddress.length() + strlen(sts) + 1; // +1 para o caractere nulo
    // Aloque memória para a string resultante
    char* ts = new char[tsSize];
    // Copie o endereço MAC e a parte "/sensorTemperaturaSolo" para a string ts
    strcpy(ts, macAddress.c_str());
    strcat(ts, sts);

    /// ------------------------------------
    // |Faz as publicações, e printa na tela| 
    /// ------------------------------------

    printLocalTime();
    client.publish(ta, tempArString);
    client.publish(ha, humArString);
    client.publish(ts, tempSoloString);
    Serial.print(ta);
    Serial.print("/");
    Serial.println(tempArString);
    Serial.print(ha);
    Serial.print("/");
    Serial.println(humArString);
    Serial.print(ts);
    Serial.print("/");
    Serial.println(tempSoloString);

  } else if  (min % 2 != 0) {
    executedThisMinute = false;  // Redefina a variável para permitir a execução no próximo minuto par
  }
}
