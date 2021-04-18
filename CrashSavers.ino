// The following code was developed by a joint venture between Desarrollos RM and Digital Strings

// References
// For the Pressure sensor: https://makersportal.com/blog/2020/6/4/mps20n0040d-pressure-sensor-calibration-with-arduino
// For the Bluetooth module: https://www.aranacorp.com/en/arduino-and-bluetooth-module-hc-06/

// Libraries
#include <SoftwareSerial.h>
#include <Q2HX711.h> // Search for Queuetue HX711 Library

// Define PINs to be used

// Bluetooth
int RX = 2; //Arduino new RX
int TX = 3; //Arduino new TX

// Sensor pins to use
int out_sen_1 = A0; //Sensor 1
int sdk_sen_1 = 4; //SDK from Sensor 1
int out_sen_2 = A1; //Sensor 2
int sdk_sen_2 = 5; //SDK from Sensor 1

// LED´s
int led_r = 6; //Red LED
int led_g = 7; //Green LED
int led_b = 8; //Blue LED

// This defines where the HC-06 (Bluetooth Module) is going to be connected.
// In this case in pins 2 and 3
SoftwareSerial hc06(RX,TX);

// Variables
float diff_sen = 0; // Sensors pressure difference 
float diff_val = 20; // Predefined value for pressure difference wanted


void setup() {
  // Outputs
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);

  //Initialize Bluetooth Serial Port
  hc06.begin(9600);
}

void loop() {

  diff_sen = get_diff(diff_val); // Get pressure difference
  
  while(hc06.available()>0){
    send_diff(diff_sen); //Send both sensor data to the app
  }
  
} 

//Send the pressure value to the app via bluetooth  
void send_diff(float a) {
  
    // Here is a space to modify the result depending on the app developer’s requirements.
    
    hc06.print(a);
    delay(100);

  }

// Function to read sensor pins and get the pressure difference
float get_diff (float press_diff) {

    // Space to get the sensors values
    
    float a = analogRead(out_sen_1); //get analog read from sensor 1
    float b = analogRead(out_sen_2); //get analog read from sensor 2
    
    // --------------------------------
    
    float diff = a - b; // Difference between sensor 1 and sensor 2

    // If the sensor diff is higher than the pressure we want, LED green is turn ON. Else it turns OFF.
    if (diff >= press_diff){

      digitalWrite(led_g, HIGH);
      digitalWrite(led_r, LOW);
      
      }else{

        digitalWrite(led_g, LOW);
        digitalWrite(led_r, HIGH);
        
        }

    // Here is a space to modify the result depending on the app developer’s requirements.
    
    return diff;

  }
