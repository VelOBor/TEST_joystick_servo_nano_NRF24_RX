//include all the necessary libraries
#include <Arduino.h> //arduino library when using VSCode PlatformIO, comment out or delete if using Arduino IDE
#include <SPI.h> //SPI library for Arduino
#include <RF24.h> //RF24 library for Arduino
#include <Servo.h> //servo library for Arduino

//code adapted from https://mkinventions.blogspot.com/2019/12/nrf24l01-part-3-controling-NRFreadvals.html

//==============PINOUT==============
/*
---NRFreadvals and LED---
D8 - x-axis, can be any PWM pin to output servo angle
D9 - y-axis, can be any PWM pin to output servo angle
D2 - LED for output of button press

---don't forget the VCC and Gnd for the NRFreadvals, 
should be a separate power supply, with Gnd connected to arduino---

---NRF24---
D11(MOSI) - MOSI on NRF24 module, ---CANNOT USE D11 pin for anything else now!---
D12(MISO) - MISO on NRF24 module, ---CANNOT USE D12 pin for anything else now!---
D13(SCK) - SCK on NRF24 module, ---CANNOT USE D13 pin for anything else now!---
D7 - CE on NRF24 module, can be any analog or digital pin
D10 - CS on NRF24 module, can be any digital pin
---don't forget the the VCC and Gnd for the NRF24 module, MUST BE 3.3V!!! 
Can use the arduino 3V3 output. Recommend a 10uF cap  across the module VCC and GND---
*/

//create servo objects
Servo xservo;
Servo yservo;


RF24 RXmodule(7, 10); // CE, CS pins where the NRF24 module is connected
 
const byte address[6] = {"42"}; //address for the module, AFAIK both TX and RX modules should have same value here

const int xservopin = 8; //signal output pin (must be PWM capable) for the x servo
const int yservopin = 9; //signal output pin (must be PWM capable) for the y servo
const int ledoutpin = 2; //LED output pin
int ledval = 0; //variable to hold the state of led

int NRFreadvals = 90; //variable to hold the read values from the NRF
 
void setup() 
{
// put your setup code here, to run once:
Serial.begin(115200); //set to whatever speed you prefer, for serial debugging
xservo.attach(xservopin); //"connect" the x servo
yservo.attach(yservopin); //y servo
pinMode(ledoutpin, OUTPUT);
 
RXmodule.begin(); //initialize the RX module
RXmodule.openReadingPipe(0, address); //open communication pipe
RXmodule.setPALevel(RF24_PA_MIN); //set power of NRF module, can be changed to RF24_PA_MAX for better range, BUT MUST BE SAME ON THE RX MODULE!!!
RXmodule.startListening(); //puts the NRF module into RECEIVE MODE
}

void loop() 
{
  // put your main code here, to run repeatedly:
delay(10); //stability pause
  while (RXmodule.available()) { //control loop to keep reading NRF module as long as it's available
 
 int NRFreadvals[3]; //initialize array of values read from the NRF module
 
      RXmodule.read(&NRFreadvals, sizeof(NRFreadvals)); //reads the values from the NRF module
      xservo.write(NRFreadvals[0]); //sends the first variable from the array to the servo
      Serial.print("Xserval: "); //serial debugging
      Serial.print(NRFreadvals[0]); //serial debugging
      yservo.write(NRFreadvals[1]); //sends the second variable from the array to the servo
      Serial.print(" Yserval: "); //serial debugging
      Serial.print(NRFreadvals[1]); //serial debugging
      ledval = (NRFreadvals[2]); //sends the third variable from the array to "ledval"
      digitalWrite(ledoutpin, ledval); //turns on the LED
      Serial.print(" butval: "); //serial debugging
      Serial.println(NRFreadvals[2]); //serial debugging
  }
}
