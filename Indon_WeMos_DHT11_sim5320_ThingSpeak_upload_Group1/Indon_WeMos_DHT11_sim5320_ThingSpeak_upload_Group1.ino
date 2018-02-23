#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include <DHT.h>
#include <ThingSpeak.h>
//#include <ESP8266Wifi.h>
#include <Servo.h>

// USER SETTINGS
const unsigned int LOGINTERVAL = 10000; //in milliseconds
String myMobileNum = "081805572414"; //no need international code in front, just use local number
unsigned long myChannelNumber = 428538; //from own Thingspeak account
const char*  myWriteAPIKey = "1N312W3GMY8D2N6R"; //from own Tohingspeak account

#define FONA_RX D6
#define FONA_TX D7
#define FONA_RST 4

SoftwareSerial fonaSS = SoftwareSerial(FONA_RX, FONA_TX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);

#define DHTPIN D3     // what digital pin we're connected to (WeMos pullup pins are D3 and D4)
#define DHTTYPE DHT11 // DHT 22  (AM2302), AM2321
/* ***********************************************
* Tambahan Baru
* ************************************************/
#define trigPin D2
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

// const char* ssid = "SEA-Conference";
// const char* password = "";

// unsigned long myChannelNumber = 432290;
// const char*  myWriteAPIKey = "53DNJWUZ52ZTS4UE";

// // the IP address for the shield:
// IPAddress ip(192, 168, 1, 137);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);

// //Sleep for 10min - 10 x 60sec
// const int sleepTimeS = 2 * 60;

// WiFiClient  client;
/* ***********************************************
* Tambahan Baru
* ************************************************/
DHT dht(DHTPIN, DHTTYPE);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);
unsigned long lastLoggedTime;

void setup() {

  Serial.begin(115200);
  fonaSerial->begin(115200);

  fonaSS.println(F("AT+IPR=115200"));  // Set baud to 9600
  delay(100); // Let the command run
  fonaSerial->begin(115200);
  
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println(F("Enabling GPS...allow up to 15min to get initial fix"));
  fona.enableGPS(true);
  Serial.println(F("FONA is ready"));

  pinMode(DHTPIN, INPUT_PULLUP);
  dht.begin();  
  // mengakses HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // mengakses servo untuk ventilasi
  ventilasi.attach(servoPin);
  ventilasi.write(0);
  // Wifi ESP
  
  delay(1000);
  Serial.println("DHT Ready");
  Serial.println("Servo Ready");
  Serial.println("HC-SR04 Ready");

//    // We start by connecting to a WiFi network
//   Serial.println();
//   Serial.print("Connecting to  ");
//   Serial.println(ssid);

//  WiFi.begin(ssid, password);

//  int timeout = 50;
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//    if (!timeout--) {
//      Serial.println("Timeout, Going into deep sleep");
//      ESP.deepSleep(sleepTimeS * 1000000, WAKE_RFCAL);
//    }
//  }

//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());

//  ThingSpeak.begin(client);

  //  ThingSpeak.begin(client); //need to change to GPRS settings

  print_menu();
}

void loop() { //choose only one of the following, comment out the others
  
 silo();

  // run_menu();

 // send_dht_thingspeak();

//  send_gps_thingspeak();
}


