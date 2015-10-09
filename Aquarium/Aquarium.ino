#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Timer.h>
//設定控制元件所需的指令

#define DOWNMOTOR 0
#define UPMOTOR 1
#define HEATER 2
#define FOG 3
#define AQ_LED_RGB 4
#define FILTER_LED_RGB 5
#define TEMPERATURE 6
#define AQ_LED_CHANGE_SLOW 7
#define FILTER_LED_CHANGE_SLOW 8
#define TEMP_QU 5
#define LED_CHANGE_STAGE 100.0

#define GET 3
#define SET 1
#define CMD_SET 2
#define CMD_GET 1
#define CMD_END 255

#define CMD_ERROR 100
#define ID_ERROR 101
#define QU_ERROR  102
#define DATA_ERROR 103
#define END_ERROR 104
#define SET_SUCCESS 0
#define GET_SUCCESS 128

//設定IO PORT
const int SLAVE_ADDRESS = 4;
const int DS1820 = 2;
const int downmotor = 4;
const int upmotor = 7;
const int heater = 13;
const int fog = 8;
const int aquarium_led_r = 3;
const int aquarium_led_g = 5;
const int aquarium_led_b = 6;
const int filter_led_r = 9;
const int filter_led_g = 10;
const int filter_led_b = 11;
const int water_h = A0;
const int water_l = A1;

float aq_led_r_duty = 12;
float aq_led_g_duty = 12;
float aq_led_b_duty = 12;
float aq_led_r_old_duty = 12;
float aq_led_g_old_duty = 12;
float aq_led_b_old_duty = 12;
bool aq_led_change_slow = false;
float aq_change_time = 0;

float filter_led_r_duty = 0;
float filter_led_g_duty = 0;
float filter_led_b_duty = 0;
float filter_led_r_old_duty = 0;
float filter_led_g_old_duty = 0;
float filter_led_b_old_duty = 0;
bool filter_led_change_slow = false;
float filter_change_time = 0;

int IO_state = 0;
int up_motor_state=0;
int return_state = CMD_END;
int temp_count = 0;

byte requestRegister;
byte ID;
byte Quantity;
byte cmd_end;
byte Temp;

Timer led_timer;
Timer temp_timer;
Timer water_level;
float filter_led_r_change_rate = 0;
float filter_led_g_change_rate = 0;
float filter_led_b_change_rate = 0;
float aq_led_r_change_rate = 0;
float aq_led_g_change_rate = 0;
float aq_led_b_change_rate = 0;

typedef union Data
{
  float temperature;
  byte byte_temp[4];
}Data;

Data TEMP;
OneWire oneWire(DS1820);
DallasTemperature Temperature(&oneWire);

void setup()
{
  //建立SLAVE ADDRESS
  Wire.begin(SLAVE_ADDRESS);
  //建立I2C接收事件
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  //指定PIN3為輸出腳
  pinMode(downmotor, OUTPUT);
  //digitalWrite(downmotor, 0);
  pinMode(upmotor, OUTPUT);
  //digitalWrite(upmotor, 0);
  pinMode(heater, OUTPUT);
 // digitalWrite(heater, 0);
  pinMode(fog, OUTPUT);
  //digitalWrite(fog, 0);
  pinMode(aquarium_led_r, OUTPUT);
 // digitalWrite(aquarium_led_r, 0);
  pinMode(aquarium_led_g, OUTPUT);
  //digitalWrite(aquarium_led_g, 0);
  pinMode(aquarium_led_b, OUTPUT);
  //digitalWrite(aquarium_led_b, 0);
  pinMode(filter_led_r, OUTPUT);
 // digitalWrite(filter_led_r, 0);
  pinMode(filter_led_g, OUTPUT);
 // digitalWrite(filter_led_g, 0);
  pinMode(filter_led_b, OUTPUT);
 // digitalWrite(filter_led_b, 0);
  //
  Serial.begin(9600);
  led_timer.every(1000/LED_CHANGE_STAGE, led_set_method);
  temp_timer.every(1000,get_temp);
  water_level.every(1000,get_water_level);
}
void get_water_level()
{
  float water_val_h=0;
  float water_val_l=0;
  water_val_h=analogRead(water_h);
  water_val_l=analogRead(water_l);
  water_val_l=water_val_l;
 if(up_motor_state==1 && (water_val_h > water_val_l))
 {
    digitalWrite(upmotor, 0);
 }
 else
 {
    digitalWrite(upmotor, up_motor_state);
 }
  Serial.println(water_val_h);
  Serial.println(water_val_l);
}
void get_temp()
{
 Temperature.requestTemperatures();
 TEMP.temperature = Temperature.getTempCByIndex(0);
 //Serial.println(TEMP.temperature);
}
void loop()
{
  // 讀取溫度
  
 temp_timer.update();
 led_timer.update();
 water_level.update();
 //delay_method();

}
void led_set_method()
{
 if(aq_led_change_slow == true)
 {
      //Serial.println(aq_led_r_old_duty);
      float aq_led_r_rate = (aq_led_r_change_rate/LED_CHANGE_STAGE)/aq_change_time;
      if(((aq_led_r_old_duty + aq_led_r_rate < aq_led_r_duty)&&(aq_led_r_rate<0)) || aq_change_time ==0 ||((aq_led_r_old_duty + aq_led_r_rate > aq_led_r_duty)&&(aq_led_r_rate>0)))
      {
        aq_led_r_old_duty=aq_led_r_duty;
      }
      else
      {
        aq_led_r_old_duty=aq_led_r_old_duty+aq_led_r_rate;
      }
      //
      float aq_led_g_rate = (aq_led_g_change_rate/LED_CHANGE_STAGE)/aq_change_time;
      if(((aq_led_g_old_duty + aq_led_g_rate < aq_led_g_duty)&&(aq_led_g_rate<0)) || aq_change_time ==0 ||((aq_led_g_old_duty + aq_led_g_rate > aq_led_g_duty)&&(aq_led_g_rate>0)))
      {
        aq_led_g_old_duty=aq_led_g_duty;
      }
      else
      {
        aq_led_g_old_duty=aq_led_g_old_duty+aq_led_g_rate;
      }
      //
      float aq_led_b_rate = (aq_led_b_change_rate/LED_CHANGE_STAGE)/aq_change_time;
      if(((aq_led_b_old_duty + aq_led_b_rate < aq_led_b_duty)&&(aq_led_b_rate<0)) || aq_change_time ==0 ||((aq_led_b_old_duty + aq_led_b_rate > aq_led_b_duty)&&(aq_led_b_rate>0)))
      {
        aq_led_b_old_duty=aq_led_b_duty;
      }
      else
      {
        aq_led_b_old_duty=aq_led_b_old_duty+aq_led_b_rate;
      }
      
            
      analogWrite(aquarium_led_r , aq_led_r_old_duty);
      analogWrite(aquarium_led_g , aq_led_g_old_duty);
      analogWrite(aquarium_led_b , aq_led_b_old_duty);
 }
 if(filter_led_change_slow == true)
 {
      
      float fl_led_r_rate = (filter_led_r_change_rate/LED_CHANGE_STAGE)/filter_change_time;
      if(((filter_led_r_old_duty + fl_led_r_rate < filter_led_r_duty)&&(fl_led_r_rate<0)) || filter_change_time ==0 ||((filter_led_r_old_duty + fl_led_r_rate > filter_led_r_duty)&&(fl_led_r_rate>0)))
      {
        filter_led_r_old_duty=filter_led_r_duty;
      }
      else
      {
        filter_led_r_old_duty=filter_led_r_old_duty+fl_led_r_rate;
      }
      //
      float fl_led_g_rate = (filter_led_g_change_rate/LED_CHANGE_STAGE)/filter_change_time;
      if(((filter_led_g_old_duty + fl_led_g_rate < filter_led_g_duty)&&(fl_led_g_rate<0)) || filter_change_time ==0 ||((filter_led_g_old_duty + fl_led_g_rate > filter_led_g_duty)&&(fl_led_g_rate>0)))
      {
        filter_led_g_old_duty=filter_led_g_duty;
      }
      else
      {
        filter_led_g_old_duty=filter_led_g_old_duty+fl_led_g_rate;
      }
      //
      float fl_led_b_rate = (filter_led_b_change_rate/LED_CHANGE_STAGE)/filter_change_time;
      if(((filter_led_b_old_duty + fl_led_b_rate < filter_led_b_duty)&&(fl_led_b_rate<0)) || filter_change_time ==0 ||((filter_led_b_old_duty + fl_led_b_rate > filter_led_b_duty)&&(fl_led_b_rate>0)))
      {
        filter_led_b_old_duty=filter_led_b_duty;
      }
      else
      {
        filter_led_b_old_duty=filter_led_b_old_duty+fl_led_b_rate;
      }
     //Serial.println(filter_led_r_old_duty);
            
      analogWrite(filter_led_r , filter_led_r_old_duty);
      analogWrite(filter_led_g , filter_led_g_old_duty);
      analogWrite(filter_led_b , filter_led_b_old_duty);
      
 }

}
void clear_buffer()
{
  byte temp;
  while(Wire.available())
  {
    temp=Wire.read();
  }
}
void receiveEvent(int howMany)
{
  //讀取master傳過來的指令
  requestRegister = Wire.read();
  //判斷指令並做對應的活動
  if(return_state != SET_SUCCESS && return_state != (GET_SUCCESS + TEMP_QU))
  {
    switch(requestRegister)
    {
      case CMD_GET:
        ID = Wire.read();
        switch(ID)
        {
          case TEMPERATURE:
            Quantity = Wire.read();
            switch(Quantity)
            {
              case TEMP_QU:
                cmd_end = Wire.read();
                switch(cmd_end)
                {
                  case CMD_END:
                    return_state = GET_SUCCESS + TEMP_QU;
                    break;
                  default:
                    return_state = END_ERROR;
                    clear_buffer();
                    break;
                }
                break;
             default:
                return_state = QU_ERROR;
                clear_buffer();
                break;
           }
           break;
        }
        break;
      case CMD_SET:
        ID = Wire.read();
        switch(ID)
        {
          case FILTER_LED_CHANGE_SLOW:
            Quantity = Wire.read();
            switch(Quantity)
            {
              case SET:
                filter_led_r_old_duty = Wire.read();
                filter_led_g_old_duty = Wire.read();
                filter_led_b_old_duty = Wire.read();
                cmd_end = Wire.read();
                switch(cmd_end)
                {
                  case CMD_END:
                    filter_led_change_slow = true;
                    return_state = SET_SUCCESS;
                    break;
                  default:
                    return_state = DATA_ERROR;
                    clear_buffer();
                    break;
                }
            }
            break;
          case AQ_LED_CHANGE_SLOW:
            Quantity = Wire.read();
            switch(Quantity)
            {
              case SET:
                aq_led_r_old_duty = Wire.read();
                aq_led_g_old_duty = Wire.read();
                aq_led_b_old_duty = Wire.read();
                cmd_end = Wire.read();
                switch(cmd_end)
                {
                  case CMD_END:
                    aq_led_change_slow = true;
                    return_state = SET_SUCCESS;
                    break;
                  default:
                    return_state = DATA_ERROR;
                    clear_buffer();
                    break;
                }
                break;
              default:
                return_state = QU_ERROR;
                clear_buffer();
                break;
            }
            break;
          case FOG:
            Quantity = Wire.read();
            switch(Quantity)
            {
              case SET:
                IO_state = Wire.read();
                cmd_end = Wire.read();
                switch(cmd_end)
                {
                  case CMD_END:
                    return_state = SET_SUCCESS;
                    digitalWrite(fog, IO_state);
                    break;
                  default:
                    return_state = DATA_ERROR;
                    clear_buffer();
                    break;
                }
                break;
              default:
                return_state = QU_ERROR;
                clear_buffer();
                break;
            }
            break;
          case HEATER:
            Quantity = Wire.read();
            switch(Quantity)
            {
              case SET:
                IO_state = Wire.read();
                cmd_end = Wire.read();
                switch(cmd_end)
                {
                  case CMD_END:
                    return_state = SET_SUCCESS;
                    digitalWrite(heater, IO_state);
                    break;
                  default:
                    return_state = DATA_ERROR;
                    clear_buffer();
                    break;
                }
                break;
              default:
                return_state = QU_ERROR;
                clear_buffer();
                break;
            }
            break;
          case UPMOTOR:
            Quantity = Wire.read();
            switch(Quantity)
            {
              case SET:
                IO_state = Wire.read();
                cmd_end = Wire.read();
                switch(cmd_end)
                {
                  case CMD_END:
                    return_state = SET_SUCCESS;
                    up_motor_state = IO_state;
                    digitalWrite(upmotor, IO_state);
                    break;
                  default:
                    return_state = DATA_ERROR;
                    clear_buffer();
                    break;
                }
                break;
              default:
                return_state = QU_ERROR;
                clear_buffer();
                break;
            }
            break;
          case DOWNMOTOR:
            Quantity = Wire.read();
            switch(Quantity)
            {
              case SET:
                IO_state = Wire.read();
                cmd_end = Wire.read();
                switch(cmd_end)
                {
                  case CMD_END:
                    return_state = SET_SUCCESS;
                    digitalWrite(downmotor, IO_state);
                    break;
                  default:
                    return_state = DATA_ERROR;
                    clear_buffer();
                    break;
                }
                break;
              default:
                return_state = QU_ERROR;
                clear_buffer();
                break;
            }
            break;
          case FILTER_LED_RGB:
            Quantity = Wire.read();
            switch(Quantity)
            {
              case SET:
                filter_change_time = Wire.read();
                filter_led_r_old_duty = filter_led_r_duty;
                filter_led_r_duty = Wire.read();
                filter_led_g_old_duty = filter_led_g_duty;
                filter_led_g_duty = Wire.read();
                filter_led_b_old_duty = filter_led_b_duty;
                filter_led_b_duty = Wire.read();
                cmd_end = Wire.read();
                switch(cmd_end)
                {
                  case CMD_END:
                    if(filter_change_time == 0)
                    {
                      analogWrite(filter_led_r , filter_led_r_duty);
                      analogWrite(filter_led_g , filter_led_g_duty);
                      analogWrite(filter_led_b , filter_led_b_duty);
                      filter_led_change_slow = false;
                    }
                    else
                    { 
                      filter_led_r_change_rate = filter_led_r_duty-filter_led_r_old_duty;
                      filter_led_g_change_rate = filter_led_g_duty-filter_led_g_old_duty;
                      filter_led_b_change_rate = filter_led_b_duty-filter_led_b_old_duty;
                      filter_led_change_slow = true;
                    }
                    return_state = SET_SUCCESS;
                    break;
                  default:
                    return_state = DATA_ERROR;
                    clear_buffer();
                    break;
                }
            }
            break;
          case AQ_LED_RGB:
            Quantity = Wire.read();
            switch(Quantity)
            {
              case SET:
                aq_change_time = Wire.read();
                aq_led_r_old_duty = aq_led_r_duty;
                aq_led_r_duty = Wire.read();
                aq_led_g_old_duty = aq_led_g_duty;
                aq_led_g_duty = Wire.read();
                aq_led_b_old_duty = aq_led_b_duty;
                aq_led_b_duty = Wire.read();
                cmd_end = Wire.read();
                switch(cmd_end)
                {
                  case CMD_END:
                    if(aq_change_time == 0)
                    {
                      analogWrite(aquarium_led_r , aq_led_r_duty);
                      analogWrite(aquarium_led_g , aq_led_g_duty);
                      analogWrite(aquarium_led_b , aq_led_b_duty);
                      aq_led_change_slow = false;
                    }
                    else
                    { 
                      aq_led_r_change_rate = aq_led_r_duty-aq_led_r_old_duty;
                      aq_led_g_change_rate = aq_led_g_duty-aq_led_g_old_duty;
                      aq_led_b_change_rate = aq_led_b_duty-aq_led_b_old_duty;
                      aq_led_change_slow = true;
                    }
                    return_state = SET_SUCCESS;
                    break;
                  default:
                    return_state = DATA_ERROR;
                    clear_buffer();
                    break;
                }
                break;
              default:
                return_state = QU_ERROR;
                clear_buffer();
                break;
            }
            break;
          default:
            return_state = ID_ERROR;
            clear_buffer();
            break;
        }
        break;
      default:
        return_state = CMD_ERROR;
        clear_buffer();
        break;
    }
  }
  else
  {
    switch(ID)
    {
      case TEMPERATURE:
        switch(requestRegister)
        {
          case CMD_END:
            if(temp_count < 4)
            {
              return_state = TEMP.byte_temp[temp_count];
              temp_count = temp_count+1;
            }
            else
            {
              return_state = CMD_END;
              temp_count = temp_count+1;
            }
            break;
          default:
            return_state = CMD_ERROR;
            temp_count = 0;
            ID = ID_ERROR;
            clear_buffer();
            break;
        }
        break;
      default:
        switch(requestRegister)
        {
          case CMD_END:
            return_state = CMD_END;
            break;
          default:
            return_state = CMD_ERROR;
            clear_buffer();
            break;
        }
        break;
    }
  }
}
void requestEvent()
{
  //byte return_value = 0;
  Wire.write(return_state);
  switch(ID)
  {
    case TEMPERATURE:
      if(temp_count <= 4)
      {
        return_state = GET_SUCCESS + TEMP_QU;
      }  
      else
      {
        temp_count = 0;
        return_state = CMD_END;
      }
      break;
    default:
      break;
  }
  
  //選擇回傳的值
 
}

