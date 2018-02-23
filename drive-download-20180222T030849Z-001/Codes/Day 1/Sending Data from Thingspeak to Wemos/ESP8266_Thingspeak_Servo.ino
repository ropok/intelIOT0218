// Last Edit - Terence Ang - 21 Feb 2018 - 8pm
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Servo.h>

#define DHTPIN D3     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

const float hcal = 0;
const float tcal = 0;

const char* ssid = "AndroidAP";
const char* password = "theonlyone";

//ThingSpeak Settings

unsigned long myChannelNumber = 431893;
const char*  myWriteAPIKey = "UN98S6GP7MI9FJW8";

String thingSpeakAPI = "api.thingspeak.com";
String talkBackAPIKey = "L1VXZZBMJI1TDSO4";
String talkBackID = "23352";

//----

WiFiClient  client;
DHT dht(DHTPIN, DHTTYPE);

Servo myservo;

int pos;

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("ESP8266 ready");

  myservo.attach(D2);  // attaches the servo on GIO2 to the servo object
  dht.begin();
  Serial.println("Testing Servos");
  myservo.write(0);
  delay(500);
  myservo.write(90);
  delay(500);
  myservo.write(0);
  delay(500);

  Serial.println("Servo Ready");
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
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);

  Serial.println("");

}

void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  
  float h = dht.readHumidity() + hcal;
  
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature() + tcal;

  delay(2000);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
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


  Serial.println("Checking Talkback from ThinkSpeak");
  getTalkback();

}

// Perform an HTTP GET request to a remote page
bool getTalkback() {

  String talkBackCommand;
  char charIn;

  // Attempt to make a connection to the remote server
  if ( !client.connect(thingSpeakAPI, 80) ) {
    Serial.println("connect falied");
    return false;
  }

  // Make an HTTP GET request
  client.println("GET /talkbacks/"+talkBackID+"/commands/execute?api_key="+talkBackAPIKey+" HTTP/1.1");
  client.print("Host: ");
  client.println(thingSpeakAPI);
  client.println("Connection: close");
  client.println();


  while (!client.available()) {
    delay(1000);
  }

  talkBackCommand = "";

  while ( client.available() ) {
    charIn = client.read();
    talkBackCommand += charIn;
  }

  //Serial.println(talkBackCommand);

  if (talkBackCommand.endsWith("SERVO_0")) {
    Serial.println("Turn to 0 deg");
    if (pos != 0) {
      myservo.write(0);
      delay(300);
      pos = 0;
    }

  }
  else if (talkBackCommand.endsWith("SERVO_90")) {
    Serial.println("Turn to 90 deg");
    if (pos != 90) {
      myservo.write(90);
      delay(300);
      pos = 90;
    }
  }
  else if (talkBackCommand.endsWith("SERVO_180")) {
    Serial.println("Turn to 180");
    if (pos != 180) {
      myservo.write(180);
      delay(300);
      pos = 180;
    }
  }

  return true;
}


