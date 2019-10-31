////////////////////////////////////////////////////////////////[CLIENTE MQTT NP2019
//
// Este codigo envia el valor analogo via wifi a un broker MQTT en topic 'casa/sensor'
// asi como tambien recibe ordenes desde el broker en topic 'casa/rele'
//
// Se puede probar con broker mosquitto (https://mosquitto.org/download/)
// Para leer datos del sensor:
//  mosquitto_sub -h localhost -t "casa/sensor"
// Para manejar remotamente el rele, publicar (0/1):
//  mosquitto_pub -h localhost -t "casa/rele" -m "1"
////////////////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
const char* ssid = "NPALGO-TPLINK"; //ACA VA EL NOMBRE DE LA RED WIFI
const char* password = "0141476165"; //ACA VA LA PASS DEL WIFI
const char* mqtt_server = "paradigma.no-ip.org"; //ACA VA LA DIRECCION DEL BROKER MQTT

/////////////////////////////////////////////////////////[VARIABLES
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

int analogo1 = A0;
int sensor1 = 0;
String topic1= "casa/sensor";
String topic2= "casa/rele";

/////////////////////////////////////////////////////////[SETUP
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);    
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

/////////////////////////////////////////////////////////[CONECTAR WIFI
void setup_wifi() {
 
  delay(10);

  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

////////////////////////////////////////////////////////[RECIBIR MENSAJES (CALLBACK)
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje de entrada: [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
 
  // SI LLEGA UN 1 PRENDER UN LED (nodemcu tiene logica inversa)
  if ((char)payload[0] == '1') {
    Serial.println("LED ON");
    digitalWrite(BUILTIN_LED, LOW);   
  } else {
    Serial.println("LED OFF");
    digitalWrite(BUILTIN_LED, HIGH);  
  }
 
}

////////////////////////////////////////////////////////[RECONECTAR
void reconnect() {
  while (!client.connected()) {
    Serial.print("Estableciendo una coneccion MQTT...");
  
    if (client.connect("ESP8266Client")) {
      Serial.println("conectado");

      sensor1= analogRead(analogo1);
      snprintf (msg, 75, "%ld", sensor1);
      client.publish("casa/sensor" , msg );
      client.subscribe("casa/rele");
      
    } else {
      Serial.print("error");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

////////////////////////////////////////////////////////[TEMPORIZADOR
void Temporizador(int milisegundos){
  long now = millis();
  if (now - lastMsg > milisegundos) {
    lastMsg = now;

    //publicar
    sensor1= analogRead(analogo1);
    snprintf (msg, 75, "%ld", sensor1);
    client.publish("casa/sensor" , msg );
  }
}
void loop() {
 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Temporizador(1000);

}
