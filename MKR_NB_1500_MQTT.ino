
#include <MKRNB.h>
#include <PubSubClient.h>

const char PINNUMBER[]     = "";

#define TOKEN ""
#define MQTT_CLIENT_NAME "MKR"

//pin nambur relay connected
#define RELAY 7

//broker ip
char mqttBroker[]  = "x.x.x.x";

//broker port in here
int mqttPort = 1883;

//var things for storage
char payload[100];
char topic[150];
char topicSubscribe[100];

//insert topic
char subtopic[50] = "prueba";

// initialize the library instance
NBClient client;
NB nbAccess;

//connect the pubsub client
PubSubClient conn(client);

//for the callbacks from the broker
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
 
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(RELAY,LOW);// Turn the LED off by making the voltage HIGH
  } else {
    
   digitalWrite(RELAY,HIGH);
  }


 
}

//connection and reconnection function
void reconnect() {
  while (!conn.connected()) {

  
    if (conn.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
      conn.subscribe("online");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(conn.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY,HIGH);
  while (!Serial) {

  }

  Serial.println("WAITING CONNECTION....");
  // connection state
  boolean connected = false;

  while (!connected) {
    if ((nbAccess.begin(PINNUMBER) == NB_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("Connecting...");

  //set the connection
  conn.setServer(mqttBroker, mqttPort);

  //set the callback
  conn.setCallback(callback);

  //subscribe to a topic
  sprintf(topicSubscribe, subtopic);

}

void loop() {

  //if the connection drops then reconnect it
  if (!conn.connected()) {
    reconnect();
    conn.subscribe(topicSubscribe);
  }
  conn.loop();


}
