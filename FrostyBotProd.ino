#include <OneWire.h>
#include <MKRGSM.h>
#include "DallasTemperature.h"
#include <RTCZero.h>
#include "ArduinoLowPower.h"

const char PINNUMBER[] = " ";
// APN data
const char GPRS_APN[] = "hologram";
const char GPRS_LOGIN[] = " ";
const char GPRS_PASSWORD[] = " ";
String HOLOGRAM_DEVICE_KEY = "your device key";
String HOLOGRAM_MESSAGE;
String HOLOGRAM_TOPIC = "TEMPS";
OneWire oneWire(7);

DallasTemperature sensors(&oneWire);
// Addresses of 6 DS18B20s
uint8_t sensor1[8] = { 0x28, 0xAA, 0xBD, 0x06, 0x55, 0x14, 0x01, 0xE0 };
uint8_t sensor2[8] = { 0x28, 0xAA, 0xDA, 0x16, 0x55, 0x14, 0x01, 0x98 };
uint8_t sensor3[8] = { 0x28, 0xAA, 0xD6, 0xE5, 0x54, 0x14, 0x01, 0xEB };
uint8_t sensor4[8] = { 0x28, 0xAA, 0x82, 0x0D, 0x55, 0x14, 0x01, 0x9F };
uint8_t sensor5[8] = { 0x28, 0xAA, 0x21, 0x17, 0x55, 0x14, 0x01, 0xC1 };
uint8_t sensor6[8] = { 0x28, 0xAA, 0x6D, 0xED, 0x54, 0x14, 0x01, 0xC8 };
// initialize the library instance
GSMClient client;
GPRS gprs;
GSM gsmAccess;

// Hologram's Embedded API (https://hologram.io/docs/reference/cloud/embedded/) URL and port
char server[] = "cloudsocket.hologram.io";
int port = 9999;
String returnTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  float tempF = round(DallasTemperature::toFahrenheit(tempC)*10) / 10.0;
  return(String(tempF));
}
void setup() {
  // put your setup code here, to run once:

 // Serial.begin(9600);
}
  
void loop() {
    sensors.begin();
  //while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
//  }
  delay(10000);
  sensors.requestTemperatures();
  delay(1000);
  int sensorValue = analogRead(ADC_BATTERY);
  HOLOGRAM_MESSAGE = returnTemperature(sensor1) + "," +returnTemperature(sensor2) + "," +returnTemperature(sensor3) + "," +returnTemperature(sensor4)+","+returnTemperature(sensor5)+","+returnTemperature(sensor6)+","+String(sensorValue);
 //- HOLOGRAM_MESSAGE = String(sensorValue);
  boolean connected = false;

  // After starting the modem with GSM.begin()
  // attach to the GPRS network with the APN, login and password
  while (!connected) {
  //   Serial.println("Begin gsm Access");
    //Serial.println(gsmAccess.begin()); //Uncomment for testing
    
    if ((gsmAccess.begin() == GSM_READY) &&
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      connected = true;
  //    Serial.println("GSM Access Success");
      
    } 
    else {
 //     Serial.println("Not connected");
      delay(1000);
    }
  }
//  Serial.println("connecting...");
   // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
 //   Serial.println("connected");
    // Send a Message request:
    client.println("{\"k\":\"" + HOLOGRAM_DEVICE_KEY +"\",\"d\":\""+ HOLOGRAM_MESSAGE+ "\",\"t\":\""+HOLOGRAM_TOPIC+"\"}");
    delay(10000);
    //client.disconnect();
    gsmAccess.shutdown();  
    LowPower.deepSleep(21600000);
  //delay(21600000);
  //delay(10000);
 //   Serial.println(HOLOGRAM_MESSAGE);
  } else {
    // if you didn't get a connection to the server:
 //   Serial.println("connection failed");
  }

}



/*void returnTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  float tempF = round(DallasTemperature::toFahrenheit(tempC)*10) / 10.0;
  return(tempF)
}
*/
