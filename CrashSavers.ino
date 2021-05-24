

// The following code was developed by a joint venture between Desarrollos RM and Digital Strings
      
// References
// For the Pressure sensor: https://makersportal.com/blog/2020/6/4/mps20n0040d-pressure-sensor-calibration-with-arduino
// For the Bluetooth module: https://www.aranacorp.com/en/arduino-and-bluetooth-module-hc-06/
// Driver https://electrocrea.com/blogs/tutoriales/como-instalar-driver-ch340-para-arduinos-genericos

// Libraries
#include <SoftwareSerial.h>
#include <Q2HX711.h> // Search for Queuetue HX711 Library
//#include <Keyboard.h>
// Define PINs to be used

// Bluetooth
int RX = 10; //Arduino new RX
int TX = 11; //Arduino new TX

// Sensor pins to use
const byte out_sen_1 = 2; //Sensor 1
const byte sdk_sen_1 = 3; //SDK from Sensor 1
const byte out_sen_2 = 4; //Sensor 2
const byte sdk_sen_2 = 5; //SDK from Sensor 1

int relay = 6; // Relay for water motor
int boton = 12; // Push button to reset the system


Q2HX711 sen_1(out_sen_1, sdk_sen_1); 
Q2HX711 sen_2(out_sen_2, sdk_sen_2); 

// LED´s
int led_r = 7; //Red LED
int led_g = 8; //Green LED
int led_b = 9; //Blue LED
int BLUE;

bool bandera = false;
bool init_method = true;

// This defines where the HC-06 (Bluetooth Module) is going to be connected.
// In this case in pins 2 and 3
SoftwareSerial hc05(RX,TX);

// Variables
float diff_sen = 0; // Sensors pressure difference 
float diff_val = 110; // Predefined value for pressure difference wanted 110 mm Hg
float base = 0; // Base value for pressure difference 
int avg_size = 10;

void setup() {
  // Outputs
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);
  pinMode(relay, OUTPUT);

  //Initialize Bluetooth Serial Port
  hc05.begin(9600);
  Serial.begin(9600);
  //Keyboard.begin();
  digitalWrite(relay, HIGH);
  digitalWrite(led_g, LOW);
}

void loop() {
  
   if(init_method == true){
    delay(5000); //Delay to stabilize preassure initial measure 
    base = init_measure();
    init_method = false;
   }
   if (bandera == false){
    
    diff_sen = get_diff(diff_val); // Get pressure difference
    if(diff_sen > diff_val){
         
      digitalWrite(relay, LOW);
      hc05.write("TORNIQUETE LISTO\nEnviando caracter ... \nY\n");
      bandera = true; // If the pressure hits more than 110 mmHg the motor will turn off 
      digitalWrite(led_g, HIGH);
      }else{
        hc05.write("TORNIQUETE EN PROCESO\nPresion: ");
        hc05.print(diff_sen);
        hc05.write(" mmHg\n");
        delay(500);
        digitalWrite(relay, HIGH);
        
        }  
    
   }
 
} 

// Function to read sensor pins and get the pressure difference
float get_diff (float press_diff) {

    float a = sen_1.read(); //get analog read from sensor 1
    float p_a = map(a,8388608, 16777215, 0, 40)*7.5;//map de sensor read to kPa
    delay(50); // delay between readings 
    
    float b = sen_2.read(); //get analog read from sensor 2
    float p_b = map(b,8388608, 16777215, 0, 40)*7.5;//map de sensor read to kPa
    delay(50);  // delay between readings
   
    float diff = p_a - p_b - base; // Difference between sensor 1 and sensor 2

    // If the sensor diff is higher than the pressure we want, LED green is turn ON. Else it turns OFF.
    
    return diff;

  }

  float init_measure () {
   
    float avg_val = 0.0; // variable for averaging
    
    while ( map(sen_1.read(),8388608, 16777215, 0, 40)*7.5 < 80.0 )
    Serial.println("Entro");
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
