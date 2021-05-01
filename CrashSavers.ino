// The following code was developed by a joint venture between Desarrollos RM and Digital Strings

// References
// For the Pressure sensor: https://makersportal.com/blog/2020/6/4/mps20n0040d-pressure-sensor-calibration-with-arduino
// For the Bluetooth module: https://www.aranacorp.com/en/arduino-and-bluetooth-module-hc-06/
// Driver https://electrocrea.com/blogs/tutoriales/como-instalar-driver-ch340-para-arduinos-genericos

// Libraries
#include <SoftwareSerial.h>
#include <Q2HX711.h> // Search for Queuetue HX711 Library

// Define PINs to be used

// Bluetooth
int RX = 2; //Arduino new RX
int TX = 1; //Arduino new TX

// Sensor pins to use
const byte out_sen_1 = 2; //Sensor 1
const byte sdk_sen_1 = 3; //SDK from Sensor 1
const byte out_sen_2 = 4; //Sensor 2
const byte sdk_sen_2 = 5; //SDK from Sensor 1

Q2HX711 sen_1(out_sen_1, sdk_sen_1); 
Q2HX711 sen_2(out_sen_2, sdk_sen_2); 

// LED´s
int led_r = 7; //Red LED
int led_g = 8; //Green LED
int led_b = 9; //Blue LED

// This defines where the HC-06 (Bluetooth Module) is going to be connected.
// In this case in pins 2 and 3
SoftwareSerial hc06(RX,TX);

// Variables
float diff_sen = 0; // Sensors pressure difference 
float diff_val = 13332; // Predefined value for pressure difference wanted 100 mm Hg


void setup() {
  // Outputs
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);

  //Initialize Bluetooth Serial Port
  //hc06.begin(9600);
  Serial.begin(9600);
}

void loop() {
  
    float a = sen_1.read(); //get analog read from sensor 1
    //float vr_a = (a*5.0)/(128.0*pow(2,24.0)-1.0);
    float p_a = map(a,8388608, 16777215, 0, 40);//(29.5/50)*(vr_a-22.6);
    Serial.print("Sensor 1: ");
    Serial.print(p_a,4);
    Serial.println("kPa");
    delay(1000);
    float b = sen_2.read(); //get analog read from sensor 2
    //float vr_b = (b*5.0)/(128.0*pow(2,24.0)-1.0);
    float p_b = map(b,8388608, 16777215, 0, 40);//(29.5/50)*(vr_b-22.6);
    Serial.print("Sensor 2: ");
    Serial.print(p_b, 4);
    Serial.println("kPa");
    delay(1000);    
    // ----------
//  diff_sen = get_diff(diff_val); // Get pressure difference
//  
//  while(hc06.available()>0){
//    send_diff(diff_sen); //Send both sensor data to the app
//  }
//  
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
    
    float a = sen_1.read(); //get analog read from sensor 1
    Serial.println(a);
    delay(50);
    float b = sen_2.read(); //get analog read from sensor 2
    Serial.println(b);
    delay(50);    
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
