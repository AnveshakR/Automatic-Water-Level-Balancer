#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//defining the variables
const int trigLower = D6;
const int echoLower = D7;
const int trigUpper = D4;
const int echoUpper = D3;
const int pump = D5;
bool pumpState = false;
int notifTimer = 0;


char auth[] = "YVxzwCyvuPFHo8OYn09espqa4hzHwvYg";
char ssid[] = "Trojan";
char pass[] = "NModi1@9";

long durLow,durHigh;
int distLow,distHigh;
BlynkTimer timer;


void setup() {
 pinMode(trigLower,OUTPUT);
 pinMode(echoLower,INPUT);
 pinMode(trigUpper,OUTPUT);
 pinMode(echoUpper,INPUT);
 pinMode(pump,OUTPUT);
 Blynk.begin(auth,ssid,pass);
 timer.setInterval(1000L, push);
 Serial.begin(9600);
}

void push() {
  //checking level of lower tank
  digitalWrite(trigLower,LOW);
  delayMicroseconds(2);
  digitalWrite(trigLower,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigLower,LOW);
  durLow = pulseIn(echoLower,HIGH);
  distLow = durLow*0.034/2;
  if(distLow <= 13){
    notifTimer = 0;
    //checking level of upper tank
    digitalWrite(trigUpper,LOW);
    delayMicroseconds(2);
    digitalWrite(trigUpper,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigUpper,LOW);
    durHigh = pulseIn(echoUpper,HIGH);
    distHigh = durHigh*0.034/2;
    //turning off pump
    if(distHigh<=4  && pumpState == true){
      digitalWrite(pump,LOW);
      pumpState = false;
    }
    //turning on pump
    if(distHigh>=8 && pumpState == false){
      digitalWrite(pump,HIGH);
      pumpState = true;
    }
  }
  else{
    //this ensures only 1 notif is sent
    if(notifTimer == 0){ 
      Blynk.notify("Water level low, please top up.");
    }
    notifTimer = 1;

    //if pump is on and lower tank is low, then stop pumping
    digitalWrite(pump,LOW);
    pumpState = false;
  }
}

void loop(){
  Blynk.run();
  timer.run();
}
