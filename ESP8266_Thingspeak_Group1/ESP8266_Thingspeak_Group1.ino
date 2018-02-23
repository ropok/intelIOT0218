// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Servo.h>

#define DHTPIN D3     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
#define trigPin D7
#define echoPin D5
#define servoPin D8
Servo ventilasi;
// kondisi ideal
int tIdeal = 15;
int hIdeal = 7;
const float decrement = 0.05;
float hSilo;
float selisihHSilo;
float penguranganIdeal;
float t,h;

const float hcal = 0;
const float tcal = 0;

const char* ssid = "SEA-Conference";
const char* password = "";

unsigned long myChannelNumber = 432290;
const char*  myWriteAPIKey = "53DNJWUZ52ZTS4UE";

WiFiClient  client;
DHT dht(DHTPIN, DHTTYPE);

// the IP address for the shield:
IPAddress ip(192, 168, 1, 137);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//Sleep for 10min - 10 x 60sec
const int sleepTimeS = 2 * 60;



void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  // mengakses HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // mengakses servo untuk ventilasi
  ventilasi.attach(servoPin);
  ventilasi.write(0);

  Serial.println("ESP8266 awake");

  dht.begin();
  delay(1000); // delay 1 sec

  Serial.println("DHT Ready");

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to  ");
  Serial.println(ssid);

//  WiFi.begin(ssid, password);
//
//  int timeout = 50;
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//    if (!timeout--) {
//      Serial.println("Timeout, Going into deep sleep");
//      ESP.deepSleep(sleepTimeS * 1000000, WAKE_RFCAL);
//    }
//  }
//
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//
//  ThingSpeak.begin(client);
}

/*====================================================
    Method untuk HC-SR04
  ===================================================*/
void HCSR04() {
  long duration;
  float distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  // untuk skala saja
  hSilo = (distance / 2)*10;
  if(hSilo>=100) hSilo=100;
  //  if (distance < 4) {  // This is where the LED On/Off happens
  //    digitalWrite(led,HIGH); // When the Red condition is met, the Green LED should turn off
  //  digitalWrite(led2,LOW);
  //}
  //  else {
  //    digitalWrite(led,LOW);
  //    digitalWrite(led2,HIGH);
  //  }
  //  if (distance >= 200 || distance <= 0){
  //    Serial.println("Out of range");
  //  }
  //  else {
  //  Serial.print(distance / 2);
  //  Serial.println(" m");
  //  //  }
  //  delay(500);
}


void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity() + hcal;
  // Read temperature as Celsius (the default)
  t = dht.readTemperature() + tcal;
  HCSR04(); // Mendapatkan data Isi(%) dari Silo
  int pos;
  
  selisihHSilo = 100 - hSilo;
  penguranganIdeal = selisihHSilo*0.05;
  // if ((hSilo == hSilo) && ( t!=tIdeal-penguranganIdeal || h!=hIdeal-penguranganIdeal ))
  // {
  //   // buka Ventilasi
  //   ventilasi.write(90);
  // }
  // // tutup Ventilasi
  // else ventilasi.write(0);
  
  if ( t!=tIdeal-penguranganIdeal || h!=hIdeal-penguranganIdeal )
  {
    // buka Ventilasi
    ventilasi.write(90);
  }
  // tutup Ventilasi
  else ventilasi.write(0);

  bacaData();

//  // Mulai pengiriman data ke thinkspeak
//  delay(3000);
//
//  // Check if any reads failed and exit early (to try again).
//  if (isnan(h) || isnan(t)) {
//    Serial.println("Failed to read from DHT sensor!");
//  }
//
//  Serial.print("Temperature: ");
//  Serial.print(t);
//  Serial.print(" *C ");
//
//  Serial.print(" |  Humidity: ");
//  Serial.print(h);
//  Serial.println(" %\t");
//
//  Serial.println("Posting results to ThinkSpeak");
//  // Update the 2 ThingSpeak fields with the new data
//  ThingSpeak.setField(1, (float)t);
//  ThingSpeak.setField(2, (float)h);
//
//  int result = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
//  if (result = 200) {
//    Serial.println("Results Posted, closing connection");
//  }
//  else
//  {
//    Serial.print("Unable to post result, closing connection : ");
//    Serial.println(result);
//  }
//
//  Serial.println();

}

void bacaData()
{
  Serial.println();
  Serial.print(" hSilo : "); Serial.print(hSilo);
  Serial.println();
//  Serial.print(" selisihHSilo : "); Serial.print(selisihHSilo);
//  Serial.println();
//  Serial.print(" penguranganIdeal : "); Serial.print(penguranganIdeal);
//  Serial.println();
  Serial.print(" tIdealSekarang : "); Serial.print(tIdeal-penguranganIdeal);
  Serial.println();
  Serial.print(" hIdealSekarang : "); Serial.print(hIdeal-penguranganIdeal);
  Serial.println();
  Serial.print(" temperature : "); Serial.print(t);
  Serial.println();
  Serial.print(" humidity : "); Serial.print(h);
  Serial.println();
  delay(350);
}
