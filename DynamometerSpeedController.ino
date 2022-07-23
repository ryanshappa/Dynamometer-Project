//Tom DeLine Motor Speed Controller for the DeLineDynamometer
//This is a motor speed controller to drive a 24V brushed DC Motor.


//--Global Variables used throughout the program
int PWMSignal = 3;          //Assigning the name PWMSignal to pin 3 of the Arduino
int tachSignal = 4;         //Assigning the name tachSignal to pin 2 of the Arduino
unsigned long RPM = 0;
unsigned long timePerRev = 0;
int POT = A0;               //Assigning the name POT to pin A0
int PWMSpeedInt = 0;        //Integer for the value given to the PWM rate (0-255)
int OperatorSelectInt;      //Integer to hold the value of the machine operator's input from menu selection
String OperatorSelectStr, PWMSpeedStr; //Strings for data input from serial console


///--------- Arduino Functions required (Setup & Loop)----------------
void setup() {
  pinMode(PWMSignal, OUTPUT); //Declaring PWMSignal pin as output
  pinMode(tachSignal, INPUT);       //Declaring tachSignal pin as input
  Serial.begin(9600);        //Setting the serial console operating speed
}

void loop() {
  PWMSpeedInt = 0;            //Makes sure that PWM signal is set to zero after each run
  Serial.println("\n");
  Serial.println("DeLine-Dynamometer Console, \nPlease select an Operating Mode\n");
  Mode();                     //Takes operator into menu selection tree
  PWMSpeedInt = 0;

}
///---------END Arduino Functions required (Setup & Loop)----------------


//------------------Dynamometer Operation Modes--------------------------
//1) Dynamometer Mode
//2) Debug Mode
void Mode() {
  Serial.println("1) Dyna      2) Debug");
  while (Serial.available() == 0) {};             //holds program at this line until operator input is entered
  OperatorSelectStr = Serial.readString();        //assigns serial string to variable
  OperatorSelectInt = OperatorSelectStr.toInt();  //converts string to integer for use in switch-case statement
  Serial.print("You Selected: ");
  Serial.println(OperatorSelectInt);
  switch (OperatorSelectInt) {                    //if OperatorSelectInt is 1, 2, default - go to specified menu item
    case 1 :
      Profile();                                  //enters Dynamometer mode an begins next tree menu item at pofile()
      break;
    case 2 :
      Debug();                                    //not used a the moment
      break;
    default :                                     //if none of the above is the case, default catches all other entries and program menu tree is restarted
      Serial.println("\nNot a valid Entry\n\n");
      break;
  }
}


//--------------Modes-------------------//
//  Two Modes
//    1) Profile Mode
//    2) Debug Mode
void Profile() {
  Serial.print("\nPlease select a Profile\n");
  Serial.println("1) Sweep      2) Max Speed    3) Speed Select   4) Potentiometer Mode");
  while (Serial.available() == 0) {};             //holds program at this line until operator input is entered
  OperatorSelectStr = Serial.readString();        //assigns serial string to variable
  OperatorSelectInt = OperatorSelectStr.toInt();  //converts string to integer for use in switch-case statement
  Serial.print("You Selected: ");
  Serial.println(OperatorSelectInt);
  Serial.print("\n");

  switch (OperatorSelectInt) {                  //if OperatorSelectInt is 1, 2, 3, 4, default - go to specified menu item
    case 1:
      Sweep();                                  //enters Sweep Menu and begins next tree
      break;
    case 2:
      MaxSpeed();                               //enters MaxSpeed Menu and begins next tree
      break;
    case 3:
      SpeedSelect();                            //enters Speed Select Menu and begins next tree
      break;
    case 4:
      POTMode();                                //enters potentiometer speed menu and begins next tree
      break;
    default :                                   //if none of the above is the case, default catches all other entries and program menu tree is restarted
      Serial.println("\nNot a valid Entry\nStarting Over\n");
      break;
  }
}
// Mode - Debug

void Debug() {
  Serial.println("\nIn Debug function\n");
  POTMode();
}
//----------------End Modes ------------------//


//----------------Dynamometer Speed Profiles--------------------------
// 1)Sweep
// 2)Max Speed
// 3)Speed Select
// 4)Potentiometer Operation
//----------------Profiles -------------------//
void Sweep() {
  Serial.println("Select an Option");
  Serial.println("1) Begin Test");
  while (Serial.available() == 0) {               //holds program at this line until operator input is entered
  };
  OperatorSelectStr = Serial.readString();        //assigns serial string to variable
  OperatorSelectInt = OperatorSelectStr.toInt();  //converts string to integer for use in switch-case statement

  PWMSpeedInt = 0;
  switch (OperatorSelectInt) {                    //if OperatorSelectInt is 1, default - go to specified menu item
    case 1 :                                      //Operator selected 1 to begin test

      //checking difference between the two
      //Loop for speeding motor up to max speed
      //loop adds 15 to PWMSPeedInt (0+15=15)
      //then checks if its value is less than 255 (max PWM value)
      //if it is <= PWMSpeedInt, increment the PWMSpeedInt by 1 and enter loop
      //repeat until loop PWMSPeedInt > 255
      for (PWMSpeedInt + 50; PWMSpeedInt <= 255; PWMSpeedInt+=10) {
        analogWrite(PWMSignal, PWMSpeedInt);
        delay(5000);
      }

      //Loop for slowing the motor down.
      //loop starts at PWMSpeedInt value
      //checks if the value is still greater than zero
      //if true, subtract 1 from PWMSpeedInt and enter the loop
      for (PWMSpeedInt; PWMSpeedInt >= 0; PWMSpeedInt--) {
        analogWrite(PWMSignal, PWMSpeedInt);
        delay(25);
      }
      PWMSpeedInt = 0;
      break;
    default :                                     //if none of the above is the case, default catches all other entries and program menu tree is restarted
      Serial.println("\nNot a valid Entry\nStarting Over\n");
      break;
  }
}

void MaxSpeed() {
  Serial.println("Select an Option");
  Serial.println("1) Begin Test");
  while (Serial.available() == 0) {               //holds program at this line until operator input is entered
  };
  OperatorSelectStr = Serial.readString();        //assigns serial string to variable
  OperatorSelectInt = OperatorSelectStr.toInt();  //converts string to integer for use in switch-case statement

  PWMSpeedInt = 0;
  switch (OperatorSelectInt) {                    //if OperatorSelectInt is 1, default - go to specified menu item
    case 1 :                                      //Operator selected 1 to begin test

      //checking difference between the two
      //Loop for speeding motor up to max speed
      //loop adds 15 to PWMSPeedInt (0+15=15)
      //then checks if its value is less than 255 (max PWM value)
      //if it is <= PWMSpeedInt, increment the PWMSpeedInt by 1 and enter loop
      //repeat until loop PWMSPeedInt > 255
      for (PWMSpeedInt + 30; PWMSpeedInt <= 255; PWMSpeedInt++) {
        analogWrite(PWMSignal, PWMSpeedInt);
        delay(50);
      }

      int time1 = millis();                       //starting a timer to run program for specific amount of time
      int time2 = millis();                       //second timer element
      int timeDiff = time2 - time1;
      while (timeDiff < 30000) {                  //while the amount of time that has passed is less than 20 seconds, set time 2 to new millis() value and compare the difference
        time2 = millis();
        timeDiff = time2 - time1;
      };

      //Loop for slowing the motor down.
      //loop starts at PWMSpeedInt value
      //checks if the value is still greater than zero
      //if true, subtract 1 from PWMSpeedInt and enter the loop
      for (PWMSpeedInt; PWMSpeedInt >= 0; PWMSpeedInt--) {
        analogWrite(PWMSignal, PWMSpeedInt);
        delay(25);
      }
      break;
    default :                                     //if none of the above is the case, default catches all other entries and program menu tree is restarted
      Serial.println("\nNot a valid Entry\nStarting Over\n");
      break;
  }
}

void SpeedSelect() {
  PWMSpeedInt = 0;
  Serial.println("Enter Speed from 1 - 255");
  while (Serial.available() == 0) {            //holds program at this line until operator input is entered
  };
  PWMSpeedStr = Serial.readString();          //assigns serial string to variable
  PWMSpeedInt = PWMSpeedStr.toInt();          //converts string to integer for use in switch-case statement

  if ((PWMSpeedInt > 255) || (PWMSpeedInt <= 0)) {
    Serial.println("Not a valid entry\nStarting Over\n");
  }
  else if ((PWMSpeedInt <= 255) || (PWMSpeedInt > 0)) {
    Serial.print("\nYou entered: ");
    Serial.println(PWMSpeedInt);
    Serial.println("Please press '1' to start");
    while (Serial.available() == 0) {};               //holds program at this line until operator input is entered
    OperatorSelectStr = Serial.readString();          //assigns serial string to variable
    OperatorSelectInt = OperatorSelectStr.toInt();    //converts string to integer for use in switch-case statement

    switch (OperatorSelectInt) {                      //if OperatorSelectInt is 1, default - go to specified menu item
      case 1 :

        //Loop for speeding motor up
        for (int i = 0; i <= PWMSpeedInt; i++) {
          analogWrite(PWMSignal, i);
          delay(50);
        }

        int time1 = millis();                         //starting a timer to run program for specific amount of time
        int time2 = millis();                         //second timer element
        int timeDiff = time2 - time1;                 //checking difference between the two
        while (timeDiff < 20000) {                    //while the amount of time that has passed is less than 20 seconds, set time 2 to new millis() value and compare the difference
          time2 = millis();
          timeDiff = time2 - time1;
        };

        //Loop for slowing the motor down.
        //loop starts at PWMSpeedInt value
        //checks if the value is still greater than zero
        //if true, subtract 1 from PWMSpeedInt and enter the loop
        for (PWMSpeedInt; PWMSpeedInt >= 0; PWMSpeedInt--) {
          analogWrite(PWMSignal, PWMSpeedInt);
          delay(25);
        }
        break;

      default : Serial.println("Not a valid Entry\n Starting over\n");  //if none of the above is the case, default catches all other entries and program menu tree is restarted
        break;
    }
  }

  else  {
    Serial.println("Not a valid Entry\n Starting over\n");
    PWMSpeedInt = 0;
  }
}

void POTMode() {
  int POTValue = analogRead(POT);
  Serial.println("\n1) to begin POT Mode");
  while (Serial.available() == 0) {};              //holds program at this line until operator input is entered
  OperatorSelectStr = Serial.readString();         //assigns serial string to variable
  OperatorSelectInt = OperatorSelectStr.toInt();   //converts string to integer for use in switch-case statement
  while (POTValue >= 200) {
    Serial.println("Reset POT first");
    POTValue = analogRead(POT);
  };
  int time1 = millis();
  int time2 = millis();
  int timeDiff = time2 - time1;
  // Serial.available() == 0;
  ESAATSPotMode();

}

//----------------End Profiles --------------------//


//--------Emergency Stop and Automatic Timeout service -----------------//
void ESAATSPotMode() {
  int time1 = millis();
  int time2 = millis();
  int timeDiff = time2 - time1;
  // Serial.available() == 0;
  while (timeDiff < 300000) {
    if (Serial.available() > 0) {
      timeDiff = 3000000;
    }
    else
    {
      PWMSpeedInt = analogRead(POT);
      PWMSpeedInt = map(PWMSpeedInt, 0, 1023, 0, 255);
      analogWrite(PWMSignal, PWMSpeedInt);
      time2 = millis();
      timeDiff = time2 - time1;
    }

  }
  for (PWMSpeedInt; PWMSpeedInt > 0; PWMSpeedInt--) {
    analogWrite(PWMSignal, PWMSpeedInt);
    delay(25);
  }
}
