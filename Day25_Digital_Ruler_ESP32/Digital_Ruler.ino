#include<Adafruit_SSD1306.h>
#include<Wire.h>
#include<Adafruit_GFX.h>


#define trig_p 5
#define echo_p  18

Adafruit_SSD1306 display(128,64,&Wire,-1);

float distance;
long duration;




void setup() {
  pinMode(trig_p,OUTPUT);
  pinMode(echo_p,INPUT);
 
  OLED_INITIAL_SCREEN();
  

}

void loop() {
  digitalWrite(trig_p,LOW);
  delayMicroseconds(2);
  digitalWrite(trig_p,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_p,LOW);
  delayMicroseconds(2);

  duration = pulseIn(echo_p,HIGH);
  distance = (duration *0.034)/2;
  OLED_DISTANCE_SHOW();
  
  delay(200);
}



void OLED_INITIAL_SCREEN()
{
  Wire.begin(21,22);
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(35,20);
  display.print("Digital Ruler");
  display.setCursor(40,30);
  display.println("V 1.0");
  display.display();
  delay(2000);
  display.clearDisplay();
  
  }


void OLED_DISTANCE_SHOW()
{display.clearDisplay();
display.setTextSize(1);
 display.setCursor(40,0);
 display.print("Distance");
 display.setTextSize(2);
 display.setCursor(30,30);
 display.print(distance,2);
 display.setTextSize(1);
 display.setCursor(90,40);
 display.print("Cm");
 display.display();
 
 
  
  
  }  
