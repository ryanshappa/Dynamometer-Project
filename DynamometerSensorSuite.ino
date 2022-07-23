/* DeLinometer Sensor Suite
 *  This program is for connecting to the Delinometer sensor
 *  array through a serial connection with an Arduino Uno.
 *  There are currently 3 functional sensors in the suite
 *  1) Tachometer - Functions by calculating the pulse length of the incoming signal from tachometer module. RPM is the output
 *  2) Thermocouple - Functions by setting up thermocouple object. A funtion call to thermocouple.readCelsius() gets data from Max6675 Thermocouple module through I2C
 *  3) Load Cell - Functions by setting up a scale object. A function call to 
 *      scale.begin(DOUT, CLK); scale.set_scale(); scale.tare(); initiates read sequence, 
 *      scale.set_scale(calibration_factor); scale.get_units()), 4) returns output
 */

// -----------Current Meter Variables -------------
#include "ACS712.h"
#define LoadCurrent A5
ACS712 sensor(ACS712_30A, LoadCurrent);



// -----------Voltage Meter Variables ----------------
#define MotorV A0
float DividerV = 0;
float DividerConst = 40.92;
float DividerVMap = 0;

// -----------Tachometer variables ----------
#define tachSignal 2
int rpm = 0;
float rpmFloat = 0;
float timePerRev = 0;
float timeArray[15] = {0, 0};
float rateOfChangeArray[15] = {0, 0};
int globali = 0;
int globalj = 1;
bool run1 = true;
///////////////////////////////////////////////////////////////////////////


// -----------Thermocouple variables
#include "max6675.h"

int thermoDO = 10;
int thermoCS = 11;
int thermoCLK = 12;
float temperature = 23.5;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
//////////////////////////////////////////////////////////////////////////


// -----------Load Cell variables
#include "HX711.h"

#define DOUT  9
#define CLK  8

HX711 scale;

float calibration_factor = -1721.68;
///////////////////////////////////////////////////////////////////////////////////

//------------- Setup necessary equipment
void setup() {
  Serial.begin(9600);
  pinMode(tachSignal, INPUT);   // Setting tachSignal as Input pin
  //Serial.print(0);
  //Serial.print(""); Serial.println(3500);
  delay(500);                                   // wait for MAX chip to stabilize
  sensor.calibrate();
  temperature = thermocouple.readCelsius();     // take first temperature reading
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  //Serial.println(""); Serial.println("System - Ready");
}

//------------------Main Loop
void loop() {
  scale.set_scale(calibration_factor); 
  timePerRev = pulseIn(tachSignal, HIGH);
  float currentReading;
  currentReading = sensor.getCurrentDC();
  currentReading *= -1;
  dividerReading();
  //Serial.println(timePerRev);
  timeArray[globali] = timePerRev;
  rateOfChangeArray[globali] = timeArray[globali] - timeArray[globalj];
  //Serial.print("timePerRev = ");

  
  // This conditional statement is for catching output during first loop of data stream
  // If the rateOfChangeArray > 20k, the motor is accellerating and data is recorded.
  // After the first run, if the rateOfChangeArray < 55k, the signal is interpreted as valid 
  // Otherwise, the data is considered noise and not output by the system.
  // Values outside of specified range are not possible since the motor would be accellerating/decelerating at rates not possible by motor
  if (run1 == true) {
    if (rateOfChangeArray[globali] > 20000) {
      rpmFloat = (1 / timePerRev) * (1000000) * (60);
      rpm = round(rpmFloat);
      if ((rpm > 0) && (rpm < 3500)) {
        //Serial.print("RPM =, ");
        //Serial.print(temperature); Serial.print(", "); Serial.println(rpm);
        //Serial.println(timePerRev);
        Serial.print(millis()); Serial.print(", "); Serial.print((-1)*(scale.get_units()), 4); Serial.print(", "); Serial.print(thermocouple.readCelsius()); Serial.print(", "); Serial.print(rpm); Serial.print(", "); Serial.print(DividerVMap); Serial.print(", "); Serial.println(currentReading, 3);
        run1 = false;
      }
    }

  }

  else {
    //Serial.println(rateOfChangeArray[globali]);
    if (rateOfChangeArray[globali] < 55000) {
      rpmFloat = (1 / timePerRev) * (1000000) * (60);
      rpm = round(rpmFloat);
      if ((rpm > 0) && (rpm < 3700)) {
        //Serial.print("RPM =, ");
        //Serial.println(temperature); Serial.print(", ");
        Serial.print(millis()); Serial.print(", "); Serial.print((-1)*(scale.get_units()), 4); Serial.print(", "); Serial.print(thermocouple.readCelsius()); Serial.print(", "); Serial.print(rpm); Serial.print(", "); Serial.print(DividerVMap); Serial.print(", "); Serial.println(currentReading, 3);
        //Serial.println(timePerRev);
      }
    }
  }

  delay(200);
}

// --- Voltage Divider function
void dividerReading() {
  for (int i = 0; i < 10; i++) {
    DividerV += analogRead(MotorV);
    delayMicroseconds(10);
  }
  DividerV = DividerV / 10;
  DividerVMap = DividerV / DividerConst;
}
