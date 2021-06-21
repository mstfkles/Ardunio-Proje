#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
 
int sicaklikSensor=A0;     //  LM35  Data  ucu A0  pinine  bağlanacak
float sicaklikDegeri;     //  Analog  değeri  dönüştüreceğimiz  sıcaklık  değeri
float olculenDeger;        // Ölçeceğimiz analog  değer
int trigPin = D0; 
int echoPin = D1;  
int led=13;
long zaman;
long mesafe;
int kapiAcilis =0;
int x = 0;
//-----------Internet Detayları---------//
char ssid[] = "keles";   // wifi ismi
char pass[] = "Mustafa34";   // wifi parola
//-------------------------------------------//

//----------- Kanal Detayları -------------//
unsigned long Channel_ID = 1256353; // Kanal ID
const int Field_number = 1;  
const char * WriteAPIKey = "0O6YJ501XAT7T24Q"; // Yazma Apı Key
// ----------------------------------------//

WiFiClient  client;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  Serial.begin(9600);
  pinMode(led, OUTPUT);  
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin,INPUT); 
  Serial.begin(9600); 
}

void loop()
{ 
  digitalWrite(led, LOW);  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  int olcum=18;
  zaman = pulseIn(echoPin, HIGH);
  mesafe= (zaman /29.1)/2;  
  olculenDeger  = analogRead  (sicaklikSensor);   //  A0  analog  ucundan değer oku
  olculenDeger  = (olculenDeger/1024)*5000;         //  mv'a  dönüşüm işlemi
  sicaklikDegeri  = olculenDeger  /12,0;                 // mV'u  sıcaklığa dönüştü
  sicaklikDegeri  = sicaklikDegeri-10;
  Serial.println("Evinize Hoşgeldiniz");
  Serial.println("Evinizin Sıcaklık Ve Hırsız Sensörü Bilgileri Aşşağıdaki Gibidir.: ");
  Serial.print("Sıcaklık: ");
  Serial.println(olcum );
  Serial.println(mesafe);

  delay(1000);
  if(mesafe>26) 
  {
     Serial.println("Dikkat Kapınız Açıldı. Hırsız Olabilir.");
     digitalWrite(led, HIGH);    
     kapiAcilis++;      
     delay(2000); 
  }
  else 
  {        
     Serial.println("Eviniz Kontrol Altındadır.");
     digitalWrite(led, LOW);   
     delay(1000); 
  }  
  delay(500); 

  internet();
  get_value();
  upload();
}

void upload()
{
  internet();
  x = ThingSpeak.writeField(Channel_ID, Field_number, kapiAcilis, WriteAPIKey);
  if (x == 200)Serial.println("Data Updated.");
  if (x != 200)
  {
    Serial.println("Data upload failed, retrying....");
    delay(15000);
    upload();
  }
}

void internet()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
}

void get_value()
{
 Serial.println("---------------------");
 Serial.println("Hırsızlara Dikkat Edin.");
 Serial.print("Evinize Yapılan Toplam Giriş : ");
 Serial.println(kapiAcilis);
 delay(15000);
}
