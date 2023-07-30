


#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HackPublisher.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_AM2320 am2320 = Adafruit_AM2320();

const char *ssid = "ASUS-F8";
const char *password = "K33pi7$@f3%";
int gasSensorVal;
int trigPin = A9;
int echoPin = A10;
int buzzPin = 12;
float t, h;
HackPublisher publisher("Team13"); 
float pingTravelTime;
float pingTravelDistance;
float distanceToTarget;
void setup() {
  Serial.begin(115200);
  pinMode(buzzPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  while (!Serial) continue;
  // Connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Initialize publisher
  publisher.begin();
  
  while (!Serial) {
    delay(10); // hang out until serial port opens
  }
  // initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial.println("Adafruit AM2320 Basic Test");
  //Initialize temperature and humidity sensor
  am2320.begin();
 
}

void loop() {

//Measure the total time of ultrasonic signal reaching objects and travel back 
digitalWrite(trigPin,LOW);
delayMicroseconds(10);
digitalWrite(trigPin,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin,LOW);
pingTravelTime=pulseIn(echoPin,HIGH);
// Calculate the distance of a detected object with the speed of soud 343 m/s 
pingTravelDistance=pingTravelTime*.0343;
distanceToTarget=pingTravelDistance/2.;

//Read the input of Gas Dectector Sensor
  gasSensorVal = analogRead(A2);
  Serial.println(gasSensorVal, DEC);
/* //Turn on the buzzer if it's at the dangerous level
  if (gasSensorVal > 1200){
     digitalWrite(buzzPin, HIGH);  
     digitalWrite(redPin, LOW);
     digitalWrite(greenPin, LOW);
  }
  //Turn on warning yellow light if it's at the concerning level
 else if(gasSensorVal < 750 && gasSensorVal >= 400){
    digitalWrite(buzzPin, LOW);
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  }
  //Turn on green light if it's at safe level
else {
  digitalWrite(buzzPin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
}*/
// Set specs for texts displayed on OLED
  display.setTextSize(1);
  display.setTextColor(WHITE);
// Show the readings of Gas Detector Sensor on OLED
  display.setCursor(0,16);
  display.print("Gas: ");
  display.println(gasSensorVal, DEC);
 // Show the temperatures and humidities on OLED 
  if (am2320.readTemperatureAndHumidity(&t, &h)) {
  display.setCursor(0,33);
  display.print("Temp: ");
  display.print((int)t);
  display.print("C");
  display.setCursor(65,33);
  display.print("Humi: ");
  display.print((int)h);
  display.println(" %");
  Serial.println(t);
  Serial.println(h);
  } 
  else {
    display.print("** READ ERROR **");
  }
// Show distance of detected objects ahead
   display.setCursor(0,48);
  display.print("Distance: ");
  display.print(distanceToTarget);
  display.println(" cm");
  display.display();
  
  publisher.store("Distance",(int) distanceToTarget); // store value for distance to target
  publisher.store("temp",(int)t);        // store value for temp
  publisher.store("Humidity", (int)h);      // store value for humidity
  publisher.store("AirQuality", gasSensorVal);      // store value for humidity
  publisher.send();                     // send stored data to website
  delay(2000);
  display.clearDisplay();

}
