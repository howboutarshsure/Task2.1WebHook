#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include "DHT.h"

char ssid[] = SECRET_SSID;   
char pass[] = SECRET_PASS;   
int keyIndex = 0;            
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#define DHTPIN 2          // What digital pin the DHT22 is connected to
#define DHTTYPE DHT22     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

void setup() {
  Serial.begin(115200);      
  while (!Serial) {
    ; 
  }
  
  dht.begin();              // Initialize the DHT sensor
  ThingSpeak.begin(client); // Initialize ThingSpeak 
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Now we can use the ThingSpeak.setField function to set the fields with the values
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  // You can set other fields with static values or sensor values
  // For example, setField(3, value3); where value3 is another sensor value

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(15000); // Wait 20 seconds to update the channel again
}
