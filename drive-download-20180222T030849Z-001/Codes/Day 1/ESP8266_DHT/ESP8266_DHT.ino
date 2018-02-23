#include <DHT.h>

/////////////////////////
// Device Definitions //
/////////////////////////
#define DHTPIN D3     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

//////////////////////
// Other Variables //
//////////////////////  
const float hcal = 0;
const float tcal = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();

  dht.begin();
  delay(1000); // delay 1 sec 

  Serial.println("DHT Ready");
}

void loop() {
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity() + hcal;
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature() + tcal;
  delay(1000);
  
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
}
