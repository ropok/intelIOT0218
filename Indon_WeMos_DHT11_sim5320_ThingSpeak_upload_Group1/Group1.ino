void silo()
{
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


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity() + hcal;
  // Read temperature as Celsius (the default)
  t = dht.readTemperature() + tcal;
//  HCSR04(); // Mendapatkan data Isi(%) dari Silo
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
 Serial.println("Posting results to ThinkSpeak");
 // Update the 2 ThingSpeak fields with the new data
 ThingSpeak.setField(1, (float)t);
 ThingSpeak.setField(2, (float)h);
//
 int result = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
 if (result = 200) {
   Serial.println("Results Posted, closing connection");
 }
 else
 {
   Serial.print("Unable to post result, closing connection : ");
   Serial.println(result);
 }
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
