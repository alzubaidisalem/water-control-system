
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "OnePlus 3T";
const char* password =  "0538350845";
  const char* swtopic = "sensor/myesp";
  const char* dtopic = "data";
const char* mqttServer = "m14.cloudmqtt.com";
const int mqttPort = 18338;
const char* mqttUser = "ysthqekm";
const char* mqttPassword = "iprkKwQKvGf4";
 int s1=0,t1,t2,contagem=0;
 int cur=0; int pre=0;  boolean flag=true;
  
 
WiFiClient espClient;
PubSubClient client(espClient);

const int  buttonPin = D2; // variable for D2 pin
const int  pump = D5; // output for pump 


void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(swtopic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  char p =(char) payload[0];              // check the subscribtion to control the pump
   if (p=='1') digitalWrite(pump, HIGH);
    else digitalWrite(pump, LOW); 
 
}

void setup() {
 pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pump, OUTPUT);
 
  
  Serial.begin(115200);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
   while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
    boolean b=  client.subscribe(swtopic);
        
 // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(buttonPin), pin_ISR, RISING);
  t2=millis(); t1=millis(); 
 s1=HIGH;
}




void loop() {
   
 client.loop();
 
  if (millis()-t1>500) {
    s1=!s1;
  digitalWrite(LED_BUILTIN, s1); 
    t1=millis();

  }

  if (millis()-t2>1000) {
    
    cur=contagem;
     contagem=0;
    t2=millis();
    
   if((cur==0)&&(pre>0)&&flag){
    client.publish(dtopic,String(cur).c_str(),true);
    flag=!flag;}
    
     if(cur) {
      Serial.println(cur);
       client.publish(dtopic,String(cur).c_str(),true);
       s1=!s1;
       digitalWrite(LED_BUILTIN, s1); 
       flag=true;
     }
        
        pre=cur; 
  }
}

//Interrupt function, so that the counting of pulse “rise ups” dont interfere with the rest of the code  (attachInterrupt)
  void pin_ISR(){
     
      contagem++;  }
      
  
