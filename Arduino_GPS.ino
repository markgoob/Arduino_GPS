#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "TM1637.h"
#define CLK 12  
#define DIO 14
TM1637 tm1637(CLK, DIO);

static const int TXPin = 33, RXPin = 32;

TinyGPSPlus gps;
SoftwareSerial ss(TXPin, RXPin);

 
void setup() {
 Serial.begin(9600);
  // 光るさ、基本は2、最大は7
  tm1637.set(3);
  
 while (!Serial) {
 ; // wait for serial port to connect. Needed for native USB port only
 }
 
 Serial.println("Connect Success");
 ss.begin(9600);
 ss.println("Connect Success");
}

// ":"の点滅
bool ShowPoint = true;

void loop() { // run over and over
  
  int time_hour , time_minute , time_second;  
     while (ss.available() > 0){
   char c = ss.read();
   double gps_location_lat = gps.location.lat();
   double gps_location_lng = gps.location.lng();
   int gps_time_hour = gps.time.hour();
   int gps_time_minute = gps.time.minute();
   int gps_time_second = gps.time.second();
   int gps_date_year = gps.date.year();
   int gps_date_month = gps.date.month();
   int gps_date_day = gps.date.day();
   int UTC_time = +9;
   int gps_hdop = gps.hdop.value();
   int gps_time_UTC_hour;
   
   gps.encode(c);
   gps_time_UTC_hour = gps_time_hour + UTC_time;
   
   if (gps.location.isUpdated()){
        Serial.print("緯度(度):");
        Serial.println(gps_location_lat, 6); // Latitude in degrees (double)
        Serial.print("経度(度):");
        Serial.println(gps_location_lng, 6); // Longitude in degrees (double)
        
        Serial.print("日付:");
        Serial.print(gps_date_year); // Year (2000+) (u16)
        Serial.print("年");
        Serial.print(gps_date_month); // Month (1-12) (u8)
        Serial.print("月");
        Serial.print(gps_date_day); // Day (1-31) (u8)
        Serial.println("日");

        /*******時間判断(時)*******/
       
         Serial.print("時間:");
            Serial.print((gps_time_UTC_hour));
        Serial.print(":");
        
         /*******時間判断(分)*******/
         
       if(gps_time_minute<10){
        Serial.print("0");
        Serial.print(gps_time_minute);
        }else{
          Serial.print(gps_time_minute); // Minute (0-59) (u8)
          }
        Serial.print(":");

         /*******時間判断(秒)*******/
        if(gps_time_second <10){
          Serial.print("0");
          Serial.println(gps_time_second);
          }else{
        Serial.println(gps_time_second); // Second (0-59) (u8)
          }

         /*******速度判断*******/
        Serial.print("速度(単位：km/h):");
        Serial.println(gps.speed.kmph()); // Speed in kilometers per hour (double)

       /*******使用中の衛星の数*******/
        Serial.print("使用中の衛星の数:");
        Serial.println(gps.satellites.value()); // Number of satellites in use (u32)
        
        /*******精度判断*******/
        Serial.print("精度:");   
        if(gps_hdop <= 150){
          Serial.println("良い");
          }else if(gps_hdop >150 && gps_hdop <= 300){
            Serial.println("中");
            }else if(gps_hdop >300){
              Serial.println("悪い");
              }
         Serial.println("-------------------------");
  int b0 = gps_time_UTC_hour / 10;
  int b1 = gps_time_UTC_hour % 10;
 
  int b2 = gps_time_minute / 10;
  int b3 = gps_time_minute % 10;
  
  tm1637.clearDisplay();
  tm1637.point(ShowPoint);
  tm1637.display(0, b0);
  tm1637.display(1, b1);
  tm1637.display(2, b2);
  tm1637.display(3, b3);
 
  ShowPoint = !ShowPoint;
  delay(50);
    }
 
  
  }


}
