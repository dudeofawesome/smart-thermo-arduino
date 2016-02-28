#include <math.h>
#include <string.h>

#define RLY0 13
#define RLY1 12
#define RLY2 11
#define RLY3 10

int a;
float temp;
int B=3975;                  //B value of the thermistor
float resistance;

String inputString = "";         // a string to hold incoming data
String command = "";
const int params_length = 10;
String params[params_length];
boolean stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(115200);

  pinMode(RLY0, OUTPUT);
  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);
  pinMode(RLY3, OUTPUT);

  joshIsStupid(0, false);
  joshIsStupid(1, false);
  joshIsStupid(2, false);
  joshIsStupid(3, false);
}

void loop() {
  serialEvent();
  // print the string when a newline arrives:
  if (stringComplete) {

    String res = runCommand(command, params);
//    String res = runCommand(command);
    Serial.println("cmd: " + command + " -> " + res);


    // clear the string:
    inputString = "";
    stringComplete = false;
    command = "";
    for (int i = 0; i < params_length - 1; i++) {
      params[i] = "";
    }
  }

  delay(100);
}

void serialEvent() {
  while (Serial.available() > 0) {
    char inChar = (char) Serial.read();

    if (inChar == '/') {
      command = inputString;
      inputString = "";
    } else if (inChar == ',') {
      int i = 0;
      while (params[i] != "") {
        i++;
      }
      params[i] = inputString;
      inputString = "";
    } else if (inChar == '\n') {
      if (inputString.length() > 0) {
        int i = 0;
        while (params[i] != "") {
          i++;
        }
        params[i] = inputString;
      }
      inputString = "";
      stringComplete = true;
      return;
    } else {
      inputString += inChar;
    }
  }
}

String runCommand (String cmd, String prms[]) {
//String runCommand (String cmd) {
  if (cmd != "") {
    if (cmd == "get-temp") {
      a=analogRead(0);
      resistance = (float) (1023 - a) * 10000 / a; //get the resistance of the sensor;
      temp = 1 / (log(resistance / 10000) / B + 1 / 298.15) - 273.15;//convert to temperature via datasheet ;
      return String(temp);
    } else if (cmd == "cool") {
      joshIsStupid(0, prms[0] == "true");
      return prms[0];
    } else if (cmd == "heat") {
      joshIsStupid(1, prms[0] == "true");
      return prms[0];
    }
    return "";
  } else {
    return "";
  }
}

void joshIsStupid (int relay, boolean on) {
  switch (relay) {
    case 0:
      digitalWrite(RLY0, !on);
      break;
    case 1:
      digitalWrite(RLY1, !on);
      break;
    case 2:
      digitalWrite(RLY2, !on);
      break;
    case 3:
      digitalWrite(RLY3, !on);
      break;
  }
}

