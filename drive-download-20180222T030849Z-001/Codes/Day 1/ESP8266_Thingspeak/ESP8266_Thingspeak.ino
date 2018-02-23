// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define DHTPIN D2     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

const float hcal = 0;
const float tcal = 0;

const char* ssid = "IOT_Community_Lab@UWCSEA";
const char* password = "commlabIOT1";

unsigned long myChannelNumber = 419725;
const char*  myWriteAPIKey = "MIGUO5OITI4C95I4";

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

  Serial.println("ESP8266 awake");

  dht.begin();
  delay(1000); // delay 1 sec 

  Serial.println("DHT Ready");

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to  ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int timeout = 50;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (!timeout--) {
      Serial.println("Timeout, Going into deep sleep");
      ESP.deepSleep(sleepTimeS * 1000000, WAKE_RFCAL);
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
}

void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity() + hcal;
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature() + tcal;

  delay(3000);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

  Serial.print(" |  Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");

  Serial.println("Posting results to ThinkSpeak");
  // Update the 2 ThingSpeak fields with the new data
  ThingSpeak.setField(1, (float)t);
  ThingSpeak.setField(2, (float)h);

  int result = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (result = 200) {
    Serial.println("Results Posted, closing connection");
  }
  else
  {
    Serial.print("Unable to post result, closing connection : ");
    Serial.println(result);
  }

  Serial.println();

}
