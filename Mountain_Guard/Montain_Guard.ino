#include "Akeru.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include "ADXL345.h"

int note[] = {262,294, 330, 349};

const int redLEDPin = 2;
const int greenLEDPin = 6;
const int blueLEDPin = 3;

const int sortieBippeur = 7;
const int BouttonSecoursPin = 8;
const int BouttonAnnulationPin = 9;

int bouttonSecours = LOW;
int bouttonAnnulation = LOW;

const int tempsDeadlineMax = 25;

ADXL345 Accel;

void setup() {
  Serial.begin(9600);
  delay(1);
  Wire.begin();
  delay(1);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  pinMode(sortieBippeur, OUTPUT);
  pinMode(BouttonSecoursPin, INPUT);
  pinMode(BouttonAnnulationPin, INPUT);
  Serial.println("Starting...");
  Accel.set_bw(ADXL345_BW_12);
  delay(1000);
  Akeru.begin();
  delay(1000);
}

typedef struct {
  double lat;
  double lon;
  double alt;
} GPS;

void loop() {
  int i = 0;
  double acc_data[3];
  Accel.get_Gxyz(acc_data);
  // accéléromètre
  if(Accel.status){
    double length = 0.;
    for(i = 0; i < 3; i++){
      length += (double)acc_data[i] * (double)acc_data[i];
      Serial.print(acc_data[i]);
      Serial.print(" ");
    }
    length = sqrt(length);
    Serial.print(length);
    Serial.println("");
    if ( length < 0.5 || length > 2.4) {
      getUrgency ();
    }
  }
  else{
    Serial.println("ERROR: ADXL345 data read error");
    delay(100);
  }
  
  bouttonSecours = digitalRead(BouttonSecoursPin);
  if(bouttonSecours == HIGH){
    getUrgency ();
  }
}

void read_data(double* flat, double* flon, double* age){               // Reading data response from serial xbee
  uint8_t x;
  char gpsdata[100];
  while(Serial.available()==0){}
  if(Serial.available()>0){
    char c = Serial.read(); Serial.print(c);
    if(c == '$')
      {char c1 = Serial.read(); Serial.print(c1);
       if(c1 == 'G')
         {char c2 = Serial.read(); Serial.print(c2);
          if(c2 == 'P')
            {char c3 = Serial.read(); Serial.print(c3);
             if(c3 == 'G')
               {char c4 = Serial.read(); Serial.print(c4);
                if(c4 == 'G')
                   {char c5 = Serial.read(); Serial.print(c5);
                    if(c5 == 'A')
                       {
                         for(x=0;x<65;x++)
                         { 
                           gpsdata[x]= Serial.read();
                           while (gpsdata[x] == '\r' || gpsdata[x] == '\n' || gpsdata[x] == '\0')
                           {
                             gpsdata[x+1] = '\0';
                             Serial.print(gpsdata[x]);
                             break;
                           }
                        }
                      }
                      else{
                        Serial.println("Not a GPGGA string");
                      }
                   }
               }
            }     
         }
      }
      
      *flat = atof((const char *)getValue(gpsdata, ',', 2).c_str());
      *flon = atof((const char *)getValue(gpsdata, ',', 4).c_str());
      *age = atof((const char *)getValue(gpsdata, ',', 9).c_str());
  }
  Serial.flush();             // Clear serial buffer
}

//Split array with characters ','
String getValue(String data, char separator, int index)
{
   int found = 0;
    int strIndex[] = {0, -1  };
    int maxIndex = data.length()-1;
    for(int i=0; i<=maxIndex && found<=index; i++){
      if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
   }
    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Urgency call method
void getUrgency () {
  GPS p;
  int i, j= 0;
  double flat, flon, age = 0;
  bouttonAnnulation = LOW;
  int deadline;
  digitalWrite(redLEDPin, HIGH);
  digitalWrite(greenLEDPin, HIGH);
  digitalWrite(blueLEDPin, LOW);
  tone(sortieBippeur, note[0]);
  deadline = 0;
  while(tempsDeadlineMax > deadline && bouttonAnnulation != HIGH){
   bouttonAnnulation = digitalRead(BouttonAnnulationPin);
   deadline++;
   delay(100);
 }
 noTone(sortieBippeur);
 read_data(&flat, &flon, &age);
 p.lat = conv_coords(flat);
 p.lon = conv_coords(flon);  
 p.alt = age;
 
 if(p.alt == 0 || p.lat == 0|| p.lon == 0 && bouttonAnnulation == HIGH){
    Serial.println("GPS no datas");
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(blueLEDPin, LOW);
    for(i=0;i<5;i++){
     delay(1000);
    }
    digitalWrite(redLEDPin, LOW);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(blueLEDPin, LOW);
  } else {
    Serial.println("GPS datas sent");
    Akeru.send(&p,sizeof(p));
    digitalWrite(redLEDPin, LOW);
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(blueLEDPin, LOW);
    for(j=0;j<5;j++){
     delay(1000);
    }
    digitalWrite(redLEDPin, LOW);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(blueLEDPin, LOW);
  }
}

//Converting Coordinates from GPS-style (ddmm.ssss) to Decimal Style (dd.mmssss) 
double conv_coords(double in_coords)
{
  //Initialize the location.
  double f = in_coords;
  // Get the first two digits by turning f into an integer, then doing an integer divide by 100;
  // firsttowdigits should be 77 at this point.
  int firsttwodigits = ((int)f)/100; //This assumes that f < 10000.
  double nexttwodigits = (double)(firsttwodigits*100);
  double nexttwodigits2 = f - nexttwodigits;
  double theFinalAnswer = (double)(firsttwodigits + nexttwodigits2/60.0);
  return theFinalAnswer;
}
