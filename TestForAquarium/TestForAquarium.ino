#include <Wire.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <LTask.h>
#include <LDateTime.h>
#define BAUDRATE 19200

#define WIFI_NAME "SoCLab" //填入WiFi AP網路名稱SSID
#define WIFI_PASSWD "0975510161" //填入密碼
#define WIFI_AUTH LWIFI_WPA  //choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define URL "ec2-52-88-134-169.us-west-2.compute.amazonaws.com" 
LWiFiClient cli; //客戶端
datetimeInfo t;
char buff[256];
const int SLAVE_ADDRESS = 4; 
byte incomingByte = 0;
float ph_data;

//transfer temp byte data
typedef union Data {
  float f_data;
  unsigned char b_data[4];
}Data;
Data temp_data;

void setup(){
  // put your setup code here, to run once:
  Wire.begin();
  LTask.begin();
  LWiFi.begin();
  Serial.begin(BAUDRATE);
  while(!Serial);
  // def. time setting
  t.year = 2015;
  t.mon = 10;
  t.day = 14;
  t.hour = 00;
  t.min = 00;
  t.sec = 00;
  LDateTime.setTime(&t);
  
  Serial.println("Connecting to AP");
  // keep retrying until connected to AP
  while(0 == LWiFi.connect(WIFI_NAME, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWD))){
    Serial.println("delay..");
    delay(1000);
  }
}

//HERE is post data to Amazon ec2
void post(float temp, int model){
  LDateTime.getTime(&t);
  sprintf(buff, "[{\"uuid\":\"A8SPV2MUX7BVXZCP111A\",\"timestamp\":\"%d-%d-%d_%d:%d:%d\",\"sen_mask\":%d,\"temp\":\"%f\"}]", 
          t.year, t.mon, t.day, t.hour, t.min, t.sec, model, temp);
  Serial.println("Connecting to website...");
  String str(buff);
  String data = buff;
  Serial.println(data);
  if (cli.connect(URL, 80)){
      cli.println("POST /debug/rest_api/A8SPV2MUX7BVXZCP111A/post_props HTTP/1.1");
      cli.println("Host: " URL);
      cli.println("Connection: keep-alive"); 
      cli.println("Content-Type: text/plain; charset=UTF-8");
      cli.println("Connection: close");
      cli.println("Content-Type: application/json");
      String thisLength = String(data.length());
      cli.println("Content-Length: " + thisLength);
      cli.println();
      cli.println(data);
      char x = cli.read(); // 讀取
      if(x > 0){
        Serial.print((char) x); // 印出到序列埠
      }
  }
  else {
      Serial.println("Connection failed");
      cli.stop();
  }
}

void loop() {
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

  //ph
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(1);
  Wire.write(9);
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
    ph_data = incomingByte;       
  }
  //*post data to server*
  post(ph_data, 0);

  //here is return code
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
  //end

  //temp 要求slave回傳溫度值
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
    temp_data.b_data[0] = incomingByte;       
  }
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);
    temp_data.b_data[1] = incomingByte;       
  }
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);
    temp_data.b_data[2] = incomingByte;       
  }
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(255);
  Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDRESS, 1);
  while (Wire.available())
  {
    incomingByte = Wire.read();
    Serial.println(incomingByte);
    temp_data.b_data[3] = incomingByte;       
  }
  Serial.println(temp_data.f_data);
  //post data to server
  post(temp_data.f_data, 1);

  //here is return code
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
  //end
 
  //LED
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
  
  //LED
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
  
  //AQ_LED
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
  
  //AQ_LED
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
