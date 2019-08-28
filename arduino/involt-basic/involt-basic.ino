/*
  INVOLT BASIC ARDUINO SKETCH
  by Ernest Warzocha 2016
  ------------------------------------------------------
  This file is for serial communication between Arduino 
  Uno and Involt App. It can be used with Bluetooth 2.0
  device connected via hardware serial.
*/

/*
  involtPin array contains values received from app.
  Each UI element refers to pin number which is index of
  this array. involtString is array for values received
  with "S" pin. You can increase the length of array to
  store more values then arduino total pins. Use them 
  in sketch for not only pin-to-pin communication.
*/
int involtPin[14] = {};      //equals involt.pin.P in app
String involtString[2] = {}; //equals involt.pin.S in app

/*
  Buffer for received data. If you plan to receive more 
  chars at once just increase the array length.
*/
char involt[16];

/*
  String for responding to function received from app.
*/
String fname;

/* 
  Definim pinul care va primi informatii legate de 
  intensitatea luminoasa 
*/
#define LIGHT_SENSOR 2
#define OUTSIDE_DARK 1
#define OUTSIDE_LIGHT 0
/* 
  Definim pinii de control pentru cele 4 relee care
  controleaza cate un bec cu incandescenta fiecare.
*/
#define LIGHT1 7
#define LIGHT2 6
#define LIGHT3 5
#define LIGHT4 4
#define LIGHT_OFF 0
#define LIGHT_ON 1

void setup()
{
  /*
  Connection speed must be same as app.
  Should remain unchanged.
 */
  Serial.begin(57600);
  /*
    In functie de ce dorim sa facem trebuie sa setam pinii cu care 
    vom lucra in mod INPUT sau OUTPUT.
  */
  pinMode(LIGHT_SENSOR, INPUT_PULLUP);
  pinMode(LIGHT1, OUTPUT);
  pinMode(LIGHT2, OUTPUT);
  pinMode(LIGHT3, OUTPUT);
  pinMode(LIGHT4, OUTPUT);
}

void loop()
{
  involtReceive();
  delay(10);

  /* 
    Cand butoanele HTML sunt activate vom activa si luminile corespunzatoare.
  */
  digitalWrite(LIGHT1, involtPin[7]);
  digitalWrite(LIGHT2, involtPin[6]);
  digitalWrite(LIGHT3, involtPin[5]);
  digitalWrite(LIGHT4, involtPin[4]);

  /*
    Citim de la pinul digital corespunzator senzorului de lumina si trimitem 
    informatia catre pinul virtual cu acelasi numar.
  */
  int LS = digitalRead(LIGHT_SENSOR);
  involtSend(LIGHT_SENSOR, LS);

  //Clear the function to trigger it only once.
  fname = "";
}

/*
  INVOLT FUNCTIONS
  ------------------------------------------------------

  involtReceive
  ------------------------------------------------------ 
  read the data from app and parse the values received 
  into proper array. The read until is used due to
  issues with missing chars when reading direct strings.
  
  involtSend, involtSendString
  ------------------------------------------------------
  send int or string to app. Multiple prints are to
  reduce the sketch size (compared to sprintf()).

  involtSendFunction
  ------------------------------------------------------
  send function name to trigger it in app.
*/

void involtReceive()
{
  if (Serial.available() > 0)
  {
    Serial.readBytesUntil('\n', involt, sizeof(involt));
    int pin;
    if (involt[0] == 'P')
    {
      int value;
      sscanf(involt, "P%dV%d", &pin, &value);
      involtPin[pin] = value;
    }
    else if (involt[0] == 'S')
    {
      char value[sizeof(involt)];
      sscanf(involt, "S%dV%s", &pin, &value);
      involtString[pin] = value;
    }
    else if (involt[0] == 'F')
    {
      char value[sizeof(involt)];
      sscanf(involt, "F%s", &value);
      fname = value;
    };
    memset(involt, 0, sizeof(involt));
  };
};

void involtSend(int pinNumber, int sendValue)
{
  Serial.print('A');
  Serial.print(pinNumber);
  Serial.print('V');
  Serial.print(sendValue);
  Serial.println('E');
  Serial.flush();
};

void involtSendString(int pinNumber, String sendString)
{
  Serial.print('A');
  Serial.print(pinNumber);
  Serial.print('V');
  Serial.print(sendString);
  Serial.println('E');
  Serial.flush();
};

void involtSendFunction(String functionName)
{
  Serial.print('F');
  Serial.print(functionName);
  Serial.println('E');
  Serial.flush();
};
