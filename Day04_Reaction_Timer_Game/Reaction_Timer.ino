#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // parameters of 0.96 OLED display
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // by default reset


#define led 4 // D4 pin for led 
#define button 2 // D2 pin for button


unsigned long Starttime;//It is the start time for the game to begin
unsigned long Reactiontime;// It stores the reaction time
bool ledon = false;// Initial led state Low
bool gamestart = false;//Game started or not



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);// defining the OLED display

void setup() {
  //Serial.begin(9600);// serial monitor

  pinMode(led, OUTPUT);//led pin used as output
  pinMode(button, INPUT_PULLUP);// pushbutton used as input with internal pullup resistor


  Wire.begin();// begin the OLED

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0X3C)) {
    while (1);

  }
  display.clearDisplay();// initially clear display
  display.setTextSize(1);// define text size
  display.setTextColor(SSD1306_WHITE); //text color
  display.setCursor(0, 10);//text position
  display.println("Click to start");//printing
  display.display();//displaying the lines
  //Serial.println("Click to start");
}
void loop() {
  if (!gamestart && digitalRead(button) == LOW)//initially when button 1st pressed 
  {
    delay(100);//debounce

    if (digitalRead(button) == LOW) {
      gamestart = true;//gamestart
      display.clearDisplay();
      display.setCursor(0, 20);
      display.println("Get Ready");
      display.display();
      //Serial.println("Get ready");
      delay(random(2000, 5000));
      digitalWrite(led, HIGH);
      ledon = true;
      Starttime = millis();// read time of starting


    }

  }
  if (gamestart && ledon && digitalRead (button) == LOW)//when led is on and button is pressed
  {
    delay(100);//debounce
    if ( digitalRead(button) == LOW) {

      Reactiontime = millis() - Starttime;// calculating reactiontime
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("Reaction time is ");
      display.setCursor(0, 20);
      display.println(Reactiontime);
      display.setCursor(30,20);
      display.print("ms");
      display.display();

      digitalWrite(led,LOW);
      //Serial.println("reaction time");
      //Serial.println(Reactiontime);

      ledon = false ;//restarting everything
      gamestart = false;

      delay (2000);

      display.setCursor(0, 30);
      display.println("Press button to restart");
      display.display();
      //Serial.println("Press button to restart");
      


    }


  }

}
