

// The following code was developed by a joint venture between Desarrollos RM and Digital Strings
      
// References
// For the Pressure sensor: https://makersportal.com/blog/2020/6/4/mps20n0040d-pressure-sensor-calibration-with-arduino
// For the ESP32 Wifi+Bluetooth module installing: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
// Download the BLE Keyboard Library "ESP32-BLE-Keyboard.zip" from: https://github.com/T-vK/ESP32-BLE-Keyboard/releases


// Libraries
#include <BleKeyboard.h>
#include <Q2HX711.h>
BleKeyboard bleKeyboard("CrashSavers Simulator", "Desarrollos RM y Digital Strings");

// Sensor pins to use
const byte out_sen_1 = 25; //Sensor 1
const byte sdk_sen_1 = 26; //SDK from Sensor 1
const byte out_sen_2 = 32; //Sensor 2
const byte sdk_sen_2 = 33; //SDK from Sensor 1

const int relay = 27; // Relay for water motor

Q2HX711 sen_1(out_sen_1, sdk_sen_1); 
Q2HX711 sen_2(out_sen_2, sdk_sen_2); 

// LED´s
const int led_g = 13; //Green LED 
const int led_y = 14; //Yellow LED

bool bandera = false;
bool init_method = true;

// Variables
float sensibilidad = 12;
float diff_sen = 0; // Sensors pressure difference 
float diff_val = 100; // Predefined value for pressure difference wanted 110 mm Hg
float base = 0; // Base value for pressure difference 
int avg_size = 10;

void setup() {
  // Outputs
  pinMode(led_g, OUTPUT);
  pinMode(led_y, OUTPUT);
  pinMode(relay, OUTPUT);
  
  digitalWrite(relay, HIGH);
  digitalWrite(led_g, LOW);
  digitalWrite(led_y, LOW);
  
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
    
  if(bleKeyboard.isConnected()) {
   
   if(init_method == true){
      delay(5000); //Delay to stabilize preassure initial measure 
      base = 0;//init_measure();
      init_method = false;
      bleKeyboard.write(KEY_RETURN);
   }
    
   if (bandera == false){
    diff_sen = get_diff(diff_val, base); // Get pressure difference
    if(diff_sen > diff_val){
         
      digitalWrite(relay, LOW);
      bleKeyboard.print(" ");
      bleKeyboard.print(diff_sen);
      bandera = true; // If the pressure hits more than 110 mmHg the motor will turn off 
      digitalWrite(led_g, HIGH);
      digitalWrite(led_y, LOW);
      }else{
        //bleKeyboard.print(diff_sen);
        Serial.println(diff_sen);
        digitalWrite(relay, HIGH);
        digitalWrite(led_g, LOW);
        digitalWrite(led_y, HIGH);
        } 
   
    
  }
}

}

// Function to read sensor pins and get the pressure difference
float get_diff (float press_diff, float base_press) {

    float avg_val_a = 0.0;
    float avg_val_b = 0.0;
    for (int ii=0;ii<avg_size;ii++){
  
        float temp_a = sen_1.read(); //get analog read from sensor 1
        float temp_p_a = map(temp_a,4000000, 16777215, 0, 40)*sensibilidad;//map de sensor read to kPa
        delay(50); // delay between readings
        
        float temp_b = sen_2.read(); //get analog read from sensor 2
        float temp_p_b = map(temp_b,4000000, 16777215, 0, 40)*sensibilidad;
        delay(50); // delay between readings
        avg_val_a += temp_p_a;
        avg_val_b += temp_p_b;
     }
  
    avg_val_a /= avg_size;
    avg_val_b /= avg_size;

    float diff = avg_val_a - avg_val_b - base_press; // Difference between sensor 1 and sensor 2
   // If the sensor diff is higher than the pressure we want, LED green is turn ON. Else it turns OFF.
    
    return diff;

  }
float init_measure () {
   
    float avg_val = 0.0; // variable for averaging
    
    while ( map(sen_1.read(),4000000, 16777215, 0, 40)*10 < 80.0 ){
      //Serial.println(map(sen_1.read(),8388608, 16777215, 0, 40)*7.5);
      }
    Serial.println("Entro");
    for (int ii=0;ii<avg_size;ii++){

      float temp_a = sen_1.read(); //get analog read from sensor 1
      float temp_p_a = map(temp_a,4000000, 16777215, 0, 40)*sensibilidad;//map de sensor read to kPa
      delay(10); // delay between readings
      
      float temp_b = sen_2.read(); //get analog read from sensor 2
      float temp_p_b = map(temp_b,4000000, 16777215, 0, 40)*sensibilidad;
      delay(10); // delay between readings
      
      avg_val += temp_p_a - temp_p_b; // add multiple ADC readings

    }

    avg_val /= avg_size;

    return avg_val;
    
    }
