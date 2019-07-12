// Declaring the required variables for the communication
char text[6];
String pinT;
String voltT;
bool ad = 0;

// Setting the length of the code recived
int len = 6;

// Variables for decoding the recived code
int pinI;
int voltI;

void setup() {
  // Beginning serial communication
  Serial.begin(9600);

  // Setting all the digital pins as output
  for (int i=0; i<=13; i++){
  pinMode(i, OUTPUT);
  }
}

void loop() {

  // Getting the input from user
  Serial.print("enter the code: ");
  Serial.println();
  while (Serial.available() <= len);
  
  // Parsing the entered data
  for(int i=0; i<len; i++){
      text[i] = Serial.read();
    }
  for (int i=0; i<2;i++){
    pinT += text[i];
    }
  for (int i=3; i<=len; i++){
    voltT += text[i];
    }
  ad = (bool)text[2];
  
  // Printing the data for debgging purpose
  Serial.print("code entered ");
  Serial.println(text);
  Serial.print("the pin number selected: ");
  Serial.println(pinT);
  Serial.print("digital or analog (0 is digital, 1 is analog): ");
  Serial.println(ad);
  Serial.print("the voltage to the selected pin: ");
  Serial.println(voltT);

  // Converting string version of volt and pin to int
  pinI = pinT.toInt();
  voltI = voltT.toInt();

  // Output in the pin
  if(ad == 1){
    analogWrite(pinI, voltI);
    }
  else{
    digitalWrite(pinI,voltI);
    }

  // Clearing the variables
  for (int i=0; i<=len; i++){
  text[i] = "";
  }  
  pinT = "";
  voltT = "";
  ad = 0;

  // Setting the minimum delay per cycle
  delay(100);
}
