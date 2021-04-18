// The following code was developed by a joint venture between Desarrollos RM and Digital Strings

#include <SoftwareSerial.h>

// This defines where the HC-06 (Bluetooth Module) is going to be connected.
// In this case in pins 2 and 3
SoftwareSerial hc06(2,3);

// Variables
float sensor_1 = 0;
float sensor_2 = 0;

void setup() {
  
  //Initialize Bluetooth Serial Port
  hc06.begin(9600);
}

void loop() {
  
  while(hc06.available()>0){
    send_diff(sensor_1, sensor_2) //Send both sensor data to the app
  }
  
} 

//Send the pressure value to the app via bluetooth  
void send_diff(a, b) {

    diff = a - b; // Difference between sensor 1 and sensor 2
    hc06.print(diff);
    delay(100);

  }
}
