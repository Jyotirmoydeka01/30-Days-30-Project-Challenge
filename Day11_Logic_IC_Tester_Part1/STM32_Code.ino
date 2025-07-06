#include<Wire.h>
#include<Adafruit_SSD1306.h>
#include<Adafruit_GFX.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define Scroll PA0
#define Select PA1

const char* IClist[] = {
  "7400 NAND", "7402 NOR", "7404 NOT", "7408 AND",
  "7432 OR", "7486 XOR"
};

int totalIC = sizeof(IClist) / sizeof(IClist[0]);
bool Scrollpressed = false;
bool Selectpressed = false;
int currentIndex = 0;

int topindex = 0;
const int itemPerPage = 4;

bool inTestMode = false;

int ICindex = 0;






void setup() {

  pinMode(Scroll, INPUT_PULLUP);
  pinMode(Select, INPUT_PULLUP);


  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0X3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(35, 10);
  display.print("IC TESTER");
  display.setCursor(35, 30);
  display.print(" v 1.0");
  display.display();
  delay(3000);
  display.clearDisplay();







}

void loop() {


handleMenuNavigation();
}

void showmenu()
{
  display.clearDisplay();
  display.setTextSize(1);

  for (int i = 0 ; i < itemPerPage; i++)
  {  ICindex = topindex + i;
    if (ICindex > totalIC ) break;

    int y = i * 16;
    display.setCursor(0, y);
    if ( ICindex == currentIndex )
    {
      display.print(">");

    }
    else {

      display.print(" ");
    }

    display.setCursor(10, y);
    display.print(IClist[ICindex]);



  }
  display.display();

}


void showSelectedIC(const char* ICname)
  { display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("IC: ");

  display.setTextSize(1);
  display.setCursor(20,0);
  display.print(ICname);

   display.setTextSize(0.5);
   display.setCursor(0,15);
   display.print("press scroll to go");
   display.setCursor(0,25);
   display.print("back");
   display.setCursor(0,40);
   display.print("select to continue");
   display.display();
   
   
   if(digitalRead(Scroll)== LOW && !Scrollpressed)
   {delay(100);
   Scrollpressed = true;

   showmenu();
   }
   if (digitalRead(Select) == LOW && !Selectpressed)
   {delay(100);
   Selectpressed = true;
     display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("Running Test");
    display.display();
    delay(1000);
    }
    else{Selectpressed = false;
    Scrollpressed = false;}





  }



void handleMenuNavigation()
{

  if (digitalRead(Scroll) == LOW && !Scrollpressed)
  {
    delay(100); 
    currentIndex = ( currentIndex + 1) % totalIC;
    Scrollpressed = true;


    if (currentIndex >= topindex + itemPerPage)
    {
      topindex ++;
    }
    if (currentIndex == 0)
    {
      topindex = 0;
    }

  showmenu();}
  if (digitalRead(Scroll == HIGH))
  {
    Scrollpressed = false;
  }

  

  if (digitalRead(Select) == LOW && !Selectpressed)
  {
    delay(100);
    Selectpressed = true;

    //showICDiagram();
    showSelectedIC(IClist[currentIndex]);
    

//    display.clearDisplay();
//    display.setTextSize(1);
//    display.setCursor(0, 0);
//    display.print("Wiring diagram");
//    display.display();
//    delay(1000);
  }
  if (digitalRead (Select) == HIGH)
  {
    Selectpressed = false;
  }
}

//
//void showICDiagram()
//{ if (ICindex = currentIndex){
//    display.clearDisplay();
//  display.setTextSize(1);
//  display.setCursor(0,0);
//  display.print(
//  
//  
//
//
//}
//}
  
  
  
  
