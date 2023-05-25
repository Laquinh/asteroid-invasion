int clockPin = 4;
int latchPin = 3;
int dataPin = 2;

int numbers[10] = //values necessary for the 7-segment display to show the correct number
{
  0b00111111, //0
  0b00000110, //1
  0b01011011, //2
  0b01001111, //3
  0b01100110, //4
  0b01101101, //5
  0b01111101, //6
  0b00100111, //7
  0b01111111, //8
  0b01100111  //9
};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() 
{
  char incomingData[2];
  
  incomingData[0] = Serial.read();
  if(incomingData[0] == 'C') //the first byte of data being 'C' means this information was sent from C++
  {
    incomingData[1] = Serial.read(); //reads the number of lives the player has left

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, numbers[incomingData[1]]);
    digitalWrite(latchPin, HIGH);
  }
  
  char data[5];
  data[0] = 'A';
  
  int moveX = analogRead(A0);
  int moveY = analogRead(A1);
  data[1] = map(moveX, 0, 1023, 0, 255);
  data[2] = map(moveY, 0, 1023, 0, 255);
  
  int shootX = analogRead(A2);
  int shootY = analogRead(A3);
  data[3] = map(shootX, 0, 1023, 0, 255);
  data[4] = map(shootY, 0, 1023, 0, 255);
  Serial.write(data, 5); //reads all the values from analog input pins, and sends them to the Serial buffer.
  
  delay(50);
}
