#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

int flagP1 = 1, flagP2 = 1, flagP3 = 1;

#define P1 D7
#define P2 D6
#define P3 D5

// Network ID
#define FIREBASE_HOST "integrated-parking-system-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "LjrmX5hG7BM25NJTSFRkjGLTxdvl5UAVHRCq24QU"
const char* ssid     = "hi, it's me. i'm the problem";
const char* password = "anti-hero";

const long utcOffsetInSeconds = 3600*7;
//we're in Indonesia,
//and its supposed to +7 in GMT
//so the program must be multiply by 7.
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

String dayStamp, timeStamp;

void setup() {
  
  // Initialize the time client
  timeClient.begin();
  
  
  Serial.begin(9600);

  // Networking
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //declaring pin
  pinMode(P1, INPUT);
  pinMode(P2, INPUT);
  pinMode(P3, INPUT);

  //start firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
}

void loop(){
 n  #define P1 D7
#define P2 D6
#define P3 D5
  WiFiClient client;

  // park 1
  int Slot1 = digitalRead(P1);
  
  if(Slot1 == LOW){
    // send status to firebase
    Firebase.setString("/ParkingLot/Slot1" , "Filled");

    // send logs to firebase
    if(flagP1 != Slot1) logs("Slot1", "In");

    flagP1 = Slot1;
  }
  else{
    // send status to firebase
    Firebase.setString("/ParkingLot/Slot1" , "Empty");

    // send logs to firebase
    if(flagP1 != Slot1) logs("Slot1", "Out");
    
    flagP1 = Slot1;
  }

  // park 2
  int Slot2 = digitalRead(P2);
  
  if(Slot2 == LOW){
    // send status to firebase
    Firebase.setString("/ParkingLot/Slot2" , "Filled");

    // send logs to firebase
    if(flagP2 != Slot2) logs("Slot2", "In");

    flagP2 = Slot2;
  }
  else{
    // send status to firebase
    Firebase.setString("/ParkingLot/Slot2" , "Empty");

    // send logs to firebase
    if(flagP2 != Slot2) logs("Slot2", "Out");
    
    flagP2 = Slot2;
  }


  // park 3
  int Slot3 = digitalRead(P3);
  
  if(Slot3 == LOW){
    // send status to firebase
    Firebase.setString("/ParkingLot/Slot3" , "Filled");

    // send logs to firebase
    if(flagP3 != Slot3) logs("Slot3", "In");

    flagP3 = Slot3;
  }
  else{
    // send status to firebase
    Firebase.setString("/ParkingLot/Slot3" , "Empty");

    // send logs to firebase
    if(flagP3 != Slot3) logs("Slot3", "Out");
    
    flagP3 = Slot3;
  }
  
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  
} 


// Function that gets current epoch time
void getTime() {
  timeClient.update();

   //Get a time structure
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  // Extract date
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;
  
  dayStamp = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.println(dayStamp);
  
  // Extract time
  timeStamp = timeClient.getFormattedTime();
  Serial.println(timeStamp);
}

void logs(String ParkingLot, String logs){
  getTime();

  StaticJsonBuffer<256> jsonBuffer;
  JsonObject& object = jsonBuffer.createObject(); 
  object["ParkingLot"] = ParkingLot;
  object["status"] = logs;
  object["date"] = dayStamp;
  object["time"] = timeStamp;

  // append a new value to /logs
  String name = Firebase.push("logs/", object);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  
  Serial.print("pushed: /logs/" + dayStamp + "/" + ParkingLot);
  Serial.println(name);  
}
