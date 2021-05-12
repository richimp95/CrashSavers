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
int RX = 10; //Arduino new RX
int TX = 11; //Arduino new TX

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
float diff_val = 90; // Predefined value for pressure difference wanted 90 mm Hg
float base = 0; // Base value for pressure difference 
int avg_size = 10;

void setup() {
  // Outputs
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);

  //Initialize Bluetooth Serial Port
  //hc06.begin(9600);
  Serial.begin(9600);

  base = init_measure();
  
}

void loop() {
  
    // ----------
  diff_sen = get_diff(diff_val); // Get pressure difference
  Serial.print(diff_sen);
  Serial.println("mmHg");
 
//  while(hc06.available()>0){
//    send_diff(diff_sen); //Send both sensor data to the app
//  }
 
} 

//Send the pressure value to the app via bluetooth  
void send_diff(float a) {
    // Here is a space to modify the result depending on the app developer’s requirements.
    hc06.print(a);
    
}

// Function to read sensor pins and get the pressure difference
float get_diff (float press_diff) {

    float a = sen_1.read(); //get analog read from sensor 1
    float p_a = map(a,8388608, 16777215, 0, 40)*7.5;//map de sensor read to kPa
    //Serial.print("Sensor 1: ");
    //Serial.print(p_a,4);
    //Serial.println("mmHg");
    delay(50); // delay between readings 
    
    float b = sen_2.read(); //get analog read from sensor 2
    float p_b = map(b,8388608, 16777215, 0, 40)*7.5;//map de sensor read to kPa
    //Serial.print("Sensor 2: ");
    //Serial.print(p_b, 4);
    //Serial.println("mmHg");
    delay(50);  // delay between readings   
    
    float diff = p_a - p_b - base; // Difference between sensor 1 and sensor 2

    // If the sensor diff is higher than the pressure we want, LED green is turn ON. Else it turns OFF.
    if (diff >= press_diff){

      digitalWrite(led_g, HIGH);
      digitalWrite(led_r, LOW);
      
      }else{

        digitalWrite(led_g, LOW);
        digitalWrite(led_r, HIGH);
        
      }
    
    return diff;

  }

  float init_measure () {
   
    float avg_val = 0.0; // variable for averaging
    
    while ( map(sen_1.read(),8388608, 16777215, 0, 40)*7.5 < 80.0 )
    
    for (int ii=0;ii<avg_size;ii++){

      float temp_a = sen_1.read(); //get analog read from sensor 1
      float temp_p_a = map(temp_a,8388608, 16777215, 0, 40)*7.5;//map de sensor read to kPa
      delay(50); // delay between readings
      
      float temp_b = sen_2.read(); //get analog read from sensor 2
      float temp_p_b = map(temp_b,8388608, 16777215, 0, 40)*7.5;
      delay(50); // delay between readings
      
      avg_val += temp_p_a - temp_p_b; // add multiple ADC readings

    }

    avg_val /= avg_size;

    return avg_val;
    
    }
