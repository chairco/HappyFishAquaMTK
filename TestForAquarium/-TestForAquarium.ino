 #include <Wire.h>
const int SLAVE_ADDRESS = 1; 
byte incomingByte = 0;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  //要求slave回傳溫度值
//fog
Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(2);
  Wire.write(3);//device
  Wire.write(1);
  Wire.write(1);//on or off
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
delay(1000);
  //motor
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(2);
  Wire.write(1);
  Wire.write(1);
  Wire.write(1);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
delay(1000);
  
 //temp
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(1);
  Wire.write(6);
  Wire.write(5);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }

  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  
  delay(1000);
 
 //
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(2);//set
  Wire.write(5);//device
  Wire.write(1);
  Wire.write(2);//sec
  Wire.write(0);//r
  Wire.write(0);//g
  Wire.write(0);//b
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  delay(3000);
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(2);
  Wire.write(5);
  Wire.write(1);
  Wire.write(2);
  Wire.write(255);
  Wire.write(255);
  Wire.write(255);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  delay(3000);
  
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(2);
  Wire.write(4);
  Wire.write(1);
  Wire.write(1);
  Wire.write(0);
  Wire.write(0);
  Wire.write(0);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  delay(2000);
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(2);
  Wire.write(4);
  Wire.write(1);
  Wire.write(1);
  Wire.write(255);
  Wire.write(255);
  Wire.write(255);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);       
  }
  delay(2000);
  
}
